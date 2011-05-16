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
	
	anchor = CENTER;
	
	orientation = o;
	vert_padding = vp;
	horz_padding = hp;
	
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
	d->setWidth(width);
	d->setPosition(0,0);
	
	drawables->push_back(d);
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
	list<Drawable*>::iterator i = drawables->begin();
	float localX = x-xPos;
	float localY = y-yPos;
	
	switch(anchor)
	{
		case CENTER			: localX -= (*i)->getWidth()/2;
							  localY += (*i)->getHeight()/2;
							  break;
		case LEFT_UPPER		: localX -= 0;
							  localY += (*i)->getHeight();
							  break;
		case RIGHT_UPPER	: localX -= (*i)->getWidth();
							  localY += (*i)->getHeight();
							  break;
		case RIGHT_LOWER	: localX -= (*i)->getWidth();
							  localY += 0;
							  break;
		case LEFT_LOWER		: localX -= 0;
							  localY += 0;
							  break;
	}
	
//	cout << x << ", " << y << " | " << localX << ", " << localY << endl;
	
	collide_flag = false;
	
	for (; i != drawables->end(); i++)
	{
		collide_flag = collide_flag || (*i)->isColliding(localX,localY);
		if (orientation == HORIZONTAL)
			localX -= ((*i)->getWidth() + horz_padding);
		if (orientation == VERTICAL)
			localY += ((*i)->getHeight() + vert_padding);
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
		list<Drawable*>::iterator i = drawables->begin();
		float x = xPos + ((*i)->getWidth()/2);
		float y = yPos - ((*i)->getHeight()/2);
		
		for (; i != drawables->end(); i++)
		{			
			glTranslatef(x,y,0);
			(*i)->draw();
			
			x = 0;
			y = 0;
			
			if (orientation == HORIZONTAL)
				x = ((*i)->getWidth()+horz_padding);
			if (orientation == VERTICAL)
				y = -((*i)->getHeight()+vert_padding);
		}
	glPopMatrix();
}
