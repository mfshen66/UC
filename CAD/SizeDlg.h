#pragma once


// CSizeDlg 对话框

class CSizeDlg : public CDlg
{
	DECLARE_DYNAMIC(CSizeDlg)

public:
	CSizeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSizeDlg();
	void Init(STL* stl, CCADDoc* pDoc) ; // 用STL模型做初始化
	void Scale(double scale) ; // 比例变换

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIZEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	afx_msg void OnClickedButton3();
	afx_msg void OnClickedButton4();
	afx_msg void OnClickedButton5();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	double m_x; // displayed, nt 2017/8/24加注释
	double m_y; // displayed, nt 2017/8/24加注释
	double m_z; // displayed, nt 2017/8/24加注释
	double m_s; // displayed, nt 2017/8/24加注释
	STL* m_stl ;
	PNT3D m_center ;
	double m_realX ; // nt modify 2017/8/24 改为real
	double m_realY ; // nt modify 2017/8/24 改为real
	double m_realZ ; // nt modify 2017/8/24 改为real
	double m_realS ; // nt modify 2017/8/24 改为real
	CCADDoc* m_pDoc ;
	virtual void OnCancel();
	void EnableUndo(BOOL enable) ; // nt add 2017/8/24
};
