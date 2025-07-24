CC:= gcc
CC_FLAGS:= -g -o
SRC:= main.c
EXEC:= ipapp
RUNARGS:= 192.168.3.0 255.255.255.0 1000


all: build run

build: 
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC) $(RUNARGS)

clean:
	rm -f ipapp