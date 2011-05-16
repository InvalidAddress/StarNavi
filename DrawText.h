//==============================================================================
// Date Created:		3 May 2011
// Last Updated:		15 May 2011
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

#ifndef TEXTALIGN
#define TEXTALIGN
enum text_align {MIDDLE, LEFT, RIGHT};
#endif

class DrawText:public Drawable
{
	protected:		
		string font_path;
		int font_size;
		
		string text;
		float aspect_ratio;
		text_align align;
		
		float text_color[4];
		float bg_color[4];
		
		float horz_padding;
		float vert_padding;
		
		GLuint rendered_text;
		GLubyte* tex_data;

	public:
		DrawText(string t, string f = "/usr/share/fonts/truetype/freefont/FreeSans.ttf", int size = 14, float x = 0, float y = 0, text_align a = MIDDLE);
		~DrawText();
		
		// Basic shape getters and setters.
		void setTextWidth(float w);
		float getTextWidth();
		
		void setTextHeight(float h);		
		float getTextHeight();
		
		float getWidth();
		float getHeight();
		
		void setHorzPadding(float h);
		float getHorzPadding();
		
		void setVertPadding(float v);
		float getVertPadding();
		
		// Font/text getters and setters
		void setFontSize(int s);
		int getFontSize();
		
		void setFont(string f);
		string getFont();
		
		void setText(string t);
		string getText();
		
		void setAlignment(text_align a);
		text_align getAlignment();
		
		// Color getters and setters
		void setTextColor(float r, float g, float b, float a);
		void setTextColor(float c[4]);
		float* getTextColor();
		
		void setBackgroundColor(float r, float g, float b, float a);
		void setBackgroundColor(float c[4]);
		float* getBackgroundColor();
		
		void initTexture();
		void refreshTexture();
		void draw();
};
#endif
