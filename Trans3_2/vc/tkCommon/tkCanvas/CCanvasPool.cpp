/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2007  Christopher Matthews
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

#include "CCanvasPool.h"

/////////////////////
//c-tor
// Create a canvas pool for canvases of sizex, sizey.
// The pool will contain nPoolSize canvases
CCanvasPool::CCanvasPool( int nCompatibleDC, int nSizeX, int nSizeY, int nPoolSize )
{
	m_nCompatibleDC = nCompatibleDC;

	//the pool is actualy one large canvas...
	int nX = nSizeX * POOLS_PER_CANVAS;
	int nY = nSizeY;

	//create canvases in groups of 25...
	if ( nPoolSize % POOLS_PER_CANVAS )
	{
		nPoolSize += ( POOLS_PER_CANVAS - ( nPoolSize % POOLS_PER_CANVAS ) );
	}

	for ( int c = 0; c < ( nPoolSize / POOLS_PER_CANVAS ); c++ )
	{
		CCanvas* pCanvas = new CCanvas();
		pCanvas->CreateBlank( ( HDC ) nCompatibleDC, nX, nY, true );
		m_vCanvases.push_back( pCanvas );
	}

	m_nSizeX = nSizeX;
	m_nSizeY = nSizeY;
	m_nPoolSize = nPoolSize;

	//mark all as unoccupied...
	for ( int i = 0; i < nPoolSize; i++ )
	{
		m_vOccupied.push_back( false );
	}
}


//////////////////////
//D-tor
CCanvasPool::~CCanvasPool()
{
	std::vector<CCanvas*>::iterator itr = m_vCanvases.begin();
	for (; itr != m_vCanvases.end(); itr++)
	{
		delete (*itr);
	}
}



//////////////////////
//Get the index of a free canvas in the pool
int CCanvasPool::getFree()
{
	//search for a free one...
	int i = 0;
	for ( i = 0; i < getPoolSize(); i++ )
	{
		if ( !m_vOccupied[ i ] )
		{
			m_vOccupied[ i ] = true;
			return i;
		}
	}

	//couldn't find any free ones...
	//increase size of cache by 25...
	int nX = getSizeX() * POOLS_PER_CANVAS;
	int nY = getSizeY();

	CCanvas* pCanvas = new CCanvas();
	pCanvas->CreateBlank( ( HDC ) m_nCompatibleDC, nX, nY, true );
	m_vCanvases.push_back( pCanvas );

	this->m_nPoolSize = getPoolSize() + POOLS_PER_CANVAS;

	for ( i = 0; i < POOLS_PER_CANVAS / 2; i++ )
	{
		m_vOccupied.push_back( false );
	}

	for ( i = 0; i < getPoolSize(); i++ )
	{
		if ( !m_vOccupied[ i ] )
		{
			m_vOccupied[ i ] = true;
			return i;
		}
	}
	
	return -1;
}


//////////////////////
//Release a canvas to the pool
void CCanvasPool::release( int nIndex )
{
	m_vOccupied[ nIndex ] = false;
}


//////////////////////
//Set pixel on an index of the pool
void CCanvasPool::SetPixel( int nX, int nY, long crColor, int nIndex )
{
	if ( nIndex > getPoolSize() - 1 || nX > getSizeX() - 1 || nY > getSizeY() - 1 )
	{
		//out of bounds...
		return;
	}

	//in bounds...
	CCanvas* pCanvas = getCanvas( nIndex );
	pCanvas->SetPixel( getX( nX, nIndex ), getY( nY, nIndex ), crColor );
}


//////////////////////
//Blt the whole thing onto another surface
void CCanvasPool::Blt( HDC hDest, int nDestX, int nDestY, int nIndex, long rasterOp )
{
	if ( nIndex > getPoolSize() - 1)
	{
		//out of bounds...
		return;
	}

	CCanvas* pCanvas = getCanvas( nIndex );
	pCanvas->BltPart( hDest, nDestX, nDestY, 
		getX( 0, nIndex ), getY( 0, nIndex ), getSizeX(), getSizeY(), rasterOp );
}


//////////////////////
//Blt the whole thing onto another surface
void CCanvasPool::Blt( CCanvas* pDest, int nDestX, int nDestY, int nIndex, long rasterOp )
{
	if ( nIndex > getPoolSize() - 1)
	{
		//out of bounds...
		return;
	}

	CCanvas* pCanvas = getCanvas( nIndex );
	pCanvas->BltPart( pDest, nDestX, nDestY, 
		getX( 0, nIndex ), getY( 0, nIndex ), getSizeX(), getSizeY(), rasterOp );
}


//////////////////////
//Lock for pixel setting.  Remember to unlock!
void CCanvasPool::Lock( int nIndex )
{
	CCanvas* pCanvas = getCanvas( nIndex );
	pCanvas->Lock();
}

//////////////////////
//Unlock for pixel setting.
void CCanvasPool::Unlock( int nIndex )
{
	CCanvas* pCanvas = getCanvas( nIndex );
	pCanvas->Unlock();
}


//////////////////////
//Set pixels into canvas
void CCanvasPool::SetPixels( long* p_crPixelArray, int x, int y, int width, int height, int nIndex )
{
	if ( nIndex > getPoolSize() - 1)
	{
		//out of bounds...
		return;
	}

	CCanvas* pCanvas = getCanvas( nIndex );
	pCanvas->SetPixels( p_crPixelArray, getX( x, nIndex ), getY( y, nIndex ), width, height );
}


//////////////////////
// Get the canvas for the associated index
CCanvas* CCanvasPool::getCanvas( int nIndex )
{
	if ( nIndex > getPoolSize() - 1)
	{
		//out of bounds...
		return NULL;
	}

	int nCnv = nIndex / POOLS_PER_CANVAS;
	return m_vCanvases[ nCnv ];
}


void CCanvasPool::BltTransparent( HDC hDest, int nDestX, int nDestY, int nIndex, long crTransparent )
{
	if ( nIndex > getPoolSize() - 1)
	{
		//out of bounds...
		return;
	}

	CCanvas* pCanvas = getCanvas( nIndex );
	pCanvas->BltTransparentPart( hDest, nDestX, nDestY, 
		getX( 0, nIndex ), getY( 0, nIndex ), getSizeX(), getSizeY(), crTransparent );
}

void CCanvasPool::BltTransparent( CCanvas* pDest, int nDestX, int nDestY, int nIndex, long crTransparent )
{
	if ( nIndex > getPoolSize() - 1)
	{
		//out of bounds...
		return;
	}

	CCanvas* pCanvas = getCanvas( nIndex );
	pCanvas->BltTransparentPart( pDest, nDestX, nDestY, 
		getX( 0, nIndex ), getY( 0, nIndex ), getSizeX(), getSizeY(), crTransparent );
}
