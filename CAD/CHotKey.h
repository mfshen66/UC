#pragma once


// CHotKey �Ի���

class CHotKey : public CDialogEx
{
	DECLARE_DYNAMIC(CHotKey)

public:
	CHotKey(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHotKey();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOTKEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
