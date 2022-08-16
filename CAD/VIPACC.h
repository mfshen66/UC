#pragma once
#include "afxwin.h"


// VIPACC 对话框

class VIPACC : public CDialogEx
{
	DECLARE_DYNAMIC(VIPACC)

public:
	VIPACC(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~VIPACC();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIPACC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeVipAcc();
	CEdit m_vip_acc;
	CEdit m_vip_pas;
	afx_msg void OnBnClickedCancel();
};
