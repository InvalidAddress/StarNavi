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
	indexer = NULL;
	stars = NULL;
	
	cout << "making a galaxy\n";
	
	width = 400;
	height = 400;
	tex_data = new GLbyte[height*width];
	
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glBindTexture(GL_TEXTURE_2D,0);

	cout << "created texture\n";

	setDirectory(s);
	
	refreshTex();
	
	setRotation(0, 0);
	setRotationSpeed(2);
	
	rotZ = 0;
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
	
	/*
	for (list<filenode*>::iterator i = files->begin(); i != files->end(); i++)
		cout << (*i)->name << endl;
	*/
	
	for (list<filenode*>::iterator i = files->begin(); i != files->end(); i++)
	{
		Star *temp = new Star(*i);
		temp->randomPosition(0,360,0,240,-10,10);
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
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buff);
	

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	cout << status << endl;

	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,width,height);

	int n = 1;
	int size = stars->size();
	for (list<Star*>::iterator i = stars->begin(); i != stars->end(); i++)
	{
		(*i)->draw();
		n++;
	}
			
	rotZ += rotSpeed;
	if (rotZ > 360) rotZ -= 360;
		
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
	
		glScalef(width,height,1);
	
		glBegin(GL_QUADS);
			glTexCoord2f(0,1);	glVertex2d(-1,1);
			glTexCoord2f(0,0);	glVertex2d(-1,-1);
			glTexCoord2f(1,0);	glVertex2d(1,-1);
			glTexCoord2f(1,1);	glVertex2d(1,1);
		glEnd();
	
		glFlush();
	glPopMatrix();
	
	glBindTexture(GL_TEXTURE_2D, 0);
}
