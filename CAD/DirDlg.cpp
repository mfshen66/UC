// DirDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "CADDoc.h"
#include "CADView.h"
#include "DirDlg.h"
#include "afxdialogex.h"


// CDirDlg 对话框

IMPLEMENT_DYNAMIC(CDirDlg, CDialogEx)

CDirDlg::CDirDlg(CWnd* pParent /*=NULL*/)
	: CDlg(IDD_DIRDLG, pParent)
	, m_xAngle(0)
	, m_yAngle(0)
	, m_zAngle(0)
{
	m_stl = NULL ;
	m_pDoc = NULL ;
	m_O[0] = 0. ;
	m_O[1] = 0. ;
	m_O[2] = 0. ;
	m_mode = 0 ;
	m_arrow = 0 ;
	m_pick = 0 ;
	m_lbd = 0 ;
	m_mpos[0] = 0. ;
	m_mpos[1] = 0. ;
	m_mpos[2] = 0. ;
	m_axis2 = ' ' ;
	m_pivot[0] = 0. ;
	m_pivot[1] = 0. ;
	m_pivot[2] = 0. ;
	m_v[0] = 0. ;
	m_v[1] = 0. ;
	m_v[2] = 0. ;
	m_a = 0. ;
	m_center[0] = 0. ;
	m_center[1] = 0. ;
	m_center[2] = 0. ;
	m_normal[0] = 0. ;
	m_normal[1] = 0. ;
	m_normal[2] = 0. ;
	m_max = 10 ;
	m_n = 0 ;
	m_ts = new TRANS[m_max] ;
	// nt add 2017/1/9
	m_rb = rballCreate(5, 0.05, 3.f, 3.f) ;
	m_axis = ' ' ;
	m_move = 0 ; // nt add 2017/1/17
	m_vec[0] = 0. ; // nt add 2017/1/17
	m_vec[1] = 0. ; // nt add 2017/1/17
	m_vec[2] = 0. ; // nt add 2017/1/17
	m_zoom = 0 ; // nt add 2017/1/17
	m_scale = 1. ; // nt add 2017/1/17
	m_zc[0] = 0. ; // nt add 2017/1/17
	m_zc[1] = 0. ; // nt add 2017/1/17
	m_zc[2] = 0. ; // nt add 2017/1/17
}

// nt modify 2017/9/3
CDirDlg::~CDirDlg()
{
	m_mode = -1 ;
	m_cancel = 1 ; // nt add 2017/3/30
	m_arrow = 0 ;
	m_pick = 0 ;
	DrawDoc() ; // nt modify 2017/3/30

	// nt add 2017/5/21
	stlCalBox(m_stl) ;
	m_pDoc->CollisionDetect() ;

	// nt add 2017/8/23
	m_pDoc->Update() ; // nt add 2017/8/27

	// nt add 2017/9/3
	if( m_n > 0 )
	{
		CRDir* pRDir = new CRDir(m_stl, m_n, m_ts) ;
		m_pDoc->m_rs.Add(pRDir) ;
		m_pDoc->SetModifiedFlag(TRUE) ;
	}

	if( m_ts )
		delete []m_ts ;
	// nt add 2017/1/9
	if( m_rb )
		rballFree(m_rb) ;

	return ;
}

void CDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_xAngle);
	DDV_MinMaxDouble(pDX, m_xAngle, -360, 360);
	DDX_Text(pDX, IDC_EDIT2, m_yAngle);
	DDV_MinMaxDouble(pDX, m_yAngle, -360, 360);
	DDX_Text(pDX, IDC_EDIT3, m_zAngle);
	DDV_MinMaxDouble(pDX, m_zAngle, -360, 360);
}


BEGIN_MESSAGE_MAP(CDirDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDirDlg::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDirDlg::OnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDirDlg::OnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDirDlg::OnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDirDlg::OnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDirDlg::OnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDirDlg::OnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CDirDlg::OnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CDirDlg::OnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CDirDlg::OnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CDirDlg::OnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CDirDlg::OnClickedButton12)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDirDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDirDlg::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CDirDlg::OnDeltaposSpin3)
END_MESSAGE_MAP()

void CDirDlg::Init(STL* stl, CCADDoc* pDoc)
{
	if( stl != NULL &&
		pDoc != NULL )
	{
		m_stl = stl ;
		stlGetCenter(m_stl, m_O) ;
		m_pDoc = pDoc ;
		m_mode = 0 ;
		m_cancel = 0 ; // nt add 2017/3/30
	}

	return ;
}

void CDirDlg::Add(TRANS* t)
{
	if( m_n > m_max ) // 应该不会出现这种情况
		return ; // error
	else
	if( m_n == m_max ) // 原有的buffer已经满了
	{
		m_max *= 2 ;
		TRANS* buf = new TRANS[m_max] ;
		memcpy(buf, m_ts, sizeof(TRANS)*m_n) ;
		delete []m_ts ;
		m_ts = buf ;
	}

	m_ts[m_n] = *t ;
	m_n++ ;

	// nt add 2017/1/19
	if( m_n == 1 )
		EnableUndo(1) ;

	return ;
}

// 2017/1/11 把累计旋转量压入m_ts
void CDirDlg::Push()
{
	TRANS t ;

	if( m_mode == 2 && mathVecLen(m_vec) > MIN_LEN ) // nt add 2017/1/18
	{
		t.type = 0 ;
		t.p[0] = 0. ;
		t.p[1] = 0. ;
		t.p[2] = 0. ;
		memcpy(t.v, m_vec, sizeof(VEC3D)) ;
		t.a = 0. ;
		Add(&t) ;
		m_a = 0. ;
	}
	else
	if( m_mode == 0 && fabs(m_a) > MIN_ANG )
	{
		t.type = 1 ;
		memcpy(t.p, m_pivot, sizeof(PNT3D)) ;
		memcpy(t.v, m_v, sizeof(VEC3D)) ;
		t.a = m_a ;
		Add(&t) ;
		m_a = 0. ;
	}
	else
	if( m_mode == 3 && fabs(m_scale-1.) > 1.e-6 ) // nt add 2017/1/18
	{
		t.type = 2 ;
		memcpy(t.p, m_zc, sizeof(PNT3D)) ;
		t.v[0] = 0. ;
		t.v[1] = 0. ;
		t.v[2] = 0. ;
		t.a = m_scale ;
		Add(&t) ;
		m_a = 0. ;
	}
	
	return ;
}

// CDirDlg 消息处理程序

// 自动定向
void CDirDlg::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}

// 旋转x轴
void CDirDlg::OnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	Rotate('X', NULL, m_xAngle, 1) ;
	m_xAngle = 0. ;
	UpdateData(FALSE) ;

	return ;
}

// 旋转y轴
void CDirDlg::OnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	Rotate('Y', NULL, m_yAngle, 1) ;
	m_yAngle = 0. ;
	UpdateData(FALSE) ;

	return ;
}

// 旋转z轴
void CDirDlg::OnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	Rotate('Z', NULL, m_zAngle, 1) ;
	m_zAngle = 0. ;
	UpdateData(FALSE) ;

	return ;
}

// 上
void CDirDlg::OnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	Rotate('X', NULL, 180., 1) ;

	return ;
}

// 左
void CDirDlg::OnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	Rotate('Y', NULL, -90., 1) ;

	return ;
}

// 前
void CDirDlg::OnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	Rotate('X', NULL, 90., 1) ;

	return ;
}

// 下
void CDirDlg::OnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	Rotate('Y', NULL, 180., 1) ;

	return ;
}

// 右
void CDirDlg::OnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	Rotate('Y', NULL, 90., 1) ;

	return ;
}

// 后
void CDirDlg::OnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	Rotate('X', NULL, -90., 1) ;

	return ;
}

// 交互选底面
void CDirDlg::OnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_mode == 1 ) // 交互确定底面
		m_mode = 0 ;
	else // 非交互确定底面
		m_mode = 1 ;
	DrawDoc() ;

	return ;
}

// 回退
void CDirDlg::OnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_n > 0 )
	{
		TRANS t = m_ts[m_n-1] ;
		if( t.type == 0 )
		{
			t.v[0] = -t.v[0] ;
			t.v[1] = -t.v[1] ;
			t.v[2] = -t.v[2] ;
			stlMove(m_stl, t.v) ;
			m_O[0] += t.v[0] ; // nt add 2017/1/18
			m_O[1] += t.v[1] ; // nt add 2017/1/18
			m_O[2] += t.v[2] ; // nt add 2017/1/18
		}
		else
		if( t.type == 1 )
		{
			stlRotate(m_stl, t.p, t.v, -t.a) ;
		}
		else
		if( t.type == 2 ) // nt add 2017/1/18
		{
			stlScale2(m_stl, t.p, 1/t.a) ; // 比例缩放
		}
		m_n-- ;
		DrawDoc() ;
	}

	// nt add 2017/1/19
	if( m_n == 0 )
		EnableUndo(0) ;

	return ;
}

void CDirDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_xAngle -= pNMUpDown->iDelta*5. ; // 度
    UpdateData(FALSE) ;
	*pResult = 0;
}

void CDirDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_yAngle -= pNMUpDown->iDelta*5. ; // 度
    UpdateData(FALSE) ;
	*pResult = 0;
}

void CDirDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_zAngle -= pNMUpDown->iDelta*5. ; // 度
    UpdateData(FALSE) ;
	*pResult = 0;
}

// nt add 2017/1/19
void CDirDlg::EnableUndo(BOOL enable)
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON12) ;
	if( pButton )
		pButton->EnableWindow(enable) ;
	
	return ;
}

// 注意a的单位是度
void CDirDlg::Rotate(char XYZ, VEC3D axis, double a, int undo)
{
	//PNT3D center ;
	VEC3D v = {0., 0., 0.} ;

	if( m_stl == NULL ||
		m_pDoc == NULL )
		return ; // error
	
	//stlGetCenter(m_stl, center) ;
	if( axis )
		memcpy(v, axis, sizeof(VEC3D)) ;
	else
	if( XYZ == 'X' )
		v[0] = 1. ;
	else
	if( XYZ == 'Y' )
		v[1] = 1. ;
	else
		v[2] = 1. ;
	stlRotate2(m_stl, m_O/*center*/, v, a/180.*PI1) ;
	stlCalBox(m_stl) ; // nt add 2017/1/5
	if( undo )
	{
		TRANS t ;
		t.type = 1 ;
		memcpy(t.p, m_O/*center*/, sizeof(PNT3D)) ;
		memcpy(t.v, v, sizeof(VEC3D)) ;
		t.a = a/180.*PI1 ;
		Add(&t) ;
	}

	// 绘制
	m_mode = 0 ;
	m_arrow = 0 ;
	m_axis = ' ' ;
	DrawDoc() ;
	/*CCADView* pView = cadGetView() ;
	if( pView )
		pView->OnDrawEx() ;*/
	
	return ;
}

void CDirDlg::Draw(void* pVI)
{
	double r, h ;
	PNT3D O ;
	VEC3D dir ;
	BOX3D box ;
	MATERIAL m_material ;
	double d = viGetScale(pVI) ;

	if( m_mode == 0 )
	{
		viGetViewDir(pVI, dir) ;
		stlGetBox(m_stl, &box) ;
		h = mathDist(box.min, box.max) ;
		r = 50*d ;
		//stlGetCenter(m_stl, m_O) ; // nt add 2017/9/6
		mathEvalLin(m_O, dir, -0.5*(h+r), O) ; // od -3
		rballSetCenterR(m_rb, O, r) ;

		viLighting(pVI, 1) ;
		viAddDefaultLight(pVI) ;

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
		glEnable(GL_BLEND) ;
		materialGet(&m_material, "Sky") ;
		materialSet(&m_material) ;
		rballDrawBall(m_rb, dir) ;
		glDisable(GL_BLEND) ;

		materialGet(&m_material, "White2") ;
		materialSet(&m_material) ;
		rballDrawBand(m_rb, 'X') ;
		rballDrawBand(m_rb, 'Y') ;
		rballDrawBand(m_rb, 'Z') ;

		viLighting(pVI, 0) ;

		glColor3ub(0, 0, 0) ;
		glEnable(GL_DEPTH_TEST) ;
		rballDrawArc(m_rb, dir) ;
		glDisable(GL_DEPTH_TEST) ;

		m_arrow = 0 ;
	}

	// nt add 2017/1/17
	if( (m_mode == 0 && m_lbd == 0) ||
		(m_mode == 2 && m_lbd == 1) )
		DrawMove(pVI, 0) ;
	if( (m_mode == 0 && m_lbd == 0) ||
		(m_mode == 3 && m_lbd == 1) )
		DrawZoom(pVI, 0) ;

	glFlush() ;

	return ;
}

void CDirDlg::DrawArrow(void* pVI)
{
	if( m_pick == 1 ) // nt modify 2017/3/30
	{
		double d = viGetScale(pVI) ;
		glEnable(GL_COLOR_LOGIC_OP) ;
		glLogicOp(GL_XOR) ;
		glColor3ub(255, 255, 255) ;
		glPointSize(5.f) ;
		glBegin(GL_POINTS) ;
		glVertex3dv(m_center) ;
		glEnd() ;
		glLineWidth(2.f) ;
		drawArrowWithDisk(m_center, m_normal, 10*d, 5*d, 50*d, 8) ;
		glDisable(GL_COLOR_LOGIC_OP) ;
		m_arrow = 1 ;
		glFlush() ;
	}
	
	return ;
}

void CDirDlg::ErazeArrow(void* pVI)
{
	if( m_arrow ) // nt modify 2017/3/30
	{
		DrawArrow(pVI) ;
		m_arrow = 0 ;
	}

	return ;
}

void CDirDlg::DrawAxis(void* pVI, char axis)
{
	MATERIAL material ;

	if( m_rb )
	{
		viLighting(pVI, 1) ;
		viAddDefaultLight(pVI) ;
		materialGet(&material, "Grey2") ;
		materialSet(&material) ;
		rballDrawBand(m_rb, axis) ;
		viLighting(pVI, 0) ;
		glFlush() ;
		m_axis = axis ;
	}

	return ;
}

void CDirDlg::ErazeAxis(void* pVI)
{
	MATERIAL material ;

	if( m_rb &&
		(m_axis == 'X' ||
		 m_axis == 'Y' ||
		 m_axis == 'Z') )
	{
		viLighting(pVI, 1) ;
		viAddDefaultLight(pVI) ;
		materialGet(&material, "White2") ;
		materialSet(&material) ;
		rballDrawBand(m_rb, m_axis) ;
		viLighting(pVI, 0) ;
		glFlush() ;
		m_axis = ' ' ;
	}

	return ;
}

// nt add 2017/1/17
void CDirDlg::DrawMove(void* pVI, int isOn)
{
	double w, r, d = viGetScale(pVI) ;
	COLORREF c ;
	RFRAME lf ;

	if( isOn )
	{
		c = RGB(200, 200, 200) ;
		m_move = 1 ;
	}
	else
	{
		c = RGB(255, 255, 255) ;
		m_move = 0 ;
	}
	r = 50*d ;
	w = r/2 ;
	viGetCenterLF(pVI, lf.X, lf.Y, lf.Z, lf.O) ;
	mathEvalPln(m_O, lf.X, lf.Y, -1.2*r, 1.2*r, lf.O) ;
	drawMove(lf.O, lf.X, lf.Y, w, w/10, c) ;
	glFlush() ;

	return ;
}

// nt add 2017/1/17
void CDirDlg::DrawZoom(void* pVI, int isOn)
{
	double w, r, d = viGetScale(pVI) ;
	COLORREF c ;
	RFRAME lf ;

	if( isOn )
	{
		c = RGB(200, 200, 200) ;
		m_zoom = 1 ;
	}
	else
	{
		c = RGB(255, 255, 255) ;
		m_zoom = 0 ;
	}
	r = 50*d ;
	w = r/2 ;
	viGetCenterLF(pVI, lf.X, lf.Y, lf.Z, lf.O) ;
	mathEvalPln(m_O, lf.X, lf.Y, 1.2*r, 1.2*r, lf.O) ;
	drawZoom2(lf.O, lf.X, lf.Y, w, c) ;
	glFlush() ;

	return ;
}

void CDirDlg::DrawDoc()
{
	if( m_pDoc )
		m_pDoc->Redraw() ;
	m_arrow = 0 ;
	m_pick = 0 ;

	return ;
}

// return 0,1,2,3,4 the same as rballPick(), return 5=move mark, 6=zoom mark
int  CDirDlg::Pick(double p[3], double v[3], void* pVI)
{
	double r, w, dx, dy, d = viGetScale(pVI) ;
	RFRAME lf ;

	int rt = rballPick(m_rb, p, v, 2.5*d) ;
	if( rt == 0 )
	{
		r = 50*d ;
		w = r/4 ;
		viGetCenterLF(pVI, lf.X, lf.Y, lf.Z, lf.O) ;
		mathEvalPln(m_O, lf.X, lf.Y, -1.2*r, 1.2*r, lf.O) ;
		dx = mathDistPntPln(p, lf.O, lf.X) ;
		dy = mathDistPntPln(p, lf.O, lf.Y) ;
		if( dx < w &&
			dy < w )
			return 5 ;
		mathEvalPln(m_O, lf.X, lf.Y, 1.2*r, 1.2*r, lf.O) ;
		dx = mathDistPntPln(p, lf.O, lf.X) ;
		dy = mathDistPntPln(p, lf.O, lf.Y) ;
		if( dx < w &&
			dy < w )
			return 6 ;
		return 0 ;
	}
	else
		return rt ;
}

int CDirDlg::OnLBD(UINT nFlags, double p[3], double v[3], void* pVI)
{
	double a ;
	PNT3D center ;
	VEC3D Z = {0., 0., -1.}, axis ;
	TRANS t ;

	m_lbd = 1 ;
	memcpy(m_mpos, p, sizeof(PNT3D)) ;
	if( m_mode == 0 ) // 处于交互旋转的状态, 检查是否可以确定旋转轴
	{
		if( m_move == 1 ) // nt add 2017/1/17
		{
			m_mode = 2 ;
			return 1 ;
		}
		else 
		if( m_zoom == 1 ) // nt add 2017/1/17
		{
			m_scale = 1. ; // nt add 2017/9/3 本次zoom的开始
			m_mode = 3 ;
			return 1 ;
		}
		
		m_v[0] = 0. ;
		m_v[1] = 0. ;
		m_v[2] = 0. ;
		if( m_axis == 'X' )
			m_v[0] = 1. ;
		else
		if( m_axis == 'Y' )
			m_v[1] = 1. ;
		else
		if( m_axis == 'Z' )
			m_v[2] = 1. ;
		m_axis2 = m_axis ;
		memcpy(m_pivot, m_O, sizeof(PNT3D)) ;
		m_a = 0. ;
	}
	else
	if( m_mode == 1 && // 处于交互确定底面的状态
		m_pick == 1 ) // 已经拾取了STL模型上一点，就执行零件旋转操作
	{
		stlGetCenter(m_stl, center) ;
		mathVProduct(m_normal, Z, axis) ;
		if( mathUniVec(axis, MIN_DBL) == ERSUCSS )
		{
			a = mathGetAngleUnit(m_normal, Z) ;
			stlRotate2(m_stl, center, axis, a) ; // nt modify 2017/3/30
			stlCalBox(m_stl) ; // nt add 2017/3/30
			t.type = 1 ;
			memcpy(t.p, center, sizeof(PNT3D)) ;
			memcpy(t.v, axis, sizeof(VEC3D)) ;
			t.a = a ;
			Add(&t) ;
			DrawDoc() ;
			m_arrow = 0 ;
			m_pick = 0 ; // nt add 2017/3/30
		}
	}

	return 1 ;
}

int CDirDlg::OnMM(UINT nFlags, double p[3], double v[3], void* pVI)
{
	if( m_mode == 0 )
	{
		if( m_lbd == 1 &&
			m_axis2 != ' ' )
		{
			double w, h, d, a ;
			VEC3D axis, temp ;
			m_pick = 0 ;
			temp[0] = p[0]-m_mpos[0] ;
			temp[1] = p[1]-m_mpos[1] ;
			temp[2] = p[2]-m_mpos[2] ;
			mathVProduct(temp, v, axis) ;
			viGetViewWidthHeight(pVI, &w, &h) ;
			d = mathVecLen(temp) ;
			if( d < MIN_DBL )
				return 0 ;
			a = 20000*d/(w+h) ;
			if( a < 1.5 )
				a = 1.5 ;
			if( a > 12.  )
				a = 12. ;
			mathUniVec(axis, MIN_DBL) ;
			if( fabs(m_v[0])+fabs(m_v[1])+fabs(m_v[2]) < 0.5 )
				memcpy(m_v, axis, sizeof(VEC3D)) ;
			else
			{
				if( mathOProduct(axis, m_v) < 0. )
					a = -a ;
			}
			Rotate(' ', m_v, a, 0) ; // 执行旋转但不保留undo信息
			m_a += a/180*PI1 ; // 累计旋转角度
		}
		else
		if( m_lbd == 0 )
		{
			int rt = Pick(p, v, pVI) ;
			DrawMove(pVI, 0) ;
			DrawZoom(pVI, 0) ;
			if( rt == 5 ) // move
				DrawMove(pVI, 1) ;
			else
			if( rt == 6 ) // zoom
				DrawZoom(pVI, 1) ;
			else
			{
				char axis = ' ' ;
				if( rt == 1 )
					axis = 'Z' ;
				else
				if( rt == 2 )
					axis = 'X' ;
				else
				if( rt == 3 )
					axis = 'Y' ; 
				if( axis == ' ' )
					ErazeAxis(pVI) ;
				else
				if( axis != m_axis )
				{
					ErazeAxis(pVI) ;
					DrawAxis(pVI, axis) ;
				}
				if( rt == 4 )
					m_axis = 4 ;
			}
		}
	}
	else
	if( m_mode == 1 )
	{
		double r = 2.5*viGetScale(pVI), z = MAX_DBL ;
		PNT3D q ;
		STLTRI* tri ;
		tri = stlPick(m_stl, p, v, r, q, &z) ;
		ErazeArrow(pVI) ;
		if( tri )
		{
			memcpy(m_center, q, sizeof(PNT3D)) ;
			memcpy(m_normal, tri->normal, sizeof(VEC3D)) ;
			m_pick = 1 ;
			DrawArrow(pVI) ;
		}
		else
			m_pick = 0 ;
	}
	else
	if( m_mode == 2 && // nt add 2017/1/17
		m_lbd == 1 )
	{
		VEC3D v1, v2 ;
		mathDecVecXY(m_mpos, p, v, v1) ;
		v1[2] = 0. ;
		v2[0] = v1[0]-m_vec[0] ;
		v2[1] = v1[1]-m_vec[1] ;
		v2[2] = 0. ;
		stlMove(m_stl, v2) ;
		m_O[0] += v2[0] ;
		m_O[1] += v2[1] ;
		memcpy(m_vec, v1, sizeof(VEC2D)) ;
		DrawDoc() ;
	}
	else
	if( m_mode == 3 && // nt add 2017/1/17
		m_lbd == 1 )
	{
		double d, w, h ;
		PNT3D q ;
		VEC3D vec, up ;
		viGetEyeCenterUp(pVI, q, q, up) ;
		mathPrjPntPln(m_mpos, p, v, q) ;
		vec[0] = p[0]-q[0] ;
		vec[1] = p[1]-q[1] ;
		vec[2] = p[2]-q[2] ;
		d = mathOProduct(up, vec) ;
		viGetViewWidthHeight(pVI, &w, &h) ;
		//m_scale = 1.+0.1*d/h ;
		double scale = 1.+0.1*d/h ; // nt add 2017/9/3
		memcpy(m_zc, m_O, sizeof(PNT3D)) ;
		//stlScale2(m_stl, m_zc, m_scale) ; // 比例缩放
		stlScale2(m_stl, m_zc, scale) ; // nt add 2017/9/3
		m_scale *= scale ; // nt add 2017/9/3
		DrawDoc() ;
	}
	else
		m_pick = 0 ;

	return 1 ;
}

int CDirDlg::OnRBD(UINT nFlags, double p[3], double v[3], void* pVI)
{
	m_mode = -1 ;
	DrawDoc() ;
	return 1 ;
}

int CDirDlg::OnLBU(UINT nFlags, double p[3], double v[3], void* pVI)
{
	m_lbd = 0 ;
	Push() ;
	if( m_mode == 2 || // nt add 2017/1/17
		m_mode == 3 )
		m_mode = 0 ;
	m_vec[0] = 0. ; // nt add 2017/1/17
	m_vec[1] = 0. ; // nt add 2017/1/17
	m_vec[2] = 0. ; // nt add 2017/1/17
	m_scale = 1. ; // nt add 2017/1/17
	m_zc[0] = 0. ; // nt add 2017/1/17
	m_zc[1] = 0. ; // nt add 2017/1/17
	m_zc[2] = 0. ; // nt add 2017/1/17
	DrawDoc() ;

	return 1 ;
}

int CDirDlg::OnReturn()
{
	return 1 ;
}

void CDirDlg::Notify(int msg)
{
	m_arrow = 0 ;

	return ;
}

// nt modify 2017/9/3
void CDirDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	/*m_mode = -1 ;
	m_cancel = 1 ; // nt add 2017/3/30
	DrawDoc() ; // nt modify 2017/3/30

	// nt add 2017/5/21
	stlCalBox(m_stl) ;
	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc )
		pDoc->CollisionDetect() ;

	CDlg::OnCancel();

	// nt add 2017/7/5
	if( m_pDoc )
		m_pDoc->ClearCmdAll() ;*/

	CDlg::OnCancel();

	m_pDoc->ClearCmdAll() ;
}


BOOL CDirDlg::OnInitDialog()
{
	CDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	EnableUndo(0) ; // nt add 2017/1/19

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
