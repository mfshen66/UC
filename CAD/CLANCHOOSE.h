#pragma once


// CLANCHOOSE �Ի���

class CLANCHOOSE : public CDialogEx
{
	DECLARE_DYNAMIC(CLANCHOOSE)

public:
	CLANCHOOSE(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLANCHOOSE();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_START };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
