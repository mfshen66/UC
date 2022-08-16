// SSPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CAD.h"
#include "SSPDlg.h"
#include "afxdialogex.h"


// CSSPDlg �Ի���

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

// CSSPDlg ��Ϣ�������
BOOL CSSPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_grid.EnableHeaderCtrl(TRUE, _T("����"), _T("ֵ")) ;
	m_grid.SetVSDotNetLook(TRUE);
	m_grid.EnableDescriptionArea();
	m_grid.MarkModifiedProperties();
	HDITEM item ;
	item.cxy = 240 ;
	item.mask = HDI_WIDTH ;
	m_grid.GetHeaderCtrl().SetItem(0, &item) ;

	CMFCPropertyGridProperty*pGroup, *pItem ;

	// ����
	pGroup = new CMFCPropertyGridProperty(_T("����")) ;
	pItem = new CMFCPropertyGridProperty(_T("�����߶�"), m_ssp.h, _T("��λ��mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("����б��"), m_ssp.a, _T("��λ����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// ֧��
	pGroup = new CMFCPropertyGridProperty(_T("֧��")) ;
	pItem = new CMFCPropertyGridProperty(_T("֧�Ŷ���뾶"), m_ssp.r, _T("��λ��mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("֧�Ŷ�׶�߶�"), m_ssp.d, _T("��λ��mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("֧�Ÿ˰뾶"), m_ssp.r2, _T("��λ��mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("֧�ŵ�׶�뾶"), m_ssp.r1, _T("��λ��mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�������浽�������߶�"), m_ssp.hPart, _T("��λ��mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// �ֲ�
	pGroup = new CMFCPropertyGridProperty(_T("�ֲ�")) ;
	pItem = new CMFCPropertyGridProperty(_T("���"), m_ssp.w, _T("��λ��mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�ܶ�"), (long)m_ssp.sDensity, _T("��λ��50-150%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// Ԥ������
	pGroup = new CMFCPropertyGridProperty(_T("Ԥ������")) ;
	CString str ;
	for( int k = 0 ; k < 5 ; k++ )
	{
		str.Format(_T("����%d"), k) ;
		pItem = new CMFCPropertyGridProperty(str, m_ssp.ts[k], _T("��λ��*")) ;
		pItem->AllowEdit(TRUE) ;
		pGroup->AddSubItem(pItem) ;
	}
	m_grid.AddProperty(pGroup) ;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CSSPDlg::GetPropertyValue(CMFCPropertyGridProperty* pProperty)
{
	CString name = pProperty->GetName() ;
	_variant_t v = pProperty->GetValue() ;
	if( name == _T("�����߶�") )
		m_ssp.h = v ;
	else
	if( name == _T("����б��") )
		m_ssp.a = v ;
	else
	if( name == _T("֧�Ŷ���뾶") )
		m_ssp.r = v ;
	else
	if( name == _T("֧�Ŷ�׶�߶�") )
		m_ssp.d = v ;
	else
	if( name == _T("֧�Ÿ˰뾶") )
		m_ssp.r2 = v ;
	else
	if( name == _T("֧�ŵ�׶�뾶") )
		m_ssp.r1 = v ;
	else
	if( name == _T("�������浽�������߶�") )
		m_ssp.hPart = v ;
	else
	if( name == _T("���") )
		m_ssp.w = v ;
	else
	if( name == _T("�ܶ�") )
		m_ssp.sDensity = v ;
	else
	if( name == _T("����0") )
		m_ssp.ts[0] = v ;
	else
	if( name == _T("����1") )
		m_ssp.ts[1] = v ;
	else
	if( name == _T("����2") )
		m_ssp.ts[2] = v ;
	else
	if( name == _T("����3") )
		m_ssp.ts[3] = v ;
	else
	if( name == _T("����4") )
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

	if( name == _T("�����߶�") )
		pProperty->SetValue(m_ssp.h) ;
	else
	if( name == _T("����б��") )
		pProperty->SetValue(m_ssp.a) ;
	else
	if( name == _T("֧�Ŷ���뾶") )
		pProperty->SetValue(m_ssp.r) ;
	else
	if( name == _T("֧�Ŷ�׶�߶�") )
		pProperty->SetValue(m_ssp.d) ;
	else
	if( name == _T("֧�Ÿ˰뾶") )
		pProperty->SetValue(m_ssp.r2) ;
	else
	if( name == _T("֧�ŵ�׶�뾶") )
		pProperty->SetValue(m_ssp.r1) ;
	else
	if( name == _T("�������浽�������߶�") )
		pProperty->SetValue(m_ssp.hPart) ;
	else
	if( name == _T("���") )
		pProperty->SetValue(m_ssp.w)  ;
	else
	if( name == _T("�ܶ�") )
		pProperty->SetValue((long)m_ssp.sDensity) ;
	else
	if( name == _T("����0") )
		pProperty->SetValue(m_ssp.ts[0]) ;
	else
	if( name == _T("����1") )
		pProperty->SetValue(m_ssp.ts[1]) ;
	else
	if( name == _T("����2") )
		pProperty->SetValue(m_ssp.ts[2]) ;
	else
	if( name == _T("����3") )
		pProperty->SetValue(m_ssp.ts[3]) ;
	else
	if( name == _T("����4") )
		pProperty->SetValue(m_ssp.ts[4]) ;

	return ;
}

// �ָ�Ĭ��ֵ nt add 2017/8/26
void CSSPDlg::OnClickedMfcbutton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ssp = m_ssp2 ;

	int i, j, m, n = m_grid.GetPropertyCount() ;
	CMFCPropertyGridProperty* pGroup, *pItem ;
	CString name ;
	for( i = 0 ; i < n ; i++ ) // ������һ��property
	{
		pGroup = m_grid.GetProperty(i) ;
		m = pGroup->GetSubItemsCount() ;
		for( j = 0 ; j < m ; j++ ) // �����ڶ���property
		{
			pItem = pGroup->GetSubItem(j) ;
			SetPropertyValue(pItem) ;
		}
	}

	return ;
}

// ��ΪĬ��ֵ nt add 2017/8/26
void CSSPDlg::OnClickedMfcbutton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SSP old = m_ssp ;

	int i, j, m, n = m_grid.GetPropertyCount() ;
	CMFCPropertyGridProperty* pGroup, *pItem ;
	CString name ;
	for( i = 0 ; i < n ; i++ ) // ������һ��property
	{
		pGroup = m_grid.GetProperty(i) ;
		m = pGroup->GetSubItemsCount() ;
		for( j = 0 ; j < m ; j++ ) // �����ڶ���property
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
	// TODO: �ڴ����ר�ô����/����û���
	int i, j, m, n = m_grid.GetPropertyCount() ;
	CMFCPropertyGridProperty* pGroup, *pItem ;
	CString name ;
	for( i = 0 ; i < n ; i++ ) // ������һ��property
	{
		pGroup = m_grid.GetProperty(i) ;
		m = pGroup->GetSubItemsCount() ;
		for( j = 0 ; j < m ; j++ ) // �����ڶ���property
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