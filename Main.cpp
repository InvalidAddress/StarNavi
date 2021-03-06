//==============================================================================
// Date Created:		14 February 2011
// Last Updated:		17 May 2011
//
// File name:			MainClass.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class to test the functionality of the Galaxy	drawing
//						class. Renamed from test_main.cpp, now that there's
//						enough to work with to stop calling it a test file.
//==============================================================================

#include <GL/glut.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "Container.h"
#include "StateManager.h"
#include "DrawableList.h"
#include "TagsList.h"
#include "StatusBar.h"
#include "Button.h"

#define START_W 800
#define START_H 600

//==============================================================================
// Method definitions.
//==============================================================================
void init();
void buildGUI();

void display();
void idleFunc();
void reshape(int w, int h);

void mouseClick(int button, int state, int x, int y);
void mouseHover(int x, int y);
//==============================================================================


//==============================================================================
// Global variables.
//==============================================================================
GLuint Star::star_texture = 0;
bool Star::starSelectionMode = false;

list<Container*> containers;
int oldW = START_W, oldH = START_H;
int oldX = 0, oldY = 0;
int delay = 0;
string path;
//==============================================================================


//==============================================================================
// Initializers.
//==============================================================================
void init()
// Set clear color and shading model and initialize systems.
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	
	glEnable(GL_TEXTURE_2D);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	
	if (TTF_Init() == -1)
		exit(1);
}

void buildGUI()
{
	// Create the galaxy state manager and bind it to a container
	StateManager *sm = new StateManager(path);
	Functor<StateManager> *f_sm = new Functor<StateManager>(sm, &StateManager::navigate);

	// Create new container to hold state manager.
	Container *c1 = new Container(sm,f_sm,150,25,500,500);
	
	// Add state manager to list of containers.
	containers.push_back(c1);

//==============================================================================
// Buttons.
//==============================================================================
	// Create the drawables list, and associated functor.
	DrawableList *bl = new DrawableList(0,0,140,0);
	AbstractFunctor *f_bl = new Functor<DrawableList>(bl, &DrawableList::activate);
	
	// Create the back button, and add to the drawables list.
	AbstractFunctor *f_back = new Functor<StateManager>(sm, &StateManager::backward);
	Button *back = new Button("<--",f_back,0,0,140,30);
	bl->addDrawable(back);

	// Create the forward button, and add to the drawables list.
	AbstractFunctor *f_forward = new Functor<StateManager>(sm, &StateManager::forward);
	Button *forward = new Button("-->",f_forward,0,0,140,30);
	bl->addDrawable(forward);

	// Create the 'by directory' button, and add to the drawables list.
	AbstractFunctor *f_dir = new NullFunctor();
	Button *dir = new Button("By Directory",f_dir,0,0,140,30);
	bl->addDrawable(dir);

	// Create the 'by name' button, and add to the drawables list.
	AbstractFunctor *f_name = new NullFunctor();
	Button *name = new Button("By Name",f_name,0,0,140,30);
	bl->addDrawable(name);

	// Create the 'by date' button, and add to the drawables list.
	AbstractFunctor *f_date = new NullFunctor();
	Button *date = new Button("By Date",f_date,0,0,140,30);
	bl->addDrawable(date);

	// Create the 'by size' button, and add to the drawables list.
	AbstractFunctor *f_size = new NullFunctor();
	Button *size = new Button("By Size",f_size,0,0,140,30);
	bl->addDrawable(size);

	// Create the 'by type' button, and add to the drawables list.
	AbstractFunctor *f_type = new NullFunctor();
	Button *type = new Button("By Type",f_type,0,0,140,30);
	bl->addDrawable(type);
	
	// Create the tags list before the button, to properly create the functor.
	TagsList *tag_l = new TagsList(sm,0,0,140,0);
	
	// Create the 'by tags' button, and add to the drawables list.
	AbstractFunctor *f_tags = new Functor<TagsList>(tag_l, &TagsList::updateStateManager);
	Button *tags = new Button("By Tags",f_tags,0,0,140,30);
	bl->addDrawable(tags);

	// Add the tags list to the drawables list.
	bl->addDrawable(tag_l);

	// Create new container to hold the drawables list.
	Container *c2 = new Container(bl,f_bl,1,0,139,525,LEFT_UPPER);
	containers.push_back(c2);
//==============================================================================
	
	// Create new container to hold file/sector info.
	Container *c3 = new Container(NULL,new NullFunctor(),660,0,140,525,LEFT_UPPER);
	containers.push_back(c3);	
	
	// Create new container to hold path buttons and location bar.
	Container *c4 = new Container(NULL,new NullFunctor(),1,535,799,64);
	containers.push_back(c4);
	
	// Make a status bar, to show the current galaxy's name and number of files,
	// and add it to the list of containers
	StatusBar *sb = new StatusBar(sm);
	Container *c5 = new Container(sb,new NullFunctor(),150,0,500,15);
	containers.push_back(c5);
}
//==============================================================================


//==============================================================================
// Display methods.
//==============================================================================
void display()
// Draw all of the containers' contents.
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (list<Container*>::iterator i = containers.begin(); i != containers.end(); i++)
		(*i)->draw();
	
	glFlush();
	glutSwapBuffers();
	
	delay++;
}

void idleFunc()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{	
	float wRatio = (float)w/(float)START_W;
	float hRatio = (float)h/(float)START_H;
	
	for (list<Container*>::iterator i = containers.begin(); i != containers.end(); i++)
	{
		// Scale the containers to their new sizes.
		(*i)->scale(wRatio, hRatio);
		
		float oldX = (*i)->getOriginalX();
		float oldY = (*i)->getOriginalY();
		
		float newX = oldX * wRatio;
		float newY = oldY * hRatio;
		
		// Reposition the containers.
		(*i)->setPosition(newX,newY);
	}
	
	oldW = w;
	oldH = h;
	
	cout << oldW << "  " << oldH << endl;
	
	glutPostRedisplay();
}
//==============================================================================


//==============================================================================
// Mouse input
//==============================================================================
void mouseClick(int button, int state, int x, int y)
{
	if (delay < 50)
		return;
	
	delay = 0;
	
	if (button == GLUT_RIGHT_BUTTON)
	{
		Star::starSelectionMode = !Star::starSelectionMode;
		cout << "star selection mode " << Star::starSelectionMode << endl;
	}
		
	if (button == GLUT_LEFT_BUTTON)
	{
		// Invert the y coord.
		int newY = oldH-y;
	
		for (list<Container*>::iterator i = containers.begin(); i != containers.end(); i++)
			if ((*i)->isColliding(x,newY))
				(*i)->activate();
	}
}	

void mouseHover(int x, int y)
{	
	// Invert the y coord.
	int newY = oldH-y;
	
	for (list<Container*>::iterator i = containers.begin(); i != containers.end(); i++)
		(*i)->isColliding(x,newY);
}
//==============================================================================


//==============================================================================
// Main method
//==============================================================================
int main(int argc, char *argv[])
{	
	// Initialize SDL.
	SDL_Init(SDL_INIT_EVERYTHING);
	
	// Set up the GLUT main window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(oldW, oldH);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("StarNavi");
	
	// Initialize the environment.
	init();

	// Set the path
	if (argc > 2)
		return 1;
		
	if (argc == 1)
		path = "./";
	else
		path = (string)argv[1];
	
	// Build the GUI components.
	buildGUI();

	// Register display methods
	glutDisplayFunc(display);
	glutIdleFunc(idleFunc);
	glutReshapeFunc(reshape);
	
	// Register input methods.
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mouseHover);
	
	// Start the GLUT main loop.
	glutMainLoop();
	
	// Stop SDL.
	SDL_Quit();
	
	return 0;
}
//==============================================================================
