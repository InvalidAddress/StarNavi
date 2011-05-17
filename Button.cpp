//==============================================================================
// Date Created:		23 April 2011
// Last Updated:		16 May 2011
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
	name = l;
	act = f;
	
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	
	anchor = CENTER;
	
	interactive = active;
	text = new DrawText(name);
	
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
//	cout << "button activated\n";
	act->Call();
}

void Button::draw()
// Draw a rectangle with a gradient, dark on the far left and right, white in
// the middle. Try to center the text.
{
	// Turn on blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	glPushMatrix();
		shiftToAnchor();
		glTranslatef(xPos,yPos,0);
		
		// Draw the slightly larger rectangle that will become the outline.
		glPushMatrix();
			glScalef(width/2,height/2,1);
		
			glBegin(GL_QUADS);
				glColor4f(.1,.1,.1,1);
				glVertex2d(-1,1);
				glVertex2d(-1,-1);
				glVertex2d(1,-1);
				glVertex2d(1,1);
			glEnd();
		glPopMatrix();
		// Done drawing outline.
	
		// Draw the gradient that will be the button.
		glPushMatrix();
			glTranslatef(0,0,1);
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
		// Done drawing the button.

		// Draw the text. This takes up two layers.
		glPushMatrix();
			glTranslatef(0,0,2);
			text->draw();
		glPopMatrix();
		// Done drawing text.
	
		// If mousing over a button, draw a shadow.
		if (collide_flag)
		{
			glPushMatrix();
				glTranslatef(0,0,4);
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
	glPopMatrix();
	
	// Turn off blending.
	glDisable(GL_BLEND);
}
