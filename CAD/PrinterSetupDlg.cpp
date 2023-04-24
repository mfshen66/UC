// PrinterSetupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "PrinterSetupDlg.h"
#include "SeniorDlg.h"
#include "afxdialogex.h"
#include "VLTDlg.h" // nt add 2017/6/1


// CPrinterSetupDlg 对话框

IMPLEMENT_DYNAMIC(CPrinterSetupDlg, CDialogEx)

CPrinterSetupDlg::CPrinterSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PRINTERSETUPDLG, pParent)
{
	m_nParameter = 0 ;
	m_parameters = NULL ;
	parameterInit(&m_cur) ;
	m_printer = _T("") ;
	m_color = _T("") ;
	m_material = _T("               ") ;
	m_h = 0.1 ;
	m_li = 80 ;
	m_lt = 0. ;
	m_constH = 1 ;
	m_pl = 20000;
	m_pw = 20000;
}

CPrinterSetupDlg::~CPrinterSetupDlg()
{
	if( m_parameters )
		delete []m_parameters ;
}

void CPrinterSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PL, m_pl);
	DDX_Text(pDX, IDC_EDIT_PW, m_pw);
}


BEGIN_MESSAGE_MAP(CPrinterSetupDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPrinterSetupDlg::OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CPrinterSetupDlg::OnSelchangeCombo3)
	ON_BN_CLICKED(IDC_BUTTON1, &CPrinterSetupDlg::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPrinterSetupDlg::OnClickedButton2)
END_MESSAGE_MAP()


// CPrinterSetupDlg 消息处理程序
void CPrinterSetupDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CButton* pB = (CButton*)GetDlgItem(IDC_RADIO1) ;
	m_constH = pB->GetCheck() ; // 0 or 1
	CComboBox* pCB ;
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO1) ;
	pCB->GetWindowText(m_printer);
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO2) ;
	pCB->GetWindowText(m_color);
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO3) ;
	pCB->GetWindowText(m_material);
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO4) ;
	CString h ;
	pCB->GetWindowText(h);
	m_h = 0.001*_tstof(h) ;

	CDialogEx::OnOK();
}

void CPrinterSetupDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}

// nt add 2017/9/30
int CPrinterSetupDlg::Load()
{
	CString filePath, fileName ;
	efpGet(filePath) ; // 运行文件目录
	filePath += _T("\\Libs\\P_M\\*.p_m") ;
	CFileFind find ;
	BOOL bWorking = find.FindFile(filePath) ;
	int n ;
	for( n = 0 ; bWorking ; n++ )
		bWorking = find.FindNextFile() ;
	if( m_parameters )
		delete []m_parameters ;
	m_parameters = new PARAMETER[n+1] ;
	bWorking = find.FindFile(filePath) ;
	PARAMETER parameter ;
	m_parameters[0] = m_cur ;
	m_nParameter = 0 ;
	while(bWorking)
	{
		bWorking = find.FindNextFile() ;
		fileName = find.GetFilePath() ;
		if( parameterLoad(&parameter, fileName) == 1 )
		{
			m_parameters[m_nParameter] = parameter ;
			m_nParameter++ ;
		}
	}
	// nt add 2017/9/30
	if( m_nParameter < 1 )
	{
		m_parameters[0] = m_cur ;
		m_nParameter = 1 ;
	}
	else
	{
		for( n = 0 ; n < m_nParameter ; n++ )
		{
			if( _tcscmp(m_cur.printer, m_parameters[n].printer) == 0 &&
				_tcscmp(m_cur.material, m_parameters[n].material) == 0 )
			{
				m_cur = m_parameters[n] ;
				break ;
			}
		}
		if( n >= m_nParameter )
			m_cur = m_parameters[0] ;
	}
	// end
	m_printer = m_cur.printer ;
	m_material = m_cur.material ;

	// 打印机
	int i, j, flag ;
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COMBO1) ;
	pCB->ResetContent() ;
	for( i = 0 ; i < m_nParameter ; i++ )
	{
		for( flag = 1, j = 0 ; j < i ; j++ )
		{
			if( _tcscmp(m_parameters[j].printer, m_parameters[i].printer) == 0 )
			{
				flag = 0 ;
				break ;
			}
		}
		if( flag )
			pCB->AddString(m_parameters[i].printer) ;
	}
	pCB->SelectString(0, m_cur.printer) ;

	// 材料
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO3) ;
	pCB->ResetContent() ;
	for( i = 0 ; i < m_nParameter ; i++ )
	{
		if( _tcscmp(m_cur.printer, m_parameters[i].printer) == 0 )
			pCB->AddString(m_parameters[i].material) ;
	}
	pCB->SelectString(0, m_cur.material) ;

	// 层厚
	j = 0 ;
	CString buf ;
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO4) ;
	pCB->ResetContent() ;
	for( i = 0 ; i < 6 ; i++ )
	{
		buf.Format(_T("%4.1lfμm"), m_cur.ts[i]) ;
		pCB->InsertString(i, buf) ;
		if( fabs(0.001*m_cur.ts[i]-m_h) < 0.001 )
			j = i ;
	}
	//pCB->SelectString(0, _T("100.0μm")) ;
	pCB->SetCurSel(j) ;

	return 1 ;
}

BOOL CPrinterSetupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	Load() ;

	// 颜色
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COMBO2) ;
	pCB->AddString(_T("黑色")) ;
	pCB->AddString(_T("白色")) ;
	pCB->AddString(_T("灰色")) ;
	pCB->AddString(_T("红色")) ;
	pCB->AddString(_T("绿色")) ;
	pCB->AddString(_T("蓝色")) ;
	pCB->AddString(_T("黄色")) ;
	pCB->AddString(_T("紫色")) ;
	pCB->SelectString(0, m_color) ;

	// 等、变厚度
	CButton* pB ;
	pB = (CButton*)GetDlgItem(IDC_RADIO1) ;
	pB->SetCheck(m_constH?1:0) ;
	pB = (CButton*)GetDlgItem(IDC_RADIO2) ;
	pB->SetCheck(m_constH?0:1) ;

	CString pixel;
	pixel.Format(_T("%d"), m_pl);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PL);
	pEdit->SetWindowText(pixel);

	pixel.Format(_T("%d"), m_pw);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PW);
	pEdit->SetWindowText(pixel);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 选择了打印机
void CPrinterSetupDlg::OnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* pCB ;
	int i ;

	pCB = (CComboBox*)GetDlgItem(IDC_COMBO1) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, m_printer) ; // get current
	// 材料
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO3) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, m_material) ; // get current
	pCB->ResetContent() ;
	for( i = 0 ; i < m_nParameter ; i++ )
	{
		if( _tcscmp(m_printer, m_parameters[i].printer) == 0 )
			pCB->AddString(m_parameters[i].material) ;
	}
	if( pCB->SelectString(0, m_material) == CB_ERR )
		pCB->SetCurSel(0) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, m_material) ; // update

	for( i = 0 ; i < m_nParameter ; i++ )
	{
		if( _tcscmp(m_printer, m_parameters[i].printer) == 0 &&
			_tcscmp(m_material, m_parameters[i].material) == 0 )
		{
			m_cur = m_parameters[i] ; // update
			break ;
		}
	}
	if( i >= m_nParameter )
		return ; // error

	// 层厚
	CString h, buf ;
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO4) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, h) ; // get current
	pCB->ResetContent() ;
	for( i = 0 ; i < 6 ; i++ )
	{
		buf.Format(_T("%4.1lfμm"), m_cur.ts[i]) ;
		pCB->AddString(buf) ;
	}
	if( pCB->SelectString(0, h) == CB_ERR )
		pCB->SetCurSel(4) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, h) ; // update
	m_h = 0.001*_tstof(h) ;

	return ;
}

// 选择了材料
void CPrinterSetupDlg::OnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* pCB ;
	int i ;

	pCB = (CComboBox*)GetDlgItem(IDC_COMBO1) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, m_printer) ; // get current
	// 材料
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO3) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, m_material) ; // get current

	for( i = 0 ; i < m_nParameter ; i++ )
	{
		if( _tcscmp(m_printer, m_parameters[i].printer) == 0 &&
			_tcscmp(m_material, m_parameters[i].material) == 0 )
		{
			m_cur = m_parameters[i] ; // update
			break ;
		}
	}
	if( i >= m_nParameter )
		return ; // error

	// 层厚
	CString h, buf ;
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO4) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, h) ; // get current
	pCB->ResetContent() ;
	for( i = 0 ; i < 6 ; i++ )
	{
		buf.Format(_T("%4.1lfμm"), m_cur.ts[i]) ;
		pCB->AddString(buf) ;
	}
	if( pCB->SelectString(0, h) == CB_ERR )
		pCB->SetCurSel(4) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, h) ; // update
	m_h = 0.001*_tstof(h) ;

	return ;
}

// 高级
void CPrinterSetupDlg::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	

	
	CSeniorDlg dlg ;
	dlg.m_li = m_li ;
	dlg.m_lt = m_lt ;
	if( dlg.DoModal() == IDOK )
	{
		m_li = dlg.m_li ;
		m_lt = dlg.m_lt ;
	}
	Load() ; // nt add 2017/9/30
	UpdateData(FALSE) ; // nt add 2017/9/30
	
	return ;
}


void CPrinterSetupDlg::OnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CVLTDlg dlg ;
	dlg.DoModal() ;

	return ;
}
