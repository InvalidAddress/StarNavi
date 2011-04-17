//==============================================================================
// Date Created:		14 February 2011
// Last Updated:		17 April 2011
//
// File name:			Star.h
// Programmer:			Matthew Hydock
//
// File description:	Header to the star objects.
//==============================================================================

#include "Drawable.h"
#include "fs_structs.h"

#ifndef STAR
#define STAR

class Star:Drawable
{
	private:
		static GLuint star_texture;
		filenode *file;
		float diameter;
		float angle, distance, depth;
		float color[4];
		
		void calculateDiameter();
		void determineColor();
		void texInit();
	
	public:
		Star(filenode *f);
		~Star();
		
		void setPosition(float a, float dis, float dep);
		void randomPosition(float a1, float a2, float dis1, float dis2, float dep1, float dep2);
		void recalc();
		void draw();
		
		bool isColliding(float x, float y) { return false; };
};

#endif
