#pragma once


// CHotKey 对话框

class CHotKey : public CDialogEx
{
	DECLARE_DYNAMIC(CHotKey)

public:
	CHotKey(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHotKey();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOTKEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
