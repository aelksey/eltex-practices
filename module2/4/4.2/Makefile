CC:= gcc
EXEC:= queueapp
CC_FLAGS:= -g -o
SRC:= queue.c main.c
SRC1:= queue.c
SRC2:= test.c

# Test section

OBJ1:= queue.o
OBJ2:= test.o
OBJ_FLAGS:= -c -o
TEST_EXEC_FLAGS:= -lcunit -o
TEST_EXEC:= queueapptest

all: build run

build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC)

test: build_test run_test

build_test:
	$(CC) $(SRC1) $(OBJ_FLAGS) $(OBJ1)
	$(CC) $(SRC2) $(OBJ_FLAGS) $(OBJ2)
	$(CC) $(OBJ1) $(OBJ2) $(TEST_EXEC_FLAGS) $(TEST_EXEC)

run_test:
	./$(TEST_EXEC)

clean:
	rm -f $(EXEC) $(TEST_EXEC) $(OBJ1) $(OBJ2)