/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2006  Colin Fitzpatrick & Jonathan Hughes
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
#include "misc.h"
#include "../common/player.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include <sstream>

/*
 * Definitions.
 */
#define KEY_TRANS3 _T("Software\\VB and VBA Program Settings\\RPGToolkit3\\Trans3")

/*
 * Split a string.
 */
void split(const STRING str, const STRING delim, std::vector<STRING> &parts)
{
	STRING::size_type pos = STRING::npos, begin = 0;
	const STRING::size_type length = delim.length();
	while ((pos = str.find(delim, pos + 1)) != STRING::npos)
	{
		parts.push_back(str.substr(begin, pos - begin));
		begin = pos + length;
	}
	parts.push_back(str.substr(begin, pos - begin));
}

/*
 * Replace text in a string.
 */
STRING &replace(STRING &str, const STRING find, const STRING replace)
{
	unsigned int pos = STRING::npos;
	while ((pos = str.find(find)) != STRING::npos)
	{
		str.erase(pos, find.length());
		str.insert(pos, replace);
	}
	return str;
}

/*
 * Replace within a string.
 *
 * str (in) - string in question
 * find (in) - find this
 * replace (in) - replace with this
 * return (out) - result
 */
STRING replace(const STRING str, const char find, const char replace)
{
	STRING toRet = str;
	const int len = str.length();
	for (int i = 0; i < len; i++)
	{
		if (toRet[i] == find) toRet[i] = replace;
	}
	return toRet;
}

/*
 * Save a setting.
 *
 * strKey (in) - name of key to save to
 * dblValue (in) - value to save
 */
void saveSetting(const STRING strKey, const double dblValue)
{
	HKEY hKey;
	RegCreateKey(HKEY_CURRENT_USER, KEY_TRANS3, &hKey);
	STRINGSTREAM ss;
	ss << dblValue;
	RegSetValueEx(hKey, strKey.c_str(), 0, REG_SZ, LPBYTE(ss.str().c_str()), strlen(ss.str().c_str()) + 1);
	RegCloseKey(hKey);
}

/*
 * Get a setting.
 *
 * strKey (in) - name of key to get
 * dblValue (out) - result
 */
void getSetting(const STRING strKey, double &dblValue)
{
	HKEY hKey;
	RegCreateKey(HKEY_CURRENT_USER, KEY_TRANS3, &hKey);
	TCHAR str[255];
	DWORD dwSize = sizeof(str);
	if (FAILED(RegQueryValueEx(hKey, strKey.c_str(), 0, NULL, (unsigned char *)str, &dwSize)))
	{
		dblValue = -1;
	}
	else
	{
#ifndef _UNICODE
		dblValue = atof(str);
#else
		dblValue = atof(getAsciiString(std::wstring(str)).c_str());
#endif
	}
	RegCloseKey(hKey);
}

/*
 * Create a temporary file, we would normally use the tmpfile() function 
 * found in the standard library but this has some issues with Windows 7, 
 * so we'll emulate its functionality here.
 *
 * return (out) - A pointer to the temporary file.
 */
FILE* createTemporaryFile()
{
	char* name = _tempnam(NULL, NULL);

	if (!name)
	{
		return 0;
	}

	FILE* filePointer = fopen(name, "wb+TD");

	// When name is no longer needed.
	if (name)
	{
		free(name);
	}

	return filePointer;
}
