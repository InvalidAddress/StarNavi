//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		17 April 2011
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

class Drawable
{
	protected:		
		bool collide_flag;

	public:
	
		virtual bool isColliding(float x, float y) = 0;
		virtual void draw() = 0;
		virtual ~Drawable() {};
		
		void printGlError();
		//void printFramebufferError();
};
#endif
