#pragma once


// CLANSAVEBOX 对话框

class CLANSAVEBOX : public CDialog
{
	DECLARE_DYNAMIC(CLANSAVEBOX)

public:
	CLANSAVEBOX(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLANSAVEBOX();



// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ML_SAVEBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();




};
