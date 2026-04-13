CC = gcc
CFLAGS = -I include -I /opt/homebrew/include
LIBS = -lm -lmicrohttpd -L /opt/homebrew/lib
SRC = src/main.c src/graph.c src/parser.c src/bfs.c src/dfs.c src/dijkstra.c src/kosaraju.c src/server.c
OUT = main

deps:
	@which brew > /dev/null && brew install libmicrohttpd || sudo apt install -y libmicrohttpd-dev

all:
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)