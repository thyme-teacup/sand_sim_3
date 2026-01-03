CC=gcc
CFLAGS=-Iinc -MMD -MP -O3 -march=native -std=c11 -pthread
LIB=-Llib -lraylib -lm
SRC=$(wildcard src/*.c)
OBJ=$(addprefix obj/,$(notdir $(SRC:.c=.o)))
DEP=$(OBJ:.o=.d)
EXE=main

.PHONY: all exe run clean
.DEFAULT_GOAL= all

-include $(DEP)

all: exe

exe: $(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(EXE)

run: exe
	./$(EXE)

clean:
	rm -rf obj $(EXE)

obj/%.o: src/%.c
	@mkdir obj -p
	$(CC) $< -c $(CFLAGS) -o $@
