//==============================================================================
// Date Created:		13 May 2011
// Last Updated:		17 May 2011
//
// File name:			TagsList.h
// Programmer:			Matthew Hydock
//
// File description:	A header for a scrollable list that holds file tags.
//==============================================================================

#include "StateManager.h"
#include "DrawableList.h"
#include "ListItem.h"

#ifndef TAGSLIST
#define TAGSLIST

#define ONE 1
#define MULTIPLE 2

class TagsList:public DrawableList
{	
	private:
		StateManager* state;
		Galaxy* curr;
		
		float realHeight;
		bool scrollable;
		
		int selectionMode;
		
	public:
		TagsList(StateManager* sm, float x, float y, float w, float h, bool sc = false, int m = MULTIPLE);
		~TagsList();
		
		void rebuildTags();
		list<string>* getAllTags();
		list<string>* getSelectedTags();
		void clearTags();	
		
		void updateStateManager();
		
		void draw();
};
#endif
