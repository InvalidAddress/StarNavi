//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		29 April 2011
//
// File name:			Container.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that can contain objects that extend
//						the Drawable class. It draws objects by building a
//						viewport of a requested size in a specified location,
//						and attaches an ortho space to it to draw in.
//==============================================================================

#include "Drawable.h"
#include "StateManager.h"

#ifndef CONTAINER
#define CONTAINER

class Container:public Drawable
{
	private:
		float originalX;
		float originalY;
		float originalW;
		float originalH;
		
		Drawable *content;
		AbstractFunctor *act;

	public:
		Container(Drawable *d,AbstractFunctor *fn, float x, float y, float w, float h, anchor_type a = CENTER);
		~Container();
		
		Drawable* getContent();
		
		float getOriginalX();
		float getOriginalY();
		
		
		void scale(float x, float y);
		void translate(float x, float y);
		
		void activate();
		bool isColliding(float x, float y);
		
		void draw();
};

#endif
