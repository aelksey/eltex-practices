CC:=gcc
CC_FLAGS:= -o
SRC:= main.c
EXEC:= app
RUN_ARGS:= string 2 -2
TEST_ARGS:= --2 

all: build run


build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC) $(RUN_ARGS)

test:
	./$(EXEC) $(TEST_ARGS)

clean:
	rm -f $(EXEC)