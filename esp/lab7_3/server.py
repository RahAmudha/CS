from http.server import HTTPServer, BaseHTTPRequestHandler
import requests

class SimpleHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        print("GOT GET REQUEST")
        try:
            location_data = requests.get("https://ipinfo.io/json").json()
            location = location_data.get("loc", "unknown")
        except Exception as e:
            location = f"Error retrieving location: {e}"

        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        message = (
            f"<html><body><h1>Device Location</h1>"
            f"<p>Coordinates: {location}</p>"
            f"<p>Path: {self.path}</p></body></html>"
        )
        self.wfile.write(message.encode())

    def do_POST(self):
        print("GOT POST REQUEST")
        content_length = int(self.headers.get('Content-Length', 0))
        post_data = self.rfile.read(content_length).decode('utf-8')

        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        response = f"POST data received:\n{post_data}"
        self.wfile.write(response.encode())

        print(post_data)

if __name__ == '__main__':
    port = 1234
    server = HTTPServer(('0.0.0.0', port), SimpleHandler)
    print(f"Serving on port {port}...")
    server.serve_forever()