CC = gcc
CFLAGS = -I"headers" -I"tests" -Wall -Wextra -g -pthread 

LDPARAMS = -L . -lcrypto -lssl

OBJ = src/network/client.c
OBJ += src/network/server.c
OBJ += src/misc/safe.c
OBJ += src/cryptosystem/rsa.c
OBJ += src/core/blockchain/wallet.c
OBJ += src/cryptosystem/coding.c

OBJ_TEST = tests/main_test.c

all: main_test server client

main_test: tests/main_test.c ${OBJ}
	${CC} ${CFLAGS} $^ -o main.out ${LDPARAMS}

server: tests/network/server_test.c ${OBJ}
	${CC} ${CFLAGS} $^ -o server.out ${LDPARAMS}

client: tests/network/client_test.c ${OBJ}
	${CC} ${CFLAGS} $^ -o client.out ${LDPARAMS}

.PHONY: clean

clean:
	${RM} *.out test