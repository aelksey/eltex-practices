CC:=gcc
SRC:=main.c
CC_FLAGS:=-o
EXEC:=app
RUNARGS1:=5000 127.0.0.1 5001
RUNARGS2:=5001 127.0.0.1 5000

all: clean build

build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

example1:
	./$(EXEC) $(RUNARGS1)

example2:
	./$(EXEC) $(RUNARGS2)

clean:
	rm -f $(EXEC)