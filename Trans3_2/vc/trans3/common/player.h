/*
 ********************************************************************
 * The RPG Toolkit, Version 3
 * This file copyright (C) 2006  Christopher Matthews & contributors
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

#ifndef _PLAYER_H_
#define _PLAYER_H_

/*
 * Inclusions.
 */
#include "../../tkCommon/strings.h"
#include "sprite.h"

/*
 * Definitions.
 */
#define PRE_VECTOR_PLAYER	7					// Last version before vectors.		

#define UBOUND_GFX 13
#define UBOUND_STANDING_GFX 7

/*
 * Player stats.
 */
typedef struct tagPlayerStats
{
	int experience;								// Initial Experience Level.
	int health;									// Initial health level.
	int maxHealth;								// Initial maximum health level.
	int defense;								// Initial DP.
	int fight;									// Initial FP.
	int sm;										// Initial SM power.
	int smMax;									// Initial Max SM power.
	int level;									// Initial level.
} PLAYER_STATS, *LPPLAYER_STATS;

/*
 * A player.
 */
typedef struct tagPlayer
{
	STRING fileName;

	STRING charname;						// Character name.
	STRING experienceVar;					// Experience variable.
	STRING defenseVar;						// DP variable.
	STRING fightVar;						// FP variable.
	STRING healthVar;						// HP variable.
	STRING maxHealthVar;					// Max HP var.
	STRING nameVar;						// Character name variable.
	STRING smVar;							// Special Move power variable.
	STRING smMaxVar;						// Special Move maximum variable.
	STRING leVar;							// Level variable.
	PLAYER_STATS stats;					// Stats.
	STRING profilePic;						// Profile picture.
	std::vector<STRING> smlist;			// Special Move list (200 in total!).
	std::vector<int> spcMinExp;					// Minimum experience for each move.
	std::vector<int> spcMinLevel;				// Min level for each move.
	std::vector<STRING> spcVar;			// Conditional variable for each special move.
	std::vector<STRING> spcEquals;			// Condition of variable for each special move.
	STRING specialMoveName;				// Name of special move.
	char smYN;									// Does he do special moves? 0-Y, 1-N.
	std::vector<STRING> accessoryName;		// Names of 10 accessories.
	std::vector<char> armorType;				// Is ARMORTYPE used (0-N,1-Y).  Armour types are: 1-head,2-neck,3-lh,4-rh,5-body,6-legs.
	int levelType;								// Initial Level progression.
	short experienceIncrease;					// Experience increase Factor.
	int maxLevel;								// Maximum level.
	short levelHp;								// HP incrase by % when level increaes.
	short levelDp;								// DP incrase by % when level increaes.
	short levelFp;								// FP incrase by % when level increaes.
	short levelSm;								// SMP incrase by % when level increaes.
	STRING charLevelUpRPGCode;				// Rpgcode program to run on level up.
	char charLevelUpType;						// Level up type 0- exponential, 1-linear.
	char charSizeType;							// Size type: 0- 32x32, 1 - 64x32.
	short nextLevel;							// Exp value at which level up occurs.
	short levelProgression;						// Exp required until level up.
	std::vector<double> levelStarts;			// Exp values at which all levels start.

	short open(const STRING fileName, SPRITE_ATTR &spriteAttr);
} PLAYER, *LPPLAYER;

#endif
