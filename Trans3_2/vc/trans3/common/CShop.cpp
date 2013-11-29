/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2006  Jonathan D. Hughes
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

#include "CShop.h"
#include "CInventory.h"
#include "mbox.h"
#include "../input/input.h"
#include "../rpgcode/CCursorMap.h"
#include "../../tkCommon/tkDirectX/platform.h"

#define PAD		12
#define EDGE	24

/*
 * Constructor.
 */
CShop::CShop(CInventory *shopInv, CInventory *playerInv, ULONG *money, STRING image):
	m_shopInv(shopInv), 
	m_playerInv(playerInv),
	m_money(money),
	m_fontSize(24),
	m_image(image)
{ 
	extern STRING g_fontFace;
	extern STRING g_projectPath;
	m_fontFace = g_fontFace;

	// Open each item and store ITEM data.
	for (CInventory *inv = m_shopInv; ; inv = m_playerInv)
	{
		for (INT i = 0; i != inv->size(); ++i)
		{
			CONST STRING file = m_shopInv->fileAt(i);
			if (!file.empty() && m_items.find(file) == m_items.end())
			{
				m_items[file] = new ITEM();
				m_items[file]->open(g_projectPath + ITM_PATH + file, NULL);
			}
		}
		if (inv == m_playerInv) break;
	}

	run(); 
}

/*
 * Destructor.
 */
CShop::~CShop()
{
	// Delete opened ITEMs.
	for (SHOP_ITEMS::iterator i = m_items.begin(); i != m_items.end(); ++i)
	{
		delete i->second;
	}
	m_items.clear();
}

/*
 * Run the shop.
 */
VOID CShop::run()
{
	extern int g_resX, g_resY;
	extern CDirectDraw *g_pDirectDraw;
	extern STRING g_projectPath;

	// Get the current scene to restore later.
	CCanvas restore;
	restore.CreateBlank(NULL, g_resX, g_resY, TRUE);
	g_pDirectDraw->CopyScreenToCanvas(&restore);

	CCanvas backup;
	backup.CreateBlank(NULL, g_resX, g_resY, TRUE);
	
	if (m_image.empty()) 
	{
		// Draw a background colour block translucently.
		restore.Blt(&backup, 0, 0, SRCCOPY);

		CCanvas cnv;
		cnv.CreateBlank(NULL, g_resX, g_resY, TRUE);
		cnv.ClearScreen(m_colors.main);
		cnv.BltTranslucent(&backup, 0, 0, 0.5, -1, -1);
	}
	else
	{
		drawImage(g_projectPath + BMP_PATH + m_image, &backup, 0, 0, -1, -1);
		// Hide the bounding boxes.
		m_colors.line = m_colors.main;
	}

	// Launch menu.
	while (true)
	{
		RECT items = {0};

		CCursorMap map = drawMainMenu(backup, items);
		CONST SHOP_OPTION res = SHOP_OPTION(map.run());
			
		if (res == SHOP_EXIT) break;

		// Stay on the subscreen whilst transactions occur.
		// Refresh the main menu each time.
		do
		{
			drawMainMenu(backup, items);
		} 
		while (itemMenu(backup, res, items));
	}

    // Restore pre-shop scene.
	g_pDirectDraw->DrawCanvas(&restore, 0, 0);
	g_pDirectDraw->Refresh();
}

/*
 * Draw the main menu, title and gp and return a cursor map ready to run.
 */
CCursorMap CShop::drawMainMenu(CONST CCanvas &backup, RECT &items)
{
	extern CDirectDraw *g_pDirectDraw;
	extern int g_resX, g_resY;

	CCanvas cnv;
	cnv.CreateBlank(NULL, g_resX - EDGE * 2, g_resY - EDGE * 2, TRUE);
	cnv.ClearScreen(m_colors.main);

	// Create a nice font.
	CONST HFONT hFont = CreateFont(
		m_fontSize,
		0, 0, 0,
		FW_NORMAL,
		0, 0, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH,
		m_fontFace.c_str()
	);

	// Select out old font.
	HDC hdc = cnv.OpenDC();
	SetBkMode(hdc, TRANSPARENT);
	CONST HGDIOBJ hOld = SelectObject(hdc, hFont);
	SetTextColor(hdc, m_colors.text);

	// Get the line height.
	SIZE sz;
	CONST STRING title = _T("Shop");
	GetTextExtentPoint32(hdc, title.c_str(), title.length(), &sz);

	CONST INT lineHeight = sz.cy + PAD;

	// Draw the title.
	RECT r = {0, 0, cnv.GetWidth(), lineHeight};
	DrawText(hdc, title.c_str(), title.length(), &r, DT_CENTER | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);

	// Draw the player's gp.
	STRINGSTREAM ss;
	ss << _T("GP: ") << *m_money;
	OffsetRect(&r, 0, cnv.GetHeight() - lineHeight);
	DrawText(hdc, ss.str().c_str(), ss.str().length(), &r, DT_CENTER | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);

	// Draw options at top-right of second rectangle.
	// Return a cursor map for the options.
	CCursorMap map;

	// Column widths. 
	// First column width = "Buy" width + padding.
	// Assume Buy/Sell/Exit have similar logical widths.
	STRING str = _T("Buy");
	GetTextExtentPoint32(hdc, str.c_str(), str.length(), &sz);
	
	CONST INT columns[] = {0, sz.cx + PAD * 4, cnv.GetWidth()};

	RECT rr = {columns[0], lineHeight + PAD, columns[1], 2 * lineHeight + PAD};
	DrawText(hdc, str.c_str(), str.length(), &rr, DT_CENTER | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);
	map.add(rr.left + EDGE + PAD * 2, int((rr.top + rr.bottom) / 2) + EDGE);

	str = _T("Sell");
	OffsetRect(&rr, 0, lineHeight + PAD);
	DrawText(hdc, str.c_str(), str.length(), &rr, DT_CENTER | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);
	map.add(rr.left + EDGE + PAD * 2, int((rr.top + rr.bottom) / 2) + EDGE);

	str = _T("Exit");
	OffsetRect(&rr, 0, lineHeight + PAD);
	DrawText(hdc, str.c_str(), str.length(), &rr, DT_CENTER | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);
	map.add(rr.left + EDGE + PAD * 2, int((rr.top + rr.bottom) / 2) + EDGE);

	// Clean up font.
	SelectObject(hdc, hOld);
	DeleteObject(hFont);
	cnv.CloseDC(hdc);

	// Draw bounding rectangles.
	cnv.DrawRect(0, 0, cnv.GetWidth() - 1, cnv.GetHeight() - 1, m_colors.line);
	cnv.DrawRect(0, 0, cnv.GetWidth() - 1, lineHeight - 1, m_colors.line);
	cnv.DrawRect(0, cnv.GetHeight() - lineHeight - 1, cnv.GetWidth() - 1, cnv.GetHeight() - 1, m_colors.line);

	// Screen destination.
	CONST INT x = (g_resX - cnv.GetWidth()) / 2, y = (g_resY - cnv.GetHeight()) / 2;

	// Draw transparently onto the translucently set backup.
	g_pDirectDraw->DrawCanvas(&backup, 0, 0, SRCCOPY);
	g_pDirectDraw->DrawCanvasTransparent(&cnv, x, y, m_colors.main);

	// Return the position for the item list.
	CONST RECT bounds = {columns[1] + PAD, lineHeight + PAD, columns[2] - PAD, cnv.GetHeight() - lineHeight - PAD};
	items = bounds;
	OffsetRect(&items, EDGE, EDGE);
	g_pDirectDraw->Refresh();
	return map;
}

/*
 * Draw and run the item menu and return whether a transaction occurred or not.
 */
BOOL CShop::itemMenu(CONST CCanvas &backup, CONST SHOP_OPTION type, CONST RECT bounds)
{
	extern CDirectDraw *g_pDirectDraw;

	CInventory *inv = (type == SHOP_BUY ? m_shopInv : m_playerInv);

	CONST INT width = bounds.right - bounds.left, height = bounds.bottom - bounds.top;

	CCanvas cnv;
	cnv.CreateBlank(NULL, width, height, TRUE);
	cnv.ClearScreen(m_colors.main);

	// Get the line height.
	SIZE sz;
	HDC hdc = cnv.OpenDC();
	GetTextExtentPoint32(hdc, STRING(_T("#### GP")).c_str(), STRING(_T("#### GP")).length(), &sz);
	cnv.CloseDC(hdc);

	// Indices of the first visible item and the highlighted item.
	INT firstLine = 0, selectedLine = 0;
	CONST INT lineHeight = sz.cy;

	// Height of the item information box (nominally 4 lines).
	CONST INT detailsHeight = lineHeight * 4 + PAD;

	// Number of list entries that can be visible at one time.
	CONST INT lines = (height - detailsHeight) / lineHeight - 1;

	// Positions of the columns within the list.
	CONST INT columns[] = {0, width - sz.cx - PAD, width};

	while (true)
	{
		cnv.ClearScreen(m_colors.main);
		cnv.DrawRect(0, 0, cnv.GetWidth() - 1, cnv.GetHeight() - 1, m_colors.line);
		cnv.DrawRect(0, cnv.GetHeight() - detailsHeight, cnv.GetWidth() - 1, cnv.GetHeight() - 1, m_colors.line);

		hdc = cnv.OpenDC();
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, m_colors.text);

		INT size = inv->size(), i = 0;
		if (size)
		{
			int j;
			// Advance the counter i to the first visible entry.
			for (j = 0; j < firstLine && i != size; ++j) ++i;
			
			// Draw each visible entry.
			for (j = 0; j < lines && i != size; ++i, ++j)
			{
				CONST LPITEM item = m_items[inv->fileAt(i)];
				STRINGSTREAM ss;
				
				// Draw handle.
				ss << item->itemName;
				if (type == SHOP_SELL) ss << _T(" (") << inv->quantityAt(i) << _T(")");
				
				RECT r = {columns[0], lineHeight * j, columns[1], height};
				OffsetRect(&r, PAD / 2, PAD / 2);
				DrawText(hdc, ss.str().c_str(), ss.str().length(), &r, DT_END_ELLIPSIS | DT_SINGLELINE);
				
				// Draw the cost.
				ss.str("");
				ss << (type == SHOP_BUY ? item->buyPrice : item->sellPrice) << _T(" GP");
				
				RECT p = {columns[1], lineHeight * j, columns[2], height};
				OffsetRect(&p, PAD / 2, PAD / 2);
				DrawText(hdc, ss.str().c_str(), ss.str().length(), &p, DT_END_ELLIPSIS | DT_SINGLELINE);
			}

			// Draw selected item information.
			// Selected line index is the index in inv.
			CONST LPITEM item = m_items[inv->fileAt(selectedLine)];
			
			// Description.
			STRING str = item->itmDescription;
			RECT r = {0, height - detailsHeight, cnv.GetWidth(), height - detailsHeight + lineHeight};
			OffsetRect(&r, PAD / 2, PAD / 2);
			DrawText(hdc, str.c_str(), str.length(), &r, DT_END_ELLIPSIS | DT_SINGLELINE);
			
			STRINGSTREAM ss;
			if (item->menuYN)
			{
				ss << _T("May be used from menu. ");

				if (item->mnuHPup > 0)
					ss << _T("Restores ") << item->mnuHPup << _T(" HP. ");
				else if (item->mnuHPup < 0)
					ss << _T("Removes ") << -item->mnuHPup << _T(" HP. ");
				
				if (item->mnuSMup > 0)
					ss << _T("Restores ") << item->mnuSMup << _T(" SMP.");
				else if (item->mnuSMup < 0)
					ss << _T("Removes ") << -item->mnuSMup << _T(" SMP.");
			}
			else
			{
				ss << _T("May not be used from menu.");
			}
			OffsetRect(&r, 0, lineHeight);
			DrawText(hdc, ss.str().c_str(), ss.str().length(), &r, DT_END_ELLIPSIS | DT_SINGLELINE);

			ss.str(_T(""));
			if (item->fightYN)
			{
				ss << _T("May be used in battle. ");

				if (item->fgtHPup > 0)
					ss << _T("Restores ") << item->fgtHPup << _T(" HP. ");
				else if (item->fgtHPup < 0)
					ss << _T("Removes ") << -item->fgtHPup << _T(" HP. ");
				
				if (item->fgtSMup > 0)
					ss << _T("Restores ") << item->fgtSMup << _T(" SMP.");
				else if (item->fgtSMup < 0)
					ss << _T("Removes ") << -item->fgtSMup << _T(" SMP.");
			}
			else
			{
				ss << _T("May not be used in battle.");
			}
			OffsetRect(&r, 0, lineHeight);
			DrawText(hdc, ss.str().c_str(), ss.str().length(), &r, DT_END_ELLIPSIS | DT_SINGLELINE);

			ss.str(_T(""));
			if (item->equipYN)
			{
				ss << _T("May be equipped. ");
			}
			else
			{
				ss << _T("May not be equipped.");
			}
			OffsetRect(&r, 0, lineHeight);
			DrawText(hdc, ss.str().c_str(), ss.str().length(), &r, DT_END_ELLIPSIS | DT_SINGLELINE);

			// Draw the selected item inverted.
			RECT rs = {0, lineHeight * (selectedLine - firstLine), width, height};
			OffsetRect(&rs, PAD / 2, PAD / 2);

			SetBkMode(hdc, OPAQUE);
			SetTextColor(hdc, m_colors.main);
			SetBkColor(hdc, m_colors.text);

			DrawText(hdc, item->itemName.c_str(), item->itemName.length(), &rs, DT_END_ELLIPSIS | DT_SINGLELINE);

		} // if (item count > 0)

		cnv.CloseDC(hdc);

		// Draw transparently onto the translucently set backup.
		g_pDirectDraw->DrawCanvasPartial(&backup, bounds.left, bounds.top, bounds.left, bounds.top, cnv.GetWidth(), cnv.GetHeight(), SRCCOPY);
		g_pDirectDraw->DrawCanvasTransparent(&cnv, bounds.left, bounds.top, m_colors.main);
		g_pDirectDraw->Refresh();

		CONST STRING key = waitForKey(true);

		// Exit if no items to select.
		if (!size) break;

		if (key == _T("DOWN") && selectedLine != size - 1)
		{
			// Scroll down.
			if (++selectedLine >= lines + firstLine) ++firstLine;
		}
		else if (key == _T("UP") && selectedLine != 0)
		{
			// Scroll up.
			if (--selectedLine < firstLine) --firstLine;
		}
		else if (key == _T("ENTER") || key == _T(" "))
		{
			// Buy / sell.
			if (transaction(type, inv->fileAt(selectedLine)))
			{
				return TRUE;
			}
		}
		else if (key == _T("ESC")) break;

	} // while (true)

	return FALSE;
}

/*
 * Process a buying or selling transaction.
 */
BOOL CShop::transaction(CONST SHOP_OPTION type, CONST STRING file)
{
	extern STRING g_projectPath;

	CONST LPITEM item = m_items[file];
	STRINGSTREAM ss;

	switch (type)
	{
		case SHOP_BUY:
		{
			if (*m_money < item->buyPrice)
			{
				messageBox(_T("You do not have enough GP to buy this item!"));
			}
			else
			{
				ss << _T("Buy one ") << item->itemName << _T(" for ") << item->buyPrice << _T(" GP?");

				// rpgcodeMsgBox returns vb MsgBox constants (6 = Yes).
				if (rpgcodeMsgBox(ss.str(), 2, m_colors.text, m_colors.main, STRING()) == 6)
				{
					m_playerInv->give(g_projectPath + ITM_PATH + file);
					*m_money -= item->buyPrice;
					return TRUE;
				}
			}
			break;
		} 
		case SHOP_SELL:
		{
			ss << _T("Sell one ") << item->itemName << _T(" for ") << item->sellPrice << _T(" GP?");
			if (rpgcodeMsgBox(ss.str(), 2, m_colors.text, m_colors.main, STRING()) == 6)
			{
				m_playerInv->take(g_projectPath + ITM_PATH + file);
				*m_money += item->sellPrice;
				return TRUE;
			}
		}
	} // switch (type)
}