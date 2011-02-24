//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		20 February 2011
//
// File name:			Drawable.h
// Programmer:			Matthew Hydock
//
// File description:	An empty class, made to make generic objects that can
//						draw to a framebuffer.
//==============================================================================

#define DRAWABLE

class Drawable
{
	public:
		virtual void draw() = 0;
		virtual ~Drawable() {};
};
