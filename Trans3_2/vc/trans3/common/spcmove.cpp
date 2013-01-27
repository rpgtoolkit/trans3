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

#include "spcmove.h"
#include "mbox.h"
#include "CFile.h"

// Open a special move.
bool tagSpcMove::open(const STRING strFile)
{
	CFile file(strFile);

	if (!file.isOpen())
	{
		messageBox(_T("File not found: ") + strFile);
		return false;
	}

	file.seek(16);
	char check;
	file >> check;
	file.seek(0);

	if (!check)
	{
		std::string header;
		file >> header;
		if (header != _T("RPGTLKIT SPLMOVE"))
		{
			messageBox(_T("Invalid special move: ") + strFile);
			return false;
		}

		short majorVer, minorVer;
		file >> majorVer >> minorVer;

		file >> name;
		file >> fp;
		file >> smp;
		file >> prg;
		file >> targSmp;
		file >> battle;
		file >> menu;
		file >> status;
		file >> animation;
		file >> description;
		return true;
	}

	std::string header = file.line();
	if (header != _T("RPGTLKIT SPLMOVE"))
	{
		messageBox(_T("Invalid special move: ") + strFile);
		return false;
	}

	file.line(); // majorVer
	file.line(); // minorVer

	name = file.line();
	fp = _ttoi(file.line().c_str());
	smp = _ttoi(file.line().c_str());
	prg = file.line();
	targSmp = _ttoi(file.line().c_str());
	battle = _ttoi(file.line().c_str());
	menu = _ttoi(file.line().c_str());
	status = file.line();
	animation = file.line();
	description = file.line();

	return true;
}
