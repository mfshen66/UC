// LayoutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "CADDoc.h"
#include "CADView.h"
#include "LayoutDlg.h"
#include "afxdialogex.h"


// CLayoutDlg 对话框

IMPLEMENT_DYNAMIC(CLayoutDlg, CDialogEx)

CLayoutDlg::CLayoutDlg(CWnd* pParent /*=NULL*/)
	: CDlg(IDD_LAYOUTDLG, pParent)
	, m_xspan(0)
	, m_xn(0)
	, m_yspan(0)
	, m_yn(0)
{
	m_xspan = 10. ;
	m_xn = 0 ;
	m_yspan = 10. ;
	m_yn = 0 ;
	m_pDoc = NULL ;
}

CLayoutDlg::~CLayoutDlg()
{
	Quit() ;

	// nt add 2017/7/3
	if( m_pDoc )
		m_pDoc->CollisionDetect() ;
}

void CLayoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_xspan);
	DDV_MinMaxDouble(pDX, m_xspan, -1000, 1000);
	DDX_Text(pDX, IDC_EDIT2, m_xn);
	DDV_MinMaxInt(pDX, m_xn, 0, 1000);
	DDX_Text(pDX, IDC_EDIT3, m_yspan);
	DDV_MinMaxDouble(pDX, m_yspan, -1000, 1000);
	DDX_Text(pDX, IDC_EDIT4, m_yn);
	DDV_MinMaxInt(pDX, m_yn, 0, 1000);
}


BEGIN_MESSAGE_MAP(CLayoutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLayoutDlg::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLayoutDlg::OnClickedButton2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CLayoutDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CLayoutDlg::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CLayoutDlg::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CLayoutDlg::OnDeltaposSpin4)
END_MESSAGE_MAP()

void CLayoutDlg::Init(CCADDoc* pDoc)
{
	m_pDoc = pDoc ;

	return ;
}

// CLayoutDlg 消息处理程序
void CLayoutDlg::EnableUndo(BOOL enable)
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON2) ;
	if( pButton )
		pButton->EnableWindow(enable) ;
	
	return ;
}

void CLayoutDlg::DrawDoc()
{
	if( m_pDoc )
		m_pDoc->Redraw() ;

	return ;
}

BOOL CLayoutDlg::OnInitDialog()
{
	CDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	EnableUndo(0) ;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLayoutDlg::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int i, j, k, n = 0 ;
	VEC3D v ;
	STL* stl, *copy ;
	CSet* pSet = new CSet() ;

	UpdateData(TRUE) ;

	if( m_pDoc == NULL )
	{
		delete pSet ;
		return ;
	}

	//n = m_pDoc->GetAllOfSelectedSTL2(pSet) ;
	n = m_pDoc->GetAllOfSelectedSTL(pSet) ; // nt add 2017/9/3
	if( n < 1 )
	{
		cadPromptStr(_T("拾取STL模型...")) ;
		delete pSet ;
		return ;
	}
	v[2] = 0. ;
	if( n > 0 &&
		(m_xn > 0 || 
		m_yn > 0) )
	{
		for( i = 0 ; i <= m_xn ; i++ )
		{
			v[0] = i*m_xspan ;
			for( j = 0 ; j <= m_yn ; j++ )
			{
				v[1] = j*m_yspan ;
				if( i > 0 || j > 0 )
				{
					for( k = 0 ; k < n ; k++ )
					{
						stl = (STL*)pSet->GetObj(k) ;
						if( stl )
						{
							copy = stlCopy(stl) ;
							stlMove(copy, v) ;
							m_pDoc->Add(copy) ;
						}
					}
				}
			}
		}
		m_createSTLRecs.AddInt(((m_xn+1)*(m_yn+1)-1)*n) ;
		EnableUndo(TRUE) ;
		DrawDoc() ;
	}

	delete pSet ;

	return ;
}


void CLayoutDlg::OnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	int n, nRec = m_createSTLRecs.GetNumOfInt() ;
	if( nRec > 0 ) // 有记录
	{
		n = m_createSTLRecs.GetInt(nRec-1) ;
		if( m_pDoc )
			m_pDoc->Remove(n) ;
		m_createSTLRecs.RemoveLast() ;
		nRec-- ;
		DrawDoc() ;
	}

	if( nRec < 1 )
		EnableUndo(0) ;

	return ;
}


void CLayoutDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_xspan -= pNMUpDown->iDelta*10; // mm
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CLayoutDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_xn -= pNMUpDown->iDelta; // 个
	if( m_xn < 0 )
		m_xn = 0 ;
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CLayoutDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_yspan -= pNMUpDown->iDelta*10; // mm
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CLayoutDlg::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_yn -= pNMUpDown->iDelta; // 个
	if( m_yn < 0 )
		m_yn = 0 ;
    UpdateData(FALSE) ;
	*pResult = 0;
}

int CLayoutDlg::OnLBD(UINT nFlags, double p[3], double v[3], void* pVI)
{
	if( m_pDoc )
	{
		m_pDoc->SetSelected(nFlags, pVI) ;
	}

	return 1 ;
}

int CLayoutDlg::OnMM(UINT nFlags, double p[3], double v[3], void* pVI)
{
	if( m_pDoc )
	{
		STLPCK pi ;
		memcpy(pi.pos, p, sizeof(PNT3D)) ;
		memcpy(pi.dir, v, sizeof(PNT3D)) ;
		pi.r = 2.5*viGetScale(pVI) ;
		pi.tri = NULL ;
		pi.stl = NULL ;
		pi.z = MAX_DBL ;
		m_pDoc->Pick(&pi) ;
		m_pDoc->SetPromptingSTL(pi.stl, pi.p, pi.tri, pVI) ;
	}

	return 1 ;
}


// nt add 2017/8/29
void CLayoutDlg::Quit()
{
	// TODO: 在此添加专用代码和/或调用基类
	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc )
	{
		// nt add 2017/9/3
		int i, nRec, n = 0 ;
		nRec = m_createSTLRecs.GetNumOfInt() ;
		for( i = 0 ; i < nRec ; i++ )
			n += m_createSTLRecs.GetInt(i) ;
		STL* stl = pDoc->m_stls ;
		for( i = 0 ; i < n ; i++ )
		{
			CRCreate* pR = new CRCreate(stl) ;
			pDoc->m_rs.Add(pR) ;
			pDoc->SetModifiedFlag(TRUE) ;
			stl = stl->next ;
		}
		// end
	}
	
	return ;
}


void CLayoutDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDlg::OnOK();

	// nt add 2017/7/5
	if( m_pDoc )
		m_pDoc->ClearCmdAll() ;

	return ;
}


// nt add 2017/8/29
void CLayoutDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDlg::OnCancel();

	// nt add 2017/7/5
	if( m_pDoc )
		m_pDoc->ClearCmdAll() ;

	return ;
}
