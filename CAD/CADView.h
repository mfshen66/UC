// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// CADView.h : CCADView ��Ľӿ�
//

#pragma once

class CCADDoc ;
class CCommand ;
class CCADView : public CView
{
protected: // �������л�����
	CCADView();
	DECLARE_DYNCREATE(CCADView)

// ����
public:
	CCADDoc* GetDocument() const;
	int m_drawMode ;
	CCommand* m_pCmd ;
	void* m_pVI ;
	BOOL m_showGrid; // TRUE=show grid
	double m_gridSize;
	int m_drawSection ; // nt add 2017/3/21
	double m_z ; // nt add 2017/3/22
	int m_direct ; // nt add 2017/7/5 1=ֱ��ƽ�ƻ���ת
	CPoint m_mpos ; // nt add 2017/8/23 ����OnMouseMove�и���

// ����
public:
	int GetDrawMode() ;
	void* GetVI() ;
	void ClearCmd() ;
	void Escape() ;
	void DrawFrame();
	void SetViewDir(VEC3D viewDir, VEC3D up);
	void SetDrawSection(int drawSection) ; // nt add 2017/8/28
	double GetZ() ;
	void SetZ(double z) ; // nt add 2017/3/22
	void ViewUp() ; // nt add 2017/9/5

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CCADView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewPan();
	afx_msg void OnViewRotate();
	afx_msg void OnViewZoom();
	afx_msg void OnViewZoomBox();
	afx_msg void OnViewZoomOut();
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewFull();
	afx_msg void OnViewFullWin();
	afx_msg void OnViewUp();
	afx_msg void OnViewBottom();
	afx_msg void OnViewFront();
	afx_msg void OnViewRear();
	afx_msg void OnViewLeft();
	afx_msg void OnViewRight();
	afx_msg void OnView3D();
};

// nt add 2017/8/28
CCADView* cadGetView() ;

#ifndef _DEBUG  // CADView.cpp �еĵ��԰汾
inline CCADDoc* CCADView::GetDocument() const
   { return reinterpret_cast<CCADDoc*>(m_pDocument); }
#endif

