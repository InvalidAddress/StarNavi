SOURCES =	MimeIdentifier.cpp DirTree.cpp Indexer.cpp Drawable.cpp DrawText.cpp \
			Star.cpp GSector.cpp Galaxy.cpp StateManager.cpp Container.cpp Button.cpp \
			ButtonList.cpp Main.cpp
			
OBJECTS = 	MimeIdentifier.o DirTree.o Indexer.o Drawable.o DrawText.o Star.o \
			GSector.o Galaxy.o StateManager.o Container.o Button.o ButtonList.o \
			Main.o
			
HEADERS =	global_header.h fs_structs.h

CC = g++
CFLAGS = -g \
	-Wall \
	-Wextra \
	-pedantic \
# endlist

CPPFLAGS := $(CFLAGS)

LDFLAGS :=  -lm -lmagic -lGL -lGLU -lglut -lIL -lILU -lILUT -lSDL -lSDL_ttf -lX11

.PHONY: default
default: testbuild

.PHONY: clean
clean:
	rm -f $(OBJECTS) starnavi

normbuild:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o starnavi

testbuild:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o starnavi	
	mv -f ./starnavi ~/test\ dir
	chmod +x ~/test\ dir/starnavi

