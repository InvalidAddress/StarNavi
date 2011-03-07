//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		5 March 2011
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

#include "Drawable.h"
#include "Indexer.h"
#include "Star.h"

#ifndef GALAXY
#define GALAXY

class Galaxy:Drawable
{
	private:
		float rotX;
		float rotY;
		float rotZ;
		float rotSpeed;
		list<Star*> *stars;
		Indexer *indexer;
		
		GLuint buff;
		GLuint buff_tex;
		GLbyte *tex_data;
		int width, height;
		
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
		void refreshTex();
		void draw();
};

#endif
