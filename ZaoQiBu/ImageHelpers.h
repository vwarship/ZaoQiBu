#if !defined(_WTL_IMAGEHELPERS_)
#define _WTL_IMAGEHELPERS_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")


ATLINLINE HBITMAP AtlLoadGdiplusImage(ATL::_U_STRINGorID bitmap, ATL::_U_STRINGorID type = (UINT) 0)
{
   USES_CONVERSION;
   static bool s_bInitied = false;
   if( !s_bInitied ) {
      s_bInitied = true;
      Gdiplus::GdiplusStartupInput gsi;
      Gdiplus::GdiplusStartupOutput gso;
      ULONG uToken = 0;
      Gdiplus::GdiplusStartup(&uToken, &gsi, &gso);
   }
   Gdiplus::Bitmap* pBitmap = NULL;
   if( HIWORD(bitmap.m_lpstr) != NULL ) {
      // Load from filename
      pBitmap = new Gdiplus::Bitmap(T2CW(bitmap.m_lpstr)); 
   }
   else if( type.m_lpstr != NULL && type.m_lpstr != RT_BITMAP ) {
      // Loading PNG, JPG resources etc
      WTL::CResource res;
      if( !res.Load(type, bitmap) ) return NULL;
      DWORD dwSize = res.GetSize();
      HANDLE hMemory = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
      if( hMemory == NULL ) return NULL;
      ::memcpy(::GlobalLock(hMemory), res.Lock(), dwSize);
      ::GlobalUnlock(hMemory);
      IStream* pStream = NULL; 
      if( FAILED( ::CreateStreamOnHGlobal(hMemory, TRUE, &pStream) ) ) {
         ::GlobalFree(hMemory);
         return FALSE;
      }
      pBitmap = new Gdiplus::Bitmap(pStream);
      pStream->Release();
   }
   else {
      // This only loads BMP resources
      pBitmap = new Gdiplus::Bitmap(_Module.GetResourceInstance(), (LPCWSTR) (UINT) bitmap.m_lpstr);
   }
   if( pBitmap == NULL ) return NULL;
   HBITMAP hBitmap = NULL;
   pBitmap->GetHBITMAP(NULL, &hBitmap); 
   delete pBitmap;
   return hBitmap;
}


ATLINLINE HBRUSH AtlGetBackgroundBrush(HWND hWnd, HWND hwndParent)
{
   CWindow wnd = hWnd;
   CWindow wndParent = hwndParent;
   CClientDC dcParent = wndParent;
   CRect rcParent;
   wndParent.GetWindowRect(&rcParent);
   CDC dcCompat1;
   dcCompat1.CreateCompatibleDC(dcParent);
   CBitmap bmpCompat1;
   bmpCompat1.CreateCompatibleBitmap(dcParent, rcParent.Width(), rcParent.Height());
   HBITMAP hOldBmp1 = dcCompat1.SelectBitmap(bmpCompat1);
   wndParent.SendMessage(WM_ERASEBKGND, (WPARAM) (HDC) dcCompat1, 0);
   wndParent.SendMessage(WM_PRINTCLIENT, (WPARAM) (HDC) dcCompat1, (LPARAM)(PRF_ERASEBKGND | PRF_CLIENT | PRF_NONCLIENT));
   CRect rcWin;
   wnd.GetWindowRect(&rcWin);
   CDC dcCompat2;
   dcCompat2.CreateCompatibleDC();
   CBitmap bmpCompat2;
   bmpCompat2.CreateCompatibleBitmap(dcCompat1, rcWin.Width(), rcWin.Height());
   HBITMAP hOldBmp2 = dcCompat2.SelectBitmap(bmpCompat2);
   CRect rcSnap = rcWin;
   ::MapWindowPoints(NULL, wndParent, (LPPOINT) (LPRECT) &rcSnap, 2);
   dcCompat2.BitBlt(0, 0, rcWin.Width(), rcWin.Height(), dcCompat1, rcSnap.left, rcSnap.top, SRCCOPY);
   HBRUSH hBrush = ::CreatePatternBrush(bmpCompat2);
   dcCompat1.SelectBitmap(hOldBmp1);
   dcCompat2.SelectBitmap(hOldBmp2);
   return hBrush;
}


ATLINLINE VOID AlphaBlendStretch(CDCHandle dcDest, CRect rcDest, CDCHandle dcSrc, CRect rcBmp, BYTE uFade, CRect rcCorners)
{
   ATLASSERT(rcBmp.Width()>rcCorners.left+rcCorners.right);
   ATLASSERT(rcBmp.Height()>rcCorners.top+rcCorners.bottom);
   BLENDFUNCTION bf = { AC_SRC_OVER, 0, uFade, AC_SRC_ALPHA };
   // left-top
   dcDest.AlphaBlend(rcDest.left, rcDest.top, rcCorners.left, rcCorners.top, dcSrc, rcBmp.left, rcBmp.top, rcCorners.left, rcCorners.top, bf);
   // top
   dcDest.AlphaBlend(rcDest.left + rcCorners.left, rcDest.top, rcDest.Width() - rcCorners.left - rcCorners.right, rcCorners.top, dcSrc, rcBmp.left + rcCorners.left, rcBmp.top, rcBmp.Width() - rcCorners.left - rcCorners.right, rcCorners.top, bf);
   // right-top
   dcDest.AlphaBlend(rcDest.right - rcCorners.right, rcDest.top, rcCorners.right, rcCorners.top, dcSrc, rcBmp.right - rcCorners.right, rcBmp.top, rcCorners.right, rcCorners.top, bf);
   // left
   dcDest.AlphaBlend(rcDest.left, rcDest.top + rcCorners.top, rcCorners.left, rcDest.Height() - rcCorners.top - rcCorners.bottom, dcSrc, rcBmp.left, rcBmp.top + rcCorners.top, rcCorners.left, rcBmp.Height() - rcCorners.top - rcCorners.bottom, bf);
   // middle
   dcDest.AlphaBlend(rcDest.left + rcCorners.left, rcDest.top + rcCorners.top, rcDest.Width() - rcCorners.left - rcCorners.right, rcDest.Height() - rcCorners.top - rcCorners.bottom, dcSrc, rcBmp.left + rcCorners.left, rcBmp.top + rcCorners.top, rcBmp.Width() - rcCorners.left - rcCorners.right, rcBmp.Height() - rcCorners.top - rcCorners.bottom, bf);
   // right
   dcDest.AlphaBlend(rcDest.right - rcCorners.right, rcDest.top + rcCorners.top, rcCorners.right, rcDest.Height() - rcCorners.top - rcCorners.bottom, dcSrc, rcBmp.right - rcCorners.right, rcBmp.top + rcCorners.top, rcCorners.right, rcBmp.Height() - rcCorners.top - rcCorners.bottom, bf);
   // left-bottom
   dcDest.AlphaBlend(rcDest.left, rcDest.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom, dcSrc, rcBmp.left, rcBmp.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom, bf);
   // bottom
   dcDest.AlphaBlend(rcDest.left + rcCorners.left, rcDest.bottom - rcCorners.bottom, rcDest.Width() - rcCorners.left - rcCorners.right, rcCorners.bottom, dcSrc, rcBmp.left + rcCorners.left, rcBmp.bottom - rcCorners.bottom, rcBmp.Width() - rcCorners.left - rcCorners.right, rcCorners.bottom, bf);
   // right-bottom
   dcDest.AlphaBlend(rcDest.right - rcCorners.right, rcDest.bottom - rcCorners.bottom, rcCorners.right, rcCorners.bottom, dcSrc, rcBmp.right - rcCorners.right, rcBmp.bottom - rcCorners.bottom, rcCorners.right, rcCorners.bottom, bf);
}

ATLINLINE HBITMAP CreateHoverBitmapWithPNG(HDC hDC, int nID)
{
	CBitmap srcBitmap = AtlLoadGdiplusImage(nID, _T("PNG"));

	SIZE bitmapSize = { 0 };
	srcBitmap.GetSize(bitmapSize);

	CDC dc;
	dc.CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = dc.SelectBitmap(srcBitmap);
	dc.BitBlt(0, 0, bitmapSize.cx, bitmapSize.cy, dc, 0, 0, PATINVERT);
	dc.SelectBitmap(hOldBitmap);

	return srcBitmap.Detach();
}

ATLINLINE CRect CalcDestImageRect(SIZE srcSize, SIZE destSize)
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

ATLINLINE HBITMAP CreateThumbnail(HDC hDC, const CBitmap &bitmap, SIZE thumbnailSize)
{
	SIZE srcSize = { 0 };
	bitmap.GetSize(srcSize);

	CDC srcDC;
	srcDC.CreateCompatibleDC(hDC);
	HBITMAP hOldSrcBitmap = srcDC.SelectBitmap(bitmap);

	CDC destDC;
	destDC.CreateCompatibleDC(hDC);

	CBitmap destBitmap;
	destBitmap.CreateCompatibleBitmap(hDC, thumbnailSize.cx, thumbnailSize.cy);
	HBITMAP hOldDestBitmap = destDC.SelectBitmap(destBitmap);
	destDC.SetStretchBltMode(HALFTONE);
	CRect destCenterRect = CalcDestImageRect(srcSize, thumbnailSize);
	destDC.StretchBlt(destCenterRect.left, destCenterRect.top, destCenterRect.Width(), destCenterRect.Height(),
		srcDC, 0, 0, srcSize.cx, srcSize.cy, SRCCOPY);
	destDC.SelectBitmap(hOldDestBitmap);

	srcDC.SelectBitmap(hOldSrcBitmap);

	return destBitmap.Detach();
}
#endif // !defined(_WTL_IMAGEHELPERS_)

