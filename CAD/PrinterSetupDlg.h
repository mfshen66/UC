#pragma once


// CPrinterSetupDlg 对话框

class CPrinterSetupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrinterSetupDlg)

public:
	CPrinterSetupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPrinterSetupDlg();

	int m_nParameter ;
	PARAMETER* m_parameters ;
	PARAMETER m_cur ;
	CString m_printer ;
	CString m_color ;
	CString m_material ;
	double m_h ;
	int m_li ;
	double m_lt ;
	int m_constH ;

	int m_pl;
	int m_pw;

	int Load() ; // nt add 2017/9/30

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRINTERSETUPDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo3();
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
};
