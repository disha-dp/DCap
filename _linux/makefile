# TwsApiC++ Debug Version

CC      = gcc
INCLUDE = -I../../Api -I../../../source/PosixClient/Shared
CFLAGS  = -D_REENTRANT -D_DEBUG -ggdb -Wall
LIBS    = -L../../Api -lTwsApiL0D -lstdc++ -lpthread

Main: ../Src/Main.cpp ../../Api/libTwsApiL0D.a
	$(CC) $(INCLUDE) $(CFLAGS) ../Src/Main.cpp    $(LIBS) -o ./debug/Main

Test: ../Src/Test.cpp ../../Api/libTwsApiL0D.a
	$(CC) $(INCLUDE) $(CFLAGS) ../Src/Test.cpp    $(LIBS) -o ./debug/Test

History: ../Src/History.cpp ../../Api/libTwsApiL0D.a
	$(CC) $(INCLUDE) $(CFLAGS) ../Src/History.cpp $(LIBS) -o ./debug/History

Bars: ../Src/Bars.cpp ../../Api/libTwsApiL0D.a
	$(CC) $(INCLUDE) $(CFLAGS) ../Src/Bars.cpp    $(LIBS) -o ./debug/Bars

Clients: ../Src/Clients.cpp ../../Api/libTwsApiL0D.a
	$(CC) $(INCLUDE) $(CFLAGS) ../Src/Clients.cpp $(LIBS) -o ./debug/Clients

all: Test History Bars Clients Main

main: Main
	
clean:
	rm -f ./debug/*