CC:= gcc
CC_FLAGS:= -o
SRC:= contact.c book.c main.c
EXEC:= app
SRC_TESTS:= test.c
EXEC_TESTS:= test

# app: gcc contact.c book.c main.c -o app

# tests: test.c -o test 

all: app run

app:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

tests:
	$(CC) $(SRC_TESTS) $(CC_FLAGS) $(EXEC_TESTS)

clean:
	rm -f $(EXEC) $(EXEC_TESTS)

run:
	./$(EXEC)

run_tests:
	./$(EXEC_TESTS)