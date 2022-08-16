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

// CADView.cpp : CCADView 类的实现
//

#include "stdafx.h"
#include "CAD.h"
#include "CADDoc.h"
#include "CADView.h"
#include "MainFrm.h"
#include "Command.h"
#include "ViewCmd.h"
#include "DocCmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////
// CCADView

IMPLEMENT_DYNCREATE(CCADView, CView)

BEGIN_MESSAGE_MAP(CCADView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCADView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_VIEW_PAN, OnViewPan)
	ON_COMMAND(ID_VIEW_ROTATE, OnViewRotate)
	ON_COMMAND(ID_VIEW_ZOOM, OnViewZoom)
	ON_COMMAND(ID_VIEW_ZOOMBOX, OnViewZoomBox)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomOut)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomIn)
	ON_COMMAND(ID_VIEW_FULL, OnViewFull)
	ON_COMMAND(ID_VIEW_FULLWIN, OnViewFullWin)
	ON_COMMAND(ID_VIEW_UP, OnViewUp)
	ON_COMMAND(ID_VIEW_BOTTOM, OnViewBottom)
	ON_COMMAND(ID_VIEW_FRONT, OnViewFront)
	ON_COMMAND(ID_VIEW_REAR, OnViewRear)
	ON_COMMAND(ID_VIEW_LEFT, OnViewLeft)
	ON_COMMAND(ID_VIEW_RIGHT, OnViewRight)
	ON_COMMAND(ID_VIEW_3D, OnView3D)
	//ON_COMMAND(ID_VIEW_SECTION, OnViewSection)
END_MESSAGE_MAP()
//--------------------------------------------------------------
// CCADView 构造/析构

CCADView::CCADView()
{
	// TODO: 在此处添加构造代码
	m_drawMode = 0 ;
	m_pCmd = NULL ;
	m_pVI = NULL ;
	m_showGrid = TRUE;
	m_gridSize = 4. ; // nt modify 2017/5/11 old 20.
	m_drawSection = 0 ; // nt add 2017/3/21
	m_z = 0. ; // nt add 2017/3/22
	m_direct = 0 ; // nt add 2017/7/5
	m_mpos.x = 0 ; // nt add 2017/8/23
	m_mpos.y = 0 ; // nt add 2017/8/23
}

CCADView::~CCADView()
{
	if( m_pCmd )
		delete m_pCmd ;
}
//--------------------------------------------------------------
// CCADView 绘制
// draw grid, work space
void CCADView::DrawFrame()
{
	// draw work space
	double x, y;
	PNT3D min, max;
	VEC3D Z = { 0., 0., 1. };
	//MATERIAL material ;

	CCADDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	pDoc->GetSpace(min, max);

	//viLighting(m_pVI, 1) ;
	//viAddDefaultLight(m_pVI) ;
	//materialGet(&material, "White2") ;
	//materialSet(&material) ;
	glColor3ub(255, 255, 255);
	glBegin(GL_QUADS); // draw bottom of work space
	glNormal3dv(Z);
	glVertex3d(min[0], min[1], min[2]);
	glNormal3dv(Z);
	glVertex3d(max[0], min[1], min[2]);
	glNormal3dv(Z);
	glVertex3d(max[0], max[1], min[2]);
	glNormal3dv(Z);
	glVertex3d(min[0], max[1], min[2]);
	glEnd();
	//viLighting(m_pVI, 0) ;

	// nt add 2017/5/11 绘制标记
	double d = 0.5*m_gridSize ;
	glBegin(GL_TRIANGLES);
	glNormal3dv(Z);
	glVertex3d(-d, min[1], min[2]);
	glNormal3dv(Z);
	glVertex3d(d, min[1], min[2]);
	glNormal3dv(Z);
	glVertex3d(0., min[1]-d, min[2]);
	glEnd();

	// nt add 2017/8/29 绘制标记
	glBegin(GL_TRIANGLES);
	glNormal3dv(Z);
	glVertex3d(max[0], -d, min[2]);
	glNormal3dv(Z);
	glVertex3d(max[0], d, min[2]);
	glNormal3dv(Z);
	glVertex3d(max[0]+d, 0., min[2]);
	glEnd();

	glLineWidth(1.f) ;
	glColor3ub(190, 190, 190);

	drawCube2(min, max); // draw work space frame

	if (m_showGrid) // draw grid
	{
		glLineWidth(1.f);
		glColor3ub(200, 200, 200);
		glBegin(GL_LINE_LOOP);
		glVertex3d(min[0], min[1], min[2]);
		glVertex3d(max[0], min[1], min[2]);
		glVertex3d(max[0], max[1], min[2]);
		glVertex3d(min[0], max[1], min[2]);
		glEnd();
		glBegin(GL_LINES);
		x = min[0] + m_gridSize;
		while (x<max[0])
		{
			glVertex3d(x, min[1], min[2]);
			glVertex3d(x, max[1], min[2]);
			x += m_gridSize;
		}
		y = min[1] + m_gridSize;
		while (y<max[1])
		{
			glVertex3d(min[0], y, min[2]);
			glVertex3d(max[0], y, min[2]);
			y += m_gridSize;
		}
		glEnd();
	}

	// draw coordinate frame
	viDrawWorldFrame(m_pVI, RGB(255, 0, 0));

	return;
}

// nt modify 2017/5/27
void CCADView::OnDraw(CDC* pDC)
{
	CCADDoc* pDoc = GetDocument();
	ZB* zb = NULL ;
	ZB2* zb2 = NULL ; // nt add 2017/5/29
	STL* stl = NULL ;
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	glDrawBuffer(GL_BACK) ;
	viSetGL(m_pVI) ;
	viClearGrey(m_pVI) ;
	DrawFrame();
	
	// draw model
	if( m_drawSection ) // nt add 2017/3/22
	{
		double cp[4] = {0., 0., -1., m_z} ;
		glClipPlane(GL_CLIP_PLANE0, cp) ;
		glEnable(GL_CLIP_PLANE0);
	}
	pDoc->Draw(0, m_pVI) ;
	if( m_drawSection ) // nt add 2017/3/22
	{
		glDisable(GL_CLIP_PLANE0);
		glColor3ub(200, 200, 200) ;
		// nt modify 2017/5/27
		for( stl = pDoc->m_stls ; stl ; stl = stl->next )
		{
			zb = (ZB*)(stl->zb) ;
			if( zb )
				zbDrawZ(zb, m_pVI) ;
			zb2 = (ZB2*)(stl->zb2) ; // nt add 2017/5/29
			if( zb2 ) // nt add 2017/5/29
				zb2DrawZ(zb2, m_pVI) ; // nt add 2017/5/29
			// nt add 2021/6/6
			zb2 = (ZB2*)(stl->zb3) ;
			if( zb2 )
				zb2DrawZ(zb2, m_pVI) ;
			// end
		}
	}

	// end draw
	glFlush() ;
	SwapBuffers(wglGetCurrentDC()) ;
	glDrawBuffer(GL_FRONT) ;

	return ;
}
//--------------------------------------------------------------
// CCADView 打印
void CCADView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCADView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCADView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCADView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CCADView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
}
//--------------------------------------------------------------
// CCADView 诊断
#ifdef _DEBUG
void CCADView::AssertValid() const
{
	CView::AssertValid();
}

void CCADView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCADDoc* CCADView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCADDoc)));
	return (CCADDoc*)m_pDocument;
}
#endif //_DEBUG
//--------------------------------------------------------------
BOOL CCADView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CS_OWNDC);

	return CView::PreCreateWindow(cs);
}

// CCADView 消息处理程序
int CCADView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	RECT rect ;
	GetClientRect(&rect) ;
	m_pVI = viCreate() ;
	viSetWnd(m_pVI, this) ;
	viSetInitZ(m_pVI, 2000.) ;
	viSetPixelSize(m_pVI, 0.35) ;
	int w = rect.right-rect.left,
		h = rect.bottom-rect.top ;
	viSetSize(m_pVI, w, h) ;
	
	//viSetViewport(m_pVI, vpXY) ;
	double d = sqrt(6.);
	VEC3D v, up;
	v[0] = 1 / d;
	v[1] = -2 / d;
	v[2] = 1 / d;
	up[0] = 0.;
	up[1] = 0.;
	up[2] = 1.;
	SetViewDir(v, up);

	SetTimer(1, 15, NULL) ;
	viWake(m_pVI) ;

	return 0;
}

void CCADView::OnDestroy() 
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if( m_pVI )
	{
		viSleep(m_pVI) ;
		viFree(m_pVI) ;
		m_pVI = NULL ;
	}

	KillTimer(1) ;
}

void CCADView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( nType == SIZE_MINIMIZED )
		return ;
	viOnSize(m_pVI, cx, cy) ;
}

BOOL CCADView::OnQueryNewPalette()
{
	BOOL nRet ;

	if( m_pVI )
	{
		viOnQueryNewPalette(m_pVI, &nRet) ;
		InvalidateRect(NULL, FALSE);
		return nRet;
	}

	return CView::OnQueryNewPalette();
}

void CCADView::OnPaletteChanged(CWnd* pFocusWnd)
{
	if( pFocusWnd != this )
	{
		viOnPaletteChanged(m_pVI) ;
	}

	CView::OnPaletteChanged(pFocusWnd);
}
//--------------------------------------------------------------
//	attributes
int CCADView::GetDrawMode()
{
	return m_drawMode ;
}

void* CCADView::GetVI()
{
	return m_pVI ;
}

void CCADView::ClearCmd()
{
	if( m_pCmd )
	{
		m_pCmd->Cancel() ;
		delete m_pCmd ;
		m_pCmd = NULL ;
		m_direct = 0 ; // nt add 2017/7/5
	}

	return ;
}

void CCADView::Escape()
{
	if( m_pCmd )
		ClearCmd() ; // clear view cmd only!!!!
	else
	{
		CCADDoc* pDoc = GetDocument() ;
		if( pDoc )
			pDoc->ClearCmd() ; // clear doc cmd
	}

	return ;
}

// nt add 2017/8/28
void CCADView::SetDrawSection(int drawSection)
{
	if( drawSection )
		m_drawSection = 1 ;
	else
		m_drawSection = 0 ;

	return ;
}

// nt add 2017/3/22
double CCADView::GetZ()
{
	return m_z ;
}

// nt add 2017/3/22
void CCADView::SetZ(double z)
{
	m_z = z ;
}

void CCADView::ViewUp()
{
	if (m_pVI)
	{
		VEC3D v, up;
		v[0] = 0. ;
		v[1] = 0. ;
		v[2] = 1. ;
		up[0] = 0.;
		up[1] = 1.;
		up[2] = 0.;
		CCADDoc* pDoc = GetDocument();
		if (pDoc == NULL)
			return;
		double d = 2000. ;
		PNT3D ori, eye;
		pDoc->GetCenter(ori);
		eye[0] = ori[0]+v[0]*d ;
		eye[1] = ori[1]+v[1]*d ;
		eye[2] = ori[2]+v[2]*d ;
		ori[0] = ori[0]-v[0]*d ;
		ori[1] = ori[1]-v[1]*d ;
		ori[2] = ori[2]-v[2]*d ;
		viSetEyeCenterUp(m_pVI, eye, ori, up);
		viZoom(m_pVI, 1.5) ;
		Invalidate(FALSE);
	}

	return;
}
//--------------------------------------------------------------
//	Mouse
void CCADView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_pVI )
	{
		double p[3] ;
		double v[3] ;
		viGetWorldPointViewD(m_pVI, point.x, point.y, p, v) ;
		if( m_pCmd )
			m_pCmd->OnLButtonDown(nFlags, p, v) ;
		else
		{
			CCADDoc* pDoc = GetDocument() ;
			if( pDoc )
				pDoc->OnLButtonDown(nFlags, p, v, m_pVI) ;
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CCADView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_pVI )
	{
		double p[3] ;
		double v[3] ;
		viGetWorldPointViewD(m_pVI, point.x, point.y, p, v) ;
		if( m_pCmd )
			m_pCmd->OnLButtonUp(nFlags, p, v) ;
		else
		{
			CCADDoc* pDoc = GetDocument() ;
			if( pDoc )
				pDoc->OnLButtonUp(nFlags, p, v, m_pVI) ;
		}
		if( m_pCmd &&
			m_direct )
			Escape() ;
	}

	CView::OnLButtonUp(nFlags, point);
}

void CCADView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_mpos = point ; // nt add 2017/8/23
	if( m_pVI )
	{
		double p[3] ;
		double v[3] ;
		viGetWorldPointViewD(m_pVI, point.x, point.y, p, v) ;
		cadPromptPt(p) ;
		if( m_pCmd )
			m_pCmd->OnMouseMove(nFlags, p, v) ;
		else
		{
			CCADDoc* pDoc = GetDocument() ;
			if( pDoc )
				pDoc->OnMouseMove(nFlags, p, v, m_pVI) ;
			// nt add 2017/7/3
			//if( pDoc->HasCmd() == 0 )
			{
				if( nFlags&MK_LBUTTON )
				{
					//OnViewRotate() ;
					//OnLButtonDown(nFlags, point) ;
					//m_direct = 1 ; // nt add 2017/7/5
				}
				else
				if( nFlags&MK_RBUTTON )
				{
					//OnViewPan() ;
					//OnLButtonDown(nFlags, point) ;
					//m_direct = 1 ; // nt add 2017/7/5
					OnViewRotate() ;
					OnLButtonDown(nFlags, point) ;
					m_direct = 1 ; // nt add 2017/7/5
				}
			}
			// end
		}
	}

	CView::OnMouseMove(nFlags, point);
}

void CCADView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_pVI )
	{
		double p[3] ;
		double v[3] ;
		viGetWorldPointViewD(m_pVI, point.x, point.y, p, v) ;
		CCADDoc* pDoc = GetDocument() ;
		if( pDoc )
			pDoc->OnRButtonDown(nFlags, p, v, m_pVI) ;
	}

	CView::OnRButtonDown(nFlags, point);
}

void CCADView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_pVI )
	{
		double p[3] ;
		double v[3] ;
		viGetWorldPointViewD(m_pVI, point.x, point.y, p, v) ;
		CCADDoc* pDoc = GetDocument() ;
		if( pDoc )
			pDoc->OnRButtonUp(nFlags, p, v, m_pVI) ;
		Escape() ;
	}

	CView::OnRButtonUp(nFlags, point);
}

void CCADView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMButtonDown(nFlags, point);
}

void CCADView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMButtonUp(nFlags, point);
}

BOOL CCADView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_pVI )
	{
		// nt add 2017/3/22
		/*if( m_drawSection )
		{
			CCADDoc* pDoc = GetDocument() ;
			if( pDoc )
			{
				double d = 1. ; // nt add 2017/5/15
				m_z += (zDelta>0?d:-d) ; // old 1.:-1.
				pDoc->CutAll(m_z) ; // nt modify 2017/5/27
				pDoc->Update() ; // nt add 2017/5/15
			}
		}
		else // nt modify 2017/8/23 keep fixed point*/
		//{
			PNT3D p, center ;
			VEC3D v ;
			viGetEyeCenterUp(m_pVI, p, center, v) ;
			viGetWorldPointViewD(m_pVI, m_mpos.x, m_mpos.y, p, v) ;
			v[0] = p[0]-center[0] ;
			v[1] = p[1]-center[1] ;
			v[2] = p[2]-center[2] ;
			if( zDelta < 0 )
			{
				viZoom(m_pVI, 1/0.9) ;
				v[0] *= (1/0.9-1.) ;
				v[1] *= (1/0.9-1.) ;
				v[2] *= (1/0.9-1.) ;
			}
			else
			{
				viZoom(m_pVI, 0.9) ;
				v[0] *= (-0.1) ;
				v[1] *= (-0.1) ;
				v[2] *= (-0.1) ;
			}
			viPan(m_pVI, v) ;
		//}
		Invalidate(FALSE) ;
	}
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
//--------------------------------------------------------------
void CCADView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	char c = '\0' ; // nt add 2009/10/21
	CString str ; // nt add 2009/10/21
	int ctrl = 0, shift = 0, /*capital = 0,*/ n ;
	VEC3D v ; // nt add 2009/10/21
	CCADDoc* pDoc = GetDocument() ;
	CCommand* pCmd = NULL ; // nt add 2017/6/8

	// nt add 2009/10/21
	if( GetKeyState(VK_LSHIFT) < 0 ||
		GetKeyState(VK_RSHIFT) < 0 )
		shift = 1 ;
	if( shift &&
		pDoc->HasCmd() == 0 )
	{
		OnViewPan() ;
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
		return ;
	}
	//if( GetKeyState(VK_CAPITAL) > 0 )
	//	capital = 1 ;

	// nt add 2009/10/21
	if( GetKeyState(VK_CONTROL) < 0 )
		ctrl = 1 ; // pressed
	if( ctrl &&
		pDoc->HasCmd() == 0 )
	{
		OnViewRotate() ;
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
		return ;
	}

	v[0] = 0. ; // nt add 2009/10/21
	v[1] = 0. ;
	v[2] = 0. ;
	switch(nChar)
	{
	case VK_ESCAPE:
		Escape() ;
		break ;
	case VK_DELETE:
		SendMessage(WM_COMMAND, ID_EDIT_DELETE) ;
		pCmd = pDoc->m_pCmd ;
		if( pCmd &&
			pCmd->GetID() == dctEditRod )
			((CEditRod*)pCmd)->Del() ;
		break ;
	case VK_SPACE: // nt add 2009/11/12
	case VK_RETURN: // nt add 2002/12/7
		pDoc->OnReturn() ; // nt add 2009/10/21
		break ;
	case VK_F2:
		SendMessage(WM_COMMAND, ID_FILE_SAVE) ; // nt add 2010/8/11
		break ;
	case VK_F3:
		//SendMessage(WM_COMMAND, ID_VIEW_PAN) ;
		OnViewPan() ;
		break ;
	case VK_F4:
		//SendMessage(WM_COMMAND, ID_VIEW_ROTATE) ;
		OnViewRotate() ;
		break ;
	case VK_F5:
		//SendMessage(WM_COMMAND, ID_VIEW_ZOOM) ;
		OnViewZoom() ;
		break ;
	case VK_UP: // nt add 2009/10/21
		v[0] = 0. ;
		v[1] = 5.*viGetScale(m_pVI) ;
		viPan(m_pVI, v) ;
		Invalidate(FALSE) ;
		break ;
	case VK_DOWN: // nt add 2009/10/21
		v[0] = 0. ;
		v[1] = -5.*viGetScale(m_pVI) ;
		viPan(m_pVI, v) ;
		Invalidate(FALSE) ;
		break ;
	case VK_RIGHT: // nt add 2009/10/21
		v[0] = 5.*viGetScale(m_pVI) ;
		v[1] = 0. ;
		viPan(m_pVI, v) ;
		Invalidate(FALSE) ;
		break ;
	case VK_LEFT: // nt add 2009/10/21
		v[0] = -5.*viGetScale(m_pVI) ;
		v[1] = 0. ;
		viPan(m_pVI, v) ;
		Invalidate(FALSE) ;
		break ;
	case VK_BACK: // nt add 2009/10/21
		cadPromptGet(str) ;
		n = str.GetLength() ;
		if( n > 0 )
			str = str.Left(n-1) ;
		cadPromptStr(str) ;
		break ;
	case 187:
		if( shift )
			c = '+' ;
		else
			c = '=' ;
		break ;
	case 189:
		if( shift )
			c = '_' ;
		else
			c = '-' ;
		break ;
	case 191: //case VK_DIVIDE:
		if( shift )
			c = '?' ;
		else
			c = '/' ;
		break ;
	case 190://case VK_DECIMAL: // nt add 2009/10/21
		if( shift )
			c = '>' ;
		else
			c = '.' ;
		break ;
	case 188://case VK_SEPARATOR: // nt add 2009/10/21
		if( shift )
			c = '<' ;
		else
			c = ',' ;
		break ;
	case 186:
		if( shift )
			c = ':' ;
		else
			c = ';' ;
		break ;
	case 220:
		if( shift )
			c = '|' ;
		else
			c = '\\' ;
		break ;
	case 222:
		if( shift )
			c = '"' ;
		else
			c = '\'' ;
		break ;
	case 219:
		if( shift )
			c = '{' ;
		else
			c = '[' ;
		break ;
	case 221:
		if( shift )
			c = '}' ;
		else
			c = ']' ;
		break ;
	default: // nt add 2009/10/21, nt modify 2010/8/11
		if( shift )
		{
			if( nChar == 49 )
				c = '!' ;
			else
			if( nChar == 50 )
				c = '@' ;
			else
			if( nChar == 51 )
				c = '#' ;
			else
			if( nChar == 52 )
				c = '$' ;
			else
			if( nChar == 53 )
				c = '%' ;
			else
			if( nChar == 54 )
				c = '^' ;
			else
			if( nChar == 55 )
				c = '&' ;
			else
			if( nChar == 56 )
				c = '*' ;
			else
			if( nChar == 57 )
				c = '(' ;
			else
			if( nChar == 48 )
				c = ')' ;
		}
		else
		{
			if( (48 <= nChar && // 0123456789
				nChar <= 57) ||
				(65 <= nChar && // ABC...Z
				nChar <= 90) )
				c = nChar ;
			else
			if( VK_NUMPAD0 <= nChar &&
				nChar <= VK_NUMPAD9 )
				c = '0'+nChar-VK_NUMPAD0 ;
		}
	}
	if( c != '\0' )
		cadPromptAttach(c) ;

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
//--------------------------------------------------------------
//	Commands
void CCADView::OnViewPan() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		CCADDoc* pDoc = GetDocument() ;
		if( m_pCmd )
			delete m_pCmd ;
		m_pCmd = new CPan(pDoc, this) ;
	}

	return ;
}

void CCADView::OnViewRotate() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		CCADDoc* pDoc = GetDocument();
		if( m_pCmd )
			delete m_pCmd ;
		m_pCmd = new CRotate(pDoc, this) ;
	}

	return ;
}


void CCADView::OnViewZoom() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		CCADDoc* pDoc = GetDocument() ;
		if( m_pCmd )
			delete m_pCmd ;
		m_pCmd = new CZoom(pDoc, this) ;
	}
}

void CCADView::OnViewZoomBox() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		CCADDoc* pDoc = GetDocument() ;
		if( m_pCmd )
			delete m_pCmd ;
		m_pCmd = new CZoomBox(pDoc, this) ;
	}

	return ;
}

void CCADView::OnViewZoomOut()
{
	// TODO: Add your command handler code here
	if (m_pVI)
	{
		ClearCmd();
		viZoom(m_pVI, 1.1);
		Invalidate(FALSE);
	}

	return;
}

void CCADView::OnViewZoomIn()
{
	// TODO: Add your command handler code here
	if (m_pVI)
	{
		ClearCmd();
		viZoom(m_pVI, 1/1.1);
		Invalidate(FALSE);
	}

	return;
}

void CCADView::OnViewFull()
{
	// TODO: Add your command handler code here
	if (m_pVI)
	{
		CCADDoc* pDoc = GetDocument();
		if (pDoc == NULL)
			return;
		PNT3D center;
		pDoc->GetCenter(center);
		ClearCmd();
		PNT3D p, q, O;
		VEC3D X, Y, Z, v;
		viGetCenterLF(m_pVI, X, Y, Z, O);
		mathPrjPntPln(center, O, Z, q);
		v[0] = O[0]-q[0];
		v[1] = O[1]-q[1];
		v[2] = O[2]-q[2];
		viPan(m_pVI, v);

		int i, j, k ;
		double d, w, h, W, H, sx = 1., sy = 1., s ;
		PNT3D min, max ;
		BOX2D box ;
		box.min[0] = MAX_DBL ;
		box.min[1] = MAX_DBL ;
		box.max[0] = -MAX_DBL ;
		box.max[1] = -MAX_DBL ;
		pDoc->GetSpace(min, max);
		for (i = 0; i < 2; i++)
		{
			p[0] = i < 1 ? min[0] : max[0];
			for (j = 0; j < 2; j++)
			{
				p[1] = j < 1 ? min[1] : max[1];
				for (k = 0; k < 2; k++)
				{
					p[2] = k < 1 ? min[2] : max[2];
					d = (p[0]-O[0])*X[0]+(p[1]-O[1])*X[1]+(p[2]-O[2])*X[2] ;
					if( box.min[0] > d )
						box.min[0] = d ;
					if( box.max[0] < d )
						box.max[0] = d ;
					d = (p[0]-O[0])*Y[0]+(p[1]-O[1])*Y[1]+(p[2]-O[2])*Y[2] ;
					if( box.min[1] > d )
						box.min[1] = d ;
					if( box.max[1] < d )
						box.max[1] = d ;
				}
			}
		}
		viGetViewWidthHeight(m_pVI, &w, &h) ;
		W = box.max[0]-box.min[0] ;
		H = box.max[1]-box.min[1] ;
		if( W > MIN_DBL )
			sx = w/W ;
		if( H > MIN_DBL )
			sy = h/H ;
		s = mathMIN(sx, sy) ;
		viZoom(m_pVI, 1./s) ;

		Invalidate(FALSE);
	}

	return;
}

void CCADView::OnViewFullWin()
{
	// TODO: Add your command handler code here
	if (m_pVI)
	{
		CCADDoc* pDoc = GetDocument();
		if (pDoc == NULL)
			return;
		BOX3D stlBox ;
		pDoc->CalBox(&stlBox) ;
		PNT3D center;
		mathMidPnt(stlBox.min, stlBox.max, center) ;
		ClearCmd();
		PNT3D p, q, O;
		VEC3D X, Y, Z, v;
		viGetCenterLF(m_pVI, X, Y, Z, O);
		mathPrjPntPln(center, O, Z, q);
		v[0] = O[0]-q[0];
		v[1] = O[1]-q[1];
		v[2] = O[2]-q[2];
		viPan(m_pVI, v);

		int i, j, k ;
		double d, w, h, W, H, sx = 1., sy = 1., s ;
		BOX2D box ;
		box.min[0] = MAX_DBL ;
		box.min[1] = MAX_DBL ;
		box.max[0] = -MAX_DBL ;
		box.max[1] = -MAX_DBL ;
		for (i = 0; i < 2; i++)
		{
			p[0] = i < 1 ? stlBox.min[0] : stlBox.max[0];
			for (j = 0; j < 2; j++)
			{
				p[1] = j < 1 ? stlBox.min[1] : stlBox.max[1];
				for (k = 0; k < 2; k++)
				{
					p[2] = k < 1 ? stlBox.min[2] : stlBox.max[2];
					d = (p[0]-O[0])*X[0]+(p[1]-O[1])*X[1]+(p[2]-O[2])*X[2] ;
					if( box.min[0] > d )
						box.min[0] = d ;
					if( box.max[0] < d )
						box.max[0] = d ;
					d = (p[0]-O[0])*Y[0]+(p[1]-O[1])*Y[1]+(p[2]-O[2])*Y[2] ;
					if( box.min[1] > d )
						box.min[1] = d ;
					if( box.max[1] < d )
						box.max[1] = d ;
				}
			}
		}
		viGetViewWidthHeight(m_pVI, &w, &h) ;
		W = box.max[0]-box.min[0] ;
		H = box.max[1]-box.min[1] ;
		if( W > MIN_DBL )
			sx = w/W ;
		if( H > MIN_DBL )
			sy = h/H ;
		s = mathMIN(sx, sy) ;
		viZoom(m_pVI, 1./s) ;

		Invalidate(FALSE);
	}

	return;
}

void CCADView::OnViewUp()
{
	// TODO: Add your command handler code here
	if (m_pVI)
	{
		VEC3D v, up;
		v[0] = 0. ;
		v[1] = 0. ;
		v[2] = 1. ;
		up[0] = 0.;
		up[1] = 1.;
		up[2] = 0.;
		SetViewDir(v, up);
	}

	return;
}

void CCADView::OnViewBottom()
{
	// TODO: Add your command handler code here
	if (m_pVI)
	{
		VEC3D v, up;
		v[0] = 0.;
		v[1] = 0.;
		v[2] = -1.;
		up[0] = 0.;
		up[1] = 1.;
		up[2] = 0.;
		SetViewDir(v, up);
	}

	return;
}

void CCADView::OnViewFront()
{
	// TODO: Add your command handler code here
	if (m_pVI)
	{
		VEC3D v, up;
		v[0] = 0.;
		v[1] = -1.;
		v[2] = 0.;
		up[0] = 0.;
		up[1] = 0.;
		up[2] = 1.;
		SetViewDir(v, up);
	}

	return;
}

void CCADView::OnViewRear()
{
	// TODO: Add your command handler code here
	if (m_pVI)
	{
		VEC3D v, up;
		v[0] = 0.;
		v[1] = 1.;
		v[2] = 0.;
		up[0] = 0.;
		up[1] = 0.;
		up[2] = 1.;
		SetViewDir(v, up);
	}

	return;
}

void CCADView::OnViewLeft()
{
	// TODO: Add your command handler code here
	if (m_pVI)
	{
		VEC3D v, up;
		v[0] = -1.;
		v[1] = 0.;
		v[2] = 0.;
		up[0] = 0.;
		up[1] = 0.;
		up[2] = 1.;
		SetViewDir(v, up);
	}

	return;
}

void CCADView::OnViewRight()
{
	// TODO: Add your command handler code here
	if (m_pVI)
	{
		VEC3D v, up;
		v[0] = 1.;
		v[1] = 0.;
		v[2] = 0.;
		up[0] = 0.;
		up[1] = 0.;
		up[2] = 1.;
		SetViewDir(v, up);
	}

	return;
}

void CCADView::OnView3D()
{
	// TODO: Add your command handler code here
	if (m_pVI)
	{
		VEC3D v, up;
		double d = sqrt(6.);
		v[0] = 1 / d;
		v[1] = -2 / d;
		v[2] = 1 / d;
		up[0] = 0.;
		up[1] = 0.;
		up[2] = 1.;
		SetViewDir(v, up);
	}

	return;
}

void CCADView::SetViewDir(VEC3D viewDir, VEC3D up)
{
	CCADDoc* pDoc = GetDocument();
	if (pDoc == NULL)
		return;
	double d = 2000. ;
	PNT3D ori, eye;
	pDoc->GetCenter(ori);
	eye[0] = ori[0]+viewDir[0]*d ;
	eye[1] = ori[1]+viewDir[1]*d ;
	eye[2] = ori[2]+viewDir[2]*d ;
	ori[0] = ori[0]-viewDir[0]*d ;
	ori[1] = ori[1]-viewDir[1]*d ;
	ori[2] = ori[2]-viewDir[2]*d ;
	viSetEyeCenterUp(m_pVI, eye, ori, up);
	//Invalidate(FALSE);
	OnViewFullWin() ; // nt add 2017/8/23

	return;
}
//--------------------------------------------------------------
// nt modify 2017/5/27
/*void CCADView::OnViewSection()
{
	// TODO: Add your command handler code here
	CCADDoc* pDoc = GetDocument() ;
	STL* stl = NULL ;
	ZB* zb = NULL ;

	if( m_drawSection )
		m_drawSection = 1 ;
	else
	{
		m_drawSection = 1 ;
		if( pDoc == NULL )
			return ;
		if( pDoc->HasZB() )
		{
			m_z = pDoc->GetMeanZ() ;
			pDoc->CutAll(m_z) ;
		}
		else
		{
			CSet* pSet = new CSet() ;
			pDoc->GetAllOfSelectedSTL(pSet) ;
			STL* stl = (STL*)pSet->GetObj(0) ;
			BOX3D box ;
			if( stl )
			{
				stlGetBox(stl, &box) ;
				m_z = 0.5*(box.min[2]+box.max[2]) ;
			}
		}
		pDoc->Update() ; // nt add 2017/5/15
		if( m_pCmd )
			delete m_pCmd ;
		//m_pCmd = new CSection(pDoc, this) ;
		m_pCmd = NULL ;
	}

	Invalidate(FALSE) ;

	return ;
}*/
//--------------------------------------------------------------
// nt add 2017/8/28
// nt modify 2017/10/10
CCADView* cadGetView()
{
	CWinApp* pApp = AfxGetApp() ;
	if( pApp == NULL )
		return NULL ;
	CMainFrame* pMF = (CMainFrame*)pApp->m_pMainWnd ;
	if( pMF == NULL )
		return NULL ;
	CCADView* pView = (CCADView*)pMF->GetActiveView() ;   

	return pView ;
}
////////////////////////////////////////////////////////////////