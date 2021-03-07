CC = gcc
CFLAGS = -Wall -Wextra -g

OBJ = main_test.o
OBJ += ../src/network/network.o

all: main_test

main_test: ${OBJ}
	${CC} ${CFLAGS} $^ -o $@

.PHONY: clean

clean:
	${RM} main_test main_test.o malloc.o