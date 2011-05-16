//==============================================================================
// Date Created:		6 May 2011
// Last Updated:		16 May 2011
//
// File name:			StatusBar.cpp
// Programmer:			Matthew Hydock
//
// File description:	A simple status bar that displays the current directory,
//						and the number of files in that directory tree.
//==============================================================================

#include "StatusBar.h"

StatusBar::StatusBar(StateManager* sm)
{
	xPos = 0;
	yPos = 0;
	width = 0;
	height = 0;
	
	state = sm;

	curr = state->getCurrent();
	
	directory = new DrawText("a");
	num_files = new DrawText("b");
	
	directory->setFontSize(12);
	num_files->setAlignment(LEFT);
	refreshState();
}

StatusBar::~StatusBar()
{}
		
void StatusBar::setStateManager(StateManager* sm)
{
	state = sm;
}

StateManager* StatusBar::getStateManager()
{
	return state;
}
		
void StatusBar::refreshState()
{
	ostringstream oss;
	curr = state->getCurrent();
	int num;
	
	dirnode* dir = curr->getDirectory();
	if (dir == NULL)
	{
//		cout << "dir is null, using filelist\n";
		num = curr->getFileList()->size();
//		cout << "got num files: " << num << endl;
	}
	else
	{
//		cout << "dir is " << curr->getDirectory()->name << endl;
		num = curr->getDirectory()->all_files.size();
//		cout << "got num files: " << num << endl;
	}

	oss << " Files: " << num;
	
	directory->setText(curr->getName());
	num_files->setText(oss.str());
	
//	num_files->setAnchor(LEFT_UPPER);

	directory->refreshTexture();
	num_files->refreshTexture();
	
//	cout << "rebuilt the status bar\n";
}
	
void StatusBar::draw()
{
	if (curr != state->getCurrent())
		refreshState();
	
	// Set the position of the labels, based on the viewport.
	int p[4];
	glGetIntegerv(GL_VIEWPORT,p);
	
	// Turn on blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	directory->setPosition(0,0);
	num_files->setPosition((float)p[2]/-2,0);
	
	directory->draw();
	num_files->draw();
	
	// Turn off blending.
	glDisable(GL_BLEND);
}
