// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// CADDoc.h : CCADDoc ��Ľӿ�
//
#pragma once

#define MAX_VLT 100 // nt add 2017/6/2
class CCommand ;
class CCADView ;
struct _ss ;
typedef struct _ss SS ;
class CCADDoc : public CDocument
{
protected: // �������л�����
	CCADDoc();
	DECLARE_DYNCREATE(CCADDoc)

// ����
public:
	// �涨�����ռ��ڵ�һ���ޣ�ԭ������һ���ǵ�
	double m_ds[3]; // �����ռ���xyz���������ϵĶ���
	CCommand* m_pCmd ;
	CDlg* m_pDlg ; // nt add 2017/1/1 ����ID_EDIT_SIZE������
	STL* m_stls ; // STL����
	// m_unit�Ǹ�ϵ����������ļ�ʱ���ļ��ĵ�λ��Ӣ�磬��m_unitӦ������Ϊ25.4
	double m_unit ; // �ڲ�STLģ�͵ĵ�λ�涨Ϊmm
	STL* m_copy ; // ��m_stls��cut��copy�����ģ������ͷŴ˿ռ�(2017/9/2)
	int m_lastCmd ;

	// ״̬����
	double m_vol ; // nt add 2017/5/14 unit:mm^3
	int m_nLayer ; // nt add 2017/5/14 ����
	int m_iLayer ; // nt add 2017/8/28 ��ǰ��� 1<=m_iLayer<=m_nLayer
	int m_ptime ; // nt add 2017/8/27 ��ӡʱ�� ��λ=��
	int m_collision ; // nt add 2017/5/21

	// nt add 2017/2/18 ��ӡ������
	PARAMETER m_parameter ; // ��ӡ�������ϵȲ��� nt add 2017/5/8
	CString m_color ; // ������ɫ
	double m_h ; // ��� mm
	double m_lt ; // �ع�ʱ��, s
	double m_t0 ; //WMZ - ��ʼ����ʱ
	int m_tm ; //WMZ - �����˶�ʱ��
	int m_li ; // �ع�ǿ��, 0-100%
	double m_min ;
	double m_max ;

	// ֧�Ų���
	SSP m_ssp ; // nt add 2017/3/29
	SSP m_ssp2 ; // nt add 2017/8/26 ȱʡֵ

	// nt add 2017/5/13
	int m_constH ; // 1=�Ȳ��,0=����
	int m_connect ; // 1=�����Ӹ�,0=����

	// nt add 2017/6/2 ���ñ��棬������Ϣ
	int m_maxVLT ;
	int m_nVLT ;
	int (*m_vlts)[3] ;

	// nt add 2017/9/2
	COA m_rs ;

	// smf add 2023/04/24
	GridModel* m_grid;

// ����
public:
	int SaveDefault() ; // nt add 2017/2/18
	int LoadDefault() ; // nt add 2017/2/18
	void EmptyUndo() ; // nt add 2017/9/2
	void Add(STL* pSTL) ;
	int Remove(STL* pSTL) ; // û���ͷ�pSTL
	int Remove(int n) ; // ɾ�����ͷ�������ǰ��n��STLģ��
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
	int GetAllOfSelectedSTL(CSet* pSet) ; // ��ѡ�е�STLģ���ռ���CSet��
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

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CCADDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	VEC3D m_v;// smf add 2022/07/18

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenOld();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
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

	static void OutPutSline2(ZB2 *zb2); // 2022/08/17 smf add: ���Sline2����Ϣ
	static void OutPutCB(CB *cb, float z); // 2022/09/16 smf add: ���CB����Ϣ
	static void FindErrorCB(CB *cb); // 2022/09/16 smf add: ���CB����Ϣ
	static bool CCADDoc::bmpSave(CString& bmpName, unsigned char * imgBuf, int width, int height, int biBitCount, RGBQUAD * pColorTable);
};
//--------------------------------------------------------------
// nt add 2017/5/14
CCADDoc* cadGetDoc() ;
int bmp4Save(CString& fileName,
	int nWidth,
	int nHeight,
	void* bits);

