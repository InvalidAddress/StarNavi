//==============================================================================
// Date Created:		14 February 2011
// Last Updated:		28 April 2011
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
		void initTexture();
	
	public:
		Star(filenode *f);
		~Star();
		
		void setDiameter(float d);
		void setDistance(float d);
		void setAngle(float a);
		void setDepth(float d);
		
		float getDiameter();
		float getDistance();
		float getAngle();
		float getDepth();
		
		void setPosition(float a, float dis, float dep);
		void randomPosition(float a1, float a2, float dis1, float dis2, float dep1, float dep2);
		void recalc();
		
		void activate();
		bool getCollideFlag();	
		bool isColliding(float x, float y);
		void draw();
};

#endif
