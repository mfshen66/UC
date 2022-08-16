#pragma once
#include "afxwin.h"


// CLayoutDlg 对话框

class CLayoutDlg : public CDlg
{
	DECLARE_DYNAMIC(CLayoutDlg)

public:
	CLayoutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLayoutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAYOUTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_xspan;
	int m_xn;
	double m_yspan;
	int m_yn;
	CCADDoc* m_pDoc ;
	CSetInt m_createSTLRecs ;

	void Init(CCADDoc* pDoc) ;
	void EnableUndo(BOOL enable) ;
	void DrawDoc() ;
	void Quit() ; // nt add 2017/8/29

	int OnLBD(UINT nFlags, double p[3], double v[3], void* pVI) ;
	int OnMM(UINT nFlags, double p[3], double v[3], void* pVI) ;

	virtual BOOL OnInitDialog();
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
	virtual void OnCancel();
};
