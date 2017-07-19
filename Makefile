
CC = gcc
CFLAGS = -Wall -g -std=gnu99

mysort: mysort.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f mysort.o mysort
