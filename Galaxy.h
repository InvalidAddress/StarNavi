//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		17 April 2011
//
// File name:			Galaxy.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that draws a list of files as Stars in
//						a galaxy.
//
//						As it extends the Drawable class, it must implement a
//						draw() method.
//==============================================================================

#include "GSector.h"

#ifndef GALAXY
#define GALAXY

enum cluster_type{DIRECTORY,TIME,NAME,TAG};

class Galaxy:public Drawable
{
	private:
		// How the galaxy is drawn (external).
		float xPos;
		float yPos;
		float side;
		
		// Define the motion and angle of galaxy.
		float rotX;
		float rotY;
		float rotZ;
		float rotSpeed;
		
		// Dimensions of galaxy (internal).
		float diameter;
		float radius;
		float thickness;
		
		// Stores galaxy's files and directories.
		list<GSector*> *sectors;
		list<filenode*> *files;
		dirnode *root;
		
		string name;
		
		GSector* selected;
		
		// Render to texture.
		GLuint texture;
		GLbyte *tex_data;
		int tex_size;
		
		// How to cluster files in the galaxy.
		cluster_type mode;
		
	public:
		Galaxy(dirnode *r, list<filenode*> *f = NULL, cluster_type m = DIRECTORY, string n = "");
		~Galaxy();
		
		void setRotation(float x, float y);
		void setRotationSpeed(float s);
		float getRotationX();
		float getRotationY();
		float getRotationSpeed();
		
		void setMode(cluster_type m);
		cluster_type getMode();
		
		void setDirectory(dirnode *r);
		dirnode* getDirectory();
		
		void buildSectors();
		void buildHierarchy();
		void clearSectors();
		
		list<GSector*>* getSectors();
		
		void initTexture();
		void refreshTex();
		void draw();
		
		bool isColliding(float x, float y);
		GSector* getSelected();
};

#endif
