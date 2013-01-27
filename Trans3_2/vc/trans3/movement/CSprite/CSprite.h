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

#ifndef _CSPRITE_H_
#define _CSPRITE_H_

/*
 * Includes 
 */
#include "../../../tkCommon/board/conversion.h"
#include "../../../tkCommon/board/coords.h"
#include "../../../tkCommon/strings.h"
#include "../movement.h"
#include "../../common/sprite.h"
#include "../CVector/CVector.h"
#include "../CPathFind/CPathFind.h"

/*
 * Rpgcode flags.
 */
// Movement functions (Push(), ItemStep()...)
typedef enum tkMV_CONSTANTS
{
	tkMV_PAUSE_THREAD		= 1,
	tkMV_CLEAR_QUEUE		= 2,
	tkMV_PATHFIND			= 4,
	tkMV_WAYPOINT_PATH		= 8,				// Apply a waypoint path.
	tkMV_WAYPOINT_LINK		= 16				// Link to a waypoint path.
};

class CProgram;
class CThread;
struct tagBoard;
typedef struct tagBoard BOARD, *LPBOARD;

class CSprite  
{
public:
	CSprite(const bool show);				// Constructor.
	virtual ~CSprite() {}					// Destructor.
	static bool m_bPxMovement;				// Using pixel/tile movement (whole engine).

	void clearQueue(void);					// Clear the queue.
	void doMovement(						// Initiate rpgcode movements.
		const CProgram *prg, 
		const bool bPauseThread);
	void freePath(void) { if (m_pPathFind) m_pPathFind->freeData(); }
	void parseQueuedMovements(				// Parse a Push() string and pass to setQueuedMovement().
		const STRING str, 
		const bool bClearQueue);	
	PF_PATH pathFind( 						// Pathfind to pixel position x, y (same layer).
		const int x,
		const int y, 
		const int type,
		const int flags);
	void runQueuedMovements(void);			// Run all the movements in the queue.
	void setQueuedMovement(					// Place a movement in the sprite's queue.
		const int queue,
		const bool bClearQueue, 
		int step = 0);
	void setQueuedPath(						// Queue up a path-finding path.
		PF_PATH &path, 
		const bool bClearQueue);	
	void setQueuedPoint(					// Queue just one point to create a path.
		const DB_POINT pt, 
		const bool bClearQueue)	
	{
		if (bClearQueue) m_pos.path.clear();
		m_pos.path.push_back(pt);
	}
	void setBoardPath(						// Set a board vector as a path.
		CVector *const pV, 
		const int cycles, 
		const int flags);

	void drawPath(							// Draw the path this sprite is on.
		CCanvas *const cnv, 
		const LONG color);		
	void drawVector(CCanvas *const cnv);	// Draw the sprite's base vector.

	bool render( 							// Render frame to canvas.
		CCanvas *const cnv,
		const int layer,
		RECT &rect);
	void setAnm(MV_ENUM dir);				// Set the facing animation.
	void customStance( 						// Start a custom stance.
		const STRING stance, 
		const CProgram *prg, 
		const bool bPauseThread);
	
	void nullCanvas(void) { m_pCanvas = NULL; } // Free the pointer (when dangling).

	bool move(								// Evaluate the current movement state.
		const CSprite *selectedPlayer,
		const bool bRunningProgram);
	void deactivatePrograms(void);			// Override repeat values for programs the player is standing on.
	void playerDoneMove(void);				// Complete the selected player's move.
	bool doBoardEdges(void);				// Send player outside movement loop.
	bool programTest(const bool keypressOnly);// Test for program activations (by programs, items, players).
	void send(void);						// Unconditionally send the sprite to the active board.
	TILE_TYPE spriteCollisions(CSprite *&pSprite);		// Evaluate sprites (players and items).

	void alignBoard( 						// Align a RECT to the sprite's location.
		RECT &rect,
		const bool bAllowNegatives);
	void getDestination(DB_POINT &p) const;
	SPRITE_POSITION getPosition(void) const { return m_pos; }
	bool isActive(void) const { return m_bActive; }
	void setActive(const bool bActive) { m_bActive = bActive; }
	void setPosition(int x, int y, const int l, const COORD_TYPE coord);
	LPBRD_SPRITE getBoardSprite(void) { return &m_brdData; }

	// Create default vectors, overwriting any user-defined (PRE_VECTOR_ITEMs).
	void createVectors(void) { m_attr.createVectors(m_brdData.activationType); };
	// Determine if the sprite's base intersects a RECT.
	CVector getVectorBase(const bool bAtLocation) const
	{
		if (!bAtLocation) return m_attr.vBase;
		const DB_POINT p = { m_pos.x, m_pos.y };
		return (CVector(m_attr.vBase) + p);
	}

	// Return the number of pixels for the whole move (e.g. 32, 1, 2).
	int moveSize(void) const
	{
		const int result = (!m_bPxMovement ? 32 : round(PX_FACTOR / m_pos.loopSpeed));
		return (result < 1 ? 1 : result);
	}
	static void setLoopOffset(const int offset) { m_loopOffset = offset; }
	static int getLoopOffset(void) { return m_loopOffset; }
	void setSpeed(const double delay) { m_attr.speed = delay * MILLISECONDS; }

	// The facing direction - tie changes to alter the animation.
	class CFacing
	{
	public:
		CFacing(CSprite *owner): m_dir(MV_S), m_owner(owner) {}
		void assign(MV_ENUM dir) { m_dir = dir; m_owner->setAnm(m_dir); }
		MV_ENUM right(void) { MV_ENUM dir = m_dir; return ++dir; }
		MV_ENUM left(void) { MV_ENUM dir = m_dir; return --dir; }
		MV_ENUM dir(void) const { return m_dir; }
		CFacing &operator+= (unsigned int i) 
		{ 
			m_dir += i; 
			m_owner->setAnm(m_dir);
			return *this;
		}
		CFacing &operator= (CFacing &rhs) 
		{ 
			m_dir = rhs.m_dir; 
			m_owner->setAnm(m_dir);
			return *this;
		}
	private:
		MV_ENUM m_dir;
		CSprite *m_owner;
	};
	CFacing *getFacing(void) { return &m_facing; }


protected:
	SPRITE_ATTR m_attr;						// Sprite attributes (common file data).
	bool m_bActive;							// Is the sprite visible?
	BRD_SPRITE m_brdData;					// Board-set sprite data (activation variables).
	CCanvas *m_pCanvas;						// Pointer to sprite's frame.
	SPRITE_POSITION m_pos;					// Current location and frame details.
	TILE_TYPE m_tileType;					// The tiletypes at the sprite's location (NOT the "tiletype" of the sprite).
	DB_POINT m_v;							// Position vector in movement direction
	CPathFind *m_pPathFind;					// Sprite-specific pathfinding information.
	CFacing m_facing;						// Facing direction.
	CThread *m_thread;						// Sleeping thread id if moving in a thread.

private:
	static bool m_bDoneMove;				// Record whether we need to run playerDoneMove().
	static int m_loopOffset;				// Global speed offset.

	// Evaluate board vectors.
	TILE_TYPE boardCollisions(LPBOARD board, const bool recursing = false);
	// Take the angle of movement and return a MV_ENUM direction.
	static MV_ENUM getDirection(const DB_POINT &unitVector);
	
	TILE_TYPE checkBoardEdges(void);		// Tests for movement at the board edges.
	void checkIdling(void);					// Update idle and custom animations.
	DB_POINT getTarget(void) const;			// Get the next position co-ordinates.
	bool push(const bool bScroll);			// Complete a single frame's movement of the sprite.
	void setPathTarget(void);				// Insert target co-ordinates from the path.
	void setTarget(MV_ENUM direction);		// Increment target co-ordinates based on a direction.
	bool findDiversion(void);
	bool handleCollision(CSprite &sprite);

	// Calculate the loopSpeed - the number of renders that equate to
	// the sprite's movement speed (and any offsets).
	int calcLoops() const
	{
		// Frames per millisecond.
		extern double g_fpms;
		const int result = round(m_attr.speed * g_fpms) - (m_loopOffset * round(g_fpms * 100.0));
		return (result < 1 ? 1 : result);
	};
};

/*
 * A z-ordered vector of players and items.
 */
typedef std::vector<CSprite *>::iterator ZO_ITR;

typedef struct tagZOrderedSprites
{
	std::vector<CSprite *> v;

	// Form v into a z-ordered vector from g_players and g_items.
	void zOrder();

	// Remove a pointer from the vector.
	void remove(CSprite *p)
	{
		for (ZO_ITR i = v.begin(); i != v.end(); ++i)
		{
			if (*i == p) 
			{
				v.erase(i);
				return;
			}
		}
	};

	ZO_ITR find(const CSprite *pSprite)
	{
		ZO_ITR i;
		for (i = v.begin(); i != v.end(); ++i)
		{
			if (*i == pSprite) break;
		}
		return i;
	}

	 // Null the pointers to animation frame canvases when their
 	 // shared animations are destroyed (see setAmbientLevel()).
 	 void nullCanvases(void)
 	 {
 	  	for (ZO_ITR i = v.begin(); i != v.end(); ++i)
 	  	{
 	  		(*i)->nullCanvas();
 	  	}
	 }

} ZO_VECTOR;

#endif
