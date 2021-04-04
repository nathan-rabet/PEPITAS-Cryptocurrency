CC = gcc
CFLAGS = -I"headers" -I"tests" -Wall -Wextra -g -pthread

LDPARAMS = -L . -lcrypto -lssl

SRC  = src/network/client.c
SRC += src/network/server.c
SRC += src/misc/safe.c
SRC += src/cryptosystem/rsa.c
SRC += src/core/blockchain/wallet.c
SRC += src/core/blockchain/block.c
SRC += src/cryptosystem/signature.c
SRC += src/network/send_data.c
SRC += src/network/get_data.c
SRC += src/network/network.c


SRC_TEST = tests/unit_testing.c tests/cryptosystem/rsa_test.c tests/cryptosystem/signature_test.c

all: test server client

test: $(SRC_TEST) ${SRC}
	${CC} ${CFLAGS} $^ -o test ${LDPARAMS}
	./test

main_test: ${SRC} tests/main_test.c
	${CC} ${CFLAGS} $^ -o test ${LDPARAMS}

server: src/server.c ${SRC}
	${CC} ${CFLAGS} -Wall $^ -o server.out ${LDPARAMS} -D TEST

client: src/client.c ${SRC}
	${CC} ${CFLAGS} -Wall $^ -o client.out ${LDPARAMS}

.PHONY: clean test

clean:
	${RM} *.out *.bin test