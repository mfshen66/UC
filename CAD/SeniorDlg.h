#pragma once


// CSeniorDlg �Ի���

class CSeniorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSeniorDlg)

public:
	CSeniorDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSeniorDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENIORDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_li;
	double m_lt;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
	afx_msg void OnClickedButton1();
};
