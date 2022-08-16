#pragma once
#include "afxeditbrowsectrl.h"


// CPrintDlg 对话框

class CPrintDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrintDlg)

public:
	CPrintDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPrintDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRINTDLG };
#endif
	PRG m_prg ; // nt add 2017/9/9
	BOOL m_gray; // nt add 2019/4/9
	CMFCEditBrowseCtrl m_browse;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedMfcbutton1();
	afx_msg void OnClickedMfcbutton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	DECLARE_MESSAGE_MAP()
};
