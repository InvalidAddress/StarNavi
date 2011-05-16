//==============================================================================
// Date Created:		5 March 2011
// Last Updated:		14 May 2011
//
// File name:			DirTree.h
// Programmer:			Matthew Hydock
//
// File description:	Header for a class that allows for the creation and
//						navigation of a tree-type structure designed for file
//						system indexing.
//==============================================================================

#include "global_header.h"
#include "fs_structs.h"
#include "MimeIdentifier.h"


#ifndef DIRTREE
#define DIRTREE

class DirTree
{
	private:
		MimeIdentifier* mrmime;
		int numfiles;
		dirnode* root;
		
		void setTags(filenode* f);
		
	public:
		DirTree(string s);
		~DirTree();
		
		void add(string p, string n);
		dirnode* getDir(string p);
		filenode* getFile(string p, string n);
		
		void clearTree();
		void dropBranch(dirnode* d);
		
		list<filenode*>* getFileList();
		string getRootPath();
		dirnode* getRootNode();
		int getNumFiles();
};

#endif
