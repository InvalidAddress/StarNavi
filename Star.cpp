//==============================================================================
// Date Created:		15 February 2011
// Last Updated:		28 April 2011
//
// File name:			Star.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class to draw stars. Star size, color, and intensity
//						are all based on associated file attributes.
//==============================================================================

#include "Star.h"

//==============================================================================
// Constructor/Deconstructor.
//==============================================================================
Star::Star(filenode* f)
// Construct a star, and connect it to a filenode, from which metadata will be
// pulled and the star's attributes will be defined.
{
	file = f;
	
	recalc();
	
	if (glIsTexture(star_texture) == GL_FALSE)	initTexture();
}

Star::~Star()
{
}
//==============================================================================


//==============================================================================
// Automatic/private methods.
//==============================================================================
void Star::calculateDiameter()
// Set the radius of the star based on the size of the file.
{
	diameter = log10((float)(file->attr.st_size)+1)/log10(1000.0) + 1;
}

void Star::determineColor()
// Uses the attached file's type to set the star's color.
{
	color[3] = 1.0;
	
	switch (file->mime_enum)
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

void Star::recalc()
// In the case that the file has changed in some way, refresh the star data.
{
	calculateDiameter();
	determineColor();
}
//==============================================================================


//==============================================================================
// State setters.
//==============================================================================
void Star::setDiameter(float d)
// Set the diameter of the star.
{
	diameter = d;
}

void Star::setDistance(float d)
// Set the distance from the origin.
{
	distance = d;
}

void Star::setAngle(float a)
// Set the angle around the origin.
{
	angle = a;
}

void Star::setDepth(float d)
// Set the distance from the camera.
{
	depth = d;
}
//==============================================================================


//==============================================================================
// State getters.
//==============================================================================
float Star::getDiameter()
// Get the diameter of the star.
{
	return diameter;
}

float Star::getDistance()
// Get the distance from the origin.
{
	return distance;
}

float Star::getAngle()
// Get the angle around the origin.
{
	return angle;
}

float Star::getDepth()
// Get the distance from the camera.
{
	return depth;
}
//==============================================================================


//==============================================================================
// Convenience methods.
//==============================================================================
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
//==============================================================================


//==============================================================================
// Methods for user interaction.
//==============================================================================
void Star::activate()
// Try to open the file with the default app.
{
	int pid = fork();
	if (pid == 0)
	{
		string f = file->path + file->name;
		char *args[3];
		args[0] = const_cast<char*>(file->default_app.c_str());
		args[1] = const_cast<char*>(f.c_str());
		args[2] = (char*)0;
		
		cout << f << endl;
		cout << "Launching child process" << endl;
		execvp(const_cast<char*>(args[0]),args);
		cout << "Child process complete" << endl;
	}
	else
		cout << "parent process\n";
}

bool Star::getCollideFlag()
{
	return collide_flag;
}

bool Star::isColliding(float x, float y)
// If the mouse is within the perimeter of a star.
{
	collide_flag = y <= diameter/2;
	
//	cout << y << endl;
	
	if (collide_flag)
		cout << "colliding with file " << file->name << endl;
	
	return collide_flag;
}
//==============================================================================


//==============================================================================
// Drawing methods.
//==============================================================================
void Star::initTexture()
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
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//------------------------------------------------------------------------------
}

void Star::draw()
// Draw the star onto the current framebuffer.
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glBindTexture(GL_TEXTURE_2D, star_texture);
	glPushMatrix();
		glRotatef(angle,0,0,1);
		glTranslatef(distance,0,depth);
		glScalef(diameter,diameter,1);
	
		glColor4fv(color);
		glBegin(GL_QUADS);
			glTexCoord2f(0,1);	glVertex2d(-1,1);
			glTexCoord2f(0,0);	glVertex2d(-1,-1);
			glTexCoord2f(1,0);	glVertex2d(1,-1);
			glTexCoord2f(1,1);	glVertex2d(1,1);
		glEnd();
	
		glFlush();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
//==============================================================================
