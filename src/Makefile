CC = g++
CFLAGS = -W -Wall -O2 $(shell pkg-config --cflags opencv)

LIBS += $(shell pkg-config --libs opencv)

BIN = ../bin/

all: webcamWizard
	
webcamWizard: $(BIN)model.o $(BIN)main.o $(BIN)calibrator.o $(BIN)pathfinder.o
	$(CC) -o $(BIN)webcamWizard $(BIN)model.o $(BIN)main.o $(BIN)calibrator.o $(BIN)pathfinder.o $(LIBS)
$(BIN)model.o: model.cpp model.hpp  calibrator.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)model.o model.cpp
$(BIN)calibrator.o: calibrator.cpp  calibrator.hpp model.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)calibrator.o calibrator.cpp
$(BIN)pathfinder.o: pathfinder.cpp pathfinder.hpp model.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)pathfinder.o pathfinder.cpp
#$(BIN)pathfinder_all_red.o: pathfinder_all_red.cpp pathfinder_all_red.hpp pathfinder.cpp pathfinder.hpp
#	$(CC) $(CFLAGS) -c -o $(BIN)pathfinder_all_red.o pathfinder_all_red.cpp
$(BIN)main.o: main.cpp model.hpp pathfinder.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)main.o main.cpp
