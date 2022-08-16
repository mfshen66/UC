// VIPACC.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CAD.h"
#include "VIPACC.h"
#include "afxdialogex.h"

#include "ParameterDlg.h"
// VIPACC �Ի���

IMPLEMENT_DYNAMIC(VIPACC, CDialogEx)

VIPACC::VIPACC(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIPACC, pParent)
{
	//m_vip_pas.SetPasswordChar(TCHAR("*"));
}

VIPACC::~VIPACC()
{
}

CString GetModuleDir_vip()
{
	HMODULE module = GetModuleHandle(0);
	char pFileName[MAX_PATH];
	GetModuleFileNameA(module, pFileName, MAX_PATH);

	CString csFullPath(pFileName);
	int nPos = csFullPath.ReverseFind(_T('\\'));
	if (nPos < 0)
		return CString("");
	else
		return csFullPath.Left(nPos);
}
BOOL GetLang_vip(void)
{
	CString g_strCurrPath = GetModuleDir_vip();
	BOOL bEnglish = GetPrivateProfileInt(_T("English"), _T("bEnglish"), 0, g_strCurrPath + _T("\\setlang.ini"));

	return bEnglish;
}
BOOL b_mLangFlagvip = GetLang_vip();
void VIPACC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIP_ACC, m_vip_acc);
	DDX_Control(pDX, IDC_VIP_PAS, m_vip_pas);
	//m_vip_pas.SetPasswordChar(TCHAR("*"));
}


BEGIN_MESSAGE_MAP(VIPACC, CDialogEx)
	ON_BN_CLICKED(IDOK, &VIPACC::OnBnClickedOk)
	ON_EN_CHANGE(IDC_VIP_ACC, &VIPACC::OnEnChangeVipAcc)
	ON_BN_CLICKED(IDCANCEL, &VIPACC::OnBnClickedCancel)
END_MESSAGE_MAP()


// VIPACC ��Ϣ�������


void VIPACC::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str_acc, str_pas;
	CString acc = _T("vip123456");
	CString pass = _T("123456");
	m_vip_acc.GetWindowText(str_acc);//��ȡ�༭������
	m_vip_pas.GetWindowText(str_pas);
	
	int iMes;
	if (str_acc == acc)
	{
		if(str_pas == pass)
		{
			CParameterDlg dlg;
			dlg.DoModal();
		}
		else
		{
			//��Ӣ��Ӧ��Ҫ�л�
			if (!b_mLangFlagvip)
				iMes = MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("�������!"), _T("����"), MB_OKCANCEL | MB_ICONINFORMATION, MAKELANGID(LANG_CHINESE, SUBLANG_ENGLISH_US));
			else
				iMes = MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Password error!"), _T("Error"), MB_OKCANCEL | MB_ICONINFORMATION, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			VIPACC dlg;
			dlg.DoModal();
		}

	}
	else
	{
		if (!b_mLangFlagvip)
			iMes = MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("�˺Ų�����!"), _T("Error"), MB_OKCANCEL | MB_ICONINFORMATION, MAKELANGID(LANG_CHINESE, SUBLANG_ENGLISH_US));
		else
			iMes = MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Account does not exist!"), _T("Error"), MB_OKCANCEL | MB_ICONINFORMATION, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		VIPACC dlg;

		dlg.DoModal();
	}
		
	CDialogEx::OnOK();
}


void VIPACC::OnEnChangeVipAcc()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void VIPACC::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
