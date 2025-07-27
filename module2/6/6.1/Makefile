# Static library section

LIB:= list.lib
LIB_CC:= ar
LIB_FLAGS:= -r

# App compile section

CC:= gcc
CC_FLAGS:= -g -L. $(LIB) -o
EXEC:= bookappv2

# Test section

TEST_EXEC_FLAGS:= -lcunit -L. $(LIB) -o
TEST_EXEC:= bookappv2test

SRC1:= contact.c
SRC2:= list.c
SRC3:= test.c
SRC4:= display.c
SRC5:= prompt.c
SRC6:= main.c

OBJ1:= contact.o
OBJ2:= list.o
OBJ3:= test.o
OBJ4:= display.o
OBJ5:= prompt.o
OBJ:= contact.o list.o display.o prompt.o

all: lib build run

build:
	$(CC) $(SRC6) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC)

test: lib build_test run_test

lib: build_objects build_lib


build_objects:
	$(CC) -c $(SRC1) -o $(OBJ1)
	$(CC) -c $(SRC2) -o $(OBJ2)
	$(CC) -c $(SRC3) -o $(OBJ3)
	$(CC) -c $(SRC4) -o $(OBJ4)
	$(CC) -c $(SRC5) -o $(OBJ5)

build_test:
	$(CC) $(OBJ3) $(TEST_EXEC_FLAGS) $(TEST_EXEC) 

build_lib:
	$(LIB_CC) $(LIB_FLAGS) $(LIB) $(OBJ)

run_test:
	./$(TEST_EXEC)

clean:
	rm -f $(EXEC) $(TEST_EXEC) $(OBJ3) $(OBJ) $(LIB)
