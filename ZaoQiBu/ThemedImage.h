
#pragma once


//////////////////////////////////////////////////////////////////////
// Themed Image control
//

class CThemedImageCtrl : public CWindowImpl<CThemedImageCtrl, CStatic>
{
public:
   CWindow m_wndThemeParent;
   CBrush m_brBack;
   CBitmap m_bmpImage;

   // Operations

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd==NULL);
      ATLASSERT(::IsWindow(hWnd));
      BOOL bRet = CWindowImpl<CThemedImageCtrl, CStatic>::SubclassWindow(hWnd);
      if( bRet ) Init();
      return bRet;
   }

   void Init()
   {
      m_wndThemeParent = GetTopLevelParent();
      Invalidate();
   }

   void SetThemeParent(HWND hWnd)
   {
      ATLASSERT(::IsWindow(hWnd));
      m_wndThemeParent = hWnd;
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
   }

   void SetBitmap(HBITMAP hBitmap)
   {
      m_bmpImage = (HBITMAP) ::CopyImage(hBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
      if( IsWindow() ) Invalidate();
   }

   void SetBitmap(UINT nRes, LPCTSTR pstrType)
   {
      m_bmpImage = AtlLoadGdiplusImage(nRes, pstrType);
      if( IsWindow() ) Invalidate();
   }

   void SizeToFit()
   {
      ATLASSERT(!m_bmpImage.IsNull());
      BITMAP BmpInfo = { 0 };
      m_bmpImage.GetBitmap(&BmpInfo);
      ResizeClient(BmpInfo.bmWidth, BmpInfo.bmHeight);
      if( IsWindow() ) Invalidate();
   }
 
   // Message map and handlers

   BEGIN_MSG_MAP(CThemedImageCtrl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_MOVE, OnSize)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      Init();
      bHandled = FALSE;
      return 0;
   }

   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
      return TRUE;
   }

   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      return 1;
   }

   LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if( wParam != NULL )
      {
         DoPaint((HDC) wParam);
      }
      else
      {
         CPaintDC dc(m_hWnd);
         DoPaint(dc.m_hDC);
      }
      return 0;
   }

   // Implementation

   void DoPaint(CDCHandle dc)
   {
      CRect rcClient;
      GetClientRect(&rcClient);

      // Preserve background
      if( m_brBack.IsNull() ) m_brBack = AtlGetBackgroundBrush(m_hWnd, m_wndThemeParent);
      HBRUSH hOldBrush = dc.SelectBrush(m_brBack);
      dc.PatBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), PATCOPY);
      dc.SelectBrush(hOldBrush);

      // Paint image (assume that it is a 32bit with alpha-pr-pixel)...
      CDC dcCompat;
      dcCompat.CreateCompatibleDC(dc);
      BITMAP BmpInfo = { 0 };
      m_bmpImage.GetBitmap(&BmpInfo);
      BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
      HBITMAP hOldBitmap = dcCompat.SelectBitmap(m_bmpImage);
      dc.AlphaBlend(0, 0, rcClient.Width(), rcClient.Height(), dcCompat, 0, 0, BmpInfo.bmWidth, BmpInfo.bmHeight, bf);
      dcCompat.SelectBitmap(hOldBitmap);
  }
};

