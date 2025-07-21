CC:=gcc
CC_FLAGS:=-o
SRC:=math.c main.c
EXEC:= calc

SRC1:=math.c
SRC2:=test.c
OBJ1:=math.o
OBJ2:=test.o
TEST_EXEC:=tests


all: run


app:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run: app
	./$(EXEC)

tests:
	$(CC) -c $(SRC1) -o $(OBJ1)	
	$(CC) -c $(SRC2) -o $(OBJ2)	
	$(CC) $(OBJ1) $(OBJ2) -lcunit -o $(TEST_EXEC)	

run_tests: tests
	./$(TEST_EXEC)

clean:
	rm -f $(EXEC) $(TEST_EXEC) $(OBJ1) $(OBJ2)