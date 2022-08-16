/*	2000/7/18 nt
 *	header file of class CCommand
 */
#pragma once

 ///////////////////////////////////////////////////////////////
//	1999/4/5 nt
//	CCommand
class CCADDoc ;
class CCADView ;
class CCommand
{
protected:
	CCADDoc*	m_pDoc ;
	CCADView*	m_pView ;
	void*		m_pVI ;
	int			m_color ;
	int			m_mouse ; // 0 up, 1 down
	int			m_drawed ;
public:
	CCommand() ;
	virtual ~CCommand() ;

	void		Init(CCADDoc* pDoc, CCADView* pView) ;
	void		DrawLin(double p1[3], double p2[3]) ;
	void		DrawRect(double p1[3], double p2[3]) ;
	virtual void Notify(int msg) = 0 ;
	virtual int	GetID() = 0 ;

	virtual int OnLButtonDown(UINT nFlags, double p[3], double v[3]) = 0 ;
	virtual int OnLButtonUp(UINT nFlags, double p[3], double v[3]) = 0 ;
	virtual int OnMouseMove(UINT nFlags, double p[3], double v[3]) = 0 ;
	virtual int OnRButtonDown(UINT nFlags, double p[3], double v[3]) = 0 ;
	virtual int OnReturn() = 0 ; // nt add 2009/10/21
	virtual int Cancel() = 0 ;
} ;
///////////////////////////////////////////////////////////////
