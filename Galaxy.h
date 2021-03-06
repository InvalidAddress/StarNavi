//==============================================================================
// Date Created:		20 February 2011
// Last Updated:		17 May 2011
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

enum cluster_type{DIRECTORY,NAME,DATE,SIZE,TYPE,TAGS,NONE};

class Galaxy:public LabeledDrawable
{
	private:
		// How the galaxy is drawn (external).
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
		list<GSector*>* sectors;
		list<filenode*>* files;
		dirnode* root;
		
		list<string>* tags;
		
		GSector* selected;
		
		// Render to texture.
		GLuint texture;
		GLubyte* tex_data;
		int tex_size;
		
		DrawText* starSelectionLabel;
		
		// How to cluster files in the galaxy.
		cluster_type cluster_mode;
		
		void buildSectors();
		void buildHierarchy();
		void buildByName();
		void buildByDate();
		void buildBySize();
		void buildByType();
		void buildByTags();
		
		void adjustSectorWidths();
		void clearSectors();
		
	public:
		Galaxy(dirnode* r, list<filenode*>* f = NULL, cluster_type m = DIRECTORY, string n = "", list<string>* t = NULL);
		~Galaxy();
		
		void setName(string n);
		string getName();
		
		void setRotation(float x, float y);
		void setRotationSpeed(float s);
		float getRotationX();
		float getRotationY();
		float getRotationSpeed();
		
		void setClusterMode(cluster_type m);
		cluster_type getClusterMode();
		
		void setTags(list<string>* t);
		void rebuildTags();
		list<string>* getTags();
		
		void setDirectory(dirnode* r);
		dirnode* getDirectory();
		void setFileList(list<filenode*>* f);
		list<filenode*>* getFileList();
		
		list<GSector*>* getSectors();
		
		bool isColliding(float x, float y);
		GSector* getSelected();
		
		void initTexture();
		void refreshTex();
		void draw();
};

#endif
