#include "stdafx.h"
#include "CoursePlayer.h"
#include "VideoPlayerDlg.h"

void CoursePlayer::VLCEventHandler(const VLCEvent *event, void *userData)
{
	CVideoPlayerDlg *dlg = reinterpret_cast<CVideoPlayerDlg*>(userData);
	switch (event->type)
	{
	case libvlc_MediaPlayerTimeChanged:
		if (dlg)
		{
			dlg->PostMessage(UM_COURSE_PLAYER_TIME_CHANGED);
		}
		break;
	}
}

void CoursePlayer::OpenMedia(const tstring &filePath)
{
	m_vlcPlayer.OpenMedia(EncodeToUTF8(std::string(CT2A(filePath.data()))).data());
}

std::string CoursePlayer::EncodeToUTF8(const std::string &mbcsStr) const
{
	int len = MultiByteToWideChar(CP_UTF8, 0, &mbcsStr[0], -1, NULL, 0);

	std::wstring wcharStr(sizeof(wchar_t)*len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, &mbcsStr[0], -1, &wcharStr[0], len);

	len = WideCharToMultiByte(CP_UTF8, 0, &wcharStr[0], -1, NULL, 0, NULL, NULL);

	std::string utf8Str(len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, &wcharStr[0], -1, &utf8Str[0], len, NULL, NULL);

	return utf8Str;
}

void CoursePlayer::Play()
{
	m_vlcPlayer.Play();
}

void CoursePlayer::Pause()
{
	m_vlcPlayer.Pause();
}

void CoursePlayer::Stop()
{
	m_vlcPlayer.Stop();
}

bool CoursePlayer::IsPlaying()
{
	return m_vlcPlayer.GetMediaState() == Playing;
}

bool CoursePlayer::IsNotOpen()
{
	return m_vlcPlayer.GetMediaState() == NothingSpecial;
}

bool CoursePlayer::IsPaused()
{
	return m_vlcPlayer.GetMediaState() == Paused;
}

bool CoursePlayer::IsStopped()
{
	return m_vlcPlayer.GetMediaState() == Stopped;
}

bool CoursePlayer::IsEnded()
{
	return m_vlcPlayer.GetMediaState() == Ended;
}

bool CoursePlayer::IsError()
{
	return m_vlcPlayer.GetMediaState() == Error;
}

void CoursePlayer::Snapshot()
{
	m_vlcPlayer.Snapshot();
}

void CoursePlayer::SetTime(int64_t newTime)
{
	m_vlcPlayer.SetTime(newTime);
}

int64_t CoursePlayer::GetTime()
{
	return m_vlcPlayer.GetTime();
}

int64_t CoursePlayer::GetLength()
{
	return m_vlcPlayer.GetLength();
}

void CoursePlayer::SetVolume(int volume)
{
	m_vlcPlayer.SetVolume(volume);
}

int CoursePlayer::GetVolume()
{
	return m_vlcPlayer.GetVolume();
}

void CoursePlayer::SetMute(bool mute/* = true*/)
{
	m_vlcPlayer.Mute(mute);
}

bool CoursePlayer::GetMute()
{
	return m_vlcPlayer.GetMute();
}
