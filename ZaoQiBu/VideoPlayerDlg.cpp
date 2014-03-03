#include "stdafx.h"
#include "VideoPlayerDlg.h"

CVideoPlayerDlg::CVideoPlayerDlg()
: m_isFullScreen(false)
{
	LoadConfig();
}

void CVideoPlayerDlg::LoadConfig()
{
	m_config.Load(_T("zaoqibu.xml"));
}

CVideoPlayerDlg::~CVideoPlayerDlg()
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
}

BOOL CVideoPlayerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			FullScreen(false);
		}
		else if (pMsg->wParam == VK_F11)
		{
			FullScreen(!m_isFullScreen);
		}
		else if (pMsg->wParam == VK_F12)
		{
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_SNAPSHOT, BN_CLICKED), 0);
		}
		else if (pMsg->wParam == VK_SPACE)
		{
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_COURSE_PLAY, BN_CLICKED), 0);
			m_bmpBtnPlay.Invalidate();
		}
		else if (pMsg->wParam == VK_PRIOR)
		{
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_COURSE_PREV_CHAPTER, BN_CLICKED), 0);
			return TRUE;
		}
		else if (pMsg->wParam == VK_NEXT)
		{
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_COURSE_NEXT_CHAPTER, BN_CLICKED), 0);
			return TRUE;
		}
		else if (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)
		{
			return TRUE; //不转发给子控件
		}
	}
	else if (pMsg->message == UM_COURSE_PLAYER_TIME_CHANGED)
	{
		BOOL bHandled = 0;
		OnCoursePlayerTimeChanged(0, 0, 0, bHandled);
	}

	return IsDialogMessage(pMsg);
}

BOOL CVideoPlayerDlg::OnIdle()
{
	return FALSE;
}

LRESULT CVideoPlayerDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	m_courseList.SubclassWindow(GetDlgItem(IDC_COURSE_LIST));
	m_courseList.SetItemHeight(0, 70);

	m_courseImageList.Create(64, 64, TRUE | ILC_COLOR32, 1, 5);

	m_courseChapterList.SubclassWindow(GetDlgItem(IDC_COURSE_ITEM_LIST));
	m_courseChapterList.SetItemHeight(0, 30);

	//ILBSETTINGS cfg;
	//m_courseList.GetPreferences(&cfg);
	//cfg.clrHighlite = ::GetSysColor(COLOR_HIGHLIGHT);	//RGB(247, 247, 247);
	//cfg.clrHighliteText = RGB(0, 0, 0);
	//cfg.clrHighliteBorder = RGB(148, 150, 148);
	//cfg.clrText = RGB(0, 0, 0);
	//cfg.clrBackground = RGB(255, 255, 255);
	//cfg.sizeMargin.cx = cfg.sizeMargin.cy = 1;
	//cfg.sizeSubIndent.cx = 10;
	//cfg.sizeSubIndent.cy = 4;
	//m_courseList.SetPreferences(cfg);

	InsertCourses();
	SetCurrentCourse(0);
	SetCurrentChapter(0);

	m_coursePlayer.SubclassWindow(GetDlgItem(IDC_COURSE_PLAYER));

	CreateBitmapButton(ID_APP_EXIT, { IDB_APP_EXIT }, _T("退出"), m_bmpBtnAppExit);
	CreateBitmapButton(IDC_COURSE_PREV_CHAPTER, { IDB_COURSE_PREV_CHAPTER }, _T("上一章 热键:Page Up"), m_bmpBtnCoursePrevChapter);
	CreateBitmapButton(IDC_COURSE_PLAY, { IDB_COURSE_PLAY, IDB_COURSE_PAUSE }, _T("播放 热键:空格"), m_bmpBtnPlay);
	CreateBitmapButton(IDC_COURSE_NEXT_CHAPTER, { IDB_COURSE_NEXT_CHAPTER }, _T("下一章 热键:Page Down"), m_bmpBtnCourseNextChapter);
	CreateBitmapButton(IDC_FULL_SCREEN, { IDB_FULL_SCREEN }, _T("全屏 热键:F11"), m_bmpBtnFullScreen);
	CreateBitmapButton(IDC_SNAPSHOT, { IDB_SNAPSHOT }, _T("快照 热键:F12"), m_bmpBtnSnapshot);
	CreateBitmapButton(IDC_MUTE, { IDB_SOUND_ON, IDB_SOUND_OFF }, _T("音量"), m_bmpBtnMute);

	m_hWndMediaTime = GetDlgItem(IDC_MEDIA_TIME);
	m_iMediaTime = MEDIA_TIME_DEFAULT_VALUE;
	m_sMediaCurrentTime = m_sMediaLength = _T("00:00:00");

	m_hWndVolume = GetDlgItem(IDC_VOLUME);
	m_iVolume = VOLUME_DEFAULT_VALUE;

	InitVolume();

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	DoDataExchange(false);
	DlgResize_Init(false, false, 0);

	return TRUE;
}

void CVideoPlayerDlg::InitVolume()
{
	SetSliderRange(m_hWndVolume, 0, 100);
	SetSliderPos(m_hWndVolume, m_iVolume);
}

void CVideoPlayerDlg::InsertCourses()
{
	size_t i = 0;
	const std::vector<Course>& courses = m_config.GetCourses();
	for (const Course &course : courses)
	{
		const Course &course = courses[i];
		InsertCourse(course, i);

		++i;
	}
}

void CVideoPlayerDlg::InsertCourse(const Course &course, int iImage)
{
	AddCourseImage(course.GetIcon().data());

	ILBITEM item = { 0 };
	item.mask = ILBIF_TEXT | ILBIF_IMAGE | ILBIF_SELIMAGE |
		ILBIF_STYLE | ILBIF_FORMAT;
	item.iItem = iImage;
	item.iImage = iImage;
	item.iSelImage = iImage;
	item.pszText = const_cast<LPTSTR>(course.GetTitle().data());
	item.style = ILBS_IMGLEFT | ILBS_SELROUND;
	item.format = DT_LEFT;
	m_courseList.InsertItem(&item);

}

void CVideoPlayerDlg::AddCourseImage(LPCTSTR pszImageFilename)
{
	CBitmap bitmap = AtlLoadGdiplusImage(pszImageFilename);
	if (bitmap.IsNull())
		bitmap = LoadBitmapWithPNG(IDB_COURSE_ICON_CARROT);

	SIZE size = { 0 };
	m_courseImageList.GetIconSize(size);

	////
	SIZE srcSize = { 0 };
	bitmap.GetSize(srcSize);

	CDC srcDC;
	srcDC.CreateCompatibleDC(GetDC());
	HBITMAP hOldSrcBitmap = srcDC.SelectBitmap(bitmap);

	CDC destDC;
	destDC.CreateCompatibleDC(GetDC());
	CBitmap destBitmap;
	destBitmap.CreateCompatibleBitmap(GetDC(), size.cx, size.cy);
	HBITMAP hOldDestBitmap = destDC.SelectBitmap(destBitmap);
	destDC.SetStretchBltMode(HALFTONE);
	CRect destRect = CalcDestImageRect(srcSize, size);
	destDC.StretchBlt(destRect.left, destRect.top, destRect.Width(), destRect.Height(), srcDC, 0, 0, srcSize.cx, srcSize.cy, SRCCOPY);
	destDC.SelectBitmap(hOldDestBitmap);

	srcDC.SelectBitmap(hOldSrcBitmap);
	/////

	m_courseImageList.Add(destBitmap);
	m_courseList.SetImageList(m_courseImageList.m_hImageList, ILSIL_NORMAL);
	m_courseList.SetImageList(m_courseImageList.m_hImageList, ILSIL_SELECTED);
}

CRect CVideoPlayerDlg::CalcDestImageRect(SIZE srcSize, SIZE destSize) const
{
	if (srcSize.cx >= destSize.cx || srcSize.cy >= destSize.cy)
	{
		// 计算出实际宽高和目标宽高的比率
		double widthRatio = (double)srcSize.cx / destSize.cx;
		double heightRatio = (double)srcSize.cy / destSize.cy;

		// 选择宽和高中最小的比率作为inSampleSize的值，这样可以保证最终图片的宽和高
		// 一定都会大于等于目标的宽和高。
		double ratio = heightRatio > widthRatio ? heightRatio : widthRatio;

		SIZE destRealSize = { (int)(srcSize.cx / ratio), (int)(srcSize.cy / ratio) };
		CRect destRect = { 0, 0, destSize.cx, destSize.cy };
		if (heightRatio > widthRatio) //调整宽度
		{
			int adjustSize = (destRect.Width() - destRealSize.cx) / 2;
			destRect.DeflateRect(adjustSize, 0);
		}
		else
		{
			int adjustSize = (destRect.Height() - destRealSize.cy) / 2;
			destRect.DeflateRect(0, adjustSize);
		}

		return destRect;
	}

	return CRect();
}

int CVideoPlayerDlg::GetCurrentCourse() const
{
	return m_courseList.GetCurSel();
}

void CVideoPlayerDlg::SetCurrentCourse(int courseIndex)
{
	SelectListBox(m_courseList, courseIndex);
}

int CVideoPlayerDlg::GetCurrentChapter() const
{
	return m_courseChapterList.GetCurSel();
}

void CVideoPlayerDlg::SetCurrentChapter(int chapterIndex)
{
	SelectListBox(m_courseChapterList, chapterIndex);
}

void CVideoPlayerDlg::SelectListBox(CImageListBoxCtrl &listbox, int index)
{
	if (listbox.GetCount() == 0)
		return;

	listbox.SetCurSel(index);
	SendMessage(WM_COMMAND,
		MAKEWPARAM(listbox.GetDlgCtrlID(), LBN_SELCHANGE),
		(LPARAM)listbox.m_hWnd);
}

void CVideoPlayerDlg::DeleteAllCourseChapters()
{
	for (int i = m_courseChapterList.GetCount() - 1; i >= 0; --i)
	{
		PILBITEM pItem = (PILBITEM)m_courseChapterList.GetItemDataPtr(i);

		DELETEITEMSTRUCT dis = { 0 };
		dis.itemData = reinterpret_cast<ULONG_PTR>(pItem);
		m_courseChapterList.DeleteItem(&dis);
		m_courseChapterList.DeleteString(i);
	}
}

void CVideoPlayerDlg::AddCourseChapters()
{
	const Course &course = GetCurrentCourse();
	for (size_t i = 0; i < course.GetChapterCount(); ++i)
	{
		const Chapter &chapter = course.GetChapter(i);
		tstring itemName = FileUtil::GetFileName(chapter.GetFilePath());

		size_t len = itemName.length() + 1;
		vector<TCHAR> buf(len, '\0');
		_tcscpy_s(&buf[0], len, itemName.data());

		ILBITEM item = { 0 };
		item.mask = ILBIF_TEXT | ILBIF_STYLE | ILBIF_FORMAT;
		item.iItem = i;
		item.pszText = &buf[0];
		item.style = ILBS_SELROUND;
		item.format = DT_LEFT | DT_VCENTER;
		m_courseChapterList.InsertItem(&item);
	}
}

void CVideoPlayerDlg::CreateBitmapButton(int nButtonID, const std::vector<int> &imageIDs, PCTSTR toolTipText, CBitmapButton &bitmapButton)
{
	bitmapButton.SubclassWindow(GetDlgItem(nButtonID));
	bitmapButton.SetToolTipText(toolTipText);
	bitmapButton.SetBitmapButtonExtendedStyle(BMPBTN_HOVER);

	size_t imageIDCount = imageIDs.size();

	CImageList imageList;
	imageList.Create(48, 48, TRUE | ILC_COLOR32, 2 * imageIDCount, 2 * imageIDCount);

	for (int imageID : imageIDs)
	{
		imageList.Add(LoadBitmapWithPNG(imageID));
		imageList.Add(CreateHoverBitmapWithPNG(GetDC(), imageID));
	}

	bitmapButton.SetImageList(imageList);
	bitmapButton.SetImages(0, -1, 1);
}

HBITMAP CVideoPlayerDlg::LoadBitmapWithPNG(int nID)
{
	return AtlLoadGdiplusImage(nID, _T("PNG"));
}

HBITMAP CVideoPlayerDlg::CreateHoverBitmapWithPNG(HDC hDC, int nID)
{
	CBitmap srcBitmap = LoadBitmapWithPNG(nID);

	SIZE bitmapSize = { 0 };
	srcBitmap.GetSize(bitmapSize);

	CDC dc;
	dc.CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = dc.SelectBitmap(srcBitmap);
	dc.BitBlt(0, 0, bitmapSize.cx, bitmapSize.cy, dc, 0, 0, PATINVERT);
	dc.SelectBitmap(hOldBitmap);

	return srcBitmap.Detach();
}

void CVideoPlayerDlg::UpdatePlayTime()
{
	int64_t curPos = m_coursePlayer.GetTime();
	int64_t mediaLength = m_coursePlayer.GetLength();

	CTimeSpan curPosTime(static_cast<time_t>(curPos / 1000));
	CTimeSpan mediaLengthTime(static_cast<time_t>(mediaLength / 1000));

	static PCTSTR TIME_FORMAT = _T("%02d:%02d:%02d");
	m_sMediaCurrentTime.Format(TIME_FORMAT,
		curPosTime.GetHours(),
		curPosTime.GetMinutes(),
		curPosTime.GetSeconds());

	m_sMediaLength.Format(TIME_FORMAT,
		mediaLengthTime.GetHours(),
		mediaLengthTime.GetMinutes(),
		mediaLengthTime.GetSeconds());

	SetMediaTimePos(curPos);

	DoDataExchange(false);
}

void CVideoPlayerDlg::SetMediaTimePos(int64_t curPos)
{
	m_iMediaTime = curPos;
	SetSliderPos(m_hWndMediaTime, (LPARAM)m_iMediaTime / 1000);
}

void CVideoPlayerDlg::SetSliderPos(HWND hWnd, LPARAM pos)
{
	::SendMessage(hWnd, (UINT)TBM_SETPOS, (WPARAM)TRUE, pos);
}

void CVideoPlayerDlg::SetSliderRange(HWND hWnd, DWORD start, DWORD end)
{
	::SendMessage(hWnd, (UINT)TBM_SETRANGE, (WPARAM)0, 
		(LPARAM)MAKELONG(start, end));
}

void CVideoPlayerDlg::FullScreen(bool isFullScreen)
{
	if (isFullScreen)
	{
		ShowChildWindows(!isFullScreen);
	}

	m_coursePlayer.ShowWindow(isFullScreen ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);

	if (!isFullScreen)
	{
		ShowChildWindows(!isFullScreen);
	}

	m_isFullScreen = isFullScreen;
}

void CVideoPlayerDlg::ShowChildWindows(bool isShow)
{
	static const int windowIDs[] = {
		IDC_LOGO,
		ID_APP_EXIT,
		IDC_COURSE_LIST,
		IDC_COURSE_ITEM_LIST,
		IDC_COURSE_PREV_CHAPTER,
		IDC_COURSE_PLAY,
		IDC_COURSE_NEXT_CHAPTER,
		IDC_SNAPSHOT,
		IDC_FULL_SCREEN,
		IDC_MUTE, IDC_VOLUME,
		IDC_MEDIA_CURRENT_TIME,
		IDC_MEDIA_TIME,
		IDC_MEDIA_LENGTH,
	};

	for (int nID : windowIDs)
	{
		::ShowWindow(GetDlgItem(nID), isShow ? SW_SHOW : SW_HIDE);
	}
}

LRESULT CVideoPlayerDlg::OnAppCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	UINT cmd = GET_APPCOMMAND_LPARAM(lParam);
	if (APPCOMMAND_MEDIA_PLAY_PAUSE == cmd)
	{
		SendMessage(WM_COMMAND, MAKEWPARAM(IDC_COURSE_PLAY, BN_CLICKED), 0);
		m_bmpBtnPlay.Invalidate();
	}

	// Return TRUE from a WM_APPCOMMAND handler, even if you process the message.
	return TRUE;
}

LRESULT CVideoPlayerDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int iScrollCode = (int)LOWORD(wParam);
	HWND hWndScrollBar = (HWND)lParam;

	if (m_hWndMediaTime == hWndScrollBar)
	{
		int nPos = SendMessage(hWndScrollBar, TBM_GETPOS, 0, 0);

		switch (iScrollCode)
		{
		case SB_THUMBTRACK://用户拖动滚动框
		case SB_PAGEUP:
		case SB_PAGEDOWN:
			m_coursePlayer.SetTime(nPos * 1000);
			break;
		}
	}
	else if (m_hWndVolume == hWndScrollBar)
	{
		int nPos = SendMessage(hWndScrollBar, TBM_GETPOS, 0, 0);

		switch (iScrollCode)
		{
		case SB_THUMBTRACK:
		case SB_PAGEUP:
		case SB_PAGEDOWN:
			m_coursePlayer.SetVolume(nPos);
			if (nPos == 0)
				m_bmpBtnMute.SetImages(2, -1, 3);
			else
				m_bmpBtnMute.SetImages(0, -1, 1);
			m_bmpBtnMute.Invalidate();
			break;
		}
	}

	return 0;
}

LRESULT CVideoPlayerDlg::OnCourseListSelChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//避免对当前已经选中的课程重复响应
	if (IsCourseSelected())
		return TRUE;

	m_selectCourseIndex = m_courseList.GetCurSel();
	m_playingObject.SetCourse(m_config.GetCourses()[m_selectCourseIndex]);

	DeleteAllCourseChapters();
	AddCourseChapters();

	SetCurrentChapter(0);

	return 0;
}

bool CVideoPlayerDlg::IsCourseSelected() const
{
	return m_selectCourseIndex == m_courseList.GetCurSel();
}

LRESULT CVideoPlayerDlg::OnCourseChapterListSelChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int iItem = m_courseChapterList.GetCurSel();
	m_selectCourseChapterIndex = iItem;
	return 0;
}

LRESULT CVideoPlayerDlg::OnCourseChapterListDoubleClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Play();
	return 0;
}

LRESULT CVideoPlayerDlg::OnCoursePrevChapter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UIEnable(IDC_COURSE_PREV_CHAPTER, !m_bmpBtnCoursePrevChapter.IsWindowEnabled());
	PlayCoursePrevChapter();
	return 0;
}

LRESULT CVideoPlayerDlg::OnCoursePlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UIEnable(IDC_COURSE_PLAY, !m_bmpBtnPlay.IsWindowEnabled());

	if (m_coursePlayer.IsNotOpen())
	{
		Play();
	}
	else if (m_coursePlayer.IsPlaying())
	{
		m_coursePlayer.Pause();
		m_bmpBtnPlay.SetImages(2, -1, 3);
	}
	else if (m_coursePlayer.IsPaused())
	{
		m_coursePlayer.Play();
		m_bmpBtnPlay.SetImages(0, -1, 1);
	}

	return 0;
}

void CVideoPlayerDlg::Play()
{
	tstring selectedFilename = GetCurrentChapter().GetFilePath();
	if (FileUtil::IsExistWithFile(selectedFilename))
	{
		m_coursePlayer.OpenMedia(selectedFilename);
		m_coursePlayer.Play();

		Sleep(100);

		InitMediaTimeControl();
		m_bmpBtnPlay.SetImages(0, -1, 1);
		m_bmpBtnPlay.Invalidate();

		m_playingObject.SetCurrentCourseIndexWithListBox(m_selectCourseIndex);
		m_playingObject.SetCurrentChapterIndexWithListBox(m_selectCourseChapterIndex);
	}
	else
	{
		//更新画面
	}
}

void CVideoPlayerDlg::InitMediaTimeControl()
{
	SetSliderRange(m_hWndMediaTime, 0, m_coursePlayer.GetLength() / 1000);
	SetSliderPos(m_hWndMediaTime, 0);
}

LRESULT CVideoPlayerDlg::OnCourseNextChapter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UIEnable(IDC_COURSE_NEXT_CHAPTER, !m_bmpBtnCourseNextChapter.IsWindowEnabled());
	PlayCourseNextChapter();
	return 0;
}

LRESULT CVideoPlayerDlg::OnSnapshot(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_coursePlayer.Snapshot();
	return 0;
}

LRESULT CVideoPlayerDlg::OnMute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UIEnable(IDC_MUTE, !m_bmpBtnMute.IsWindowEnabled());

	int currentVolume = m_coursePlayer.GetVolume();
	if (currentVolume == -1)
		return 0;

	if (m_coursePlayer.GetMute())
	{
		m_coursePlayer.SetMute(false);
		SetSliderPos(m_hWndVolume, m_coursePlayer.GetVolume());
		m_bmpBtnMute.SetImages(0, -1, 1);
	}
	else
	{
		m_coursePlayer.SetMute(true);
		SetSliderPos(m_hWndVolume, 0);
		m_bmpBtnMute.SetImages(2, -1, 3);
	}

	return 0;
}

LRESULT CVideoPlayerDlg::OnFullScreen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UIEnable(IDC_FULL_SCREEN, !m_bmpBtnFullScreen.IsWindowEnabled());
	FullScreen(true);
	return 0;
}

LRESULT CVideoPlayerDlg::OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UIEnable(ID_APP_EXIT, !m_bmpBtnAppExit.IsWindowEnabled());
	this->GetParent().PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CVideoPlayerDlg::OnCoursePlayerTimeChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	UpdatePlayTime();

	if (m_iMediaTime >= m_coursePlayer.GetLength())
	{
		m_coursePlayer.Stop();
		m_iMediaTime = 0;

		PlayCourseNextChapter();
	}

	return TRUE;
}

void CVideoPlayerDlg::PlayCourseNextChapter()
{
	if (m_selectCourseChapterIndex < GetCurrentCourse().GetChapterCount()-1)
	{
		++m_selectCourseChapterIndex;
		SetCurrentChapter(m_selectCourseChapterIndex);
		Play();
	}
}

void CVideoPlayerDlg::PlayCoursePrevChapter()
{
	if (m_selectCourseChapterIndex > 0 && GetCurrentCourse().GetChapterCount())
	{
		--m_selectCourseChapterIndex;
		SetCurrentChapter(m_selectCourseChapterIndex);
		Play();
	}
}

LRESULT CVideoPlayerDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_coursePlayer.Stop();

	EndDialog(wID);
	return 0;
}

const Course& CVideoPlayerDlg::GetCurrentCourse() const
{
	return m_playingObject.GetCourse();
}

const Chapter& CVideoPlayerDlg::GetCurrentChapter() const
{
	return GetCurrentCourse().GetChapter(m_selectCourseChapterIndex);
}
