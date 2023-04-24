// PrinterSetupDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CAD.h"
#include "PrinterSetupDlg.h"
#include "SeniorDlg.h"
#include "afxdialogex.h"
#include "VLTDlg.h" // nt add 2017/6/1


// CPrinterSetupDlg �Ի���

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


// CPrinterSetupDlg ��Ϣ�������
void CPrinterSetupDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}

// nt add 2017/9/30
int CPrinterSetupDlg::Load()
{
	CString filePath, fileName ;
	efpGet(filePath) ; // �����ļ�Ŀ¼
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

	// ��ӡ��
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

	// ����
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO3) ;
	pCB->ResetContent() ;
	for( i = 0 ; i < m_nParameter ; i++ )
	{
		if( _tcscmp(m_cur.printer, m_parameters[i].printer) == 0 )
			pCB->AddString(m_parameters[i].material) ;
	}
	pCB->SelectString(0, m_cur.material) ;

	// ���
	j = 0 ;
	CString buf ;
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO4) ;
	pCB->ResetContent() ;
	for( i = 0 ; i < 6 ; i++ )
	{
		buf.Format(_T("%4.1lf��m"), m_cur.ts[i]) ;
		pCB->InsertString(i, buf) ;
		if( fabs(0.001*m_cur.ts[i]-m_h) < 0.001 )
			j = i ;
	}
	//pCB->SelectString(0, _T("100.0��m")) ;
	pCB->SetCurSel(j) ;

	return 1 ;
}

BOOL CPrinterSetupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	Load() ;

	// ��ɫ
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COMBO2) ;
	pCB->AddString(_T("��ɫ")) ;
	pCB->AddString(_T("��ɫ")) ;
	pCB->AddString(_T("��ɫ")) ;
	pCB->AddString(_T("��ɫ")) ;
	pCB->AddString(_T("��ɫ")) ;
	pCB->AddString(_T("��ɫ")) ;
	pCB->AddString(_T("��ɫ")) ;
	pCB->AddString(_T("��ɫ")) ;
	pCB->SelectString(0, m_color) ;

	// �ȡ�����
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
				  // �쳣: OCX ����ҳӦ���� FALSE
}

// ѡ���˴�ӡ��
void CPrinterSetupDlg::OnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox* pCB ;
	int i ;

	pCB = (CComboBox*)GetDlgItem(IDC_COMBO1) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, m_printer) ; // get current
	// ����
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

	// ���
	CString h, buf ;
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO4) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, h) ; // get current
	pCB->ResetContent() ;
	for( i = 0 ; i < 6 ; i++ )
	{
		buf.Format(_T("%4.1lf��m"), m_cur.ts[i]) ;
		pCB->AddString(buf) ;
	}
	if( pCB->SelectString(0, h) == CB_ERR )
		pCB->SetCurSel(4) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, h) ; // update
	m_h = 0.001*_tstof(h) ;

	return ;
}

// ѡ���˲���
void CPrinterSetupDlg::OnSelchangeCombo3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox* pCB ;
	int i ;

	pCB = (CComboBox*)GetDlgItem(IDC_COMBO1) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, m_printer) ; // get current
	// ����
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

	// ���
	CString h, buf ;
	pCB = (CComboBox*)GetDlgItem(IDC_COMBO4) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, h) ; // get current
	pCB->ResetContent() ;
	for( i = 0 ; i < 6 ; i++ )
	{
		buf.Format(_T("%4.1lf��m"), m_cur.ts[i]) ;
		pCB->AddString(buf) ;
	}
	if( pCB->SelectString(0, h) == CB_ERR )
		pCB->SetCurSel(4) ;
	if( (i = pCB->GetCurSel()) >= 0 )
		pCB->GetLBText(i, h) ; // update
	m_h = 0.001*_tstof(h) ;

	return ;
}

// �߼�
void CPrinterSetupDlg::OnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

	
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CVLTDlg dlg ;
	dlg.DoModal() ;

	return ;
}
