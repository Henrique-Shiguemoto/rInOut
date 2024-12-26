SRC=main.c rInOut.c rInOut.h
BIN=main.exe
COMPILER=gcc
COMPILER_WIN32=CL
COMPILER_FLAGS=-Wall -Wextra -pedantic -g -std=c11 -Wno-unused-parameter -Wno-unused-variable

# /OUT:$(BIN) doesn't seem to be necessary, it already outputs main.exe for me
COMPILER_FLAGS_WIN32=/Wall /std:c11 /Zi /wd4100 /wd5045 /wd4668 /options:strict
DEBUGGER=gdb
DEBUGGER_WIN32=raddbg
TRASH=*.o *.exe *.ilk *.obj *.pdb *.rdi *.txt *.out

all:
	$(COMPILER) $(SRC) $(COMPILER_FLAGS) -o $(BIN)

run: 
	$(BIN)

debug: clean all
	$(DEBUGGER) $(BIN)

win:
	$(COMPILER_WIN32) $(SRC) $(COMPILER_FLAGS_WIN32)

dwin: clean win
	$(DEBUGGER_WIN32) $(BIN)

# rm on Linux and del on Windows
clean:
	rm $(TRASH)