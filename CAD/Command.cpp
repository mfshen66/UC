/*	2000/7/18 nt
 *	implementation file class command
 */
#include "stdafx.h"
#include "CADDoc.h"
#include "CADView.h"
#include "Command.h"

///////////////////////////////////////////////////////////////
//	2000/5/13 nt
//	CViewCmd
CCommand::CCommand()
{
	m_pDoc = NULL ;
	m_pView = NULL ;
	m_pVI = NULL ;
}

CCommand::~CCommand()
{
}

void CCommand::Init(CCADDoc* pDoc, CCADView* pView)
{
	m_mouse = 0 ;
	m_drawed = 0 ;
	m_color = 0 ;
	m_pDoc = pDoc ;
	m_pView = pView ;
	m_pVI = pView->GetVI() ;
}

void CCommand::DrawLin(double p1[3], double p2[3])
{
	glEnable(GL_COLOR_LOGIC_OP) ;
	glLogicOp(GL_XOR) ;
	glBegin(GL_LINES) ;
	glVertex3dv(p1) ;
	glVertex3dv(p2) ;
	glEnd() ;
	glDisable(GL_COLOR_LOGIC_OP) ;
	glFlush() ;

	return ;
}

void CCommand::DrawRect(double p1[3], double p2[3])
{
	glEnable(GL_COLOR_LOGIC_OP) ;
	glLogicOp(GL_XOR) ;
	double X[3], Y[3], Z[3] ;
	double O[3], c1[3], c2[3] ;
	viGetCenterLF(m_pVI, X, Y, Z, O) ;
	viPrjPntLin(p2, p1, X, c1) ;
	viPrjPntLin(p2, p1, Y, c2) ;
	glBegin(GL_LINE_LOOP) ;
	glVertex3dv(p1) ;
	glVertex3dv(c1) ;
	glVertex3dv(p2) ;
	glVertex3dv(c2) ;
	glEnd() ;
	glDisable(GL_COLOR_LOGIC_OP) ;
	glFlush() ;
	return ;
}
///////////////////////////////////////////////////////////////