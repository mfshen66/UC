#pragma once
#include "afxpropertygridctrl.h"


// CSSPDlg 对话框

class CSSPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSSPDlg)

public:
	CSSPDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSSPDlg();

	SSP m_ssp ;
	SSP m_ssp2 ; // nt add 2017/8/26
	void GetPropertyValue(CMFCPropertyGridProperty* pProperty) ;
	void SetPropertyValue(CMFCPropertyGridProperty* pProperty) ; // nt add 2017/8/26

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SSPDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	CMFCPropertyGridCtrl m_grid;
	afx_msg void OnClickedMfcbutton1();
	afx_msg void OnClickedMfcbutton2();
	afx_msg void OnClickedMfcbutton3();
};
