/************************************************************************
    This file is part of VLCWrapper.
    
    File:    VLCWrapper.cpp
    Desc.:   VLCWrapper Implementation.

	Author:  Alex Skoruppa
	Date:    08/10/2009
	Updated: 03/12/2012
	eM@il:   alex.skoruppa@googlemail.com

	VLCWrapper is distributed under the Code Project Open License (CPOL).

	You should have received a copy of the Code Project Open License
	along with VLCWrapper.  If not, see <http://www.codeproject.com/info/cpol10.aspx>.
************************************************************************/
#include "stdafx.h"
#include "VLCWrapper.h"
#include "VLCWrapperImpl.h"

VLCWrapper::VLCWrapper(void)
:   pImpl_(std::auto_ptr<VLCWrapperImpl>(new VLCWrapperImpl()))
{
}

VLCWrapper::~VLCWrapper(void)
{
}

void VLCWrapper::SetOutputWindow(void* pHwnd)
{
    pImpl_->SetOutputWindow(pHwnd);
}

void VLCWrapper::SetEventHandler(VLCEventHandler event, void* pUserData)
{
    pImpl_->SetEventHandler(event, pUserData);
}

void VLCWrapper::Play()
{
    pImpl_->Play();
}

void VLCWrapper::Pause()
{
    pImpl_->Pause();
}

void VLCWrapper::Stop()
{
    pImpl_->Stop();
}

int64_t VLCWrapper::GetLength()
{
    return pImpl_->GetLength();
}

int64_t VLCWrapper::GetTime()
{
    return pImpl_->GetTime();
}

void VLCWrapper::SetTime(int64_t newTime)
{
    pImpl_->SetTime(newTime);
}

void VLCWrapper::Mute(bool mute)
{
    pImpl_->Mute(mute);
}

bool VLCWrapper::GetMute()
{
    return pImpl_->GetMute();
}

int  VLCWrapper::GetVolume()
{
    return pImpl_->GetVolume();
}

void VLCWrapper::SetVolume(int volume)
{
    pImpl_->SetVolume(volume);
}

void VLCWrapper::OpenMedia(const char* pMediaPathName)
{
    pImpl_->OpenMedia(pMediaPathName);
}

MediaState VLCWrapper::GetMediaState()
{
	return pImpl_->GetMediaState();
}

bool VLCWrapper::IsPlaying()
{
	return pImpl_->IsPlaying();
}

void VLCWrapper::Snapshot()
{
	pImpl_->Snapshot();
}
