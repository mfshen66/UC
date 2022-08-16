#pragma once

// CDirDlg 对话框

class CDirDlg : public CDlg
{
	DECLARE_DYNAMIC(CDirDlg)

public:
	CDirDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDirDlg();

	STL* m_stl ;
	CCADDoc* m_pDoc ;
	PNT3D m_O ; // STL模型的初始中心

	// 0=交互旋转零件, 1=交互选择底面，2=交互平移零件，3=交互视图放大
	int m_mode ;

	RBALL* m_rb ; // rotating ball
	char m_axis ; // 'Z'=拾取到了Z轴绘制了XY平面大圆, 'X'=X, 'Y'=Y, 'B'=拾取到了球

	int m_pick ; // 1=picked, 0=not picked
	PNT3D m_center ; // STL模型上一点
	VEC3D m_normal ; // 该点处的法矢
	int m_arrow ; // 1=绘制了拾取点及方向，0=未绘制

	int m_lbd ;
	PNT3D m_mpos ; // m_inter=0状态下鼠标左键按下时的位置
	char m_axis2 ; // 见m_axis
	PNT3D m_pivot ; // 旋转轴上一点
	VEC3D m_v ; // 正在绕此轴旋转
	double m_a ; // 一次lbd和mm累计旋转的角度

	int m_move ; // 1=mouse is on mark of moving, 0=not, nt add 2017/1/17
	VEC3D m_vec ; // 一次按下鼠标左键拖动产生的平移量
	int m_zoom ; // 1=mouse is on mark of zooming, 0=not, nt add 2017/1/17
	double m_scale ; // 一次按下鼠标左键拖动产生的缩放量
	PNT3D m_zc ; // zoom center

	int m_max ;
	int m_n ;
	TRANS* m_ts ;

	int m_cancel ; // nt add 2017/3/30

	void Init(STL* stl, CCADDoc* pDoc) ;
	void Rotate(char XYZ, VEC3D axis, double a, int undo) ; // 一定是绕中心旋转, a是度
	void Draw(void* pVI) ;
	void DrawArrow(void* pVI) ;
	void ErazeArrow(void* pVI) ;
	void DrawAxis(void* pVI, char axis) ;
	void ErazeAxis(void* pVI) ;
	void DrawMove(void* pVI, int isOn) ; // nt add 2017/1/17
	void DrawZoom(void* pVI, int isOn) ; // nt add 2017/1/17
	void DrawDoc() ;
	void Add(TRANS* t) ;
	void Push() ; // 把累计旋转角压入m_ts中
	// return 0,1,2,3,4 the same as rballPick(), return 5=move mark, 6=zoom mark
	int Pick(double p[3], double v[3], void* pVI) ;
	void EnableUndo(BOOL enable) ; // nt add 2017/1/19
	
	int OnLBD(UINT nFlags, double p[3], double v[3], void* pVI) ;
	int OnMM(UINT nFlags, double p[3], double v[3], void* pVI) ;
	int OnRBD(UINT nFlags, double p[3], double v[3], void* pVI) ;
	int OnLBU(UINT nFlags, double p[3], double v[3], void* pVI) ;
	int OnReturn() ;
	void Notify(int msg) ;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRDLG };
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
