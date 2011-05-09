//==============================================================================
// Date Created:		3 May 2011
// Last Updated:		3 May 2011
//
// File name:			DrawText.h
// Programmer:			Matthew Hydock
//
// File description:	Header for a class that renders text to a texture.
//==============================================================================

#include <SDL/SDL_ttf.h>
#include "Drawable.h"

#ifndef DRAWTEXT
#define DRAWTEXT

class DrawText:public Drawable
{
	private:
		string font_path;
		string text;
		int size;
		
		float aspect_ratio;
		
		GLuint rendered_text;
		GLubyte* tex_data;

		
	public:
		DrawText(string t, string f = "/usr/share/fonts/truetype/freefont/FreeSans.ttf", int size = 14, float x = 0, float y = 0, anchor_type a = CENTER);
		~DrawText();
		
		void setWidth(float w);
		void setHeight(float h);		
		
		void setFontSize(int s);
		int getFontSize();
		
		void setFont(string f);
		string getFont();
		
		void setText(string t);
		string getText();
		
		bool isColliding(float x, float y) {return false;};
		
		void initTexture();
		void refreshTexture();
		void draw();
};
#endif
