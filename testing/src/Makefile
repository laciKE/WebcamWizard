CC = g++
CFLAGS = -W -Wall -O2 -I/usr/include/opencv

ifeq ($(shell uname),Darwin)
	LIBS = -framework OpenGL -framework GLUT
else
	LIBS = -lglut -lGLU -lGL
endif

LIBS += -lcv -lhighgui -lcvaux -lml

BIN = ../bin/

all: tabula
	
tabula: $(BIN)tabula.o $(BIN)main.o
	$(CC) -o $(BIN)tabula $(BIN)tabula.o $(BIN)main.o $(LIBS)
$(BIN)tabula.o: tabula.cpp tabula.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)tabula.o tabula.cpp
$(BIN)main.o: main.cpp tabula.hpp
	$(CC) $(CFLAGS) -c -o $(BIN)main.o main.cpp
