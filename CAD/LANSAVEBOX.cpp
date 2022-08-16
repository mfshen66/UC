// LANSAVEBOX.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "LANSAVEBOX.h"
#include "afxdialogex.h"
#include "CADDoc.h"

// CLANSAVEBOX 对话框

IMPLEMENT_DYNAMIC(CLANSAVEBOX, CDialog)

CLANSAVEBOX::CLANSAVEBOX(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ML_SAVEBOX, pParent)
{

}

CLANSAVEBOX::~CLANSAVEBOX()
{
}

void CLANSAVEBOX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLANSAVEBOX, CDialog)
	ON_BN_CLICKED(IDOK, &CLANSAVEBOX::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLANSAVEBOX::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLANSAVEBOX 消息处理程序



void CLANSAVEBOX::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CCADDoc* pDocF = cadGetDoc();
	CString pathName = pDocF->GetPathName();
	int n = pDocF->GetNumOfSTL();
	BOOL bEnglishF = theApp.g_bEnglish;
	if (n == 0)
	{ 
		//结束总进程&重启
		PostMessage(WM_QUIT, 0, 0);
		char Path[256];
		GetModuleFileName(NULL, (LPWSTR)(LPCTSTR)Path, 250);
		ShellExecute(NULL, _T(""), (LPWSTR)(LPCTSTR)Path, NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
		if (!(pathName.Find(_T(".SWK")) >= 0 ||
			pathName.Find(_T(".swk")) >= 0))
		{
			if(bEnglishF)
				MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Please save the existing file manually"), _T("CAD"), MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			else
				AfxMessageBox(_T("请手动保存现有文件"));
		}
		else
		{
			PostMessage(WM_QUIT, 0, 0);
			char Path[256];
			GetModuleFileName(NULL, (LPWSTR)(LPCTSTR)Path, 250);
			ShellExecute(NULL, _T(""), (LPWSTR)(LPCTSTR)Path, NULL, NULL, SW_SHOWNORMAL);
		}
	}
	
	CDialog::OnOK();
}


void CLANSAVEBOX::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
