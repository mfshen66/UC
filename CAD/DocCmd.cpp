/*	2016/11/23 nt
 *	implementation file doc command
 */
#include "stdafx.h"
#include "CAD.h"
#include "CADDoc.h"
#include "CADView.h"
#include "Command.h"
#include "DocCmd.h"

///////////////////////////////////////////////////////////////
//	paste
CPaste::CPaste(CCADDoc* pDoc, CCADView* pView, PNT3D insertPos, STL* stl)
{
	Init(pDoc, pView) ;
	memcpy(m_insertPos, insertPos, sizeof(PNT3D)) ;
	m_p[0] = 0. ;
	m_p[1] = 0. ;
	m_stl = stl ;

	return ;
}

CPaste::~CPaste()
{
}

void CPaste::Notify(int msg)
{
	return ;
}

int	CPaste::GetID()
{
	return dctPaste ;
}

int	CPaste::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	VEC3D trans_v ;
	STL* copy = NULL ;

	//Navigate(nvFreePt, nFlags, p) ;
	//GetNavP(m_p) ;

	if( m_stl == NULL ||
		m_pDoc == NULL )
		return 0 ;

	// 计算视线与xy平面的交点
	if( fabs(v[2]) < MIN_DBL )
		return 0 ;
	m_p[0] = p[0]-v[0]*p[2]/v[2] ;
	m_p[1] = p[1]-v[1]*p[2]/v[2] ;
	m_p[2] = 0. ;
	trans_v[0] = m_p[0]-m_insertPos[0] ;
	trans_v[1] = m_p[1]-m_insertPos[1] ;
	trans_v[2] = 0. ;
	copy = stlCopy(m_stl) ;
	stlMove(copy, trans_v) ;
	m_pDoc->Add(copy) ;
	// nt add 2017/9/2
	CRCreate* pR = new CRCreate(copy) ;
	m_pDoc->m_rs.Add(pR) ;
	m_pDoc->CollisionDetect() ; // nt add 2017/9/5
	m_pDoc->SetModifiedFlag(TRUE) ;
	// end
	m_pDoc->Redraw() ;
	
	return 1 ;
}

int	CPaste::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

int	CPaste::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	//Navigate(nvFreePt, nFlags, p) ;

	return 1 ;
}

int	CPaste::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	if( m_pDoc )
		m_pDoc->ClearCmd() ;

	return 1 ;
}

// nt add 2009/10/21
int CPaste::OnReturn()
{
	double x, y ;
	VEC3D trans_v ;
	STL* copy = NULL ;
	CString str ;

	if( m_stl == NULL ||
		m_pDoc == NULL )
		return 0 ;

	cadPromptGet(str) ;
	cadPromptClear() ;
	if( strToNums(str, x, y) == 2 )
	{
		if( str.Find(_T("d")) >= 0 || // 作为增量处理
			str.Find(_T("D")) >= 0 )
		{
			trans_v[0] = x ;
			trans_v[1] = y ;
		}
		else
		{
			trans_v[0] = m_insertPos[0]-x ;
			trans_v[1] = m_insertPos[1]-y ;
		}
		
		trans_v[2] = 0. ;
		copy = stlCopy(m_stl) ;
		stlMove(copy, trans_v) ;
		m_pDoc->Add(copy) ;
		m_pDoc->Redraw() ;
	}

	return 1 ;
}

int	CPaste::Cancel()
{
	return 1 ;
}
//==============================================================
//  2017/3/28 nt
//	edit support
CEditSupport::CEditSupport(CCADDoc* pDoc, CCADView* pView, STL* stl, SSP* ssp)
{
	Init(pDoc, pView) ;
	m_ssp = *ssp ;
	m_stl = stl ;
	m_pick = 0 ; // 1=在STL模型上找到了一点及法矢
	m_arrow = 0 ; 
	m_lastId = -1 ;

	// nt add 2017/9/4
	if( stl->ss )
		m_copy = ssCopy((SS*)(stl->ss)) ;
	else
		m_copy = NULL ;

	if( m_pDoc )
	{
		SS* ss = (SS*)(stl->ss) ; // nt modify 2017/5/27
		if( ss )
			ssBeginEdit(ss) ;
	}
	m_old = NULL ;
	m_support = 0 ;

	return ;
}

CEditSupport::~CEditSupport()
{
	if( m_pDoc )
	{
		SS* ss = (SS*)(m_stl->ss) ; // nt modify 2017/5/27
		if( ss )
		{
			if( ssIsEditing(ss) == 1 )
				ssRebuild(ss, m_stl, &m_ssp) ;
			ssEndEdit(ss) ;
			// nt add 2017/9/4
			CRMakeSS* pRMakeSS = new CRMakeSS(m_stl, (SS*)(m_stl->ss), m_copy) ;
			m_pDoc->m_rs.Add(pRMakeSS) ;
			m_pDoc->SetModifiedFlag(TRUE) ;
			// end
			m_pDoc->Redraw() ;
		}
	}

	return ;
}

void CEditSupport::Notify(int msg)
{
	m_pick = 0 ;
	m_arrow = 0 ;
	m_old = NULL ;
	m_support = 0 ;

	return ;
}

int	CEditSupport::GetID()
{
	return dctEditSupport ;
}

int	CEditSupport::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	void* pVI = m_pView->GetVI() ;
	SUPPORT* support = NULL ;
	SS* ss = (SS*)(m_stl->ss) ; // nt modify 2017/5/27
	if( ss == NULL )
		return 0 ;

	if( m_old )
	{
		//ErazeSupport(pVI) ;
		supportMarkDel(m_old) ;
		m_lastId++ ;
		supportSetId(m_old, m_lastId) ;
		m_pDoc->Redraw() ;
	}
	m_old = NULL ;

	if( m_pick )
	{
		ErazeArrow(pVI) ;
		support = supportCreate(m_p, 
			                    m_normal, 
			                    m_ssp.h, 
			                    m_ssp.r1, 
			                    m_ssp.r2, 
			                    m_ssp.r,
			                    m_ssp.d) ;
		if( support )
		{
			m_lastId++ ;
			supportSetId(support, m_lastId) ;
			ssAddSupport(ss, support) ;
			supportDraw(support, pVI) ;
			DrawBall(m_p, 2*m_ssp.r, pVI) ;
		}
	}
	m_pick = 0 ;

	
	return 1 ;
}

int	CEditSupport::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

int	CEditSupport::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	void* pVI = m_pView->GetVI() ;
	double r = 2.5*viGetScale(pVI), z = MAX_DBL ;
	SS* ss = (SS*)(m_stl->ss) ; // nt modify 2017/5/27
	ErazeArrow(pVI) ;
	m_pick = 0 ;
	ErazeSupport(pVI) ;
	m_old = NULL ;
	if( ss == NULL )
		return 0 ;
	SUPPORT* support = ssPickSupport(ss, p, v, 2*m_ssp.r+r) ;
	if( support )
	{
		m_old = support ;
		DrawSupport(pVI) ;
	}
	else
	{
		PNT3D q ;
		STLTRI* tri = stlPick(m_stl, p, v, r, q, &z) ;
		if( tri )
		{
			memcpy(m_p, q, sizeof(PNT3D)) ;
			memcpy(m_normal, tri->normal, sizeof(VEC3D)) ;
			m_pick = 1 ;
			DrawArrow(pVI) ;
		}
	}

	return 1 ;
}

int	CEditSupport::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	if( m_pDoc )
		m_pDoc->ClearCmd() ;

	return 1 ;
}

int CEditSupport::OnReturn()
{
	return 1 ;
}

int	CEditSupport::Cancel()
{
	/*if( m_pDoc )
	{
		SS* ss = (SS*)(m_stl->ss) ; // nt modify 2017/5/27
		if( ss &&
			ssIsEditing(ss) == 1 )
			ssRebuild(ss, m_stl, &m_ssp) ;
	}*/

	return 1 ;
}

// arrow
void CEditSupport::DrawArrow(void* pVI)
{
	if( m_pick == 1 )
	{
		double d = viGetScale(pVI) ;
		glEnable(GL_COLOR_LOGIC_OP) ;
		glLogicOp(GL_XOR) ;
		glColor3ub(255, 255, 255) ;
		glPointSize(5.f) ;
		glBegin(GL_POINTS) ;
		glVertex3dv(m_p) ;
		glEnd() ;
		glLineWidth(2.f) ;
		drawArrowWithDisk(m_p, m_normal, 10*d, 5*d, 50*d, 8) ;
		glDisable(GL_COLOR_LOGIC_OP) ;
		m_arrow = 1 ;
		glFlush() ;
	}
	
	return ;
}

// arrow
void CEditSupport::ErazeArrow(void* pVI)
{
	if( m_arrow )
	{
		DrawArrow(pVI) ;
		m_arrow = 0 ;
	}

	return ;
}

// support
void CEditSupport::DrawSupport(void* pVI)
{
	if( m_old )
	{
		glEnable(GL_COLOR_LOGIC_OP) ;
		glLogicOp(GL_XOR) ;
		glColor3ub(255, 255, 255) ;
		glLineWidth(2.5f) ;
		VEC3D X, Y, Z ;
		viGetViewDir(pVI, Z) ;
		mathGetXYFromZ(Z, X, Y) ;
		drawArc(m_old->p, X, Y, 2*m_old->r, PI2, 12) ;
		glDisable(GL_COLOR_LOGIC_OP) ;
		m_support = 1 ;
		glFlush() ;
	}
	
	return ;
}

// support
void CEditSupport::ErazeSupport(void* pVI)
{
	if( m_support )
	{
		DrawSupport(pVI) ;
		m_support = 0 ;
	}

	return ;
}

// ball
void CEditSupport::DrawBall(PNT3D center, double r, void* pVI)
{
	viLighting(pVI, 1) ;
	viAddDefaultLight(pVI) ;
	MATERIAL material ;
	materialGet(&material, "BlackRubber2") ;
	materialSet(&material) ;
	drawSphere(center, r, 8, 8) ;
	viLighting(pVI, 0) ;
	glFlush() ;

	return ;
}
//==============================================================
// nt add 2017/6/7
void srInit(SR* sr)
{
	sr->type = -1 ; // 0=SUPPORT, 1=ROD
	sr->entity = NULL ; // SUPPORT or ROD
	sr->drawed = 0 ; // 0=not draw, 1=drawed
	sr->drawMode = 0 ; // dmNormal, dmPrompt, dmSelect, dmEraze

	return ;
}

void srInit2(SR* sr, int type, void* entity, PNT3D p)
{
	if( sr &&
		entity )
	{
		sr->type = type ;
		sr->entity = entity ;
		if( p )
			memcpy(sr->p, p, sizeof(PNT3D)) ;
		else
		{
			sr->p[0] = 0. ; // initialize sr->p by 0
			sr->p[1] = 0. ;
			sr->p[2] = 0. ;
		}
	}

	return ;
}

void srDraw(SR* sr, void* pVI, int drawMode)
{
	if( sr &&
		sr->entity )
	{
		switch(drawMode)
		{
		case dmNormal:
			glColor3ub(0, 0, 0) ;
			break ;
		case dmPrompt:
			glColor3ub(255, 0, 0) ;
			break ;
		case dmSelect:
			glColor3ub(0, 255, 0) ;
			break ;
		case dmEraze:
			glColor3ub(230, 230, 230) ;
			break ;
		default:
			break ;
		}
		glEnable(GL_DEPTH_TEST) ;
		glLineWidth(1.f) ;
		if( sr->type == 0 )
		{ // nt modify 20178/24 显示支撑的端点
			SUPPORT* support = (SUPPORT*)(sr->entity) ;
			supportDraw2(support, pVI) ;
			//if( fabs(support->h-sr->p[2]) < 0.001 )
			if( fabs(support->z-sr->p[2]) < 0.001 ) // nt add 2017/8/30
			{
				if( drawMode == dmNormal )
					glColor3ub(230, 230, 230) ;
				glPointSize(5.f) ;
				glBegin(GL_POINTS) ;
					glVertex3dv(sr->p) ;
				glEnd() ;
			}
		}
		else
			rodDraw2((ROD*)(sr->entity), pVI) ;
		sr->drawMode = drawMode ;
		glDisable(GL_DEPTH_TEST) ;
		glFlush() ;
	}

	return ;
}
//--------------------------------------------------------------
//  2017/6/5 nt
//	edit rod
CEditRod::CEditRod(CCADDoc* pDoc, CCADView* pView, STL* stl, double r)
{
	Init(pDoc, pView) ;
	m_r = r ;
	m_stl = stl ;

	// nt add 2017/9/4
	if( stl->ss )
		m_copy = ssCopy((SS*)(stl->ss)) ;
	else
		m_copy = NULL ;

	if( stl )
	{
		SS* ss = (SS*)(stl->ss) ;
		if( ss )
			ssBeginEditRod(ss) ;
	}
	srInit(&m_selected) ;
	srInit(&m_prompting) ;

	return ;
}

CEditRod::~CEditRod()
{
	if( m_stl )
	{
		SS* ss = (SS*)(m_stl->ss) ;
		if( ss )
		{
			if( ssIsEditing(ss) == 2 )
				ssRebuild2(ss) ;
			ssEndEdit(ss) ;
			// nt add 2017/9/4
			CRMakeSS* pRMakeSS = new CRMakeSS(m_stl, (SS*)(m_stl->ss), m_copy) ;
			m_pDoc->m_rs.Add(pRMakeSS) ;
			m_pDoc->SetModifiedFlag(TRUE) ;
			// end
			m_pDoc->Redraw() ;
		}
	}

	return ;
}

SS* CEditRod::GetSS()
{
	if( m_stl == NULL )
		return NULL ;
	return (SS*)(m_stl->ss) ;
}

int CEditRod::Make(SUPPORT* support1, 
	               SUPPORT* support2, 
	               PNT3D p1, 
	               PNT3D p2)
{
	int rt = 1 ;
	void* pVI = m_pView->GetVI() ;
	SS* ss = GetSS() ;
	ROD* rod = NULL ;
	
	glEnable(GL_DEPTH_TEST) ;
	glColor3ub(0, 0, 0) ;
	glLineWidth(1.f) ;
	if( fabs(p1[2]-support1->z) < 0.001 && // ->h, nt modify 2017/8/30
		fabs(p2[2]-support2->z) < 0.001 ) // ->h, nt modify 2017/8/30
	{
		PNT3D q1, q2 ;
		mathEvalLin(support1->p, support1->normal, support1->d, q1) ;
		mathEvalLin(support2->p, support2->normal, support2->d, q2) ;
		double z0 = support1->z+support1->h1 ; // ->h, nt modify 2017/8/30
		double z1 = q1[2] ;
		double z2 = q2[2] ;
		double d = mathDist2D(q1, q2) ;
		q1[2] = z0 ;
		q2[2] = z0+d ;
		while((q1[2] < z1+0.001) && 
			  (q2[2] < z2+0.001))
		{
			rod = rodCreate(support1, support2, q1, q2, m_r) ;
			if( rod )
			{
				rodDraw2(rod, pVI) ;
				ssAddRod(ss, rod) ;
			}
			if( q1[2] < q2[2] )
				q1[2] += (2*d) ;
			else
				q2[2] += (2*d) ;
		}
		rt = 2 ;
	}
	else
	{
		rod = rodCreate(support1, support2, p1, p2, m_r) ;
		if( rod )
		{
			ssAddRod(ss, rod) ;
			rodDraw2(rod, pVI) ;
		}
	}
	glEnable(GL_DEPTH_TEST) ;

	return rt ;
}

void CEditRod::AddPromptEntity(int type, void* entity, PNT3D p)
{
	if( entity )
	{
		if( entity == m_prompting.entity ||
			entity == m_selected.entity )
			return ;
		void* pVI = m_pView->GetVI() ;
		if( m_prompting.entity )
			srDraw(&m_prompting, pVI, dmNormal) ;
		srInit2(&m_prompting, type, entity, p) ;
		srDraw(&m_prompting, pVI, dmPrompt) ;
	}

	return ;
}

void CEditRod::Notify(int msg)
{
}

int	CEditRod::GetID()
{
	return dctEditRod ;
}

int	CEditRod::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	SS* ss = GetSS() ;
	if( ss == NULL )
		return 0 ;
	void* pVI = m_pView->GetVI() ;
	if( m_prompting.entity )
	{
		int rt = 0 ;
		if( m_selected.entity )
		{
			if( m_selected.type == 0 && // generate ROD structure
				m_prompting.type == 0 )
				rt = Make((SUPPORT*)m_selected.entity,
					      (SUPPORT*)m_prompting.entity,
					      m_selected.p,
					      m_prompting.p) ;
			srDraw(&m_selected, pVI, dmNormal) ;
			srInit(&m_selected) ;
		}
		if( rt != 2 )
		{
			m_selected = m_prompting ;
			srDraw(&m_selected, pVI, dmSelect) ;
		}
		srInit(&m_prompting) ;
	}
	else
	{
		if( m_selected.entity )
		{
			srDraw(&m_selected, pVI, dmNormal) ;
			srInit(&m_selected) ;
		}
	}

	return 1 ;
}

int	CEditRod::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

int	CEditRod::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	SS* ss = GetSS() ;
	if( ss == NULL )
		return 0 ;
	void* pVI = m_pView->GetVI() ;
	double r = 5*viGetScale(pVI), z = MAX_DBL ;
	PNT3D q ;
	stlPick(m_stl, p, v, r, q, &z) ; // 就要z
	SUPPORT* support = NULL ;
	ROD* rod = NULL ;
	if( support = ssPickSupport2(ss, p, v, r, z, q) )
		AddPromptEntity(0, support, q) ;
	else
	if( rod = ssPickRod(ss, p, v, r, z) )
		AddPromptEntity(1, rod, NULL) ;
	else
	{
		if( m_prompting.entity )
		{
			srDraw(&m_prompting, pVI, dmNormal) ;
			srInit(&m_prompting) ;
		}
	}
		
	return 1 ;
}

int	CEditRod::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	if( m_pDoc )
		m_pDoc->ClearCmd() ;

	return 1 ;
}

int CEditRod::OnReturn()
{
	return 1 ;
}

int	CEditRod::Cancel()
{
	return 1 ;
}

int CEditRod::Del()
{
	SS* ss = GetSS() ;
	if( ss == NULL )
		return 0 ;
	if( m_selected.type == 1 &&
		m_selected.entity )
	{
		void* pVI = m_pView->GetVI() ;
		srDraw(&m_selected, pVI, dmEraze) ;
		ROD* rod = (ROD*)m_selected.entity ;
		rod->del = 1 ; // 未释放空间!!!!!!!!!!!!!
		srInit(&m_selected) ;
		return 1 ;
	}

	return 0 ;
}
///////////////////////////////////////////////////////////////
