//==============================================================================
// Date Created:		16 February 2011
// Last Updated:		28 April 2011
//
// File name:			MimeIdentifier.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that identifies the mime-type of a
//						file, using libmagic.
//==============================================================================

#include "global_header.h"
#include "fs_structs.h"
#include <magic.h>

#ifndef MIMEIDENTIFIER
#define MIMEIDENTIFIER

class MimeIdentifier
{
	private:
		list< vector<string> > default_apps;
		
		void buildDefaultAppsList();
		void setDefaultApp(filenode *f);
		void setFileType(filenode *f);
		void enumFileType(filenode *f);
		
	public:
		MimeIdentifier();
		void obtainType(filenode *f);
};

#endif
