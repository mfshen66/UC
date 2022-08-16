// SizeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "CADDoc.h"
#include "SizeDlg.h"
#include "afxdialogex.h"


// CSizeDlg 对话框

IMPLEMENT_DYNAMIC(CSizeDlg, CDialogEx)

CSizeDlg::CSizeDlg(CWnd* pParent /*=NULL*/)
	: CDlg(IDD_SIZEDLG, pParent)
	, m_x(100)
	, m_y(100)
	, m_z(100)
	, m_s(100)
{
	m_x = 0. ;
	m_y = 0. ;
	m_z = 0. ;
	m_s = 100. ;
	m_realX = 0. ;
	m_realY = 0. ;
	m_realZ = 0. ;
	m_realS = 100. ;
	m_stl = NULL ;
	m_center[0] = 0. ;
	m_center[1] = 0. ;
	m_center[2] = 0. ;
}

CSizeDlg::~CSizeDlg()
{
	// nt add 2017/7/3
	if( m_pDoc )
		m_pDoc->CollisionDetect() ;
}

void CSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_x);
	DDV_MinMaxDouble(pDX, m_x, 0.001, 10000);
	DDX_Text(pDX, IDC_EDIT2, m_y);
	DDV_MinMaxDouble(pDX, m_y, 0.001, 10000);
	DDX_Text(pDX, IDC_EDIT3, m_z);
	DDV_MinMaxDouble(pDX, m_z, 0.001, 10000);
	DDX_Text(pDX, IDC_EDIT4, m_s);
	DDV_MinMaxDouble(pDX, m_s, 1, 10000);
}

BEGIN_MESSAGE_MAP(CSizeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSizeDlg::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSizeDlg::OnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSizeDlg::OnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSizeDlg::OnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSizeDlg::OnClickedButton5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CSizeDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CSizeDlg::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CSizeDlg::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CSizeDlg::OnDeltaposSpin4)
END_MESSAGE_MAP()

void CSizeDlg::Init(STL* stl, CCADDoc* pDoc)
{
	if( stl )
	{
		BOX3D box ;
		stlGetCenter(stl, m_center) ;
		stlGetBox(stl, &box) ;
		m_realX = box.max[0]-box.min[0] ;
		m_realY = box.max[1]-box.min[1] ;
		m_realZ = box.max[2]-box.min[2] ;
		m_realS = 100. ;
		m_x = roundOff(m_realX, 3) ; // nt add 2017/8/24
		m_y = roundOff(m_realY, 3) ; // nt add 2017/8/24
		m_z = roundOff(m_realZ, 3) ; // nt add 2017/8/24
		m_s = 100. ;
		m_stl = stl ;
		m_pDoc = pDoc ;
	}

	return ;
}

void CSizeDlg::Scale(double scale)
{
	if( m_stl != NULL &&
		m_pDoc != NULL &&
		fabs(scale-1.) > MIN_DBL )
	{
		stlScale2(m_stl, m_center, scale) ;

		// update parmeters
		m_realX *= scale ;
		m_realY *= scale ;
		m_realZ *= scale ;
		m_realS *= scale ;
		m_x = roundOff(m_realX, 3) ;
		m_y = roundOff(m_realY, 3) ;
		m_z = roundOff(m_realZ, 3) ;
		m_s = roundOff(m_realS, 3) ;
		UpdateData(FALSE) ;
		m_pDoc->Update() ; // nt add 2017/8/27
		m_pDoc->Redraw() ;
	}

	return ;
}

// CSizeDlg 消息处理程序
void CSizeDlg::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	double xOld = roundOff(m_realX, 3) ;
	if( fabs(m_x-xOld) > 0.001 && // 有一定量的变化
		m_x > MIN_LEN &&
		m_realX > MIN_LEN )
	{
		double scale = ((m_x-xOld)+m_realX)/m_realX ;
		Scale(scale) ;
		EnableUndo(TRUE) ; // nt add 2017/8/24
	}

	return ;
}

void CSizeDlg::OnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	double yOld = roundOff(m_realY, 3) ;
	if( fabs(m_y-yOld) > 0.001 && // 有一定量的变化
		m_y > MIN_LEN &&
		m_realY > MIN_LEN )
	{
		double scale = ((m_y-yOld)+m_realY)/m_realY ;
		Scale(scale) ;
		EnableUndo(TRUE) ; // nt add 2017/8/24
	}

	return ;
}


void CSizeDlg::OnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	double zOld = roundOff(m_realZ, 3) ;
	if( fabs(m_z-zOld) > 0.001 && // 有一定量的变化
		m_z > MIN_LEN &&
		m_realZ > MIN_LEN )
	{
		double scale = ((m_z-zOld)+m_realZ)/m_realZ ;
		Scale(scale) ;
		EnableUndo(TRUE) ; // nt add 2017/8/24
	}

	return ;
}


void CSizeDlg::OnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	double sOld = roundOff(m_realS, 3) ;
	if( fabs(m_s-sOld) > 0.001 && // 有一定量的变化
		m_s > MIN_DBL &&
		m_realS > MIN_DBL )
	{
		double scale = ((m_s-sOld)+m_realS)/m_realS ;
		Scale(scale) ;
		EnableUndo(TRUE) ; // nt add 2017/8/24
	}

	return ;
}


void CSizeDlg::OnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_realS > MIN_DBL )
	{
		double scale = 100./m_realS ;
		Scale(scale) ;
		EnableUndo(FALSE) ; // nt add 2017/8/24
	}
}


void CSizeDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_x -= pNMUpDown->iDelta*1. ; // mm
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CSizeDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_y -= pNMUpDown->iDelta*1. ; // mm
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CSizeDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_z -= pNMUpDown->iDelta*1. ; // mm
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CSizeDlg::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_s -= pNMUpDown->iDelta*10. ; // %
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CSizeDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc )
	{
		if( fabs(m_realS-100.) > MIN_DBL ) // nt add 2017/9/3
		{
			CRSize* pR = new CRSize(m_stl, m_center, m_realS) ;
			pDoc->m_rs.Add(pR) ;
			pDoc->SetModifiedFlag(TRUE) ;
		}
		pDoc->CollisionDetect() ;
		pDoc->Update() ; // nt add 2017/9/3
	}

	CDlg::OnCancel();

	// nt add 2017/7/5
	if( m_pDoc )
		m_pDoc->ClearCmdAll() ;
}

// nt add 2017/8/24
void CSizeDlg::EnableUndo(BOOL enable)
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON5) ;
	if( pButton )
		pButton->EnableWindow(enable) ;
	
	return ;
}