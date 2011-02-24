//==============================================================================
// Date Created:		16 February 2011
// Last Updated:		17 February 2011
//
// File name:			MimeIdentifier.cpp
// Programmer:			Matthew Hydock
//
// File description:	A class that identifies the mime-type of a file, based
//						on its extension. The mime table is held in memory, to
//						make seeking faster.
//==============================================================================

#include "MimeIdentifier.h"

//==============================================================================
// Private methods.
//==============================================================================	
void MimeIdentifier::createMimeTable()
// Reads the mime.types file and turns it into a table in memory.
{
	ifstream mimefile("/etc/mime.types");
	string line;
	vector<string> toks;

	// Skip all of the lines that don't contain type/extension associations.
	for (getline(mimefile,line); line.compare("") != 0 && !mimefile.eof(); getline(mimefile,line));
	
	// Begin reading lines and looking for the appropriate type.
	getline(mimefile,line);
	while (!mimefile.eof())
	{
		toks = tokenize(line," \t");

		if (!toks.empty()) mimetypes.push_back(toks);
		
		getline(mimefile,line);
	}
	
	mimefile.close();
}

string MimeIdentifier::getExtension(string f)
// Tear the extention off of the file name.
{
	int i;
	for (i = f.size()-1; i > 0 && f.at(i) != '.'; i--);
	
	if (i > 0)
		return f.substr(i+1);
		
	return "";
}

string MimeIdentifier::getFileType(string f)
// Seeks through the mime filetype database on the user's computer, and attempts
// to determine the requested file's filetype.
{
	for (list<vector<string> >::iterator i = mimetypes.begin(); i != mimetypes.end(); i++)
		for (vector<string>::iterator j = (*i).begin(); j != (*i).end(); j++)
			if (f.compare(*j) == 0)
				return *((*i).begin());

	return "";
}

filetype MimeIdentifier::enumFileType(string t)
// Take a string representation of a file type, and turn it into an enumeration.
{
	string s = t.substr(0, t.find_first_of('/',0));
		
	if (s.compare("application") == 0)
		return APP;
	if (s.compare("audio") == 0)
		return AUDIO;
	if (s.compare("image") == 0)
		return IMAGE;
	if (s.compare("text") == 0)
		return TEXT;
	if (s.compare("video") == 0)
		return VIDEO;
		
	return UNKNOWN;
}
//==============================================================================


//==============================================================================
// Public methods.
//==============================================================================
MimeIdentifier::MimeIdentifier()
// Create the mime table.
{
	createMimeTable();
}

filetype MimeIdentifier::obtainType(string f)
// Scan through the mime table, and return the type of the requested file.
{
	return enumFileType(getFileType(getExtension(f)));
}
//==============================================================================
