//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		5 March 2011
//
// File name:			Drawable.h
// Programmer:			Matthew Hydock
//
// File description:	An empty class, made to make generic objects that can
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
	public:
		virtual void draw() = 0;
		virtual ~Drawable() {};
};

#endif
