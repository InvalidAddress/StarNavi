//==============================================================================
// Date Created:		18 March 2011
// Last Updated:		6 April 2011
//
// File name:			GSector.h
// Programmer:			Matthew Hydock
//
// File description:	A class that draws a chunck of a galaxy, using a file
//						list and dimensions provided by it's patron galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================

#include "GSector.h"

GSector::GSector(dirnode *r, list<filenode*> *f, float ra, float b, float e)
// Creates a sector, with a list of files and the given dimensions. Can take a
// dirnode for hierarchical functionality, but it is not necessary.
{	
	if (r != NULL)
		setDirectory(r);
	else
	{
		root = NULL;
		files = f;
	}
		
	radius = ra;
	
	arc_begin = b;
	arc_end = e;
	
	cout << arc_begin << " , " << arc_end << endl;
	
	thickness = pow(radius*2,.5);
	
	stars = NULL;
	buildStars();
}

GSector::~GSector()
// Deletes all of the stars. Does not delete the file list or the directory node
// as they are external values, and should be dealt with externally.
{
	clearStars();
}

void GSector::buildStars()
// Erases the current star list, then creates new stars within the sector's
// physical range.
{
	clearStars();
	
	stars = new list<Star*>();
	
	for (list<filenode*>::iterator i = files->begin(); i != files->end(); i++)
	{
		Star *temp = new Star(*i);
		temp->randomPosition(arc_begin,arc_end,0,radius,-thickness,thickness);
		stars->push_back(temp);
	}
}

void GSector::clearStars()
// Erases all of the stars, and deletes the star list.
{
	if (stars != NULL)
	{
		for (list<Star*>::iterator i = stars->begin(); i != stars->end(); i++)
			delete (*i);
		delete (stars);
	}
}

//==============================================================================
// Methods that work with the dimensions of the sector.
//==============================================================================
void GSector::setRadius(float r)
{
	radius = r;
}

void GSector::setArcBegin(float b)
{
	arc_begin = b;
}

void GSector::SetArcEnd(float e)
{
	arc_end = e;
}

float GSector::getRadius()
{
	return radius;
}

float GSector::getArcBegin()
{
	return arc_begin;
}

float GSector::getArcEnd()
{
	return arc_end;
}
//==============================================================================

list<filenode*>* GSector::getFileList()
// Return the sector's file list. Use this only if getDirectory returns NULL.
{
	return files;
}

void GSector::setDirectory(dirnode *r)
// Set the sector's root directory.
{
	root = r;
	files = &(r->all_files);
}

dirnode* GSector::getDirectory()
// Obtain the sector's root directory.
{
	return root;
}


void GSector::buildMask()
{
	tex_size = (int)(1024.0 * (radius*2/500.0));

	cout << radius*2 << "  " << tex_size << endl;

	// Create a depth buffer for the framebuffer object.
	GLuint fbo_depth;
	glGenRenderbuffers(1, &fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tex_size, tex_size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);

	printGlError();

	// Prepare the texture for rendering.
	glBindTexture(GL_TEXTURE_2D, texture);
	if (tex_data = NULL) delete(tex_data);
	tex_data = new GLbyte[tex_size*tex_size*4];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_size, tex_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	printGlError();
	
	// Create and bind the frame buffer object.
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);
	
	//printFramebufferError();

	// Push the viewport to an attribute stack, and render as usual.
	glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,tex_size,tex_size);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-radius,radius,-radius,radius,-thickness,thickness);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBegin(GL_TRIANGLES);
			glColor4d(0,0,0,.75);
			float outer = 360-(arc_end-arc_begin);
			float i = arc_end;
			for (float j = i+10; j <= arc_end+outer; j += 10)
			{
				glVertex3d(0,0,1);
				glVertex3d((radius+50)*cos(i),(radius+50)*sin(i),1);
				glVertex3d((radius+50)*cos(j),(radius+50)*sin(j),1);
				i = j;
			}
			glVertex3d(0,0,1);
			glVertex3d((radius+50)*cos(i),(radius+50)*sin(i),1);
			glVertex3d((radius+50)*cos(arc_end+outer),(radius+50)*sin(arc_end+outer),1);
		glEnd();
			
		glFlush();
	glPopAttrib();
	
	// Release the buffers, and return to the defaults.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
		
void GSector::drawMask()
// Accentuate the sector, by darkening the rest of the galaxy, and drawing a 
// glowing outline around the sector.
{
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
		glTexCoord2f(0,1);	glVertex2d(-1,1);
		glTexCoord2f(0,0);	glVertex2d(-1,-1);
		glTexCoord2f(1,0);	glVertex2d(1,-1);
		glTexCoord2f(1,1);	glVertex2d(1,1);
	glEnd();
	
	glFlush();
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GSector::draw()
{
	for (list<Star*>::iterator i = stars->begin(); i != stars->end(); i++)
		(*i)->draw();
}

bool GSector::isColliding(float x, float y)
// X and Y are actually polar coordinates in this case.
{
	return collide_flag = (x >= arc_begin) && (x <= arc_end) && (y <= radius);
}
		
