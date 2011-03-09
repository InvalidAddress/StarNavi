//==============================================================================
// Date Created:		14 February 2011
// Last Updated:		21 February 2011
//
// File name:			test main.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class to test the functionality of the Galaxy	drawing
//						class.
//==============================================================================

#include <GL/glut.h>
#include "Galaxy.h"

#define WIDTH 640
#define HEIGHT 480

Galaxy *galaxy;
GLuint Star::star_texture = 0;

float orthoW = WIDTH;
float orthoH = HEIGHT;

void init()
// Set clear color and shading model, initialize variables, and make menu.
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.01);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glLoadIdentity();
	
	glViewport(0,0,WIDTH,HEIGHT);
	
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-orthoW/2,orthoW/2,-orthoH/2,orthoH/2,-10,10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glScalef(HEIGHT/2,HEIGHT/2,1);
	galaxy->draw();
	
	glFlush();
	glutSwapBuffers();
}

void idleFunc()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	float ratio = (float)w/(float)h;
	
	if (w < h)
	{
		orthoW = HEIGHT;
		orthoH = HEIGHT / ratio;
	}
	else if (w > h)
	{
		orthoW = HEIGHT * ratio;
		orthoH = HEIGHT;
	}
	else if (w == h)
	{
		orthoW = HEIGHT;
		orthoH = HEIGHT;
	}
	
	cout << orthoW << "  " << orthoH << endl;
	
	glViewport(0,0,w,h);
}

int main(int argc, char *argv[])
{
	// set up main window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("StarDM");
	
	init();

	// Set up the galaxy.
	string path;
	
	if (argc > 2)
		return 1;
		
	if (argc == 1)
		path = "./";
	else
		path = (string)argv[1];
		
	cout << path << "\n";
	
	galaxy = new Galaxy(path);

	// register display methods
	glutDisplayFunc(display);
	glutIdleFunc(idleFunc);
	glutReshapeFunc(reshape);
	
	glutMainLoop();

	return 0;
}
