#pragma once


#include <afx.h>
#include <afxwin.h>
// CPropertyDefine 对话框

class CPropertyDefineDlg : public CDialog
{
	DECLARE_DYNAMIC(CPropertyDefineDlg)

public:
	CPropertyDefineDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPropertyDefineDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = "IDD_PROPERTY_DEFINE" };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
