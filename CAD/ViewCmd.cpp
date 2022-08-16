/*	2000/5/13 nt
 *	implementation file view command
 */
#include "stdafx.h"
#include "CADDoc.h"
#include "CADView.h"
#include "Command.h"
#include "ViewCmd.h"

///////////////////////////////////////////////////////////////
/*	1999/4/8 nt
 *	CPan
 */
CPan::CPan(CCADDoc* pDoc, CCADView* pView)
{
	Init(pDoc, pView) ;
	m_p[0] = 0.0 ;
	m_p[1] = 0.0 ;
	m_p[2] = 0.0 ;
}

CPan::~CPan()
{
}

void CPan::Notify(int msg)
{
	return ;
}

int CPan::GetID()
{
	return vctPan ;
}

int CPan::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	void* pVI ;

	if( m_pView &&
		(pVI = m_pView->GetVI()) != NULL )
	{
		m_p[0] = p[0] ;
		m_p[1] = p[1] ;
		m_p[2] = p[2] ;
		m_mouse = 1 ;
		return 1 ;
	}
	else
		return 0 ;
}

int CPan::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	void* pVI ;

	if( m_pView &&
		(pVI = m_pView->GetVI()) != NULL )
	{
		m_mouse = 0 ;
		return 1 ;
	}
	else
		return 0 ;
}

int CPan::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	void* pVI ;
	double vec[3] ;

	if( m_pView &&
		(pVI = m_pView->GetVI()) != NULL )
	{
		if( m_mouse )
		{
			vec[0] = p[0]-m_p[0] ;
			vec[1] = p[1]-m_p[1] ;
			vec[2] = p[2]-m_p[2] ;
			viPan(pVI, vec) ;
			m_pView->Invalidate(FALSE) ;
		}
		return 1 ;
	}
	else
		return 0 ;
}

int CPan::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

// nt add 2009/10/21
int CPan::OnReturn()
{
	return 1 ;
}

int CPan::Cancel()
{
	return 1 ;
}
//-------------------------------------------------------------
/*	1999/4/8 nt
*	CRotate
*/
CRotate::CRotate(CCADDoc* pDoc, CCADView* pView)
{
	Init(pDoc, pView) ;
	m_p[0] = 0.0 ;
	m_p[1] = 0.0 ;
	m_p[2] = 0.0 ;
}

CRotate::~CRotate()
{
}

void CRotate::Notify(int msg)
{
	return ;
}

int CRotate::GetID()
{
	return vctRotate ;
}

int CRotate::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	void* pVI ;

	if( m_pView &&
		(pVI = m_pView->GetVI()) != NULL )
	{
		m_p[0] = p[0] ;
		m_p[1] = p[1] ;
		m_p[2] = p[2] ;
		m_mouse = 1 ;
		return 1 ;
	}
	else
		return 0 ;
}

int CRotate::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	void* pVI ;

	if( m_pView &&
		(pVI = m_pView->GetVI()) != NULL )
	{
		m_mouse = 0 ;
		return 1 ;
	}
	else
		return 0 ;
}

int CRotate::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	void* pVI ;
	double d, angle, viewdir[3], raxis[3] ;
	double w, h ;

	if( m_pView &&
		(pVI = m_pView->GetVI()) != NULL )
	{
		if( m_mouse )
		{
			v[0] = p[0]-m_p[0] ;
			v[1] = p[1]-m_p[1] ;
			v[2] = p[2]-m_p[2] ;
			viGetViewWidthHeight(pVI, &w, &h) ;
			if( (d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2])) < 0.0001*(w+h) )
				return 1 ;
			angle = 32.0*d/(w+h) ;
			if( angle < 0.02 )
				angle = 0.02 ;
			if( angle > 0.2  )
				angle = 0.2 ;

			viGetViewDir(pVI, viewdir) ;
			viVProduct(viewdir, v, raxis) ;
			if( !viUniVec(raxis, 1.0e-10) )
				return 1 ;
			m_p[0] = p[0] ;
			m_p[1] = p[1] ;
			m_p[2] = p[2] ;
			// nt add 2017/7/7 实现绕零件中心旋转
			PNT3D center ;
			BOX3D box ;
			m_pDoc->CalBox(&box) ;
			mathMidPnt(box.min, box.max, center) ;
			viRotate(pVI, raxis, 0.75*angle, center) ;
			mathRotPnt(raxis, center, 0.75*angle, m_p, m_p) ;
			// end
			//viRotate(pVI, raxis, angle, m_p) ;
			m_pView->Invalidate(FALSE) ;
		}
		return 1 ;
	}
	else
		return 0 ;
}

int CRotate::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

// nt add 2009/10/21
int CRotate::OnReturn()
{
	return 1 ;
}

int CRotate::Cancel()
{
	return 1 ;
}
//-------------------------------------------------------------
/*	1999/4/8 nt
*	CZoom
*/
CZoom::CZoom(CCADDoc* pDoc, CCADView* pView)
{
	Init(pDoc, pView) ;
	m_p[0] = 0.0 ;
	m_p[1] = 0.0 ;
	m_p[2] = 0.0 ;
	m_scale = 1.0 ;
}

CZoom::~CZoom()
{
}

void CZoom::Notify(int msg)
{
	return ;
}

int CZoom::GetID()
{
	return vctZoom ;
}

int CZoom::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	void* pVI ;

	if( m_pView &&
		(pVI = m_pView->GetVI()) != NULL )
	{
		m_p[0] = p[0] ;
		m_p[1] = p[1] ;
		m_p[2] = p[2] ;
		m_mouse = 1 ;
		m_scale = 1.0 ;
		return 1 ;
	}
	else
		return 0 ;
}

int CZoom::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	void* pVI ;

	if( m_pView &&
		(pVI = m_pView->GetVI()) != NULL )
	{
		m_mouse = 0 ;
		return 1 ;
	}
	else
		return 0 ;
}

int CZoom::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	void* pVI ;
	double eye[3], center[3] ;
	double up[3], w, h, d, zm_scale ;

	if( m_pView &&
		(pVI = m_pView->GetVI()) != NULL )
	{
		if( m_mouse )
		{
			viGetEyeCenterUp(pVI, eye, center, up) ;
			v[0] = p[0]-m_p[0] ;
			v[1] = p[1]-m_p[1] ;
			v[2] = p[2]-m_p[2] ;
			d = v[0]*up[0]+v[1]*up[1]+v[2]*up[2] ;
			viGetViewWidthHeight(pVI, &w, &h) ;
			h = fabs(h) ;
			if( d < -h )
				d = -h ;
			if( d >  h )
				d =  h ;
			if( h < 1.0e-20 )
				return 1 ;
			d /= h ;
			zm_scale = 49.0*d*d+1.0 ;
			if( d < 0.0 )
				zm_scale = 1.0 / zm_scale ;
			if( fabs(zm_scale-1.0) < 0.01 ||
				m_scale < 1.0e-10 ||
				m_scale > 1.0e+10 )
				return 1 ;
			if( viZoomByMouse(pVI, zm_scale/m_scale, m_p) != 1 )
			{
				AfxMessageBox(_T("Can not zoom any more!")) ;
				return 1 ;
			}
			m_scale = zm_scale ;
			m_pView->Invalidate(FALSE) ;
		}
		return 1 ;
	}
	else
		return 0 ;
}

int CZoom::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

// nt add 2009/10/21
int CZoom::OnReturn()
{
	return 1 ;
}

int CZoom::Cancel()
{
	return 1 ;
}
//-------------------------------------------------------------
//	zoom by window
/*	2004/11/5 nt
 *	用窗口放大视区
 *	nt modify 2005/3/31
 */
CZoomBox::CZoomBox(CCADDoc* pDoc, CCADView* pView)
{
	Init(pDoc, pView) ;
	m_p1[0] = -1.0e10 ;
	m_p1[1] = -1.0e10 ;
	m_p1[2] = -1.0e10 ;
	m_p2[0] = -1.0e10 ;
	m_p2[1] = -1.0e10 ;
	m_p2[2] = -1.0e10 ;
	m_drawed = 0 ;

	return ;
}

CZoomBox::~CZoomBox()
{
}

void CZoomBox::Notify(int msg)
{
	return ;
}

int CZoomBox::GetID()
{
	return vctZoomBox ;
}

/*
 *
 */
int CZoomBox::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	m_mouse = 1 ;
	m_p1[0] = p[0] ;
	m_p1[1] = p[1] ;
	m_p1[2] = p[2] ;
	m_p2[0] = p[0] ;
	m_p2[1] = p[1] ;
	m_p2[2] = p[2] ;
	m_drawed = 0 ;

	return 1 ;
}

int CZoomBox::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	m_mouse = 0 ;
	if( m_drawed == 1 )
		Draw() ;
	double d = sqrt((m_p2[0]-m_p1[0])*(m_p2[0]-m_p1[0])+
				   (m_p2[1]-m_p1[1])*(m_p2[1]-m_p1[1])+
				   (m_p2[2]-m_p1[2])*(m_p2[2]-m_p1[2])) ;
	if( d < 5.0*viGetScale(m_pVI) )
		return 1 ;
	viZoomByBox(m_pVI, m_p1, m_p2) ;
	m_pView->Invalidate(FALSE) ;

	return 1 ;
}

int CZoomBox::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	if( m_mouse )
	{
		if( m_drawed == 1 )
			Draw() ;
		m_p2[0] = p[0] ;
		m_p2[1] = p[1] ;
		m_p2[2] = p[2] ;
		Draw() ;
	}

	return 1 ;
}

int CZoomBox::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

// nt add 2009/10/21
int CZoomBox::OnReturn()
{
	return 1 ;
}

int CZoomBox::Cancel()
{
	return 1 ;
}

int CZoomBox::Draw()
{
	glLineWidth(1.f) ; // nt add 2017/1/11
	glColor3ub(255, 255, 255) ;
	DrawRect(m_p1, m_p2) ;
	m_drawed = m_drawed?0:1 ;

	return 1 ;
}
///////////////////////////////////////////////////////////////