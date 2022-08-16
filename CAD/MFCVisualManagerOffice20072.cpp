#include "stdafx.h"
#include "MFCVisualManagerOffice20072.h"

////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMFCVisualManagerOffice20072, CMFCVisualManagerOffice2007);

CMFCVisualManagerOffice20072::CMFCVisualManagerOffice20072()
{
}

CMFCVisualManagerOffice20072::~CMFCVisualManagerOffice20072()
{
}

BOOL CMFCVisualManagerOffice20072::DrawTextOnGlass(CDC * pDC, CString strText, CRect rect, DWORD dwFlags, int nGlowSize, COLORREF clrText)
{
	return CMFCVisualManagerOffice2007::DrawTextOnGlass( pDC, strText, rect, dwFlags, nGlowSize, RGB(0,0,0));
}
////////////////////////////////////////////////////////////////