//==============================================================================
// Date Created:		6 February 2011
// Last Updated:		17 April 2011
//
// File name:			global_header.h
// Programmer:			Matthew Hydock
//
// File description:	A header file to contain common and helpful includes and
//						methods, compiled for the StarNavi project.
//==============================================================================

#ifndef STARDM
#define STARDM

#define _USE_MATH_DEFINES

//C includes
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

//C++ includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>


#include "MersenneTwister.h"
#include "Functors.h"

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
	for (size_t i = 0; i < s1.size()-1 && i < s2.size()-1 && diff == 0; i++)
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

extern inline list<string>* tokenizeL(string s, string del)
{
	if (s.compare("") == 0)
		return NULL;
		
	list<string>* l = new list<string>();
	char* cs = (char*)malloc(sizeof(char) * (s.size()+1));
	char* ptr;
	
	strcpy(cs,s.c_str());
	ptr = strtok(cs,del.c_str());
	
	// Place tokens in a list.
	while (ptr != NULL)
	{
		l->push_back((string)ptr);
		ptr = strtok(NULL,del.c_str());
	}
	
	free(cs);
	
	return l;
}

extern inline vector<string>* tokenizeV(string s, string del)
// Split up a string and store the segments into a list, using a user-specified
// delimiter.
{
	if (s.compare("") == 0)
		return NULL;
		
	list<string>* l = tokenizeL(s,del);
	
	// Convert list to an array.
	vector<string>* v = new vector<string>(l->size(),"");
	list<string>::iterator it = l->begin();
	for (unsigned int i = 0; i < v->size(); i++)
	{
		v->at(i) = *it;
		it++;
	}
	
	return v;
}

template<typename T> extern inline void append(list<T>* l1, list<T>* l2)
// Since the list library doesn't have a function to append a list onto another
// list...
{
	typename list<T>::iterator i2 = l2->begin();
	for(; i2 != l2->end(); i2++)
		l1->push_back(*i2);
}

template<typename T> extern inline bool contains(list<T>* l, T i)
// Checks to see if the given list contains the requested item.
{
	return (find(l->begin(),l->end(),i) != l->end());
}
#endif
