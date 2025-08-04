CC:=gcc
SRC:=main.c
CC_FLAGS:=-lpthread -o
EXEC:=app
DATA_FILE:=numbers.txt
SEM_FILE:=/file_semaphore

all: build run

build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC)

clean:
	rm -f $(EXEC) $(DATA_FILE) $(SEM_FILE)