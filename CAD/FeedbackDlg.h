#pragma once
#include "afxwin.h"


// FeedbackDlg �Ի���

class FeedbackDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FeedbackDlg)

public:
	FeedbackDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FeedbackDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FEEDBACK_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_fb_stastic;
	afx_msg void OnStnClickedStaticFeedback();
	virtual BOOL OnInitDialog();
	CEdit m_fb_edit;
	afx_msg void OnBnClickedGettxt();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEditFeedback();
};
