CC:=gcc

CC_FLAGS:=-o

SRC1:=producer.c
SRC2:=consumer.c

EXEC1:=producer
EXEC2:=consumer

DATA_FILE:=shared_file.txt

all: build

build:
	$(CC) $(SRC1) $(CC_FLAGS) $(EXEC1)
	$(CC) $(SRC2) $(CC_FLAGS) $(EXEC2)

clean:
	rm -f $(EXEC1) $(EXEC2) $(DATA_FILE)

# Cleanup: To remove the semaphore after execution, run:
# ipcrm -s $(ipcs -s | grep 1234 | awk '{print $2}')

# Check Existing Semaphores
# ipcs -s

# Remove semaphore
#ipcrm -s <semaphore_id>