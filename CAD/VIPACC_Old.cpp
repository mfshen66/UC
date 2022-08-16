// VIPACC.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "VIPACC.h"
#include "afxdialogex.h"

#include "ParameterDlg.h"
// VIPACC 对话框

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


// VIPACC 消息处理程序


void VIPACC::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str_acc, str_pas;
	CString acc = _T("vip123456");
	CString pass = _T("123456");
	m_vip_acc.GetWindowText(str_acc);//读取编辑框内容
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
			//中英文应该要切换
			if (!b_mLangFlagvip)
				iMes = MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("密码错误!"), _T("错误"), MB_OKCANCEL | MB_ICONINFORMATION, MAKELANGID(LANG_CHINESE, SUBLANG_ENGLISH_US));
			else
				iMes = MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Password error!"), _T("Error"), MB_OKCANCEL | MB_ICONINFORMATION, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			VIPACC dlg;
			dlg.DoModal();
		}

	}
	else
	{
		if (!b_mLangFlagvip)
			iMes = MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("账号不存在!"), _T("Error"), MB_OKCANCEL | MB_ICONINFORMATION, MAKELANGID(LANG_CHINESE, SUBLANG_ENGLISH_US));
		else
			iMes = MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Account does not exist!"), _T("Error"), MB_OKCANCEL | MB_ICONINFORMATION, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		VIPACC dlg;

		dlg.DoModal();
	}
		
	CDialogEx::OnOK();
}


void VIPACC::OnEnChangeVipAcc()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void VIPACC::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
