CC:=gcc
CC_FLAGS:= -o
SRC:=main.c
EXEC:=chmod

all: build run
	
build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC)

clean:
	rm -f $(EXEC)