CXX=g++
CFLAGS=-Iinc -MMD -MP -O3 -march=native
LIB=-Llib -lraylib -lm
SRC=$(wildcard src/*.cpp)
OBJ=$(addprefix obj/,$(notdir $(SRC:.cpp=.o)))
DEP=$(OBJ:.o=.d)
EXE=main

.PHONY: all exe run clean
.DEFAULT_GOAL= all

-include $(DEP)

all: exe

exe: $(OBJ)
	$(CXX) $(OBJ) $(LIB) -o $(EXE)

run: exe
	./$(EXE)

clean:
	rm -rf obj $(EXE)

obj/%.o: src/%.cpp
	@mkdir obj -p
	$(CXX) $< -c $(CFLAGS) -o $@
