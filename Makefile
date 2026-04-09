CC = gcc
CFLAGS = -I include
SRC = src/main.c
OUT = main

all:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)