//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		20 February 2011
//
// File name:			Container.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that can contain objects that extend
//						the Drawable class. It's role is to switch the OpenGL
//						framebuffer to it's own buffer, and then make the
//						Drawable objects it contains draw to that buffer, which
//						will later be mapped as a texture on a plane.
//==============================================================================

class Container:Drawable
{
	private:
		float xPos;
		float yPos;
		Drawable content;
		//FRAMEBUFFER SHIT
	public:
		void Container(Drawable d);
}
		
