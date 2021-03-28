CC = gcc
CFLAGS = -I"headers" -I"tests" -Wextra -g -pthread

LDPARAMS = -L . -lcrypto -lssl

OBJ = src/network/client.c
OBJ += src/network/server.c
OBJ += src/misc/safe.c
OBJ += src/cryptosystem/rsa.c
OBJ += src/core/blockchain/wallet.c
OBJ += src/cryptosystem/signature.c
OBJ += src/network/send_data.c
OBJ += src/network/get_data.c


OBJ_TEST = tests/unit_testing.c tests/cryptosystem/rsa_test.c tests/cryptosystem/signature_test.c

all: test server client

test: $(OBJ_TEST) ${OBJ}
	${CC} ${CFLAGS} $^ -o test ${LDPARAMS}
	./test

main_test: ${OBJ} tests/main_test.c
	${CC} ${CFLAGS} $^ -o test ${LDPARAMS}

server: src/server.c ${OBJ}
	${CC} ${CFLAGS} -Wall $^ -o server.out ${LDPARAMS}

client: src/client.c ${OBJ}
	${CC} ${CFLAGS} -Wall $^ -o client.out ${LDPARAMS}

.PHONY: clean test

clean:
	${RM} *.out *.bin test