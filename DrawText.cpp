//==============================================================================
// Date Created:		3 May 2011
// Last Updated:		13 May 2011
//
// File name:			DrawText.h
// Programmer:			Matthew Hydock
//
// File description:	Header for a class that renders text to a texture.
//==============================================================================

#include "DrawText.h"

DrawText::DrawText(string t,string f,int s,float x,float y,text_align a)
{
	xPos = x;
	yPos = y;

	font_path	= f;
	font_size	= s;
	
	text		= t;
	align		= a;
	
	setTextColor(1,1,1,1);
	setBackgroundColor(0,0,0,0);
	
	horz_padding = 0;
	vert_padding = 0;
	
	anchor = CENTER;

	rendered_text = 0;

	initTexture();
	refreshTexture();
}
	
DrawText::~DrawText()
{
}

//==============================================================================
// Basic setters and getters.
//==============================================================================
void DrawText::setTextWidth(float w)
{
	width = w;
	height = w/aspect_ratio;
}

float DrawText::getTextWidth()
{
	return width;
}

void DrawText::setTextHeight(float h)
{
	height = h;
	width = h*aspect_ratio;
}

float DrawText::getTextHeight()
{
	return height;
}

float DrawText::getWidth()
{
	return width+horz_padding;
}

float DrawText::getHeight()
{
	return height+vert_padding;
}

void DrawText::setHorzPadding(float h)
{
	horz_padding = h;
}

float DrawText::getHorzPadding()
{
	return horz_padding;
}
		
void DrawText::setVertPadding(float v)
{
	vert_padding = v;
}

float DrawText::getVertPadding()
{
	return vert_padding;
}
//==============================================================================


//==============================================================================
// Font/text methods.
//==============================================================================
void DrawText::setFontSize(int s)
{
	font_size = s;
}

int DrawText::getFontSize()
{
	return font_size;
}


void DrawText::setFont(string f)
{
	font_path = f;
}

string DrawText::getFont()
{
	return font_path;
}


void DrawText::setText(string t)
{
	text = t;
}

string DrawText::getText()
{
	return text;
}

void DrawText::setAlignment(text_align a)
{
	align = a;
}

text_align DrawText::getAlignment()
{
	return align;
}
//==============================================================================


//==============================================================================
// Color methods
//==============================================================================
void DrawText::setTextColor(float r, float g, float b, float a)
{
	text_color[0] = r;
	text_color[1] = g;
	text_color[2] = b;
	text_color[3] = a;
}

void DrawText::setTextColor(float c[4])
{
	setTextColor(c[0],c[1],c[2],c[3]);
}

float* DrawText::getTextColor()
{
	return text_color;
}

void DrawText::setBackgroundColor(float r, float g, float b, float a)
{
	bg_color[0] = r;
	bg_color[1] = g;
	bg_color[2] = b;
	bg_color[3] = a;
}

void DrawText::setBackgroundColor(float c[4])
{
	setBackgroundColor(c[0],c[1],c[2],c[3]);
}

float* DrawText::getBackgroundColor()
{
	return bg_color;
}
//==============================================================================


//==============================================================================
// Methods for drawing.
//==============================================================================
void DrawText::initTexture()
// Initialize the object's texture.
{
	//Generate an OpenGL 2D texture from the SDL_Surface*
	glGenTextures(1, &rendered_text);	
	glBindTexture(GL_TEXTURE_2D, rendered_text);
//------------------------------------------------------------------------------
// Set the state of the current texture
//------------------------------------------------------------------------------
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//------------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawText::refreshTexture()
// Using the stored font and text, build a texture.
{
	// Make SDL surfaces to render text to.
	SDL_Surface* initial = NULL;
	
	// Open the font.
	TTF_Font* font = TTF_OpenFont(font_path.c_str(),font_size);
	SDL_Color fg_color = {255*text_color[0],255*text_color[1],255*text_color[2],255*text_color[3]};
	
	// Make the initial text buffer.	
	initial = TTF_RenderText_Blended(font, text.c_str(), fg_color);
	TTF_CloseFont(font);

	// Store the buffer's width and height.
	tex_data = (GLubyte*)(initial->pixels);
	width	= initial->w;
	height	= initial->h;

	// To maintain the text's shape while drawing.
	aspect_ratio = (float)width/(float)height;

	// Bind the texture, set the its pixel map, then release.
	glBindTexture(GL_TEXTURE_2D, rendered_text);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, tex_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Free the surfaces
	SDL_FreeSurface(initial);
		
//	printGlError();
}
		
void DrawText::draw()
{
	// Draw the solid background.
	glPushMatrix();
		glTranslatef(xPos,yPos,0);
		glScalef((width+horz_padding)/2,(height+vert_padding)/2,1);
		
		glBegin(GL_QUADS);
			glColor4fv(bg_color);
			glVertex2d(-1,1);
			glVertex2d(-1,-1);
			glVertex2d(1,-1);
			glVertex2d(1,1);
		glEnd();
	glPopMatrix();
	// Done drawing background.

	// Turn texture mode.
	glEnable(GL_TEXTURE_2D);	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glPushMatrix();
		switch(align)
		{
			case MIDDLE			: break;
			case LEFT			: glTranslatef(width/2,0,0);
								  break;
			case RIGHT			: glTranslatef(-width/2,0,0);
								  break;
		}
		
		glTranslatef(xPos,yPos,1);
		glScalef(width/2, height/2,1);
		
		glBindTexture(GL_TEXTURE_2D, rendered_text);
		glBegin(GL_QUADS);
			glColor4f(1,1,1,1);
			glTexCoord2f(0,0);	glVertex2d(-1,1);
			glTexCoord2f(0,1);	glVertex2d(-1,-1);
			glTexCoord2f(1,1);	glVertex2d(1,-1);
			glTexCoord2f(1,0);	glVertex2d(1,1);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}
//==============================================================================
