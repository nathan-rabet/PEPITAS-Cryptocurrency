CC = gcc
CFLAGS = -Wall -Wextra -g -pthread

OBJ = tests/main_test.c
OBJ += src/network/client.c
OBJ += src/network/server.c

all: main_test

main_test: ${OBJ}
	${CC} ${CFLAGS} $^ -o main

.PHONY: clean

clean:
	${RM} main