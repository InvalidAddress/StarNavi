//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		23 March 2011
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

class Galaxy:Drawable
{
	private:
		float rotX;
		float rotY;
		float rotZ;
		float rotSpeed;
		list<GSector*> *sectors;
		list<filenode*> *files;
		dirnode *root;
		
		GLuint texture;
		GLbyte *tex_data;
		int tex_size;
		float diameter;
		float radius;
		float thickness;
		
		cluster_type mode;
		
	public:
		Galaxy(dirnode *r, list<filenode*> *f = NULL);
		~Galaxy();
		
		void setRotation(float x, float y);
		void setRotationSpeed(float s);
		float getRotationX();
		float getRotationY();
		float getRotationSpeed();
		
		void setDirectory(dirnode *r);
		dirnode* getDirectory();
		
		void buildSectors();
		void buildHierarchy();
		void clearSectors();
		
		void initTexture();
		void refreshTex();
		void draw();
};

#endif
