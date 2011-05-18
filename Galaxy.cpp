//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		17 May 2011
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
Galaxy::Galaxy(dirnode *r, list<filenode*> *f, cluster_type m, string n, list<string>* t)
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
		name = root->path + root->name;
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
	
	tags = t;
	if (tags == NULL)
		rebuildTags();
	
	cluster_mode = m;
	
	sectors = NULL;
	selected = NULL;
	buildSectors();
	
	tex_data = NULL;
	initTexture();
	refreshTex();
	
	label = NULL;
	initLabel();
	
	starSelectionLabel = new DrawText(" Star Selection Mode");
	starSelectionLabel->setAlignment(LEFT);
	starSelectionLabel->setTextColor(0,0,0,1);
	starSelectionLabel->refreshTexture();
	
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
// Miscellanious getters and setters.
//==============================================================================
void Galaxy::setName(string n)
// Set the galaxy's name (to be used if root == NULL)
{
	name = n;
}

string Galaxy::getName()
// Obtain the galaxy's name.
{
	return name;
}

void Galaxy::setTags(list<string>* t)
// In the event an outside object already made a set of tags, might as well just
// use those.
{
	tags = t;
}

void Galaxy::rebuildTags()
// Build the galaxy's tag list.
{
	cout << "building tags" << endl;
	
	// Clean out the original tags list, if it wasn't empty to begin with.
	if (tags != NULL)	delete(tags);
	tags = new list<string>;
	
	// Temporary list for the current file's tags.
	list<string>* temp = NULL;
	
	// Go through all of the file nodes, and try to add that file's tags to the
	// tag list. If the list already contains the current tag, skip it.
	for (list<filenode*>::iterator i = files->begin(); i != files->end(); i++)
	{
		temp = &((*i)->tags);
		for (list<string>::iterator j = temp->begin(); j != temp->end(); j++)
			if (!contains(tags,*j))
			{
				cout << *j << endl;
				tags->push_back(*j);
			}
	}
	
	cout << "size of tags list: " << tags->size() << endl;	
}

list<string>* Galaxy::getTags()
// Return the galaxy's list of tags.
{
	return tags;
}

void Galaxy::setClusterMode(cluster_type m)
// Set the galaxy's clustering mode.
{
	cluster_mode = m;
}

cluster_type Galaxy::getClusterMode()
// Return the galaxy's clustering mode.
{
	return cluster_mode;
}

void Galaxy::setDirectory(dirnode *r)
// Set the galaxy's file indexer.
{
	root = r;
	files = &(r->all_files);
}

dirnode* Galaxy::getDirectory()
// Obtain a pointer to the root of the indexer used by the galaxy.
{
	return root;
}

void Galaxy::setFileList(list<filenode*> *f)
// Set the galaxy's file list (to be used if root == NULL)
{
	files = f;
}

list<filenode*>* Galaxy::getFileList()
// Obtain the galaxy's file list (to be used if root == NULL)
{
	return files;
}
//==============================================================================


//==============================================================================
// Sector building.
//==============================================================================
void Galaxy::buildSectors()
// Build the galaxy's sectors based on the current build mode.
{
	cout << "building sectors\n";
	clearSectors();
	
	sectors = new list<GSector*>();
	
	switch (cluster_mode)
	{
		case NONE:
		case DIRECTORY:	buildHierarchy();
						break;
		case NAME:		buildByName();
						break;
		case DATE:		buildByDate();
						break;
		case SIZE:		buildBySize();
						break;
		case TYPE:		buildByType();
						break;
		case TAGS:		buildByTags();
						break;
		default:		break;
	}
		
	cout << "sectors built\n";
	
	if (sectors->size() == 1)
		(*(sectors->begin()))->setSingleSectorMode(true);
	else
		adjustSectorWidths();
}

void Galaxy::buildHierarchy()
// Build a galaxy using the file hierarchy for the structure.
{
	cout << "hierarchy build mode\n";
	
	if (root == NULL)
	{
		name += " [files]";
		sectors->push_back(new GSector(NULL,files,radius,0,360,name));
		return;
	}
	
	float arc_begin = 0;
	float arc_width = 360.0*((float)root->files.size()/(float)root->all_files.size());
	
	// Make the sector that holds the current directories loose files.
	sectors->push_back(new GSector(NULL,&(root->files),radius,arc_begin,arc_width,"./"));
	cout << "root sector built\n";
	
	// Make sectors for the other subdirectories. 
	cout << "creating sectors for directories\n";
	for (list<dirnode*>::iterator i = root->dirs.begin(); i != root->dirs.end(); i++)
	{
		arc_begin += arc_width;
		arc_width = 360.0*((float)(*i)->all_files.size()/(float)root->all_files.size());
		sectors->push_back(new GSector(*i,NULL,radius,arc_begin,arc_width));
	}
}

void Galaxy::buildByName()
// Build a galaxy by organizing files by their names.
{
	list<filenode*> sorted_files;
	list<list<filenode*>*> file_lists;
	size_t charindex = 0;
	
	// Sort the galaxy's file list by name of the files.
	list<filenode*>::iterator i = files->begin();
	list<filenode*>::iterator j;
	sorted_files.push_back(*i);
	i++;
	while (sorted_files.size() < files->size())
	{
		j = sorted_files.begin();
		for(;j != sorted_files.end() && isLessThan((*j)->name,(*i)->name);j++);
		if (j == sorted_files.end())	sorted_files.push_back(*i);
		else							sorted_files.insert(j,(*i));
		i++;
	}
	// Done sorting the sectors.
	
	// Preliminary test. Go through the sorted list, and see at what index do
	// the file names start being different.
	i = sorted_files.end();
	filenode *b = (*(sorted_files.begin()));
	for (charindex = 0; i == sorted_files.end(); charindex++)
	{
		for (i = sorted_files.begin(); i != sorted_files.end(); i++)
			if (charindex < (*i)->name.size() && charindex < b->name.size())
				if ((*i)->name[charindex] != b->name[charindex])
					break;
	}
	// End preliminary test.
	
	// Begin splitting sorted list.
	i = sorted_files.begin();
	list<filenode*>* temp = new list<filenode*>;
	temp->push_back(*i);
	
	j = i;
	j++;
	while (j != sorted_files.end())
	{
		if (charindex >= (*j)->name.size() || charindex >= (*i)->name.size() || (*j)->name[charindex] != (*i)->name[charindex])
		{
			file_lists.push_back(temp);
			temp = new list<filenode*>;
		}
		
		temp->push_back(*j);
		i = j;
		j++;
	}
	
	file_lists.push_back(temp);
	// End splitting sorted list.
	
	// Start making the sectors.
	float arc_begin = 0;
	float arc_width = 0;
	 
	cout << "creating sectors divided by name\n";
	for (list<list<filenode*>*>::iterator i = file_lists.begin(); i != file_lists.end(); i++)
	{
		string name = (*((*i)->begin()))->name;
		name = name.substr(0,charindex);
		arc_begin += arc_width;
		arc_width = 360.0*((float)(*i)->size()/(float)sorted_files.size());
		sectors->push_back(new GSector(NULL,(*i),radius,arc_begin,arc_width,name));
	}
	// End making sectors.
}

void Galaxy::buildByDate()
{}

void Galaxy::buildBySize()
{}

void Galaxy::buildByType()
{}

void Galaxy::buildByTags()
// Build sectors by separating files according to what tags they have.
{
	list<list<filenode*>*>* temp_list = new list<list<filenode*>*>;
	int total_size = 0;
	
	// Make a file list for each tag, containing files that have those tags.
	for (list<string>::iterator i = tags->begin(); i != tags->end(); i++)
	{
		cout << "collecting files with tag " << *i << endl;
		// Make a temporary list for filenodes.
		list<filenode*>* temp_files = new list<filenode*>;
		
		// Fill it with files that have the current tag.
		for (list<filenode*>::iterator j = files->begin(); j != files->end(); j++)
			if (contains(&((*j)->tags),*i))
			{
				cout << "found a file!\n";
				temp_files->push_back(*j);
			}
				
		// Keep track of the total size, then add it to the list of lists.
		total_size += temp_files->size();
		temp_list->push_back(temp_files);
		
		cout << total_size << " " << temp_files->size() << endl;
	}
	
	float arc_begin = 0;
	float arc_width = 0;
	
	// Make sectors for the tags.
	cout << "creating sectors for tags\n";
	list<string>::iterator n = tags->begin();
	for (list<list<filenode*>*>::iterator i = temp_list->begin(); i != temp_list->end(); i++)
	{
		arc_begin += arc_width;
		arc_width = 360.0*((float)(*i)->size()/total_size);
		sectors->push_back(new GSector(NULL,*i,radius,arc_begin,arc_width,*n));
		n++;
	}

	if (sectors->size() <= 1)
		rebuildTags();
}
//==============================================================================


//==============================================================================
// Sector management.
//==============================================================================
void Galaxy::adjustSectorWidths()
// Dynamically resize the sectors' widths, in case some are too small. This
// method will probably derp hard if there are a ton of sectors that are too
// small. Until I fix that, just try not to have ~90 directories with only 1
// file in each of them.
{	
	list<GSector*> *temp = new list<GSector*>();
	list<GSector*>::iterator i = sectors->begin();
	list<GSector*>::iterator j;
	
	// Sort the sectors into another list.
	temp->push_back(*i);
	i++;
	while (temp->size() < sectors->size())
	{
		j = temp->begin();
		while(j != temp->end() && (*i)->getArcWidth() > (*j)->getArcWidth())
			j++;
		if (j == temp->end())	temp->push_back(*i);
		else					temp->insert(j,(*i));
		i++;
	}
	// Done sorting the sectors.
		
	// If the current sector is too small, grow it and shrink all larger ones.
	for (i = temp->begin(); i != temp->end() && (*i)->getArcWidth() < ((*i)->calcMinArcWidth()+4); i++)
	{
		float diff = ((*i)->calcMinArcWidth()+4)-(*i)->getArcWidth();
		(*i)->setArcWidth((*i)->getArcWidth()+diff);
		
		int remain = 0;
		for (j = i; j != temp->end(); j++)
			remain++;
		remain--;
		
		float distr = diff/(float)remain;
		
		j = i;
		j++;
		while (j != temp->end())
		{
			(*j)->setArcWidth((*j)->getArcWidth()-distr);
			j++;
		}
	}
	// Done shrinking the sectors.
	
	// Shift all of the sectors into proper positions
	i = sectors->begin();
	j = sectors->begin();
	
	j++;
	for(;j != sectors->end(); j++)
	{
		(*j)->setArcBegin((*i)->getArcEnd());
		i = j;
	}
	// Done shifting sectors.
	
	// Make sure all the stars are within their sector's bounds.
	for (i = sectors->begin(); i != sectors->end(); i++)
	{
		float beg = (*i)->getArcBegin();
		float wid = (*i)->getArcWidth();
		float thk = (*i)->getThickness();
		float rad = (*i)->getRadius();
		
		for (list<Star*>::iterator k = (*i)->getStars()->begin(); k != (*i)->getStars()->end(); k++)
			if ((*k)->getAngle() > beg+wid || (*k)->getAngle() < beg)
				(*k)->randomPosition(beg,beg+wid,0,rad,-thk,thk);
	}
	// Done repositioning stars.
}
		

void Galaxy::clearSectors()
{
	if (sectors != NULL)
	{
		for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
			sectors->erase(i);
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
// Methods for user interaction.
//==============================================================================
bool Galaxy::isColliding(float x, float y)
{
	selected = NULL;
	
	// Turn the given coordinates to local coordinates.
	float localX = x-xPos;
	float localY = y-yPos;
	
	// Turn the local cartesian coordinates into local polar coordinates.
	float angle_r = atan2(localY,localX);
	float angle_d = angle_r*(180.0/M_PI);
	float magnitude = sqrt(pow(localX,2.0)+pow(localY,2.0));
	float norm_mag = magnitude/(side/2);

	// Shift the angle to match the galaxy's rotation
	angle_d -= rotZ;	
	angle_d = (angle_d < 0)?angle_d+360:angle_d;

//	cout << xPos << ", " << yPos << endl;
//	cout << x << ", " << y << " | " << localX << ", " << localY << endl;
//	cout << angle_d << ", " << norm_mag << endl;
	
	if (norm_mag > 1.0)
		return collide_flag = false;
	
	for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end() && selected == NULL; i++)
		if ((*i)->isColliding(angle_d, norm_mag))
			selected = (*i);
		
//	if (selected != NULL) cout << "colliding with sector " << selected->getName() << endl;
	
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

//	cout << diameter << "  " << tex_size << endl;

	// Create a depth buffer for the framebuffer object.
	GLuint fbo_depth;
	glGenRenderbuffers(1, &fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tex_size, tex_size);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);

//	printGlError();

	// Prepare the texture for rendering.
	glBindTexture(GL_TEXTURE_2D, texture);
	if (tex_data != NULL) delete(tex_data);
	tex_data = new GLubyte[tex_size*tex_size*4];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_size, tex_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
//	printGlError();
	
	// Create and bind the frame buffer object.
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);
	
//	printFramebufferError();

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
	// Set the size of the galaxy, based on the viewport.
	int p[4];
	glGetIntegerv(GL_VIEWPORT,p);
	side = (p[2]<p[3])?p[2]:p[3];

	// Turn on blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Turn on texture mode.
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Draw the texture-mapped galaxy.
	glPushMatrix();
		// Rotate the galaxy 
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
	// Done drawing the texture-mapped galaxy.
	
	// Turn off texture mode.
	glDisable(GL_TEXTURE_2D);
	
	// If the user is not in star selection mode, draw the sector lines and the
	// sector selection mask.
	if (!Star::starSelectionMode && sectors->size() > 1)
	{
		// Draw the sector division lines.
		glPushMatrix();
			glTranslatef(0,0,1);
			glRotatef(rotZ,0,0,1);
			glScalef((side-5)/2,(side-5)/2,1);
		
			glBegin(GL_LINES);
				for (list<GSector*>::iterator i = sectors->begin(); i != sectors->end(); i++)
				{
					float arc_begin = (*i)->getArcBegin();
					float arc_begin_r = arc_begin * M_PI/180;

					glColor4d(1,1,1,1);
					glVertex2d(0.0,0.0);
					glColor4d(0,0,0,0);
					glVertex2d(cos(arc_begin_r),sin(arc_begin_r));
				}
			glEnd();		
		glPopMatrix();
		// Done drawing sector division lines.
		
		// Draw the selection mask.
		if (collide_flag && selected != NULL)
		{			
			glPushMatrix();
				glTranslatef(0,0,2);
				glRotatef(rotZ,0,0,1);
				glScalef((side)/2,(side)/2,1);
				selected->drawMask();
			glPopMatrix();
			
			// Try to initialize the text label. If it already exists, this does
			// nothing.
			selected->initLabel();
			
			float angle = rotZ + selected->getArcBegin() + (selected->getArcWidth()/2);			
			float x = ((side-5)/4) * cos(angle*M_PI/180);
			float y = ((side-5)/4) * sin(angle*M_PI/180);
			
//			cout << angle << " " << x << " " << y << endl;
			
			selected->getLabel()->setPosition(x,y);
		
			glPushMatrix();
				glTranslatef(0,0,3);
				selected->drawLabel();
			glPopMatrix();
		}
		// Done drawing selection mask.
	}
	
	// User is in star selection mode.
	if (Star::starSelectionMode || sectors->size() <= 1)
	{
		// Draw the Star Selection Mode banner.
		glPushMatrix();
			glTranslatef(p[2]/-2,p[3]/2,1);
			glScalef(starSelectionLabel->getTextWidth(),starSelectionLabel->getTextHeight(),1);
			glBegin(GL_QUADS);
				glColor4d(1,1,1,1);
				glVertex2d(0,0);
				glVertex2d(0,-1);
				glColor4d(1,1,1,0);
				glVertex2d(2,-1);
				glVertex2d(2,0);
			glEnd();
		glPopMatrix();
			
		glPushMatrix();
			glTranslatef(0,0,2);
			starSelectionLabel->setPosition(p[2]/-2,p[3]/2-starSelectionLabel->getTextHeight()/2);
			starSelectionLabel->draw();
		glPopMatrix();
		// Done drawing the Star Selection Mode banner.

		// If the user is mousing over the galaxy...
		if (collide_flag)
			// If the user is mousing over a sector...
			if (selected != NULL)
			{
				Star* star = selected->getSelected();
				
				// If the user is mousing over a star, draw the star's label.
				if (star != NULL)
				{
					// Try to initialize the text label. If it already exists,
					// this does nothing.
					star->initLabel();
					
					// Galaxy is being scaled to window. The star's label's
					// coordinates also need to be scaled, if they are to hover
					// over their star.
					float d = star->getDistance();
					float a = star->getAngle() + rotZ;
					
					float x = d*cos(a*M_PI/180);
					float y = d*sin(a*M_PI/180);
					float w = star->getLabel()->getWidth();
					float h = star->getLabel()->getHeight();
					
					x *= ((side-5)/2)/radius;
					y *= ((side-5)/2)/radius;

					// Get the label away from the edges of the viewport
					if (x-w/2 < -p[2]/2.0) x = -p[2]/2.0 + w/2;
					if (x+w/2 > p[2]/2.0) x = p[2]/2.0 - w/2;
					if (y-h/2 < -p[3]/2.0) y = -p[3]/2.0 + h/2;
					if (y+h/2 > p[3]/2.0) y = p[3]/2.0 - h/2;					
					
					// Set the label's new position.
					star->getLabel()->setPosition(x,y);
					
					// Shift the label up a little, and draw.
					glPushMatrix();
						glTranslatef(0,0,2);
						star->drawLabel();
					glPopMatrix();
				}
			}
	}
	// End star selection mode drawing.
	
	// Turn off blending.
	glDisable(GL_BLEND);
	
	rotZ += rotSpeed;
		if (rotZ > 360) rotZ -= 360;
}
//==============================================================================
