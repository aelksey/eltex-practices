CC:=gcc
CC_FLAGS:= -g -o
SRC:= main.c
EXEC:= siapp

# Progs section

PROGS:= gluestr minmax sumargs

PROG1_EXEC:= gluestr
PROG2_EXEC:= minmax
PROG3_EXEC:= sumargs

PROG1_SRC:= gluestr.c
PROG2_SRC:= minmax.c
PROG3_SRC:= sumargs.c

all: build_progs copy_progs build run
	
build:
	$(CC) $(SRC) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC)

build_progs:
	$(CC) progs/$(PROG1_SRC) $(CC_FLAGS) $(PROG1_EXEC)
	$(CC) progs/$(PROG2_SRC) $(CC_FLAGS) $(PROG2_EXEC)
	$(CC) progs/$(PROG3_SRC) $(CC_FLAGS) $(PROG3_EXEC)

copy_progs:
	sudo cp $(PROGS) /usr/bin

clean:
	sudo rm -f $(EXEC) $(PROGS) /usr/bin/$(PROG1_EXEC) /usr/bin/$(PROG2_EXEC) /usr/bin/$(PROG3_EXEC)