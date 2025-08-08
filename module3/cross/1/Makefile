CC:=gcc
CC_FLAGS:=-luuid -std=c11 -o
SRC1:=client.c
SRC2:=server.c
EXEC1:=client
EXEC2:=server
RUNARGS:=127.0.0.1

all: build

build:
	$(CC) $(SRC1) $(CC_FLAGS) $(EXEC1)
	$(CC) $(SRC2) $(CC_FLAGS) $(EXEC2)

run_client: build
	sudo ./$(EXEC1) $(RUNARGS)

run_server: build
	sudo ./$(EXEC2)

clean:
	rm -f $(EXEC1) $(EXEC2)

# To use uuid library install uuid-dev package:
# sudo apt-get install uuid-dev