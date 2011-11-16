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
	
blackboard: $(BIN)blackboard.o $(BIN)main.o
	$(CC) -o $(BIN)blackboard $(BIN)blackboard.o $(BIN)main.o $(LIBS)
$(BIN)blackboard.o: blackboard.cpp blackboard.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)blackboard.o blackboard.cpp
$(BIN)main.o: main.cpp blackboard.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)main.o main.cpp
