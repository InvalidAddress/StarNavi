//==============================================================================
// Date Created:		12 March 2011
// Last Updated:		17 April 2011
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

class GSector:public Drawable
{
	private:
		// Real dimensions of the sector.
		float arc_begin;
		float arc_end;
		float radius;
		float thickness;
		
		// File representation.
		list<Star*> *stars;
		list<filenode*> *files;
		dirnode *root;
		
		// Render to texture.
		GLuint texture;
		GLbyte *tex_data;
		int tex_size;
		
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
		
		list<filenode*>* getFileList();
		void setDirectory(dirnode *r);
		dirnode* getDirectory();
		
		void buildMask();
		void drawMask();
		void draw();
		
		bool isColliding(float x, float y);
};

#endif
