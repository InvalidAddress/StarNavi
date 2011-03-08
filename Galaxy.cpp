//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		21 February 2011
//
// File name:			Galaxy.h
// Programmer:			Matthew Hydock
//
// File description:	A class that draws the files indexed by an Indexer as
//						Stars in a galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================

#include "Galaxy.h"

Galaxy::Galaxy(string s)
{
	cout << "making a galaxy...\n";
	diameter = 0;
	
	setRotation(0, 0);
	setRotationSpeed(.25);
	
	rotZ = 0;
	
	indexer = NULL;
	stars = NULL;
	
//==============================================================================
// Make the texture
//==============================================================================
	tex_size = 1024;
	tex_data = new GLbyte[tex_size*tex_size*4];
	
	cout << "made buffer\n";
	
	glGenTextures((GLsizei)1, &buff_tex);	
	glBindTexture(GL_TEXTURE_2D, buff_tex);
//------------------------------------------------------------------------------
// Set the state of the current texture
//------------------------------------------------------------------------------
	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//------------------------------------------------------------------------------
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_size, tex_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glBindTexture(GL_TEXTURE_2D,0);
//==============================================================================
	cout << "created texture\n";

	setDirectory(s);
	
	refreshTex();
}

Galaxy::~Galaxy()
{
	stars->clear();
	delete(stars);
	delete(indexer);
}

float Galaxy::getRotationX()
{
	return rotX;
}

float Galaxy::getRotationY()
{
	return rotY;
}

float Galaxy::getRotationSpeed()
{
	return rotSpeed;
}

Indexer* Galaxy::getIndexer()
{
	return indexer;
}
	
void Galaxy::setRotation(float x, float y)
{
	rotX = x;
	rotY = y;
}

void Galaxy::setRotationSpeed(float s)
{
	rotSpeed = s;
}

void Galaxy::setDirectory(string d)
{
	if (stars != NULL)
	{
		stars->clear();
		delete (stars);
	}
	if (indexer != NULL) delete(indexer);
	
	stars	= new list<Star*>();
	indexer	= new Indexer(d);
	indexer->build();
	
	cout << "file list built\n";
	
	list<filenode*> *files = indexer->getFileList();
	
	diameter = 64.0 * pow((3.0*(double)files->size())/(4.0*M_PI),(1.0/3.0));
	
	cout << "diameter: " << diameter << endl;
	
	for (list<filenode*>::iterator i = files->begin(); i != files->end(); i++)
	{
		Star *temp = new Star(*i);
		temp->randomPosition(0,360,0,(diameter/2)-10,-10,10);
		stars->push_back(temp);
	}
	

	cout << "stars made\n";
}

void Galaxy::refreshTex()
{
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buff_tex, 0);
	
	GLuint depth_buff;
	glGenRenderbuffers(1, &depth_buff);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buff);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, tex_size, tex_size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buff);
	

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	cout << status << endl;

	glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,tex_size,tex_size);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-diameter/2,diameter/2,-diameter/2,diameter/2,-10,10);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		int n = 1;
		int size = stars->size();
		for (list<Star*>::iterator i = stars->begin(); i != stars->end(); i++)
		{
			(*i)->draw();
			n++;
		}
			
		glFlush();
	glPopAttrib();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Galaxy::draw()
{
	glBindTexture(GL_TEXTURE_2D, buff_tex);

	glPushMatrix();
		glRotatef(rotX,1,0,0);
		glRotatef(rotY,0,1,0);
		glRotatef(rotZ,0,0,1);
	
		glBegin(GL_QUADS);
			glTexCoord2f(0,1);	glVertex2d(-1,1);
			glTexCoord2f(0,0);	glVertex2d(-1,-1);
			glTexCoord2f(1,0);	glVertex2d(1,-1);
			glTexCoord2f(1,1);	glVertex2d(1,1);
		glEnd();
	
		glFlush();
	glPopMatrix();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	rotZ += rotSpeed;
		if (rotZ > 360) rotZ -= 360;
	
	glFlush();
}
