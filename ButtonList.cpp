//==============================================================================
// Date Created:		29 April 2011
// Last Updated:		3 May 2011
//
// File name:			ButtonList.h
// Programmer:			Matthew Hydock
//
// File description:	Class that contains and draws a list of	buttons. Manages
//						the separation between them, and for checking mouse
//						collisions.
//==============================================================================

#include "ButtonList.h"


//==============================================================================
// Constructor/Deconstructor.
//==============================================================================
ButtonList::ButtonList(float x, float y, float w, float h, int o, float vp, float hp)
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	
	orientation = o;
	vert_padding = vp;
	horz_padding = hp;
	
	buttons = new list<Button*>;
}

ButtonList::~ButtonList()
{
	delete(buttons);
}
//==============================================================================


//==============================================================================
// Button management.
//==============================================================================
void ButtonList::addButton(Button *b)
{
	b->setWidth(width);
	b->setPosition(0,0);
	
	buttons->push_back(b);
}
	
list<Button*>* ButtonList::getButtonsList()
{
	return buttons;
}

Button* ButtonList::getButton(size_t i)
{
	if (i > buttons->size())
	{
		cout << "button requested outside range\n";
		return NULL;
	}
	
	size_t j = 0;
	list<Button*>::iterator k = buttons->begin();
	for (;j < i && k != buttons->end(); k++)
		j++;
	
	return (*k);
}
		
//==============================================================================


//==============================================================================
// Setters and getters.
//==============================================================================
void ButtonList::setOrientation(int o)
{
	orientation = o;
}

int ButtonList::getOrientation()
{
	return orientation;
}


void ButtonList::setHorizPadding(float h)
{
	horz_padding = h;
}

void ButtonList::setVertPadding(float v)
{
	vert_padding = v;
}

float ButtonList::getHorizPadding()
{
	return horz_padding;
}

float ButtonList::getVertPadding()
{
	return vert_padding;
}
//==============================================================================


//==============================================================================
// Methods for user interaction.
//==============================================================================
void ButtonList::activate()
{
	for (list<Button*>::iterator i = buttons->begin(); i != buttons->end(); i++)
		if ((*i)->getCollideFlag())
		{
			(*i)->activate();
			break;
		}
}
	
bool ButtonList::isColliding(float x, float y)
{
	list<Button*>::iterator i = buttons->begin();
	float localX = x-(xPos + ((*i)->getWidth()/2));
	float localY = y-(yPos - ((*i)->getHeight()/2));
	
	cout << x << ", " << y << " | " << localX << ", " << localY << endl;
	
	collide_flag = false;
	
	for (; i != buttons->end(); i++)
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


void ButtonList::draw()
{
	glPushMatrix();
		list<Button*>::iterator i = buttons->begin();
		float x = xPos + ((*i)->getWidth()/2);
		float y = yPos - ((*i)->getHeight()/2);
		
		for (; i != buttons->end(); i++)
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
