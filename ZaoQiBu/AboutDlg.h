// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());

		m_linkSourceCode.SubclassWindow(GetDlgItem(IDC_SOURCE_CODE_LINK));
		m_linkSourceCode.SetHyperLink(_T("https://github.com/vwarship/ZaoQiBu"));

		m_linkDownload360.SubclassWindow(GetDlgItem(IDC_DOWNLOAD_360_LINK));
		m_linkDownload360.SetHyperLink(_T("http://yunpan.cn/QiQntrDdypkmB"));

		m_linkDownloadBaidu.SubclassWindow(GetDlgItem(IDC_DOWNLOAD_BAIDU_LINK));
		m_linkDownloadBaidu.SetHyperLink(_T("http://pan.baidu.com/share/home?uk=2990918483"));

		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

private:
	CHyperLink m_linkSourceCode;
	CHyperLink m_linkDownload360;
	CHyperLink m_linkDownloadBaidu;

};
