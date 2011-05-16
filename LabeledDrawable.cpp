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
	{
		label = new DrawText(name);
		label->setBackgroundColor(.2,.2,.2,.25);
		label->setHorzPadding(10);
		label->setVertPadding(8);
	}
}	

void LabeledDrawable::drawLabel()
// Draw the name of the star.
{
	label->draw();
}
