CC = gcc
UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
    CFLAGS = -I include -I /opt/homebrew/include
    LIBS = -lm -lmicrohttpd -L /opt/homebrew/lib
else
    CFLAGS = -I include
    LIBS = -lm -lmicrohttpd
endif

SRC = src/main.c src/graph.c src/parser.c src/bfs.c src/dfs.c src/dijkstra.c src/kosaraju.c src/server.c
OUT = main

deps:
	@if [ "$(UNAME)" = "Darwin" ]; then \
		brew install libmicrohttpd; \
	else \
		sudo apt install -y libmicrohttpd-dev; \
	fi

all:
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)