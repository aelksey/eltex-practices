CC:= gcc
CC_FLAGS:= -o
SRC:= contact.c book.c main.c
EXEC:= app

TEST_SRC_1:= contact.c
TEST_SRC_2:= book.c
TEST_SRC_3:= test.c
TEST_OBJ_1:= contact.o
TEST_OBJ_2:= book.o
TEST_OBJ_3:= test.o
TEST_CC_FLAGS:= -lcunit -o
TEST_EXEC:= test 


# Install Cunit on linux

# sudo apt install libcunit1 libcunit1-doc libcunit1-dev 
# dpkg -l | grep -i cunit


all: app run

app:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)


tests_obj:
	$(CC) -c $(TEST_SRC_1) -o $(TEST_OBJ_1)
	$(CC) -c $(TEST_SRC_2) -o $(TEST_OBJ_2)
	$(CC) -c $(TEST_SRC_3) -o $(TEST_OBJ_3)


tests: tests_obj
	$(CC) $(TEST_OBJ_1) $(TEST_OBJ_2) $(TEST_OBJ_3) $(TEST_CC_FLAGS) $(TEST_EXEC)

clean:
	rm -f $(EXEC) $(TEST_EXEC) $(TEST_OBJ_1) $(TEST_OBJ_2) $(TEST_OBJ_3)

run:
	./$(EXEC)

run_tests: tests
	./$(TEST_EXEC)