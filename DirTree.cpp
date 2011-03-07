//==============================================================================
// Date Created:		5 March 2011
// Last Updated:		5 March 2011
//
// File name:			DirTree.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class that allows for the creation and navigation of a
//						tree-type structure designed for file system indexing.
//==============================================================================

#include "DirTree.h"

//==============================================================================
// Constructors/Deconstructors
//==============================================================================
DirTree::DirTree()
// Parameterless directory tree constructor.
{	
}


DirTree::DirTree(string dir)
// Creates a directory tree with the root node at dir.
{
	mrmime = new MimeIdentifier();
	file_list = new list<filenode*>();
	numfiles = 0;
	
	// Make sure the directory ends with a forward slash
	if (*(dir.rbegin()) != '/')
		dir.push_back('/');
		
	root.name = dir;
}


DirTree::~DirTree()
// Directory tree deconstructor.
{
	clearTree();
	delete(mrmime);
	delete(file_list);
	
	cout << "DIRTREE DELETED\n";
}
//==============================================================================


//==============================================================================
// Node-based methods
//==============================================================================
void DirTree::add(string p, string n)
// Method to insert a file into its appropriate place in the file list.
{	
	// Create a filenode and set its attributes.	
	filenode *tempf = new filenode;
	string temp_string = p + n;
	tempf->name = n;
	tempf->path = p;
	stat(temp_string.c_str(), &(tempf->attr));
	tempf->type = mrmime->obtainType(n);
	
	//cout << tempf->path << tempf->name << endl;
	
	// Tokenize the given path, and store in an array.
	vector<string> path_toks = tokenize(p.substr(root.name.size()),"/");
	
	// Set the current node to the root of the directory tree, and start an
	// iterator on the current node's directory list.
	dirnode *currnode = &root;
	list<dirnode>::iterator d = currnode->dirs.begin();
	
	// While there are still more paths to navigate...
	for (int i = 0; i < path_toks.size();)
	{		
		// Scan the directory list for the current directory.
		for (;d != currnode->dirs.end() && strcmp(path_toks[i].c_str(),d->name.c_str()) != 0 && !isLessThan(path_toks[i],d->name);d++);
		
		// If current directory exists, go to it.
		if (d != currnode->dirs.end() && strcmp(path_toks[i].c_str(),d->name.c_str()) == 0)
		{
			currnode = &(*d);
			d = currnode->dirs.begin();
			i++;
		}
		// If current directory does not exist, make it.
		else
		{
			dirnode tempd;
			tempd.name = path_toks[i];
			currnode->dirs.insert(d,tempd);
			d--;
		}
	}
	
	// Now that we're in the right directory, add file to list. Doesn't matter
	// that the list is out of order, as items in a list object take O(n) time
	// to access, making neat searching algorithms useless.
	currnode->files.push_back(tempf);
	// i still have seriously no clue how lists work, or why they appear to
	// override the pointer character...
	file_list->push_back(tempf);
	
	//cout << (*(file_list->rbegin()))->path << (*(file_list->rbegin()))->name << endl;
	
	numfiles++;
}


dirnode* DirTree::getDir(string p)
// Try to obtain a directory given its name.
{
	vector<string> path_toks = tokenize(p.substr(root.name.size()),"/");
	
	// Set the current node to the root of the directory tree, and start an
	// iterator on the current node's directory list.
	dirnode *currnode = &root;
	list<dirnode>::iterator d = currnode->dirs.begin();
	
	// While there are still more paths to navigate...
	for (int i = 0; i < path_toks.size();)
	{
		// Scan the directory list for the current directory.
		for (;d != currnode->dirs.end() && strcmp(path_toks[i].c_str(),d->name.c_str()) != 0 && !isLessThan(path_toks[i],d->name);d++);
		
		// If current directory exists, go to it.
		if (d != currnode->dirs.end() && strcmp(path_toks[i].c_str(),d->name.c_str()) == 0)
		{
			currnode = &(*d);
			d = currnode->dirs.begin();
			i++;
		}
		// If current directory does not exist, return null
		else
			return NULL;
	}
	
	return &(*d);
}


filenode* DirTree::getFile(string p, string n)
// Tries to find a file, given its path and name.
{
	// Get the directory that the file is supposed to be in.
	dirnode *dir = getDir(p);
	
	// If directory doesn't exist, return null.
	if (dir == NULL) return NULL;
	
	// Now scan the current directory's file list
	list<filenode*>::iterator f = (dir->files).begin();
	for (; f != (dir->files).end() && (*f)->name.compare(n) == 0; f++);
	
	// If the file doesn't exist, return null.
	if (f == (dir->files).end())
		return NULL;
	
	return (*f);
}
//==============================================================================


//==============================================================================
// Whole-tree methods.
//==============================================================================
void DirTree::clearTree()
// Empties the entire tree.
{
	dropBranch(&root);
}


void DirTree::dropBranch(dirnode *d)
// Drop a branch from the tree.
{
	d->files.clear();

	list<dirnode>::iterator li = d->dirs.begin();
	
	for (; li != d->dirs.end(); li++)
		dropBranch(&(*li));
		
	d->dirs.clear();
}
//==============================================================================


//==============================================================================
// Convenience methods.
//==============================================================================
list<filenode*>* DirTree::getFileList()
// Return a pointer to the list of files.
{
	return file_list;
}


string DirTree::getRootPath()
// Returns the name (path) of the root node.
{
	return root.name;
}


int DirTree::getNumFiles()
// Get the number of files in the file tree.
{
	return numfiles;
}
//==============================================================================
