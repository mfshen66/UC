#pragma once
#include "afxwin.h"


// VIPACC �Ի���

class VIPACC : public CDialogEx
{
	DECLARE_DYNAMIC(VIPACC)

public:
	VIPACC(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~VIPACC();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIPACC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeVipAcc();
	CEdit m_vip_acc;
	CEdit m_vip_pas;
	afx_msg void OnBnClickedCancel();
};
