CC:=gcc
SRC:=main.c
CC_FLAGS:=-o
EXEC:=app
DUMP:=dump.bin
RUNARGS:=5000

all: build run

build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run:
	sudo ./$(EXEC) $(RUNARGS)

clean:
	rm -f $(EXEC) $(DUMP)

# First run task12
# make example1

# Example: ./app 5000