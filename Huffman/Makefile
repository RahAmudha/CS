CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror -g -gdwarf-4
LFLAGS = -lm
SRC = $(wildcard *.c)
BITWRTROBJ = bitwriter.o bwtest.o
NODEOBJ = node.o nodetest.o
PQOBJ = node.o pq.o pqtest.o
HUFFOBJ = huff.o pq.o node.o bitwriter.o
LIBS1 = io-$(shell uname -m).a
EXECBIN = bwtest nodetest pqtest huff

.PHONY = all clean debug

all: bwtest nodetest pqtest huff

clean:
	rm -f $(EXECBIN) $(BITWRTROBJ) $(NODEOBJ) $(PQOBJ) $(HUFFOBJ)

format:
	clang-format -i --style=file *.[ch]

debug: CFLAGS += -g
debug: clean all

bwtest: $(BITWRTROBJ) $(LIBS1)
	$(CC) $(LFLAGS) -o $@ $^

nodetest: $(NODEOBJ)
	$(CC) $(LFLAGS) -o nodetest $(NODEOBJ)

pqtest: $(PQOBJ)
	$(CC) $(LFLAGS) -o pqtest $(PQOBJ)

huff: $(HUFFOBJ) $(LIBS1)
	$(CC) $(LFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

scan-build: clean
	scan-build --use-cc=$(CC) make

