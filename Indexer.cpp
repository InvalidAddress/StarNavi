//==============================================================================
// Date Created:		4 February 2011
// Last Updated:		6 March 2011
//
// File name:			indexer.cpp
// Programmer:			Matthew Hydock
//
// File description:	Contains a C++ class to index a file system recursively
//						from a given directory. Horrendously overcomplicated.
//==============================================================================

#include "Indexer.h"

//==============================================================================
// Private methods
//==============================================================================
void Indexer::build(string dir)
// Recursive method to build a directory hierarchy.
{
	DIR *d = opendir(dir.c_str());
	dirent *dr = readdir(d);
	
	while (dr != NULL)
	{
		if (dr->d_type == DT_REG)
		// If a file, add to file list
			dir_tree->add(dir, dr->d_name);
		else if (dr->d_type == DT_DIR && (strcmp(dr->d_name, "..") != 0 && strcmp(dr->d_name, ".") != 0))
		// If a directory, but not ./ or ../, then immediately dive into it.
			build(dir + dr->d_name + "/");
			
		dr = readdir(d);
	}
	
	closedir(d);
}
//==============================================================================


//==============================================================================
// Constructor/Deconstructor
//==============================================================================	
Indexer::Indexer(string root_path)
// Constructor. Initialized the number of files, and sets the root path.
{
	dir_tree = new DirTree(root_path);
	
	build();
}


Indexer::~Indexer()
// Deconstructor. Clear and delete the file list.
{
	clearTree();
	delete(dir_tree);
}
//==============================================================================


//==============================================================================
// Public methods
//==============================================================================
void Indexer::build()
{
	build(dir_tree->getRootPath());
}


void Indexer::changeRoot(string new_root)
{
	delete(dir_tree);
	dir_tree = new DirTree(new_root);
}


void Indexer::clearTree()
// Function to empty the directory tree.
{
	dir_tree->clearTree();
}


DirTree* Indexer::getDirectoryTree()
// Get the file list that the indexer filled.
{
	return dir_tree;
}

list<filenode*>* Indexer::getFileList()
{
	return dir_tree->getFileList();
}
//==============================================================================
