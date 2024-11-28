COMPILER=clang
COMPILER_FLAGS=-Wall -Wextra -pedantic -g -std=c11 -Wno-unused-parameter -Wno-unused-variable
SRC=main.c rInOut.c
BIN=main.exe
DEBUGGER=gdb
TRASH=*.o *.exe

all:
	$(COMPILER) $(SRC) $(COMPILER_FLAGS) -o $(BIN)

run: 
	$(BIN)

debug: 
	$(DEBUGGER) $(BIN)

e: clean all run

d: clean all debug

clean:
	del $(TRASH)