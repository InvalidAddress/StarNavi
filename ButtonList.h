//==============================================================================
// Date Created:		25 April 2011
// Last Updated:		25 April 2011
//
// File name:			ButtonList.h
// Programmer:			Matthew Hydock
//
// File description:	Header for an object that contains and draws a list of
//						buttons. Manages the separation between them, and
//						checking mouse collision.
//==============================================================================

#include "Button.h"

#ifndef BUTTONLIST
#define BUTTONLIST

#define HORIZONTAL 1
#define VERTICAL 2

class ButtonList:public Drawable
{
	private:
		float lastX;
		float lastY;
			
		int orientation;
		float vert_padding;
		float horz_padding;
		
		list<Button*> *buttons;
	
	public:
		ButtonList(float x, float y, float w, float h, int o = VERTICAL, float vp = 5, float hp = 5);
		~ButtonList();
		
		void addButton(Button *b);
		list<Button*>* getButtonsList();
		Button* getButton(size_t i);
		
		void setOrientation(int o);
		int getOrientation();
		
		void setHorizPadding(float h);
		void setVertPadding(float v);
		float getHorizPadding();
		float getVertPadding();
		
		void activate();
		bool isColliding(float x, float y);
		
		void draw();
};
#endif
