//==============================================================================
// Date Created:		12 March 2011
// Last Updated:		21 March 2011
//
// File name:			GSector.h
// Programmer:			Matthew Hydock
//
// File description:	A class that draws a chunck of a galaxy, using a file
//						list and dimensions provided by it's patron galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================


#include "Star.h"

#ifndef GSECTOR
#define GSECTOR

class GSector:Drawable
{
	private:
		float arc_begin;
		float arc_end;
		float radius;
		float thickness;
		
		list<Star*> *stars;
		list<filenode*> *files;
		dirnode *root;
		
		void clearStars();
		
	public:
		GSector(dirnode *r, list<filenode*> *f, float ra, float b, float e);
		~GSector();
		void buildStars();
		
		void setRadius(float r);
		void setArcBegin(float b);
		void SetArcEnd(float e);
		
		float getRadius();
		float getArcBegin();
		float getArcEnd();
		
		void setDirectory(dirnode *r);
		dirnode* getDirectory();
		
		void draw();
		void drawOutline();
};

#endif
