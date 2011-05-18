//==============================================================================
// Date Created:		6 April 2011
// Last Updated:		17 May 2011
//
// File name:			StateManager.h
// Programmer:			Matthew Hydock
//
// File description:	A class to manage the current state of the galaxy
//						displayer. Maintains a list of previously generated
//						galaxies, and provides for the creation of new galaxies
//						and navigation to previous galaxies.
//==============================================================================

#include "StateManager.h"

//==============================================================================
// Constructor/Deconstructor.
//==============================================================================
StateManager::StateManager(string dir)
// Make a new galactic state manager.
{
	indexer = new Indexer(dir);
	
	Galaxy* temp = new Galaxy(indexer->getDirectoryTree()->getRootNode());	
	galaxies.push_back(temp);
	
	curr = galaxies.begin();
}

StateManager::~StateManager()
// Clean up after the galactic state manager.
{
	delete(indexer);
	
	for (curr = galaxies.begin(); curr != galaxies.end(); curr++)
		delete(*curr);
}
//==============================================================================


//==============================================================================
// Methods for observing/changing the state.
//==============================================================================	
Galaxy* StateManager::getCurrent()
// Return the current galaxy.
{
	return (*curr);
}

void StateManager::forward()
// Move forwards in history, if possible.
{
	curr++;
	if (curr == galaxies.end())
		curr--;
}

void StateManager::backward()
// Move backwards in history, if possible.
{
	if (curr != galaxies.begin())
		curr--;
}

void StateManager::navigate()
// If a sector in the current directory has been selected, make it into a
// galaxy, and make that galaxy the currently displayed one.
{
	GSector* selected = (*curr)->getSelected();
	
	if (selected != NULL)
	{
		if ((*curr)->getSectors()->size() == 1 || Star::starSelectionMode)
		{
			selected->activate();
			return;
		}
	
		list<Galaxy*>::iterator i = curr;
		i++;
		galaxies.erase(i, galaxies.end());
	
		dirnode* dir = selected->getDirectory();
		Galaxy* temp;
		
		if (dir != NULL)
			temp = new Galaxy(dir,NULL,(*curr)->getClusterMode());
		else
		{
			list<filenode*>* files = selected->getFileList();
			temp = new Galaxy(NULL,files,NONE,(*curr)->getName());
		}
		
		galaxies.push_back(temp);
		curr++;
	}
}

void StateManager::setActiveTags(list<string>* t)
{
	tags = t;
}
//==============================================================================


//==============================================================================
// Methods to change the clustering mode of the current set.
//==============================================================================
void StateManager::setDirectoryMode()
// Backtrack until the current galaxy is in directory mode.
{
	while ((*curr)->getClusterMode() != DIRECTORY && curr != galaxies.begin())
		curr--;
}

void StateManager::setNameMode()
{}

void StateManager::setDateMode()
{}

void StateManager::setSizeMode()
{}

void StateManager::setTypeMode()
{}

void StateManager::setTagsMode()
{
	// Remove all of the previously generated galaxies that are ahead of the
	// current position.
	list<Galaxy*>::iterator i = curr;
	i++;
	galaxies.erase(i, galaxies.end());
	// Done cleaning up future history.
	
	// Prepare lists to hold all files, and only those with the tags.
	list<filenode*>* files = (*curr)->getFileList();
	list<filenode*>* valid_files = new list<filenode*>;
	
	// Name of the new galaxy, based on the selected tags.
	string name = (*curr)->getName() + " - ";
	
	for (list<filenode*>::iterator i = files->begin(); i!= files->end(); i++)
	{
		for (list<string>::iterator j = tags->begin(); j != tags->end(); j++)
			if (contains(&((*i)->tags),*j))
			{
				valid_files->push_back(*i);
				break;
			}
	}
	
	// Add the tags to the name.
	for (list<string>::iterator i = tags->begin(); i != tags->end(); i++)
		name += (*i) + " ";
	
	// Make a new galaxy, push it onto the stack, and navigate to it.	
	Galaxy* temp = new Galaxy(NULL,valid_files,TAGS,name,tags);
	galaxies.push_back(temp);
	curr++;
}
//==============================================================================


//==============================================================================
// Inherited methods.
//==============================================================================
bool StateManager::isColliding(float x, float y)
{
	return collide_flag = (*curr)->isColliding(x,y);
}

void StateManager::draw()
{
	(*curr)->draw();
}
//==============================================================================
