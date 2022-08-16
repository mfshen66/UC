// SSPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "SSPDlg.h"
#include "afxdialogex.h"


// CSSPDlg 对话框

IMPLEMENT_DYNAMIC(CSSPDlg, CDialogEx)

CSSPDlg::CSSPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SSPDLG, pParent)
{
	sspInit(&m_ssp, 45., 2., 5., 0.3, 2., 1.2, 0.6, 75., 1.e-8) ;
}

CSSPDlg::~CSSPDlg()
{
}

void CSSPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_grid);
}

BEGIN_MESSAGE_MAP(CSSPDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CSSPDlg::OnClickedMfcbutton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CSSPDlg::OnClickedMfcbutton2)
	ON_BN_CLICKED(IDC_MFCBUTTON3, &CSSPDlg::OnClickedMfcbutton3)
END_MESSAGE_MAP()

// CSSPDlg 消息处理程序
BOOL CSSPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_grid.EnableHeaderCtrl(TRUE, _T("参数"), _T("值")) ;
	m_grid.SetVSDotNetLook(TRUE);
	m_grid.EnableDescriptionArea();
	m_grid.MarkModifiedProperties();
	HDITEM item ;
	item.cxy = 240 ;
	item.mask = HDI_WIDTH ;
	m_grid.GetHeaderCtrl().SetItem(0, &item) ;

	CMFCPropertyGridProperty*pGroup, *pItem ;

	// 底座
	pGroup = new CMFCPropertyGridProperty(_T("底座")) ;
	pItem = new CMFCPropertyGridProperty(_T("底座高度"), m_ssp.h, _T("单位：mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("底座斜度"), m_ssp.a, _T("单位：度")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// 支撑
	pGroup = new CMFCPropertyGridProperty(_T("支撑")) ;
	pItem = new CMFCPropertyGridProperty(_T("支撑顶点半径"), m_ssp.r, _T("单位：mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("支撑顶锥高度"), m_ssp.d, _T("单位：mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("支撑杆半径"), m_ssp.r2, _T("单位：mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("支撑底锥半径"), m_ssp.r1, _T("单位：mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("底座底面到零件底面高度"), m_ssp.hPart, _T("单位：mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// 分布
	pGroup = new CMFCPropertyGridProperty(_T("分布")) ;
	pItem = new CMFCPropertyGridProperty(_T("间距"), m_ssp.w, _T("单位：mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("密度"), (long)m_ssp.sDensity, _T("单位：50-150%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// 预留参数
	pGroup = new CMFCPropertyGridProperty(_T("预留参数")) ;
	CString str ;
	for( int k = 0 ; k < 5 ; k++ )
	{
		str.Format(_T("参数%d"), k) ;
		pItem = new CMFCPropertyGridProperty(str, m_ssp.ts[k], _T("单位：*")) ;
		pItem->AllowEdit(TRUE) ;
		pGroup->AddSubItem(pItem) ;
	}
	m_grid.AddProperty(pGroup) ;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CSSPDlg::GetPropertyValue(CMFCPropertyGridProperty* pProperty)
{
	CString name = pProperty->GetName() ;
	_variant_t v = pProperty->GetValue() ;
	if( name == _T("底座高度") )
		m_ssp.h = v ;
	else
	if( name == _T("底座斜度") )
		m_ssp.a = v ;
	else
	if( name == _T("支撑顶点半径") )
		m_ssp.r = v ;
	else
	if( name == _T("支撑顶锥高度") )
		m_ssp.d = v ;
	else
	if( name == _T("支撑杆半径") )
		m_ssp.r2 = v ;
	else
	if( name == _T("支撑底锥半径") )
		m_ssp.r1 = v ;
	else
	if( name == _T("底座底面到零件底面高度") )
		m_ssp.hPart = v ;
	else
	if( name == _T("间距") )
		m_ssp.w = v ;
	else
	if( name == _T("密度") )
		m_ssp.sDensity = v ;
	else
	if( name == _T("参数0") )
		m_ssp.ts[0] = v ;
	else
	if( name == _T("参数1") )
		m_ssp.ts[1] = v ;
	else
	if( name == _T("参数2") )
		m_ssp.ts[2] = v ;
	else
	if( name == _T("参数3") )
		m_ssp.ts[3] = v ;
	else
	if( name == _T("参数4") )
		m_ssp.ts[4] = v ;

	return ;
}

// nt add 2017/8/26
void CSSPDlg::SetPropertyValue(CMFCPropertyGridProperty* pProperty)
{
	CString name ;

	if( pProperty == NULL )
		return ;
	name = pProperty->GetName() ;

	if( name == _T("底座高度") )
		pProperty->SetValue(m_ssp.h) ;
	else
	if( name == _T("底座斜度") )
		pProperty->SetValue(m_ssp.a) ;
	else
	if( name == _T("支撑顶点半径") )
		pProperty->SetValue(m_ssp.r) ;
	else
	if( name == _T("支撑顶锥高度") )
		pProperty->SetValue(m_ssp.d) ;
	else
	if( name == _T("支撑杆半径") )
		pProperty->SetValue(m_ssp.r2) ;
	else
	if( name == _T("支撑底锥半径") )
		pProperty->SetValue(m_ssp.r1) ;
	else
	if( name == _T("底座底面到零件底面高度") )
		pProperty->SetValue(m_ssp.hPart) ;
	else
	if( name == _T("间距") )
		pProperty->SetValue(m_ssp.w)  ;
	else
	if( name == _T("密度") )
		pProperty->SetValue((long)m_ssp.sDensity) ;
	else
	if( name == _T("参数0") )
		pProperty->SetValue(m_ssp.ts[0]) ;
	else
	if( name == _T("参数1") )
		pProperty->SetValue(m_ssp.ts[1]) ;
	else
	if( name == _T("参数2") )
		pProperty->SetValue(m_ssp.ts[2]) ;
	else
	if( name == _T("参数3") )
		pProperty->SetValue(m_ssp.ts[3]) ;
	else
	if( name == _T("参数4") )
		pProperty->SetValue(m_ssp.ts[4]) ;

	return ;
}

// 恢复默认值 nt add 2017/8/26
void CSSPDlg::OnClickedMfcbutton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ssp = m_ssp2 ;

	int i, j, m, n = m_grid.GetPropertyCount() ;
	CMFCPropertyGridProperty* pGroup, *pItem ;
	CString name ;
	for( i = 0 ; i < n ; i++ ) // 遍历第一层property
	{
		pGroup = m_grid.GetProperty(i) ;
		m = pGroup->GetSubItemsCount() ;
		for( j = 0 ; j < m ; j++ ) // 遍历第二层property
		{
			pItem = pGroup->GetSubItem(j) ;
			SetPropertyValue(pItem) ;
		}
	}

	return ;
}

// 存为默认值 nt add 2017/8/26
void CSSPDlg::OnClickedMfcbutton2()
{
	// TODO: 在此添加控件通知处理程序代码
	SSP old = m_ssp ;

	int i, j, m, n = m_grid.GetPropertyCount() ;
	CMFCPropertyGridProperty* pGroup, *pItem ;
	CString name ;
	for( i = 0 ; i < n ; i++ ) // 遍历第一层property
	{
		pGroup = m_grid.GetProperty(i) ;
		m = pGroup->GetSubItemsCount() ;
		for( j = 0 ; j < m ; j++ ) // 遍历第二层property
		{
			pItem = pGroup->GetSubItem(j) ;
			GetPropertyValue(pItem) ;
		}
	}
	m_ssp2 = m_ssp ;
	m_ssp = old ;

	return ;
}

void CSSPDlg::OnClickedMfcbutton3()
{
	// TODO: 在此添加专用代码和/或调用基类
	int i, j, m, n = m_grid.GetPropertyCount() ;
	CMFCPropertyGridProperty* pGroup, *pItem ;
	CString name ;
	for( i = 0 ; i < n ; i++ ) // 遍历第一层property
	{
		pGroup = m_grid.GetProperty(i) ;
		m = pGroup->GetSubItemsCount() ;
		for( j = 0 ; j < m ; j++ ) // 遍历第二层property
		{
			pItem = pGroup->GetSubItem(j) ;
			GetPropertyValue(pItem) ;
		}
	}

	return ;
}

void CSSPDlg::OnOK()
{
	CDialogEx::OnOK();
}