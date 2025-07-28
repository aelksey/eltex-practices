CC:=gcc
CC_FLAGS:= -g -L. -lsum -lsub -lmul -ldel -lmod -o
OBJ_FLAGS:= -fPIC -c -o
LIB_FLAGS:= -shared -o

EXEC:=calc

SRC1:= main.c
SRC2:= sum.c 
SRC3:= sub.c
SRC4:= mul.c
SRC5:= del.c
SRC6:= mod.c 

OBJ2:= sum.o 
OBJ3:= sub.o
OBJ4:= mul.o
OBJ5:= del.o
OBJ6:= mod.o

LIB2:= libsum.so
LIB3:= libsub.so
LIB4:= libmul.so
LIB5:= libdel.so
LIB6:= libmod.so

# TODO: Buil libs & build app 

all: lib build run

build:
	$(CC) $(SRC1) $(CC_FLAGS) $(EXEC)

run:
	./$(EXEC)

lib: objects build_lib copy_libs 
	
objects:
	$(CC) $(SRC2) $(OBJ_FLAGS) $(OBJ2)
	$(CC) $(SRC3) $(OBJ_FLAGS) $(OBJ3)
	$(CC) $(SRC4) $(OBJ_FLAGS) $(OBJ4)
	$(CC) $(SRC5) $(OBJ_FLAGS) $(OBJ5)
	$(CC) $(SRC6) $(OBJ_FLAGS) $(OBJ6)

build_lib:
	$(CC) $(OBJ2) $(LIB_FLAGS) $(LIB2)
	$(CC) $(OBJ3) $(LIB_FLAGS) $(LIB3)
	$(CC) $(OBJ4) $(LIB_FLAGS) $(LIB4)
	$(CC) $(OBJ5) $(LIB_FLAGS) $(LIB5)
	$(CC) $(OBJ6) $(LIB_FLAGS) $(LIB6)

copy_libs:
	sudo cp $(LIB2) $(LIB3) $(LIB4) $(LIB5) $(LIB6) /usr/lib
	
clean:
	sudo rm -rf $(EXEC) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(LIB2) $(LIB3) $(LIB4) $(LIB5) $(LIB6) /usr/lib/$(LIB2) /usr/lib/$(LIB3) /usr/lib/$(LIB4) /usr/lib/$(LIB5) /usr/lib/$(LIB6)