//==============================================================================
// Date Created:		6 April 2011
// Last Updated:		17 April 2011
//
// File name:			Container.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that can contain objects that extend
//						the Drawable class. It draws objects by building a
//						viewport of a requested size in a specified location,
//						and attaches an ortho space to it to draw in.
//==============================================================================

#include "Container.h"

Container::Container(Drawable *d,AbstractFunctor *fn, float x, float y, float w, float h, anchor_type a)
{
	content = d;
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	anchor = a;
	
	action = fn;
}

Container::~Container()
{
	delete(content);
}

	
Drawable* Container::getContent()
{
	return content;
}
	
void Container::setPos(float x, float y)
// Set both the x and y position of the container.
{
	setPosX(x);
	setPosY(y);
}

void Container::setPosX(float x)
// Set the x-position of the container's viewport.
{
	xPos = x;
}

void Container::setPosY(float y)
// Set the y-position of the container's viewport.
{
	yPos = y;
}

float Container::getPosX()
// Return the x-position of the container's viewport.
{
	return xPos;
}

float Container::getPosY()
// Return the y-position of the container's viewport.
{
	return yPos;
}
		
		
void Container::setWidth(float w)
// Set the width of the container.
{
	width = w;
}

void Container::setHeight(float h)
// Set the height of the container.
{
	height = h;
}

float Container::getWidth()
// Return the width of the container.
{
	return width;
}

float Container::getHeight()
// Return the height of the container.
{
	return height;
}

	
void Container::setAnchor(anchor_type a)
// Set the origin of the container's coordinate world.
{
	anchor = a;
}

anchor_type Container::getAnchor()
// Return the origin of the container's coordinate world.
{
	return anchor;
}

		
bool Container::isColliding(float x, float y)
// Check if mouse is colliding with container. If so, set flag, and see if the
// mouse is also colliding with the content.
{
	collide_flag = (x >= xPos) && (x <= xPos+width) && (y >= yPos) && (y <= yPos+height);
	
	if (collide_flag)
		content->isColliding(x-xPos,y-yPos);
		
	return collide_flag;
}

void Container::draw()
// Create the new viewport, set the world mode, and draw the contained object.
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch(anchor)
	{
		case CENTER			: gluOrtho2D(-width/2,width/2,-height/2,height/2);
							  break;
		case LEFT_UPPER		: gluOrtho2D(0,width,-height,0);
							  break;
		case RIGHT_UPPER	: gluOrtho2D(-width,0,-height,0);
							  break;
		case RIGHT_LOWER	: gluOrtho2D(-width,0,0,height);
							  break;
		case LEFT_LOWER		: gluOrtho2D(0,width,0,height);
							  break;
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glViewport(xPos,yPos,width,height);
	
	content->draw();
	
	collide_flag = false;
}
