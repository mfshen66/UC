// FeedbackDlg.cpp : ʵ���ļ�
//WMZ - FeedBack

#include "stdafx.h"
#include "CAD.h"
#include "FeedbackDlg.h"
#include "afxdialogex.h"

#include <windows.h>
#include <shellapi.h>  

//CFont cfont;
// FeedbackDlg �Ի���

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


// FeedbackDlg ��Ϣ�������


BOOL FeedbackDlg::OnInitDialog()
{
	CFont cfont;
	CDialogEx::OnInitDialog();
	cfont.CreatePointFont(150, _T("����"), NULL);
	GetDlgItem(IDC_STATIC_FEEDBACK)->SetFont(&cfont);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
CString GetModuleDir()//��ȡ��ǰ·��
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
	m_fb_edit.GetWindowText(str);//��ȡ�༭������
	//д��TXT
	CFile file;
	CString filepath, filename;
	CString mailexepath, mailexename;


	filepath = GetModuleDir();
	filename = "\\Feedback\\feedback.txt";  //�ļ���
	filename = filepath + filename;  //�ļ�·��

	mailexename = "\\Feedback\\mail.exe";
	mailexepath = filepath + mailexename;

	if (file.Open(filename, CFile::typeBinary |CFile::modeReadWrite |CFile::modeCreate|CFile::modeNoTruncate))
	{

		if (file.Seek(0, CFile::begin) == file.Seek(0, CFile::end))
		{
			file.Write("You can send your suggestions to us!", UINT(strlen("You can send your suggestions to us!"))); //д���ļ�
			file.SeekToEnd();
			file.Write("\n\n", UINT(strlen("\n\n"))); //д���ļ�
			CTime t = CTime::GetCurrentTime();

			CString str_t = t.Format(_T("%Y-%m-%d   %H:%M:%S"));

			file.SeekToEnd();
			file.Write(T2A(str_t), UINT(strlen(T2A(str_t))));
			file.Write("\n", UINT(strlen("\n")));
			file.Write(T2A(str), UINT(strlen(T2A(str))));
			file.Close();  //�ر��ļ�
			//�����ⲿ�ʼ�exe
			ShellExecute(NULL, _T("open"), mailexepath, _T(""), _T(""), SW_HIDE);



		}
		else
		{
			//file.Open(filename, CFile::modeReadWrite | CFile::modeNoTruncate);
			file.SeekToEnd();
			file.Write("\n\n", UINT(strlen("\n\n"))); //д���ļ�
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
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void FeedbackDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void FeedbackDlg::OnEnChangeEditFeedback()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
