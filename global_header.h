//==============================================================================
// Date Created:		6 February 2011
// Last Updated:		7 March 2011
//
// File name:			global_header.h
// Programmer:			Matthew Hydock
//
// File description:	A header file to contain common and helpful includes and
//						methods, compiled for the StarDM project.
//==============================================================================

#ifndef STARDM
#define STARDM

//C includes
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>

//C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#include "MersenneTwister.h"

using namespace std;

extern inline char toLower(char a)
// Makes lowercase characters uppercase, doesn't affect other characters.
{
	if (a >= 97 && a <= 122)
		a -= 32;
		
	return a;
}

extern inline bool isLessThan(string s1, string s2)
// Checks to see if s1 is alphabetically before s2.
{
	int diff = 0;
	
	// Compare the strings one letter at a time. Quit early if s1 is less than
	// s2. Also, keep track of the equality of the strings.
	for (int i = 0; i < s1.size()-1 && i < s2.size()-1 && diff == 0; i++)
		diff = toLower(s1.at(i)) - toLower(s2.at(i));
	
	// If the strings appear equal but s1 is smaller than s2, then s1 is less
	// than s2. Otherwise, let the lessThan variable be returned (which should
	// be false after all this anyway).
	if (diff == 0)
	{
		if (s1.size() < s2.size())
			return true;
		return false;
	}
			
	return diff < 0;
}

extern inline vector<string> tokenize(string s, string del)
// Split up a string and store the segments into a list, using a user-specified
// delimiter.
{
	if (s.compare("") == 0)
		return vector<string>();
		
	list<string> l;
	char *cs = (char*)malloc(sizeof(char) * (s.size()+1));
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
	free(cs);
	
	return sv;
}

#endif
