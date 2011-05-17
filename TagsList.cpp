//==============================================================================
// Date Created:		13 May 2011
// Last Updated:		13 May 2011
//
// File name:			TagsList.cpp
// Programmer:			Matthew Hydock
//
// File description:	Class for a scrollable list that holds file tags.
//==============================================================================

#include "TagsList.h"

//==============================================================================
// Constructor/Deconstructor
//==============================================================================
TagsList::TagsList(StateManager* sm, float x, float y, float w, float h, bool sc, int m):DrawableList(x,y,w,h,VERTICAL,0,0)
{	
//	cout << xPos << " " << yPos << " " << width << " " << height << " " << orientation << endl;
	
	state = sm;
	curr = state->getCurrent();
	
	scrollable = sc;
	
	selectionMode = m;
	
	rebuildTags();
}

TagsList::~TagsList()
{}
//==============================================================================


//==============================================================================
// Tag manipulation
//==============================================================================	
void TagsList::rebuildTags()
// Get a list of tags from the current galaxy, and turn them into text labels.
{
	clearTags();

//	cout << "rebuilding tags list\n";
	
	list<string>* galaxy_tags = curr->getTags();
	
	for (list<string>::iterator i = galaxy_tags->begin(); i != galaxy_tags->end(); i++)
	{
		ListItem* temp = new ListItem(*i);
//		cout << "adding list item to drawables list\n";
		addDrawable(temp);
//		cout << temp->getPosX() << " " << temp->getPosY() << endl;
	}
}

list<string>* TagsList::getAllTags()
// Make a list of strings from all of the tags' labels.
{
	list<string>* temp = new list<string>;
	
	for (list<Drawable*>::iterator i = drawables->begin(); i != drawables->end(); i++)
		temp->push_back(((ListItem*)(*i))->getText());
		
	return temp;
}

list<string>* TagsList::getSelectedTags()
// Make a list of strings from only the selected tags' labels.
{
	list<string>* temp = new list<string>;
	
	for (list<Drawable*>::iterator i = drawables->begin(); i != drawables->end(); i++)
		if (((ListItem*)(*i))->isSelected())
			temp->push_back(((ListItem*)(*i))->getText());
		
	return temp;
}

void TagsList::clearTags()
{
	if (drawables != NULL)
		delete(drawables);		
	
	height = 0;
	
	drawables = new list<Drawable*>;
//	cout << "drawables list recreated successfully\n";
}
//==============================================================================


//==============================================================================
// User interaction methods.
//==============================================================================
void TagsList::updateStateManager()
{

}
//==============================================================================


//==============================================================================
// Slightly different draw method needed.
//==============================================================================
void TagsList::draw()
{
	Galaxy* temp = state->getCurrent();
	if (temp != curr)
	{
		cout << "state changed\n";
		curr = temp;
		rebuildTags();
	}
	
	DrawableList::draw();
}
//==============================================================================
