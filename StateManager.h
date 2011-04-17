//==============================================================================
// Date Created:		6 April 2011
// Last Updated:		17 April 2011
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
		Indexer *indexer;
		list<Galaxy*> galaxies;
		list<Galaxy*>::iterator curr;
	
	public:
		StateManager(string dir);
		~StateManager();
		
		void forward();
		void backward();
		void navigate();
		void buildGalaxy(list<filenode*> *files, cluster_type mode);
		
		bool isColliding(float x, float y);
		
		void draw();
};

#endif
