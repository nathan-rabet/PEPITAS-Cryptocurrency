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
	${CC} ${CFLAGS} $^ ${PCFLAGS} -o bin/test ${LDPARAMS}

server: src/server.c ${SRC}
	${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o bin/server ${LDPARAMS} -D TEST

client: src/client.c ${SRC}
	${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o bin/client ${LDPARAMS}

ui: src/gui.c ${SRC}
	${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o bin/ui ${LDPARAMS}
	cp src/ui/pepitas.glade ./pepitas.glade

test: .test_build
	@(cd bin ; ./test)
	rm -rf ./bin/~test
	rm -rf ./bin/.keys
	rm -rf ./bin/.general
	@cd ..

.test_build: $(SRC_TEST) ${SRC}
	@mkdir -p bin
	@${CC} ${CFLAGS} $^ ${PCFLAGS} -o bin/test ${LDPARAMS} -D TEST

.PHONY: clean test .test_build

clean:
	${RM} -r bin