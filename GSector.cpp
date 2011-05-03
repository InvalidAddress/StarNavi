//==============================================================================
// Date Created:		18 March 2011
// Last Updated:		3 May 2011
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

GSector::GSector(dirnode *r, list<filenode*> *f, float ra, float b, float w, string n)
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
	
	if (root != NULL && n == "")
		name = root->name;
	else
		name = n;
	
	singleSectorMode = false;
	
	radius = ra;
	
	arc_begin = b;
	arc_width = w;
	
//	cout << arc_begin << " , " << arc_width << endl;
	
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
		temp->randomPosition(getArcBegin(),getArcEnd(),0,radius,-thickness,thickness);
		if (temp->getDistance()+(temp->getDiameter()/2) > radius)
			temp->setDistance(radius-(temp->getDiameter()/2));
		stars->push_back(temp);
	}
}

float GSector::getMinStarDist(Star *s)
{	
	// Check if the chord length at this star's distance is long enough to
	// accomodate the star's diameter.
	float width = getArcWidth()*(M_PI/180);
	float chord = 2*sin(width/2);
	float lower_bound = s->getDiameter()/chord;

	return lower_bound;
}

void GSector::clearStars()
// Erases all of the stars, and deletes the star list.
{
	if (stars != NULL)
	{
		for (list<Star*>::iterator i = stars->begin(); i != stars->end(); i++)
			stars->erase(i);
		delete (stars);
	}
}


void GSector::setName(string n)
{
	name = n;
}

string GSector::getName()
{
	return name;
}

//==============================================================================
// Methods that work with the dimensions of the sector.
//==============================================================================
float GSector::getBiggestStarSize()
// Get the diameter of the largest star in the sector.
{
	float d = 0;
	
	for (list<Star*>::iterator i = stars->begin(); i != stars->end(); i++)
		if ((*i)->getDiameter() > d)
			d = (*i)->getDiameter();
	
	return d;
}

float GSector::calcMinArcWidth()
// Returns the smallest possible arc width, in degrees.
{
	float d = getBiggestStarSize();		// get the size of the largest star,
										// which will act as a chord.
	float crd = d/radius;				// normalize the length of the chord.
	
	float theta = asin(crd/2)*2;
	theta *= 180.0/M_PI;
	
	return theta;
}

void GSector::setRadius(float r)
{
	radius = r;
}

void GSector::setArcBegin(float b)
{
	arc_begin = b;
}

void GSector::setArcWidth(float w)
{
	arc_width = w;
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
	return arc_begin+arc_width;
}

float GSector::getArcWidth()
{
	return arc_width;
}
//==============================================================================


//==============================================================================
// File node related methods.
//==============================================================================
list<filenode*>* GSector::getFileList()
// Return the sector's file list. Use this only if getDirectory returns NULL.
{
	return files;
}

void GSector::setDirectory(dirnode *r)
// Set the sector's root directory.
{
	root = r;
	files = &(r->all_files);
}

dirnode* GSector::getDirectory()
// Obtain the sector's root directory.
{
	if (root == NULL) cout << "sector root is null!\n";
	return root;
}
//==============================================================================


//==============================================================================
// Methods for user interactions.
//==============================================================================
void GSector::setSingleSectorMode(bool m)
{
	singleSectorMode = m;
}

bool GSector::isSingleSectorMode()
{
	return singleSectorMode;
}

void GSector::activate()
{
	list<Star*> active;
	for (list<Star*>::iterator i = stars->begin(); i != stars->end(); i++)
		if ((*i)->getCollideFlag())
			active.push_back(*i);
			
	Star *curr;
	float near = -100;
	for (list<Star*>::iterator i = active.begin(); i != active.end(); i++)
	{
		if ((*i)->getDepth() > near)
		{
			curr = *i;
			near = (*i)->getDepth();
		}
	}
	
	curr->activate();
}

bool GSector::isColliding(float x, float y)
// X and Y are actually polar coordinates in this case.
{
	if (singleSectorMode)
	{
		float t_x = y*radius*cos(x*(M_PI/180));
		float t_y = y*radius*sin(x*(M_PI/180));
	
//		cout << t_x << ", " << t_y << endl;
	
		for (list<Star*>::iterator i = stars->begin(); i != stars->end(); i++)
		{
			float s_x = (*i)->getDistance()*cos((*i)->getAngle()*(M_PI/180));
			float s_y = (*i)->getDistance()*sin((*i)->getAngle()*(M_PI/180));
		
			float r_x = t_x-s_x;
			float r_y = t_y-s_y;
//			cout << r_x << ", " << r_y << endl;
		
			(*i)->isColliding(0,sqrt(pow(r_x,2.0)+pow(r_y,2.0)));
		}
	}
	
	return collide_flag = (x >= getArcBegin()) && (x <= getArcEnd()) && (y <= 1.0);
}
//==============================================================================


//==============================================================================
// Drawing methods.
//==============================================================================		
void GSector::drawMask()
// Accentuate the sector, by darkening the rest of the galaxy, and drawing a 
// glowing outline around the sector.
{
	glBegin(GL_TRIANGLES);
		float outer = 360-abs(arc_width);
		float arc_end = getArcEnd();
		float i = arc_end;
		for (float j = i+1; j < arc_end+outer; j += 1)
		{
			glColor4d(.2,.2,.2,.5);
			glVertex2d(0,0);
			glColor4d(0,0,0,.5);

			glVertex2d(cos(i*M_PI/180.0),sin(i*M_PI/180.0));
			glVertex2d(cos(j*M_PI/180.0),sin(j*M_PI/180.0));
			
			i = j;
		}
		
		glColor4d(.2,.2,.2,.5);
		glVertex2d(0,0);
		glColor4d(0,0,0,.5);

		glVertex2d(cos(i*M_PI/180.0),sin(i*M_PI/180.0));
		glVertex2d(cos(arc_begin*M_PI/180.0),sin(arc_begin*M_PI/180.0));
	glEnd();
}

void GSector::draw()
{
	for (list<Star*>::iterator i = stars->begin(); i != stars->end(); i++)
		(*i)->draw();
}
//==============================================================================	
