// VLTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "CADDoc.h"
#include "VLTDlg.h"
#include "afxdialogex.h"


// CVLTDlg 对话框

IMPLEMENT_DYNAMIC(CVLTDlg, CDialogEx)

CVLTDlg::CVLTDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VLTDLG, pParent)
	, m_i0(0)
	, m_i1(0)
	, m_ratio(0)
{
	m_i0 = 1 ;
	m_i1 = 10 ;
	m_ratio = 0.5 ;
}

CVLTDlg::~CVLTDlg()
{
}

void CVLTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_i0);
	DDV_MinMaxInt(pDX, m_i0, 1, 10000);
	DDX_Text(pDX, IDC_EDIT2, m_i1);
	DDV_MinMaxInt(pDX, m_i1, 1, 10000);
	DDX_Text(pDX, IDC_EDIT3, m_ratio);
	DDV_MinMaxDouble(pDX, m_ratio, 0.125, 8);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CVLTDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CVLTDlg::OnClickedMfcbutton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CVLTDlg::OnClickedMfcbutton2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CVLTDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CVLTDlg::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CVLTDlg::OnDeltaposSpin3)
END_MESSAGE_MAP()

void CVLTDlg::AddString(int i0, int i1, double ratio)
{
	CString str ;
	if( i0 < i1 &&
		ratio > 0. )
	{
		str.Format(_T("起始层号=%d,终止层号=%d,层厚=%1.3lf倍"), 
			       i0, 
			       i1, 
			       ratio) ;
		m_list.AddString(str) ;
	}

	return ;
}

// CVLTDlg 消息处理程序
BOOL CVLTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc )
	{
		for( int i = 0 ; i < pDoc->m_nVLT ; i++ )
			AddString(pDoc->m_vlts[i][0],
				      pDoc->m_vlts[i][1],
				      0.001*pDoc->m_vlts[i][2]) ;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CVLTDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_i0 -= pNMUpDown->iDelta*10 ;
	if( m_i0 < 1 )
		m_i0 = 1 ;
	else
	if( m_i0 >= 10000 )
		m_i0 = 10000 ;
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CVLTDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_i1 -= pNMUpDown->iDelta*10 ;
	if( m_i1 < 1 )
		m_i1 = 1 ;
	else
	if( m_i1 >= 10000 )
		m_i1 = 10000 ;
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CVLTDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	if( pNMUpDown->iDelta < 0 )
		m_ratio *= 2. ;
	else
		m_ratio *= 0.5 ;
	if( m_ratio < 0.125 )
		m_ratio = 0.125 ;
	else
	if( m_ratio > 8. )
		m_ratio = 8. ;
    UpdateData(FALSE) ;
	*pResult = 0;
}

void CVLTDlg::OnClickedMfcbutton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// nt add 2019/3/24
	UpdateData(TRUE) ;
	if( m_i0 > m_i1 )
		goto ExceExit ;
	int i, n = m_list.GetCount(), a, b ;
	for( i = 0 ; i < n ; i++ )
	{
		CString text ;
		m_list.GetText(i, text) ;
		int k = text.Find('=') ;
		text.Delete(0, k+1) ;
		int i0 = _ttoi(text) ; // 读取i0
		k = text.Find('=') ;
		text.Delete(0, k+1) ;
		int i1 = _ttoi(text) ; // 读取i1
		a = mathMAX(i0, m_i0) ;
		b = mathMIN(i1, m_i1) ;
		if( a <= b )
			goto ExceExit ;
	}

	AddString(m_i0, m_i1, m_ratio) ;

	return ;
ExceExit:
	AfxMessageBox(_T("层号有冲突!")) ;
	return ;
}

void CVLTDlg::OnClickedMfcbutton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	int i = m_list.GetCurSel() ;
	if( i >= 0 )
	{
		m_list.DeleteString(i) ;
	}

	return ;
}

void CVLTDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc )
	{
		int n = m_list.GetCount() ;
		if( n > pDoc->m_maxVLT )
			n = pDoc->m_maxVLT ;
		for( int i = 0 ; i < n ; i++ )
		{
			CString text ;
			m_list.GetText(i, text) ;
			int k = text.Find('=') ;
			text.Delete(0, k+1) ;
			int i0 = _ttoi(text) ; // 读取i0
			k = text.Find('=') ;
			text.Delete(0, k+1) ;
			int i1 = _ttoi(text) ; // 读取i1
			k = text.Find('=') ;
			text.Delete(0, k+1) ;
			double ratio = _ttof(text) ; // 读取ratio
			pDoc->m_vlts[i][0] = i0 ;
			pDoc->m_vlts[i][1] = i1 ;
			pDoc->m_vlts[i][2] = (int)(ratio*1000+0.5) ;
		}
		pDoc->m_nVLT = n ;
	}

	CDialogEx::OnOK();
}
