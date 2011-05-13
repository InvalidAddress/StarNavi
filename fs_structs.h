//==============================================================================
// Date Created:		6 March 2011
// Last Updated:		13 May 2011
//
// File name:			fs_structs.h
// Programmer:			Matthew Hydock
//
// File description:	Contains structures and definitions for managing file
//						and directory information
//==============================================================================

#include <sys/stat.h>
#include <dirent.h>

#include <list>
#include <string>

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
	string mimetype;
	string default_app;
	enum filetype mime_enum;
	struct stat attr;
	list<string> tags;
}filenode;

// Structure to represent a directory, and its list of files and directories.
typedef struct dirnode
{
	string name;
	string path;
	list<filenode*> files;
	list<struct dirnode*> dirs;
	list<filenode*> all_files;
}dirnode;
//==============================================================================

#endif
