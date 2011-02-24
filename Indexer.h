//==============================================================================
// Date Created:		14 February 2011
// Last Updated:		16 February 2011
//
// File name:			indexer.h
// Programmer:			Matthew Hydock
//
// File description:	Class definition for file indexer built in C++.
//==============================================================================

#include "global_header.h"
#include "MimeIdentifier.h"

class Indexer
{
	private:
		MimeIdentifier *mrmime;
		int numfiles;
		dirnode dir_tree;
		list<filenode*> file_list;
	
		void build(string dir);
		void add(string p, string n);
		bool isLessThan(string s1, string s2);
		void clearTree();
		void clearTree(dirnode *d);
		
	public:
		Indexer(string root_path);
		~Indexer();
		void build();
		dirnode* getDirectoryTree();
		list<filenode*>* getFileList();
		int getNumFiles();
};
