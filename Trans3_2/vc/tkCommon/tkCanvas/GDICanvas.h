/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2007  Christopher Matthews & contributors
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

//--------------------------------------------------------------------------
// A canvas
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Protect the header
//--------------------------------------------------------------------------
#ifndef _CGDICANVAS_H_
#define _CGDICANVAS_H_
#ifdef _MSC_VER
#	pragma once
#endif

//--------------------------------------------------------------------------
// Inclusions
//--------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN			// Flag lean version of Windows
#include <windows.h>				// The Windows API
#include <ddraw.h>					// For DirectDraw
#include "../strings.h"
#include <Gdiplus.h>
using namespace Gdiplus;
//--------------------------------------------------------------------------
// Definitions
//--------------------------------------------------------------------------
#if !defined(INLINE) && !defined(FAST_CALL)
#	if defined(_MSC_VER)
#		define INLINE __inline		// VC++ prefers the __inline keyword
#		define FAST_CALL __fastcall
#	else
#		define INLINE inline
#		define FAST_CALL			// Register (fast) calls are specific to VC++
#	endif
#endif
#if !defined(DOUBLE)
typedef double DOUBLE;
#endif
#if !defined(STATIC)
#define STATIC static
#endif

const long TRANSP_COLOR = 16711935;	// Magic pink

//--------------------------------------------------------------------------
// Definition of the CCanvas class
//--------------------------------------------------------------------------
class CCanvas
{

//
// Public visibility
//
public:

	CCanvas(
		VOID
	);

	CCanvas(
		CONST CCanvas &rhs
	);

	CCanvas(
		LPDIRECTDRAWSURFACE7 surface,
		INT width,
		INT height,
		BOOL bRam
	);

	~CCanvas(
		VOID
	);

	CCanvas &operator=(
		CONST CCanvas &rhs
	);

	HBITMAP GetHBitmap(
		VOID
	) CONST { return m_hBitmap; }

	VOID FAST_CALL CreateBlank(
		CONST HDC hdcCompatible,
		CONST INT width,
		CONST INT height,
		CONST BOOL bDX = FALSE
	);

	VOID Destroy(
		VOID
	);

	VOID SetPixel(
		CONST INT x,
		CONST INT y,
		CONST LONG crColor
	);

	INT FAST_CALL GetPixel(
		CONST INT x,
		CONST INT y
	) CONST;

	INT GetWidth(
		VOID
	) CONST { return m_nWidth; }

	INT GetHeight(
		VOID
	) CONST { return m_nHeight; }

	HDC OpenDC(
		VOID
	) CONST;

	VOID CloseDC(
		HDC hdc
	) CONST;

	VOID Lock(
		VOID
	) { m_hdcLocked = OpenDC(); }

	VOID Unlock(
		VOID
	) {	CONST HDC hdc = m_hdcLocked; m_hdcLocked = NULL; CloseDC(hdc); }

	VOID FAST_CALL SetPixels(
		CONST LPLONG p_crPixelArray,
		CONST INT x,
		CONST INT y,
		CONST INT width,
		CONST INT height
	);

	VOID FAST_CALL Resize(
		CONST HDC hdcCompatible,
		CONST INT width,
		CONST INT height
	);

	VOID SetSrcColorKey(
		CONST LONG crTransparentColor
	)CONST;

	BOOL usingDX(
		VOID
	) CONST { return m_bUseDX; }

	INT FAST_CALL Blt(
		CONST HDC hdcTarget,
		CONST INT x,
		CONST INT y,
		CONST LONG lRasterOp = SRCCOPY
	) CONST;

	INT FAST_CALL Blt(
		CONST CCanvas *pCanvas,
		CONST INT x,
		CONST INT y,
		CONST LONG lRasterOp = SRCCOPY
	) CONST;

	INT FAST_CALL Blt(
		CONST LPDIRECTDRAWSURFACE7 lpddsSurface,
		CONST INT x,
		CONST INT y,
		CONST LONG lRasterOp = SRCCOPY
	) CONST;

	INT FAST_CALL BltPart(
		CONST HDC hdcTarget,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST LONG lRasterOp = SRCCOPY
	) CONST;

	INT FAST_CALL BltPart(
		CONST CCanvas *pCanvas,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST LONG lRasterOp = SRCCOPY
	) CONST;

	INT FAST_CALL BltPart(
		CONST LPDIRECTDRAWSURFACE7 lpddsSurface,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST LONG lRasterOp = SRCCOPY
	) CONST;

	INT FAST_CALL BltTransparent(
		CONST HDC hdcTarget,
		CONST INT x,
		CONST INT y,
		CONST LONG crTransparentColor
	) CONST;

	INT FAST_CALL BltTransparent(
		CONST CCanvas *pCanvas,
		CONST INT x,
		CONST INT y,
		CONST LONG crTransparentColor
	) CONST;

	INT FAST_CALL BltTransparent(
		CONST LPDIRECTDRAWSURFACE7 lpddsSurface,
		CONST INT x,
		CONST INT y,
		CONST LONG crTransparentColor
	) CONST;

	INT FAST_CALL BltTransparentPart(
		CONST HDC hdcTarget,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST LONG crTransparentColor
	) CONST;

	INT FAST_CALL BltTransparentPart(
		CONST CCanvas *pCanvas,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST LONG crTransparentColor
	) CONST;

	INT FAST_CALL BltTransparentPart(
		CONST LPDIRECTDRAWSURFACE7 lpddsSurface,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST LONG crTransparentColor
	) CONST;

	INT FAST_CALL BltTranslucentPart(
		CONST LPDIRECTDRAWSURFACE7 lpddsSurface,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST DOUBLE dIntensity,
		CONST LONG crUnaffectedColor,
		CONST LONG crTransparentColor
	) CONST;

	INT FAST_CALL BltTranslucentPart(
		CONST CCanvas *pCanvas,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST DOUBLE dIntensity,
		CONST LONG crUnaffectedColor,
		CONST LONG crTransparentColor
	) CONST;

	INT FAST_CALL BltTranslucentPart(
		CONST HDC hdcTarget,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST DOUBLE dIntensity,
		CONST LONG crUnaffectedColor,
		CONST LONG crTransparentColor
	) CONST;

	INT FAST_CALL BltTranslucent(
		CONST HDC hdcTarget,
		CONST INT x,
		CONST INT y,
		CONST DOUBLE dIntensity,
		CONST LONG crUnaffectedColor,
		CONST LONG crTransparentColor
	) CONST;

	INT FAST_CALL BltTranslucent(
		CONST CCanvas *pCanvas,
		CONST INT x,
		CONST INT y,
		CONST DOUBLE dIntensity,
		CONST LONG crUnaffectedColor,
		CONST LONG crTransparentColor
	) CONST;

	INT BltTranslucent(
		CONST LPDIRECTDRAWSURFACE7 lpddsSurface,
		CONST INT x,
		CONST INT y,
		CONST DOUBLE dIntensity,
		CONST LONG crUnaffectedColor,
		CONST LONG crTransparentColor
	) CONST;

	INT FAST_CALL BltStretch(
		CONST HDC hdc,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST INT newWidth,
		CONST INT newHeight,
		CONST LONG lRasterOp
	) CONST;

	INT FAST_CALL BltStretch(
		CONST CCanvas *cnv,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST INT newWidth,
		CONST INT newHeight,
		CONST LONG lRasterOp
	) CONST;

	INT FAST_CALL BltStretch(
		CONST LPDIRECTDRAWSURFACE7 lpddsSurface,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST INT newWidth,
		CONST INT newHeight,
		CONST LONG lRasterOp,
		CONST BOOL bInRam
	) CONST;

	BOOL FAST_CALL BltStretchMask(
		CONST CCanvas *cnvMask,
		CONST CCanvas *cnvTarget,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST INT newWidth,
		CONST INT newHeight
	) CONST;

	VOID FAST_CALL ClearScreen(
		CONST LONG crColor
	);

	BOOL FAST_CALL DrawText(
		CONST INT x,
		CONST INT y,
		CONST STRING strText,
		CONST STRING strTypeFace,
		CONST INT size,
		CONST LONG clr,
		CONST BOOL bold = FALSE,
		CONST BOOL italics = FALSE,
		CONST BOOL underline = FALSE,
		CONST BOOL centred = FALSE,
		CONST BOOL outlined = FALSE
	);

	SIZE FAST_CALL GetTextSize(
		CONST STRING strText,
		CONST STRING strTypeFace,
		CONST INT size,
		CONST BOOL bold,
		CONST BOOL italics
	);

	BOOL FAST_CALL DrawLine(
		CONST INT x1,
		CONST INT y1,
		CONST INT x2,
		CONST INT y2,
		CONST LONG clr
	);

	BOOL FAST_CALL DrawRect(
		CONST INT x1,
		CONST INT y1,
		CONST INT x2,
		CONST INT y2,
		CONST LONG clr
	);

	BOOL FAST_CALL DrawFilledRect(
		CONST INT x1,
		CONST INT y1,
		CONST INT x2,
		CONST INT y2,
		CONST LONG clr
	);

	BOOL FAST_CALL DrawEllipse(
		CONST INT x1,
		CONST INT y1,
		CONST INT x2,
		CONST INT y2,
		CONST LONG clr
	);

	BOOL FAST_CALL DrawFilledEllipse(
		CONST INT x1,
		CONST INT y1,
		CONST INT x2,
		CONST INT y2,
		CONST LONG clr
	);

	INT FAST_CALL ShiftLeft(
		CONST INT nPixels
	);

	INT FAST_CALL ShiftRight(
		CONST INT nPixels
	);

	INT FAST_CALL ShiftUp(
		CONST INT nPixels
	);

	INT FAST_CALL ShiftDown(
		CONST INT nPixels
	);

	INT FAST_CALL BltAdditivePart(
		CONST LPDIRECTDRAWSURFACE7 lpddsSurface,
		CONST INT x,
		CONST INT y,
		CONST INT xSrc,
		CONST INT ySrc,
		CONST INT width,
		CONST INT height,
		CONST DOUBLE percent,
		CONST LONG crUnaffectedColor,
		CONST LONG crTransparentColor
	) CONST;

	LPDIRECTDRAWSURFACE7 GetDXSurface(
		VOID
	) CONST { return m_lpddsSurface; }

	LONG GetSurfaceColor(
		CONST LONG dxColor
	) CONST;

	VOID EmulateGamma();

	COLORREF FAST_CALL matchColor(
		CONST COLORREF rgb
	) CONST;

	BOOL FAST_CALL CheckSurfaces(
		VOID
	) CONST;
//
// Private visibility
//
private:

	STATIC COLORREF ConvertDDColor(
		CONST DWORD dwColor,
		CONST LPDDPIXELFORMAT pddpf
	);

	STATIC DWORD ConvertColorRef(
		CONST COLORREF crColor,
		CONST LPDDPIXELFORMAT pddpf
	);

	STATIC WORD GetNumberOfBits(
		CONST DWORD dwMask
	);

	STATIC WORD GetMaskPos(
		CONST DWORD dwMask
	);

	STATIC VOID SetRGBPixel(
		CONST LPDDSURFACEDESC2 destSurface,
		CONST LPDDPIXELFORMAT pddpf,
		CONST INT x,
		CONST INT y,
		CONST LONG rgb
	);

	STATIC LONG GetRGBPixel(
		CONST LPDDSURFACEDESC2 destSurface,
		CONST LPDDPIXELFORMAT pddpf,
		CONST INT x,
		CONST INT y
	);

	VOID FAST_CALL SetPixelsDX(
		CONST LPLONG p_crPixelArray,
		CONST INT x,
		CONST INT y,
		CONST INT width,
		CONST INT height
	);

	VOID SetPixelsGDI(
		CONST LPLONG p_crPixelArray,
		CONST INT x,
		CONST INT y,
		CONST INT width,
		CONST INT height
	);

	INT m_nWidth;							// Width
	INT m_nHeight;							// Height
	BOOL m_bUseDX;							// Using DirectX?
	HDC m_hdcMem;							// Memory DC
	HDC m_hdcLocked;						// Locked hdc
	LPDIRECTDRAWSURFACE7 m_lpddsSurface;	// DirectDraw surface
	HBITMAP m_hBitmap;						// Bitmap for GDI canvases
	HBITMAP m_hOldBitmap;					// Old bitmap for GDI canvases
	BOOL m_bInRam;							// In RAM?
};

//--------------------------------------------------------------------------
// End of the header
//--------------------------------------------------------------------------
#endif
