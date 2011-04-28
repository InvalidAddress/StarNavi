SOURCES = MimeIdentifier.cpp DirTree.cpp Indexer.cpp Drawable.cpp Star.cpp GSector.cpp Galaxy.cpp StateManager.cpp Container.cpp Button.cpp Main.cpp
OBJECTS = MimeIdentifier.o DirTree.o Indexer.o Drawable.o Star.o GSector.o Galaxy.o StateManager.o Container.o Button.o Main.o
HEADERS = global_header.h fs_structs.h

CC = g++
CFLAGS = -g -Wall
	
normbuild:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -lm -lmagic -lGL -lGLU -lglut -lIL -lILU -lILUT -lSDL -lSDL_ttf -lX11 -o starnavi

testbuild:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -lm -lmagic -lGL -lGLU -lglut -lIL -lILU -lILUT -lSDL -lSDL_ttf -lX11 -o starnavi	
	mv -f ./starnavi ~/test\ dir
	chmod +x ~/test\ dir/starnavi
