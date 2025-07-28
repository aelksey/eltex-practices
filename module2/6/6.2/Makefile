# Dynamic library section
OBJ_FLAGS:= -fPIC -c -o
LIB:= liblist.so
LIBNAME:= list
LIB_CC:= gcc
LIB_FLAGS:= -shared -o

# App compile section

CC:= gcc
CC_FLAGS:= -g -L. -l$(LIBNAME) -o
EXEC:= bookappv2

# Test section

TEST_EXEC_FLAGS:= -lcunit -L. -l$(LIBNAME) -o
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

all: lib copy_lib build run

build:
	$(CC) $(SRC6) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC)

test: lib copy_lib build_test run_test

lib: build_objects build_lib

build_objects:
	$(CC) $(SRC1) $(OBJ_FLAGS) $(OBJ1)
	$(CC) $(SRC2) $(OBJ_FLAGS) $(OBJ2)
	$(CC) -c $(SRC3) -o $(OBJ3)
	$(CC) $(SRC4) $(OBJ_FLAGS) $(OBJ4)
	$(CC) $(SRC5) $(OBJ_FLAGS) $(OBJ5)

build_test:
	$(CC) $(OBJ3) $(TEST_EXEC_FLAGS) $(TEST_EXEC) 

build_lib:
	$(LIB_CC) $(OBJ) $(LIB_FLAGS) $(LIB)

copy_lib:
	sudo cp $(LIB) /usr/lib

run_test:
	./$(TEST_EXEC)

clean:
	sudo rm -f $(EXEC) $(TEST_EXEC) $(OBJ3) $(OBJ) $(LIB) /usr/lib/$(LIB)