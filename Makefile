CFLAGS = -Wall -g

LIB_CODE = socket_helper.c
LIB_HEADER = socket_helper.h
DICT_CODE = dictionary.c
DICT_HEADER = dictionary.h

all: folder client server hangman

folder:
	mkdir -p build

client: client.c $(LIB_CODE) $(LIB_HEADER)
	gcc $(CFLAGS) client.c $(LIB_CODE) -o build/client

server: server.c $(LIB_CODE) $(LIB_HEADER)
	gcc $(CFLAGS) server.c $(LIB_CODE) $(DICT_CODE) -o build/server

hangman: hangman.c $(DICT_CODE) $(DICT_HEADER)
	gcc $(CFLAGS) hangman.c $(DICT_CODE) -o build/hangman

.PHONY: clean
clean:
	rm -rf build
