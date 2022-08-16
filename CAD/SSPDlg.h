#pragma once
#include "afxpropertygridctrl.h"


// CSSPDlg �Ի���

class CSSPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSSPDlg)

public:
	CSSPDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSSPDlg();

	SSP m_ssp ;
	SSP m_ssp2 ; // nt add 2017/8/26
	void GetPropertyValue(CMFCPropertyGridProperty* pProperty) ;
	void SetPropertyValue(CMFCPropertyGridProperty* pProperty) ; // nt add 2017/8/26

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SSPDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	CMFCPropertyGridCtrl m_grid;
	afx_msg void OnClickedMfcbutton1();
	afx_msg void OnClickedMfcbutton2();
	afx_msg void OnClickedMfcbutton3();
};
