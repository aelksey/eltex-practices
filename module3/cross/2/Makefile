CC:=gcc
CC_FLAGS:=-o
SRC:=main.c
EXEC:=app

all: clean build run

build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC)

clean:
	rm -f $(EXEC)