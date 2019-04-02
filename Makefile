CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror

all: traceroute.o utils.o receive.o send.o
	$(CC) $(CFLAGS) traceroute.o utils.o receive.o send.o -o traceroute

traceroute.o: traceroute.c receive.h utils.h send.h
	$(CC) $(CFLAGS) traceroute.c -c -o traceroute.o

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c -c -o utils.o

send.o: send.c send.h utils.h
	$(CC) $(CFLAGS) send.c -c -o send.o 

receive.o: receive.c receive.h utils.h
	$(CC) $(CFLAGS) receive.c -c -o receive.o

clean:
	rm -f *.o

distclean:
	rm -f *.o traceroute

