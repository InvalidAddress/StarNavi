//==============================================================================
// Date Created:		14 February 2011
// Last Updated:		13 May 2011
//
// File name:			Star.h
// Programmer:			Matthew Hydock
//
// File description:	Header to the star objects.
//==============================================================================

#include "LabeledDrawable.h"
#include "fs_structs.h"

#ifndef STAR
#define STAR

class Star:public LabeledDrawable
{
	private:
		static GLuint star_texture;
		
		filenode* file;
		float radius, diameter;
		float angle, distance, depth;
		float color[4];
		
		void calculateRadius();
		void determineColor();
		void initTexture();
	
	public:
		static bool starSelectionMode;
		
		Star(filenode* f);
		~Star();
		
		void setRadius(float r);
		void setDiameter(float d);
		void setDistance(float d);
		void setAngle(float a);
		void setDepth(float d);
		
		string getName();
		float getRadius();
		float getDiameter();
		float getDistance();
		float getAngle();
		float getDepth();
		
		void setPosition(float a, float dis, float dep);
		void randomPosition(float a1, float a2, float dis1, float dis2, float dep1, float dep2);
		void recalc();
		
		void activate();
		bool isColliding(float x, float y);
		void draw();
};

#endif
