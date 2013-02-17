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

	void init(std::string font, int size, long colordefault, long colorhighlight, long colordisable, bool bold, bool italics, bool underline, bool center, bool outline)
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

	SystemFont(std::string font, int size, long colordefault, long colorhighlight, long colordisable, bool bold, bool italics, bool underline, bool center, bool outline)
	{
		init(font, size, colordefault, colorhighlight, colordisable, bold, italics, underline, center, outline);
	}

	~SystemFont()
	{
		strFont.clear();
	}
};

#endif