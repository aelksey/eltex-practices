CC:= gcc
CC_FLAGS:= -g -o
SRC:= contact.c list.c display.c prompt.c main.c
EXEC:= bookappv2

# Test section

TEST_OBJ_FLAGS:= -c -o
TEST_EXEC_FLAGS:= -lcunit -o
TEST_EXEC:= bookappv2test

SRC1:= contact.c
SRC2:= list.c
SRC3:= test.c

OBJ1:= contact.o
OBJ2:= list.o
OBJ3:= test.o

all: build run

build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC)

test: build_test run_test

build_test:
	$(CC) $(SRC1) $(TEST_OBJ_FLAGS) $(OBJ1)
	$(CC) $(SRC2) $(TEST_OBJ_FLAGS) $(OBJ2)
	$(CC) $(SRC3) $(TEST_OBJ_FLAGS) $(OBJ3)
	$(CC) $(OBJ1) $(OBJ2) $(OBJ3) $(TEST_EXEC_FLAGS) $(EXEC) 

run_test:
	./$(TEST_EXEC)


clean:
	rm -f $(EXEC) $(TEST_EXEC) $(OBJ1) $(OBJ2) $(OBJ3)