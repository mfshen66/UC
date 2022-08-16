#pragma once


// CSeniorDlg 对话框

class CSeniorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSeniorDlg)

public:
	CSeniorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSeniorDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENIORDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_li;
	double m_lt;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
	afx_msg void OnClickedButton1();
};
