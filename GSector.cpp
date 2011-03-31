//==============================================================================
// Date Created:		18 March 2011
// Last Updated:		23 March 2011
//
// File name:			GSector.h
// Programmer:			Matthew Hydock
//
// File description:	A class that draws a chunck of a galaxy, using a file
//						list and dimensions provided by it's patron galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================

#include "GSector.h"

GSector::GSector(dirnode *r, list<filenode*> *f, float ra, float b, float e)
// Creates a sector, with a list of files and the given dimensions. Can take a
// dirnode for hierarchical functionality, but it is not necessary.
{	
	if (r != NULL)
		setDirectory(r);
	else
	{
		root = NULL;
		files = f;
	}
		
	radius = ra;
	
	arc_begin = b;
	arc_end = e;
	
	cout << arc_begin << " , " << arc_end << endl;
	
	thickness = pow(radius*2,.5);
	
	stars = NULL;
	buildStars();
}

GSector::~GSector()
// Deletes all of the stars. Does not delete the file list or the directory node
// as they are external values, and should be dealt with externally.
{
	clearStars();
}

void GSector::buildStars()
// Erases the current star list, then creates new stars within the sector's
// physical range.
{
	clearStars();
	
	stars = new list<Star*>();
	
	for (list<filenode*>::iterator i = files->begin(); i != files->end(); i++)
	{
		Star *temp = new Star(*i);
		temp->randomPosition(arc_begin,arc_end,0,radius,-thickness,thickness);
		stars->push_back(temp);
	}
}

void GSector::clearStars()
// Erases all of the stars, and deletes the star list.
{
	if (stars != NULL)
	{
		for (list<Star*>::iterator i = stars->begin(); i != stars->end(); i++)
			delete (*i);
		delete (stars);
	}
}

//==============================================================================
// Methods that work with the dimensions of the sector.
//==============================================================================
void GSector::setRadius(float r)
{
	radius = r;
}

void GSector::setArcBegin(float b)
{
	arc_begin = b;
}

void GSector::SetArcEnd(float e)
{
	arc_end = e;
}

float GSector::getRadius()
{
	return radius;
}

float GSector::getArcBegin()
{
	return arc_begin;
}

float GSector::getArcEnd()
{
	return arc_end;
}
//==============================================================================
void GSector::setDirectory(dirnode *r)
// Set the sector's root directory.
{
	root = r;
	files = &(r->all_files);
}

dirnode* GSector::getDirectory()
// Obtain the sector's root directory.
{
	return root;
}
			
void GSector::drawOutline()
// Accentuate the sector, by darkening the rest of the galaxy, and drawing a 
// glowing outline around the sector.
{
	glBegin(GL_TRIANGLES);
		glColor4d(0,0,0,.75);
		float outer = 360-(arc_end-arc_begin);
		float i = arc_end;
		for (float j = i+10; j <= arc_end+outer; j += 10)
		{
			glVertex3d(0,0,1);
			glVertex3d((radius+50)*cos(i),(radius+50)*sin(i),1);
			glVertex3d((radius+50)*cos(j),(radius+50)*sin(j),1);
			i = j;
		}
		glVertex3d(0,0,1);
		glVertex3d((radius+50)*cos(i),(radius+50)*sin(i),1);
		glVertex3d((radius+50)*cos(arc_end+outer),(radius+50)*sin(arc_end+outer),1);
	glEnd();
	
	
}

void GSector::draw()
{
	for (list<Star*>::iterator i = stars->begin(); i != stars->end(); i++)
		(*i)->draw();
}
