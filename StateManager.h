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

#include "Indexer.h"
#include "Galaxy.h"

#ifndef STATEMANAGER
#define STATEMANAGER

class StateManager:public Drawable
{
	private:
		Indexer* indexer;
		list<Galaxy*> galaxies;
		list<Galaxy*>::iterator curr;
		
		list<string>* tags;
	
	public:
		StateManager(string dir);
		~StateManager();
		
		Galaxy* getCurrent();
		
		void forward();
		void backward();
		void navigate();
		
		void setActiveTags(list<string>* t);
		
		void setDirectoryMode();
		void setNameMode();
		void setDateMode();
		void setSizeMode();
		void setTypeMode();
		void setTagsMode();		
		
		bool isColliding(float x, float y);
		
		void draw();
};

#endif
