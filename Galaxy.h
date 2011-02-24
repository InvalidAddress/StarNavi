//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		20 February 2011
//
// File name:			Galaxy.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that draws the files indexed by an
//						Indexer as Stars in a galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================

#include "Indexer.h"
#include "Star.h"

#ifndef DRAWABLE
#include "Drawable.h"
#endif

class Galaxy:Drawable
{
	private:
		float rotX;
		float rotY;
		float rotZ;
		float rotSpeed;
		list<Star*> stars;
		Indexer *indexer;
	public:
		Galaxy(string s);
		~Galaxy();
		
		float getRotationX();
		float getRotationY();
		float getRotationSpeed();
		Indexer* getIndexer();
		
		void setRotation(float x, float y);
		void setRotationSpeed(float s);
		void setDirectory(string d);
		void draw();
};
