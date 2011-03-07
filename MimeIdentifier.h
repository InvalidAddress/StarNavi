//==============================================================================
// Date Created:		16 February 2011
// Last Updated:		17 February 2011
//
// File name:			MimeIdentifier.h
// Programmer:			Matthew Hydock
//
// File description:	Header to a class that identifies the mime-type of a
//						file, based on its extension.
//==============================================================================

#include "global_header.h"
#include "fs_structs.h"

#ifndef MIMEIDENTIFIER
#define MIMEIDENTIFIER

class MimeIdentifier
{
	private:
		list<vector<string> > mimetypes;
		void createMimeTable();
		string getExtension(string f);
		string getFileType(string f);
		filetype enumFileType(string f);
		
	public:
		MimeIdentifier();
		filetype obtainType(string f);
};

#endif
