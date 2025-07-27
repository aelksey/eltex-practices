CC:= gcc
CC_FLAGS:= -g -o
SRC:= contact.c tree.c display.c prompt.c main.c
EXEC:= bookappv3

# Test section

TEST_EXEC_FLAGS:= -lcunit -o
TEST_EXEC:= bookappv3test

SRC1:= contact.c
SRC2:= tree.c
SRC3:= test.c
SRC4:= display.c

OBJ1:= contact.o
OBJ2:= tree.o
OBJ3:= test.o
OBJ4:= display.o

all: build run

build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC)

test: build_test run_test

build_test:
	$(CC) -c $(SRC1) -o $(OBJ1)
	$(CC) -c $(SRC2) -o $(OBJ2)
	$(CC) -c $(SRC3) -o $(OBJ3)
	$(CC) -c $(SRC4) -o $(OBJ4)
	$(CC) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(TEST_EXEC_FLAGS) $(TEST_EXEC) 

run_test:
	./$(TEST_EXEC)

clean:
	rm -f $(EXEC) $(TEST_EXEC) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4)