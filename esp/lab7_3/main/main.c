/* HTTP GET Example using plain POSIX sockets

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "driver/i2c_master.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "sdkconfig.h"

#define I2C_MASTER_SCL_IO           8       /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           10       /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0                   /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          400000 /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define SHTC3_ADDR                  0x70
#define WAKEUP_CMD                  0x3517 
#define EEPY                        0xB098
#define SHTC3_PERIOD                50
#define CRC_POLYNOMIAL              0x131

/* Constants that aren't configurable in menuconfig */
#define MOBILE_SERVER "169.233.213.67"
#define MOBILE_PORT "7482"
#define MOBILE_PATH "/"

#define WEB_SERVER "wttr.in"
#define WEB_PORT "80"
#define WEB_PATH "/%s?format=%%t"

typedef enum{
    NO_ERROR       = 0x00, // no error
    ACK_ERROR      = 0x01, // no acknowledgment error
    CHECKSUM_ERROR = 0x02 // checksum mismatch error
  }etError;

static const char *TAG = "Lab7";

static const char *GET_REQUEST = "GET %s HTTP/1.0\r\n"
    "Host: "WEB_SERVER":"WEB_PORT"\r\n"
    "User-Agent: esp-idf/1.0 esp32 curl\r\n"
    "\r\n";

static const char *POST = 
    "POST " MOBILE_PATH " HTTP/1.0\r\n"
    "Host: "MOBILE_SERVER":"MOBILE_PORT"\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: %d\r\n"
    "\r\n"
    "%s";

static const char *GEO_GET_REQUEST = 
    "GET " MOBILE_PATH " HTTP/1.0\r\n"
    "Host: "MOBILE_SERVER":"MOBILE_PORT"\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
    "\r\n";


static const char *OUTPUT_TEMPLATE = 
    "Temperature: %d°C (%d°F), Humidity: %d%%, Outisde Temperature: %s, Location: %s";

void temperature_conv(uint16_t *temperature){
    *temperature = -45 + 175 * ((float)(*temperature)/(1<<16));
}

void humidity_conv(uint16_t *humidity){
    *humidity = 100 * ((float)(*humidity)/(1<<16));
}

etError SHTC3_CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum){
    uint8_t bit;        // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter

    // calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr) {
        crc ^= (data[byteCtr]);
        for(bit = 8; bit > 0; --bit) {
            if(crc & 0x80) {
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            } else {
                crc = (crc << 1);
            }
        }
    }

    // verify checksum
    if(crc != checksum) {
        return CHECKSUM_ERROR;
    } else {
        return NO_ERROR;
    }
}

esp_err_t shtc3_read_data(i2c_master_dev_handle_t dev_handle, uint16_t *temperature, uint16_t *humidity){
    uint8_t data[6] = {0};
    esp_err_t err = i2c_master_receive(
        dev_handle, 
        data, 
        sizeof(data), 
        I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS
    );

    uint8_t temperature_data[2];
    uint8_t temperature_checksum;
    etError temperature_err;

    uint8_t humidity_data[2];
    uint8_t humidity_checksum;
    etError humidity_err;

    *temperature = (data[0] << 8 | data[1]);
    temperature_data[0] = data[0];
    temperature_data[1] = data[1];
    temperature_checksum = data[2];

    temperature_err = SHTC3_CheckCrc(temperature_data, 2, temperature_checksum);
    if (temperature_err){
        ESP_LOGE(TAG, "Temperature Checksum does not match!");
    }
    temperature_conv(temperature);

    *humidity = (data[3] << 8 | data[4]);
    humidity_data[0] = data[3];
    humidity_data[1] = data[4];
    humidity_checksum = data[5];
    humidity_err = SHTC3_CheckCrc(humidity_data, 2, humidity_checksum);
    if (humidity_err){
        ESP_LOGE(TAG, "Humidity Checksum does not match!");
    }
    humidity_conv(humidity);

    return err;
}

esp_err_t shtc3_send_command(i2c_master_dev_handle_t dev_handle, uint16_t cmd){
    uint8_t data[2] = {(uint8_t)((cmd & 0xff00) >> 8), (uint8_t)(cmd & 0xff)};
    return i2c_master_transmit(
        dev_handle, 
        data, 
        sizeof(data), 
        I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS
    );
}

/**
 * @brief i2c master initialization
 */
static void i2c_master_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle));

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SHTC3_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_config, dev_handle));
}

static void http_task(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *get_res, *post_res, *geo_res;
    int web_socket, get_char_count, device_socket;
    char recv_buf[128];
    char request[256];
    char formatted_request[256];

    uint16_t temperature;
    uint16_t humidity;

    int celsius;
    int fahrenheit;

    i2c_master_dev_handle_t dev_handle;
    i2c_master_bus_handle_t bus_handle;
    i2c_master_init(&bus_handle, &dev_handle);
    ESP_LOGI(TAG, "I2C initialized successfully");

    int err = getaddrinfo(MOBILE_SERVER, MOBILE_PORT, &hints, &geo_res);
    if (err != 0 || geo_res == NULL) {
        ESP_LOGE(TAG, "DNS lookup failed");
        return;
    }

    int geo_socket = socket(geo_res->ai_family, geo_res->ai_socktype, 0);
    if (geo_socket < 0) {
        ESP_LOGE(TAG, "Socket creation failed");
        freeaddrinfo(geo_res);
        return;
    }

    if (connect(geo_socket, geo_res->ai_addr, geo_res->ai_addrlen) != 0) {
        ESP_LOGE(TAG, "Socket connect failed");
        close(geo_socket);
        freeaddrinfo(geo_res);
        return;
    }

    write(geo_socket, GEO_GET_REQUEST, strlen(GEO_GET_REQUEST));

    char geo_buf[512];
    int len;
    while ((len = read(geo_socket, geo_buf, sizeof(geo_buf) - 1)) > 0) {
        geo_buf[len] = 0;
    }

    char *geo_body = strstr(geo_buf, "\r\n\r\n");
    geo_body += 4;

    for (int i = 0; geo_body[i] != '\0'; i++) {
        if (geo_body[i] == ' ') {
            geo_body[i] = '_';
        }
    }

    close(geo_socket);
    freeaddrinfo(geo_res);

    char web_path[64];
    snprintf(web_path, sizeof(web_path), WEB_PATH, geo_body);

    char web_get_request[256];
    snprintf(web_get_request, sizeof(web_get_request), GET_REQUEST, web_path);

    while(1) {
        shtc3_send_command(dev_handle, WAKEUP_CMD);
        vTaskDelay(SHTC3_PERIOD / portTICK_PERIOD_MS);
        shtc3_send_command(dev_handle, 0x7CA2);
        vTaskDelay(SHTC3_PERIOD / portTICK_PERIOD_MS);

        shtc3_read_data(dev_handle, &temperature, &humidity);
        vTaskDelay(SHTC3_PERIOD / portTICK_PERIOD_MS);

        shtc3_send_command(dev_handle, EEPY);

        celsius = (int) temperature;
        fahrenheit = celsius * (9/5) + 32;
        humidity = (int) humidity;

        // get addrinfo
        int get_err = getaddrinfo(WEB_SERVER, WEB_PORT, &hints, &get_res);
        if(get_err != 0 || get_res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", get_err, get_res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }
        
        // post addrinfo
        int post_err = getaddrinfo(MOBILE_SERVER, MOBILE_PORT, &hints, &post_res);
        if(post_err != 0 || post_res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", post_err, post_res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        // creating GET socket
        web_socket = socket(get_res->ai_family, get_res->ai_socktype, 0);
        if(web_socket < 0) {
            ESP_LOGE(TAG, "... Failed to allocate GET socket.");
            freeaddrinfo(get_res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }
        //ESP_LOGI(TAG, "... allocated GET socket");
        
        // creating POST socket
        device_socket = socket(post_res->ai_family, post_res->ai_socktype, 0);
        if(device_socket < 0) {
            ESP_LOGE(TAG, "... Failed to allocate POST socket.");
            freeaddrinfo(post_res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }
        //ESP_LOGI(TAG, "... allocated POST socket");
        
        // connect the GET socket
        if(connect(web_socket, get_res->ai_addr, get_res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... GET socket connect failed errno=%d", errno);
            close(web_socket);
            freeaddrinfo(get_res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        //ESP_LOGI(TAG, "... GET connected");
        freeaddrinfo(get_res);

        // connecting the POST socket
        if(connect(device_socket, post_res->ai_addr, post_res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... POST socket connect failed errno=%d", errno);
            close(device_socket);
            freeaddrinfo(post_res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        //ESP_LOGI(TAG, "... POST connected");
        freeaddrinfo(post_res);

        // send GET request
        if (write(web_socket, web_get_request, strlen(web_get_request)) < 0) {
            ESP_LOGE(TAG, "... GET socket send failed");
            close(web_socket);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        //ESP_LOGI(TAG, "... GET socket send success");

        struct timeval get_receiving_timeout;
        get_receiving_timeout.tv_sec = 5;
        get_receiving_timeout.tv_usec = 0;
        if (setsockopt(web_socket, SOL_SOCKET, SO_RCVTIMEO, &get_receiving_timeout,
                sizeof(get_receiving_timeout)) < 0) {
            ESP_LOGE(TAG, "... failed to set GET socket receiving timeout");
            close(web_socket);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        //ESP_LOGI(TAG, "... set GET socket receiving timeout success");

        /* Read HTTP response */
        int request_len = 0;
        int formatted_len = 0;
        char parsed_request[128];
        do {
            bzero(recv_buf, sizeof(recv_buf));
            get_char_count = read(web_socket, recv_buf, sizeof(recv_buf)-1);
            if (get_char_count){
                memcpy(parsed_request, recv_buf, 128);
            }
        } while(get_char_count > 0);

        for (int i = 0; geo_body[i] != '\0'; i++) {
            if (geo_body[i] == '_') {
                geo_body[i] = ' ';
            }
        }

        char *parsed_body = strstr(parsed_request, "\r\n\r\n");
        parsed_body += 4;

        formatted_len = snprintf(formatted_request, sizeof(formatted_request), OUTPUT_TEMPLATE, celsius, fahrenheit, humidity, parsed_body, geo_body);
        ESP_LOGI(TAG, "%s", formatted_request);
        request_len = snprintf(request, sizeof(request), POST, formatted_len, formatted_request);
        
        // send POST request
        if (write(device_socket, request, request_len) < 0) {
            ESP_LOGE(TAG, "... POST socket send failed");
            close(device_socket);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        //ESP_LOGI(TAG, "... POST socket send success");

        //ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d.", get_char_count, errno);
        close(web_socket);
        close(device_socket);
        //ESP_LOGI(TAG, "Starting again!");
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    xTaskCreate(&http_task, "http_task", 4096, NULL, 5, NULL);
}
