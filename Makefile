CC = gcc
CFLAGS = -I"headers" -I"tests" -Wextra -g -pthread 

LDPARAMS = -L . -lcrypto -lssl

OBJ = src/network/client.c
OBJ += src/network/server.c
OBJ += src/misc/safe.c
OBJ += src/cryptosystem/rsa.c
OBJ += src/core/blockchain/wallet.c
OBJ += src/cryptosystem/coding.c

OBJ_TEST = tests/main_test.c tests/cryptosystem/rsa_test.c

all: test server client

test: $(OBJ_TEST) ${OBJ}
	${CC} ${CFLAGS} $^ -o test ${LDPARAMS}

server: tests/network/server_test.c ${OBJ}
	${CC} ${CFLAGS} -Wall $^ -o server.out ${LDPARAMS}

client: tests/network/client_test.c ${OBJ}
	${CC} ${CFLAGS} -Wall $^ -o client.out ${LDPARAMS}

.PHONY: clean

clean:
	${RM} *.out test