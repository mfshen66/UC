#pragma once


// CLANSAVEBOX �Ի���

class CLANSAVEBOX : public CDialog
{
	DECLARE_DYNAMIC(CLANSAVEBOX)

public:
	CLANSAVEBOX(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLANSAVEBOX();



// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ML_SAVEBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();




};
