#pragma once
#include "afxwin.h"


// CVLTDlg �Ի���

class CVLTDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVLTDlg)

public:
	CVLTDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVLTDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VLTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_i0;
	int m_i1;
	double m_ratio;
	CListBox m_list;
	void AddString(int i0, int i1, double ratio) ;
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedMfcbutton1();
	afx_msg void OnClickedMfcbutton2();
	virtual void OnOK();
};
