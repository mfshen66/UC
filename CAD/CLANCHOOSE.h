#pragma once


// CLANCHOOSE 对话框

class CLANCHOOSE : public CDialogEx
{
	DECLARE_DYNAMIC(CLANCHOOSE)

public:
	CLANCHOOSE(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLANCHOOSE();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_START };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
