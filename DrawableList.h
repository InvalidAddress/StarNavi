//==============================================================================
// Date Created:		25 April 2011
// Last Updated:		15 May 2011
//
// File name:			DrawableList.h
// Programmer:			Matthew Hydock
//
// File description:	Header for a class that contains and draws a list of
//						drawables. Manages the separation between them, and for
//						checking mouse collisions.
//
//						Was originally just for buttons, but has since been
//						repurposed for all kinds of drawables.
//==============================================================================

#include "Drawable.h"

#ifndef DRAWABLELIST
#define DRAWABLELIST

#define HORIZONTAL 1
#define VERTICAL 2

class DrawableList:public Drawable
{
	protected:			
		int orientation;
		float vert_padding;
		float horz_padding;
		
		list<Drawable*>* drawables;
	
	public:
		DrawableList() {};
		DrawableList(float x, float y, float w, float h, int o = VERTICAL, float vp = 5, float hp = 5);
		~DrawableList();
		
		void addDrawable(Drawable* d);
		list<Drawable*>* getDrawablesList();
		Drawable* getDrawable(size_t i);
		
		void setWidth(float w);
		void setHeight(float h);
		
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
