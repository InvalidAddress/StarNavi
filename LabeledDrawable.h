//==============================================================================
// Date Created:		13 May 2011
// Last Updated:		16 May 2011
//
// File name:			LabeledDrawable.h
// Programmer:			Matthew Hydock
//
// File description:	Header for a special class of drawable items that have
//						built-in name labels.
//==============================================================================

#include "DrawText.h"

#ifndef LABELEDDRAWABLE
#define LABELEDDRAWABLE

class LabeledDrawable:public Drawable
{
	protected:
		DrawText* label;
		
	public:
		virtual ~LabeledDrawable() {};
		
		DrawText* getLabel();
		void initLabel();
		void drawLabel();
};
#endif
