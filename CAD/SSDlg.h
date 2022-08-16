#pragma once


// CSSDlg 对话框

class CSSDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSSDlg)

public:
	CSSDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSSDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SSDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_density;
	double m_densityMin ;
	double m_densityMax ;
	double m_densityD ;
	double m_r;
	double m_rMin ;
	double m_rMax ;
	double m_rD ;

	PRG m_prg ; // nt add 2017/9/10

	void ResDensity() ;
	void ResR() ;
	void UpdateSlider1() ;
	void UpdateSlider2() ;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedMfcbutton1();
	afx_msg void OnClickedMfcbutton2();
	afx_msg void OnClickedMfcbutton3();
	afx_msg void OnClickedMfcbutton4();
	virtual void OnCancel();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
