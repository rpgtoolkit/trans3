/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2006  Christopher Matthews & contributors
 *
 * Contributors:
 *    - Colin James Fitzpatrick
 *    - Jonathan D. Hughes
 ********************************************************************
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

/*
 * Inclusions.
 */
#include "tilebitmap.h"
#include "CFile.h"
#include "paths.h"
#include "mbox.h"
#include "../../tkCommon/tkGfx/CTile.h"

/*
 * Open a tile bitmap.
 *
 * fileName (in) - file to open
 * bool (out) - open success
 */
bool tagTileBitmap::open(const STRING fileName)
{

	CFile file(fileName);

	STRING fileHeader;
	file >> fileHeader;

	if (fileHeader != _T("TK3 TILEBITMAP"))
	{
		messageBox(_T("Invalid tile bitmap ") + fileName);
		return false;
	}

	short majorVer, minorVer;
	file >> majorVer;
	file >> minorVer;

	file >> width;
	file >> height;
	resize(width, height);

	for (unsigned int i = 0; i <= width; i++)
	{
		for (unsigned int j = 0; j <= height; j++)
		{
			file >> tiles[i][j];
			file >> red[i][j];
			file >> green[i][j];
			file >> blue[i][j];
		}
	}
	return true;
}

/*
 * Save a tile bitmap.
 *
 * fileName (in) - file to save to
 */
void tagTileBitmap::save(const STRING fileName) const
{

	CFile file(fileName, OF_CREATE | OF_WRITE);

	file << _T("TK3 TILEBITMAP");
	file << short(3);
	file << short(0);

	file << width;
	file << height;

	for (unsigned int i = 0; i <= width; i++)
	{
		for (unsigned int j = 0; j <= height; j++)
		{
			file << tiles[i][j];
			file << red[i][j];
			file << green[i][j];
			file << blue[i][j];
		}
	}

}

/*
 * Resize a tile bitmap.
 *
 * width (in) - new width
 * height (in) - new height
 */
void tagTileBitmap::resize(const int width, const int height)
{
	this->width = width;
	this->height = height;
	tiles.clear();
	red.clear();
	green.clear();
	blue.clear();
	for (unsigned int i = 0; i <= width; i++)
	{
		tiles.push_back(VECTOR_STR());
		VECTOR_STR &a = tiles.back();
		//
		red.push_back(VECTOR_SHORT());
		VECTOR_SHORT &b = red.back();
		//
		green.push_back(VECTOR_SHORT());
		VECTOR_SHORT &c = green.back();
		//
		blue.push_back(VECTOR_SHORT());
		VECTOR_SHORT &d = blue.back();
		for (unsigned int j = 0; j <= height; j++)
		{
			a.push_back(_T(""));
			b.push_back(0);
			c.push_back(0);
			d.push_back(0);
		}
	}
}

/*
 * Draw a tile bitmap, including tst.
 * Called when rendering sprite frames.
 */
bool tagTileBitmap::draw(CCanvas *cnv, 
						 CCanvas *cnvMask, 
						 const int x, 
						 const int y)
{
	extern STRING g_projectPath;
	extern AMBIENT_LEVEL g_ambientLevel;
	const RGBSHADE al = g_ambientLevel.rgb;

    const int xx = x / 32 + 1, yy = y / 32 + 1;

    for (int i = 0; i != width; ++i)
	{
        for (int j = 0; j != height; ++j)
		{
            if (!tiles[i][j].empty())
			{
                if (cnv)
				{
					CTile::drawByBoardCoord(
						g_projectPath + TILE_PATH + tiles[i][j],
						i + xx, 
						j + yy,						
						red[i][j] + al.r, 
						green[i][j] + al.g,
						blue[i][j] + al.b,
						cnv,
						TM_NONE,
						0, 0,
						TILE_NORMAL, 0		// No isometric tbm implementation.
					);
                }                
                if (cnvMask)
				{
					CTile::drawByBoardCoord(
						g_projectPath + TILE_PATH + tiles[i][j],
						i + xx, 
						j + yy,						
						red[i][j], 
						green[i][j],
						blue[i][j],
						cnvMask,
						TM_COPY,
						0, 0,
						TILE_NORMAL, 0		// No isometric tbm implementation.
					);                      
	             }
            }
			else
			{
                if (cnv && cnvMask)
				{
//                    Call canvasFillBox(cnv, x + i * 32, y + j * 32, x + 32 + i * 32, y + 32 + j * 32, TRANSP_COLOR_ALT)
//                    Call canvasFillBox(cnvMask, x + i * 32, y + j * 32, x + 32 + i * 32, y + 32 + j * 32, TRANSP_COLOR)
                }
            } // if (!tiles[i][j].empty())
		}	// for (j)
	} // for (i)

	return true;        
}