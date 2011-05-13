//==============================================================================
// Date Created:		13 May 2011
// Last Updated:		13 May 2011
//
// File name:			LabeledDrawable.h
// Programmer:			Matthew Hydock
//
// File description:	A special class of drawable items that have	built-in
//						name labels.
//==============================================================================

#include "LabeledDrawable.h"

DrawText* LabeledDrawable::getLabel()
// Get the text object of the star.
{
	return label;
}

void LabeledDrawable::initLabel()
{
	if (label == NULL)
		label = new DrawText(name);
}	

void LabeledDrawable::drawLabel()
// Draw the name of the star.
{
	glPushMatrix();
		glTranslatef(label->getPosX(),label->getPosY(),0);
		glScalef(label->getWidth()/2+5,label->getHeight()/2+4,1);
		
		glBegin(GL_QUADS);
			glColor4f(.2,.2,.2,.25);
			glVertex2d(-1,1);
			glVertex2d(-1,-1);
			glVertex2d(1,-1);
			glVertex2d(1,1);
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(0,0,1);
		label->draw();
	glPopMatrix();
}
