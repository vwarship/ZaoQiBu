/************************************************************************
    This file is part of VLCWrapper.
    
    File:   VLCWrapperImpl.cpp
    Desc.:  VLCWrapperImpl Implementation.

	Author:  Alex Skoruppa
	Date:    08/10/2009
	Updated: 03/12/2012
	eM@il:   alex.skoruppa@googlemail.com

	VLCWrapper is distributed under the Code Project Open License (CPOL).

	You should have received a copy of the Code Project Open License
	along with VLCWrapper.  If not, see <http://www.codeproject.com/info/cpol10.aspx>.
************************************************************************/
#include "stdafx.h"
#include "VLCWrapperImpl.h"
#include "vlc\vlc.h"
#include "vlc\libvlc_events.h"
#include <stdio.h>
#include <sstream>
#include <iomanip>

VLCWrapperImpl::VLCWrapperImpl(void)
:	pVLCInstance_(0),
	pMediaPlayer_(0),
	pMedia_(0),
    pEventManager_(0),
    eventHandler(0)
{
	const char * const vlc_args[] = {
		"-I", "dumy",      // No special interface
		"--ignore-config", // Don't use VLC's config
		"--plugin-path=./plugins" };

	// init vlc modules, should be done only once
	pVLCInstance_ = libvlc_new (sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
     
    // Create a media player playing environement
    pMediaPlayer_ = libvlc_media_player_new(pVLCInstance_);

    // Create an event manager for the player for handling e.g. time change events
    pEventManager_ = libvlc_media_player_event_manager(pMediaPlayer_);
}

VLCWrapperImpl::~VLCWrapperImpl(void)
{
    // Free the media_player
    libvlc_media_player_release (pMediaPlayer_);
	libvlc_release (pVLCInstance_);
}

void VLCWrapperImpl::SetOutputWindow(void* pHwnd)
{
    // Set the output window    
	libvlc_media_player_set_hwnd(pMediaPlayer_, pHwnd);
}

void VLCWrapperImpl::SetEventHandler( VLCEventHandler event, void* pUserData )
{
    eventHandler = event;
    libvlc_event_attach(pEventManager_,        
                        libvlc_MediaPlayerTimeChanged,
                        eventHandler,
                        pUserData);
}

void VLCWrapperImpl::Play()
{
	// play the media_player
    libvlc_media_player_play (pMediaPlayer_);
}

void VLCWrapperImpl::Pause()
{
	// Pause playing
    libvlc_media_player_pause (pMediaPlayer_);
}

void VLCWrapperImpl::Stop()
{
    // Stop playing
    libvlc_media_player_stop (pMediaPlayer_);
}

int64_t VLCWrapperImpl::GetLength()
{
    int64_t length = libvlc_media_player_get_length(pMediaPlayer_);
    return length;
}

int64_t VLCWrapperImpl::GetTime()
{
    int64_t time = libvlc_media_player_get_time(pMediaPlayer_);    
    return time;
}

void VLCWrapperImpl::SetTime( int64_t newTime )
{
    libvlc_media_player_set_time(pMediaPlayer_,(libvlc_time_t)newTime);
}

void VLCWrapperImpl::Mute( bool mute /*= true*/ )
{
    libvlc_audio_set_mute(pMediaPlayer_, mute);
}

bool VLCWrapperImpl::GetMute()
{
    bool bMuteState=!!libvlc_audio_get_mute(pMediaPlayer_);
    return bMuteState;
}

int VLCWrapperImpl::GetVolume()
{
    int volume = libvlc_audio_get_volume(pMediaPlayer_);
    return volume;
}

void VLCWrapperImpl::SetVolume( int volume )
{
    libvlc_audio_set_volume(pMediaPlayer_, volume);
}

void VLCWrapperImpl::OpenMedia(const char* pMediaPathName)
{
	// Load a new item
	pMedia_ = libvlc_media_new_path(pVLCInstance_, pMediaPathName);
	libvlc_media_player_set_media(pMediaPlayer_, pMedia_);

	//////get file information
	//libvlc_media_parse(pMedia_);
	//unsigned w = 0, h = 0;
	//libvlc_video_get_size(pMediaPlayer_, 0, &w, &h);
	//libvlc_meta_t meta;
	//char *s = libvlc_media_get_meta(pMedia_, libvlc_meta_Title);
	////libvlc_media_get_tracks_info(pMedia_, );
}

MediaState VLCWrapperImpl::GetMediaState()
{
	if (!pMedia_)
		return NothingSpecial;
	return (MediaState)(libvlc_media_get_state(pMedia_));
}

bool VLCWrapperImpl::IsPlaying()
{
	return libvlc_media_player_is_playing(pMediaPlayer_);
}

void VLCWrapperImpl::Snapshot()
{
	unsigned width = 0, height = 0;
	libvlc_video_get_size(pMediaPlayer_, 0, &width, &height);

	SYSTEMTIME curTime = { 0 };
	GetLocalTime(&curTime);

	std::ostringstream filename;
	filename << "snapshot_" 
		<< std::setfill('0')
		<< std::setw(4) << curTime.wYear
		<< std::setw(2) << curTime.wMonth
		<< std::setw(2) << curTime.wDay << '_'
		<< std::setw(2) << curTime.wHour
		<< std::setw(2) << curTime.wMinute
		<< std::setw(2) << curTime.wSecond << '_'
		<< std::setw(3) << curTime.wMilliseconds
		<< ".png";

	libvlc_video_take_snapshot(pMediaPlayer_, 0,
		filename.str().data(), 
		width, height);


	//static bool bFullScreen = false;
	//HWND hWnd = (HWND)libvlc_media_player_get_hwnd(pMediaPlayer_);
	//if (hWnd)
	//{
	//	bFullScreen = !bFullScreen;
	//	//::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//	ShowWindow(hWnd, bFullScreen?SW_SHOWMAXIMIZED:SW_SHOWNORMAL);
	//}

	//libvlc_video_set_scale(pMediaPlayer_, 2.5);

	//libvlc_media_player_t *mp = pMediaPlayer_;
	//bool fs = libvlc_get_fullscreen(mp);
	//libvlc_set_fullscreen(mp, true);
	//assert(libvlc_get_fullscreen(mp));
	//libvlc_set_fullscreen(mp, false);
	//assert(!libvlc_get_fullscreen(mp));
	//libvlc_toggle_fullscreen(mp);
	//assert(libvlc_get_fullscreen(mp));
	//libvlc_toggle_fullscreen(mp);
	//assert(!libvlc_get_fullscreen(mp));
	//libvlc_set_fullscreen(mp, fs);
	//assert(libvlc_get_fullscreen(mp) == fs);

	//assert(libvlc_video_get_scale(mp) == 0.); /* default */
	//libvlc_video_set_scale(mp, 0.); /* no-op */
	//libvlc_video_set_scale(mp, 2.5);
	//assert(libvlc_video_get_scale(mp) == 2.5);
	//libvlc_video_set_scale(mp, 0.);
	//libvlc_video_set_scale(mp, 0.); /* no-op */
	//assert(libvlc_video_get_scale(mp) == 0.);
}