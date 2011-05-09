#include "StatusBar.h"

StatusBar::StatusBar(StateManager* sm)
{
	state = sm;

	curr = state->getCurrent();
	
	string n = "" + curr->getDirectory()->all_files.size();
	
	directory = new DrawText(curr->getName());
	num_files = new DrawText(n);
	
	num_files->setAnchor(LEFT_UPPER);
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
		cout << "dir is null, using filelist\n";
		num = curr->getFileList()->size();
		cout << "got num files: " << num << endl;
	}
	else
	{
		cout << "dir is " << curr->getDirectory()->name << endl;
		num = curr->getDirectory()->all_files.size();
		cout << "got num files: " << num << endl;
	}

	oss << num;
	
	directory->setText(curr->getName());
	num_files->setText(oss.str());
	
//	num_files->setAnchor(LEFT_UPPER);

	directory->refreshTexture();
	num_files->refreshTexture();
	
	cout << "rebuilt the status bar\n";
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
	num_files->setPosition((float)p[2]/2,0);
	
	directory->draw();
	num_files->draw();
	
	// Turn off blending.
	glDisable(GL_BLEND);
}
