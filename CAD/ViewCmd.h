/*	2000/5/13 nt
 *	Header File of Class CViewCmd
 */
#pragma once

///////////////////////////////////////////////////////////////
/*	2009/12/1 nt
 *	view command type
 */
enum EViewCmdType
{
	vctPan = 0,
	vctRotate = 1,
	vctZoom = 2,
	vctZoomBox = 3
} ;
//-------------------------------------------------------------
/*	1999/4/8 nt
 *	CPan
 */
class CPan : public CCommand
{
private:
	double		m_p[3] ;
public:
	CPan(CCADDoc* pDoc, CCADView* pView) ;
	~CPan() ;

	void        Notify(int msg) ;
	int			GetID() ;
	int			OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int			OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int			OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int			OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int			OnReturn() ; // nt add 2009/10/21
	int			Cancel() ;
} ;

/*	1999/4/8 nt
 *	CRotate
 */
class CRotate : public CCommand
{
private:
	double		m_p[3] ;
public:
	CRotate(CCADDoc* pDoc, CCADView* pView) ;
	~CRotate() ;

	void        Notify(int msg) ;
	int			GetID() ;
	int			OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int			OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int			OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int			OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int			OnReturn() ; // nt add 2009/10/21
	int			Cancel() ;
} ;

/*	1999/4/8 nt
 *	CZoom
 */
class CZoom : public CCommand
{
private:
	double		m_p[3] ;
	double		m_scale ;
public:
	CZoom(CCADDoc* pDoc, CCADView* pView) ;
	~CZoom() ;

	void        Notify(int msg) ;
	int			GetID() ;
	int			OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int			OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int			OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int			OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int			OnReturn() ; // nt add 2009/10/21
	int			Cancel() ;
} ;

/*	2004/11/5 nt
 *	CZoomBox
 */
class CZoomBox : public CCommand
{
private:
	double		m_p1[3] ;
	double		m_p2[3] ;
	int			m_drawed ;
public:
	CZoomBox(CCADDoc* pDoc, CCADView* pView) ;
	~CZoomBox() ;

	void        Notify(int msg) ;
	int			GetID() ;
	int			OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int			OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int			OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int			OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int			OnReturn() ; // nt add 2009/10/21
	int			Cancel() ;
	int			Draw() ;
} ;
///////////////////////////////////////////////////////////////