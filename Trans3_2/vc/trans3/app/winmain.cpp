/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2006  Colin Fitzpatrick & Jonathan Hughes
 *
 * Contributors:
 *				Joshua Michael Daly, LJ Gutierrez
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
* This file contains the entry point for the engine defined as mainEntry().
* Most of the initial setup and central processing is performed here.
*/

/*
 * Inclusions.
 */
#include "../../tkCommon/strings.h"
#include "../rpgcode/CProgram.h"
#include "../rpgcode/CGarbageCollector.h"
#include "../rpgcode/virtualvar.h"
#include "../plugins/plugins.h"
#include "../common/paths.h"
#include "../common/mainfile.h"
#include "../common/item.h"
#include "../common/player.h"
#include "../common/mbox.h"
#include "../common/CAllocationHeap.h"
#include "../common/CAnimation.h"
#include "../common/board.h"
#include "../common/CFile.h"
#include "../render/render.h"
#include "../movement/CPlayer/CPlayer.h"
#include "../movement/CItem/CItem.h"
#include "../movement/movement.h"
#include "../input/input.h"
#include "../misc/misc.h"
#include "../audio/CAudioSegment.h"
#include "../../tkCommon/images/FreeImage.h"
#include "../resource.h"
#include "winmain.h"
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <vector>
#include <sstream>
#include <iostream>
#include "Shlwapi.h"
#include "../../tkCommon/tkDirectX/platform.h"

#define FPS_CAP 120.0					// Maximum number of fps to render.
#define HALF_FPS_CAP 60.0				// Half said value.

/*
 * Globals.
 */
GAME_STATE g_gameState = GS_IDLE;		// The current gamestate.
MAIN_FILE g_mainFile;					// The loaded main file.
CAllocationHeap<BOARD> g_boards;		// All boards.
LPBOARD g_pBoard = NULL;				// The active board.
CAllocationHeap<CAudioSegment> g_music;	// All music.
CAudioSegment *g_bkgMusic = NULL;		// Playing background music.

GAME_TIME g_gameTime;					// Length of game info.
unsigned long g_pxStepsTaken = 0;		// Number of PIXELs the player has moved.
double g_fpms = 0.0;					// Frames per millisecond (renderCount / renderTime).
bool g_loadFromStartPrg = false;		// Was a game loaded from the start prg? (See Load()).

std::vector<CPlayer *> g_players;		// Loaded players.
ZO_VECTOR g_sprites;					// z-ordered players and items.
CPlayer *g_pSelectedPlayer = NULL;		// Pointer to selected player?
int g_selectedPlayer = 0;				// Index of current player.

HINSTANCE g_hInstance = NULL;			// Handle to application.
IPlugin *g_pMenuPlugin = NULL;			// The menu plugin.

/*
 * Locals.
 */
double m_renderCount = 0.0;				// Count of GS_MOVEMENT state loops.
double m_renderTime = 0.0;				// Millisecond cumulative GS_MOVEMENT state loop time.
bool m_testingProgram = false;			// Has trans3 been passed a program to test?

/*
 * Defines.
 */
#define _DEBUG 1
#ifdef _DEBUG

unsigned long g_allocated = 0;
unsigned int counter = 0;

void memlog(void *p, size_t size, const char *mode)
{
	// Save to log the number of bytes written/freed
	FILE *fp = fopen("Trans3MemLog.txt", "a+");
	if (fp)
	{
		fprintf(fp, "\n%d)%x:%s %ld", ++counter, (size_t*)p, mode, size);
	}
	fclose(fp);
}

void *operator new(size_t size)
{
	void *p = malloc(sizeof(size_t) + size);
	*(size_t *)p = size;
	g_allocated += size;

	//memlog(p, size, "write <<");

	return ((size_t *)p + 1);
}

void operator delete(void *p)
{
	if (!p) return;
	p = (size_t *)p - 1;
	g_allocated -= *(size_t *)p;

	//memlog(p, *(size_t *)p, "free >>");

	free(p);
	p = NULL;
}

#endif

void termFunc()
{
	messageBox(	_T("An unhandled exception has occurred. ")
				_T("Trans3 will now close.\n\n")
				_T("We apologize for this inconvenience. ")
				_T("If this problem persists, please post at www.rpgtoolkit.net ")
				_T("on the \"Software\" forum with an explanation of ")
				_T("what you were doing when the bug occurred and instructions ")
				_T("on how to reproduce the bug, and we will attempt to solve ")
				_T("this problem.")	);
	exit(EXIT_FAILURE);
}

/*
 * Register the fonts in the game's font directory.
 */
void registerFonts(const STRING path, bool bRegister)
{
	WIN32_FIND_DATA fd;

	// Fonts need a .ttf extension!
	HANDLE hSearch = FindFirstFile((path + _T("*.ttf")).c_str(), &fd);

	do
	{
		const STRING strFile = fd.cFileName;

		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Register this subdirectory as well.
			if ((strFile != _T(".")) && (strFile != _T("..")))
			{
				registerFonts(path + strFile + _T("\\"), bRegister);
			}
		}
		else
		{
			// Register or unresgister each font.
			(bRegister ? AddFontResource : RemoveFontResource)
				((path + fd.cFileName).c_str());
		}
	} while (FindNextFile(hSearch, &fd));
}
void registerFonts(bool bRegister)
{
	extern STRING g_projectPath;
	registerFonts(g_projectPath + FONT_PATH, bRegister);

	// Broadcast to the system that the fonts were updated.
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
}

/*
 * Set up the game.
 */
void setUpGame()
{
	extern int g_selectedPlayer;
	extern STRING g_projectPath;
	extern RECT g_screen;
	extern SCROLL_CACHE g_scrollCache;

	// Load plugins.
	CProgram::freePlugins();
	std::vector<STRING>::iterator i = g_mainFile.plugins.begin();

	for (; i != g_mainFile.plugins.end(); ++i)
	{
		if (i->empty())
		{
			continue;
		}

		IPlugin *p = loadPlugin(g_projectPath + PLUG_PATH + *i);

		if (!p)
		{
			continue;
		}

		if (p->plugType(PT_RPGCODE))
		{
			CProgram::addPlugin(p);
		}
		else
		{
			delete p;
		}
	}

	// Menu plugin.
	if (!g_mainFile.menuPlugin.empty())
	{
		IPlugin *p = loadPlugin(g_projectPath + PLUG_PATH + g_mainFile.menuPlugin);
		if (p && p->plugType(PT_MENU))
		{
			extern IPlugin *g_pMenuPlugin;
			if (g_pMenuPlugin)
			{
				g_pMenuPlugin->terminate();
				delete g_pMenuPlugin;
			}
			g_pMenuPlugin = p;
		}
		else
		{
			delete p;
		}
	}

	// Fight plugin.
	if (!g_mainFile.fightPlugin.empty())
	{
		IPlugin *p = loadPlugin(g_projectPath + PLUG_PATH + g_mainFile.fightPlugin);
		if (p && p->plugType(PT_FIGHT))
		{
			extern IPlugin *g_pFightPlugin;
			if (g_pFightPlugin)
			{
				g_pFightPlugin->terminate();
				delete g_pFightPlugin;
			}
			g_pFightPlugin = p;
		}
		else
		{
			delete p;
		}
	}

	CSprite::m_bPxMovement = g_mainFile.pixelMovement;
	CSprite::setLoopOffset(g_mainFile.getGameSpeed());
	g_selectedPlayer = 0;

    // Get the last gAvgTime from the registry.
	double avgTime = -1;
    if (!g_mainFile.extendToFullScreen)
	{
		switch (g_mainFile.mainResolution)
		{
			case 0: // 640 * 480
				getSetting(_T("gAvgTime_640_Win"), avgTime);
				break;
			case 2: // 1024 * 768
				getSetting(_T("gAvgTime_1024_Win"), avgTime);
				break;
			default: // Custom -- use 800 * 600
				getSetting(_T("gAvgTime_800_Win"), avgTime);
				break;
		}
	}
	else
	{
		switch (g_mainFile.mainResolution)
		{
			case 0: // 640 * 480
				getSetting(_T("gAvgTime_640_Full"), avgTime);
				break;
			case 2: // 1024 * 768
				getSetting(_T("gAvgTime_1024_Full"), avgTime);
				break;
			default: // Custom -- use 800 * 600
				getSetting(_T("gAvgTime_800_Full"), avgTime);
				break;
		}
	}

	// Do an fps estimate.
	if (avgTime < 0)
	{
		avgTime = 0.1;
	}

	m_renderTime = avgTime * MILLISECONDS;
	m_renderCount = 100;
	m_renderTime *= m_renderCount;

	// Create and load start player.
	for (std::vector<CPlayer *>::const_iterator j = g_players.begin(); j != g_players.end(); ++j)
	{
		delete *j;
	}

	g_players.clear();

	if (!g_mainFile.initChar.empty())
	{
		g_players.push_back(new CPlayer(g_projectPath + TEM_PATH + g_mainFile.initChar, true, true));
		g_pSelectedPlayer = g_players.front();

		// If .initChar is empty a player must be loaded via AddPlayer() in the start program.
	}

	// Initialise virtual variables.
	initVirtualVars();

	// Run startup program.
	if (!g_mainFile.startupPrg.empty())
	{
		CProgram(g_projectPath + PRG_PATH + g_mainFile.startupPrg).run();
	}

	// Cannot proceed without a player.
	if (!g_pSelectedPlayer)
	{
		throw STRING(_T("Error: an initial character must be defined in the main file or loaded in the start program: cannot proceed."));
	}

	if (!g_mainFile.initBoard.empty() && !g_loadFromStartPrg)
	{
		g_pBoard->open(g_projectPath + BRD_PATH + g_mainFile.initBoard);

		// Set player position before rendering in order to align board.
		g_pSelectedPlayer->setPosition(g_pBoard->startX, g_pBoard->startY, g_pBoard->startL, PX_ABSOLUTE);
		g_pSelectedPlayer->alignBoard(g_screen, true);

		g_scrollCache.render(true);

		// z-order the sprites on board loading.
		g_sprites.zOrder();

		if (!g_pBoard->bkgMusic.empty())
		{
			g_bkgMusic->open(g_pBoard->bkgMusic);
			g_bkgMusic->play(true);
		}
		if (!g_pBoard->enterPrg.empty())
		{
			CProgram(g_projectPath + PRG_PATH + g_pBoard->enterPrg).run();
		}
	}
}

/*
 * Save registry keys on shutdown.
 */
void saveSettings(void)
{
	// Average time taken per frame, in seconds.
	const double avgTime = m_renderTime / (m_renderCount * MILLISECONDS);

    if (!m_testingProgram && avgTime > 0)
	{
		if (!g_mainFile.extendToFullScreen)
		{
			switch (g_mainFile.mainResolution)
			{
				case 0: // 640 * 480
					saveSetting(_T("gAvgTime_640_Win"), avgTime);
					break;
				case 2: // 1024 * 768
					saveSetting(_T("gAvgTime_1024_Win"), avgTime);
					break;
				default: // Custom -- use 800 * 600
					saveSetting(_T("gAvgTime_800_Win"), avgTime);
					break;
			}
		}
		else
		{
			switch (g_mainFile.mainResolution)
			{
				case 0: // 640 * 480
					saveSetting(_T("gAvgTime_640_Full"), avgTime);
					break;
				case 2: // 1024 * 768
					saveSetting(_T("gAvgTime_1024_Full"), avgTime);
					break;
				default: // Custom -- use 800 * 600
					saveSetting(_T("gAvgTime_800_Full"), avgTime);
					break;
			}
		}
	}
}

/*
 * Open engine subsystems.
 */
void openSystems()
{
	extern void initRpgCode();
	extern GAME_TIME g_gameTime;
	registerFonts(true);
	initPluginSystem();
	FreeImage_Initialise();
	srand(GetTickCount());
	initGraphics();
	CProgram::initialize();
	initRpgCode();
	CAudioSegment::initLoader();
	g_bkgMusic = g_music.allocate();
	g_pBoard = g_boards.allocate();
	g_gameTime.reset(0);
	setUpGame();
}

/*
 * Reset the game (for RPGCode functions and default battle loss).
 */
void reset()
{
	extern IPlugin *g_pFightPlugin;

	CProgram::freeGlobals();
	CProgram::clearRedirects();
	CProgram::freePlugins();

	if (g_pMenuPlugin)
	{
		g_pMenuPlugin->terminate();
		delete g_pMenuPlugin;
		g_pMenuPlugin = NULL;
	}

	if (g_pFightPlugin)
	{
		g_pFightPlugin->terminate();
		delete g_pFightPlugin;
		g_pFightPlugin = NULL;
	}

	CThread::destroyAll();
	setUpGame();
}

/*
 * Close engine subsystems.
 */
void closeSystems()
{
	// Free plugins first so that they have access to
	// everything we're about to kill.
	CProgram::freePlugins();
	CThread::destroyAll();
	extern IPlugin *g_pMenuPlugin, *g_pFightPlugin;

	if (g_pMenuPlugin)
	{
		g_pMenuPlugin->terminate();
		delete g_pMenuPlugin;
		g_pMenuPlugin = NULL;
	}

	if (g_pFightPlugin)
	{
		g_pFightPlugin->terminate();
		delete g_pFightPlugin;
		g_pFightPlugin = NULL;
	}

	freePluginSystem();

	closeGraphics();
	extern void freeInput();
	freeInput();

	// Destroy sprites (move to somewhere)
	for (std::vector<CPlayer *>::const_iterator i = g_players.begin(); i != g_players.end(); ++i)
	{
		delete (*i);
	}

	g_players.clear();
	g_music.free(g_bkgMusic);
	g_bkgMusic = NULL;
	CAudioSegment::freeLoader();

	FreeImage_DeInitialise();

	// Items - currently freed by the board destructor.
	g_boards.free(g_pBoard);
	g_pBoard = NULL;

	CThreadAnimation::destroyAll();
	CSharedAnimation::freeAll();

	saveSettings();

	uninitialisePakFile();

	// Unregister fonts.
	registerFonts(false);
}

/*
 * Get a main file name.
 */
STRING getMainFileName(const STRING cmdLine)
{
	const STRING mainGam = _T("main.gam");

	// First, check for a standalone game.
	if (isStandaloneGame())
	{
		// We're standalone.
		return mainGam;
	}

	std::vector<STRING> parts;
	split(cmdLine, _T(" "), parts);

	if (parts.size() == 2)
	{
		// Main game file passed on command line.
		const STRING ret = GAM_PATH + parts[1];

		if (CFile::fileExists(ret))
		{
			return ret;
		}
	}
	else if (parts.size() == 3)
	{
		// Run program.
		const STRING main = GAM_PATH + parts[1];

		if (CFile::fileExists(main))
		{
			GdiplusStartupInput gdiplusStartupInput;
			ULONG_PTR gdiplusToken;

			GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

			m_testingProgram = true;
			g_mainFile.open(main);
			g_mainFile.startupPrg = parts[2];
			g_mainFile.initBoard = _T("");

			try
			{
				openSystems();
			}
			catch (STRING str)
			{
				// This will be thrown if no initial character is set
				// but it does not actually matter, so we just ignore
				// the error.
			}

			GdiplusShutdown(gdiplusToken);
			closeSystems();

			return _T("");
		}
	}

	if (CFile::fileExists(GAM_PATH + mainGam))
	{
		return GAM_PATH + mainGam;
	}

	TCHAR strFileName[MAX_PATH] = _T("");

	OPENFILENAME ofn = {
		sizeof(OPENFILENAME),
		NULL,
		g_hInstance,
		_T("Supported Files\0*.gam;*.tpk\0RPG Toolkit Main File (*.gam)\0*.gam\0RPG Toolkit PakFile (*.tpk)\0*.tpk\0All files(*.*)\0*.*"),
		NULL, 
		0, 
		1,
		strFileName, 
		MAX_PATH,
		NULL, 
		0,
		GAM_PATH, 
		_T("Open Main File"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, 
		0, 
		0,
		_T(".gam"),
		0, 
		NULL, 
		NULL
	};

	const STRING fileName = (GetOpenFileName(&ofn) ? strFileName : _T(""));

	if (_ftcsicmp(getExtension(fileName).c_str(), _T("TPK")) == 0)
	{
//		MessageBox(NULL, _T("It is a pakfile, PakFile"),_T("Just Checking"),0);
		initialisePakFile(fileName);
//		MessageBox(NULL, _T("Still here returning maingame"),_T("Just Checking"),0);
		return mainGam;
	}
	return fileName;
}

/*
 * Run a frame of game logic.
 *
 * return (out) - current game state
 */
GAME_STATE gameLogic()
{
	switch (g_gameState)
	{
		case GS_IDLE:
			// Only receive input when the player is idle.
			scanKeys();

		case GS_MOVEMENT:
		{
			// Frames per millisecond.
			g_fpms = (m_renderCount / m_renderTime);
			const unsigned long fps = g_fpms * MILLISECONDS;
			g_mainFile.bFpsInTitleBar = 1;//~TEMP

			if (g_mainFile.bFpsInTitleBar)
			{
				extern HWND g_hHostWnd;
				STRINGSTREAM ss;

#if _DEBUG
				ss <<	g_mainFile.gameTitle.c_str()
					<< _T(" � ") << g_pBoard->vectors.size()
					<< _T(" vectors, ") << (g_fpms * MILLISECONDS)
					<< _T(" FPS");
				ss << _T(", ") << g_allocated << _T(" bytes");
#endif

				SetWindowText(g_hHostWnd, ss.str().c_str());
			}

			// Multitask.
			// I couldn't see any sense in basing the number of thread
			// units executed on HALF_FPS_CAP (which is 60) divided
			// by the current fps.
			// 
			// For example if the current fps was about 100 then we would be
			// executing 0.6 (because this is unsigned it would become 1).
			// Basically as the fps decreased we would end up executing even
			// more units which would simply magnifiy any performance issues.
			//
			// It makes more sense to use the reverse of: fps / (HALF_FPS_CAP / 2).
			// Engine threaded multitasking will need to be reviewed properly,
			// threads are roughly 50 times slower than normal programs...
			//unsigned int units = HALF_FPS_CAP / fps;

			unsigned int units = fps / (HALF_FPS_CAP / 2);
			CThread::multitask((units < 1) ? 10 : ((units > 8) ? 80 : units*10));

			// Movement.
			// Remember the vector size, so we'll know if it has been tampered with from within the move function
			int s = g_sprites.v.size(), i;

			for (i = 0; i < s; ++i)
			{
				g_sprites.v[i]->move(g_pSelectedPlayer, false);
			}

			// Run programs outside of the above loop for the cases
			// when sprites may be removed from the vector.
			if (!g_pSelectedPlayer->doBoardEdges())
			{
				g_pSelectedPlayer->playerDoneMove();
			}

			// Render.
			renderNow();
		} 
			break;
		case GS_PAUSE:
			// Relinquish some CPU time.
			Sleep(100);
		case GS_QUIT:
		default:
			break;
	}

	return g_gameState;
}

/*
 * Main event loop.
 */
int mainEventLoop()
{
	// Calculate how long one frame should take, in milliseconds
	const DWORD dwOneFrame = DWORD(MILLISECONDS / FPS_CAP);

	// Define a structure to hold the messages we recieve
	MSG message;
	bool active = true;

	while (TRUE)
	{
		// Get current time
		DWORD dwTimeNow = GetTickCount();

		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			// There was a message, check if it's eventProcessor() asking
			// to leave this loop
			if (message.message == WM_QUIT)
			{
				// It was; quit
				break;
			}
			else if (message.message == WM_NCACTIVATE)
			{
				active = message.wParam;
			}
			else if (message.message == WM_ACTIVATEAPP)
			{
				active = message.wParam;
			}
			else
			{
				// Change ascii keys and the like to virtual keys
				TranslateMessage(&message);
				// Send the message to the event processor
				DispatchMessage(&message);
			}
		}

		if (active)
			// Run a frame of game logic
			if (gameLogic() != GS_PAUSE)
			{
				// Count this loop if not in Paused state
				while ((GetTickCount() - dwTimeNow) < dwOneFrame)
				{
					// Sleep for any remaining time
				}

				// Update length rendering took
				dwTimeNow = GetTickCount() - dwTimeNow;

				// Add the time for this loop and increment the counter.
				// Add only if this is a "short" loop.
				if (dwTimeNow < 256)
				{
					m_renderTime += dwTimeNow;
					++m_renderCount;

					// Make the fps more responsive.
					if (m_renderTime > 4096)
					{
						m_renderCount = int(m_renderCount / 16);
						m_renderTime = m_renderCount / g_fpms;
					}
				}
			}

	}

	return message.wParam;
}

#include <direct.h>

/*
 * Main entry point.
 */
int mainEntry(const HINSTANCE hInstance, const HINSTANCE /*hPrevInstance*/, const LPTSTR lpCmdLine, const int nCmdShow)
{
	extern STRING g_savePath;

	setResolve(false);

	TCHAR buffer [_MAX_PATH], *path = buffer;
	if (_tgetcwd(buffer, _MAX_PATH) == NULL) return EXIT_SUCCESS;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
   
	set_terminate(termFunc);

	g_hInstance = hInstance;

	_tchdir(path);
	
//	_tfreopen(_T("log.txt"), _T("w"), stderr); // Destination for std::cerr.

	// Make the default save game folder before pak file stuff.
	_tmkdir(g_savePath.c_str());

	const STRING fileName = getMainFileName(lpCmdLine);
	STRING filePath = fileName;
	
	// Change current directory based on the location of the main game file (*.gam)
	if (_ftcsicmp(getExtension(fileName).c_str(), _T("GAM")) == 0)
	{
		PathRemoveFileSpec((char*)filePath.c_str());
		_tchdir(filePath.c_str());
		_tchdir("..\\");
	}
	// For everything else use the patch of this program
	else
	{
		_tchdir(path);
	}

	if (fileName.empty())
	{
		return EXIT_SUCCESS;
	}

	if (!g_mainFile.open(fileName)) 
	{
		return EXIT_SUCCESS;
	}

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

//	MessageBox(NULL, _T("We made it through"),_T("Just Checking"),0);

	try
	{
		openSystems();
		const int toRet = mainEventLoop();
		closeSystems();
		return toRet;
	}
	catch (STRING str)
	{
		messageBox(str);
	}
	catch (...)
	{
		terminate();
	}

	GdiplusShutdown(gdiplusToken);

	return EXIT_SUCCESS;
}

