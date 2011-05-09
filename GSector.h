//==============================================================================
// Date Created:		12 March 2011
// Last Updated:		28 April 2011
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
		float arc_width;
		float radius;
		float thickness;
		
		// File representation.
		list<Star*>* stars;
		list<filenode*>* files;
		dirnode* root;
		string name;
		
		// Render to texture.
		GLuint texture;
		GLbyte* tex_data;
		int tex_size;
		
		float getMinStarDist(Star* s);
		void clearStars();
		
		bool singleSectorMode;
		
	public:
		GSector(dirnode* r, list<filenode*>* f, float ra, float b, float w, string n = "");
		~GSector();
		
		void buildStars();
		list<Star*>* getStars();
		
		void setName(string n);
		string getName();
		
		float getBiggestStarSize();
		float calcMinArcWidth();
		
		void setRadius(float r);
		void setArcBegin(float b);
		void setArcWidth(float e);
		void setThickness(float t);
		
		float getRadius();
		float getArcBegin();
		float getArcEnd();
		float getArcWidth();
		float getThickness();
		
		list<filenode*>* getFileList();
		void setDirectory(dirnode *r);
		dirnode* getDirectory();
		
		void setSingleSectorMode(bool m);
		bool isSingleSectorMode();
		
		Star* getSelected();
		
		void activate();
		bool isColliding(float x, float y);
		
		void buildMask();
		void drawMask();
		void draw();
};

#endif
