//==============================================================================
// Date Created:		23 April 2011
// Last Updated:		3 May 2011
//
// File name:			Button.cpp
// Programmer:			Matthew Hydock
//
// File description:	GUI component to draw a button to screen. Can be static
//						or interactive, and calls a functor when clicked.
//==============================================================================

#include "Button.h"

Button::Button(string l, AbstractFunctor* f, float x, float y, float w, float h, bool active)
// Make a button with a label l, that performs action f when clicked.
{
	label = l;
	act = f;
	
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	
	interactive = active;
	text = new DrawText(label);
	
	text->setPosition(xPos,yPos);
}

Button::~Button()
{
}


bool Button::isInteractive()
// Is the button clickable?
{
	return interactive;
}

void Button::setInteract(bool c)
// Set whether the button is clickable or not.
{
	interactive = c;
}

		
void Button::setAction(AbstractFunctor* f)
// Set the button's action.
{
	act = f;
}

void Button::activate()
// Perform the button's assigned action.
{
	act->Call();
}


bool Button::isColliding(float x, float y)
// Check to see if the given coordinate is inside the button.
{
	float w = width/2;
	float h = height/2;
	
	collide_flag = (x >= xPos-w) && (x <= xPos+w) && (y >= yPos-h) && (y <= yPos+h);

//	if (collide_flag)
//		cout << "colliding with button " << label << endl;
		
	return collide_flag;
}	


void Button::draw()
// Draw a rectangle with a gradient, dark on the far left and right, white in
// the middle. Try to center the text.
{
	// Turn on blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	glPushMatrix();
		glTranslatef(xPos,yPos,0);
		glScalef(width/2,height/2,1);
		
		glBegin(GL_QUADS);
			glColor4f(.1,.1,.1,1);
			glVertex2d(-1,1);
			glVertex2d(-1,-1);
			glVertex2d(1,-1);
			glVertex2d(1,1);
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(xPos,yPos,1);
		glScalef(width/2-2,height/2-2,1);

		glBegin(GL_QUADS);
			glColor4f(.15,.15,.15,0);
			glVertex2d(-1,1);
			glVertex2d(-1,-1);
			glColor4f(.25,.25,.25,1);
			glVertex2d(0,-1);
			glVertex2d(0,1);

			glColor4f(.25,.25,.25,1);
			glVertex2d(0,1);
			glVertex2d(0,-1);
			glColor4f(.15,.15,.15,0);
			glVertex2d(1,-1);
			glVertex2d(1,1);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,0,2);
		text->draw();
	glPopMatrix();
	
	if (collide_flag)
	{
		glPushMatrix();
			glTranslatef(xPos,yPos,3);
			glScalef(width/2,height/2,1);
		
			glBegin(GL_QUADS);
				glColor4f(0,0,0,.4);
				glVertex2d(-1,1);
				glVertex2d(-1,-1);
				glVertex2d(1,-1);
				glVertex2d(1,1);
			glEnd();
		glPopMatrix();
	}
	
	// Turn off blending.
	glDisable(GL_BLEND);
}
