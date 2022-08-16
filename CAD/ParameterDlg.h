#pragma once
#include "afxpropertygridctrl.h"


// CParameterDlg �Ի���

class CParameterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParameterDlg)

public:
	CParameterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParameterDlg();

	PARAMETER m_parameter ;
	CString m_pathName ;

	void InitProperty() ;
	void ResetAllProperty() ;
	void GetPropertyValue(CMFCPropertyGridProperty* pProperty) ;
	void SetPropertyValue(CMFCPropertyGridProperty* pProperty) ;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAMETERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMFCPropertyGridCtrl m_grid;
	afx_msg void OnClickedMfcbutton1();
	afx_msg void OnClickedMfcbutton2();
	afx_msg void OnClickedMfcbutton3();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
};
