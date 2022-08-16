#pragma once

class CMFCVisualManagerOffice20072 : public CMFCVisualManagerOffice2007
{
	DECLARE_DYNCREATE(CMFCVisualManagerOffice20072);

public:
	CMFCVisualManagerOffice20072();
	virtual ~CMFCVisualManagerOffice20072();
	BOOL DrawTextOnGlass(CDC* pDC,
						 CString strText,
						 CRect rect,
						 DWORD dwFlags,
						 int nGlowSize = 0,
						 COLORREF clrText = (COLORREF)-1) override;
};
