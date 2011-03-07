SOURCES = MimeIdentifier.cpp DirTree.cpp Indexer.cpp Star.cpp Galaxy.cpp test_main.cpp
OBJECTS = MimeIdentifier.o DirTree.o Indexer.o Star.o Galaxy.o test_main.o
HEADERS = global_header.h fs_structs.h

CC = g++
CFLAGS = -g -Wall
	
normbuild:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -lm -lGL -lGLU -lglut -lIL -lILU -lILUT -lX11 -o testmain

testbuild:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -lm -lGL -lGLU -lglut -lIL -lILU -lILUT -lX11 -o testmain	
	mv -f ./testmain ~/indexer
	chmod +x ~/indexer/testmain
