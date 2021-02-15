# Compile test program
CC=gcc
PRG=tm1637_test
SRC=tm1637_test.c tm1637.c

$(PRG): $(SRC)
	$(CC) -Wall -o tm1637_test tm1637_test.c tm1637.c -lpigpio -lm


