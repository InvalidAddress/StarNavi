//==============================================================================
// Date Created:		6 March 2011
// Last Updated:		7 March 2011
//
// File name:			fs_structs.h
// Programmer:			Matthew Hydock
//
// File description:	Contains structures and definitions for managing file
//						and directory information
//==============================================================================

#include <dirent.h>
#include <list>

#ifndef FS_STRUCTS
#define FS_STRUCTS

//==============================================================================
// Structs and typedefs
//==============================================================================

// cuz i don't feel like typing 'struct dirent' every time i want to use one
typedef struct dirent dirent;

// To represent the different types of files.
enum filetype {BIN, APP, AUDIO, IMAGE, TEXT, VIDEO, UNKNOWN};

// Structure to a file and its various attributes.
typedef struct filenode
{
	string path;
	string name;
	enum filetype type;
	struct stat attr;
}filenode;

// Structure to represent a directory, and its list of files and directories.
typedef struct dirnode
{
	string name;
	list<filenode*> files;
	list<struct dirnode> dirs;
}dirnode;
//==============================================================================

#endif
