# Compiling & cleaning configuration
CC 		:= gcc
LINK 	:= gcc
RM 		:= rm -rf 
EXT 	:= elf

# Project configuration
OUTPUTFOLDER := build
GUI_OUT := $(OUTPUTFOLDER)/data/pepitas.glade
GENESIS_OUT := $(OUTPUTFOLDER)/data/blockchain/block0

# Compilation flags
CFLAGS := -I"headers" -I"tests/headers" -I"tests" `pkg-config --cflags gtk+-3.0` `pkg-config --cflags openssl` -Wall -Wextra -Wno-deprecated-declarations -g -pthread
PCFLAGS := `pkg-config --libs gtk+-3.0` `pkg-config --libs openssl`
LDPARAMS := -lssl -lcrypto -lrt -lpthread

# Sources & objects files 
SRC 			:= $(shell find src/core -type f -name *.c)
OBJ 			:= $(SRC:.c=.o)
SRC_TEST 		:= $(shell find tests/src -type f -name *.c)
OBJ_TEST 		:= $(SRC_TEST:.c=.o)

# Makefile rules
_test       	:= $(OUTPUTFOLDER)/test.$(EXT)
_client     	:= $(OUTPUTFOLDER)/client.$(EXT)
_genesis    	:= $(OUTPUTFOLDER)/genesis.$(EXT)
_doorserver 	:= $(OUTPUTFOLDER)/doorserver.$(EXT)

# Rules objects dependencies
_test_OBJ 		:= tests/unit_testing.o $(OBJ_TEST) $(OBJ)
_client_OBJ 	:= src/client.o $(OBJ)
_genesis_OBJ 	:= src/genesis.o $(OBJ)
_doorserver_OBJ := src/serverdoor.o $(OBJ)

all: $(_test) $(_client) $(_server) $(_genesis) $(_ui) $(_doorserver)
	
# Linking
$(_test): $(_test_OBJ) $(OUTPUTFOLDER)
	$(LINK) $(_test_OBJ) -o $@ $(PCFLAGS) $(CFLAGS) $(LDPARAMS) 
$(_client): $(_client_OBJ) $(OUTPUTFOLDER) $(GUI_OUT) $(GENESIS_OUT)
	$(LINK) $(_client_OBJ) -o $@ $(PCFLAGS) $(CFLAGS) $(LDPARAMS)
$(_genesis): $(_genesis_OBJ) $(OUTPUTFOLDER)
	$(LINK) $(_genesis_OBJ) -o $@ $(PCFLAGS) $(CFLAGS) $(LDPARAMS)
$(_doorserver): $(_doorserver_OBJ) $(OUTPUTFOLDER)
	$(LINK) $(_doorserver_OBJ) -o $@ $(PCFLAGS) $(CFLAGS) $(LDPARAMS)

$(OUTPUTFOLDER):
	mkdir $@

# Including graphic dependency
$(GUI_OUT): $(shell find src/core -type f -name $(notdir $(GUI_OUT)))
	mkdir -p $(@D)
	cp $< $@

$(GENESIS_OUT): $(shell find data -type f -name $(notdir $(GENESIS_OUT)))
	mkdir -p $(@D)
	cp $< $@

# Executing
test: $(_test)
	@mkdir -p $(OUTPUTFOLDER)/test
	@cp -r $(OUTPUTFOLDER)/data $(OUTPUTFOLDER)/test/data
	@cd $(OUTPUTFOLDER)/test ; ../$@.$(EXT)
	rm -rf $(OUTPUTFOLDER)/test

# Compiling
%.o: %.c
	$(CC) $< -o $@ $(CFLAGS) $(PCFLAGS) $(LDPARAMS) -c

# General case
$(notdir %): $(OUTPUTFOLDER)/%.$(EXT)
	
# Cleaning built files
clean:
	$(RM) tests/*.o src/*.o $(OBJ) $(OBJ_TEST) $(OUTPUTFOLDER)

clear:
	$(RM) tests/*.o src/*.o $(OBJ) $(OBJ_TEST)
	cd $(OUTPUTFOLDER) && $(RM) `ls | grep -v "data"`
	cd $(OUTPUTFOLDER)/data && $(RM) `ls | grep -v "keys\|contact\|ui"`


.PHONY: clean clear test
