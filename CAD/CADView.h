// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// CADView.h : CCADView 类的接口
//

#pragma once

class CCADDoc ;
class CCommand ;
class CCADView : public CView
{
protected: // 仅从序列化创建
	CCADView();
	DECLARE_DYNCREATE(CCADView)

// 特性
public:
	CCADDoc* GetDocument() const;
	int m_drawMode ;
	CCommand* m_pCmd ;
	void* m_pVI ;
	BOOL m_showGrid; // TRUE=show grid
	double m_gridSize;
	int m_drawSection ; // nt add 2017/3/21
	double m_z ; // nt add 2017/3/22
	int m_direct ; // nt add 2017/7/5 1=直接平移或旋转
	CPoint m_mpos ; // nt add 2017/8/23 仅在OnMouseMove中更新

// 操作
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

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCADView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // CADView.cpp 中的调试版本
inline CCADDoc* CCADView::GetDocument() const
   { return reinterpret_cast<CCADDoc*>(m_pDocument); }
#endif

