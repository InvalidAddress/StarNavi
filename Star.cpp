//==============================================================================
// Date Created:		15 February 2011
// Last Updated:		21 February 2011
//
// File name:			Star.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class to draw stars. Star size, color, and intensity
//						are all based on associated file attributes.
//==============================================================================

#include "Star.h"

//==============================================================================
// Private methods.
//==============================================================================
void Star::calculateDiameter()
// Set the radius of the star based on the size of the file.
{
	diameter = log10((float)(file->attr.st_size)+1)/log10(1000.0);
}

void Star::determineColor()
// Uses the attached file's type to set the star's color.
{
	color[3] = 1.0;
	
	switch (file->type)
	{
		case BIN:		color[0] = 0.0;	color[1] = 0.0;	color[2] = 1.0;				// blue
						break;
		case APP:		color[0] = 0.5;	color[1] = 0.5;	color[2] = 1.0;				// light blue
						break;
		case AUDIO:		color[0] = 1.0;	color[1] = 0.0;	color[2] = 0.0;				// red
						break;
		case IMAGE:		color[0] = 1.0;	color[1] = 1.0;	color[2] = 0.0;				// yellow
						break;
		case VIDEO:		color[0] = 1.0;	color[1] = .25;	color[2] = 0.0;				// red-orange
						break;
		case TEXT:		color[0] = 1.0;	color[1] = 1.0;	color[2] = 1.0;				// white
						break;
		case UNKNOWN:	color[0] = 0.5;	color[1] = 0.3;	color[2] = 0.0;				// brown
						break;
	}
}

void Star::texInit()
// Initialize the global star texture. Cobbled together from bits of code found
// online and from the OpenGL reference book (sixth edition).
{
	ilGenImages(1, &star_texture);
	ilBindImage(star_texture);
	ilLoadImage((ILstring)"./images/star2.png");
	ilutGLBindTexImage();
	ilDeleteImages(1, &star_texture);
	
//------------------------------------------------------------------------------
// Set the state of the current texture
//------------------------------------------------------------------------------
	// select modulate to mix texture with color for shading
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//------------------------------------------------------------------------------
}
//==============================================================================


//==============================================================================
// Public methods.
//==============================================================================
Star::Star(filenode* f)
// Construct a star, and connect it to a filenode, from which metadata will be
// pulled and the star's attributes will be defined.
{
	file = f;
	
	calculateDiameter();
	determineColor();
	
	if (glIsTexture(star_texture) == GL_FALSE)	texInit();
}

Star::~Star()
{
}

void Star::randomPosition(float a1, float a2, float dis1, float dis2, float dep1, float dep2)
// Generate and assign a random position to this star within user defined ranges
{
	MTRand mtrand;
	
	angle		= mtrand.rand(a2-a1)+a1;
	distance	= mtrand.rand(dis2-dis1)+dis1;
	depth		= mtrand.rand(dep2-dep1)+dep1;
}

void Star::setPosition(float a, float dis, float dep)
// Manually set the position of the star.
{
	angle = a;
	distance = dis;
	depth = dep;
}

void Star::recalc()
// In the case that the file has changed in some way, refresh the star data.
{
	calculateDiameter();
	determineColor();
}

void Star::draw()
// Draw the star onto the current framebuffer.
{
	glBindTexture(GL_TEXTURE_2D, star_texture);
	
	glPushMatrix();
	
		glRotatef(angle,0,0,1);
		glTranslatef(distance,0,depth);
		glScalef(diameter,diameter,1);
	
		glColor4fv(color);
		glBegin(GL_QUADS);
			glTexCoord2f(0,1);	glVertex2d(-1,1);
			glTexCoord2f(0,0); glVertex2d(-1,-1);
			glTexCoord2f(1,0);	glVertex2d(1,-1);
			glTexCoord2f(1,1);	glVertex2d(1,1);
		glEnd();
	
		glFlush();
	glPopMatrix();
}
