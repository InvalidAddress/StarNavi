//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		21 February 2011
//
// File name:			Galaxy.h
// Programmer:			Matthew Hydock
//
// File description:	A class that draws the files indexed by an Indexer as
//						Stars in a galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================

#include "Galaxy.h"

Galaxy::Galaxy(string s)
{
	setDirectory(s);
	
	setRotation(0, 0);
	setRotationSpeed(2);
	
	rotZ = 0;
}

Galaxy::~Galaxy()
{
	stars.clear();
	delete(indexer);
}

float Galaxy::getRotationX()
{
	return rotX;
}

float Galaxy::getRotationY()
{
	return rotY;
}

float Galaxy::getRotationSpeed()
{
	return rotSpeed;
}

Indexer* Galaxy::getIndexer()
{
	return indexer;
}
	
void Galaxy::setRotation(float x, float y)
{
	rotX = x;
	rotY = y;
}

void Galaxy::setRotationSpeed(float s)
{
	rotSpeed = s;
}

void Galaxy::setDirectory(string d)
{
	if (!stars.empty()) stars.clear();
	if (indexer != NULL) delete(indexer);
	
	indexer = new Indexer(d);
	indexer->build();
	
	list<filenode*> *files = indexer->getFileList();

	for (list<filenode*>::iterator i = files->begin(); i != files->end(); i++)
	{
		Star *temp = new Star(*i);
		temp->randomPosition(0,360,0,240,-10,10);
		stars.push_back(temp);
		cout << "|";
	}	
}

void Galaxy::draw()
{
	cout << "drawing a galaxy\n";
	
	glPushMatrix();
		glRotatef(rotX,1,0,0);
		glRotatef(rotY,0,1,0);
		glRotatef(rotZ,0,0,1);
	
		int n = 1;
		int size = stars.size();
		for (list<Star*>::iterator i = stars.begin(); i != stars.end(); i++)
		{
			cout << "drawing star " << n << " of " << size << ".\n";
			(*i)->draw();
			n++;
		}
			
		rotZ += rotSpeed;
		if (rotZ > 360) rotZ -= 360;
		
		glFlush();
	glPopMatrix();
}
