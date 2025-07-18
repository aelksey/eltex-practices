CC:=gcc
CC_FLAGS:=-o
SRC:=math.c main.c
EXEC:= calc

all: run


app:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run: app
	./$(EXEC)

clean:
	rm -f $(EXEC)