//==============================================================================
// Date Created:		29 April 2011
// Last Updated:		15 May 2011
//
// File name:			DrawableList.h
// Programmer:			Matthew Hydock
//
// File description:	Class that contains and draws a list of	drawables.
//						Manages the separation between them, and for checking
//						mouse collisions.
//
//						Was originally just for buttons, but has since been
//						repurposed for all kinds of drawables.
//==============================================================================

#include "DrawableList.h"


//==============================================================================
// Constructor/Deconstructor.
//==============================================================================
DrawableList::DrawableList(float x, float y, float w, float h, int o, float vp, float hp)
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	
	anchor = LEFT_UPPER;
	
	orientation = o;
	vert_padding = vp;
	horz_padding = hp;
	
	if (orientation == HORIZONTAL)
		width = 0;
	if (orientation == VERTICAL)
		height = 0;
	
	drawables = NULL;
	drawables = new list<Drawable*>;
}

DrawableList::~DrawableList()
{
	delete(drawables);
}
//==============================================================================


//==============================================================================
// Button management.
//==============================================================================
void DrawableList::addDrawable(Drawable* d)
{
	if (orientation == HORIZONTAL)
	{
		d->setPosition(width,0);
		d->setHeight(height);
		width += d->getWidth()+horz_padding;
	}
	if (orientation == VERTICAL)
	{
		d->setPosition(0,-height);
		d->setWidth(width);
		height += d->getHeight()+vert_padding;
	}
	
	d->setAnchor(anchor);
	drawables->push_back(d);
//	cout << d->getPosX() << " " << d->getPosY() << " " << height << endl;
}
	
list<Drawable*>* DrawableList::getDrawablesList()
{
	return drawables;
}

Drawable* DrawableList::getDrawable(size_t i)
{
	if (i > drawables->size())
	{
		cout << "drawable requested outside range\n";
		return NULL;
	}
	
	size_t j = 0;
	list<Drawable*>::iterator k = drawables->begin();
	for (;j < i && k != drawables->end(); k++)
		j++;
	
	return (*k);
}
		
//==============================================================================


//==============================================================================
// Setters and getters.
//==============================================================================
void DrawableList::setWidth(float w)
// Sets the width of the list, and all of its drawables.
{
	width = w;

	if (orientation == VERTICAL)
		for (list<Drawable*>::iterator i = drawables->begin(); i != drawables->end(); i++)
			(*i)->setWidth(w);
}

void DrawableList::setHeight(float h)
// Sets the height of the list, and all of its drawables.
{
	width = h;
	
	if (orientation == HORIZONTAL)
		for (list<Drawable*>::iterator i = drawables->begin(); i != drawables->end(); i++)
			(*i)->setHeight(h);
}

void DrawableList::setOrientation(int o)
{
	orientation = o;
}

int DrawableList::getOrientation()
{
	return orientation;
}


void DrawableList::setHorizPadding(float h)
{
	horz_padding = h;
}

void DrawableList::setVertPadding(float v)
{
	vert_padding = v;
}

float DrawableList::getHorizPadding()
{
	return horz_padding;
}

float DrawableList::getVertPadding()
{
	return vert_padding;
}
//==============================================================================


//==============================================================================
// Methods for user interaction.
//==============================================================================
void DrawableList::activate()
{
//	cout << "button list activated\n";
	
	for (list<Drawable*>::iterator i = drawables->begin(); i != drawables->end(); i++)
		if ((*i)->getCollideFlag())
		{
//			cout << "activating button...\n";
			(*i)->activate();
			break;
		}
}
	
bool DrawableList::isColliding(float x, float y)
{
	float localX = x-xPos;
	float localY = y-yPos;
	
//	cout << x << ", " << y << " | " << localX << ", " << localY << endl;
	
	collide_flag = false;
	bool temp;
	
	for (list<Drawable*>::iterator i = drawables->begin(); i != drawables->end(); i++)
	{
		temp = false;
		temp = (*i)->isColliding(localX,localY);
		collide_flag = collide_flag || temp;
	}
	
	return collide_flag;
}
//==============================================================================


void DrawableList::draw()
{
	// Set the size of the drawable list, based on the viewport.
	int p[4];
	glGetIntegerv(GL_VIEWPORT,p);
	
	if (orientation == VERTICAL) setWidth(p[2]);
	if (orientation == HORIZONTAL) setHeight(p[3]);

	glPushMatrix();
		//shiftToAnchor();		
		glTranslatef(xPos,yPos,0);
		
		for (list<Drawable*>::iterator i = drawables->begin(); i != drawables->end(); i++)
			(*i)->draw();
	glPopMatrix();
}
