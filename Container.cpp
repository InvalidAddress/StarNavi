//==============================================================================
// Date Created:		6 April 2011
// Last Updated:		25 April 2011
//
// File name:			Container.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that can contain objects that extend
//						the Container class. It draws objects by building a
//						viewport of a requested size in a specified location,
//						and attaches an ortho space to it to draw in.
//==============================================================================

#include "Container.h"

Container::Container(Drawable *d,AbstractFunctor *fn, float x, float y, float w, float h, anchor_type a)
{
	act = NULL;
	
	content = d;
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	anchor = a;
	
	act = fn;
	
	originalX = x;
	originalY = y;
	originalW = w;
	originalH = h;
}

Container::~Container()
{
	delete(content);
}

	
Drawable* Container::getContent()
{
	return content;
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

void Container::scale(float x, float y)
// Scale the container's width from the original width, based on some ratios for
// x and y scaling.
{
	width = originalW * x;
	height = originalH * y;
}


void Container::translate(float x, float y)
// Shift the container's position by x and y, based on the original position.
{
	xPos = originalX + x;
	yPos = originalY + y;
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
	
	glBegin(GL_LINES);
		glColor3d(1,1,1);
		
		glVertex2d(-(float)width/2.0,(float)height/2.0);
		glVertex2d((float)width/2.0,(float)height/2.0);
		
		glVertex2d((float)width/2.0,(float)height/2.0);
		glVertex2d((float)width/2.0,-(float)height/2.0);
		
		glVertex2d((float)width/2.0,-(float)height/2.0);
		glVertex2d(-(float)width/2.0,-(float)height/2.0);
		
		glVertex2d(-(float)width/2.0,-(float)height/2.0);
		glVertex2d(-(float)width/2.0,(float)height/2.0);
	glEnd();
	
	content->draw();
	
	collide_flag = false;
}

void Container::activate()
{
	if (act != NULL)
		act->Call();
}
