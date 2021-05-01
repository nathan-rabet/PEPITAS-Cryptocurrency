CC = gcc
CFLAGS = -I"headers" -I"tests/src" -I"tests" `pkg-config --cflags gtk+-3.0` -Wall -Wextra -g -pthread
PCFLAGS = `pkg-config --libs gtk+-3.0`

LDPARAMS = -L . -lcrypto -lssl

SRC:=$(shell find src/core -name *.c)

SRC_TEST =  tests/unit_testing.c 
SRC_TEST += tests/src/cryptosystem/rsa_test.c 
SRC_TEST += tests/src/cryptosystem/signature_test.c
SRC_TEST += tests/src/blockchain/block_test.c
SRC_TEST += tests/src/blockchain/block_test.c

all: test server client sign ui doorserver

main_test: ${SRC} tests/main_test.c
	@mkdir -p bin
	${CC} ${CFLAGS} $^ ${PCFLAGS} -o bin/main ${LDPARAMS}

server: src/server.c ${SRC}
	@mkdir -p bin
	${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o bin/server ${LDPARAMS}

doorserver: src/serverdoor.c ${SRC}
	@mkdir -p bin
	${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o bin/serverdoor ${LDPARAMS}

client: src/client.c ${SRC}
	@mkdir -p bin
	${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o bin/client ${LDPARAMS}

ui: src/gui.c ${SRC}
	@mkdir -p bin
	${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o bin/ui ${LDPARAMS}
	cp src/core/ui/pepitas.glade ./pepitas.glade

test: .test_build
	@(cd bin ; ./test)
	rm -rf ./bin/~test
	rm -rf ./bin/.keys
	rm -rf ./bin/blockchain
	rm -rf ./bin/validators.state
	@cd ..

.test_build: $(SRC_TEST) ${SRC}
	@mkdir -p bin
	@${CC} ${CFLAGS} $^ ${PCFLAGS} -o bin/test ${LDPARAMS} -D TEST

sign: src/sign.c ${SRC}
	@mkdir -p bin
	@${CC} ${CFLAGS} $^ ${PCFLAGS} -o bin/sign ${LDPARAMS}

.PHONY: clean test .test_build

clean:
	${RM} -r bin