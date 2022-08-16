// SSDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CAD.h"
#include "CADDoc.h"
#include "SSDlg.h"
#include "SSPDlg.h"
#include "afxdialogex.h"


// CSSDlg �Ի���

IMPLEMENT_DYNAMIC(CSSDlg, CDialogEx)

CSSDlg::CSSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SSDLG, pParent)
	, m_density(0)
	, m_r(0)
{
	m_density = 1. ;
	m_densityMin = 0.5 ;
	m_densityMax = 1.5 ;
	m_densityD = 1. ;
	m_r = 0.6 ;
	m_rMin = 0.05 ;
	m_rMax = 2.5 ;
	m_rD = 0.3 ;

	// nt add 2017/9/10
	m_prg.ws = NULL ;
}

CSSDlg::~CSSDlg()
{
	// nt add 2017/9/9
	prgReleaseBuf(&m_prg) ;
}

void CSSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_density);
	DDV_MinMaxDouble(pDX, m_density, 0., 2.);
	DDX_Text(pDX, IDC_EDIT2, m_r);
	DDV_MinMaxDouble(pDX, m_r, 0.1, 10.);
}


BEGIN_MESSAGE_MAP(CSSDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CSSDlg::OnClickedMfcbutton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CSSDlg::OnClickedMfcbutton2)
	ON_BN_CLICKED(IDC_MFCBUTTON3, &CSSDlg::OnClickedMfcbutton3)
	ON_BN_CLICKED(IDC_MFCBUTTON4, &CSSDlg::OnClickedMfcbutton4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CSSDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CSSDlg::OnDeltaposSpin2)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSSDlg ��Ϣ�������
BOOL CSSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc == NULL )
		return FALSE ;
	m_r = pDoc->m_ssp.r ;
	m_density = 0.01*pDoc->m_ssp.sDensity ;

	if( m_densityMax-m_densityMin < 0.01 ||
		m_rMax-m_rMin < 0.01 )
		return FALSE ;
	ResDensity() ;
	ResR() ;

	CSliderCtrl* pSlider1 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1) ;
	pSlider1->SetRange(0, 100, TRUE) ; // ���û�������Χ
	UpdateSlider1() ;
	CSliderCtrl* pSlider2 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2) ;
	pSlider2->SetRange(0, 100, TRUE) ; // ���û�������Χ
	UpdateSlider2() ;

	CMFCButton* pB = (CMFCButton*)GetDlgItem(IDC_MFCBUTTON1) ;
	pB->SetFaceColor(RGB(100,255,255)) ;
	pB->m_bTransparent = FALSE ;
	pB->m_bDontUseWinXPTheme = TRUE ;

	SetTimer(0, 1000, NULL) ; // nt add 2017/9/10

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CSSDlg::ResDensity()
{
	if( m_density > m_densityMax )
		m_density = m_densityMax ;
	else
	if( m_density < m_densityMin )
		m_density = m_densityMin ;

	return ;
}

void CSSDlg::ResR()
{
	if( m_r > m_rMax )
		m_r = m_rMax ;
	else
	if( m_r < m_rMin )
		m_r = m_rMin ;

	return ;
}

void CSSDlg::UpdateSlider1()
{
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1) ;
	int d = (int)((m_density-m_densityMin)/(m_densityMax-m_densityMin)*100) ;
	pSlider->SetPos(d) ; // ���û�����λ��

	return ;
}

void CSSDlg::UpdateSlider2()
{
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2) ;
	int d = (int)((m_r-m_rMin)/(m_rMax-m_rMin)*100) ;
	pSlider->SetPos(d) ; // ���û�����λ��

	return ;
}

void CSSDlg::OnClickedMfcbutton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CProgressCtrl* pCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1) ;
	if( pCtrl )
		pCtrl->SetPos(0) ;

	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc )
	{
		SSP sspOld ;
		sspOld = pDoc->m_ssp ;
		pDoc->m_ssp.r = m_r ;
		pDoc->m_ssp.w *= (m_density<1.?(3.-2*m_density):(2.-m_density)) ;
		pDoc->MakeSupport(&m_prg) ;
		pDoc->m_ssp = sspOld ; // ���ı�Doc��ssp
	}

	return ;
}

// nt modify 2017/8/24
void CSSDlg::OnClickedMfcbutton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( AfxMessageBox(_T("�������֧�ţ�"), MB_YESNO) == IDYES )
	{
		CCADDoc* pDoc = cadGetDoc() ;
		if( pDoc )
			pDoc->ClearSupport() ;
	}

	return ;
}


void CSSDlg::OnClickedMfcbutton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc )
	{
		CSSPDlg dlg ;
		dlg.m_ssp = pDoc->m_ssp ;
		dlg.m_ssp2 = pDoc->m_ssp2 ; // nt add 2017/8/26
		if( dlg.DoModal() == IDOK )
		{
			pDoc->m_ssp = dlg.m_ssp ;
			m_r = dlg.m_ssp.r ; // nt add 2017/8/26 ����ͬ��
			m_density = 0.01*dlg.m_ssp.sDensity ; // nt add 2017/8/26 ����ͬ��
			pDoc->m_ssp2 = dlg.m_ssp2 ; // nt add 2017/8/26
			UpdateSlider1() ; // nt add 2017/8/26
			UpdateSlider2() ; // nt add 2017/8/26
			UpdateData(FALSE) ; // nt add 2017/8/26
		}
	}

	return ;
}

// �ָ�Ĭ��ֵ
void CSSDlg::OnClickedMfcbutton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_density = m_densityD ;
	m_r = m_rD ;
	UpdateSlider1() ;
	UpdateSlider2() ;
	UpdateData(FALSE) ;

	return ;
}


void CSSDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE) ;
	m_density -= pNMUpDown->iDelta*0.1 ;
	ResDensity() ;
	UpdateSlider1() ;
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CSSDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE) ;
	m_r -= pNMUpDown->iDelta*0.05 ;
	ResR() ;
	UpdateSlider2() ;
    UpdateData(FALSE) ;
	*pResult = 0;
}


void CSSDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CSliderCtrl* pSlider = NULL ;
	int pos ;

	switch(pScrollBar->GetDlgCtrlID())
    {
    case IDC_SLIDER1:
		pSlider =(CSliderCtrl*)GetDlgItem(IDC_SLIDER1) ;
		pos = pSlider->GetPos() ; // ȡ�õ�ǰλ��ֵ
		m_density = m_densityMin+(m_densityMax-m_densityMin)*pos/100 ;
		ResDensity() ;
		break ;
	case IDC_SLIDER2:
		pSlider =(CSliderCtrl*)GetDlgItem(IDC_SLIDER2) ;
		pos = pSlider->GetPos() ; // ȡ�õ�ǰλ��ֵ
		m_r = m_rMin+(m_rMax-m_rMin)*pos/100 ;
		ResR() ;
		break ;
	default:
		break ;
	}
	UpdateData(FALSE) ;

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CSSDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}

void CSSDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int nPos = prgGetPercent(&m_prg) ;
	CProgressCtrl* pCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1) ;
	if( pCtrl )
		pCtrl->SetPos(nPos) ;

	CDialogEx::OnTimer(nIDEvent);
}