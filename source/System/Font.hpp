/*
Copyright (C) 2004 Lion Vollnhals
Copyright (C) 2003 Matthias Braun

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef __LIB_2D_FONT_HPP__
#define __LIB_2D_FONT_HPP__

#include <string>
#include <stdexcept>
#include "SFont.h"

/** A thin c++ wrapper around my custom SFont version */
class SFont
{
public:
	int getHeight() const
	{ return SFont_TextHeight(font); }

	int getTextWidth(const char* text) const
	{ return SFont_TextWidth(font, text); }
	int getTextWidth(const std::string& text) const
	{ return getTextWidth(text.c_str()); }

	void write(SDL_Surface* surface, const char* text, int x, int y) const
	{ SFont_Write(surface, font, x, y, text); }
	
	void write(SDL_Surface* surface, const std::string& text, int x, int y) const
    { SFont_Write(surface, font, x, y, text.c_str()); }

	SFont(SDL_Surface* surface)
	{
		font = SFont_InitFont(surface);
		if(!font)
			throw std::runtime_error("Couldn't initialize font.");
	}
	~SFont()
	{ SFont_FreeFont(font); }
	
private:
	SFont_Font* font;
};

#endif


