/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2006-2014 
 *				- Colin James Fitzpatrick
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

/*
 * Implementation of string parsing functions.
 */

/*
 * Inclusions
 */
#include "parser.h"

/*
 * Trim whitespace from the sides of a string.
 *
 * str (in) - string to trim
 * return (out) - trimmed string
 */
STRING parser::trim(const STRING str)
{
	const int len = str.length();

	if (len == 0)
	{
		return _T("");
	}

	int start = -1, end = -1;
	for (int i = 0; i < len; i++)
	{
		if (str[i] != _T(' ') && str[i] != _T('\t'))
		{
			start = i;
			break;
		}
	}

	if (start == -1)
	{
		return _T("");
	}

	for (int j = len - 1; j >= 0; j--)
	{
		if (str[j] != _T(' ') && str[j] != _T('\t'))
		{
			end = j + 1 - start;
			break;
		}
	}

	return str.substr(start, end);
}
