/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2007 Christopher Matthews & contributors
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
 */

/*
 * Includes
 */

#include "CBoard.h"
#include "stdafx.h"
#include <set>
#include <string>
#include "../../tkCommon/tkGfx/CTile.h"

#ifndef TKGFX_H
#define TKGFX_H

#if !defined(CNV_HANDLE)
typedef INT CNV_HANDLE;
#endif

/*
 * Structures...
 */

///////////////////////////////////////////////
//Exported functions...
long APIENTRY GFXFunctionPtr(long functionAddr);

int APIENTRY GFXInit(long *pCBArray, int nCallbacks);

int APIENTRY GFXInitScreen( int screenX,
														int screenY );

int APIENTRY GFXAbout		( );

int APIENTRY GFXdrawpixel	( long hdc, 
													long x, 
													long y, 
													long col );

int APIENTRY GFXdrawboard ( CBoard *brd, int hdc,
														int maskhdc,
														int layer,
														int topx, 
														int topy,
														int tilesX,
														int tilesY,
														int nBsizex,
														int nBsizey,
														int nBsizel,
														int ar, 
														int ag, 
														int ab,
														int nIsometric = 0,
														int nIsoEvenOdd = 0 );

int APIENTRY GFXdrawtile		( const char* fname, 
													double x, 
													double y, 
													int rred, 
													int ggreen, 
													int bblue, 
													long hdc,
													int nIsometric = 0,
													int nIsoEvenOdd = 0 );

int APIENTRY GFXdrawtilemask( const char *fname, 
									 double x, 
									 double y, 
									 int rred, 
									 int ggreen, 
									 int bblue, 
									 long hdc,
									 int nDirectBlt = 1,
									 int nIsometric = 0,
									 int nIsoEvenOdd = 0 ) ;

int APIENTRY GFXdrawTstWindow ( char fname[],
														 int hdc,
														 int start,
														 int tilesx, int tilesy,
														 int nIsometric = 0 );


int APIENTRY GFXdrawEnemy	( char fname[],
													int x, 
												  int y,
													int r,
													int g,
													int b, 
												  long hdc );

int APIENTRY GFXBitBltTransparent ( long hdcDest,
																 long xDest,
																 long yDest,
																 long width,
																 long height,
																 long hdcSrc,
																 long xSrc,
																 long ySrc,
																 int transRed,
																 int transGreen,
																 int transBlue );

int APIENTRY GFXBitBltTranslucent ( long hdcDest,
																		long xDest,
																		long yDest,
																		long width,
																		long height,
																		long hdcSrc,
																		long xSrc,
																		long ySrc );

int APIENTRY GFXBitBltAdditive ( long hdcDest,
																 long xDest,
																 long yDest,
																 long width,
																 long height,
																 long hdcSrc,
																 long xSrc,
																 long ySrc,
																 long nPercent );

int APIENTRY GFXClearTileCache();

long APIENTRY GFXGetDOSColor( int nColorIdx );

int APIENTRY GFXDrawTileCNV ( const char* fname, 
									 				 double x, 
													 double y, 
													 int rred, 
													 int ggreen, 
													 int bblue, 
													 CNV_HANDLE cnv,
													 int nIsometric,
													 int nIsoEvenOdd );

int APIENTRY GFXDrawTileMaskCNV ( const char fname[], 
									 double x, 
									 double y, 
									 int rred, 
									 int ggreen, 
									 int bblue, 
									 CNV_HANDLE cnv,
									 int nDirectBlt,
									 int nIsometric,
									 int nIsoEvenOdd );

///////////////////////////////////////////////
//Local functions...
void initboard					( );

void openboard					( char fname[],
													int topx,
													int topy,
													int tilesX,
													int tilesY );

void openWinTile				( char fname[] );

void opentile						( char fname[] );

void increasedetail			( );

void increasecolors			( );

void drawBoardTile ( int x, 
										 int y, 
										 int layer,
										 int ar, 
										 int ag, 
										 int ab,
										 int tilesX,
										 int tilesY, 
										 long hdc,
										 long maskhdc,
										 int nIsometric = 0,
										 int nIsoEvenOdd = 0 );

void copyimg						( long x1, 
													long y1, 
													long x2, 
													long y2, 
													long xdest, 
													long ydest, 
													long hdc );

int checkmatch					( int x, 
													int y, 
													int layer,
													int tilesX,
													int tilesY, 
													int *xmatch, 
													int *ymatch );

void resolveName				( char fname[] );

//void open256pal					( );

long rgb								( int red, 
													int green, 
													int blue );

int red									( long rgb );

int green								( long rgb );

int blue								( long rgb );

void extention					( char fname[], 
													char toReturn[] );

int getTileNum					( char fname[] );

void openFromTileSet		( char fname[], 
													int number );

int tilesetInfo					( char fname[] );

long calcInsertionPoint ( int d, int number );

void tilesetFilename ( char fname[], char toReturn[] );

void openEnemy ( char fname[] );

// Added for 3.0.4 by Delano
void createIsometricMask ();

#endif
/* end of file */
