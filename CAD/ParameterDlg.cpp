// ParameterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CAD.h"
#include "CADDoc.h" // nt add 2017/8/19
#include "ParameterDlg.h"
#include "afxdialogex.h"


// CParameterDlg �Ի���

IMPLEMENT_DYNAMIC(CParameterDlg, CDialogEx)

CParameterDlg::CParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PARAMETERDLG, pParent)
{
	parameterInit(&m_parameter) ;
	m_pathName = _T("") ;
}

CParameterDlg::~CParameterDlg()
{
}

void CParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_grid);
}


BEGIN_MESSAGE_MAP(CParameterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CParameterDlg::OnClickedMfcbutton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CParameterDlg::OnClickedMfcbutton2)
	ON_BN_CLICKED(IDC_MFCBUTTON3, &CParameterDlg::OnClickedMfcbutton3)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

void CParameterDlg::InitProperty()
{
	CMFCPropertyGridProperty* pGroup, *pItem ;

	// ��ӡ��
	pGroup = new CMFCPropertyGridProperty(_T("��ӡ��")) ;
	pItem = new CMFCPropertyGridProperty(_T("��ӡ���ͺ�"), m_parameter.printer, _T("�ַ���")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("��ӡ����X��ߴ�"), m_parameter.L, _T("������")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("��ӡ����Y��ߴ�"), m_parameter.W, _T("������")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("��ӡ����Z��ߴ�"), m_parameter.H, _T("������")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("��ӡX��ֱ���"), (long)m_parameter.PL, _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("��ӡY��ֱ���"), (long)m_parameter.PW, _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���Ͷ��ԭ�㶨��"), (long)m_parameter.P0, _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���Ͷ��X'�᷽����"), (long)m_parameter.Dx, _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// ����
	pGroup = new CMFCPropertyGridProperty(_T("����")) ;
	pItem = new CMFCPropertyGridProperty(_T("�����ͺ�"), m_parameter.material, _T("�ַ���")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("��һ����Ƭ���t0"), m_parameter.ts[0], _T("��m")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("��С���t1"), m_parameter.ts[1], _T("��m")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t2"), m_parameter.ts[2], _T("��m")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t3"), m_parameter.ts[3], _T("��m")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t4"), m_parameter.ts[4], _T("��m")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�����t5"), m_parameter.ts[5], _T("��m")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t0�ع��ǿ"), (long)m_parameter.lis[0], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t1�ع��ǿ"), (long)m_parameter.lis[1], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t2�ع��ǿ"), (long)m_parameter.lis[2], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t3�ع��ǿ"), (long)m_parameter.lis[3], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t4�ع��ǿ"), (long)m_parameter.lis[4], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t5�ع��ǿ"), (long)m_parameter.lis[5], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	// nt add 2017/6/27
	pItem = new CMFCPropertyGridProperty(_T("���t1С��ǿ�ع��ǿ"), (long)m_parameter.lils[0], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t2С��ǿ�ع��ǿ"), (long)m_parameter.lils[1], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t3С��ǿ�ع��ǿ"), (long)m_parameter.lils[2], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t4С��ǿ�ع��ǿ"), (long)m_parameter.lils[3], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t5С��ǿ�ع��ǿ"), (long)m_parameter.lils[4], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	// end
	pItem = new CMFCPropertyGridProperty(_T("���t0�ع�ʱ��"), m_parameter.lts[0], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t1�ع�ʱ��"), m_parameter.lts[1], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t2�ع�ʱ��"), m_parameter.lts[2], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t3�ع�ʱ��"), m_parameter.lts[3], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t4�ع�ʱ��"), m_parameter.lts[4], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t5�ع�ʱ��"), m_parameter.lts[5], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	// nt add 2017/6/27
	pItem = new CMFCPropertyGridProperty(_T("���t1С��ǿ�ع�ʱ��"), m_parameter.ltls[0], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t2С��ǿ�ع�ʱ��"), m_parameter.ltls[1], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t3С��ǿ�ع�ʱ��"), m_parameter.ltls[2], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t4С��ǿ�ع�ʱ��"), m_parameter.ltls[3], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���t5С��ǿ�ع�ʱ��"), m_parameter.ltls[4], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	// end
	m_grid.AddProperty(pGroup) ;

	// һ�ŵ��
	pGroup = new CMFCPropertyGridProperty(_T("һ�ŵ��")) ;
	pItem = new CMFCPropertyGridProperty(_T("���ת��1"), m_parameter.w1max, _T("rpm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���ת��1"), m_parameter.w1min, _T("rpm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�����ٶ�"), m_parameter.w1p, _T("rpm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("������ʼ����"), m_parameter.L1, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���ٽ�������"), m_parameter.L2, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("��ӡ��������ֹͣ����"), m_parameter.L3, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// ���ŵ��
	pGroup = new CMFCPropertyGridProperty(_T("���ŵ��")) ;
	pItem = new CMFCPropertyGridProperty(_T("���ת��2"), m_parameter.w2max, _T("rpm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���ת��2"), m_parameter.w2min, _T("rmp")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("ת���Ƕ�"), m_parameter.beta, _T("deg")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// �ҶȽ���
	pGroup = new CMFCPropertyGridProperty(_T("�ҶȽ���")) ;
	pItem = new CMFCPropertyGridProperty(_T("���ص�1�Ҷ�"), (long)m_parameter.grays[0], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���ص�2�Ҷ�"), (long)m_parameter.grays[1], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���ص�3�Ҷ�"), (long)m_parameter.grays[2], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���ص�4�Ҷ�"), (long)m_parameter.grays[3], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// �߶�
	pItem = new CMFCPropertyGridProperty(_T("��ӡƽ̨ȱʡ�߶�"), m_parameter.H0, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;

	// �Ҷ���С�ɴ���߶�, nt add 2017/6/1
	pItem = new CMFCPropertyGridProperty(_T("�Ҷ���С�ɴ���߶�"), (long)m_parameter.wp, _T("��m")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;

	// nt add 2017/6/27
	pItem = new CMFCPropertyGridProperty(_T("��е���"), m_parameter.LDdelta, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("��դ�ߵ�ǰλ��"), m_parameter.LD0, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�������ϵ��"), m_parameter.LNF, _T("ȱʡֵ0.9")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("Ͷ�����"), m_parameter.LF, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�����X���"), m_parameter.LENx, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�����Y���"), m_parameter.LENy, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�Ϻ�������"), m_parameter.n, _T("ȱʡֵ1.5")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�Ϻк��"), m_parameter.tmc, _T("ȱʡֵ10.mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�Ϻг��ּ��ٱ�"), m_parameter.ratio, _T("ȱʡֵ7.5")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	//end

	// Ԥ��
	pGroup = new CMFCPropertyGridProperty(_T("Ԥ��")) ;
	pItem = new CMFCPropertyGridProperty(_T("�������0"), m_parameter.tps[0], _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�������1"), m_parameter.tps[1], _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�������2"), m_parameter.tps[2], _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�������3"), m_parameter.tps[3], _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("�������4"), m_parameter.tps[4], _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���Ͳ���0"), (long)m_parameter.tis[0], _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���Ͳ���1"), (long)m_parameter.tis[1], _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���Ͳ���2"), (long)m_parameter.tis[2], _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���Ͳ���3"), (long)m_parameter.tis[3], _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("���Ͳ���4"), (long)m_parameter.tis[4], _T("����")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	return ;
}

// CParameterDlg ��Ϣ�������
BOOL CParameterDlg::OnInitDialog()
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

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

// �򿪴�ӡ�����ϲ����ļ�
void CParameterDlg::OnClickedMfcbutton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE);
    dlg.m_ofn.lpstrFilter = _T("P_M�ļ�(*.p_m)") ;
    dlg.m_ofn.lpstrTitle = _T("�����ļ�") ;
	CString filePath ;
	efpGet(filePath) ; // �����ļ�Ŀ¼
	filePath += _T("\\Libs\\P_M") ;
	dlg.m_ofn.lpstrInitialDir = filePath.GetBuffer(0) ;
	if( dlg.DoModal() == IDOK )
	{
		m_pathName = dlg.GetPathName() ;
		PARAMETER parameter ;
		if( parameterLoad(&parameter, m_pathName) == 1 )
		{
			m_parameter = parameter ;
			if( m_grid.GetPropertyCount() > 0 )
				ResetAllProperty() ;
			else
				InitProperty() ;
		}
		else
			AfxMessageBox(_T("���ļ�����!")) ;
	}

	return ;
}

// �����ӡ�����ϲ����ļ�
void CParameterDlg::OnClickedMfcbutton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCADDoc* pDoc = cadGetDoc() ; // nt add 2017/8/19
	if( m_pathName != _T("") )
	{
		parameterResByLimits(&m_parameter) ; // nt add 2017/8/20
		parameterSave(&m_parameter, m_pathName) ;
		// nt add 2017/8/19
		if( _tcscmp(m_parameter.printer, pDoc->m_parameter.printer) == 0 &&
			_tcscmp(m_parameter.material, pDoc->m_parameter.material) == 0 )
			pDoc->m_parameter = m_parameter ;
		// end
	}
	else
		AfxMessageBox(_T("�ȴ򿪲��������ļ�!")) ;
}

// ����ӡ�����ϲ����ļ�
void CParameterDlg::OnClickedMfcbutton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( m_pathName != _T("") )
	{
		CFileDialog dlg(FALSE) ;
		dlg.m_ofn.lpstrFilter = _T("P_M�ļ�(*.p_m)") ;
		dlg.m_ofn.lpstrTitle = _T("�����ļ�") ;
		if( dlg.DoModal() == IDOK )
		{
			m_pathName = dlg.GetPathName() ;
			parameterResByLimits(&m_parameter) ; // nt add 2017/8/20
			parameterSave(&m_parameter, m_pathName) ;
		}
	}
	else
		AfxMessageBox(_T("�ȴ򿪲��������ļ�!")) ;

	return ;
}


void CParameterDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnOK();
}

LRESULT CParameterDlg::OnPropertyChanged(WPARAM wParam,LPARAM lParam)
{
	CMFCPropertyGridProperty* pItem = (CMFCPropertyGridProperty*)lParam ;
	GetPropertyValue(pItem) ;

	return 0;
}

// ����m_parameter���¸����е�property��ֵ
void CParameterDlg::ResetAllProperty()
{
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
		SetPropertyValue(pGroup) ;
	}

	return ;
}

// nt add 2017/5/9 ���ַ�������ȡһ������
int cstrGetInt(CString& str)
{
	int i, n = str.GetLength() ;
	CString buf ;

	for( i = 0 ; i < n ; i++ )
	{
		if( '0' <= str[i] && str[i] <= '9' )
			buf.AppendChar(str[i]) ;
	}
	if( buf.GetLength() > 0 )
		return _tstoi(buf) ;
	else
		return -1 ;
}

void CParameterDlg::GetPropertyValue(CMFCPropertyGridProperty* pProperty)
{
	CString name = pProperty->GetName() ;
	_variant_t v = pProperty->GetValue() ;
	int k ;
	if( name == _T("��ӡ���ͺ�") )
		_tcscpy_s(m_parameter.printer, 64, (CString)v) ;
	else
	if( name == _T("��ӡ����X��ߴ�") )
		m_parameter.L = v ;
	else
	if( name == _T("��ӡ����Y��ߴ�") )
		m_parameter.W = v ;
	else
	if( name == _T("��ӡ����Z��ߴ�") )
		m_parameter.H = v ;
	else
	if( name == _T("��ӡX��ֱ���") )
		m_parameter.PL = v ;
	else
	if( name == _T("��ӡY��ֱ���") )
		m_parameter.PW = v ;
	else
	if( name == _T("���Ͷ��ԭ�㶨��") )
		m_parameter.P0 = v ;
	else
	if( name == _T("���Ͷ��X'�᷽����") )
		m_parameter.Dx = v ;
	else
	if( name == _T("�����ͺ�") )
		_tcscpy_s(m_parameter.material, 64, (CString)v) ;
	else
	if( name.Find(_T("���t")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 6 )
		{
			if( name.Find(_T("�ع��ǿ")) >= 0 )
			{
				if( name.Find(_T("С")) >= 0 )
					m_parameter.lils[k-1] = v ;
				else
					m_parameter.lis[k] = v ;
			}
			else
			if( name.Find(_T("�ع�ʱ��")) >= 0 )
			{
				if( name.Find(_T("С")) >= 0 )
					m_parameter.ltls[k-1] = v ;
				else
					m_parameter.lts[k] = v ;
			}
			else
				m_parameter.ts[k] = v ;
		}
	}
	else
	if( name == _T("���ת��1") )
		m_parameter.w1max = v ;
	else
	if( name == _T("���ת��1") )
		m_parameter.w1min = v ;
	else
	if( name == _T("�����ٶ�") )
		m_parameter.w1p = v ;
	else
	if( name == _T("������ʼ����") )
		m_parameter.L1 = v ;
	else
	if( name == _T("���ٽ�������") )
		m_parameter.L2 = v ;
	else
	if( name == _T("��ӡ��������ֹͣ����") )
		m_parameter.L3 = v ;
	else
	if( name == _T("���ת��2") )
		m_parameter.w2max = v ;
	else
	if( name == _T("���ת��2") )
		m_parameter.w2min = v ;
	else
	if( name == _T("ת���Ƕ�") )
		m_parameter.beta = v ;
	else
	if( name.Find(_T("���ص�")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 1 && k <= 4 )
			m_parameter.grays[k-1] = v ;
	}
	else
	if( name == _T("��ӡƽ̨ȱʡ�߶�") )
		m_parameter.H0 = v ;
	else
	if( name == _T("�Ҷ���С�ɴ���߶�") ) // nt add 2017/6/1
		m_parameter.wp = v ;
	else
	if( name == _T("��е���") )
		m_parameter.LDdelta = v ;
	else
	if( name == _T("��դ�ߵ�ǰλ��") )
		m_parameter.LD0 = v ;
	else
	if( name == _T("�������ϵ��") )
		m_parameter.LNF = v ;
	else
	if( name == _T("Ͷ�����") )
		m_parameter.LF = v ;
	else
	if( name == _T("�����X���") )
		m_parameter.LENx = v ;
	else
	if( name == _T("�����Y���") )
		m_parameter.LENy = v ;
	else
	if( name == _T("�Ϻ�������") )
		m_parameter.n = v ;
	else
	if( name == _T("�Ϻк��") ) // nt add 2017/8/20
		m_parameter.tmc = v ;
	else
	if( name == _T("�Ϻг��ּ��ٱ�") ) // nt add 2017/8/20
		m_parameter.ratio = v ;
	else
	if( name.Find(_T("�������")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 5 )
			m_parameter.tps[k] = v ;
	}
	else
	if( name.Find(_T("���Ͳ���")) >=0 ) // nt add 2017/8/20
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 5 )
			m_parameter.tis[k] = v ;
	}

	// nt add 2017/8/21
	if( parameterResByLimits(&m_parameter) > 0 )
		AfxMessageBox(_T("����������ȡֵ��Χ!")) ;

	return ;
}

void CParameterDlg::SetPropertyValue(CMFCPropertyGridProperty* pProperty)
{
	CString name ;

	if( pProperty == NULL ||
		pProperty->GetSubItemsCount() > 0 )
		return ; // ���������������property
	name = pProperty->GetName() ;

	int k ;
	if( name == _T("��ӡ���ͺ�") )
		pProperty->SetValue(m_parameter.printer) ;
	else
	if( name == _T("��ӡ����X��ߴ�") )
		pProperty->SetValue(m_parameter.L) ;
	else
	if( name == _T("��ӡ����Y��ߴ�") )
		pProperty->SetValue(m_parameter.W) ;
	else
	if( name == _T("��ӡ����Z��ߴ�") )
		pProperty->SetValue(m_parameter.H) ;
	else
	if( name == _T("��ӡX��ֱ���") )
		pProperty->SetValue((long)m_parameter.PL) ;
	else
	if( name == _T("��ӡY��ֱ���") )
		pProperty->SetValue((long)m_parameter.PW) ;
	else
	if( name == _T("���Ͷ��ԭ�㶨��") )
		pProperty->SetValue((long)m_parameter.P0) ;
	else
	if( name == _T("���Ͷ��X'�᷽����") )
		pProperty->SetValue((long)m_parameter.Dx) ;
	else
	if( name == _T("�����ͺ�") )
		pProperty->SetValue(m_parameter.material) ;
	else
	if( name.Find(_T("���t")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 6 )
		{
			if( name.Find(_T("�ع��ǿ")) >= 0 )
			{
				if( name.Find(_T("С")) >= 0 )
					pProperty->SetValue((long)m_parameter.lils[k-1]) ;
				else
					pProperty->SetValue((long)m_parameter.lis[k]) ;
			}
			else
			if( name.Find(_T("�ع�ʱ��")) >= 0 )
			{
				if( name.Find(_T("С")) >= 0 )
					pProperty->SetValue(m_parameter.ltls[k-1]) ;
				else
					pProperty->SetValue(m_parameter.lts[k]) ;
			}
			else
				pProperty->SetValue(m_parameter.ts[k]) ;
		}
	}
	else
	if( name == _T("���ת��1") )
		pProperty->SetValue(m_parameter.w1max) ;
	else
	if( name == _T("���ת��1") )
		pProperty->SetValue(m_parameter.w1min) ;
	else
	if( name == _T("�����ٶ�") )
		pProperty->SetValue(m_parameter.w1p) ;
	else
	if( name == _T("������ʼ����") )
		pProperty->SetValue(m_parameter.L1) ;
	else
	if( name == _T("���ٽ�������") )
		pProperty->SetValue(m_parameter.L2) ;
	else
	if( name == _T("��ӡ��������ֹͣ����") )
		pProperty->SetValue(m_parameter.L3) ;
	else
	if( name == _T("���ת��2") )
		pProperty->SetValue(m_parameter.w2max) ;
	else
	if( name == _T("���ת��2") )
		pProperty->SetValue(m_parameter.w2min) ;
	else
	if( name == _T("ת���Ƕ�") )
		pProperty->SetValue(m_parameter.beta) ;
	else
	if( name.Find(_T("���ص�")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 1 && k <= 4 )
			pProperty->SetValue((long)m_parameter.grays[k-1]) ;
	}
	else
	if( name == _T("��ӡƽ̨ȱʡ�߶�") )
		pProperty->SetValue(m_parameter.H0) ;
	else
	if( name == _T("�Ҷ���С�ɴ���߶�") ) // nt add 2017/6/1
		pProperty->SetValue((long)m_parameter.wp) ;
	else
	if( name == _T("��е���") )
		pProperty->SetValue(m_parameter.LDdelta) ;
	else
	if( name == _T("��դ�ߵ�ǰλ��") )
		pProperty->SetValue(m_parameter.LD0) ;
	else
	if( name == _T("�������ϵ��") )
		pProperty->SetValue(m_parameter.LNF) ;
	else
	if( name == _T("Ͷ�����") )
		pProperty->SetValue(m_parameter.LF) ;
	else
	if( name == _T("�����X���") )
		pProperty->SetValue(m_parameter.LENx) ;
	else
	if( name == _T("�����Y���") )
		pProperty->SetValue(m_parameter.LENy) ;
	else
	if( name == _T("�Ϻ�������") )
		pProperty->SetValue(m_parameter.n) ;
	else
	if( name == _T("�Ϻк��") )
		pProperty->SetValue(m_parameter.tmc) ;
	else
	if( name == _T("�Ϻг��ּ��ٱ�") )
		pProperty->SetValue(m_parameter.ratio) ;
	else
	if( name.Find(_T("�������")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 5 )
			pProperty->SetValue(m_parameter.tps[k]) ;
	}
	else
	if( name.Find(_T("���Ͳ���")) >=0 ) // nt add 2017/8/20
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 5 )
			pProperty->SetValue((long)m_parameter.tis[k]) ;
	}

	return ;
}
