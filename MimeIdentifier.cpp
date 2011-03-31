//==============================================================================
// Date Created:		16 February 2011
// Last Updated:		18 March 2011
//
// File name:			MimeIdentifier.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class that identifies the mime-type of a file. Used to
//						be a custom build, now a wrapper for libmagic.
//==============================================================================

#include "MimeIdentifier.h"

//==============================================================================
// Private methods.
//==============================================================================	
void MimeIdentifier::setFileType(filenode *f)
// Seeks through the mime filetype database on the user's computer, and attempts
// to determine the requested file's filetype.
{
	magic_t magic_cookie;
	
	/*MAGIC_MIME tells magic to return a mime of the file, but you can specify different things*/
	magic_cookie = magic_open(MAGIC_MIME);

	if (magic_cookie == NULL)
	{
		printf("unable to initialize magic library\n");
		exit(1);
	}

	if (magic_load(magic_cookie, NULL) != 0)
	{
		printf("cannot load magic database - %s\n", magic_error(magic_cookie));
		magic_close(magic_cookie);
		exit(1);
	}

	string temp1 = f->path+f->name;
	string temp2 = magic_file(magic_cookie, temp1.c_str());
	if (temp2.find_first_of("binary") != string::npos)
		f->mimetype = temp2.substr(0,temp2.find_first_of('/'));
	else
		f->mimetype = "binary";
		
	magic_close(magic_cookie);
}

void MimeIdentifier::enumFileType(filenode *f)
// Take a string representation of a file type, and turn it into an enumeration.
{
	if ((f->mimetype).compare("") == 0)	
		f->mime_enum = UNKNOWN;
	else if ((f->mimetype).compare("binary") == 0)
		f->mime_enum = BIN;
	else if ((f->mimetype).compare("application") == 0)
		f->mime_enum = APP;
	else if ((f->mimetype).compare("audio") == 0)
		f->mime_enum = AUDIO;
	else if ((f->mimetype).compare("image") == 0)
		f->mime_enum = IMAGE;
	else if ((f->mimetype).compare("text") == 0)
		f->mime_enum = TEXT;
	else if ((f->mimetype).compare("video") == 0)
		f->mime_enum = VIDEO;
	else	
		f->mime_enum = UNKNOWN;
}
//==============================================================================


//==============================================================================
// Public methods.
//==============================================================================
MimeIdentifier::MimeIdentifier()
// errm, do nothing?
{
}

void MimeIdentifier::obtainType(filenode *f)
// Scan through the mime table, and return the type of the requested file.
{
	setFileType(f);
	enumFileType(f);
}
//==============================================================================
