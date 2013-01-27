/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2006  Christopher Matthews
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

// tkzip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "iostream"

#include "zip.h"
#include "unzip.h"
#include "tkzip.h"
#include "zipoperate.h"

unzFile g_uf;
zipFile g_zf;

int main(int argc, char* argv[])
{
	unzFile uf = OpenZip("testit.zip");
	//ExtractFiles(uf, "testdir\\");
	CloseZip(uf);

	/*zipFile zf = CreateZip("testit.zip");
	AddFiles(zf, "testit.zip", "*.*", "", true);
	CloseCreatedZip(zf);*/

	/*unzFile uf = OpenZip("c:\\windows\\profiles\\cbm\\desktop\\downloads\\alcatraz.zip");
	int a = FilesInZip(uf);
	for (int i=0; i<a; i++)
	{
		char file[MAX_PATH];
		GetFile(file, i, uf);
		cout << file << endl;
		char saveas[MAX_PATH];
		strcpy(saveas, "test\\");
		strcat(saveas, file);
		Extract(file, saveas, uf);
	}

	CloseZip(uf);*/
	return 0;
}
												

///////////////////////////////////
// DLL Entry point...
int APIENTRY ZIPTest()
{
	g_zf = 0;
	g_uf = 0;
	return 1;
}


int APIENTRY ZIPCreate(char* pstrZipToCreate, int nTackOntoEndYN)
{
	g_zf = CreateZip(pstrZipToCreate, nTackOntoEndYN);
	return 1;
}


int APIENTRY ZIPCloseNew()
{
	if (g_zf)
		CloseCreatedZip(g_zf);
	return 1;
}

int APIENTRY ZIPAdd(char* pstrToAdd, char* pstrAddAs)
{
	if (g_zf)
		Add(pstrToAdd, pstrAddAs, g_zf);
	return 1;
}


int APIENTRY ZIPOpen(char* pstrZipToOpen)
{
	g_uf = OpenZip(pstrZipToOpen);
	return 1;
}

int APIENTRY ZIPClose()
{
	if (g_uf)
		CloseZip(g_uf);
	return 1;
}


bool IsDir(char* pstrFilename)
{
	int l= strlen(pstrFilename);
	if (l>0)
		if (pstrFilename[l-1] == '\\' || pstrFilename[l-1] == '/')
			return true;
	return false;
}

int APIENTRY ZIPExtract(char* pstrToExt, char* pstrSaveAs)
{
	if (g_uf)
	{
		CreateDir(pstrSaveAs);
		if (!IsDir(pstrSaveAs))
			return Extract(pstrToExt, pstrSaveAs, g_uf);
	}
	return 0;
}


int APIENTRY ZIPGetFileCount()
{
	if (g_uf)
		return FilesInZip(g_uf);
	return 0;
}


int APIENTRY ZIPGetFile(int nFileNum, char* pstrFileOut)
{
	strcpy(pstrFileOut, "");
	if (g_uf)
		GetFile(pstrFileOut, nFileNum, g_uf);
	return strlen(pstrFileOut);
}

int APIENTRY ZIPFileExist(char* pstrFileToCheck)
{
	//determine if a file exists in the zipfile...
	if (g_uf)
	{
		bool found = FileExists(pstrFileToCheck, g_uf);
		if (found)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}


//tack a zipfile onto the end of an existing file...
int APIENTRY ZIPCreateCompoundFile(char* pstrOrigFile, char* pstrExtendedFile)
{
	unsigned char buffer[260];
	int nBufSize = 256;

	FILE* orig = fopen(pstrOrigFile, "ab");
	FILE* extend = fopen(pstrExtendedFile, "rb");
	if (orig && extend)
	{
		//get respective sizes...
		fseek(orig, 0, SEEK_END);
		int nOLen = ftell(orig);

		fseek(extend, 0, SEEK_END);
		int nELen = ftell(extend);
		fseek(extend, 0, SEEK_SET);

		//determine how many '256 k' chunks are in extended file...
		int nExtendChunks = nELen / nBufSize;
		int nExtendRemain = nELen - (nExtendChunks * nBufSize);

		//copy chunks over...
		int i=0;
		for (i=0; i < nExtendChunks; i++)
		{
			fread(buffer, nBufSize, sizeof(unsigned char), extend);
			fwrite(buffer, nBufSize, sizeof(unsigned char), orig);
		}

		//copy remainder over...
		for (i=0; i< nExtendRemain; i++)
		{
			unsigned char c;
			fread(&c, 1, sizeof(unsigned char), extend);
			fwrite(&c, 1, sizeof(unsigned char), orig);
		}

		//now write out tail...
		char strIDString[5];
		strcpy(strIDString, "CMP1");
		fwrite(strIDString, 4, 1, orig);
		fwrite(&nOLen, 1, sizeof(int), orig);
		fwrite(&nELen, 1, sizeof(int), orig);

		fclose(orig);
		fclose(extend);
		return 1;
	}
	fclose(orig);
	fclose(extend);
	return 0;
}


//determine if a file is a compound file...
int APIENTRY ZIPIsCompoundFile(char* pstrFileToCheck)
{
	FILE* f = fopen(pstrFileToCheck, "rb");
	if (f)
	{
		//get size...
		fseek(f, 0, SEEK_END);
		int nOLen = ftell(f);

		//now seek to 16 bytes before the end...
		fseek(f, -12, SEEK_END);

		//now write out tail...
		char strIDString[5];
		fread(strIDString, 4, 1, f);
		if (strIDString[0] == 'C' &&
			  strIDString[1] == 'M' &&
				strIDString[2] == 'P' &&
				strIDString[3] == '1')
		{
			int nOLen;
			int nELen;
			fread(&nOLen, 1, sizeof(int), f);
			fread(&nELen, 1, sizeof(int), f);
			fclose(f);
			return 1;
		}
		else
		{
			fclose(f);
			return 0;
		}
	}
	return 0;
}


//tack a zipfile onto the end of an existing file...
int APIENTRY ZIPExtractCompoundFile(char* pstrOrigFile, char* pstrSaveTo)
{
	unsigned char buffer[260];
	int nBufSize = 256;

	FILE* orig = fopen(pstrOrigFile, "rb");
	FILE* extend = fopen(pstrSaveTo, "wb");
	if (orig && extend)
	{
		//now seek to 16 bytes before the end...
		fseek(orig, -12, SEEK_END);

		//now readin tail...
		char strIDString[5];
		fread(strIDString, 4, 1, orig);
		if (strIDString[0] == 'C' &&
			  strIDString[1] == 'M' &&
				strIDString[2] == 'P' &&
				strIDString[3] == '1')
		{
			int nOLen;
			int nELen;
			fread(&nOLen, 1, sizeof(int), orig);
			fread(&nELen, 1, sizeof(int), orig);
			
			//determine how many '256 k' chunks are in extended file...
			int nExtendChunks = nELen / nBufSize;
			int nExtendRemain = nELen - (nExtendChunks * nBufSize);

			fseek(orig, nOLen, SEEK_SET);

			//copy chunks over...
			int i=0;
			for (i=0; i < nExtendChunks; i++)
			{
				fread(buffer, nBufSize, sizeof(unsigned char), orig);
				fwrite(buffer, nBufSize, sizeof(unsigned char), extend);
			}

			//copy remainder over...
			for (i=0; i< nExtendRemain; i++)
			{
				unsigned char c;
				fread(&c, 1, sizeof(unsigned char), orig);
				fwrite(&c, 1, sizeof(unsigned char), extend);
			}


			fclose(orig);
			fclose(extend);
			return 1;
		}
		else
		{
			fclose(orig);
			fclose(extend);
			return 0;
		}
	}
	fclose(orig);
	fclose(extend);
	return 0;
}
