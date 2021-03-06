//==============================================================================
// Date Created:		17 April 2011
// Last Updated:		15 May 2011
//
// File name:			Drawable.cpp
// Programmer:			Matthew Hydock
//
// File description:	Methods for an abstract class. Removes a lot of
//						duplicate code, especially from container-type objects.
//==============================================================================

#include "Drawable.h"

void Drawable::setPosition(float x, float y)
// Set position. Two coordinates.
{
	setPosX(x);
	setPosY(y);
}

void Drawable::setPosX(float x)
// Set only x coord.
{
	xPos = x;
}

void Drawable::setPosY(float y)
// Set only y coord.
{
	yPos = y;
}

float Drawable::getPosX()
// Return the object's x coord.
{
	return xPos;
}

float Drawable::getPosY()
// Return the object's y coord.
{
	return yPos;
}
		

void Drawable::setWidth(float w)
// Set the object's width.
{
	width = w;
}

void Drawable::setHeight(float h)
// Set the object's height.
{
	height = h;
}

float Drawable::getWidth()
// Return the object's width.
{
	return width;
}

float Drawable::getHeight()
// Return the object's height.
{
	return height;
}

void Drawable::shiftToAnchor()
{
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
}

void Drawable::setAnchor(anchor_type a)
// Set the origin of the container's coordinate world.
{
	anchor = a;
}

anchor_type Drawable::getAnchor()
// Return the origin of the container's coordinate world.
{
	return anchor;
}

bool Drawable::getCollideFlag()
// Return the flag that knows if the object is colliding or not.
{
	return collide_flag;
}

bool Drawable::isColliding(float x, float y)
// Check to see if the given coordinate is inside the button.
{
	float w = width;
	float h = height;
	
//	cout << name << endl;
//	cout << xPos << " " << yPos << " " << w << " " << h << endl;
//	cout << x << " " << y << endl;
	
	switch (anchor)
	{
		case CENTER			:	collide_flag = (x >= xPos-w/2) && (x <= xPos+w/2) && (y >= yPos-h/2) && (y <= yPos+h/2);
								break;
		case LEFT_UPPER		:	collide_flag = (x >= xPos) && (x <= xPos+w) && (y >= yPos-h) && (y <= yPos);
								break;
		case RIGHT_UPPER	:	collide_flag = (x >= xPos-w) && (x <= xPos) && (y >= yPos-h) && (y <= yPos);
								break;
		case RIGHT_LOWER	: 	collide_flag = (x >= xPos-w) && (x <= xPos) && (y >= yPos) && (y <= yPos+h);
								break;
		case LEFT_LOWER		:	collide_flag = (x >= xPos) && (x <= xPos+w) && (y >= yPos) && (y <= yPos+h);
								break;
	}
		
	return collide_flag;
}

void Drawable::printGlError()
// Check to see if there were any gl errors
{
	switch(glGetError())
	{
		case GL_INVALID_ENUM		: cout << "invalid enum\n";
									  break;
		case GL_INVALID_VALUE		: cout << "invalid value\n";
									  break;
		case GL_INVALID_OPERATION	: cout << "invalid operation\n";
									  break;
		case GL_STACK_OVERFLOW		: cout << "stack overflow\n";
									  break;
		case GL_STACK_UNDERFLOW		: cout << "stack underflow\n";
									  break;
		case GL_OUT_OF_MEMORY		: cout << "out of memory\n";
									  break;
		case GL_TABLE_TOO_LARGE		: cout << "table too large\n";
									  break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: cout << "invalid framebuffer operation\n";
									  break;
		default						: cout << "no error\n";
	}
}

/*
void Drawable::printFramebufferError()
// Check to see if there were any framebuffer errors
{
	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER); 
	switch(e)
	{
		case GL_FRAMEBUFFER_UNSUPPORTED						: cout << "format not supported\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT	: cout << "missing attachment\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT			: cout << "incomplete attachment\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE			: cout << "incomplete multisample\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER			: cout << "missing draw buffer\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER			: cout << "missing read buffer\n";
															  break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS		: cout << "incomplete layer targets\n";
															  break;
		case GL_FRAMEBUFFER_COMPLETE						: cout << "complete\n";
															  break;
		default												: cout << "mystery error: " << e << endl;
															  break;		
	}
}*/
