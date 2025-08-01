CC:=gcc
SRC1:= client.c
SRC2:= server.c

CC_FLAGS_1:= -lpthread -o
CC_FLAGS_2:= -o

EXEC1:= client
EXEC2:= server

all: build server

build:
	$(CC) $(SRC1) $(CC_FLAGS_1) $(EXEC1)
	$(CC) $(SRC2) $(CC_FLAGS_2) $(EXEC2)

server:
	./$(EXEC2)

client:
	./$(EXEC1)

clean:
	rm -f $(EXEC1) $(EXEC2)
