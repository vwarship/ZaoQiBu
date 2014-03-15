#pragma once

#include <vector>
#include <atltime.h>
#include <atlddx.h>
#include <vlc/vlc.h>
#include "Config.h"
#include "Course.h"
#include "VLCWrapper.h"
#include "ImageListBox.h"
#include "CoursePlayer.h"
#include "FileUtil.h"
#include "Playlist.h"

using std::vector;
using std::string;


#define UM_COURSE_PLAYER_TIME_CHANGED WM_USER+1

class CVideoPlayerDlg : 
	public CDialogImpl<CVideoPlayerDlg>,
	public CDialogResize<CVideoPlayerDlg>,
	public CUpdateUI<CVideoPlayerDlg>,
	public CWinDataExchange<CVideoPlayerDlg>,
	public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_VIDEOPLAYER };

	CVideoPlayerDlg();
	~CVideoPlayerDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_MSG_MAP(CVideoPlayerDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_APPCOMMAND, OnAppCommand)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtrlColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtrlColor)
		COMMAND_HANDLER(IDC_COURSE_LIST, LBN_SELCHANGE, OnCourseListSelChanged)
		COMMAND_HANDLER(IDC_COURSE_ITEM_LIST, LBN_SELCHANGE, OnCourseChapterListSelChanged)
		COMMAND_HANDLER(IDC_COURSE_ITEM_LIST, LBN_DBLCLK, OnCourseChapterListDoubleClicked)
		COMMAND_ID_HANDLER(IDC_COURSE_PREV_CHAPTER, OnCoursePrevChapter)
		COMMAND_ID_HANDLER(IDC_COURSE_PLAY, OnCoursePlay)
		COMMAND_ID_HANDLER(IDC_COURSE_NEXT_CHAPTER, OnCourseNextChapter)
		COMMAND_ID_HANDLER(IDC_SNAPSHOT, OnSnapshot)
		COMMAND_ID_HANDLER(IDC_MUTE, OnMute)
		COMMAND_ID_HANDLER(IDC_FULL_SCREEN, OnFullScreen)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAbout)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnExit)
		COMMAND_ID_HANDLER(IDC_ADD_COURSE, OnAddCourse)
		COMMAND_ID_HANDLER(IDC_DELETE_COURSE, OnDeleteCourse)
		//MESSAGE_HANDLER(WM_CHAR, OnChar)
		//MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		//MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDBlclk)
		//COMMAND_HANDLER(IDC_COURSE_PLAYER, STN_DBLCLK, OnCoursePlayerDBLCLK)

		//自定义消息
		MESSAGE_HANDLER(UM_COURSE_PLAYER_TIME_CHANGED, OnCoursePlayerTimeChanged)
		CHAIN_MSG_MAP(CDialogResize<CVideoPlayerDlg>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CVideoPlayerDlg)
		DLGRESIZE_CONTROL(IDC_COURSE_LIST, DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_COURSE_ITEM_LIST, DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_COURSE_PLAYER, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_COURSE_PLAYER, DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(ID_APP_ABOUT, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(ID_APP_EXIT, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_MEDIA_CURRENT_TIME, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_MEDIA_TIME, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_MEDIA_TIME, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_MEDIA_LENGTH, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_MEDIA_LENGTH, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_COURSE_PREV_CHAPTER, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_COURSE_PLAY, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_COURSE_NEXT_CHAPTER, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_SNAPSHOT, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_MUTE, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_MUTE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_VOLUME, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_VOLUME, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_FULL_SCREEN, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_FULL_SCREEN, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_ADD_COURSE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_DELETE_COURSE, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_SOURCE_CODE_LINK, DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_UPDATE_UI_MAP(CVideoPlayerDlg)
		UPDATE_ELEMENT(IDC_COURSE_PREV_CHAPTER, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(IDC_COURSE_PLAY, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(IDC_COURSE_NEXT_CHAPTER, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(IDC_SNAPSHOT, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(IDC_MUTE, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(IDC_FULL_SCREEN, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(ID_APP_ABOUT, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(ID_APP_EXIT, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(IDC_ADD_COURSE, UPDUI_CHILDWINDOW)
		UPDATE_ELEMENT(IDC_DELETE_COURSE, UPDUI_CHILDWINDOW)
	END_UPDATE_UI_MAP()

	BEGIN_DDX_MAP(CVideoPlayerDlg)
		DDX_TEXT(IDC_MEDIA_CURRENT_TIME, m_sMediaCurrentTime)
		DDX_TEXT(IDC_MEDIA_LENGTH, m_sMediaLength)
	END_DDX_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	
	LRESULT OnAppCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCtrlColor(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnCourseListSelChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCourseChapterListSelChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCourseChapterListDoubleClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAddCourse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDeleteCourse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCoursePrevChapter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCoursePlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCourseNextChapter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSnapshot(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFullScreen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCoursePlayerTimeChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	//LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)	{return TRUE;}
	//LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) { if (VK_ESCAPE == wParam){} return TRUE; }
	//LRESULT OnLButtonDBlclk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) { return TRUE; }
	//LRESULT OnCoursePlayerDBLCLK(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) { return TRUE; }

	void PlayCoursePrevChapter();
	void PlayCourseNextChapter();
	void UpdatePlayTime(int64_t currentPlayTime);

private:
	void LoadConfig();

	void InsertCourses();
	void InsertCourse(const shared_ptr<Course> course, int iImage);
	void AddCourseImage(LPCTSTR pszImageFilename);
	HBITMAP LoadBitmapWithPNG(int nID);
	void DeleteAllCourseChapters();
	void AddCourseChapters(const shared_ptr<Course> course);

	void SetCurrentCourseIndex(int courseIndex);
	void SetCurrentChapterIndex(int courseIndex);
	void SelectListBox(CImageListBoxCtrl &listbox, int index);
	int GetCurrentCourseIndex() const;
	int GetCurrentChapterIndex() const;

	bool IsCourseSelected() const;

	void CreateBitmapButton(int nButtonID, const std::vector<int> &imageIDs, PCTSTR toolTipText, CBitmapButton &bitmapButton, int size = 48);

	void ShowChildWindows(bool isShow);

	void Play();
	void FullScreen(bool isFullScreen);
	void SetVolume();

	void InitMediaTimeControl();
	void InitVolume();

	void SetMediaTimePos(int64_t curPos);
	void SetSliderPos(HWND hWnd, LPARAM pos);
	void SetSliderRange(HWND hWnd, DWORD start, DWORD end);

private:
	HBRUSH m_hBrushBackgroundColor = NULL;
	CBitmapButton m_bmpBtnTitle;

	CImageListBoxCtrl m_courseList;
	CImageList m_courseImageList;
	//int m_selectCourseIndex;
	CBitmapButton m_bmpBtnAddCourse;
	CBitmapButton m_bmpBtnDeleteCourse;


	CImageListBoxCtrl m_courseChapterList;
	//int m_selectCourseChapterIndex;

	CoursePlayer m_coursePlayer;

	CHyperLink m_linkSourceCode;
	CBitmapButton m_bmpBtnHelp;
	CBitmapButton m_bmpBtnAppExit;
	CBitmapButton m_bmpBtnCoursePrevChapter;
	CBitmapButton m_bmpBtnPlay;
	CBitmapButton m_bmpBtnCourseNextChapter;
	CBitmapButton m_bmpBtnFullScreen;
	CBitmapButton m_bmpBtnSnapshot;
	CBitmapButton m_bmpBtnMute;

	bool m_isFullScreen;

	WTL::CString m_sMediaCurrentTime;
	WTL::CString m_sMediaLength;

	HWND m_hWndMediaTime;
	static const int64_t MEDIA_TIME_DEFAULT_VALUE = 0;

	HWND m_hWndVolume;
	static const int VOLUME_DEFAULT_VALUE = 100;

	Config m_config;
	Playlist m_playlist;

};

