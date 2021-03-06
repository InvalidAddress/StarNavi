//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		15 May 2011
//
// File name:			Drawable.h
// Programmer:			Matthew Hydock
//
// File description:	An abstract class, made to make generic objects that can
//						draw to a framebuffer.
//==============================================================================

#define GL_GLEXT_PROTOTYPES

#include "global_header.h"
#include <GL/gl.h>
#include <GL/glu.h>

#define ILUT_USE_OPENGL
#include <IL/ilut.h>


#ifndef DRAWABLE
#define DRAWABLE

#ifndef ANCHOR_TYPE
#define ANCHOR_TYPE
enum anchor_type {CENTER, LEFT_UPPER, RIGHT_UPPER, RIGHT_LOWER, LEFT_LOWER};
#endif

class Drawable
{
	protected:
		float xPos;
		float yPos;
		float width;
		float height;
	
		string name;
	
		anchor_type anchor;
		
		bool collide_flag;

	public:
		virtual void activate() {};
		virtual bool isColliding(float x, float y);
		virtual void draw() = 0;
		virtual ~Drawable() {};
		
		virtual void setPosition(float x, float y);
		virtual void setPosX(float x);
		virtual void setPosY(float y);
		virtual float getPosX();
		virtual float getPosY();
		
		virtual void setWidth(float w);
		virtual void setHeight(float h);
		virtual float getWidth();
		virtual float getHeight();
		
		virtual void shiftToAnchor();
		virtual void setAnchor(anchor_type a);
		virtual anchor_type getAnchor();
		
		virtual bool getCollideFlag();

		void printGlError();
		//void printFramebufferError();
};
#endif
