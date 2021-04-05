CC = gcc
CFLAGS = -I"headers" -I"tests" `pkg-config --cflags gtk+-3.0` -Wall -Wextra -g -pthread
PCFLAGS = `pkg-config --libs gtk+-3.0`

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
SRC += tests/core/blockchain/block_test.c
SRC += src/ui/ui.c
SRC += src/misc/files.c
SRC += src/cryptosystem/hash.c
SRC += src/core/validation/validations.c

SRC_TEST =  tests/unit_testing.c 
SRC_TEST += tests/cryptosystem/rsa_test.c 
SRC_TEST += tests/cryptosystem/signature_test.c
SRC_TEST += tests/core/blockchain/block_test.c

all: test server client

main_test: ${SRC} tests/main_test.c
	${CC} ${CFLAGS} $^ ${PCFLAGS} -o test ${LDPARAMS}

server: src/server.c ${SRC}
	${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o server ${LDPARAMS} -D TEST

client: src/client.c ${SRC}
	${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o client ${LDPARAMS}

ui: src/gui.c ${SRC}
	${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o ui ${LDPARAMS}
	cp src/ui/pepitas.glade ./pepitas.glade

test: test_build
	@./test
	rm -rf ./~test
	rm -rf ./.keys
	rm -rf ./.general

test_build: $(SRC_TEST) ${SRC}
	@${CC} ${CFLAGS} $^ ${PCFLAGS} -o test ${LDPARAMS} -D TEST

.PHONY: clean test

clean:
	${RM} test server client ui pepitas.glade