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

#include "CAudioSegment.h"
#include "../common/paths.h"
#include "../rpgcode/parser/parser.h"
#include <mmsystem.h>
#include <string>

#define DRIVER_NONE 0
#define DRIVER_AUDIERE 2
#define DRIVER_MCI 1
HANDLE CAudioSegment::m_notify = NULL;
static CAudioSegment *g_pSoundEffect = NULL;

/*
 * Construct and load a file.
 */
CAudioSegment::CAudioSegment(const STRING file, STRING handle)
{
	init(handle);
	if (file != "")
		open(file);
}

#include "../common/mbox.h"

bool CAudioSegment::openMIDI(STRING file)
{
	extern STRING g_projectPath;

	int rc;
	STRING path = resolve(g_projectPath + MEDIA_PATH + file);
	STRING command = getAsciiString("open \"" + path + "\" type sequencer alias " + m_handle + " wait");

	rc = mciSendString(command.c_str(), NULL, 0, 0);
	if (rc != 0)
	{
		mciGetErrorString(rc, m_mciErr, m_mciErrLen);
		messageBox("Error opening file: " + path + " code: " + m_mciErr);
		m_ucDriver = DRIVER_NONE;
		return false;
	}

	// Set volume for L/R channels
	int midiVOL = ((m_volume / 100) * 0xFF00) | ((m_volume / 100) * 0x00FF);
	int ret = midiOutSetVolume((HMIDIOUT)m_handle.c_str(), midiVOL);
	return true;
}

/*
 * Open a file.
 */
bool CAudioSegment::open(const STRING file)
{
	extern STRING g_projectPath;

	if (_strcmpi(file.c_str(), m_file.c_str()) == 0)
	{
		// Already playing this file.
		return false;
	}
	stop();
	const STRING ext = parser::uppercase(getExtension(file));

	m_ucDriver = DRIVER_NONE;
	// If MDI file, use MCI functions
	if (ext == "MID")
	{

		if (openMIDI(file))
		{
			m_audiere = false;
			m_file = file;
			m_ucDriver = DRIVER_MCI;
			m_mci = true;
			return true;
		}
		return false;
	}
	else
	{		
		if (m_outputStream = audiere::OpenSound(m_device, getAsciiString(resolve(g_projectPath + MEDIA_PATH + file)).c_str(), true))
		{
			m_audiere = true;
			m_file = file;
			m_ucDriver = DRIVER_AUDIERE;
			return true;
		}
	}
	m_file = _T("");
	return false;
}

/*
 * Play this segment.
 */
void CAudioSegment::play(const bool repeat)
{
	if (isPlaying()) return;
	switch(m_ucDriver)
	{
	case DRIVER_AUDIERE:
		if (m_audiere)
		{
			if (m_outputStream)
			{
				m_outputStream->setVolume(m_volume / 100.0);
				m_outputStream->setRepeat(repeat);
				m_outputStream->play();
			}
		}
		break;
	case DRIVER_MCI:
		int rc;

		rc = mciSendString(getAsciiString("play " + m_handle).c_str(), NULL, 0, 0);
		if (rc != 0)
		{
			mciGetErrorString(rc, m_mciErr, m_mciErrLen);
			messageBox("Error playing file: " + m_file + " code: " + m_mciErr);
			m_ucDriver = DRIVER_NONE;
		}
		break;
	}
}

/*
 * Stop this segment.
 */
void CAudioSegment::stop()
{
	switch(m_ucDriver)
	{
	case DRIVER_AUDIERE:
		if (m_audiere)
		{
			if (m_outputStream)
			{
				m_outputStream->stop();
				m_outputStream->reset();
			}
		}
		break;
	case DRIVER_MCI:
		int rc;
		rc = mciSendString(getAsciiString("close " + m_handle).c_str(), NULL, 0, 0);
		m_mci = false;
		if (rc != 0)
		{
			mciGetErrorString(rc, m_mciErr, m_mciErrLen);
			messageBox("Error stopping file: " + m_file + " code: " + m_mciErr);
			m_ucDriver = DRIVER_NONE;
		}
		break;
	}
}

/*
 * Determine whether the segment is currently playing.
 */
bool CAudioSegment::isPlaying()
{
	switch(m_ucDriver)
	{
	case DRIVER_AUDIERE:
		if (m_audiere)
		{
			return (m_outputStream ? m_outputStream->isPlaying() : false);
		}
		break;
	case DRIVER_MCI:
		int rc;
		char buf[20];
		if (!m_mci)
		{
			openMIDI(m_file);
		}
		rc = mciSendString(getAsciiString("status " + m_handle + " ready").c_str(), buf, 19, 0);
		if (rc != 0)
		{
			mciGetErrorString(rc, m_mciErr, m_mciErrLen);
			messageBox("Error getting status for file: " + m_file + " code: " + m_mciErr);
			return true;
		}
		STRING temp = buf;
		// Ready was true, isPlaying returns FALSE
		if (temp == "true")
			return false;
		else
			return true;
		break;
	}
	return false;
}

/*
 * Initialize this audio segment.
 */
void CAudioSegment::init(STRING handle)
{
	HRESULT result;
	
	m_handle = handle;
	m_mciErrLen = sizeof(m_mciErr);
	m_audiere = false;
	m_mci = false;
	m_ucDriver = DRIVER_NONE;
	// Set up Audiere.
	m_device = audiere::OpenDevice();
}

/*
 * Event manager.
 * Used only for sound effects.
 */
DWORD WINAPI CAudioSegment::eventManager(LPVOID lpv)
{/*
	CAudioSegment *pAudioSegment = (CAudioSegment *)lpv;
	IDirectMusicPerformance8 *pPerf = pAudioSegment->m_pPerformance;
	DMUS_NOTIFICATION_PMSG *pMsg = NULL;

	while (m_notify)
	{
		WaitForSingleObject(m_notify, 100);
		while (pPerf->GetNotificationPMsg(&pMsg) == S_OK)
		{
			if (pMsg->guidNotificationType == GUID_NOTIFICATION_SEGMENT)
			{
				// Segment notification.
				// Check if it's a sound effect finishing playback.
				if (pMsg->dwNotificationOption == DMUS_NOTIFICATION_SEGEND)
				{
					// Delete the sound effect.
					pPerf->FreePMsg((DMUS_PMSG *)pMsg);
					delete pAudioSegment;
					return S_OK;
				}
			}
			pPerf->FreePMsg((DMUS_PMSG *)pMsg);
		}
	}
*/
	return S_OK;
}

/*
 * Play a sound effect, optionally idling until it is finished.
 */
void CAudioSegment::playSoundEffect(const STRING file, const bool waitToFinish)
{
	// Crappy -- but anything better will take a while
	// to implement, and I'd like to have some form
	// of sound effects done.

	// Try to avoid an infinite loop if the effect isn't loaded,
	// but allow the same effect to be played repeatedly.
	if (!g_pSoundEffect->open(file) && _strcmpi(file.c_str(), g_pSoundEffect->m_file.c_str()) != 0) return;
	g_pSoundEffect->play(false);

#if 0
	CAudioSegment *pSeg = new CAudioSegment();
	if (!pSeg->open(file))
	{
		delete pSeg;
		return;
	}

	if (pSeg->m_pPerformance)
	{
		pSeg->m_pPerformance->SetNotificationHandle(m_notify, 0);
		pSeg->m_pPerformance->AddNotificationType(GUID_NOTIFICATION_SEGMENT);
		DWORD id = 0;
		HANDLE hThread = CreateThread(NULL, 0, eventManager, pSeg, 0, &id);
		CloseHandle(hThread);
	}
	else
	{
		// Audiere events?
		// Delete for now.
		delete pSeg;
		return; // Temp!
	}

	pSeg->play(false);
#endif

	if (waitToFinish)
	{
		extern void processEvent();
		// Idle until the sound effect's event comes up.
		while (!g_pSoundEffect->isPlaying()) processEvent();
		// Idle until the sound effect ends.
		while (g_pSoundEffect->isPlaying()) processEvent();
	}

}

/*
 * Stop a sound effect.
 */
void CAudioSegment::stopSoundEffect()
{	
	g_pSoundEffect->stop(); 
}

/*
 * Set the volumes of all performances.
 */
void CAudioSegment::setMasterVolume(int percent)
{
	extern CAudioSegment *g_bkgMusic;
	percent = percent < 0 ? 0 : (percent > 100 ? 100 : percent);
	g_bkgMusic->setVolume(percent);
	g_pSoundEffect->setVolume(percent);
}

/*
 * Set the volume of a performance.
 */
void CAudioSegment::setVolume(const int percent)
{
	m_volume = percent;
	if (m_audiere)
	{
		// Volume is a float between 0.0 and 1.0.
		if (m_outputStream)
		{			
			m_outputStream->setVolume(percent / 100.0);
		}
	}
	else
	{
		// MasterVolume is a value in 1/100ths of a decibel ranging
		// between -100dB and +10dB (-30dB is quiet enough, though).
		// Note this is not really a linear relationship!
		//long db = (percent - 100) * 30;
		//m_volume = db;
	}	
}

/*
 * Initialize the Audio loader.
 */
void CAudioSegment::initLoader()
{
	extern STRING g_projectPath;
	HRESULT result;

	// Not loader related, but I don't feel like making another function.
	m_notify = CreateEvent(NULL, FALSE, FALSE, NULL);

	// Loader related stuff starts now.
#ifndef _UNICODE
	WCHAR searchPath[MAX_PATH + 1];
	MultiByteToWideChar(CP_ACP, 0, resolve(g_projectPath + MEDIA_PATH).c_str(), -1, searchPath, MAX_PATH);
	//m_pLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, searchPath, FALSE);
#else
	m_pLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, resolve(g_projectPath + MEDIA_PATH).c_str(), FALSE);
#endif

	// Initialise after CoCreateInstance().
	g_pSoundEffect = new CAudioSegment("", "SFX");
}

/*
 * Free the DirectMusic loader.
 */
void CAudioSegment::freeLoader()
{
	delete g_pSoundEffect;
}

/*
 * Deconstructor.
 */
CAudioSegment::~CAudioSegment()
{
	stop();
}
