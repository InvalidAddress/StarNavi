//==============================================================================
// Date Created:		23 April 2011
// Last Updated:		28 April 2011
//
// File name:			Button.cpp
// Programmer:			Matthew Hydock
//
// File description:	GUI component to draw a button to screen. Can be static
//						or interactive, and calls a functor when clicked.
//==============================================================================

#include "Button.h"

Button::Button(string l, AbstractFunctor *f, float x, float y, float w, float h, bool active)
// Make a button with a label l, that performs action f when clicked.
{
	label = l;
	act = f;
	
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	
	interactive = active;
	buildText();
}

Button::~Button()
{
}
		
void Button::buildText()
{
	TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf",12);
	SDL_Color Color = {255,255,255,255};
	buffer = TTF_RenderText_Blended(const_cast<TTF_Font*>(font), label.c_str(), Color);

	text_w = buffer->w;
	text_h = buffer->h;
	rendered_text = 0;
/*
	cout << text_w << ", " << text_h << endl;
	for (int i = 0; i < (int)(buffer->h)*(int)(buffer->w); i++)
			cout << ((int *)buffer->pixels)[i];
	cout << endl;
*/
	//Generate an OpenGL 2D texture from the SDL_Surface*
	glGenTextures(1, &rendered_text);
	glBindTexture(GL_TEXTURE_2D, rendered_text);
//-----------------------------------------------------------------------------
// Set the state of the current texture
//------------------------------------------------------------------------------
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//------------------------------------------------------------------------------
 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, text_w, text_h, 0, GL_BGR, GL_UNSIGNED_BYTE, buffer->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}


bool Button::isInteractive()
// Is the button clickable?
{
	return interactive;
}

void Button::setInteract(bool c)
// Set whether the button is clickable or not.
{
	interactive = c;
}

		
void Button::setAction(AbstractFunctor *f)
// Set the button's action.
{
	act = f;
}

void Button::activate()
// Perform the button's assigned action.
{
	act->Call();
}


bool Button::isColliding(float x, float y)
// Check to see if the given coordinate is inside the button.
{
	float w = width/2;
	float h = height/2;
	
	collide_flag = (x >= xPos-w) && (x <= xPos+w) && (y >= yPos-h) && (y <= yPos+h);

	if (collide_flag)
	{
		cout << "colliding with button " << label << endl;
		activate();
	}
	
	return collide_flag;
}	


void Button::draw()
// Draw a rectangle with a gradient, dark on the far left and right, white in
// the middle. Try to center the text.
{
	glPushMatrix();
		glTranslatef(xPos,yPos,-1);
		glScalef(width,height,1);
		
		glBegin(GL_QUADS);
			glColor4f(.1,.1,.1,1);
			glVertex2d(-1,1);
			glVertex2d(-1,-1);
			glVertex2d(1,-1);
			glVertex2d(1,1);
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(xPos,yPos,0);
		glScalef(width-2,height-2,1);

		glBegin(GL_QUADS);
			glColor4f(.15,.15,.15,0);
			glVertex2d(-1,1);
			glVertex2d(-1,-1);
			glColor4f(.25,.25,.25,1);
			glVertex2d(0,-1);
			glVertex2d(0,1);

			glColor4f(.25,.25,.25,1);
			glVertex2d(0,1);
			glVertex2d(0,-1);
			glColor4f(.15,.15,.15,0);
			glVertex2d(1,-1);
			glVertex2d(1,1);
		glEnd();
	glPopMatrix();

	// Turn on blending and texture mode.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glEnable(GL_TEXTURE_2D);	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	glPushMatrix();
		glTranslatef(xPos,yPos,1);
		glScalef(text_w, text_h,1);
		
		glBindTexture(GL_TEXTURE_2D, rendered_text);
		glBegin(GL_QUADS);
			glColor4f(0,0,0,0);
			glTexCoord2f(0,1);	glVertex2d(-1,1);
			glTexCoord2f(0,0);	glVertex2d(-1,-1);
			glTexCoord2f(1,0);	glVertex2d(1,-1);
			glTexCoord2f(1,1);	glVertex2d(1,1);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	
	// Turn off blending and texture mode.
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
