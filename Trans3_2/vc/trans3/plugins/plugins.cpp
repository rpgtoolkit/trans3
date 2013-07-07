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

#include "../trans3.h"
static ICallbacks *g_pCallbacks = NULL;

#include "plugins.h"
#include "oldCallbacks.h"
#include <oleauto.h>
#include <objbase.h>
#include <atlbase.h>
#include <vector>
#include <set>
#include "../common/mbox.h"
#include "../common/paths.h"

// A plugin that accepts input using the special methods.
interface IPluginInput
{
	virtual bool inputRequested(const int type) = 0;
	virtual bool eventInform(const int keyCode, const int x, const int y, const int button, const int shift, const STRING key, const int type) = 0;
};

// Number of members in a COM-based plugin's interface.
#define MEMBER_COUNT 12

// A COM based plugin.
class CComPlugin : public IPlugin
{
public:
	CComPlugin();
	CComPlugin(ITypeInfo *pTypeInfo);
	~CComPlugin() { unload(); }
	bool load(ITypeInfo *pTypeInfo);
	void unload();

	void initialize();
	void terminate();
	bool query(const STRING function);
	bool execute(const STRING line, int &retValDt, STRING &retValLit, double &retValNum, const short usingReturn);
	int fight(const int enemyCount, const int skillLevel, const STRING background, const bool canRun);
	int fightInform(const int sourcePartyIndex, const int sourceFighterIndex, const int targetPartyIndex, const int targetFighterIndex, const int sourceHpLost, const int sourceSmpLost, const int targetHpLost, const int targetSmpLost, const STRING strMessage, const int attackCode);
	bool getFighterLocation(const int party, const int idx, int &x, int &y);
	int menu(const int request);
	bool plugType(const int request);

private:
	CComPlugin(const CComPlugin &rhs);
	CComPlugin &operator=(const CComPlugin &rhs);
	IDispatch *m_plugin;
	MEMBERID m_members[MEMBER_COUNT];
};

// Typedefs for old plugins.
typedef long (__stdcall *VERSION_PROC)();
typedef int (__stdcall *INIT_PROC)(int *pCbArray, int nCallbacks);
typedef void (__stdcall *BEGIN_PROC)();
typedef int (__stdcall *QUERY_PROC)(char *pstrQuery);
typedef int (__stdcall *EXECUTE_PROC)(char *pstrCommand, int &retValDt, char *retValLit, double &retValNum, const short usingReturn);//(char *pstrCommand);
typedef void (__stdcall *END_PROC)();
typedef int (__stdcall *TYPE_PROC)(int nRequestedFeature);
typedef int (__stdcall *MENU_PROC)(int nRequestedMenu);
typedef int (__stdcall *FIGHT_PROC)(int nEnemyCount, int nSkillLevel, char *pstrBackground, int nCanRun);
typedef int (__stdcall *FIGHT_INFORM_PROC)(int nSourcePartyIndex, int nSourceFighterIndex, int nTargetPartyIndex, int nTargetFighterIndex, int nSourceHp, int nSourceSmp, int nTargetHp, int nTargetSmp, char *pstrMessage, int nCode);
typedef int (__stdcall *INPUT_REQUESTED_PROC)(int nCode);
typedef int (__stdcall *EVENT_INFORM_PROC)(int nKeyCode, int nX, int nY, int nButton, int nShift, char *pstrKey, int nCode);

// An old plugin.
class COldPlugin : public IPlugin, public IPluginInput
{
public:
	COldPlugin();
	COldPlugin(const STRING file);
	~COldPlugin() { unload(); }
	bool load(const STRING file);
	void unload();

	void initialize();
	void terminate();
	bool query(const STRING function);
	bool execute(const STRING line, int &retValDt, STRING &retValLit, double &retValNum, const short usingReturn);
	int fight(const int enemyCount, const int skillLevel, const STRING background, const bool canRun);
	int fightInform(const int sourcePartyIndex, const int sourceFighterIndex, const int targetPartyIndex, const int targetFighterIndex, const int sourceHpLost, const int sourceSmpLost, const int targetHpLost, const int targetSmpLost, const STRING strMessage, const int attackCode);
	bool getFighterLocation(const int party, const int idx, int &x, int &y) { return false; }
	int menu(const int request);
	bool plugType(const int request);
	bool inputRequested(const int type);
	bool eventInform(const int keyCode, const int x, const int y, const int button, const int shift, const STRING key, const int type);

private:
	COldPlugin(const COldPlugin &rhs);
	COldPlugin &operator=(const COldPlugin &rhs);
	HMODULE m_hModule;
	BEGIN_PROC m_plugBegin;
	QUERY_PROC m_plugQuery;
	EXECUTE_PROC m_plugExecute;
	END_PROC m_plugEnd;
	TYPE_PROC m_plugType;
	MENU_PROC m_plugMenu;
	FIGHT_PROC m_plugFight;
	FIGHT_INFORM_PROC m_plugFightInform;
	INPUT_REQUESTED_PROC m_plugInputRequested;
	EVENT_INFORM_PROC m_plugEventInform;
};

static std::vector<int> g_oldCallbacks;
static std::set<IPluginInput *> g_inputPlugins;

// Location of MEMBERIDs in a CComPlugin::m_members array.
// The order is arbitrary.
#define MEMBER_SETCALLBACKS			0
#define MEMBER_INITIALIZE			1
#define MEMBER_TERMINATE			2
#define MEMBER_VERSION				3
#define MEMBER_DESCRIPTION			4
#define MEMBER_TYPE					5
#define MEMBER_QUERY				6
#define MEMBER_EXECUTE				7
#define MEMBER_MENU					8
#define MEMBER_FIGHT				9
#define MEMBER_FIGHTINFORM			10
#define MEMBER_GETFIGHTERLOCATION	11

/*
 * Load a plugin.
 */
IPlugin *loadPlugin(const STRING path)
{
	const STRING file = resolve(path);

	HMODULE mod = LoadLibrary(file.c_str());

	if (!mod)
	{
		messageBox(_T("The file ") + file + _T(" is not a valid dynamically linkable library."));
		return NULL;
	}

	FARPROC pReg = GetProcAddress(mod, _T("DllRegisterServer"));

	if (!pReg)
	{
		FreeLibrary(mod);
		COldPlugin *p = new COldPlugin();
		if (p->load(file))
		{
			p->initialize();
			return p;
		}
		messageBox(_T("The file ") + file + _T(" is not a valid plugin."));
		delete p;
		return NULL;
	}

	FreeLibrary(mod);

	CComPlugin *p = new CComPlugin();
	STRING manifestFile = file + ".x.manifest";

	// Create an "Active Context" to load the DLL into this processes
	// execution address.
	//
	// Normally you would have to register a COM DLL to access it which
	// would require administrative rights, to get around that issue
	// we are using "Registration Free COM".
	ACTCTX actCtx;
	memset((void*)&actCtx, 0, sizeof(ACTCTX));
	actCtx.cbSize = sizeof(ACTCTX);
	actCtx.lpSource = manifestFile.c_str();

	HANDLE hCtx = ::CreateActCtx(&actCtx);

	if (hCtx == INVALID_HANDLE_VALUE)
	{
		messageBox(_T("Failed to load the type library manifest ") + manifestFile + _T("."));
		return NULL;
	}
	else
	{
		ULONG_PTR cookie;

		if (::ActivateActCtx(hCtx, &cookie))
		{
			ITypeLib *pTypeLib = NULL;

			// Because we can't simply cast to 'LPCOLESTR'.
			std::wstring fileWstring = getUnicodeString(file);
			LPCOLESTR fileLibrary = fileWstring.c_str();

			if (FAILED(LoadTypeLib(fileLibrary, &pTypeLib)))
			{
				messageBox(_T("Failed to load the type library of ") + file + _T("."));
				return NULL;
			}

			const int types = pTypeLib->GetTypeInfoCount();

			// Check all the types in the library.
			bool bLoaded = false;
			for (int i = 0; i < types; ++i)
			{
				ITypeInfo *pTypeInfo = NULL;
				pTypeLib->GetTypeInfo(i, &pTypeInfo);

				if (bLoaded = p->load(pTypeInfo)) 
				{
					break;
				}
			}

			// Release the type library.
			pTypeLib->Release();

			if (!bLoaded)
			{
				messageBox(_T("A remotable class was not found in ") + file + _T("."));
				delete p;
				return NULL;
			}

			::DeactivateActCtx(0, cookie);
		}
	}

	p->initialize();

	return p;
}

/*
 * Inform applicable plugins of an event.
 */
void informPluginEvent(const int keyCode, const int x, const int y, const int button, const int shift, const STRING key, const int type)
{
	std::set<IPluginInput *>::iterator i = g_inputPlugins.begin();
	for (; i != g_inputPlugins.end(); ++i)
	{
		if ((*i)->inputRequested(type))
		{
			(*i)->eventInform(keyCode, x, y, button, shift, key, type);
		}
	}
}

/*
 * Initialize the plugin system.
 */
void initPluginSystem()
{
	if (g_pCallbacks) return;
	CoCreateInstance(CLSID_Callbacks, NULL, CLSCTX_INPROC_SERVER, IID_ICallbacks, (void **)&g_pCallbacks);
	// For backward compatibility.
	g_oldCallbacks.push_back(int(CBRpgCode));
	g_oldCallbacks.push_back(int(CBGetString));
	g_oldCallbacks.push_back(int(CBGetNumerical));
	g_oldCallbacks.push_back(int(CBSetString));
	g_oldCallbacks.push_back(int(CBSetNumerical));
	g_oldCallbacks.push_back(int(CBGetScreenDC));
	g_oldCallbacks.push_back(int(CBGetScratch1DC));
	g_oldCallbacks.push_back(int(CBGetScratch2DC));
	g_oldCallbacks.push_back(int(CBGetMwinDC));
	g_oldCallbacks.push_back(int(CBPopupMwin));
	g_oldCallbacks.push_back(int(CBHideMwin));
	g_oldCallbacks.push_back(int(CBLoadEnemy));
	g_oldCallbacks.push_back(int(CBGetEnemyNum));
	g_oldCallbacks.push_back(int(CBGetEnemyString));
	g_oldCallbacks.push_back(int(CBSetEnemyNum));
	g_oldCallbacks.push_back(int(CBSetEnemyString));
	g_oldCallbacks.push_back(int(CBGetPlayerNum));
	g_oldCallbacks.push_back(int(CBGetPlayerString));
	g_oldCallbacks.push_back(int(CBSetPlayerNum));
	g_oldCallbacks.push_back(int(CBSetPlayerString));
	g_oldCallbacks.push_back(int(CBGetGeneralString));
	g_oldCallbacks.push_back(int(CBGetGeneralNum));
	g_oldCallbacks.push_back(int(CBSetGeneralString));
	g_oldCallbacks.push_back(int(CBSetGeneralNum));
	g_oldCallbacks.push_back(int(CBGetCommandName));
	g_oldCallbacks.push_back(int(CBGetBrackets));
	g_oldCallbacks.push_back(int(CBCountBracketElements));
	g_oldCallbacks.push_back(int(CBGetBracketElement));
	g_oldCallbacks.push_back(int(CBGetStringElementValue));
	g_oldCallbacks.push_back(int(CBGetNumElementValue));
	g_oldCallbacks.push_back(int(CBGetElementType));
	g_oldCallbacks.push_back(int(CBDebugMessage));
	g_oldCallbacks.push_back(int(CBGetPathString));
	g_oldCallbacks.push_back(int(CBLoadSpecialMove));
	g_oldCallbacks.push_back(int(CBGetSpecialMoveString));
	g_oldCallbacks.push_back(int(CBGetSpecialMoveNum));
	g_oldCallbacks.push_back(int(CBLoadItem));
	g_oldCallbacks.push_back(int(CBGetItemString));
	g_oldCallbacks.push_back(int(CBGetItemNum));
	g_oldCallbacks.push_back(int(CBGetBoardNum));
	g_oldCallbacks.push_back(int(CBGetBoardString));
	g_oldCallbacks.push_back(int(CBSetBoardNum));
	g_oldCallbacks.push_back(int(CBSetBoardString));
	g_oldCallbacks.push_back(int(CBGetHwnd));
	g_oldCallbacks.push_back(int(CBRefreshScreen));
	g_oldCallbacks.push_back(int(CBCreateCanvas));
	g_oldCallbacks.push_back(int(CBDestroyCanvas));
	g_oldCallbacks.push_back(int(CBDrawCanvas));
	g_oldCallbacks.push_back(int(CBDrawCanvasPartial));
	g_oldCallbacks.push_back(int(CBDrawCanvasTransparent));
	g_oldCallbacks.push_back(int(CBDrawCanvasTransparentPartial));
	g_oldCallbacks.push_back(int(CBDrawCanvasTranslucent));
	g_oldCallbacks.push_back(int(CBCanvasLoadImage));
	g_oldCallbacks.push_back(int(CBCanvasLoadSizedImage));
	g_oldCallbacks.push_back(int(CBCanvasFill));
	g_oldCallbacks.push_back(int(CBCanvasResize));
	g_oldCallbacks.push_back(int(CBCanvas2CanvasBlt));
	g_oldCallbacks.push_back(int(CBCanvas2CanvasBltPartial));
	g_oldCallbacks.push_back(int(CBCanvas2CanvasBltTransparent));
	g_oldCallbacks.push_back(int(CBCanvas2CanvasBltTransparentPartial));
	g_oldCallbacks.push_back(int(CBCanvas2CanvasBltTranslucent));
	g_oldCallbacks.push_back(int(CBCanvasGetScreen));
	g_oldCallbacks.push_back(int(CBLoadString));
	g_oldCallbacks.push_back(int(CBCanvasDrawText));
	g_oldCallbacks.push_back(int(CBCanvasPopup));
	g_oldCallbacks.push_back(int(CBCanvasWidth));
	g_oldCallbacks.push_back(int(CBCanvasHeight));
	g_oldCallbacks.push_back(int(CBCanvasDrawLine));
	g_oldCallbacks.push_back(int(CBCanvasDrawRect));
	g_oldCallbacks.push_back(int(CBCanvasFillRect));
	g_oldCallbacks.push_back(int(CBCanvasDrawHand));
	g_oldCallbacks.push_back(int(CBDrawHand));
	g_oldCallbacks.push_back(int(CBCheckKey));
	g_oldCallbacks.push_back(int(CBPlaySound));
	g_oldCallbacks.push_back(int(CBMessageWindow));
	g_oldCallbacks.push_back(int(CBFileDialog));
	g_oldCallbacks.push_back(int(CBDetermineSpecialMoves));
	g_oldCallbacks.push_back(int(CBGetSpecialMoveListEntry));
	g_oldCallbacks.push_back(int(CBRunProgram));
	g_oldCallbacks.push_back(int(CBSetTarget));
	g_oldCallbacks.push_back(int(CBSetSource));
	g_oldCallbacks.push_back(int(CBGetPlayerHP));
	g_oldCallbacks.push_back(int(CBGetPlayerMaxHP));
	g_oldCallbacks.push_back(int(CBGetPlayerSMP));
	g_oldCallbacks.push_back(int(CBGetPlayerMaxSMP));
	g_oldCallbacks.push_back(int(CBGetPlayerFP));
	g_oldCallbacks.push_back(int(CBGetPlayerDP));
	g_oldCallbacks.push_back(int(CBGetPlayerName));
	g_oldCallbacks.push_back(int(CBAddPlayerHP));
	g_oldCallbacks.push_back(int(CBAddPlayerSMP));
	g_oldCallbacks.push_back(int(CBSetPlayerHP));
	g_oldCallbacks.push_back(int(CBSetPlayerSMP));
	g_oldCallbacks.push_back(int(CBSetPlayerFP));
	g_oldCallbacks.push_back(int(CBSetPlayerDP));
	g_oldCallbacks.push_back(int(CBGetEnemyHP));
	g_oldCallbacks.push_back(int(CBGetEnemyMaxHP));
	g_oldCallbacks.push_back(int(CBGetEnemySMP));
	g_oldCallbacks.push_back(int(CBGetEnemyMaxSMP));
	g_oldCallbacks.push_back(int(CBGetEnemyFP));
	g_oldCallbacks.push_back(int(CBGetEnemyDP));
	g_oldCallbacks.push_back(int(CBAddEnemyHP));
	g_oldCallbacks.push_back(int(CBAddEnemySMP));
	g_oldCallbacks.push_back(int(CBSetEnemyHP));
	g_oldCallbacks.push_back(int(CBSetEnemySMP));
	g_oldCallbacks.push_back(int(CBCanvasDrawBackground));
	g_oldCallbacks.push_back(int(CBCreateAnimation));
	g_oldCallbacks.push_back(int(CBDestroyAnimation));
	g_oldCallbacks.push_back(int(CBCanvasDrawAnimation));
	g_oldCallbacks.push_back(int(CBCanvasDrawAnimationFrame));
	g_oldCallbacks.push_back(int(CBAnimationCurrentFrame));
	g_oldCallbacks.push_back(int(CBAnimationMaxFrames));
	g_oldCallbacks.push_back(int(CBAnimationSizeX));
	g_oldCallbacks.push_back(int(CBAnimationSizeY));
	g_oldCallbacks.push_back(int(CBAnimationFrameImage));
	g_oldCallbacks.push_back(int(CBGetPartySize));
	g_oldCallbacks.push_back(int(CBGetFighterHP));
	g_oldCallbacks.push_back(int(CBGetFighterMaxHP));
	g_oldCallbacks.push_back(int(CBGetFighterSMP));
	g_oldCallbacks.push_back(int(CBGetFighterMaxSMP));
	g_oldCallbacks.push_back(int(CBGetFighterFP));
	g_oldCallbacks.push_back(int(CBGetFighterDP));
	g_oldCallbacks.push_back(int(CBGetFighterName));
	g_oldCallbacks.push_back(int(CBGetFighterAnimation));
	g_oldCallbacks.push_back(int(CBGetFighterChargePercent));
	g_oldCallbacks.push_back(int(CBFightTick));
	g_oldCallbacks.push_back(int(CBDrawTextAbsolute));
	g_oldCallbacks.push_back(int(CBReleaseFighterCharge));
	g_oldCallbacks.push_back(int(CBFightDoAttack));
	g_oldCallbacks.push_back(int(CBFightUseItem));
	g_oldCallbacks.push_back(int(CBFightUseSpecialMove));
	g_oldCallbacks.push_back(int(CBDoEvents));
	g_oldCallbacks.push_back(int(CBFighterAddStatusEffect));
	g_oldCallbacks.push_back(int(CBFighterRemoveStatusEffect));
}

/*
 * Shut down the plugin system.
 */
void freePluginSystem()
{
	if (!g_pCallbacks) return;
	g_pCallbacks->Release();
	g_pCallbacks = NULL;
	g_oldCallbacks.clear();
}

/*
 * Default constructor.
 */
CComPlugin::CComPlugin()
{
	m_plugin = NULL;
}

COldPlugin::COldPlugin()
{
	m_hModule = NULL;
}

/*
 * Work constructor.
 *
 * cls (in) - class ID to construct
 */
CComPlugin::CComPlugin(ITypeInfo *pTypeInfo)
{
	m_plugin = NULL;
	load(pTypeInfo);
}

COldPlugin::COldPlugin(const STRING file)
{
	m_hModule = NULL;
	load(file);
}

/*
 * Load a plugin.
 *
 * cls (in) - class ID to construct
 */
bool CComPlugin::load(ITypeInfo *pTypeInfo)
{
	if (m_plugin || !g_pCallbacks) 
	{
		return false;
	}

	//messageBox(_T("Passed first hurdle not m_plugin and g_pCallbacks works"));

	// Create an instance of the plugin's class.
	// Failing on Windows 7 at this point.
	HRESULT hr = pTypeInfo->CreateInstance(NULL, IID_IDispatch, (LPVOID *)&m_plugin);

	if (FAILED(hr)) 
	{
		return false;
	}

	/*messageBox(_T("HRESULT from creating an instance worked."));*/

	// I regret this arbitrary naming scheme, but it's too
	// late to change it now.
	LPOLESTR names[] = {
		L"SetCallbacks",
		L"Plugin_Initialize",
		L"Plugin_Terminate",
		L"Plugin_Version",
		L"Plugin_Description",
		L"PlugType",
		L"RPGCode_Query",
		L"RPGCode_Execute",
		L"Menu",
		L"Fight",
		L"FightInform",
		L"GetFighterLocation"
	};

	// Get the MEMBERIDs of members of the class.
	for (unsigned int i = 0; i < MEMBER_COUNT; ++i)
	{
		pTypeInfo->GetIDsOfNames(names + i, 1, m_members + i);
	}

	// Get the DISPIP of the setCallbacks() member.
	const DISPID disp = m_members[MEMBER_SETCALLBACKS];

	// Some methods might validly be missing, but setCallbacks()
	// should be here for all valid plugins.
	if (disp == DISPID_UNKNOWN)
	{
		unload();
		return false;
	}

	/*messageBox(_T("DISPID is known you have passed another hurdle"));*/

	// Provide the plugin with a pointer to an instance of
	// CCallbacks, via the setCallbacks() function.
	DISPPARAMS params = {NULL, NULL, 1, 1};
	VARIANT var;
	var.vt = VT_DISPATCH;
	var.pdispVal = g_pCallbacks;
	params.rgvarg = &var;
	DISPID put = DISPID_PROPERTYPUT;
	params.rgdispidNamedArgs = &put;

	if (FAILED(m_plugin->Invoke(disp, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUTREF, &params, NULL, NULL, NULL)))
	{
		/*messageBox(_T("Property set failed inspection possible worry here"));*/
		// No property set; try for property let.
		if (FAILED(m_plugin->Invoke(disp, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &params, NULL, NULL, NULL)))
		{
			unload();
			return false;
		}
	}

	/*messageBox(_T("At least a property set or property let passed inspection"));
	messageBox(_T("Honestly if you're here then something else has gone wrong somewhere. Trying more opotions"));*/

	return true;
}

bool COldPlugin::load(const STRING file)
{
	if (m_hModule || !g_pCallbacks) return false;
	m_hModule = LoadLibrary(file.c_str());
	if (!m_hModule) return false;
	INIT_PROC pInit = INIT_PROC(GetProcAddress(m_hModule, _T("TKPlugInit")));
	VERSION_PROC pVersion = VERSION_PROC(GetProcAddress(m_hModule, _T("TKPlugVersion")));
	if (pInit)
	{
		pInit(&g_oldCallbacks[0], (!pVersion) ? 45 : g_oldCallbacks.size());
	}
	else
	{
		FreeLibrary(m_hModule);
		return false;
	}
	m_plugBegin = BEGIN_PROC(GetProcAddress(m_hModule, _T("TKPlugBegin")));
	m_plugQuery = QUERY_PROC(GetProcAddress(m_hModule, _T("TKPlugQuery")));
	m_plugExecute = EXECUTE_PROC(GetProcAddress(m_hModule, _T("TKPlugExecute")));
	m_plugEnd = END_PROC(GetProcAddress(m_hModule, _T("TKPlugEnd")));
	m_plugType = TYPE_PROC(GetProcAddress(m_hModule, _T("TKPlugType")));
	m_plugMenu = MENU_PROC(GetProcAddress(m_hModule, _T("TKPlugMenu")));
	m_plugFight = FIGHT_PROC(GetProcAddress(m_hModule, _T("TKPlugFight")));
	m_plugFightInform = FIGHT_INFORM_PROC(GetProcAddress(m_hModule, _T("TKPlugFightInform")));
	m_plugInputRequested = INPUT_REQUESTED_PROC(GetProcAddress(m_hModule, _T("TKPlugInputRequested")));
	m_plugEventInform = EVENT_INFORM_PROC(GetProcAddress(m_hModule, _T("TKPlugEventInform")));
	if (m_plugInputRequested)
	{
		// This plugin accepts 'special' input.
		g_inputPlugins.insert(this);
	}
	return true;
}

/*
 * Initialize the plugin.
 */
void CComPlugin::initialize()
{
	if (!m_plugin) return;

	const MEMBERID member = m_members[MEMBER_INITIALIZE];
	if (member == DISPID_UNKNOWN) return;

	DISPPARAMS params = {NULL, NULL, 0, 0};

	m_plugin->Invoke(
		member,
		IID_NULL,
		LOCALE_USER_DEFAULT,
		DISPATCH_METHOD,
		&params,
		NULL,
		NULL,
		NULL
	);
}

void COldPlugin::initialize()
{
	if (!m_hModule) return;
	m_plugBegin();
}

/*
 * Terminate the plugin.
 */
void CComPlugin::terminate()
{
	if (!m_plugin) return;

	const MEMBERID member = m_members[MEMBER_TERMINATE];
	if (member == DISPID_UNKNOWN) return;

	DISPPARAMS params = {NULL, NULL, 0, 0};

	m_plugin->Invoke(
		member,
		IID_NULL,
		LOCALE_USER_DEFAULT,
		DISPATCH_METHOD,
		&params,
		NULL,
		NULL,
		NULL
	);
}

void COldPlugin::terminate()
{
	if (!m_hModule) return;
	m_plugEnd();
}

/*
 * Query to check whether we support a function.
 */
bool CComPlugin::query(const STRING function)
{
	if (!m_plugin) return false;

	const MEMBERID member = m_members[MEMBER_QUERY];
	if (member == DISPID_UNKNOWN) return false;

	CComVariant vars[] = {function.c_str()}, ret;
	DISPPARAMS params = {vars, NULL, 1, 0};

	m_plugin->Invoke(
		member,
		IID_NULL,
		LOCALE_USER_DEFAULT,
		DISPATCH_METHOD,
		&params,
		&ret,
		NULL,
		NULL
	);

	return (ret.boolVal == VARIANT_TRUE);
}

bool COldPlugin::query(const STRING function)
{
	if (!m_hModule) return false;
#pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
	return m_plugQuery((char *)getAsciiString(function).c_str());
#pragma warning (default : 4800) // forcing value to bool 'true' or 'false' (performance warning)
}

/*
 * Execute an RPGCode function.
 */
bool CComPlugin::execute(const STRING line, int &retValDt, STRING &retValLit, double &retValNum, const short usingReturn)
{
	if (!m_plugin) return false;

	const MEMBERID member = m_members[MEMBER_EXECUTE];
	if (member == DISPID_UNKNOWN) return false;

	// Arguments in *reverse* order.
	CComVariant vars[5], ret;
	vars[4] = line.c_str();
	vars[3].vt = VT_I4 | VT_BYREF;
	vars[3].plVal = (long *)&retValDt;
	vars[2].vt = VT_BSTR | VT_BYREF;
	BSTR bstr = SysAllocString(L"");
	vars[2].pbstrVal = &bstr;
	vars[1].vt = VT_R8 | VT_BYREF;
	vars[1].pdblVal = &retValNum;
	vars[0] = bool(usingReturn);

	DISPPARAMS params = {vars, NULL, 5, 0};

	HRESULT hr = m_plugin->Invoke(
		member,
		IID_NULL,
		LOCALE_USER_DEFAULT,
		DISPATCH_METHOD,
		&params,
		&ret,
		NULL,
		NULL
	);

	retValLit = getString(bstr);
	SysFreeString(bstr);

	return (ret.boolVal == VARIANT_TRUE);
}

bool COldPlugin::execute(const STRING line, int &retValDt, STRING &retValLit, double &retValNum, const short usingReturn)
{
	if (!m_hModule) return false;
	retValDt = 0;
	retValNum = 0.0;
	retValLit = "";
	char retLit[255];
	memset(retLit, 0, sizeof(retLit));
#pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
	bool ret = m_plugExecute((char*)getAsciiString(line).c_str(), retValDt, retLit, retValNum, usingReturn);//((char *)getAsciiString(line).c_str());
	retValLit = std::string(retLit);
	return ret;
#pragma warning (default : 4800) // forcing value to bool 'true' or 'false' (performance warning)
}

/*
 * Start a fight.
 */
int CComPlugin::fight(const int enemyCount, const int skillLevel, const STRING background, const bool canRun)
{
	if (!m_plugin) return 0;

	const MEMBERID member = m_members[MEMBER_FIGHT];
	if (member == DISPID_UNKNOWN) return 0;

	// Arguments in *reverse* order.
	CComVariant vars[] = {int(canRun), background.c_str(), skillLevel, enemyCount}, ret;
	DISPPARAMS params = {vars, NULL, 4, 0};

	HRESULT hr = m_plugin->Invoke(
		member,
		IID_NULL,
		LOCALE_USER_DEFAULT,
		DISPATCH_METHOD,
		&params,
		&ret,
		NULL,
		NULL
	);

	return ret.intVal;
}

int COldPlugin::fight(const int enemyCount, const int skillLevel, const STRING background, const bool canRun)
{
	if (!m_hModule) return 0;
	return m_plugFight(enemyCount, skillLevel, (char *)background.c_str(), canRun);
}

/*
 * Send a fight message.
 */
int CComPlugin::fightInform(const int sourcePartyIndex, const int sourceFighterIndex, const int targetPartyIndex, const int targetFighterIndex, const int sourceHpLost, const int sourceSmpLost, const int targetHpLost, const int targetSmpLost, const STRING strMessage, const int attackCode)
{
	if (!m_plugin) return 0;

	const MEMBERID member = m_members[MEMBER_FIGHTINFORM];
	if (member == DISPID_UNKNOWN) return 0;

	// Arguments in *reverse* order.
	CComVariant vars[] = {attackCode, strMessage.c_str(), targetSmpLost, targetHpLost, sourceSmpLost, sourceHpLost, targetFighterIndex, targetPartyIndex, sourceFighterIndex, sourcePartyIndex}, ret;
	DISPPARAMS params = {vars, NULL, 10, 0};

	HRESULT hr = m_plugin->Invoke(
		member,
		IID_NULL,
		LOCALE_USER_DEFAULT,
		DISPATCH_METHOD,
		&params,
		&ret,
		NULL,
		NULL
	);

	return ret.intVal;
}

int COldPlugin::fightInform(const int sourcePartyIndex, const int sourceFighterIndex, const int targetPartyIndex, const int targetFighterIndex, const int sourceHpLost, const int sourceSmpLost, const int targetHpLost, const int targetSmpLost, const STRING strMessage, const int attackCode)
{
	if (!m_hModule) return 0;
	return m_plugFightInform(sourcePartyIndex, sourceFighterIndex, targetPartyIndex, targetFighterIndex, sourceHpLost, sourceSmpLost, targetHpLost, targetSmpLost, (char *)strMessage.c_str(), attackCode);
}

/*
 * Get the location of a fighter.
 */
bool CComPlugin::getFighterLocation(const int party, const int idx, int &x, int &y)
{
	if (!m_plugin) return false;

	const MEMBERID member = m_members[MEMBER_GETFIGHTERLOCATION];
	if (member == DISPID_UNKNOWN) return false;

	// Arguments in *reverse* order.
	CComVariant vars[4], ret;
	vars[3] = party;
	vars[2] = idx;

	vars[1].vt = VT_I4 | VT_BYREF;
	vars[1].plVal = (long *)&x;

	vars[0].vt = VT_I4 | VT_BYREF;
	vars[0].plVal = (long *)&y;

	DISPPARAMS params = {vars, NULL, 4, 0};

	HRESULT hr = m_plugin->Invoke(
		member,
		IID_NULL,
		LOCALE_USER_DEFAULT,
		DISPATCH_METHOD,
		&params,
		&ret,
		NULL,
		NULL
	);

	return (ret.boolVal == VARIANT_TRUE);
}

/*
 * Open a menu.
 */
int CComPlugin::menu(const int request)
{
	if (!m_plugin) return 0;

	const MEMBERID member = m_members[MEMBER_MENU];
	if (member == DISPID_UNKNOWN) return 0;

	CComVariant vars[] = {request}, ret;
	DISPPARAMS params = {vars, NULL, 1, 0};

	HRESULT hr = m_plugin->Invoke(
		member,
		IID_NULL,
		LOCALE_USER_DEFAULT,
		DISPATCH_METHOD,
		&params,
		&ret,
		NULL,
		NULL
	);

	return ret.intVal;
}

int COldPlugin::menu(const int request)
{
	if (!m_hModule) return 0;
	return m_plugMenu(request);
}

/*
 * Check whether we are a certain type of plugin.
 */
bool CComPlugin::plugType(const int request)
{
	if (!m_plugin) return false;

	const MEMBERID member = m_members[MEMBER_TYPE];
	if (member == DISPID_UNKNOWN) return false;

	CComVariant vars[] = {request}, ret;
	DISPPARAMS params = {vars, NULL, 1, 0};

	HRESULT hr = m_plugin->Invoke(
		member,
		IID_NULL,
		LOCALE_USER_DEFAULT,
		DISPATCH_METHOD,
		&params,
		&ret,
		NULL,
		NULL
	);

	return (ret.boolVal == VARIANT_TRUE);
}

bool COldPlugin::plugType(const int request)
{
	if (!m_hModule) return false;
#pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
	return (m_plugType ? m_plugType(request) : (request == PT_RPGCODE));
#pragma warning (default : 4800) // forcing value to bool 'true' or 'false' (performance warning)
}

/*
 * Determine whether this plugin requested input.
 */
bool COldPlugin::inputRequested(const int type)
{
	if (m_plugInputRequested)
	{
#pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
		return m_plugInputRequested(type);
#pragma warning (default : 4800) // forcing value to bool 'true' or 'false' (performance warning)
	}
	return false;
}

/*
 * Inform this plugin that an event occurred.
 */
bool COldPlugin::eventInform(const int keyCode, const int x, const int y, const int button, const int shift, const STRING key, const int type)
{
	if (m_plugEventInform)
	{
#pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
		return m_plugEventInform(keyCode, x, y, button, shift, (char *)getAsciiString(key).c_str(), type);
#pragma warning (default : 4800) // forcing value to bool 'true' or 'false' (performance warning)
	}
	return true;
}

/*
 * Unload a plugin.
 */
void CComPlugin::unload()
{
	if (m_plugin)
	{
		m_plugin->Release();
		m_plugin = NULL;
	}
}

void COldPlugin::unload()
{
	if (m_hModule)
	{
		if (m_plugInputRequested)
		{
			g_inputPlugins.erase(this);
		}
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}
