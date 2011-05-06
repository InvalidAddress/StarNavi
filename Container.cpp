//==============================================================================
// Date Created:		6 April 2011
// Last Updated:		3 May 2011
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


float Container::getOriginalX()
{
	return originalX;
}

float Container::getOriginalY()
{
	return originalY;
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


void Container::activate()
// Call the container's functor.
{
	if (act != NULL)
		act->Call();
}
			
bool Container::isColliding(float x, float y)
// Check if mouse is colliding with container. If so, set flag, and see if the
// mouse is also colliding with the content.
{
	collide_flag = (x >= xPos) && (x <= xPos+width) && (y >= yPos) && (y <= yPos+height);
	
	float localX = x-xPos;
	float localY = y-yPos;
	
	switch(anchor)
	{
		case CENTER			: localX -= width/2;
							  localY -= height/2;
							  break;
		case LEFT_UPPER		: localX -= 0;
							  localY -= height;
							  break;
		case RIGHT_UPPER	: localX -= width;
							  localY -= height;
							  break;
		case RIGHT_LOWER	: localX -= width;
							  localY -= 0;
							  break;
		case LEFT_LOWER		: localX -= 0;
							  localY -= 0;
							  break;
	}
	
	if (content != NULL)
		content->isColliding(localX,localY);
		
	return collide_flag;
}


void Container::draw()
// Create the new viewport, set the world mode, and draw the contained object.
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch(anchor)
	{
		case CENTER			: glOrtho(-width/2,width/2,-height/2,height/2,-100,100);
							  break;
		case LEFT_UPPER		: glOrtho(0,width,-height,0,-100,100);
							  break;
		case RIGHT_UPPER	: glOrtho(-width,0,-height,0,-100,100);
							  break;
		case RIGHT_LOWER	: glOrtho(-width,0,0,height,-100,100);
							  break;
		case LEFT_LOWER		: glOrtho(0,width,0,height,-100,100);
							  break;
	}	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glViewport(xPos,yPos,width,height);
	
	glPushMatrix();
		switch(anchor)
		{
			case CENTER			: break;
			case LEFT_UPPER		: glTranslatef(width/2,-height/2,0);
								  break;
			case RIGHT_UPPER	: glTranslatef(-width/2,-height/2,0);
								  break;
			case RIGHT_LOWER	: glTranslatef(-width/2,height/2,0);
								  break;
			case LEFT_LOWER		: glTranslatef(width/2,height/2,0);
								  break;
		}
		glScalef(width,height,1);
		
		glBegin(GL_LINES);		
			glColor3d(1,1,1);
			
			glVertex3d(-.5,.5,100);
			glVertex3d(.5,.5,100);
			
			glVertex3d(.5,.5,100);
			glVertex3d(.5,-.5,100);
			
			glVertex3d(.5,-.5,100);
			glVertex3d(-.5,-.5,100);
			
			glVertex3d(-.5,-.5,100);
			glVertex3d(-.5,.5,100);
		glEnd();
	glPopMatrix();
	
	if (content != NULL)
		content->draw();
	
	collide_flag = false;
}
