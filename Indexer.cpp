//==============================================================================
// Date Created:		4 February 2011
// Last Updated:		17 February 2011
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

//==============================================================================
// The next five methods are solely for building the directory tree
//==============================================================================
void Indexer::build(string dir)
// Recursive method to build a directory hierarchy.
{
	// Make sure the directory ends with a forward slash
	if (*(dir.rbegin()) != '/')
		dir.push_back('/');
	
	DIR *d = opendir(dir.c_str());
	dirent *dr = readdir(d);
				
	while (dr != NULL)
	{
		if (dr->d_type == DT_REG)
		// If a file, add to file list
			add(dir, dr->d_name);
			
		else if (dr->d_type == DT_DIR && (strcmp(dr->d_name, "..") != 0 && strcmp(dr->d_name, ".") != 0))
		// If a directory, but not ./ or ../, then immediately dive into it.
			build(dir + dr->d_name + "/");
			
		dr = readdir(d);
	}
	
	closedir(d);
}

void Indexer::add(string p, string n)
// Method to insert a file into its appropriate place in the file list.
{
	// Create a filenode and set its attributes.	
	filenode tempf;
	string temp_string = p + n;	
	tempf.name = n;
	tempf.path = p;
	stat(temp_string.c_str(), &(tempf.attr));
	tempf.type = mrmime->obtainType(n);
	
	// Tokenize the given path, and store in an array.
	vector<string> path_toks = tokenize(p.substr(dir_tree.name.size()),"/");
	
	// Set the current node to the root of the directory tree, and start an
	// iterator on the current node's directory list.
	dirnode *currnode = &dir_tree;
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
	file_list.push_back(&(*(currnode->files.rbegin())));
	
	numfiles++;
}

bool Indexer::isLessThan(string s1, string s2)
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
//==============================================================================

void Indexer::clearTree()
// Function to empty the directory tree.
{
	clearTree(&dir_tree);
}

void Indexer::clearTree(dirnode *d)
// Recursive function to do clearTree's dirty work.
{
	d->files.clear();
//	cout << d->name << " files deleted\n";
	list<dirnode>::iterator li = d->dirs.begin();
	
	for (; li != d->dirs.end(); li++)
		clearTree(&(*li));
		
	d->dirs.clear();
//	cout << d->name << " subdirectories deleted\n";
}
//==============================================================================


//==============================================================================
// Public methods
//==============================================================================	
Indexer::Indexer(string root_path)
// Constructor. Initialized the number of files, and sets the root path.
{
	numfiles = 0;
	
	dir_tree.name = root_path;
	
	mrmime = new MimeIdentifier();
}

void Indexer::build()
{
	build(dir_tree.name);
}

dirnode* Indexer::getDirectoryTree()
// Get the file list that the indexer filled.
{
	return &dir_tree;
}

list<filenode*>* Indexer::getFileList()
// Return a pointer to the list of files.
{
	return &file_list;
}

int Indexer::getNumFiles()
// Get the number of files in the file tree.
{
	return numfiles;
}
	
Indexer::~Indexer()
// Deconstructor. Clear and delete the file list.
{
	clearTree();
}
//==============================================================================
