//==============================================================================
// Date Created:		6 February 2011
// Last Updated:		16 February 2011
//
// File name:			global_header.h
// Programmer:			Matthew Hydock
//
// File description:	A header file to contain structs and includes relevant
//						to the StarDM project.
//==============================================================================

#ifndef STARDM
#define STARDM

//C includes
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

//C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

using namespace std;

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
	list<filenode> files;
	list<struct dirnode> dirs;
}dirnode;

extern inline char toLower(char a)
// Makes lowercase characters uppercase, doesn't affect other characters.
{
	if (a >= 97 && a <= 122)
		a -= 32;
		
	return a;
}

extern inline vector<string> tokenize(string s, string del)
// Split up a string and store the segments into a list, using a user-specified
// delimiter.
{
	if (s.compare("") == 0)
		return vector<string>();
		
	list<string> l;
	char *cs = new char[s.size()+1];
	char *ptr;
	
	strcpy(cs,s.c_str());
	ptr = strtok(cs,del.c_str());
	
	// Place tokens in a list.
	while (ptr != NULL)
	{
		l.push_back((string)ptr);
		ptr = strtok(NULL,del.c_str());
	}
	
	// Convert list to an array.
	vector<string> sv (l.size(),"");
	list<string>::iterator it = l.begin();
	for (unsigned int i = 0; i < sv.size(); i++)
	{
		sv[i] = *it;
		it++;
	}
	
	// cleanup
	delete (cs);
	
	return sv;
}

#endif
