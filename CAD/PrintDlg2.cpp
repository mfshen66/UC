// PrintDlg.cpp : ʵ���ļ�
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
#include <locale>//�ַ���ͷ�ļ�

//==============================================================
void runexe(CString ex)//CSY
{

	USES_CONVERSION;
	LPSTR path = T2A(ex);
	system(path);
	//LPCSTR path = (LPCSTR)ex.GetBuffer();
	//ShellExecute(NULL, _T("open"),_T("path"), NULL, NULL, SW_SHOWNORMAL);
	//rt = WinExec(path ,SW_SHOWDEFAULT);
	/*rt = WinExec(_T("1.txt"), SW_SHOWDEFAULT);//  ����2 û���ҵ�·����
	if (rt<32)
	{
		mess.Format(_T("%d"),rt);
		AfxMessageBox(mess);
	}*/

}

DWORD WINAPI DISPLAY1(LPVOID lpParameter)//CSY,���߳�
{
	CString    sPath;
	CString    txtPath;//��������Ҫ������ļ���λ��
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);

//	AfxMessageBox(sPath);
	CString TCPpath = _T("\\TCP1.exe");
	/*if (sPath == _T("E:\\ʵ��������\\3DPrint2017-5-19toLBS\\3DPrint2017-5-19toLBS\\Debug"))
		AfxMessageBox(_T("1"));
	else
		AfxMessageBox(_T("0"));*/
	sPath = sPath + TCPpath;
	//CString path = _T("E:\\ʵ��������\\3DPrint2017-5-19toLBS\\3DPrint2017-5-19toLBS\\Debug\\TCP.exe");
	/*for (int i = 0; i < 50; i++)
	{
		if (sPath[i] == path[i])
			;
		else
			AfxMessageBox(_T("��"));
	}*/

	//AfxMessageBox(sPath);
	//CString path = _T("notepad f:\\deng\\001.TXT");//ok
	runexe(sPath);
	//CString path9= _T( "E:\\1.txt");//ok
	//runexe(path9);
	return 0;
}
//==============================================================

// CPrintDlg �Ի���

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


// CPrintDlg ��Ϣ�������

BOOL CPrintDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CPrintDlg::OnClickedMfcbutton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc )
		pDoc->Print() ;

	return ;
}


void CPrintDlg::OnClickedMfcbutton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	CString    txtPath;//CSY��������Ҫ������ļ���λ��
	GetModuleFileName(NULL, txtPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	txtPath.ReleaseBuffer();
	int    nPos;
	nPos = txtPath.ReverseFind('\\');
	txtPath = txtPath.Left(nPos);
	txtPath = txtPath + _T("\\path.txt");//����debug\path.txt
	CStdioFile file;
	file.Open(txtPath, CFile::modeWrite | CFile::modeCreate); //��������ھʹ���
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//�趨
	file.WriteString(filePath);//����д��
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);//��ԭ�����趨
	/*file.Open(txtPath, CFile::modeWrite | CFile::modeCreate); //��������ھʹ���
	file.Write(filePath/*, wcslen(filePath ,sizeof(wchar_t));)*/
	file.Close();//�ر��ļ���
	
	if (pDoc) {

		pDoc->Out(filePath);
		HANDLE pThread1 = CreateThread(NULL, 0, DISPLAY1, NULL, 0, NULL);//�������߳�CSY
		CloseHandle(pThread1);
	}

	return ;
}
