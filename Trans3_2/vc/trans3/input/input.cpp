/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2007 Colin James Fitzpatrick & Jonathan D. Hughes
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
#include <vector>
#include "input.h"
#include "../common/sprite.h"
#include "../common/paths.h"
#include "../common/board.h"
#include "../common/mainfile.h"
#include "../plugins/plugins.h"
#include "../plugins/constants.h"
#include "../movement/CPlayer/CPlayer.h"
#include "../rpgcode/CProgram.h"

/*
 * Globals.
 */
std::vector<TCHAR> g_keys;
std::vector<TCHAR> g_vkeys;
IDirectInput8A *g_lpdi = NULL;
IDirectInputDevice8A *g_lpdiKeyboard = NULL;
IDirectInputDevice8A *g_lpdiMouse = NULL;
struct tagMOUSE
{
	POINT move;
	POINT click;
} g_mouse;

/*
 * Process an event from the message queue.
 */
void processEvent()
{
	MSG message;
	if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
	{
		// There was a message. Check if it's eventProcessor() asking to leave this loop.
		if (message.message == WM_QUIT)
		{
			// It was; quit.
			extern void closeSystems();
			closeSystems();
			exit(message.wParam);
		}
		else
		{
			// Change ascii keys and the like to virtual keys.
			TranslateMessage(&message);
			// Send the message to the event processor.
			DispatchMessage(&message);
		}
	}
}

/*
 * Transform a char to an STRING, converting
 * common characters to string representations.
 */
STRING getName(const char chr, const char isVirtual, const bool bCapital)
{
    if(isVirtual)
	{
		switch (chr)
		{
			case 37: return _T("LEFT");
			case 39: return _T("RIGHT");
			case 38: return _T("UP");
			case 40: return _T("DOWN");
		}
    }
	
	switch (chr)
	{
		case 13: return _T("ENTER");
		case 27: return _T("ESC");
		case 8: return _T("BACKSPACE");
		case 9: return _T("TAB");
		case 20: return _T("CAPSLOCK");
	}

	const TCHAR toRet[] = 
	{
		bCapital ? toupper(chr) : chr, _T('\0')
	};

	return toRet;
}

/*
* Get the virtual key code associated with a ASCII character.
*/
SHORT getVirtualKey(const char chr)
{
	// Assume that the keyboard layout does not matter, if there are locale
	// issues VkKeyScanEx() should be used in place of this.
	return VkKeyScan(chr);
}

/*
* Get the virtual key code associated with a virtual string.
*/
SHORT getVirtualKey(const STRING str)
{
	SHORT vkCode = -1;

	if (str == _T("LEFT"))
	{
		vkCode = 37;
	}
	else if (str == _T("RIGHT"))
	{
		vkCode = 39;
	}
	else if (str == _T("UP"))
	{
		vkCode = 38;
	}
	else if (str == _T("DOWN"))
	{
		vkCode = 40;
	}
	else if (str == _T("ENTER"))
	{
		vkCode = 13;
	}
	else if (str == _T("ESC"))
	{
		vkCode = 27;
	}
	else if (str == _T("BACKSPACE"))
	{
		vkCode = 8;
	}
	else if (str == _T("TAB"))
	{
		vkCode = 9;
	}
	else if (str == _T("CAPSLOCK"))
	{
		vkCode = 20;
	}

	return vkCode;
}

/*
* Gets the physical state of a key on the keyboard
* is it pressed, depressed, or toggled for keys like
* Caps Lock.
*/
BOOL getKeyState(const SHORT key)
{
	return !!(GetAsyncKeyState(key) & 0x8000);
}

/*
 * Wait for a key.
 *
 * return (out) - the key pressed
 */
STRING waitForKey(const bool bCapital)
{
	g_keys.clear();
	g_vkeys.clear();
	while (g_keys.size() == 0)
	{
		processEvent();
	}
	const char chr = g_keys.front();
	const char isVirtual = g_vkeys.front(); 
	g_keys.erase(g_keys.begin());
	g_vkeys.erase(g_vkeys.begin());

	return getName(chr, isVirtual, bCapital);
}

/*
 * Get last mouse click / wait for mouse.
 */
POINT getMouseClick(const bool bWait)
{
	if (bWait)
	{
		g_mouse.click.x = -1;
		while (g_mouse.click.x == -1)
		{
			processEvent();
		}
	}
	return g_mouse.click;
}

/*
 * Get last mouse move.
 */
POINT getMouseMove(const bool bWait)
{
	const int x = g_mouse.move.x, y = g_mouse.move.y;

	if (bWait)
	{
		while (g_mouse.move.x == x && y == g_mouse.move.y)
		{
			processEvent();
		}
	}
	return g_mouse.move;
}

/*
 * Enumerate keyboards.
 */
BOOL CALLBACK diEnumKeyboardProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	*(LPGUID)pvRef = lpddi->guidProduct;
	if (GET_DIDEVICE_SUBTYPE(lpddi->dwDevType) == DI8DEVTYPEKEYBOARD_PCENH)
	{
		return DIENUM_STOP;
	}
	return DIENUM_CONTINUE;
}

/*
 * Initialize input.
 */
void initInput()
{
	if (g_lpdi) return;
	extern HINSTANCE g_hInstance;
	extern HWND g_hHostWnd;
	DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8A, (void **)&g_lpdi, NULL);
	
	GUID kbGuid = GUID_SysKeyboard;
	g_lpdi->EnumDevices(DI8DEVTYPE_KEYBOARD, diEnumKeyboardProc, &kbGuid, DIEDFL_ATTACHEDONLY);
	g_lpdi->CreateDevice(kbGuid, &g_lpdiKeyboard, NULL);
	g_lpdiKeyboard->SetCooperativeLevel(g_hHostWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	g_lpdiKeyboard->SetDataFormat(&c_dfDIKeyboard);
	g_lpdiKeyboard->Acquire();

	GUID msGuid = GUID_SysMouse;
	// No need to enumerate the mouse.
	g_lpdi->CreateDevice(msGuid, &g_lpdiMouse, NULL);
	g_lpdiMouse->SetCooperativeLevel(g_hHostWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	g_lpdiMouse->SetDataFormat(&c_dfDIMouse);
	g_lpdiMouse->Acquire();
}

/*
 * Close input.
 */
void freeInput()
{
	if (!g_lpdi) return;
	g_lpdiKeyboard->Unacquire();
	g_lpdiKeyboard->Release();
	g_lpdiKeyboard = NULL;

	g_lpdiMouse->Unacquire();
	g_lpdiMouse->Release();
	g_lpdiMouse = NULL;

	g_lpdi->Release();
	g_lpdi = NULL;
}

/*
 * Scan for keys.
 */
void scanKeys()
{
	extern GAME_STATE g_gameState;
	extern CPlayer *g_pSelectedPlayer;
	extern MAIN_FILE g_mainFile;
	extern STRING g_projectPath;
	extern LPBOARD g_pBoard;

	BYTE keys[256];
	if (FAILED(g_lpdiKeyboard->GetDeviceState(256, keys))) return;
	#define SCAN_KEY_DOWN(x) (keys[x] & 0x80)

	// General activation key.
	if (SCAN_KEY_DOWN(g_mainFile.key))
	{
		if (g_pSelectedPlayer->programTest(true)) return;
	}

	// Menu key.
	if (SCAN_KEY_DOWN(g_mainFile.menuKey))
	{
		extern IPlugin *g_pMenuPlugin;
		if (g_pMenuPlugin) g_pMenuPlugin->menu(MNU_MAIN);
		renderNow(NULL, true);

		// Delay to prevent the menu from immediately reopening.
		Sleep(75);
		return;
	}

	// Runtime keys.
	std::vector<short>::const_iterator i = g_mainFile.runTimeKeys.begin();
	for (; i != g_mainFile.runTimeKeys.end(); ++i)
	{
		if (SCAN_KEY_DOWN(*i))
		{
			STRING prg = g_mainFile.runTimePrg[i - g_mainFile.runTimeKeys.begin()];
			prg = g_projectPath + PRG_PATH + prg;
			CProgram cp(prg);
			cp.run();
		}
	}

	MV_ENUM queue = MV_IDLE;

	if (g_mainFile.movementControls & MF_USE_KEYS)
	{
		const bool diag = g_mainFile.movementControls & MF_ALLOW_DIAGONALS;
		
		// movementKeys are ordered as MV_E to MV_NE.
		// mv will cycle from MV_MIN to MV_MAX continually (i.e. multiple sets allowed).
		MV_ENUM mv = MV_E;
		for (i = g_mainFile.movementKeys.begin(); i != g_mainFile.movementKeys.end(); ++i, ++mv)
		{
			if (SCAN_KEY_DOWN(*i))
			{
				if (!diag)
				{
					// If disallowing combinations, exit straight away.
					queue = mv;
					break;
				}
				else
				{
					// Process two keys held down.
					if(queue == MV_IDLE) 
					{
						// First key down.
						queue = mv;
						continue;
					}
					else if (queue == mv - 2) 
					{
						// Second key down at 90 degrees anticlockwise (MV_E to MV_NW).
						queue = ++queue;
					}
					else if (queue == mv - 6) 
					{
						// Second key down at 90 degrees clockwise (MV_N or MV_NE).
						queue = ++mv;
					}
					break;
				}
			} // if (key down)
		} // for (keys)

		if (queue != MV_IDLE)
		{
			// Queue up the movement, and clear any currently queued movements.
			g_pSelectedPlayer->setQueuedMovement(queue, true);
			g_gameState = GS_MOVEMENT;
			return;
		}

	} // if (using keys)

	if (g_mainFile.movementControls & MF_USE_MOUSE)
	{
		// Process mouse-driven movement.
		// Use DI to get the status of the mousebuttons at this point.
		extern RECT g_screen;
		extern HWND g_hHostWnd;

		DIMOUSESTATE dims;
		memset(&dims, 0, sizeof(dims));
		if (FAILED(g_lpdiMouse->GetDeviceState(sizeof(dims), &dims))) return;

		if (dims.rgbButtons[0] & 0x80)
		{
			// Left button.
			// Use the API to get the location to avoid having to deal with
			// DI's relative co-ordinates.
			POINT p = {0, 0};
			if (GetCursorPos(&p))
			{
				ScreenToClient(g_hHostWnd, &p);
				if (p.x > 0 && p.y > 0)
				{
					// No flags - walk up to any sprite that blocks the goal.
					PF_PATH pf = g_pSelectedPlayer->pathFind(p.x + g_screen.left, p.y + g_screen.top, PF_PREVIOUS, 0);
					if (pf.size())
					{
						g_pSelectedPlayer->setQueuedPath(pf, true);
					}
				}
			}
		}
	} // if (using mouse)
}

/*
 * Host window event processor.
 *
 * hwnd (in) - handle of window
 * msg (in) - message being sent
 * wParam + lParam (in) - parameters of the message
 */
LRESULT CALLBACK eventProcessor(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	extern CPlayer *g_pSelectedPlayer;
	extern RECT g_screen;

	// Switch on the message we're to process.
	switch (msg)
	{

		// Window needs painting.
		case WM_PAINT:
		{

			// Begin painting the window
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);

			// Force a render of the screen
			extern CDirectDraw *g_pDirectDraw;
			g_pDirectDraw->Refresh();

			// End of painting of the window
			EndPaint(hwnd, &ps);

		} break;

		//Window was closed.
		case WM_DESTROY:
		{
			PostQuitMessage(EXIT_SUCCESS);
		} break;

		// Key down.
		case WM_KEYDOWN:
		{
			// wParam is the virtual key down.
			const UINT vir = UINT(wParam);

			// I am using the esoteric __int16 type here because it is
			// the fact that the variable is 16 bits that is important,
			// not the fact that it is an integer.
			//
			// Bits 16-24 of lParam are the scan code.
			const unsigned __int16 scan = *(((__int16 *)&lParam) + 1);

			// Get the current state of the keyboard.
			BYTE state[256];
			GetKeyboardState(state);

			// Get an ASCII representation of the key.
			WORD key = 0;
			char isVirtual = 0;
			if ((vir != VK_SHIFT) && !ToAscii(vir, scan, state, &key, 0))
			{
				// If ToAscii() failed, use the virtual code.
				key = vir;
				isVirtual = 1;
			}

			// Queue the character.
			g_keys.push_back(key);
            g_vkeys.push_back(isVirtual);
			// Pass the virtual key to the plugin.
			const STRING strKey = getName(key, isVirtual, true);
			informPluginEvent(vir, -1, -1, -1, state[VK_SHIFT], strKey, INPUT_KB);
		} break;

		// Mouse moved.
		case WM_MOUSEMOVE:
		{
			// Handle the mouse move event.
			g_mouse.move.x = LOWORD(lParam);
			g_mouse.move.y = HIWORD(lParam);

		} break;

		// Left mouse button clicked.
		case WM_LBUTTONDOWN:
		{
			g_mouse.click.x = LOWORD(lParam);
			g_mouse.click.y = HIWORD(lParam);

			informPluginEvent(-1, g_mouse.click.x, g_mouse.click.y, 1, /*shift*/0, _T(""), INPUT_MOUSEDOWN);
		} break;

		// Right mouse button clicked.
		case WM_RBUTTONDOWN:
		{
			const int x = LOWORD(lParam), y = HIWORD(lParam);
			informPluginEvent(-1, x, y, 2, /*shift*/0, _T(""), INPUT_MOUSEDOWN);
		} break;

		// Window activated/deactivated.
		case WM_ACTIVATE:
		{
			extern GAME_STATE g_gameState;
			if (wParam != WA_INACTIVE)
			{
				// Window is being activated,
				if (g_lpdiKeyboard) g_lpdiKeyboard->Acquire();
				if (g_lpdiMouse) g_lpdiMouse->Acquire();
				g_gameState = GS_IDLE;
			}
			else
			{
				// Window is being deactivated.
				g_gameState = GS_PAUSE;
			}
		} break;

		default:
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		} break;

	}

	return 0;
}
