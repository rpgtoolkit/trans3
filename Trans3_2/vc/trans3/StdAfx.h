//////////////////////////////////////////////////////////////////
// This is ATL generated code.
//
// Do not modify this file directly. If you need to change its
// contents, use ATL's wizards.
//////////////////////////////////////////////////////////////////

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__9E2FB3FE_D55A_45C2_A480_1AD4DC992671__INCLUDED_)
#define AFX_STDAFX_H__9E2FB3FE_D55A_45C2_A480_1AD4DC992671__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WIN32_DCOM
#define _WIN32_FUSION 0x0100 // This causes activation context structs and APIs to be included

#ifndef STRICT
#define STRICT
#endif // STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include "atlbase.h"
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

class CExeModule : public CComModule
{
public:
	LONG Unlock();
	DWORD dwThreadID;
	HANDLE hEventShutdown;
	void MonitorShutdown();
	bool StartMonitor();
	bool bActivity;
};
extern CExeModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9E2FB3FE_D55A_45C2_A480_1AD4DC992671__INCLUDED)
