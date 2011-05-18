//==============================================================================
// Date Created:		15 May 2011
// Last Updated:		17 May 2011
//
// File name:			ListItem.h
// Programmer:			Matthew Hydock
//
// File description:	Header for a class that acts as a selectable text item
//						in a graphical list.
//==============================================================================

#include "Drawable.h"
#include "DrawText.h"

#ifndef LISTITEM
#define LISTITEM

class ListItem:public Drawable
{
	private:
		DrawText* text;
		bool selected;
		
		float selectionColor[4];
		float selectedTextColor[4];
		float defaultTextColor[4];
		
	public:
		ListItem(string t, string f = "/usr/share/fonts/truetype/freefont/FreeSans.ttf", int size = 12, float x = 0, float y = 0, text_align a = LEFT);
		~ListItem();
		
		void setPosition(float x, float y);
		void setPosX(float x);
		void setPosY(float y);
		
		void setText(string t);
		string getText();
		DrawText* getTextObject();
		
		void toggleSelected();
		bool isSelected();
		
		void setSelectionColor(float r, float g, float b, float a);
		void setSelectionColor(float c[4]);
		float* getSelectionColor();
		
		void setSelectedTextColor(float r, float g, float b, float a);
		void setSelectedTextColor(float c[4]);
		float* getSelectedTextColor();
		
		void setDefaultTextColor(float r, float g, float b, float a);
		void setDefaultTextColor(float c[4]);
		float* getDefaultTextColor();
		
		void activate();
		
		void draw();
};
#endif
