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

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

/*
 * Inclusions.
 */
#include "../../tkCommon/strings.h"
#include "../render/render.h"
#include "tilebitmap.h"
#include <string>
#include <vector>

/*
 * An animation.
 */
typedef struct tagAnimation
{
	STRING filename;					// Filename (no path).
	int pxWidth;						// Width.
	int pxHeight;						// Height.
	int frameCount;						// Total number of frames.
	std::vector<STRING> frameFiles;		// Filenames of each image in animation.
	std::vector<int> transpColors;		// Transparent color for frame.
	std::vector<STRING> sounds;			// Sounds for each frame.
	double delay;						// Pause length (sec) between each frame.

	bool open(const STRING fileName);
	void save(const STRING fileName) const;
	void loadFromGif(const STRING file);
	tagAnimation(): frameCount(0), pxWidth(1), pxHeight(1), delay(0) {}

} ANIMATION, *LPANIMATION;

/*
 * One frame of an animation.
 */
typedef struct tagAnimationFrame
{
	tagAnimationFrame(void): cnv(NULL) { }
	CCanvas *cnv;					// Canvas of frame.
	STRING file;					// Animation filename.
	int frame;							// Frame number.
	int maxFrames;						// Max frames in this anim.
	STRING strSound;				// Sound played on this frame.
} ANIMATION_FRAME;


void drawImage(const STRING strFile, const HDC hdc, const int x, const int y, const int width, const int height);

void drawImage(const STRING strFile, CCanvas *const cnv, const int x, const int y, const int width, const int height);

#endif
