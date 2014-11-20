/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2006  Colin James Fitzpatrick
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
#include "paths.h"
#include "CFile.h"
#include "mbox.h"
#include "../../tkzip/tkzip.h"

/*
 * Globals.
 */
STRING g_projectPath;
STRING g_savePath = _T("Saved\\");	// May be altered for pack files.
STRING g_pakTempPath;				// Temp path for the pak file.
STRING g_pakFile;					// The file name of the pak file.
bool g_bStandalone = false;			// Are we a standalone game?

/*
 * Remove the path from a file name, perserving folders.
 * preserveFrom should carry a trailing \.
 */
STRING removePath(const STRING str, const STRING preserveFrom)
{
	const int pos = str.find(preserveFrom);
	if (pos != str.npos)
	{
		return str.substr(pos + preserveFrom.length());
	}
	// Return the path unaltered, since it didn't contain the
	// default folder and any other folders should be subfolders
	// (although a different default folder may be present).
	return str;
}

// Remove the path from a file name.
STRING removePath(const STRING str)
{
	return str.substr(str.find_last_of(_T('\\')) + 1);
}

// Don't resolve a file name.
STRING resolveNonPakFile(const STRING path)
{
	return path;
}

// Resolve a file name
STRING resolvePakFile(const STRING path)
{
	if (path.find("Saved\\", 0) == 0)
	{ 
		return path;
	}

	const STRING file = g_pakTempPath + path;
	resolve = resolveNonPakFile;

	if (!CFile::fileExists(file))
	{
		// Extract the file from the pak file or this executable.
		// (Assume the zip is open!)
		ZIPExtract(const_cast<char *>(path.c_str()), const_cast<char *>(file.c_str()));
	}

	resolve = resolvePakFile;

	return file;
}

void setResolve(const bool bPak)
{
	resolve = bPak ? resolvePakFile : resolveNonPakFile;
}

bool initialisePakFile(const STRING file)
{
//	MessageBox(NULL, _T("We are checking PakFile"),_T("Just Checking"),0);

	TCHAR pstrTemp[_MAX_PATH];
	GetTempPath(_MAX_PATH, pstrTemp);
	_tcscat(pstrTemp, _T("TKTemp"));

	g_pakTempPath = pstrTemp + STRING(_T("\\"));

	// Create the directory %temp%\TKTemp.
	if (FAILED(CreateDirectory(pstrTemp, NULL)))
	{
		MessageBox(NULL, _T("A temporary directory for this application could not be created."), _T("Directory error."), 0);
		return false;
	}

//	MessageBox(NULL, _T("Passed Directory Check"),_T("Just Checking"),0);

	// Set of all directories.
	const TCHAR *directories[] = {
		TILE_PATH, BRD_PATH, TEM_PATH, SPC_PATH, BKG_PATH, MEDIA_PATH, 
		PRG_PATH, FONT_PATH, ITM_PATH, ENE_PATH, BMP_PATH, STATUS_PATH, 
		MISC_PATH, PLUG_PATH};

	for (unsigned int i = 0; i < 14; ++i)
	{
		CreateDirectory((pstrTemp + STRING(_T("\\")) + directories[i]).c_str(), NULL);
	}

//	MessageBox(NULL, _T("CreatedDirectories"),_T("Just Checking"),0);

	// Open the archive.
	if (!ZIPOpen(const_cast<char *>(file.c_str())))
	{
		MessageBox(NULL, _T("The PAK file or executable could not be successfully opened and is probably invalid."), 
			_T("Invalid PAK File"), 0);
	}

	g_pakFile = file;

//	MessageBox(NULL, _T("Set Pak File"),_T("Just Checking"),0);

	setResolve(true);

//	MessageBox(NULL, _T("Resolve is true"),_T("Just Checking"),0);

	return true;
}

// Detect whether we are a standalone game.
bool isStandaloneGame()
{
	extern HINSTANCE g_hInstance;

	char path[_MAX_PATH];
	GetModuleFileNameA(g_hInstance, path, _MAX_PATH);

	if (!ZIPIsCompoundFile(path))
	{
		return false;
	}

	// We are a standalone file, so let's cheat a bit and
	// do some stuff here.
	TCHAR pstrTemp[_MAX_PATH];
	GetTempPath(_MAX_PATH, pstrTemp);
	_tcscat(pstrTemp, _T("temp.tpk"));

	if (!ZIPExtractCompoundFile(path, pstrTemp))
	{
		MessageBox(NULL, _T("This application is corrupt."), _T("Corrupt application"), 0);
		return false;
	}

	initialisePakFile(pstrTemp);

	return (g_bStandalone = true);
}

// Delete a tree.
// 'path' should include a trailing backslash.
void deleteTree(const STRING path)
{
	WIN32_FIND_DATA fd;

	HANDLE hSearch = FindFirstFile((path + _T("*")).c_str(), &fd);
	do
	{
		const STRING strFile = fd.cFileName;

		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Delete this directory too.
			if ((strFile != _T(".")) && (strFile != _T("..")))
			{
				deleteTree(path + strFile + _T("\\"));
			}
		}
		else
		{
			// Unlink each file.
			_tunlink((path + fd.cFileName).c_str());
		}
	} while (FindNextFile(hSearch, &fd));
	FindClose(hSearch);

	// Then remove the directory.
	RemoveDirectory(path.c_str());
}

void uninitialisePakFile()
{
	// Return if this isn't a pak file.
	if (g_pakTempPath.empty()) return;

	deleteTree(g_pakTempPath);
	ZIPClose();

	// If we are a standalone game, also delete the tag on archive.
	if (g_bStandalone)
	{
		unlink(g_pakFile.c_str());
	}
}
