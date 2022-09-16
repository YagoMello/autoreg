FLAGS=-std=c++20 -pipe -Wall -Wextra -Wshadow -Wundef -Wconversion -Wdisabled-optimization -Wlogical-op -Wwrite-strings
DEBUG=-Og -ggdb3 -fsignaling-nans -D DEBUG -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined
RELEASE=-O3 -D NDEBUG -flto=11 -fuse-linker-plugin
ASM=-S -fverbose-asm

LIBS=
PROG=*.cpp

.PHONY: debug
debug:
	g++ $(FLAGS) $(LIBS) $(DEBUG) $(PROG) -flto -fuse-linker-plugin -o debug

.PHONY: release
release:
	g++ $(FLAGS) $(LIBS) $(RELEASE) $(PROG) -march=native -flto -fuse-linker-plugin -o release

.PHONY: asm
asm:
	g++ $(FLAGS) $(LIBS) $(RELEASE) $(ASM) $(PROG) -o asm.s 
