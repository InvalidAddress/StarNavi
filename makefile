SOURCES = Indexer.cpp test_main.cpp MimeIdentifier.cpp Star.cpp Galaxy.cpp
OBJECTS = Indexer.o test_main.o MimeIdentifier.o Star.o Galaxy.o
HEADERS = global_header.h

CC = g++
CFLAGS = -g -Wall
	
main:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -lm -lGL -lGLU -lglut -lilut -lX11 -o testmain
	
	mv -f ./testmain /home/mhydock/indexer
	chmod +x /home/mhydock/indexer/testmain
