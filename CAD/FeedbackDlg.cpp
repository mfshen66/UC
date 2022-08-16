// FeedbackDlg.cpp : 实现文件
//WMZ - FeedBack

#include "stdafx.h"
#include "CAD.h"
#include "FeedbackDlg.h"
#include "afxdialogex.h"

#include <windows.h>
#include <shellapi.h>  

//CFont cfont;
// FeedbackDlg 对话框

IMPLEMENT_DYNAMIC(FeedbackDlg, CDialogEx)

FeedbackDlg::FeedbackDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FEEDBACK_DLG, pParent)
{

}

FeedbackDlg::~FeedbackDlg()
{
}

void FeedbackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FEEDBACK, m_fb_stastic);
	DDX_Control(pDX, IDC_EDIT_FEEDBACK, m_fb_edit);
}

void FeedbackDlg::OnStnClickedStaticFeedback()
{
}


BEGIN_MESSAGE_MAP(FeedbackDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_FEEDBACK, &FeedbackDlg::OnStnClickedStaticFeedback)
	ON_BN_CLICKED(IDOK_GETTXT, &FeedbackDlg::OnBnClickedGettxt)
	ON_BN_CLICKED(IDCANCEL, &FeedbackDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_FEEDBACK, &FeedbackDlg::OnEnChangeEditFeedback)
END_MESSAGE_MAP()


// FeedbackDlg 消息处理程序


BOOL FeedbackDlg::OnInitDialog()
{
	CFont cfont;
	CDialogEx::OnInitDialog();
	cfont.CreatePointFont(150, _T("宋体"), NULL);
	GetDlgItem(IDC_STATIC_FEEDBACK)->SetFont(&cfont);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
CString GetModuleDir()//获取当前路径
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

void FeedbackDlg::OnBnClickedGettxt()
{
	
	USES_CONVERSION;
	CString str;
	m_fb_edit.GetWindowText(str);//读取编辑框内容
	//写入TXT
	CFile file;
	CString filepath, filename;
	CString mailexepath, mailexename;


	filepath = GetModuleDir();
	filename = "\\Feedback\\feedback.txt";  //文件名
	filename = filepath + filename;  //文件路径

	mailexename = "\\Feedback\\mail.exe";
	mailexepath = filepath + mailexename;

	if (file.Open(filename, CFile::typeBinary |CFile::modeReadWrite |CFile::modeCreate|CFile::modeNoTruncate))
	{

		if (file.Seek(0, CFile::begin) == file.Seek(0, CFile::end))
		{
			file.Write("You can send your suggestions to us!", UINT(strlen("You can send your suggestions to us!"))); //写入文件
			file.SeekToEnd();
			file.Write("\n\n", UINT(strlen("\n\n"))); //写入文件
			CTime t = CTime::GetCurrentTime();

			CString str_t = t.Format(_T("%Y-%m-%d   %H:%M:%S"));

			file.SeekToEnd();
			file.Write(T2A(str_t), UINT(strlen(T2A(str_t))));
			file.Write("\n", UINT(strlen("\n")));
			file.Write(T2A(str), UINT(strlen(T2A(str))));
			file.Close();  //关闭文件
			//调用外部邮件exe
			ShellExecute(NULL, _T("open"), mailexepath, _T(""), _T(""), SW_HIDE);



		}
		else
		{
			//file.Open(filename, CFile::modeReadWrite | CFile::modeNoTruncate);
			file.SeekToEnd();
			file.Write("\n\n", UINT(strlen("\n\n"))); //写入文件
			CTime t = CTime::GetCurrentTime();

			CString str_t= t.Format(_T("%Y-%m-%d   %H:%M:%S"));

			file.SeekToEnd();
			file.Write(T2A(str_t), UINT(strlen(T2A(str_t))));
			file.Write("\n", UINT(strlen("\n")));
			file.Write(T2A(str), UINT(strlen(T2A(str))));
			file.Close();


			ShellExecute(NULL, _T("open"), mailexepath, _T(""), _T(""), SW_HIDE);

		}
	}
	CDialogEx::OnCancel();
	
	// TODO: 在此添加控件通知处理程序代码
}


void FeedbackDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void FeedbackDlg::OnEnChangeEditFeedback()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
