CC:=gcc
SRC:=main.c
CC_FLAGS:=-o
EXEC:=app
DATA_FILE:=data.txt

all: clean build run

build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC) 

run:
	./$(EXEC)&
	ps | grep $(EXEC)

clean:
	rm -f $(EXEC) $(DATA_FILE)