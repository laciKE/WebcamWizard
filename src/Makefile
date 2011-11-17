CC = g++
CFLAGS = -W -Wall -O2 -I/usr/include/opencv

ifeq ($(shell uname),Darwin)
	LIBS = -framework OpenGL -framework GLUT
else
	LIBS = -lglut -lGLU -lGL
endif

LIBS += -lcv -lhighgui -lcvaux -lml

BIN = ../bin/

all: blackboard
	
blackboard: $(BIN)blackboard.o $(BIN)main.o $(BIN)calibrator.o $(BIN)desktopdrawer.o
	$(CC) -o $(BIN)blackboard $(BIN)blackboard.o $(BIN)main.o $(BIN)calibrator.o $(BIN)desktopdrawer.o $(LIBS)
$(BIN)blackboard.o: blackboard.cpp blackboard.hpp desktopdrawer.hpp calibrator.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)blackboard.o blackboard.cpp
$(BIN)calibrator.o: calibrator.cpp  calibrator.hpp blackboard.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)calibrator.o calibrator.cpp
$(BIN)desktopdrawer.o: desktopdrawer.cpp desktopdrawer.hpp blackboard.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)desktopdrawer.o desktopdrawer.cpp
$(BIN)main.o: main.cpp blackboard.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)main.o main.cpp
