SOURCES =	MimeIdentifier.cpp DirTree.cpp Indexer.cpp \
			Drawable.cpp DrawableList.cpp DrawText.cpp LabeledDrawable.cpp \
			Container.cpp Button.cpp ListItem.cpp TagsList.cpp \
			Star.cpp GSector.cpp Galaxy.cpp StateManager.cpp StatusBar.cpp \
			Main.cpp
			
OBJECTS = 	MimeIdentifier.o DirTree.o Indexer.o \
			Drawable.o DrawableList.o DrawText.o LabeledDrawable.o \
			Container.o Button.o ListItem.o TagsList.o \
			Star.o GSector.o Galaxy.o StateManager.o StatusBar.o \
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

