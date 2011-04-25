//==============================================================================
// Date Created:		20 April 2011
// Last Updated:		20 April 2011
//
// File name:			Button.h
// Programmer:			Matthew Hydock
//
// File description:	GUI component to draw a button to screen. Can be static
//						or interactive and calls a functor.
//==============================================================================

#include "Drawable.h"
#include <FTGL/ftgl.h>

#ifndef BUTTON
#define BUTTON

class Button:public Drawable
{
	private:
		bool interactive;
		
		string label;
		FTFont *buffer;
		
		AbstractFunctor *act;
		
	public:
		Button(string l, AbstractFunctor *f, float x, float y, float w, float h, bool active = true);
		~Button();
		
		bool isColliding(float x, float y);
		bool isInteractive();
		void setInteract(bool c);
		
		void setAction(AbstractFunctor *f);
		void activate();
		
		void drawText(char *input, int x, int y, void *font);
		void draw();
};
#endif
