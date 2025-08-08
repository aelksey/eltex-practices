CC:=gcc
SRC1:=client.c
SRC2:=server.c
CC_FLAGS:=-o
EXEC1:=client
EXEC2:=server
RUNARGS1:=localhost 5000
RUNARGS2:=5000

all: clean build

build:
	$(CC) $(SRC1) $(CC_FLAGS) $(EXEC1)
	$(CC) $(SRC2) $(CC_FLAGS) $(EXEC2)

run_client: build
	./$(EXEC1) $(RUNARGS1)

run_server: build
	./$(EXEC2) $(RUNARGS2)

clean:
	rm -f $(EXEC1) $(EXEC2)