// SeniorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "SeniorDlg.h"
#include "ParameterDlg.h"
#include "afxdialogex.h"
#include "VIPACC.h"

// CSeniorDlg 对话框

IMPLEMENT_DYNAMIC(CSeniorDlg, CDialogEx)

CSeniorDlg::CSeniorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENIORDLG, pParent)
	, m_li(0)
	, m_lt(0)
{
	m_li = 80 ;
	m_lt = 0. ;
}

CSeniorDlg::~CSeniorDlg()
{
}

void CSeniorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_li);
	DDV_MinMaxInt(pDX, m_li, 0, 100);
	DDX_Text(pDX, IDC_EDIT2, m_lt);
	DDV_MinMaxDouble(pDX, m_lt, 0, 10000);
}


BEGIN_MESSAGE_MAP(CSeniorDlg, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CSeniorDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CSeniorDlg::OnDeltaposSpin2)
	ON_BN_CLICKED(IDC_BUTTON1, &CSeniorDlg::OnClickedButton1)
END_MESSAGE_MAP()


// CSeniorDlg 消息处理程序


void CSeniorDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_li -= pNMUpDown->iDelta*5 ;
	if( m_li > 100 )
		m_li = 100 ;
	else
	if( m_li < 0 )
		m_li = 0 ;
    UpdateData(FALSE) ;
	*pResult = 0;
}

void CSeniorDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_lt -= pNMUpDown->iDelta*10. ;
	if( m_lt > 10000 )
		m_lt = 10000 ;
	else
	if( m_lt < 0 )
		m_lt = 0 ;
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CSeniorDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void CSeniorDlg::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	/*CParameterDlg dlg ;
	dlg.DoModal() ;*/
	VIPACC dlg;
	INT_PTR nResponse = dlg.DoModal();
	return ;
}
