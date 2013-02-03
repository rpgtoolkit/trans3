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

#ifndef _CAUDIO_SEGMENT_H_
#define _CAUDIO_SEGMENT_H_

#define WIN32_LEAN_AND_MEAN
#define DIRECTSOUND_VERSION 0x0800
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <set>
#include "audiere.h"
#include "../../tkCommon/strings.h"

class CAudioSegment
{
	bool openMIDI(STRING handle);
public:
	static void initLoader();
	static void freeLoader();
	CAudioSegment() { init("BGM"); }
	CAudioSegment(const STRING file, STRING handle);
	~CAudioSegment();
	bool open(const STRING file);
	void play(const bool repeat);
	static void playSoundEffect(const STRING file, const bool waitToFinish);
	static void stopSoundEffect();
	static void setMasterVolume(int percent);
	void stop();

	STRING getPlayingFile() const { return m_file; }

protected:
	CAudioSegment(const CAudioSegment &rhs);			// No implementation.
	CAudioSegment &operator=(const CAudioSegment &rhs); // No implementation.
	void init(STRING handle);
	bool isPlaying();
	void setVolume(const int percent);
	static DWORD WINAPI eventManager(LPVOID lpv);

	static HANDLE m_notify;
	audiere::AudioDevicePtr m_device;
	audiere::OutputStreamPtr m_outputStream;
	bool m_audiere;
	unsigned char m_ucDriver;
	STRING m_file;
	STRING m_handle;
	char m_mciErr[130];
	int m_mciErrLen;
	bool m_mci;
	long m_volume; // for MCI
};

#endif
