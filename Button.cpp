#include "Button.h"

Button::Button(string l, AbstractFunctor *f, float x, float y, float w, float h, bool active)
// Make a button with a label l, that performs action f when clicked.
{
	label = l;
	act = f;
	
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	
	interactive = active;
	
	buffer = new FTBufferFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf");
	if (buffer->Error())
	{
		cout << "error occured in font generation\n";
		exit(1);
	}
	buffer->FaceSize(70);
}

Button::~Button()
{
}
		

bool Button::isColliding(float x, float y)
// Check to see if the given coordinate is inside the button.
{
	float w = width/2;
	float h = height/2;
	
	collide_flag = (x >= xPos-w) && (x <= xPos+w) && (y >= yPos-h) && (y <= yPos+h);

	if (collide_flag)
	{
		cout << "colliding with button " << label << endl;
		activate();
	}
}	

bool Button::isInteractive()
// Is the button clickable?
{
	return interactive;
}

void Button::setInteract(bool c)
// Set whether the button is clickable or not.
{
	interactive = c;
}

		
void Button::setAction(AbstractFunctor *f)
// Set the button's action.
{
	act = f;
}

void Button::activate()
// Perform the button's assigned action.
{
	act->Call();
}


void Button::draw()
// Draw a rectangle with a gradient, dark on the far left and right, white in
// the middle. Try to center the text.
{
	glPushMatrix();
		glScalef(width,height,1);
		glTranslatef(xPos,yPos,0);
				
		glBegin(GL_QUADS);
			glColor4f(0,0,0,0);
			glVertex2d(-1,1);
			glColor4f(.25,.25,.25,1);
			glVertex2d(0,1);
			glVertex2d(0,-1);
			glColor4f(0,0,0,0);
			glVertex2d(-1,-1);

			glColor4f(.25,.25,.25,1);
			glVertex2d(0,1);
			glColor4f(0,0,0,0);
			glVertex2d(1,1);
			glVertex2d(1,-1);
			glColor4f(.25,.25,.25,1);
			glVertex2d(0,-1);
		glEnd();
		
		glColor4f(0,0,0,1);
//		glTranslatef(xPos,yPos,0.0);
		buffer->Render(label.c_str());
	glPushMatrix();
}
