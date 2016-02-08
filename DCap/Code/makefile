# TwsApiC++ Debug Version

CC      = gcc
INCLUDE = -I../Api -I../../../source/PosixClient/Shared
CFLAGS  = -D_REENTRANT -D_DEBUG -ggdb -Wall
LIBS    = -L../Api -lTwsApiL0D -lstdc++ -lpthread

Main: Src/Main.cpp ../Api/libTwsApiL0D.a
	$(CC) $(INCLUDE) $(CFLAGS) Src/Main.cpp    $(LIBS) -o ./debug/Main

all: Main

main: Main
	
clean:
	rm -f ./debug/*