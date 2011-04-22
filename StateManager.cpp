//==============================================================================
// Date Created:		6 April 2011
// Last Updated:		20 April 2011
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
	
	Galaxy *temp = new Galaxy(indexer->getDirectoryTree()->getRootNode());	
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
// Methods for changing the state.
//==============================================================================	
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
	curr--;
	if ((list<Galaxy*>::reverse_iterator)curr == galaxies.rend())
		curr++;
}

void StateManager::navigate()
// If a sector in the current directory has been selected, make it into a
// galaxy, and make that galaxy the currently displayed one.
{
	if ((*curr)->getSectors()->size() == 1)
		return;
		
	GSector* selected = (*curr)->getSelected();
	
	if (selected != NULL)
	{
		dirnode *dir = selected->getDirectory();
		Galaxy *temp;
		
		if (dir != NULL)
			temp = new Galaxy(dir,NULL,(*curr)->getMode());
		else
		{
			list<filenode*> *files = selected->getFileList();
			temp = new Galaxy(NULL,files,(*curr)->getMode(),(*curr)->getDirectory()->name);
		}
		
		galaxies.push_back(temp);
		curr++;
	}
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
