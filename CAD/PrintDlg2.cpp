// PrintDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "PrintDlg.h"
#include "afxdialogex.h"
#include "CADDoc.h"
#include <iostream>//CSY
#include <Windows.h>//CSY
#include <string>
#include<afxpriv.h>
#include <locale>//字符集头文件

//==============================================================
void runexe(CString ex)//CSY
{

	USES_CONVERSION;
	LPSTR path = T2A(ex);
	system(path);
	//LPCSTR path = (LPCSTR)ex.GetBuffer();
	//ShellExecute(NULL, _T("open"),_T("path"), NULL, NULL, SW_SHOWNORMAL);
	//rt = WinExec(path ,SW_SHOWDEFAULT);
	/*rt = WinExec(_T("1.txt"), SW_SHOWDEFAULT);//  返回2 没有找到路径。
	if (rt<32)
	{
		mess.Format(_T("%d"),rt);
		AfxMessageBox(mess);
	}*/

}

DWORD WINAPI DISPLAY1(LPVOID lpParameter)//CSY,新线程
{
	CString    sPath;
	CString    txtPath;//用来传递要传输的文件的位置
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);

//	AfxMessageBox(sPath);
	CString TCPpath = _T("\\TCP1.exe");
	/*if (sPath == _T("E:\\实验室任务\\3DPrint2017-5-19toLBS\\3DPrint2017-5-19toLBS\\Debug"))
		AfxMessageBox(_T("1"));
	else
		AfxMessageBox(_T("0"));*/
	sPath = sPath + TCPpath;
	//CString path = _T("E:\\实验室任务\\3DPrint2017-5-19toLBS\\3DPrint2017-5-19toLBS\\Debug\\TCP.exe");
	/*for (int i = 0; i < 50; i++)
	{
		if (sPath[i] == path[i])
			;
		else
			AfxMessageBox(_T("错"));
	}*/

	//AfxMessageBox(sPath);
	//CString path = _T("notepad f:\\deng\\001.TXT");//ok
	runexe(sPath);
	//CString path9= _T( "E:\\1.txt");//ok
	//runexe(path9);
	return 0;
}
//==============================================================

// CPrintDlg 对话框

IMPLEMENT_DYNAMIC(CPrintDlg, CDialogEx)

CPrintDlg::CPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PRINTDLG, pParent)
{
}

CPrintDlg::~CPrintDlg()
{
}

void CPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_browse);
}


BEGIN_MESSAGE_MAP(CPrintDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CPrintDlg::OnClickedMfcbutton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CPrintDlg::OnClickedMfcbutton2)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPrintDlg 消息处理程序

BOOL CPrintDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	static int count ;
	CString filePath, buf ;
	count++ ;
	efpGet(filePath) ;
	buf.Format(_T("\\Works\\PrintData%d.swk"), count) ;
	filePath += buf ;
	m_browse.SetWindowText(filePath) ;

	CMFCButton* pB = (CMFCButton*)GetDlgItem(IDC_MFCBUTTON1) ;
	pB->SetFaceColor(RGB(100,255,255)) ;
	pB->m_bTransparent = FALSE ;
	pB->m_bDontUseWinXPTheme = TRUE ;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPrintDlg::OnClickedMfcbutton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc )
		pDoc->Print() ;

	return ;
}


void CPrintDlg::OnClickedMfcbutton2()
{
	// TODO: 在此添加控件通知处理程序代码
	/*CString filePath ;
	//GetDlgItemText(IDC_MFCEDITBROWSE1, filePath);
	m_browse.GetWindowText(filePath) ;
	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc )
		pDoc->Out(filePath) ;*/

	CString filePath ;
	//GetDlgItemText(IDC_MFCEDITBROWSE1, filePath);
	m_browse.GetWindowText(filePath) ;
	CCADDoc* pDoc = cadGetDoc() ;

	CString    txtPath;//CSY用来传递要传输的文件的位置
	GetModuleFileName(NULL, txtPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	txtPath.ReleaseBuffer();
	int    nPos;
	nPos = txtPath.ReverseFind('\\');
	txtPath = txtPath.Left(nPos);
	txtPath = txtPath + _T("\\path.txt");//放在debug\path.txt
	CStdioFile file;
	file.Open(txtPath, CFile::modeWrite | CFile::modeCreate); //如果不存在就创建
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//设定
	file.WriteString(filePath);//正常写入
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);//还原区域设定
	/*file.Open(txtPath, CFile::modeWrite | CFile::modeCreate); //如果不存在就创建
	file.Write(filePath/*, wcslen(filePath ,sizeof(wchar_t));)*/
	file.Close();//关闭文件流
	
	if (pDoc) {

		pDoc->Out(filePath);
		HANDLE pThread1 = CreateThread(NULL, 0, DISPLAY1, NULL, 0, NULL);//启动新线程CSY
		CloseHandle(pThread1);
	}

	return ;
}
