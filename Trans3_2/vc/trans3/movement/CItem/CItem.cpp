/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2007 Jonathan D. Hughes & Colin James Fitzpatrick
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
 * Implementation of an item class.
 */

#include "CItem.h"
#include "../../rpgcode/CProgram.h"
#include "../../common/paths.h"
#include "../../common/CFile.h"
#include "../../fight/fight.h"

/*
 * Default constructor.
 */
CItem::CItem(const STRING file, const bool show, const bool thread):
CSprite(false),
m_pThread(NULL)
{
	if (open(file, thread) <= PRE_VECTOR_ITEM)
	{
		// Create standard vectors for old items.
		createVectors();
	}
	m_bActive = show;				// Overwrite open() result.
}

/*
 * Constructor - load an item directly from the board.
 */
CItem::CItem(const STRING file, const BRD_SPRITE spr, short &version, const bool thread):
CSprite(false),
m_pThread(NULL)
{
	m_brdData = spr;
	// Set the version that is passed in.
	version = open(file, thread);
}

/*
 * Common opening procedure. Return the minor version.
 */
short CItem::open(const STRING file, const bool thread) throw(CInvalidItem)
{
	const short minorVer = m_itemMem.open(file, &m_attr);
	if (minorVer == 0)
	{
		throw CInvalidItem();
	}

	// Load animations, but do not render frames.
	m_attr.loadAnimations(false);
	setAnm(MV_S);
	m_brdData.fileName = removePath(file, ITM_PATH);

	// Get these into milliseconds!
	m_attr.speed *= MILLISECONDS;
	m_attr.idleTime *= MILLISECONDS;

	// Override the item's programs for the board's, and
	// make them accessible to CSprite.
	if (m_brdData.prgMultitask.empty()) m_brdData.prgMultitask = m_itemMem.itmPrgOnBoard;
	if (m_brdData.prgActivate.empty()) m_brdData.prgActivate = m_itemMem.itmPrgPickUp;

	// Check activation conditions.
	m_bActive = true;

	if (!m_brdData.loadingVar.empty())
	{
		if (CProgram::getGlobal(m_brdData.loadingVar)->getLit() != m_brdData.loadingValue)
		{
			m_bActive = false;
		}
	}

	// Create thread
	if (thread && !m_brdData.prgMultitask.empty())
	{
		m_pThread = CItemThread::create(m_brdData.prgMultitask, this);
	}

	return minorVer;
}

/*
 * Attach a thread to the item.
 */
void CItem::attachThread(CItemThread *pThread)
{
	if (m_pThread) CThread::destroy(m_pThread);
	m_pThread = pThread;
}

/*
 * Create an item thread.
 */
CItemThread *CItemThread::create(const STRING str, CItem *pItem)
{
	CItemThread *p = new CItemThread(str);
	m_threads.insert(p);
	p->m_pItem = pItem;
	return p;
}

/*
 * Execute an item thread.
 */
bool CItemThread::execute(const unsigned int units)
{
	extern ENTITY g_target, g_source;

	if (!m_pItem->isActive() || (m_i == m_units.end())) return false;

	ENTITY t = g_target, s = g_source;

	g_target.p = g_source.p = m_pItem;
	g_target.type = g_source.type = ET_ITEM;

	unsigned int i = 0;
	while ((i++ < units) && (m_i != m_units.end()) && m_pItem->isActive() && !isSleeping())
	{
		m_i->execute(this);
		++m_i;
	} 
	g_target = t; g_source = s;

	return true;
}

/*
 * Deconstruct an item.
 */
CItem::~CItem()
{
	if (m_pThread)
	{
		CThread::destroy(m_pThread);
	}
}
