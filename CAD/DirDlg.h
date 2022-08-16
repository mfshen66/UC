#pragma once

// CDirDlg �Ի���

class CDirDlg : public CDlg
{
	DECLARE_DYNAMIC(CDirDlg)

public:
	CDirDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDirDlg();

	STL* m_stl ;
	CCADDoc* m_pDoc ;
	PNT3D m_O ; // STLģ�͵ĳ�ʼ����

	// 0=������ת���, 1=����ѡ����棬2=����ƽ�������3=������ͼ�Ŵ�
	int m_mode ;

	RBALL* m_rb ; // rotating ball
	char m_axis ; // 'Z'=ʰȡ����Z�������XYƽ���Բ, 'X'=X, 'Y'=Y, 'B'=ʰȡ������

	int m_pick ; // 1=picked, 0=not picked
	PNT3D m_center ; // STLģ����һ��
	VEC3D m_normal ; // �õ㴦�ķ�ʸ
	int m_arrow ; // 1=������ʰȡ�㼰����0=δ����

	int m_lbd ;
	PNT3D m_mpos ; // m_inter=0״̬������������ʱ��λ��
	char m_axis2 ; // ��m_axis
	PNT3D m_pivot ; // ��ת����һ��
	VEC3D m_v ; // �����ƴ�����ת
	double m_a ; // һ��lbd��mm�ۼ���ת�ĽǶ�

	int m_move ; // 1=mouse is on mark of moving, 0=not, nt add 2017/1/17
	VEC3D m_vec ; // һ�ΰ����������϶�������ƽ����
	int m_zoom ; // 1=mouse is on mark of zooming, 0=not, nt add 2017/1/17
	double m_scale ; // һ�ΰ����������϶�������������
	PNT3D m_zc ; // zoom center

	int m_max ;
	int m_n ;
	TRANS* m_ts ;

	int m_cancel ; // nt add 2017/3/30

	void Init(STL* stl, CCADDoc* pDoc) ;
	void Rotate(char XYZ, VEC3D axis, double a, int undo) ; // һ������������ת, a�Ƕ�
	void Draw(void* pVI) ;
	void DrawArrow(void* pVI) ;
	void ErazeArrow(void* pVI) ;
	void DrawAxis(void* pVI, char axis) ;
	void ErazeAxis(void* pVI) ;
	void DrawMove(void* pVI, int isOn) ; // nt add 2017/1/17
	void DrawZoom(void* pVI, int isOn) ; // nt add 2017/1/17
	void DrawDoc() ;
	void Add(TRANS* t) ;
	void Push() ; // ���ۼ���ת��ѹ��m_ts��
	// return 0,1,2,3,4 the same as rballPick(), return 5=move mark, 6=zoom mark
	int Pick(double p[3], double v[3], void* pVI) ;
	void EnableUndo(BOOL enable) ; // nt add 2017/1/19
	
	int OnLBD(UINT nFlags, double p[3], double v[3], void* pVI) ;
	int OnMM(UINT nFlags, double p[3], double v[3], void* pVI) ;
	int OnRBD(UINT nFlags, double p[3], double v[3], void* pVI) ;
	int OnLBU(UINT nFlags, double p[3], double v[3], void* pVI) ;
	int OnReturn() ;
	void Notify(int msg) ;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	afx_msg void OnClickedButton3();
	afx_msg void OnClickedButton4();
	afx_msg void OnClickedButton5();
	afx_msg void OnClickedButton6();
	afx_msg void OnClickedButton7();
	afx_msg void OnClickedButton8();
	afx_msg void OnClickedButton9();
	afx_msg void OnClickedButton10();
	afx_msg void OnClickedButton11();
	afx_msg void OnClickedButton12();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	double m_xAngle;
	double m_yAngle;
	double m_zAngle;
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
};
