SOURCES = Indexer.cpp test_main.cpp MimeIdentifier.cpp Star.cpp Galaxy.cpp
OBJECTS = Indexer.o test_main.o MimeIdentifier.o Star.o Galaxy.o
HEADERS = global_header.h

CC = g++
CFLAGS = -g -Wall
	
normbuild:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -lm -lGL -lGLU -lglut -lIL -lILU -lILUT -lX11 -o testmain

testbuild:
	$(CC) $(CFLAGS) $(OBJECTS) -lm -lGL -lGLU -lglut -lIL -lILU -lILUT -lX11 -o testmain	
	mv -f ./testmain ~/indexer
	chmod +x ~indexer/testmain
