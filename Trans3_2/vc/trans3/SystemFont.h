/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2013-2014 
 *				- LJ Gutierrez
 *
 * Contributors:
 *				- Joshua Michael Daly
 ********************************************************************
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Creating a game EXE using the Make EXE feature creates a 
 * derivative version of trans3 that includes the game's files. 
 * Therefore the EXE must be licensed under the GPL. However, as a 
 * special exception, you are permitted to license EXEs made with 
 * this feature under whatever terms you like, so long as 
 * Corresponding Source, as defined in the GPL, of the version 
 * of trans3 used to make the EXE is available separately under 
 * terms compatible with the Licence of this software and that you 
 * do not charge, aside from any price of the game EXE, to obtain 
 * these components.
 * 
 * If you publish a modified version of this Program, you may delete
 * these exceptions from its distribution terms, or you may choose
 * to propagate them.
 */

#ifndef SYSTEMFONT_H
#define SYSTEMFONT_H

#include "../../tkCommon/strings.h"

class SystemFont
{
public:
	std::string strFont;
	int iSize;
	long lColorDefault, lColorHighlight, lColorDisable;
	int iUseColor; // 0 - Default, 1 - hightlight , 2 - disable
	bool bBold, bItalics, bUnderline, bCenter, bOutline;

	void init(std::string font, int size, long colordefault, long colorhighlight, long colordisable, 
		bool bold, bool italics, bool underline, bool center, bool outline)
	{
		strFont = font;
		iSize = size;
		lColorDefault = colordefault;
		lColorHighlight = colorhighlight;
		lColorDisable = colordisable;
		bBold = bold;
		bItalics = italics;
		bUnderline = underline;
		bCenter = center;
		bOutline = outline;
		iUseColor = 0;
	}

	SystemFont()
	{
		init("", 12, RGB(0, 0, 0), RGB(255, 255, 255), RGB(128, 128, 128), false, false, false, false, false);
	}

	SystemFont(std::string font, int size, long colordefault, long colorhighlight, long colordisable, bool bold, 
		bool italics, bool underline, bool center, bool outline)
	{
		init(font, size, colordefault, colorhighlight, colordisable, bold, italics, underline, center, outline);
	}

	~SystemFont()
	{
		strFont.clear();
	}
};

#endif