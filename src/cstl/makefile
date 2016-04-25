GPP=g++
GCC=gcc
OUTFILE="./cstl.so"

COMPILE_FLAGS=-c -O3 -w -DLINUX -fno-exceptions -I ./AMX_SDK/amx/

all:
	$(GCC) $(COMPILE_FLAGS) ./AMX_SDK/amx/*.c
	$(GPP) $(COMPILE_FLAGS) ./AMX_SDK/*.cpp
	$(GPP) $(COMPILE_FLAGS) ./*.cpp
	$(GPP) -O2 -fshort-wchar -shared -o $(OUTFILE) *.o -lpthread
	rm -f *.o