CFLAGS = -Wall -g

LIB_CODE = socket_helper.c
LIB_HEADER = socket_helper.h
DICT_CODE = dictionary.c
DICT_HEADER = dictionary.h

all: cliente servidor hangman#cliente servidor

#cliente: cliente.c $(LIB_CODE) $(LIB_HEADER)
#	gcc $(CFLAGS) cliente.c $(LIB_CODE) -o cliente
#
#servidor: servidor.c $(LIB_CODE) $(LIB_HEADER)
#	gcc $(CFLAGS) servidor.c $(LIB_CODE) -o servidor

cliente: cliente.c $(LIB_CODE) $(LIB_HEADER)
	gcc $(CFLAGS) cliente.c $(LIB_CODE) -o build/cliente

servidor: servidor.c $(LIB_CODE) $(LIB_HEADER)
	gcc $(CFLAGS) servidor.c $(LIB_CODE) -o build/servidor

hangman: hangman.c $(DICT_CODE) $(DICT_HEADER)
	gcc $(CFLAGS) hangman.c $(DICT_CODE) -o build/hangman

.PHONY: clean
clean:
	rm -rf build/*
