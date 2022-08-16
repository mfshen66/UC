// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// CAD.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "CAD.h"
#include "MainFrm.h"
#include "CLANCHOOSE.h"
#include "LANSAVEBOX.h"
#include "CADDoc.h"
#include "CADView.h"
#include <iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// CCADApp

BEGIN_MESSAGE_MAP(CCADApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CCADApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	//ON_COMMAND(ID_CHINESE, &CCADApp::OnChinese)
	//ON_COMMAND(ID_ENGLISH, &CCADApp::OnEnglish)
	ON_BN_CLICKED(ID_CHINESE, &CCADApp::OnBnClickedChinese)
	ON_BN_CLICKED(ID_ENGLISH, &CCADApp::OnBnClickedEnglish)
	ON_COMMAND(ID_SW_LANG, &CCADApp::OnSwLan)
END_MESSAGE_MAP()

CString GetModuleDir_cad();
// CCADApp ����

CCADApp::CCADApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("CAD.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CCADApp ����

CCADApp theApp;


// CCADApp ��ʼ��

BOOL CCADApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	//g_bStart = GetStart();
	g_bFirst = GetFirst();
	if (!g_bFirst)
	{
		CLANCHOOSE Dlg;
		Dlg.DoModal();
		SetFirst(TRUE);
	}

	//SetFirst(TRUE);
	g_bEnglish = GetLang();
	//CString g_strCurrPath = GetModuleDir_cad();
	if (!g_bEnglish)
	{
		m_hLangDLL = ::LoadLibrary(_T("3D-Chinese.dll"));
	}
	else
	{
		m_hLangDLL = LoadLibrary(_T("3D-English.dll"));
	}

	AfxSetResourceHandle(m_hLangDLL);
	EnableTaskbarInteraction(TRUE);

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();



	// ʹ�� RichEdit �ؼ���Ҫ AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCADDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CCADView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	//char proA = ProcessShellCommand(cmdInfo);
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// register draw functions which are used in GP.dll
	if (regDrawPt(viDrawPoint) != ERSUCSS ||
		regDrawLin(viDrawLine) != ERSUCSS ||
		regDrawPoly(viDrawPolyline) != ERSUCSS ||
		regDrawTris(viDrawTris) != ERSUCSS ||
		regDrawQuas(viDrawQuas) != ERSUCSS)
		return FALSE;

	// ��λĬ��Ϊmm
	gpSetTol(1.0e-5) ;
	gpSetAng(1.0e-11) ;
	double pre[3] ;
	pre[0] = 1. ;
	pre[1] = 0.2 ;
	pre[2] = 10. ;
	gpSetPre(pre) ;
	double tra[3] ;
	tra[0] = 1. ;
	tra[1] = 0.2 ;
	tra[2] = 25. ;
	gpSetTra(tra) ;
	double dsc[3] ;
	dsc[0] = 2.5 ;
	dsc[1] = 0.2 ;
	dsc[2] = 10. ;
	gpSetDsc(dsc);
	//gpSetFit(0.001) ;
	double tes[3] ;
	tes[0] = 1. ;
	tes[1] = 1.047197551196597 ;
	tes[2] = 0.001 ;
	gpSetTes(tes) ;

	return TRUE;
}

int CCADApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);
	//���������Դ����������
	//BOOL bStart = GetStart();
	if (m_hLangDLL)
	{
		FreeLibrary(m_hLangDLL);
	}

	/*if (bStart)
	{
		STARTUPINFO StartupInfo = { 0 };
		PROCESS_INFORMATION ProcessInfo;

		StartupInfo.cb = sizeof(STARTUPINFO);
		char Path[256];
		GetModuleFileName(NULL, (LPWSTR)(LPCTSTR)Path, 250);
		ShellExecute(NULL, _T(""), (LPWSTR)(LPCTSTR)Path, NULL, NULL, SW_SHOWNORMAL);
		//CreateProcess(NULL, (LPWSTR)(LPCTSTR)Path, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);
	}*/
	//SetStart(FALSE);
	//g_bRestart = FALSE;
	return CWinAppEx::ExitInstance();
}

// CCADApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CCADApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CCADApp �Զ������/���淽��

void CCADApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CCADApp::LoadCustomState()
{
}

void CCADApp::SaveCustomState()
{
}

CString GetModuleDir_cad()
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

// CCADApp ��Ϣ�������

void CCADApp::SetLang(BOOL bEnglish)
{
	CString strTemp;
	strTemp.Format(_T("%d"), bEnglish);

	CString g_strCurrPath = GetModuleDir_cad();
	WritePrivateProfileString(_T("English"), _T("bEnglish"), strTemp, g_strCurrPath + _T("\\setlang.ini"));
}

void CCADApp::SetStart(BOOL bStart)
{
	CString strTemp;
	strTemp.Format(_T("%d"), bStart);

	CString g_strCurrPath = GetModuleDir_cad();
	WritePrivateProfileString(_T("Start"), _T("bStart"), strTemp, g_strCurrPath + _T("\\setlang.ini"));
}

void CCADApp::SetFirst(BOOL bFirst)
{
	CString strTemp;
	strTemp.Format(_T("%d"), bFirst);

	CString g_strCurrPath = GetModuleDir_cad();
	WritePrivateProfileString(_T("First"), _T("bFirst"), strTemp, g_strCurrPath + _T("\\setlang.ini"));
}

BOOL CCADApp::GetLang(void)
{
	CString g_strCurrPath = GetModuleDir_cad();
	BOOL bEnglish = GetPrivateProfileInt(_T("English"), _T("bEnglish"), 0, g_strCurrPath + _T("\\setlang.ini"));

	return bEnglish;
}

BOOL CCADApp::GetStart(void)
{
	CString g_strCurrPath = GetModuleDir_cad();
	BOOL bStart = GetPrivateProfileInt(_T("Start"), _T("bStart"), 0, g_strCurrPath + _T("\\setlang.ini"));

	return bStart;
}

BOOL CCADApp::GetFirst(void)
{
	CString g_strCurrPath = GetModuleDir_cad();
	BOOL bFirst = GetPrivateProfileInt(_T("First"), _T("bFirst"), 0, g_strCurrPath + _T("\\setlang.ini"));

	return bFirst;
}

/*
void CCADApp::OnChinese()
{
	// TODO: �ڴ���������������
	int iRet;
	if (g_bEnglish)
	{
		
		// Ӣ�İ��OK �� Cancel
		iRet = MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Really change to Chinese version?"), _T("Information"), MB_OKCANCEL | MB_ICONINFORMATION, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
	else
	{
		iRet = MessageBox(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("�Ѿ������İ汾��"), _T("��ʾ"), MB_OKCANCEL | MB_ICONINFORMATION);
		return;
	}

	if (iRet == IDCANCEL)
	{
		return;
	}

	SetLang(FALSE);
	SetStart(TRUE);
	//g_bRestart = TRUE;
	//PostMessage(WM_CLOSE, 0, 0);
	PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_CLOSE, 0, 0);
}

void CCADApp::OnEnglish()
{
	// TODO: �ڴ���������������
	int iRet;

	if (!g_bEnglish)
	{
		iRet = MessageBox(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("ȷ���л���Ӣ�İ汾?"), _T("��ʾ"), MB_OKCANCEL | MB_ICONINFORMATION);
	}
	else
	{
		iRet = MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("It's English version now."), _T("Information"), MB_OKCANCEL | MB_ICONINFORMATION, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

		return;
	}

	if (iRet == IDCANCEL)
	{
		return;
	}

	SetLang(TRUE);
	//g_bRestart = TRUE;
	SetStart(TRUE);
	PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_CLOSE, 0, 0);
}*/

void CCADApp::OnBnClickedChinese()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetLang(FALSE);
	PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_CLOSE, 0, 0);
}

void CCADApp::OnBnClickedEnglish()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetLang(TRUE);
	PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_CLOSE, 0, 0);

}

void CCADApp::OnSwLan()
{
	// TODO: �ڴ���������������
	//�����ó�δ��������״̬
	SetFirst(FALSE);
	CLANSAVEBOX dlg;
	dlg.DoModal();
}

// nt add 2019/3/24
void CCADApp::AddToRecentFileList(LPCTSTR lpszPathName)
{
	// TODO: �ڴ����ר�ô����/����û���
	if( _tcsstr(lpszPathName, _T(".swk")) != NULL ||
		_tcsstr(lpszPathName, _T(".SWK")) != NULL )
		CWinAppEx::AddToRecentFileList(lpszPathName);
	return ;
}
