// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// CADDoc.h : CCADDoc 类的接口
//
#pragma once

#define MAX_VLT 100 // nt add 2017/6/2
class CCommand ;
class CCADView ;
struct _ss ;
typedef struct _ss SS ;
class CCADDoc : public CDocument
{
protected: // 仅从序列化创建
	CCADDoc();
	DECLARE_DYNCREATE(CCADDoc)

// 特性
public:
	// 规定工作空间在第一象限，原点是其一个角点
	double m_ds[3]; // 工作空间在xyz三个方向上的度量
	CCommand* m_pCmd ;
	CDlg* m_pDlg ; // nt add 2017/1/1 用于ID_EDIT_SIZE等命令
	STL* m_stls ; // STL链表
	// m_unit是个系数，例如打开文件时若文件的单位是英寸，则m_unit应该设置为25.4
	double m_unit ; // 内部STL模型的单位规定为mm
	STL* m_copy ; // 从m_stls中cut或copy出来的，无需释放此空间(2017/9/2)
	int m_lastCmd ;

	// 状态参数
	double m_vol ; // nt add 2017/5/14 unit:mm^3
	int m_nLayer ; // nt add 2017/5/14 层数
	int m_iLayer ; // nt add 2017/8/28 当前层号 1<=m_iLayer<=m_nLayer
	int m_ptime ; // nt add 2017/8/27 打印时间 单位=秒
	int m_collision ; // nt add 2017/5/21

	// nt add 2017/2/18 打印机设置
	PARAMETER m_parameter ; // 打印机及材料等参数 nt add 2017/5/8
	CString m_color ; // 材料颜色
	double m_h ; // 厚度 mm
	double m_lt ; // 曝光时间, s
	double m_t0 ; //WMZ - 初始化用时
	int m_tm ; //WMZ - 单层运动时间
	int m_li ; // 曝光强度, 0-100%
	double m_min ;
	double m_max ;

	// 支撑参数
	SSP m_ssp ; // nt add 2017/3/29
	SSP m_ssp2 ; // nt add 2017/8/26 缺省值

	// nt add 2017/5/13
	int m_constH ; // 1=等层厚,0=变层厚
	int m_connect ; // 1=带连接杆,0=不带

	// nt add 2017/6/2 不用保存，变层厚信息
	int m_maxVLT ;
	int m_nVLT ;
	int (*m_vlts)[3] ;

	// nt add 2017/9/2
	COA m_rs ;

	// smf add 2023/04/24
	GridModel* m_grid;

// 操作
public:
	int SaveDefault() ; // nt add 2017/2/18
	int LoadDefault() ; // nt add 2017/2/18
	void EmptyUndo() ; // nt add 2017/9/2
	void Add(STL* pSTL) ;
	int Remove(STL* pSTL) ; // 没有释放pSTL
	int Remove(int n) ; // 删除并释放链表最前面n个STL模型
	int GetNumOfSTL() ;
	STL* GetFirstSTL() ;
	void DelAllSTL() ;
	void SetUnit(double unit) ;
	double GetUnit() ;
	CCADView* GetView() ;
	void GetCenter(PNT3D center); // center of work space
	void GetSpace(PNT3D min, PNT3D max); // work space of the printer
	void CalBox(BOX3D* box) ; // nt add 2017/5/26
	double GetZMax() ; // nt add 2017/5/27
	double CalH(double z) ; // nt add 2017/6/2
	int HasZB() ; // nt add 2017/5/27
	double GetMeanZ() ; // nt add 2017/5/27
	 // 2022/08/16 smf modify: double to float
	void CutAll(float z) ; // nt add 2017/5/27
	void Draw(int drawMode, void* pVI) ;
	void Redraw() ;
	void ClearCmd() ;
	void ClearCmdAll() ;
	int HasCmd() ;
	void InitCmd(int cmdID) ; // nt add 2016/12/31
	void SetLastCmd(int cmdID) ;
	int GetLastCmd() ;
	int OnReturn() ; // nt add 2016/12/31
	void GetParameter(PARAMETER* parameter) ; // nt add 2017/5/8
	void SetParameter(PARAMETER* parameter) ; // nt add 2017/5/8
	//void GetLogFont(LOGFONT* pLF) ;
	int GetNumOfSelectedSTL() ;
	int GetAllOfSelectedSTL(CSet* pSet) ; // 把选中的STL模型收集到CSet中
	int GetAllOfSelectedSTL2(CSet* pSet) ; // nt add 2017/8/23
	void SetPromptingSTL(STL* stl, PNT3D p, STLTRI* tri, void* pVI) ;
	void SetSelected(int nFlags, void* pVI) ;
	int Pick(STLPCK* pi) ; // 0=error, 1=picked, 2=not picked
	void OnMouseMove(int nFlags, double p[3], double v[3], void* pVI) ;
	void OnLButtonDown(int nFlags, double p[3], double v[3], void* pVI) ;
	void OnLButtonUp(int nFlags, double p[3], double v[3], void* pVI) ;
	void OnRButtonDown(int nFlags, double p[3], double v[3], void* pVI) ;
	void OnRButtonUp(int nFlags, double p[3], double v[3], void* pVI) ;
	void Update() ; // nt add 2017/8/27
	void MakeSupport(PRG* pPrg) ; // nt modify 2017/9/10
	void ClearSupport() ;
	void Print(PRG* pPrg) ; // nt modify 2017/9/9
	void CBFill(int iType, double iLength, double iWidth, double iHeight, 
				int iShape, double iRadiasE, double iRadiasV, int iRemoveOption, PRG* pPrg);
	void Out(CString& filePath, PRG* pPrg, BOOL gray, int gvm) ; // output printing file(*.p_f) nt add 2017/5/14
	void CollisionDetect() ; // nt add 2017/5/21
	void CollisionDetect2(PRG* pPrg) ; // nt add 2017/10/1
	void ExportSTL(CString& pathName); // nt add 2019/4/13
	void SetPropertySTL(short proper);
	int IsPropertySetAllStl();

	void Add(GridModel* pGM);
	int GetNumberOfGridModel();

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CCADDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	VEC3D m_v;// smf add 2022/07/18

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenOld();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditSize();
	afx_msg void OnEditDir();
	afx_msg void OnEditHeight();
	afx_msg void OnEditDelSupport();
	afx_msg void OnEditSupport();
	afx_msg void OnEditRod();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditCancelSel();
	afx_msg void OnEditSTLMaterial();
	afx_msg void OnInsertSupport();
	afx_msg void OnInsertLayout();
	afx_msg void OnPrintSet();
	//afx_msg void OnPrintInit();
	afx_msg void OnPrint();
	afx_msg void OnPrintPreview();
	afx_msg void OnHelpShortCutKeys();
	afx_msg void OnHelpSupport();
	afx_msg void OnHelpFeedback() ;
	afx_msg void OnButtonDef();
	afx_msg void OnButtonCBFill();
	//afx_msg void OnComboDef();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

	static void OutPutSline2(ZB2 *zb2); // 2022/08/17 smf add: 输出Sline2的信息
	static void OutPutCB(CB *cb, float z); // 2022/09/16 smf add: 输出CB的信息
	static void FindErrorCB(CB *cb); // 2022/09/16 smf add: 输出CB的信息
	static bool CCADDoc::bmpSave(CString& bmpName, unsigned char * imgBuf, int width, int height, int biBitCount, RGBQUAD * pColorTable);
};
//--------------------------------------------------------------
// nt add 2017/5/14
CCADDoc* cadGetDoc() ;
int bmp4Save(CString& fileName,
	int nWidth,
	int nHeight,
	void* bits);

