//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		28 April 2011
//
// File name:			Galaxy.h
// Programmer:			Matthew Hydock
//
// File description:	A class that draws a list of files as Stars in a galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================

#include "Galaxy.h"

//==============================================================================
// Constructors/Deconstructors
//==============================================================================
Galaxy::Galaxy(dirnode *r, list<filenode*> *f, cluster_type m, string n)
{
	cout << "making a galaxy...\n";
	
	if (r != NULL)
		setDirectory(r);
	else
	{
		root = NULL;
		files = f;
	}
	
	if (root != NULL && n == "")
		name = root->name;
	else
		name = n;
	
	xPos = 0;
	yPos = 0;
	side = 0;
		
	diameter = 64.0 * pow((3.0*(double)files->size())/(4.0*M_PI),(1.0/3.0));
	radius = diameter/2;
	thickness = pow(radius*2.0,.5);
	
	setRotation(0,0);
	setRotationSpeed(0.02);
	
	rotZ = 0;
	
	mode = DIRECTORY;
	
	sectors = NULL;
	selected = NULL;
	buildSectors();
	
	tex_data = NULL;
	
	initTexture();
	refreshTex();
	
	cout << "galaxy built\n";
}

Galaxy::~Galaxy()
{
	for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
		delete (*i);

	delete(sectors);
}
//==============================================================================


//==============================================================================
// Methods related to the angle and velocity of the galaxy.
//==============================================================================
void Galaxy::setRotation(float x, float y)
{
	rotX = x;
	rotY = y;
}

void Galaxy::setRotationSpeed(float s)
{
	rotSpeed = s;
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
//==============================================================================


//==============================================================================
// Methods related to sector management.
//==============================================================================
void Galaxy::buildSectors()
{
	cout << "building sectors\n";
	clearSectors();
	
	sectors = new list<GSector*>();
	
	if (mode == DIRECTORY)
		buildHierarchy();
		
	cout << "sectors built\n";
	
	if (sectors->size() == 1)
		(*(sectors->begin()))->setSingleSectorMode(true);
}

void Galaxy::buildHierarchy()
{
	cout << "hierarchy build mode\n";
	
	if (root == NULL)
	{
		sectors->push_back(new GSector(NULL,files,radius,0,360,name));
		return;
	}
	
	float arc_begin = 0;
	float arc_width = 360.0*((float)root->files.size()/(float)root->all_files.size());
	
	sectors->push_back(new GSector(NULL,&(root->files),radius,arc_begin,arc_width,"./"));
	cout << "root sector built\n";
	
	cout << "creating sectors for directories\n";
	for (list<dirnode*>::iterator i = root->dirs.begin(); i != root->dirs.end(); i++)
	{
		arc_begin += arc_width;
		arc_width = 360.0*((float)(*i)->all_files.size()/(float)root->all_files.size());
		sectors->push_back(new GSector(*i,NULL,radius,arc_begin,arc_width));
	}
	
	adjustSectorWidths();
}

void Galaxy::adjustSectorWidths()
// Dynamically resize the sectors' widths, in case some are too small.
{
	cout << endl;
	
	list<GSector*> *temp = new list<GSector*>();
	list<GSector*>::iterator i = sectors->begin();
	
	// Sort the sectors into another list.
	temp->push_back(*i);
	i++;
	while (temp->size() < sectors->size())
	{
		list<GSector*>::iterator j = temp->begin();
		while(j != temp->end() && (*i)->getArcWidth() > (*j)->getArcWidth())
			j++;
		if (j == temp->end())	temp->push_back(*i);
		else					temp->insert(j,(*i));
		i++;
	}
	// Done sorting the sectors.
	
	for (list<GSector*>::iterator j = temp->begin(); j != temp->end(); j++)
		cout << (*j)->getArcWidth() << endl;
	
	// If the current sector is too small, grow it and shrink all larger ones.
	for (i = temp->begin(); i != temp->end() && (*i)->getArcWidth() < ((*i)->calcMinArcWidth()+5); i++)
	{
		float diff = ((*i)->calcMinArcWidth()+5)-(*i)->getArcWidth();
		(*i)->setArcWidth((*i)->getArcWidth()+diff);
		
		int remain = 0;
		for (list<GSector*>::iterator j = i; j != temp->end(); j++)
			remain++;
		remain--;
		
		float distr = diff/(float)remain;
		cout << "diff = " << diff << "; remain = " << remain << "; distr = " << distr << endl; 
		
		list<GSector*>::iterator j = i;
		j++;
		while (j != temp->end())
		{
			cout << "shrinking sector " << (*j)->getName() << endl;
			(*j)->setArcWidth((*j)->getArcWidth()-distr);
			j++;
		}
	}
	// Done shrinking the sectors.
	
	// Shift all of the sectors into proper positions
	list<GSector*>::iterator j = sectors->begin();
	list<GSector*>::iterator k = sectors->begin();
	
	k++;
	for(;k != sectors->end(); k++)
	{
		(*k)->setArcBegin((*j)->getArcEnd());
		j = k;
	}
	// Done shifting sectors.
	
	cout << "new sector sizes\n";
	
	for (list<GSector*>::iterator j = temp->begin(); j != temp->end(); j++)
		cout << (*j)->getArcBegin() << "  " << (*j)->getArcEnd() << "  " << (*j)->getArcWidth() << endl;
	
	cout << "done resizing." << endl << endl;
}
		

void Galaxy::clearSectors()
{
	if (sectors != NULL)
	{
		for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
			delete (*i);
		delete (sectors);
	}
}


list<GSector*>* Galaxy::getSectors()
// Return a list of the galaxy's sectors.
{
	return sectors;
}
//==============================================================================


//==============================================================================
// Miscellanious getters and setters.
//==============================================================================
void Galaxy::setMode(cluster_type m)
// Set the galaxy's clustering mode.
{
	mode = m;
}

cluster_type Galaxy::getMode()
// Return the galaxy's clustering mode.
{
	return mode;
}

void Galaxy::setDirectory(dirnode *r)
// Set the galaxy's file indexer.
{
	root = r;
	files = &(r->all_files);
}

dirnode* Galaxy::getDirectory()
// Obtain a pointer to the indexer used by the galaxy.
{
	return root;
}
//==============================================================================


//==============================================================================
// Methods for user interaction.
//==============================================================================
bool Galaxy::isColliding(float x, float y)
{
	selected = NULL;
	
	float localX = x-xPos;
	float localY = y-yPos;
	
	float angle_r = atan2(localY,localX);
	float angle_d = angle_r*(180.0/M_PI);
	float magnitude = sqrt(pow(localX,2.0)+pow(localY,2.0));
	float norm_mag = magnitude/(side/2);

	angle_d -= rotZ;	
	angle_d = (angle_d < 0)?angle_d+360:angle_d;
	
//	cout << angle_d << ", " << norm_mag << endl;
	
	if (norm_mag > 1.0)
		return collide_flag = false;
	
	for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
		if ((*i)->isColliding(angle_d, norm_mag))
			selected = (*i);
		
	if (selected != NULL) cout << "collide with sector " << selected->getName() << endl;
	return collide_flag = true;
}

GSector* Galaxy::getSelected()
{
	return selected;	
}
//==============================================================================


//==============================================================================
// Methods related to drawing.
//==============================================================================
void Galaxy::initTexture()
{
	glGenTextures((GLsizei)1, &texture);	
	glBindTexture(GL_TEXTURE_2D, texture);
//------------------------------------------------------------------------------
// Set the state of the current texture
//------------------------------------------------------------------------------
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//------------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D,0);

	cout << "created texture\n";
}

void Galaxy::refreshTex()
// Update the galaxy texture.
{
	tex_size = (int)(1024.0 * (diameter/500.0));
	tex_size = (tex_size < 512)?512:tex_size;

	cout << diameter << "  " << tex_size << endl;

	// Create a depth buffer for the framebuffer object.
	GLuint fbo_depth;
	glGenRenderbuffers(1, &fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tex_size, tex_size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);

	printGlError();

	// Prepare the texture for rendering.
	glBindTexture(GL_TEXTURE_2D, texture);
	if (tex_data == NULL) delete(tex_data);
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

		for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
			(*i)->draw();
			
		glFlush();
	glPopAttrib();
	
	// Release the buffers, and return to the defaults.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Galaxy::draw()
// Draw the galaxy.
{
	// Set the size and origin of the galaxy, based on the viewport.
	int p[4];
	glGetIntegerv(GL_VIEWPORT,p);
	side = (p[2]<p[3])?p[2]:p[3];
	xPos = ((float)p[2])/2.0;
	yPos = ((float)p[3])/2.0;

	// Turn on blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Turn on texture mode.
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPushMatrix();
		// Rotate the galaxy 
		glTranslatef(0,0,-1);
		glRotatef(rotZ,0,0,1);
		glScalef((side-5)/2,(side-5)/2,1);
		
		// Bind the previously rendered texture.
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
			glTexCoord2f(0,1);	glVertex2d(-1,1);
			glTexCoord2f(0,0);	glVertex2d(-1,-1);
			glTexCoord2f(1,0);	glVertex2d(1,-1);
			glTexCoord2f(1,1);	glVertex2d(1,1);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	
	// Turn off texture mode.
	glDisable(GL_TEXTURE_2D);
	
	// Draw the sector division lines.
	glPushMatrix();
		glRotatef(rotZ,0,0,1);
		glScalef((side-5)/2,(side-5)/2,1);
		
		if (sectors->size() > 1)
		{
			glColor3d(1,1,1);
			glBegin(GL_LINES);
				for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
				{
					float arc_begin = (*i)->getArcBegin();
					float arc_begin_r = arc_begin * M_PI/180;
					float arc_end = (*i)->getArcEnd();
					float arc_end_r = arc_end * M_PI/180;

					glVertex2d(0.0,0.0);
					glVertex2d(cos(arc_begin_r),sin(arc_begin_r));

					glVertex2d(0.0,0.0);
					glVertex2d(cos(arc_end_r),sin(arc_end_r));
				}
			glEnd();		
		}
	glPopMatrix();
	
	// Draw the selection mask.
	glPushMatrix();
		glTranslatef(0,0,1);
		glRotatef(rotZ,0,0,1);
		glScalef((side)/2,(side)/2,1);
		if (selected != NULL) selected->drawMask();
	glPopMatrix();
	
	// Turn off blending.
	glDisable(GL_BLEND);
	
	rotZ += rotSpeed;
		if (rotZ > 360) rotZ -= 360;
}
//==============================================================================
