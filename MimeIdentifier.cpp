//==============================================================================
// Date Created:		16 February 2011
// Last Updated:		28 April 2011
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
void MimeIdentifier::buildDefaultAppsList()
{
	ifstream default_file("/usr/share/applications/defaults.list");
	string line;
	vector<string>* toks = NULL;
	
	// Begin reading lines and looking for the appropriate type.
	getline(default_file,line);
	getline(default_file,line);
	while (!default_file.eof())
	{
		toks = tokenizeV(line,"=");

		if (toks != NULL) default_apps.push_back(*toks);
		
		getline(default_file,line);
	}
	
	default_file.close();
}

void MimeIdentifier::setDefaultApp(filenode *f)
{
	for (list< vector<string> >::iterator i = default_apps.begin(); i != default_apps.end(); i++)
		if (f->mimetype.compare((*i)[0]) == 0)
		{
			f->default_app = (*i)[1].substr(0,(*i)[1].find_last_of('.'));
			return;
		}
		
	f->default_app = "gedit";
}

void MimeIdentifier::setFileType(filenode* f)
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

	f->mimetype = temp2.substr(0,temp2.find_first_of(';'));
		
	magic_close(magic_cookie);
}

void MimeIdentifier::enumFileType(filenode* f)
// Take a string representation of a file type, and turn it into an enumeration.
{
	string type = f->mimetype.substr(0,f->mimetype.find_first_of('/'));

	if (type.compare("") == 0)	
		f->mime_enum = UNKNOWN;
	else if (type.compare("binary") == 0)
		f->mime_enum = BIN;
	else if (type.compare("application") == 0)
		f->mime_enum = APP;
	else if (type.compare("audio") == 0)
		f->mime_enum = AUDIO;
	else if (type.compare("image") == 0)
		f->mime_enum = IMAGE;
	else if (type.compare("text") == 0)
		f->mime_enum = TEXT;
	else if (type.compare("video") == 0)
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
	buildDefaultAppsList();
}

void MimeIdentifier::obtainType(filenode* f)
// Scan through the mime table, and return the type of the requested file.
{
	setFileType(f);
	enumFileType(f);
	
	setDefaultApp(f);
}
//==============================================================================
