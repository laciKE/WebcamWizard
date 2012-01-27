CC = g++
CFLAGS = -W -Wall -O2 -I/usr/include/opencv

LIBS += -lcv -lhighgui -lcvaux -lml -lcxcore

BIN = ../bin/

all: blackboard
	
blackboard: $(BIN)blackboard.o $(BIN)main.o $(BIN)calibrator.o $(BIN)desktopdrawer.o $(BIN)pathfinder.o
	$(CC) -o $(BIN)blackboard $(BIN)blackboard.o $(BIN)main.o $(BIN)calibrator.o $(BIN)desktopdrawer.o $(BIN)pathfinder.o $(LIBS)
$(BIN)blackboard.o: blackboard.cpp blackboard.hpp desktopdrawer.hpp calibrator.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)blackboard.o blackboard.cpp
$(BIN)calibrator.o: calibrator.cpp  calibrator.hpp blackboard.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)calibrator.o calibrator.cpp
$(BIN)desktopdrawer.o: desktopdrawer.cpp desktopdrawer.hpp blackboard.hpp pathfinder.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)desktopdrawer.o desktopdrawer.cpp
$(BIN)pathfinder.o: pathfinder.cpp pathfinder.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)pathfinder.o pathfinder.cpp
#$(BIN)pathfinder_all_red.o: pathfinder_all_red.cpp pathfinder_all_red.hpp pathfinder.cpp pathfinder.hpp
#	$(CC) $(CFLAGS) -c -o $(BIN)pathfinder_all_red.o pathfinder_all_red.cpp
$(BIN)main.o: main.cpp blackboard.hpp pathfinder.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)main.o main.cpp
