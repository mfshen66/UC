// PrintDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "CAD.h"
#include "PrintDlg.h"
#include "afxdialogex.h"
#include "CADDoc.h"
#include <iostream>//CSY
#include <Windows.h>//CSY
#include <string>//CSY
#include<afxpriv.h>//CSY
#include <locale>//CSY�ַ���ͷ�ļ�

#include <GLFW/glfw3.h>
#include <iostream>
// CPrintDlg �Ի���

IMPLEMENT_DYNAMIC(CPrintDlg, CDialogEx)

CPrintDlg::CPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PRINTDLG, pParent)
{
	m_prg.ws = NULL ; // nt add 2017/9/9
	m_gray = TRUE ; // nt add 2019/4/9
}

CPrintDlg::~CPrintDlg()
{
	// nt add 2017/9/9
	prgReleaseBuf(&m_prg) ;
}

void CPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_browse);
	DDX_Check(pDX, IDC_CHECK1, m_gray);
}


BEGIN_MESSAGE_MAP(CPrintDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CPrintDlg::OnClickedMfcbutton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CPrintDlg::OnClickedMfcbutton2)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
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
	buf.Format(_T("\\Works\\PrintData%d.p_f"), count) ;
	filePath += buf ;
	m_browse.SetWindowText(filePath) ;

	CMFCButton* pB = (CMFCButton*)GetDlgItem(IDC_MFCBUTTON1) ;
	pB->SetFaceColor(RGB(100,255,255)) ;
	pB->m_bTransparent = FALSE ;
	pB->m_bDontUseWinXPTheme = TRUE ;


	SetTimer(0, 1000, NULL) ;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CPrintDlg::OnClickedMfcbutton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CProgressCtrl* pCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1) ;
	if( pCtrl )
		pCtrl->SetPos(0) ;

	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc == NULL )
		return ;
	int n = pDoc->GetNumOfSTL() ;
	if (n < 1)
	{
		AfxMessageBox(_T("��STLģ��!"));
		return;
	}
	if (pDoc->IsPropertySetAllStl())
	{
		pDoc->Print(&m_prg);

	}
	return ;
}


void CPrintDlg::OnClickedMfcbutton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE) ; // nt add 2019/5/5
	CString filePath ;
	//GetDlgItemText(IDC_MFCEDITBROWSE1, filePath);
	m_browse.GetWindowText(filePath) ;

	// 2017 CSY-------------------------------------------------
	CString txtPath;//CSY��������Ҫ������ļ���λ��
	GetModuleFileName(NULL, txtPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	txtPath.ReleaseBuffer();
	int nPos;
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
					 //file.Open(txtPath, CFile::modeWrite | CFile::modeCreate); //��������ھʹ���
					 //file.Write(filePath/*, wcslen(filePath ,sizeof(wchar_t));)*/
	file.Close();//�ر��ļ���
	// end CSY--------------------------------------------------
	
	CProgressCtrl* pCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1) ;
	if( pCtrl )
		pCtrl->SetPos(0) ;
	CCADDoc* pDoc = cadGetDoc() ;
	if( pDoc )
	{
		// nt add 2019/5/27
		int gvm = 2 ; // 1=old algorithm, 1=new algorithm
		CString filePath ;
		efpGet(filePath) ; // �����ļ�Ŀ¼
		filePath += _T("\\Libs\\GVM.txt") ; // �ҶȽ����㷨����
		FILE* fp = NULL ;
		_tfopen_s(&fp, filePath.GetBuffer(0), _T("r+t")) ;
		if( fp )
		{
			fscanf_s(fp, "%d", &gvm) ;
			fclose(fp) ;
		}

		cadPromptStr(_T("��ʼ������...")) ; // nt add 2017/10/1
		pDoc->CollisionDetect2(&m_prg) ; // nt add 2017/10/1
		cadPromptStr(_T("��ɸ�����!")) ; // nt add 2017/10/1
		if( pDoc->m_collision ) // nt add 2017/10/1
			AfxMessageBox(_T("ģ�ͳ���������Χ����������빤��̨!")) ;
		else
			pDoc->Out(filePath, &m_prg, m_gray, gvm) ; // nt modify 2019/4/9 add m_gray
	}

	return ;
}

void CPrintDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int nPos = prgGetPercent(&m_prg) ;
	CProgressCtrl* pCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1) ;
	if( pCtrl )
		pCtrl->SetPos(nPos) ;

	CDialogEx::OnTimer(nIDEvent);
}



