#pragma once
#include "afxeditbrowsectrl.h"


// CPrintDlg �Ի���

class CPrintDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrintDlg)

public:
	CPrintDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPrintDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRINTDLG };
#endif
	PRG m_prg ; // nt add 2017/9/9
	BOOL m_gray; // nt add 2019/4/9
	CMFCEditBrowseCtrl m_browse;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedMfcbutton1();
	afx_msg void OnClickedMfcbutton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	DECLARE_MESSAGE_MAP()
};
