CC:=gcc
CC_FLAGS:= -lrt -o 

SRC:=main.c
EXEC:=chat

all: build run

build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC)

clean:
	rm -f $(EXEC)

# example :
# TERM1 : ./chat chat1 chat2
# TERM2 : ./chat chat2 chat1
