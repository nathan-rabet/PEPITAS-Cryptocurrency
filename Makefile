CC := gcc
LINK := gcc
RM := rm -rf 

OUTPUTFOLDER := build

CFLAGS := -I"headers" -I"tests/headers" -I"tests" `pkg-config --cflags gtk+-3.0` `pkg-config --cflags openssl` -Wall -Wextra -g -pthread
PCFLAGS := `pkg-config --libs gtk+-3.0` `pkg-config --libs openssl`
LDPARAMS := -lssl -lcrypto -lrt -lpthread

SRC := $(shell find src/core -type f -name *.c)
OBJ := $(SRC:.c=.o)
SRC_TEST := $(shell find tests/src -type f -name *.c)
OBJ_TEST := $(SRC_TEST:.c=.o)

_test       := test.elf
_client     := client.elf
_server     := server.elf
_genesis    := genesis.elf
_ui         := ui.elf
_doorserver := doorserver.elf

executables := $(_test) $(_client) $(_server) $(_genesis) $(_ui) $(_doorserver)

SUBFOLDERS := $(shell ls -a -I".git*" -I".github*" -I"." -I"..")

bootstrap:
	mkdir -p $(OUTPUTFOLDER)
	cp -R $(SUBFOLDERS) $(OUTPUTFOLDER)
	$(MAKE) binaries -j 64 -C $(OUTPUTFOLDER)

binaries: $(executables) pepitas.glade

$(_test): tests/unit_testing.o $(OBJ_TEST) $(OBJ)
	$(LINK) $^ -o $@ $(PCFLAGS) $(CFLAGS) $(LDPARAMS) 
$(_client): src/client.o $(OBJ)
	$(LINK) $^ -o $@ $(PCFLAGS) $(CFLAGS) $(LDPARAMS)
$(_server): src/server.o $(OBJ)
	$(LINK) $^ -o $@ $(PCFLAGS) $(CFLAGS) $(LDPARAMS)
$(_genesis): src/genesis.o $(OBJ)
	$(LINK) $^ -o $@ $(PCFLAGS) $(CFLAGS) $(LDPARAMS)
$(_ui): src/gui.o $(OBJ)
	$(LINK) $^ -o $@ $(PCFLAGS) $(CFLAGS) $(LDPARAMS)
$(_doorserver): src/serverdoor.o $(OBJ)
	$(LINK) $^ -o $@ $(PCFLAGS) $(PCFLAGS) $(LDPARAMS)

$(OUTPUTFOLDER):
	mkdir -p $(OUTPUTFOLDER)

%.o: %.c
	$(CC) $< -o $@ $(CFLAGS) $(PCFLAGS) $(LDPARAMS) -c

pepitas.glade: src/core/ui/pepitas.glade
	cp $< $@

clean:
	$(RM) tests/*.o src/*.o $(OBJ) $(OBJ_TEST) $(executables) build pepitas.glade

.PHONY: clean

# all: test server client sign ui doorserver
# 
# main_test: ${SRC} tests/main_test.c
	# @mkdir -p bin
	# ${CC} ${CFLAGS} $^ ${PCFLAGS} -o bin/$@ ${LDPARAMS}
# 
# server: src/server.c ${SRC}
	# @mkdir -p bin
	# ${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o bin/$@ ${LDPARAMS}
# 
# doorserver: src/serverdoor.c ${SRC}
	# @mkdir -p bin
	# ${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o bin/$@ ${LDPARAMS}
# 
# client: src/client.c ${SRC}
	# @mkdir -p bin
	# ${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o bin/$@ ${LDPARAMS}
	# cp src/core/ui/pepitas.glade ./pepitas.glade
# 
# ui: src/gui.c ${SRC}
	# @mkdir -p bin
	# ${CC} ${CFLAGS} -Wall $^ ${PCFLAGS} -o bin/$@ ${LDPARAMS}
	# cp src/core/ui/pepitas.glade ./pepitas.glade
# 
# 
# test: .test_build
	# @(cd bin ; ./test)
	# rm -rf ./bin/~test
	# rm -rf ./bin/.keys
	# rm -rf ./bin/blockchain
	# rm -rf ./bin/validators.state
	# @cd ..
# 
# .test_build: $(SRC_TEST) ${SRC} tests/unit_testing.c
	# @mkdir -p bin
	# @${CC} ${CFLAGS} $^ ${PCFLAGS} -o bin/test ${LDPARAMS} -D TEST
# 
# genesis: src/genesis.c ${SRC}
	# @mkdir -p bin
	# ${CC} ${CFLAGS} $^ ${PCFLAGS} -o bin/sign ${LDPARAMS}
# 
# .PHONY: clean test .test_build
# 
# clean:
	# ${RM} -r bin