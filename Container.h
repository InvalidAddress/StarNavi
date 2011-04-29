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

#ifndef ANCHOR_TYPE
#define ANCHOR_TYPE
enum anchor_type {CENTER, LEFT_UPPER, RIGHT_UPPER, RIGHT_LOWER, LEFT_LOWER};
#endif

class Container:public Drawable
{
	private:
		float originalX;
		float originalY;
		float originalW;
		float originalH;
		
		anchor_type anchor;
		
		Drawable *content;
		AbstractFunctor *act;

	public:
		Container(Drawable *d,AbstractFunctor *fn, float x, float y, float w, float h, anchor_type a = CENTER);
		~Container();
		
		Drawable* getContent();
		
		void scale(float x, float y);
		void translate(float x, float y);
		
		void setAnchor(anchor_type a);
		anchor_type getAnchor();
		
		void activate();
		bool isColliding(float x, float y);
		
		void draw();
};

#endif
