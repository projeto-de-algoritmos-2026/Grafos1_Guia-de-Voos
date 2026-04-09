CC = gcc
CFLAGS = -I include -lm
SRC = src/main.c src/graph.c src/parser.c
OUT = main

all:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)