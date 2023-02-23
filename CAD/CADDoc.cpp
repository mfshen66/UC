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

// CADDoc.cpp : CCADDoc 类的实现
//

#include "stdafx.h"
#include "cstring"
#include "resource.h"
#include "CAD.h"
#include "CADDoc.h"
#include "CADView.h"
#include "Command.h"
#include "DocCmd.h"
#include <propkey.h>
#include "MainFrm.h"
#include "SizeDlg.h"
#include "DirDlg.h"
#include "LayoutDlg.h"
#include "PrinterSetupDlg.h"
#include "SSPDlg.h"
#include "SSDlg.h"
#include "PrintDlg.h"
#include "Shell.h" // nt add 2017/10/10
#include "FeedbackDlg.h"
#include "LANSAVEBOX.h"
#include "CHotKey.h"
#include "CB.h" // nt add 2021/6/4
#include "CPropertyDefineDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCADDoc

IMPLEMENT_DYNCREATE(CCADDoc, CDocument)

BEGIN_MESSAGE_MAP(CCADDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_EDIT_UNDO, &CCADDoc::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CCADDoc::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_SIZE, &CCADDoc::OnEditSize)
	ON_COMMAND(ID_EDIT_DIR, &CCADDoc::OnEditDir)
	ON_COMMAND(ID_EDIT_HEIGHT, &CCADDoc::OnEditHeight)
	ON_COMMAND(ID_EDIT_DELSUPPORT, &CCADDoc::OnEditDelSupport)
	ON_COMMAND(ID_EDIT_SUPPORT, &CCADDoc::OnEditSupport)
	ON_COMMAND(ID_EDIT_ROD, &CCADDoc::OnEditRod)
	ON_COMMAND(ID_EDIT_CUT, &CCADDoc::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CCADDoc::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CCADDoc::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CCADDoc::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_DELETE, &CCADDoc::OnEditDelete)
	ON_COMMAND(ID_EDIT_SELECTALL, &CCADDoc::OnEditSelectAll)
	ON_COMMAND(ID_EDIT_CANCELSEL, &CCADDoc::OnEditCancelSel)
	ON_COMMAND(ID_EDIT_STLMATERIAL, &CCADDoc::OnEditSTLMaterial)
	ON_COMMAND(ID_INSERT_SUPPORT, &CCADDoc::OnInsertSupport)
	ON_COMMAND(ID_INSERT_LAYOUT, &CCADDoc::OnInsertLayout)
	ON_COMMAND(ID_PRINT_SET, &CCADDoc::OnPrintSet)
	ON_COMMAND(ID_PRINT, &CCADDoc::OnPrint)
	ON_COMMAND(ID_PRINT_PREVIEW, &CCADDoc::OnPrintPreview)
	ON_COMMAND(ID_HELP_SHORTCUTKEYS, &CCADDoc::OnHelpShortCutKeys)
	ON_COMMAND(ID_HELP_SUPPORT, &CCADDoc::OnHelpSupport)
	ON_COMMAND(ID_HELP_FEEDBACK, &CCADDoc::OnHelpFeedback)
	ON_COMMAND(ID_BUTTON_DEF, &CCADDoc::OnButtonDef)

	//ON_COMMAND(ID_COMBO_DEF, &CCADDoc::OnComboDef)
	//ON_BN_CLICKED(IDOK, &CCADDoc::OnSWLOk)
	//ON_BN_CLICKED(IDCANCEL, &CCADDoc::OnSWLCancel)
END_MESSAGE_MAP()
CString GetModuleDir_doc()
{
	HMODULE module = GetModuleHandle(0);
	char pFileName[MAX_PATH];
	GetModuleFileNameA(module, pFileName, MAX_PATH);

	CString csFullPath(pFileName);
	int nPos = csFullPath.ReverseFind(_T('\\'));
	if (nPos < 0)
		return CString("");
	else
		return csFullPath.Left(nPos);
}
BOOL GetLang_doc(void)
{
	CString g_strCurrPath = GetModuleDir_doc();
	BOOL bEnglish = GetPrivateProfileInt(_T("English"), _T("bEnglish"), 0, g_strCurrPath + _T("\\setlang.ini"));

	return bEnglish;
}
//BOOL b_mLangFlag = GetLang_doc();
///////////////////////////////////////////////////////////////
// CCADDoc 构造/析构
CCADDoc::CCADDoc()
{
	// TODO: 在此添加一次性构造代码
	m_ds[0] = 1000.; // smf modify 2022/07/18
	m_ds[1] = 1000.; // smf modify 2022/07/18
	m_ds[2] = 1000.; // smf modify 2022/07/18
	m_pCmd = NULL ;
	m_pDlg = NULL ; // nt add 2017/1/1
	m_stls = NULL ;
	m_unit = 1. ;
	m_copy = NULL ; // nt add 2016/12/30
	m_lastCmd = -1 ; // nt add 2016/12/31
	m_vol = 0. ; // nt add 2017/5/14
	m_nLayer = 0 ; // nt add 2017/5/14
	m_iLayer = 0 ; // nt add 2017/8/28
	m_ptime = 0 ; // nt add 2017/8/27
	m_collision = 0 ; // nt add 2017/5/21

	// nt add 2017/2/18 打印机设置
	m_h = 0.1 ; // nt add 2017/5/11
	m_lt = 0 ; // nt add 2017/5/11

	m_t0 = 60 ;//预设值
	m_tm = 40 ;

	m_li = 80 ; // nt add 2017/5/11
	//if (!b_mLangFlag)
	m_color =_T("黑色");
	//else
		//m_color = _T("Black");
	m_min = 0.025 ;
	m_max = 0.2 ;
	sspInit(&m_ssp, 
		    45., // 底座斜度 old PI1/180*45, nt modify 2017/5/11
		    2., // 底座高度
		    5., // 间距
		    0.3, // 支撑点半径
		    2., // 从支撑点沿法矢延伸长度
		    1.2, // 支撑底部锥体半径
		    0.6, // 支撑圆柱半径
		    45., // 大于这个角度不加支撑
		    1.e-8) ;
	m_ssp2 = m_ssp ; // nt add 2017/8/27

	// nt add 2017/5/13
	m_constH = 1 ;
	m_connect = 0 ;

	// nt add 2017/6/2
	m_maxVLT = MAX_VLT ;
	m_nVLT = 0 ;
	m_vlts = new int[MAX_VLT][3] ;

	// smf add 2022/07/19
	m_v[0] = 0.;
	m_v[1] = 0.;
	m_v[2] = 0.;

	// nt add 2017/5/8
	parameterInit(&m_parameter) ;

	// nt add 2017/2/18 load default parameters
	LoadDefault() ;

	return ;
}

CCADDoc::~CCADDoc()
{
	if( m_pCmd )
		delete m_pCmd ;

	// nt add 2017/1/1
	if( m_pDlg )
		delete m_pDlg ;

	DelAllSTL() ;

	m_copy = NULL ;

	// nt add 2017/6/2
	delete []m_vlts ;

	// nt add 2017/2/18 save default parameters
	SaveDefault() ;

	// nt add 2017/9/2
	EmptyUndo() ;

	return ;
}
//-------------------------------------------------------------
BOOL CCADDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	Update() ; // nt add 2017/9/7

	return TRUE;
}

// nt add 2017/9/2
BOOL CCADDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString pathName = lpszPathName ;
	int n = pathName.GetLength() ;
	if( n >= 4 &&
		(pathName[n-3] == 's' || pathName[n-3] == 'S') &&
		(pathName[n-2] == 't' || pathName[n-2] == 'T') &&
		(pathName[n-1] == 'l' || pathName[n-1] == 'L') )
	{
		pathName.SetAt(n-2, _T('w')) ;
		pathName.SetAt(n-1, _T('k')) ;
	}

	return CDocument::OnSaveDocument(pathName);
}

// CCADDoc 序列化
// nt add 2017/8/31
void CCADDoc::Serialize(CArchive& ar)
{
	int i, n ;
	STL* stl, *prev = NULL ;
	CCADView* pView = NULL ;
	void* pVI = NULL ;
	VINFO vinfo ;

	// nt modify 2017/10/10
	pView = cadGetView() ;
	if( pView )
		pVI = pView->GetVI() ;

	if( ar.IsStoring() )
	{
		n = GetNumOfSTL() ;
		ar << m_unit
		   << m_vol
		   << m_nLayer
		   << m_iLayer
		   << m_color
		   << m_h
		   << m_lt
		   << m_li
		   << m_min
		   << m_max
		   << m_constH
		   << m_connect
		   << m_maxVLT
		   << m_nVLT
		   << n ;
		ar.Write(&m_ssp, sizeof(SSP)) ;
		if( m_nVLT > 0 )
			ar.Write(m_vlts, sizeof(int[3])*m_nVLT) ;
		for( stl = m_stls ; stl ; stl = stl->next )
			stlSerialize(stl, ar) ;

		if( pVI ) // this line : nt add 2017/10/10
			viSave(pVI, &vinfo) ;
		ar.Write(&vinfo, sizeof(VINFO)) ;
	}
	else
	{
		ar >> m_unit
		   >> m_vol
		   >> m_nLayer
		   >> m_iLayer
		   >> m_color
		   >> m_h
		   >> m_lt
		   >> m_li
		   >> m_min
		   >> m_max
		   >> m_constH
		   >> m_connect
		   >> m_maxVLT
		   >> m_nVLT
		   >> n ;
		m_copy = NULL ;
		m_lastCmd = -1 ;
		ar.Read(&m_ssp, sizeof(SSP)) ;
		if( m_nVLT > 0 )
			ar.Read(m_vlts, sizeof(int[3])*m_nVLT) ;
		for( i = 0 ; i < n ; i++ )
		{
			stl = stlCreate2() ;
			stlSerialize(stl, ar) ;
			if( i > 0 )
				prev->next = stl ;
			else
				m_stls = stl ;
			prev = stl ;
		}

		ar.Read(&vinfo, sizeof(VINFO)) ;
		if( pVI ) // this line : nt add 2017/10/10
			viLoad(pVI, &vinfo) ;
	}

	return ;
}
//-------------------------------------------------------------
// CCADDoc 诊断

#ifdef _DEBUG
void CCADDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCADDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
//-------------------------------------------------------------
// attributes
// nt add 2017/2/18
int CCADDoc::SaveDefault()
{
	CString filePath ;
	efpGet(filePath) ; // 运行文件目录
	filePath += _T("\\Libs\\Default.dat") ;
	CFile file ;
	if( file.Open(filePath, CFile::modeCreate | CFile::modeWrite) )
	{
		CArchive ar(&file, CArchive::store) ;
		ar << m_h
		   << m_lt
		   << m_li
		   << m_color
		   << m_min
		   << m_max ;
		ar.Write(&m_ssp, sizeof(SSP)) ;
		ar.Write(&m_ssp2, sizeof(SSP)) ; // nt add 2017/8/26
		ar.Write(&m_parameter, sizeof(PARAMETER)) ; // nt add  2017/5/8
		ar.Close() ;
		file.Close() ;
		return 1 ;
	}
	else
		return 0 ;
	/*CString exePath ;
	efpGet(exePath) ; // 运行文件目录
	exePath += _T("\\Libs\\P_M\\") ;
	//_tcscpy_s(m_parameter.printer, 64, _T("printer1")) ;
	for( int i = 5 ; i < 10 ; i++ )
	{
		CString filePath ;
		filePath.Format(_T("%d.p_m"), i) ;
		filePath = exePath+filePath ;
		CFile file ;
		if( file.Open(filePath, CFile::modeCreate | CFile::modeWrite) )
		{
			CArchive ar(&file, CArchive::store) ;
			CString s ;
			s.Format(_T("material%d"), i-5) ;
			_tcscpy_s(m_parameter.material, 64, s.GetBuffer(0)) ;
			ar.Write(&m_parameter, sizeof(PARAMETER)) ;
			ar.Close() ;
			file.Close() ;
		}
	}

	return 1 ;*/
}

// nt add 2017/2/18
int CCADDoc::LoadDefault()
{
	CString filePath ;
	efpGet(filePath) ; // 运行文件目录
	filePath += _T("\\Libs\\Default.dat") ;
	CFile file ;
	if( file.Open(filePath, CFile::modeRead) )
	{
		CArchive ar(&file, CArchive::load) ;
		ar >> m_h
		   >> m_lt
		   >> m_li
		   >> m_color
		   >> m_min
		   >> m_max ;
		ar.Read(&m_ssp, sizeof(SSP)) ;
		ar.Read(&m_ssp2, sizeof(SSP)) ;
		//m_ssp2 = m_ssp ;
		ar.Read(&m_parameter, sizeof(PARAMETER)) ; // nt add  2017/5/8
		ar.Close() ;
		file.Close() ;
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2017/9/2
void CCADDoc::EmptyUndo()
{
	int i, n = (int)m_rs.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CRecord* pR = (CRecord*)m_rs[i] ;
		if( pR )
			delete pR ;
	}
	m_rs.RemoveAll() ;

	m_copy = NULL ;

	SetModifiedFlag(FALSE) ;

	return ;
}

void CCADDoc::Add(STL* pSTL)
{
	if( pSTL )
	{
		pSTL->next = m_stls ;
		m_stls = pSTL ;

		m_vol += stlCalVol(pSTL) ; // nt add 2017/5/14
		Update() ; // nt add 2017/8/27
	}

	return ;
}

// 没有释放pSTL
int CCADDoc::Remove(STL* pSTL)
{
	if( pSTL )
	{
		STL* prev = NULL ; // previous to temp
		STL* temp = m_stls ;
		while(temp)
		{
			if( temp == pSTL )
			{
				if( prev == NULL ) // pSTL是链表的头
					m_stls = temp->next ;
				else
					prev->next = temp->next ;
				pSTL->next = NULL ;
				m_vol -= stlCalVol(pSTL) ; // nt add 2017/5/14
				if( m_stls == NULL )
					m_vol = 0. ;
				Update() ; // nt add 2017/8/27

				return 1 ;
			}
			prev = temp ;
			temp = temp->next ;
		}
	}

	return 0 ;
}

// 删除并释放链表最前面n个STL模型
int CCADDoc::Remove(int n)
{
	int i, m = GetNumOfSTL() ;
	STL* stl ;

	if( m >= n )
	{
		for( i = 0 ; i < n ; i++ )
		{
			stl = GetFirstSTL() ;
			if( stl )
			{
				Remove(stl) ;
				stlFree(stl) ;
			}
		}

		return 1 ;
	}
	else
		return 0 ;
}

int CCADDoc::GetNumOfSTL()
{
	int n = 0 ;
	STL* pSTL ;

	for( pSTL = m_stls ; pSTL != NULL ; pSTL = pSTL->next )
		n++ ;

	return n ;
}

STL* CCADDoc::GetFirstSTL()
{
	return m_stls ;
}

void CCADDoc::DelAllSTL()
{
	STL* pSTL = m_stls ;
	while(pSTL)
	{
		STL* temp = pSTL ;
		pSTL = pSTL->next ;
		stlFree(temp) ;
	}
	m_stls = NULL ;
	m_vol = 0. ;
	m_nLayer = 0 ;

	return ;
}
//-------------------------------------------------------------
void CCADDoc::SetUnit(double unit)
{
	m_unit = unit ;
}

double CCADDoc::GetUnit()
{
	return m_unit ;
}

CCADView* CCADDoc::GetView()
{
	POSITION pos = GetFirstViewPosition() ;
	if(pos != NULL)
		return (CCADView*)GetNextView(pos) ;
	else
		return NULL ;
}

void CCADDoc::GetCenter(PNT3D center)
{
	center[0] = 0. ;
	center[1] = 0. ;
	center[2] = 0.5*m_ds[2];

	return;
}

void CCADDoc::GetSpace(PNT3D min, PNT3D max)
{
	min[0] = -m_ds[0]/2 ;
	min[1] = -m_ds[1]/2 ; // nt modify 2017/5/11
	min[2] = 0. ;
	max[0] = m_ds[0]/2 ;
	max[1] = m_ds[1]/2 ;
	max[2] = m_ds[2] ;

	return;
}

// nt add 2017/5/26
void CCADDoc::CalBox(BOX3D* box)
{
	int k ;
	BOX3D temp ;
	STL* stl ;

	if( m_stls )
	{
		mathClnBox(box) ;
		for( stl = m_stls ; stl ; stl = stl->next )
		{
			stlGetBox(stl, &temp) ;
			for( k = 0 ; k < 3 ; k++ )
			{
				if( box->min[k] > temp.min[k] )
					box->min[k] = temp.min[k] ;
				if( box->max[k] < temp.max[k] )
					box->max[k] = temp.max[k] ;
			}
		}
	}
	else // nt add 2017/7/7
		GetSpace(box->min, box->max) ;

	return ;
}

// nt add 2017/5/27
double CCADDoc::GetZMax()
{
	BOX3D box ;
	CalBox(&box) ;
	return box.max[2] ;
}

// nt add 2017/6/2
double CCADDoc::CalH(double z)
{
	double h0 = m_parameter.ts[0]*0.001 ; // 头一层的厚度
	if( z < 0.9*h0 ) // 不超头层
		return h0 ;
	else
	if( m_constH == 1 ) // 等层厚
		return m_h ;
	else // 变层厚
	{
		double z1, z2, h ; // h是理想厚度
		for( int i = 0 ; i < m_nVLT ; i++ )
		{
			z1 = h0+m_vlts[i][0]*m_h ;
			z2 = h0+m_vlts[i][1]*m_h ;
			if( z1-MIN_DBL < z &&
				z < z2+MIN_DBL )
			{
				h = 0.001*m_vlts[i][2]*m_h ;
				int k, I = -1 ;
				double d, min = MAX_DBL ;
				for( k = 1 ; k < 6 ; k++ )
				{
					d = fabs(h-m_parameter.ts[k]*0.001) ;
					if( min > d )
					{
						min = d ;
						I = k ;
					}
				}
				if( I >= 1 )
					return m_parameter.ts[I]*0.001 ;
			}
		}

		return m_h ;
	}
}

// nt add 2017/5/27
int CCADDoc::HasZB()
{
	for( STL* stl = m_stls ; stl ; stl = stl->next )
	{
		if( stl->zb == NULL )
			return 0 ;
	}

	return 1 ;
}

// nt add 2017/5/27
double CCADDoc::GetMeanZ()
{
	BOX3D box ;
	CalBox(&box) ;
	return 0.5*(box.min[2]+box.max[2]) ;
}

// nt add 2017/5/27
void CCADDoc::CutAll(float z) // 2022/08/16 smf modify: double to float
{

	for( STL* stl = m_stls ; stl ; stl = stl->next )
	{
		ZB* zb = (ZB*)(stl->zb) ;
		if( zb )
			zbCut(zb, z) ;
		ZB2* zb2 = (ZB2*)(stl->zb2) ; // nt add 2017/5/30
		if( zb2 ) // nt add 2017/5/30
			zb2Cut(zb2, z) ; // nt add 2017/5/30
		
		// nt add 2021/6/7
		if (stl->cb)
		{
			double w = m_parameter.L / m_parameter.PL; // 2022/09/28 smf add
			double r1 = ((CB*)stl->cb)->r1;
			ZB2* zb3 = NULL;
			BOX3D box;
			memcpy(&box, &stl->box, sizeof(BOX3D));
			box.min[0] -= r1;
			box.min[1] -= r1;
			box.min[2] -= r1;
			if (stl->zb3)
				zb2Free((ZB2*)(stl->zb3));
			zb3 = zb2Create(&box, w, m_h, 1.e-6, 1.e-11); // 2022/08/17 smf modify: 1.e-8 to 1.e-6
			zb2SliceCBOnZ(zb3, (CB*)stl->cb, z);
			stl->zb3 = zb3;
			zb2Cut(zb3, z);
			zb3 = NULL;
		}

		// end
	}

	return ;
}

void CCADDoc::ClearCmd()
{
	if( m_pCmd )
	{
		m_pCmd->Cancel() ;
		delete m_pCmd ;
		m_pCmd = NULL ;
	}

	// nt add 2017/1/1
	if( m_pDlg )
	{
		delete m_pDlg ;
		m_pDlg = NULL ;
	}

	// nt modify 2017/8/28
	CCADView* pView = cadGetView() ;
	if( pView )
		pView->SetDrawSection(0) ;

	return ;
}

void CCADDoc::ClearCmdAll()
{
	ClearCmd() ;

	// nt modify 2017/8/28
	CCADView* pView = cadGetView() ;
	if( pView )
		pView->ClearCmd() ;

	return ;
}

int CCADDoc::HasCmd()
{
	if( m_pCmd ||
		m_pDlg )
		return 1 ;
	else
		return 0 ;
}

void CCADDoc::SetLastCmd(int cmdID)
{
	m_lastCmd = cmdID ;
}

int CCADDoc::GetLastCmd()
{
	return m_lastCmd ;
}

// nt add 2016/12/31
void CCADDoc::InitCmd(int cmdID)
{
	ClearCmdAll() ; // 清除已有的命令
	SetLastCmd(cmdID) ; // 保存命令ID
	cadPromptClear() ;

	return ;
}

int CCADDoc::OnReturn()
{
	if( m_pCmd )
		m_pCmd->OnReturn() ;
	else
	{
		int lastCmd = GetLastCmd() ;
		if( lastCmd != -1 )
		{
			SetLastCmd(-1) ;
			CCADView* pView = GetView() ;
			if( pView )
				pView->SendMessage(WM_COMMAND, lastCmd) ;
		}
	}

	return 1 ;
}

// nt add 2017/5/8
void CCADDoc::GetParameter(PARAMETER* parameter)
{
	if( parameter )
		*parameter = m_parameter ;

	return ;
}

// nt add 2017/5/8
void CCADDoc::SetParameter(PARAMETER* parameter)
{
	if( parameter )
		m_parameter = *parameter ;

	return ;
}

// 绘制模型
void CCADDoc::Draw(int drawMode, void* pVI)
{
	STL* pSTL ;

	// STL model
	viLighting(pVI, 1) ;
	viAddDefaultLight(pVI) ;
	glColor3ub(0, 0, 0) ;
	for( pSTL = m_stls ; pSTL != NULL ; pSTL = pSTL->next )
	{
		// 显示方式1=只画STL
		//stlDraw(pSTL, pVI) ;

		// 显示方式2=画STL或线框CELL
		if( pSTL->cb == NULL ) // nt add 2021/6/6
			stlDraw(pSTL, pVI) ;
		else
			cbDraw((CB*)pSTL->cb, pVI) ; // nt add 2021/6/6

		if( pSTL->ss &&
			((SS*)(pSTL->ss))->editing != 2 )
			ssDraw((SS*)(pSTL->ss), pVI) ;
	}
	viLighting(pVI, 0) ;

	// nt add 2017/6/7
	if( m_pCmd &&
		m_pCmd->GetID() == dctEditRod )
	{
		SS* ss = ((CEditRod*)m_pCmd)->GetSS() ;
		if( ss &&
			ss->editing == 2 )
		{
			glEnable(GL_DEPTH_TEST) ;
			ssDraw(ss, pVI) ;
			glDisable(GL_DEPTH_TEST) ;
		}
	}

	// notify model redraw
	if( m_pDlg )
		m_pDlg->Draw(pVI) ;

	// nt add 2017/3/29
	if( m_pCmd )
		m_pCmd->Notify(1) ;
	
	return ;
}

void CCADDoc::Redraw()
{
	CCADView* pView = GetView() ;
	if( pView )
		pView->Invalidate(FALSE) ;

	return ;
}

int CCADDoc::GetNumOfSelectedSTL()
{
	int n = 0 ;
	STL* stl ;

	for( stl = m_stls ; stl != NULL ; stl = stl->next )
	{
		if( stl->stt == stlSelected )
			n++ ;
	}

	return n ;
}

// 把选中的STL模型收集到CSet中
int CCADDoc::GetAllOfSelectedSTL(CSet* pSet)
{
	STL* stl ;

	if( pSet == NULL )
		return -1 ;

	for( stl = m_stls ; stl != NULL ; stl = stl->next )
	{
		if( stl->stt == stlSelected )
			pSet->AddObj(stl) ;
	}

	return pSet->GetNumOfObj() ;
}

// nt add 2017/8/23 找到所有选择的STL，如果没有选择的stl就
// 返回所有的STL
int CCADDoc::GetAllOfSelectedSTL2(CSet* pSet)
{
	int n ;
	STL* stl ;

	n = GetAllOfSelectedSTL(pSet) ;
	if( n != 0 )
		return n ;

	for( stl = m_stls ; stl != NULL ; stl = stl->next )
		pSet->AddObj(stl) ;

	return pSet->GetNumOfObj() ;
}


void CCADDoc::SetPromptingSTL(STL* stl, PNT3D p, STLTRI* tri, void* pVI)
{
	int refresh = 0 ;
	STL* pSTL ;

	viLighting(pVI, 1) ;
	viAddDefaultLight(pVI) ;

	// eraze the old STL
	for( pSTL = m_stls ; pSTL != NULL ; pSTL = pSTL->next )
	{
		if( pSTL->stt == stlPrompting )
		{
			if( pSTL == stl )
			{
				viLighting(pVI, 0) ; // nt add 2017/3/30
				return ;
			}
			pSTL->stt = stlNormal ;
			stlDraw(pSTL, pVI) ;
			refresh = 1 ;
			break ; // nt add 2017/3/30
		}
	}

	// draw the new STL
	if( stl &&
		p &&
		tri )
	{
		memcpy(stl->p, p, sizeof(double[3])) ; // 捕捉点
		stl->tri = *tri ; // 捕捉的三角形
		stl->stt = stlPrompting ;
		stlDraw(stl, pVI) ;
		refresh = 1 ;
	}

	viLighting(pVI, 0) ;

	if( refresh )
		glFlush() ;

	return ;
}

void CCADDoc::SetSelected(int nFlags, void* pVI)
{
	int refresh = 0 ;
	STL* pSTL ;

	viLighting(pVI, 1) ;
	viAddDefaultLight(pVI) ;

	if( !(nFlags & MK_CONTROL) ) // control button not be pressed
	{
		for( pSTL = m_stls ; pSTL != NULL ; pSTL = pSTL->next )
		{
			if( pSTL->stt == stlSelected )
			{
				pSTL->stt = stlNormal ;
				stlDraw(pSTL, pVI) ;
				refresh = 1 ;
			}
		}
	}

	for( pSTL = m_stls ; pSTL != NULL ; pSTL = pSTL->next )
	{
		if( pSTL->stt == stlPrompting )
		{
			pSTL->stt = stlSelected ;
			stlDraw(pSTL, pVI) ;
			refresh = 1 ;
		}
	}

	viLighting(pVI, 0) ;

	if( refresh )
		glFlush() ;

	return ;
}

/*  2016/12/28 nt
 *	0=error, 1=picked, 2=not picked
 *  输入pi->pos,pi->dir,pi->r
 *	输出:pi->p,z,pT,pSTL
 *	不拾取已处于选中状态和提示状态的STL模型
 */
int CCADDoc::Pick(STLPCK* pi)
{
	STL* stl ;
	STLTRI* tri ;

	pi->tri = NULL ;
	pi->stl = NULL ;
	pi->z = MAX_DBL ;
	for( stl = m_stls ; stl != NULL ; stl = stl->next )
	{
		if( stl->stt == stlNormal ||
			stl->stt == stlPrompting )
		{
			tri = stlPick(stl, pi->pos, pi->dir, pi->r, pi->p, &(pi->z)) ;
			if( tri )
			{
				pi->tri = tri ;
				pi->stl = stl ;
			}
		}
	}

	if( pi->stl )
		return 1 ;
	else
		return 2 ;
}

void CCADDoc::OnMouseMove(int nFlags, double p[3], double v[3], void* pVI)
{
	if( m_pCmd )
		m_pCmd->OnMouseMove(nFlags, p, v) ;
	else
	if( m_pDlg )
		m_pDlg->OnMM(nFlags, p, v, pVI) ;
	else
	{
		STLPCK pi ;
		memcpy(pi.pos, p, sizeof(PNT3D)) ;
		memcpy(pi.dir, v, sizeof(PNT3D)) ;
		pi.r = 2.5*viGetScale(pVI) ;
		pi.tri = NULL ;
		pi.stl = NULL ;
		pi.z = MAX_DBL ;
		Pick(&pi) ;
		if( pi.stl ) // nt add 2019/7/10
		{
			cadPromptStr(pi.stl->material.GetBuffer(0)) ;
		}
		else
			cadPromptClear() ;
		SetPromptingSTL(pi.stl, pi.p, pi.tri, pVI) ;
	}

	return ;
}

void CCADDoc::OnLButtonDown(int nFlags, double p[3], double v[3], void* pVI)
{
	if( m_pCmd )
		m_pCmd->OnLButtonDown(nFlags, p, v) ;
	else
	if( m_pDlg )
		m_pDlg->OnLBD(nFlags, p, v, pVI) ;
	else
	{
		SetSelected(nFlags, pVI) ;
		// nt add 2017/8/26
		CSet set ;
		int n = GetAllOfSelectedSTL(&set) ;
		if (n > 0)
		{
			OnEditDir();

		}
		// end
	}

	return ;
}

void CCADDoc::OnLButtonUp(int nFlags, double p[3], double v[3], void* pVI)
{
	if( m_pCmd )
		m_pCmd->OnLButtonUp(nFlags, p, v) ;
	else
	if( m_pDlg )
		m_pDlg->OnLBU(nFlags, p, v, pVI) ;

	return ;
}

void CCADDoc::OnRButtonDown(int nFlags, double p[3], double v[3], void* pVI)
{
	// nt 2017/8/23做4行注释，目的：使右键只负责视图的旋转
	//if( m_pCmd )
	//	m_pCmd->OnRButtonDown(nFlags, p, v) ;
	//else
	//if( m_pDlg )
	//	m_pDlg->OnRBD(nFlags, p, v, pVI) ; //nt modify 2017/3/30

	return ;
}

void CCADDoc::OnRButtonUp(int nFlags, double p[3], double v[3], void* pVI)
{
}
//-------------------------------------------------------------
// nt add 2017/5/14
// nt add 2017/5/14
// nt modify 2017/5/17
// nt modify 2017/6/2
// nt rewrite 2017/8/27
void CCADDoc::Update()
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd() ;
	if( pMainWnd == NULL ) // nt add 2017/9/6
		return ;
	CMFCRibbonBar* pRibbon = ((CMainFrame*)AfxGetMainWnd())->GetRibbonBar() ;
	CMFCRibbonBaseElement* pObj ;
	CMFCRibbonEdit* pEdit ;
	CString str ;
	CMFCRibbonComboBox *pCmbox;
	

	// vol
	STL* stl ;
	m_vol = 0. ;
	for( stl = m_stls ; stl != NULL ; stl = stl->next )
		m_vol += stl->vol ;
	if( m_vol < 0. )
		m_vol = 0. ;
	pObj = pRibbon->FindByID(ID_EDIT1) ;
	pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pObj) ;
	if( pEdit )
	{
		str.Format(_T("%8.2lf mL"), 0.001*m_vol) ; // to mL
		pEdit->SetEditText(str) ;
	}

	// layer
	m_nLayer = 0 ;
	if( m_stls ) // nt rewrite 2017/5/26
	{
		double h0 = m_parameter.ts[0]*0.001 ; // nt add 2017/6/2
		double zmax = GetZMax() ;
		ZB* zb = (ZB*)(m_stls->zb) ;
		if( zb )
		{
			double h = zb->z ;
			if( h < 0. )
				h = 0. ;
			else
			if( h > zmax )
				h = zmax ;
			m_nLayer = (int)((h-h0)/m_h+0.5) ; // nt moodify 2017/6/2
		}
		else
			m_nLayer = (int)((zmax-h0)/m_h+0.5)+1 ; // nt modify 2017/6/2
	}
	if( m_nLayer < 0 ) // nt add 2017/6/2
		m_nLayer = 0 ;
	pObj = pRibbon->FindByID(ID_EDIT2) ;
	pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pObj) ;
	if( pEdit )
	{
		str.Format(_T("%d层"), m_nLayer) ;
		pEdit->SetEditText(str) ;
	}

	// thickness
	pObj = pRibbon->FindByID(ID_EDIT3) ;
	pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pObj) ;
	if( pEdit )
	{
		str.Format(_T("%.3gmm"), m_h) ;
		pEdit->SetEditText(str) ;
	}

	// time
	// 计算打印时间
	m_ptime = int(m_t0 + (m_lt + m_tm) * m_nLayer); //打印时间
	// ...
	pObj = pRibbon->FindByID(ID_EDIT4) ;
	pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pObj) ;
	if( pEdit )
	{

		int h = m_ptime/3600, m = (m_ptime%3600)/60 ;
		str.Format(_T("%d小时%d分钟"), h, m) ;
		pEdit->SetEditText(str) ;
	}

	// nt add 2017/8/28
	pObj = pRibbon->FindByID(ID_SLIDER1) ;
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pObj) ;
	if( pSlider )
	{
		int nRange = pSlider->GetRangeMax() ;
		if( nRange != m_nLayer )
		{
			pSlider->SetRange(1, m_nLayer) ;
			pSlider->SetPos(1) ;
		}
	}

	// nt add 2017/8/28
	pObj = pRibbon->FindByID(ID_EDIT5) ;
	pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pObj) ;
	if( pEdit )
	{
		str.Format(_T("%d"), m_iLayer) ;
		pEdit->SetEditText(str) ;
	}

	// smf add 2021/10/30
	pCmbox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pRibbon->FindByID(ID_COMBO_DEF));
	//int nIndex = pCmbox->GetCurSel();
	//if (nIndex >= 0)
	//{
	//	MessageBox(NULL, pCmbox->GetItem(nIndex), _T("消息"), MB_OK);
	//}
	//else
	//{
	//	//MessageBox(this, _T("请选择！"), _T("消息"), MB_OK);
	//}

	return ;
}

// nt modify 2017/5/26
// nt modify 2017/9/10 add PRG
void CCADDoc::MakeSupport(PRG* pPrg)
{
	STL* stl = NULL ;
	VEC3D v ;
	BOX3D box ;
	SS* ss = NULL ;

	// nt add 2017/9/10
	int n = GetNumOfSTL() ;
	if( pPrg == NULL ||
		n < 1 )
		return ;
	prgInit(pPrg, n) ;

	if( m_stls )
	{
		int i = 0 ;
		double s = 0., ww, hh ;
		for( stl = m_stls ; stl ; stl = stl->next )
		{
			ww = stl->box.max[0]-stl->box.min[0] ;
			hh = stl->box.max[1]-stl->box.min[1] ;
			pPrg->ws[i] = fabs(ww*hh) ;
			s += pPrg->ws[i] ;
			i++ ;
		}
		if( s < MIN_DBL )
			return ; // error
		for( i = 0 ; i < n ; i++ )
			pPrg->ws[i] /= s ;

		for( stl = m_stls ; stl ; stl = stl->next )
		{
			stlGetBox(stl, &box) ;
			double hPart = m_ssp.hPart+m_ssp.h ; // nt modify 2017/9/8
			mathVec(v, 0., 0., hPart-box.min[2]) ;
			stlMove(stl, v) ;
			ss = ssCreate() ;
			ssMake(ss, stl, &m_ssp, pPrg) ; // nt modify 2017/9/10
			if( stl->ss )
			{
				ssMerge(ss, (SS*)(stl->ss), stl, &m_ssp) ;
				//ssFree((SS*)(stl->ss)) ; // nt modify 2017/9/3
			}
			// nt add 2017/9/3
			CRMakeSS* pRMakeSS = new CRMakeSS(stl, ss, (SS*)stl->ss) ;
			m_rs.Add(pRMakeSS) ;
			SetModifiedFlag(TRUE) ;
			// end
			stl->ss = ss ;
			ss = NULL ;

			(pPrg->i)++ ; // nt add 2017/9/10
		}

		prgFinish(pPrg) ; // nt add 2017/9/10

		Update() ;
		Redraw() ;
	}

	return ;
}

// nt modify 2017/5/26
void CCADDoc::ClearSupport()
{
	STL* stl = NULL ;

	if( m_stls )
	{
		for( stl = m_stls ; stl ; stl = stl->next )
		{
			if( stl->ss ) // nt modify 2017/9/3
			{
				//ssFree((SS*)(stl->ss)) ;
				CRDelSS* pRDelSS = new CRDelSS(stl, (SS*)stl->ss) ;
				m_rs.Add(pRDelSS) ;
				SetModifiedFlag(TRUE) ;
			}
			stl->ss = NULL ;
		}
		Redraw() ;
	}

	return ;
}

// nt modify 2017/9/9 add progress
void CCADDoc::Print(PRG* pPrg)
{
	int i, n ;
	double cell_w = 2., r1 = 0.1, r2 = 0.2 ;
	BOX3D box ;
	STL* stl = NULL ;
	CB* cb = NULL ;

	n = GetNumOfSTL() ;
	if( pPrg == NULL ||
		n < 1 )
		return ;
	prgInit(pPrg, n) ;

	if( m_stls )
	{
		double s = 0., ww, hh ;
		i = 0 ;
		for( stl = m_stls ; stl ; stl = stl->next )
		{
			ww = stl->box.max[0]-stl->box.min[0] ;
			hh = stl->box.max[1]-stl->box.min[1] ;
			pPrg->ws[i] = fabs(ww*hh) ;
			s += pPrg->ws[i] ;
			i++ ;
		}
		if( s < MIN_DBL )
			return ; // error
		for( i = 0 ; i < n ; i++ )
		{
			pPrg->ws[i] /= s ;
			pPrg->ws[i] *= 0.5 ;
		}
		
		for( stl = m_stls ; stl ; stl = stl->next )
		{
			stlGetBox(stl, &box) ;
			//box.min[0] -= r1 ;
			//box.min[1] -= r1 ;
			//box.min[2] -= r1 ;
			box.max[0] += r1 ;
			box.max[1] += r1 ;
			box.max[2] += r1 ;
			cb = cbCreate(&box, cell_w, r1, r2, 1.e-6, 1.e-11) ; // 2022/08/17 smf modify: 1.e-8 to 1.e-6
			cbFill( cb, stl, pPrg, m_stls , cell_w) ;
			if( stl->cb )
				cbFree((CB*)(stl->cb)) ;
			stl->cb = cb ;
			cb = NULL ;

			if( stl->cb == NULL )
				continue ;

			//ZB2* zb3 = NULL;
			//zb3 = zb2Create(&box, w, m_h, 1.e-6, 1.e-11) ; // 2022/08/17 smf modify: 1.e-8 to 1.e-6
			//if (stl->zb3)
			//	zb2Free((ZB2*)(stl->zb3));
			//stl->zb3 = zb3;
			//zb3 = NULL;

			//FindErrorCB((CB*)(stl->cb));
			//OutPutCB((CB*)(stl->cb), m_h * 2185);
			//OutPutSline2((ZB2*)zb3); //  2022/08/17 smf add: 输出Sline2的信息


			(pPrg->i)++ ;
		}

		prgFinish(pPrg) ;
	}

	return ;
}

COLORREF str2ColorRef(CString& color)
{
	if( color == _T("白色") )
		return RGB(255, 255, 255) ;
	else
	if( color == _T("红色") )
		return RGB(255, 0, 0) ;
	else
	if( color == _T("绿色") )
		return RGB(0, 255, 0) ;
	else
	if( color == _T("蓝色") )
		return RGB(0, 0, 255) ;
	else
		return RGB(0, 0, 0) ;

}

// nt add 2017/5/16
// nWidth必须为4的倍数
int bmp4Save(CString& fileName, 
	         int nWidth, 
	         int nHeight, 
	         void* bits)
{
	int i ;
	FILE* fp = NULL ;
	unsigned int fileSize, // size of file
				 bitSize ; // size of bitmap pixels
	BITMAPFILEHEADER header ; // file header
	BITMAPINFO info ;
	RGBQUAD ct[16] ;

	// 各部分大小
	bitSize = nWidth*nHeight/2 ;
	fileSize = sizeof(BITMAPFILEHEADER)+
		       sizeof(BITMAPINFOHEADER)+
		       sizeof(RGBQUAD)*16+
		       bitSize ;

	// BITMAPFILEHEADER
	header.bfType      = 'MB' ;	// Non-portable... sign 固定的
	header.bfSize      = fileSize ; // 整个文件的大小
	header.bfReserved1 = 0 ;
	header.bfReserved2 = 0 ;
	header.bfOffBits   = fileSize-bitSize ;

	// BITMAPINFO
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER) ;
    info.bmiHeader.biWidth = nWidth ;
    info.bmiHeader.biHeight = nHeight ;
    info.bmiHeader.biPlanes = 1 ;
    info.bmiHeader.biBitCount = 4 ;
    info.bmiHeader.biCompression = BI_RGB ;
    info.bmiHeader.biSizeImage = bitSize ;
    info.bmiHeader.biXPelsPerMeter = 0 ;
    info.bmiHeader.biYPelsPerMeter = 0 ;
	info.bmiHeader.biClrImportant = 0 ;
	for( i = 0 ; i < 16 ; i++ )
	{
		ct[i].rgbBlue = i*16+15 ;
		ct[i].rgbGreen = i*16+15 ;
		ct[i].rgbRed = i*16+15 ;
		ct[i].rgbReserved = 0 ;
	}

	_tfopen_s(&fp, fileName.GetBuffer(0), _T("wb")) ;
	if( fp == NULL )
		return 0 ; // error

	if( fwrite(&header, 1, sizeof(BITMAPFILEHEADER), fp) < 
		sizeof(BITMAPFILEHEADER) ||
		fwrite(&info, 1, sizeof(BITMAPINFOHEADER), fp) < 
		sizeof(BITMAPINFOHEADER) ||
		fwrite(ct, 1, sizeof(RGBQUAD)*16, fp) <
		sizeof(RGBQUAD)*16 ||
		fwrite(bits, 1, bitSize, fp) < bitSize )
	{
		fclose(fp) ;
		return 0 ;
	}
	fclose(fp) ;

	return 1 ;
}

// nt add 2017/5/16
int layerSave2(LAYER* layer, int nWidth, int nHeight, CString& filePath)
{
	CString temp ;
	temp.Format(_T("%d.bmp"), layer->iLayer) ;
	temp = filePath+temp ;
	bmp4Save(temp, nWidth, nHeight, layer->data) ;

	return 1 ;
}

// output printing file(*.p_f)
// nt modify 2017/5/26
// nt modify 2017/5/31
// nt modify 2017/9/10 add PRG
// nt modify 2019/4/9 add gray
// nt modify 2019/5/27 add gvm
void CCADDoc::Out(CString& filePath, PRG* pPrg, BOOL gray, int gvm)
{
	// nt add 2017/9/10
	int n = GetNumOfSTL() ;
	BOOL b_mLangFlag = GetLang_doc();
	if( pPrg == NULL ||
		n < 1 )
		return ;
	prgInit(pPrg, n) ;

	// nt add 2019/5/5 debug only
	/*m_parameter.grays[0] = 100 ;
	m_parameter.grays[1] = 80 ;
	m_parameter.grays[2] = 60 ;
	m_parameter.grays[3] = 40 ;*/

	STL* stl = NULL ;
	for( stl = m_stls ; stl ; stl = stl->next )
	{
		if( stl->zb == NULL &&
			stl->zb3 == NULL ) // nt add 2021/12/29 这两个条件之一不为NULL
		{
			if(!b_mLangFlag)
				AfxMessageBox(_T("先执行层切操作!")) ;
			if (b_mLangFlag)
				MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Perform layer cut operation first!"), _T("CAD"), MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			return ;
		}
	}

	int PL, PW ;
	if( abs(m_parameter.Dx) == 1 ) // X'与工作台坐标轴X对应同向或相反
	{
		PL = m_parameter.PL ;
		PW = m_parameter.PW ;
	}
	else
	{
		PL = m_parameter.PW ;
		PW = m_parameter.PL ;
	}
	if( PL < 1 || PW < 1 ) // nt add 2017/10/26
	{
		if (!b_mLangFlag)
			AfxMessageBox(_T("参数错误!")) ;
		else
			MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Parameter error!"), _T("CAD"), MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return ;
	}
	double w = m_parameter.W/PW ; // nt add 2017/5/31
	COLORREF color = str2ColorRef(m_color) ;
	PF* pf = pfCreate(&m_parameter, 
		              m_vol, 
		              0., 
		              color, 
		              m_h, 
		              m_lt, 
		              m_li, 
		              m_lt,
		              m_li,
		              m_constH, 
		              PL,
		              PW) ;
	if( pf == NULL )
	{
		if (!b_mLangFlag)
			cadPromptStr(_T("内存错误!")) ;
		else
			cadPromptStr(_T("Memory error!"));
		return ;
	}
	PARRAY* parray = parrayCreate(PL*PW, w) ; // nt add 2017/5/31

	uchar c ;
	int iLayer, i, j, k, I, J, li1, li2 ;
	double x, y, z, h0, h, zmax = GetZMax(), lt1, lt2 ;
	LAYER* layer = NULL ;
	ZB* zb = NULL ;
	ZB2* zb2 = NULL, *zb3 = NULL ; // nt add 2017/5/31 // nt add zb3 2021/12/29
	h0 = 0.001*m_parameter.ts[0] ; // nt add 2017/6/2
	//n = (int)ceil(zmax/m_h) ; // 总的层数
	iLayer = 0 ; // nt add 2017/6/2
	z = h0 ; // 起始层z值，nt add 2017/6/2
	h = h0 ; // 起始层厚度，nt add 2017/6/2
	while(z < zmax) // nt add 2017/6/2
	{
		parameterMatch(&m_parameter, h, li1, lt1, li2, lt2) ; // nt add 2017/6/3
		layer = layerCreate(iLayer, h, li1, lt1, li2, lt2, pf->nx, pf->ny) ;
		if( layer == NULL ) // nt add 2018/1/10
		{
			parrayFree(parray) ;
			pfFree(pf) ;
			if (!b_mLangFlag)
				AfxMessageBox(_T("内存错误!")) ;
			else
				MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Memory error!"), _T("CAD"), MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			return ;
		}
		for( stl = m_stls ; stl ; stl = stl->next )
		{
			if( stl->zb )
			{
				zb = (ZB*)(stl->zb) ;
				if( zb )
					// 2022/08/16 smf modify: 强转double to float
					zbCut(zb, (float)z) ;
				zb2 = (ZB2*)(stl->zb2) ; // nt add 2017/5/31
				if( zb2 ) // nt add 2017/5/31
					 // 2022/08/16 smf modify: 强转double to float
					zb2Cut(zb2, (float)z) ; // nt add 2017/5/31
				parrayMergeZBZB2(parray, zb, zb2) ; // nt add 2017/5/31
			}
			else // stl->zb3 != NULL, nt add 2021/12/29
			{
				zb3 = (ZB2*)(stl->zb3) ;
				if (zb3)
					// 2022/08/16 smf modify: 强转double to float
					zb2Cut(zb3, (float)z);
				zb2 = (ZB2*)(stl->zb2) ; // nt add 2017/5/31
				if (zb2) // nt add 2017/5/31
					 // 2022/08/16 smf modify: 强转double to float
					zb2Cut(zb2, (float)z); // nt add 2017/5/31
				parrayMergeZB2ZB2(parray, zb2, zb3) ; // nt add 2017/5/31
			}

			if( gray ) // nt add 2019/4/9
			{
				if( gvm == 1) // nt add 2019/5/27
					parrayGray(parray, m_parameter.grays) ; // nt add 2017/5/31
				else
					parrayGray1(parray, m_parameter.grays) ;
			}
			
			k = 0 ;
			for( j = 0 ; j < parray->ny ; j++ )
			{
				y = parray->ymin+w*j ;
				for( i = 0 ; i < parray->nx ; i++ )
				{
					x = parray->xmin+w*i ;
					c = parray->pixels[k] ; ;
					if( c > 0 )
					{
						parameter2PixelCoord(&m_parameter, x, y, I, J) ;
						// nt add 2018/3/1 implement mirror function
						if( m_parameter.tis[0]&1 )
							I = pf->nx-I ;
						// end mirror
						layerSet(layer, J*pf->nx+I, c/16) ;
					}
					k++ ;
				}
			}
		}
		//layerSave2(layer, PL, PW, filePath) ; // test only
		pfAddLayer2(pf, layer) ;

		Update() ; // nt add 2017/5/17

		h = CalH(z) ; // nt add 2017/6/2
		z += h ; // nt modify 2017/6/2
		iLayer++ ; // nt add 2017/6/3

		// nt add 2017/9/10
		pPrg->c = z/zmax ;
	}

	parrayFree(parray) ; // nt add 2017/5/31

	if( pfSave(pf, filePath) != 1 )
	{
		pfFree(pf) ;
		if (!b_mLangFlag)
			cadPromptStr(_T("存储错误!")) ;
		else
			cadPromptStr(_T("Storage error!"));
		return ;
	}
	pfFree(pf) ;
	pf = NULL ;

	// nt add 2017/9/10
	prgFinish(pPrg) ;

	//启动新线程CSY
	HANDLE pThread1 = CreateThread(NULL, 0, DISPLAY1, NULL, 0, NULL);
	CloseHandle(pThread1);

	return ;
}

void CCADDoc::CollisionDetect()
{
	double tol ;
	BOX3D box1, box2 ;
	STL* stl1, *stl2 ;
	BOOL b_mLangFlag = GetLang_doc();
	m_collision = 0 ;
	tol = m_parameter.L/m_parameter.PL ;
	for( stl1 = m_stls ; stl1 != NULL ; stl1 = stl1->next )
	{
		stlGetBox(stl1, &box1) ;
		// smf 2021/12/30 注释掉
		//if( box1.min[0] < -0.5*m_ds[0] ||
		//	box1.max[0] > 0.5*m_ds[0] ||
		//	box1.min[1] < -0.5*m_ds[1] ||
		//	box1.max[1] > 0.5*m_ds[1] ||
		//	box1.max[2]-box1.min[2]+m_ssp.h > m_ds[2] )
		//{
		//	m_collision = 1 ;
		//	if (!b_mLangFlag)
		//		cadPromptStr(_T("STL模型超出工作台范围!")) ;
		//	else
		//		cadPromptStr(_T("STL model is beyond the scope of the workbench!"));
		//	return ;
		//}
		for( stl2 = stl1->next ; stl2 != NULL ; stl2 = stl2->next )
		{
			stlGetBox(stl2, &box2) ;
			if( mathChkBoxInt(&box1, &box2, tol) == IDINT )
			{
				m_collision = 1 ;
				if (!b_mLangFlag)
					cadPromptStr(_T("STL模型可能有干涉!")) ;
				else
					cadPromptStr(_T("STL models may have interference!"));
				return ;
			}
		}
	}
	if (!b_mLangFlag)
		cadPromptStr(_T("STL模型无涉!")) ;
	else
		cadPromptStr(_T("STL model does not interfere!"));

	return ;
}

// nt add 2017/10/1
void CCADDoc::CollisionDetect2(PRG* pPrg)
{
	int i, n = GetNumOfSTL() ;
	double tol ;
	BOX3D box1, box2 ;
	STL* stl1, *stl2 ;
	SS* ss ;

	prgInit(pPrg, 0) ;
	m_collision = 0 ;
	if( n < 1 ) // nt add 2017/10/8
	{
		prgFinish(pPrg) ;
		return ;
	}
	tol = 1.e-3 ;
	for( i = 1, stl1 = m_stls ; stl1 != NULL ; stl1 = stl1->next )
	{
		stlCalBox2(stl1, &box1) ;
		// smf 2021/12/30 注释掉
		//if( box1.min[0] < -0.5*m_ds[0] ||
		//	box1.max[0] > 0.5*m_ds[0] ||
		//	box1.min[1] < -0.5*m_ds[1] ||
		//	box1.max[1] > 0.5*m_ds[1] ||
		//	box1.min[2] < 0. ||
		//	box1.max[2] > m_ds[2] )
		//{
		//	prgFinish(pPrg) ;
		//	m_collision = 1 ;
		//	return ;
		//}
		for( stl2 = stl1->next ; stl2 != NULL ; stl2 = stl2->next )
		{
			stlCalBox2(stl2, &box2) ;
			if( stlCollision(stl1, stl2) )
			{
				prgFinish(pPrg) ;
				m_collision = 1 ;
				return ;
			}
		}
		pPrg->c = (double)i/n ;
		i++ ;
	}

	for( stl1 = m_stls ; stl1 != NULL ; stl1 = stl1->next )
	{
		ss = (SS*)stl1->ss ;
		stlGetBox(stl1, &box1) ;
		if( ssHasValidSupport(ss) == 0 &&
			box1.min[2] > tol )
		{
			prgFinish(pPrg) ;
			m_collision = 1 ;
			return ;
		}
	}

	// nt add 2017/10/8
	prgFinish(pPrg) ;

	return ;
}

// JH 19/04/10
// nt modify 2019/4/13 export all STL models
void CCADDoc::ExportSTL(CString& pathName)
{
	CString str ;
	STL* pSTL = NULL ;

	// SAVE STL model
	CStdioFile File(pathName, CFile::modeCreate | CFile::modeReadWrite);
	str.Format(_T("solid STL generated by 3D print\n"));
	File.WriteString(str);

	// save stl & base & support & rod
	for( pSTL = m_stls ; pSTL != NULL ; pSTL = pSTL->next )
	{
		for (int i = 0; i < pSTL->nBlk; i++)  // save this stl ，notice：stl->blk->tri
		{
			// save STL model
			STLBLK* pB = &(pSTL->blks[i]);
			STLTRI* pT;
			for (int j = 0; j < pB->nTri; j++)
			{
				pT = &(pB->tris[j]);
				str.Format(_T("  facet normal %e %e %e\n"), pT->normal[0], pT->normal[1], pT->normal[2]);
				File.WriteString(str);
				str.Format(_T("    outer loop\n"));
				File.WriteString(str);
				// point0
				str.Format(_T("      vertex %e %e %e\n"), pT->vs[0][0], pT->vs[0][1], pT->vs[0][2]);
				File.WriteString(str);
				// point1
				str.Format(_T("      vertex %e %e %e\n"), pT->vs[1][0], pT->vs[1][1], pT->vs[1][2]);
				File.WriteString(str);
				// point 2
				str.Format(_T("      vertex %e %e %e\n"), pT->vs[2][0], pT->vs[2][1], pT->vs[2][2]);
				File.WriteString(str);
				str.Format(_T("    endloop\n"));
				File.WriteString(str);
				str.Format(_T("  endfacet\n"));
				File.WriteString(str);
			}
		}
		// save base & support & rod
		FACET* facet = NULL;
		SS* ss = ((SS*)(pSTL->ss));
		if(ss == NULL) //没有挂支撑，跳出本轮循环
			continue;

		// SAVE BASE
		BASE * base = ss->base;
		if (base && base->facet)
		{
			facet = base->facet;
			// 对 facet 数据进行写入
			int i_fct, I, J, K, L;
			STLTRI stltri;
			// stltriInit(stltri);
			if (facet == NULL)
				return;
			for (i_fct = 0; i_fct < facet->ntria; i_fct++)
			{
				I = facet->trias[i_fct][0];
				J = facet->trias[i_fct][1];
				K = facet->trias[i_fct][2];
				stltri.vs[0][0] = facet->nodes[I].p[0];
				stltri.vs[0][1] = facet->nodes[I].p[1];
				stltri.vs[0][2] = facet->nodes[I].p[2];
				stltri.vs[1][0] = facet->nodes[J].p[0];
				stltri.vs[1][1] = facet->nodes[J].p[1];
				stltri.vs[1][2] = facet->nodes[J].p[2];
				stltri.vs[2][0] = facet->nodes[K].p[0];
				stltri.vs[2][1] = facet->nodes[K].p[1];
				stltri.vs[2][2] = facet->nodes[K].p[2];
				stltriCorrNormal(&stltri, 1e-10);
				str.Format(_T("  facet normal %e %e %e\n"), stltri.normal[0], stltri.normal[1], stltri.normal[2]);
				File.WriteString(str);
				str.Format(_T("    outer loop\n"));
				File.WriteString(str);
				// point0
				str.Format(_T("      vertex %e %e %e\n"), stltri.vs[0][0], stltri.vs[0][1], stltri.vs[0][2]);
				File.WriteString(str);
				// point1
				str.Format(_T("      vertex %e %e %e\n"), stltri.vs[1][0], stltri.vs[1][1], stltri.vs[1][2]);
				File.WriteString(str);
				// point 2
				str.Format(_T("      vertex %e %e %e\n"), stltri.vs[2][0], stltri.vs[2][1], stltri.vs[2][2]);
				File.WriteString(str);
				str.Format(_T("    endloop\n"));
				File.WriteString(str);
				str.Format(_T("  endfacet\n"));
				File.WriteString(str);
			}
			for (i_fct = 0; i_fct < facet->nquad; i_fct++)
			{
				I = facet->quads[i_fct][0];
				J = facet->quads[i_fct][1];
				K = facet->quads[i_fct][2];
				L = facet->quads[i_fct][3];
				stltri.vs[0][0] = facet->nodes[I].p[0];
				stltri.vs[0][1] = facet->nodes[I].p[1];
				stltri.vs[0][2] = facet->nodes[I].p[2];
				stltri.vs[1][0] = facet->nodes[J].p[0];
				stltri.vs[1][1] = facet->nodes[J].p[1];
				stltri.vs[1][2] = facet->nodes[J].p[2];
				stltri.vs[2][0] = facet->nodes[K].p[0];
				stltri.vs[2][1] = facet->nodes[K].p[1];
				stltri.vs[2][2] = facet->nodes[K].p[2];
				stltriCorrNormal(&stltri, 1e-10);
				str.Format(_T("  facet normal %e %e %e\n"), stltri.normal[0], stltri.normal[1], stltri.normal[2]);
				File.WriteString(str);
				str.Format(_T("    outer loop\n"));
				File.WriteString(str);
				// point0
				str.Format(_T("      vertex %e %e %e\n"), stltri.vs[0][0], stltri.vs[0][1], stltri.vs[0][2]);
				File.WriteString(str);
				// point1
				str.Format(_T("      vertex %e %e %e\n"), stltri.vs[1][0], stltri.vs[1][1], stltri.vs[1][2]);
				File.WriteString(str);
				// point 2
				str.Format(_T("      vertex %e %e %e\n"), stltri.vs[2][0], stltri.vs[2][1], stltri.vs[2][2]);
				File.WriteString(str);
				str.Format(_T("    endloop\n"));
				File.WriteString(str);
				str.Format(_T("  endfacet\n"));
				File.WriteString(str);

				stltri.vs[0][0] = facet->nodes[I].p[0];
				stltri.vs[0][1] = facet->nodes[I].p[1];
				stltri.vs[0][2] = facet->nodes[I].p[2];
				stltri.vs[1][0] = facet->nodes[K].p[0];
				stltri.vs[1][1] = facet->nodes[K].p[1];
				stltri.vs[1][2] = facet->nodes[K].p[2];
				stltri.vs[2][0] = facet->nodes[L].p[0];
				stltri.vs[2][1] = facet->nodes[L].p[1];
				stltri.vs[2][2] = facet->nodes[L].p[2];
				stltriCorrNormal(&stltri, 1e-10);
				str.Format(_T("  facet normal %e %e %e\n"), stltri.normal[0], stltri.normal[1], stltri.normal[2]);
				File.WriteString(str);
				str.Format(_T("    outer loop\n"));
				File.WriteString(str);
				// point0
				str.Format(_T("      vertex %e %e %e\n"), stltri.vs[0][0], stltri.vs[0][1], stltri.vs[0][2]);
				File.WriteString(str);
				// point1
				str.Format(_T("      vertex %e %e %e\n"), stltri.vs[1][0], stltri.vs[1][1], stltri.vs[1][2]);
				File.WriteString(str);
				// point 2
				str.Format(_T("      vertex %e %e %e\n"), stltri.vs[2][0], stltri.vs[2][1], stltri.vs[2][2]);
				File.WriteString(str);
				str.Format(_T("    endloop\n"));
				File.WriteString(str);
				str.Format(_T("  endfacet\n"));
				File.WriteString(str);
			}
			// 结束对 facet 数据写入
		}
		// SAVE SUPPORT
		SUPPORT* support = ss->supports;
		for (support; support; support = support->next)
		{
			if (supportIsDel(support) == 1)
				continue;
			int k_spt;
			for (k_spt = 0; k_spt < SUPPORT_MAXNUMFACET; k_spt++) // 6 facet per support
			{
				if (support->facets[k_spt])
				{
					facet = support->facets[k_spt];
					// 对 facet 数据进行写入
					int i_fct, I, J, K, L;
					STLTRI stltri;
					// stltriInit(stltri);
					if (facet == NULL)
						return;
					for (i_fct = 0; i_fct < facet->ntria; i_fct++)
					{
						I = facet->trias[i_fct][0];
						J = facet->trias[i_fct][1];
						K = facet->trias[i_fct][2];
						stltri.vs[0][0] = facet->nodes[I].p[0];
						stltri.vs[0][1] = facet->nodes[I].p[1];
						stltri.vs[0][2] = facet->nodes[I].p[2];
						stltri.vs[1][0] = facet->nodes[J].p[0];
						stltri.vs[1][1] = facet->nodes[J].p[1];
						stltri.vs[1][2] = facet->nodes[J].p[2];
						stltri.vs[2][0] = facet->nodes[K].p[0];
						stltri.vs[2][1] = facet->nodes[K].p[1];
						stltri.vs[2][2] = facet->nodes[K].p[2];
						stltriCorrNormal(&stltri, 1e-10);
						str.Format(_T("  facet normal %e %e %e\n"), stltri.normal[0], stltri.normal[1], stltri.normal[2]);
						File.WriteString(str);
						str.Format(_T("    outer loop\n"));
						File.WriteString(str);
						// point0
						str.Format(_T("      vertex %e %e %e\n"), stltri.vs[0][0], stltri.vs[0][1], stltri.vs[0][2]);
						File.WriteString(str);
						// point1
						str.Format(_T("      vertex %e %e %e\n"), stltri.vs[1][0], stltri.vs[1][1], stltri.vs[1][2]);
						File.WriteString(str);
						// point 2
						str.Format(_T("      vertex %e %e %e\n"), stltri.vs[2][0], stltri.vs[2][1], stltri.vs[2][2]);
						File.WriteString(str);
						str.Format(_T("    endloop\n"));
						File.WriteString(str);
						str.Format(_T("  endfacet\n"));
						File.WriteString(str);
					}
					for (i_fct = 0; i_fct < facet->nquad; i_fct++)
					{
						I = facet->quads[i_fct][0];
						J = facet->quads[i_fct][1];
						K = facet->quads[i_fct][2];
						L = facet->quads[i_fct][3];
						stltri.vs[0][0] = facet->nodes[I].p[0];
						stltri.vs[0][1] = facet->nodes[I].p[1];
						stltri.vs[0][2] = facet->nodes[I].p[2];
						stltri.vs[1][0] = facet->nodes[J].p[0];
						stltri.vs[1][1] = facet->nodes[J].p[1];
						stltri.vs[1][2] = facet->nodes[J].p[2];
						stltri.vs[2][0] = facet->nodes[K].p[0];
						stltri.vs[2][1] = facet->nodes[K].p[1];
						stltri.vs[2][2] = facet->nodes[K].p[2];
						stltriCorrNormal(&stltri, 1e-10);
						str.Format(_T("  facet normal %e %e %e\n"), stltri.normal[0], stltri.normal[1], stltri.normal[2]);
						File.WriteString(str);
						str.Format(_T("    outer loop\n"));
						File.WriteString(str);
						// point0
						str.Format(_T("      vertex %e %e %e\n"), stltri.vs[0][0], stltri.vs[0][1], stltri.vs[0][2]);
						File.WriteString(str);
						// point1
						str.Format(_T("      vertex %e %e %e\n"), stltri.vs[1][0], stltri.vs[1][1], stltri.vs[1][2]);
						File.WriteString(str);
						// point 2
						str.Format(_T("      vertex %e %e %e\n"), stltri.vs[2][0], stltri.vs[2][1], stltri.vs[2][2]);
						File.WriteString(str);
						str.Format(_T("    endloop\n"));
						File.WriteString(str);
						str.Format(_T("  endfacet\n"));
						File.WriteString(str);

						stltri.vs[0][0] = facet->nodes[I].p[0];
						stltri.vs[0][1] = facet->nodes[I].p[1];
						stltri.vs[0][2] = facet->nodes[I].p[2];
						stltri.vs[1][0] = facet->nodes[K].p[0];
						stltri.vs[1][1] = facet->nodes[K].p[1];
						stltri.vs[1][2] = facet->nodes[K].p[2];
						stltri.vs[2][0] = facet->nodes[L].p[0];
						stltri.vs[2][1] = facet->nodes[L].p[1];
						stltri.vs[2][2] = facet->nodes[L].p[2];
						stltriCorrNormal(&stltri, 1e-10);
						str.Format(_T("  facet normal %e %e %e\n"), stltri.normal[0], stltri.normal[1], stltri.normal[2]);
						File.WriteString(str);
						str.Format(_T("    outer loop\n"));
						File.WriteString(str);
						// point0
						str.Format(_T("      vertex %e %e %e\n"), stltri.vs[0][0], stltri.vs[0][1], stltri.vs[0][2]);
						File.WriteString(str);
						// point1
						str.Format(_T("      vertex %e %e %e\n"), stltri.vs[1][0], stltri.vs[1][1], stltri.vs[1][2]);
						File.WriteString(str);
						// point 2
						str.Format(_T("      vertex %e %e %e\n"), stltri.vs[2][0], stltri.vs[2][1], stltri.vs[2][2]);
						File.WriteString(str);
						str.Format(_T("    endloop\n"));
						File.WriteString(str);
						str.Format(_T("  endfacet\n"));
						File.WriteString(str);
					}
					// 结束对 facet 数据写入
				}
			}
		}
		// SAVE ROD
		ROD* rod = ss->rods;
		for (rod; rod; rod = rod->next)
		{
			if (rodIsDel(rod) == 1)
				continue;
			if (rod && rod->facet)
			{
				facet = rod->facet;
				// 对 facet 数据进行写入
				int i_fct, I, J, K, L;
				STLTRI stltri;
				// stltriInit(stltri);
				if (facet == NULL)
					return;
				for (i_fct = 0; i_fct < facet->ntria; i_fct++)
				{
					I = facet->trias[i_fct][0];
					J = facet->trias[i_fct][1];
					K = facet->trias[i_fct][2];
					stltri.vs[0][0] = facet->nodes[I].p[0];
					stltri.vs[0][1] = facet->nodes[I].p[1];
					stltri.vs[0][2] = facet->nodes[I].p[2];
					stltri.vs[1][0] = facet->nodes[J].p[0];
					stltri.vs[1][1] = facet->nodes[J].p[1];
					stltri.vs[1][2] = facet->nodes[J].p[2];
					stltri.vs[2][0] = facet->nodes[K].p[0];
					stltri.vs[2][1] = facet->nodes[K].p[1];
					stltri.vs[2][2] = facet->nodes[K].p[2];
					stltriCorrNormal(&stltri, 1e-10);
					str.Format(_T("  facet normal %e %e %e\n"), stltri.normal[0], stltri.normal[1], stltri.normal[2]);
					File.WriteString(str);
					str.Format(_T("    outer loop\n"));
					File.WriteString(str);
					// point0
					str.Format(_T("      vertex %e %e %e\n"), stltri.vs[0][0], stltri.vs[0][1], stltri.vs[0][2]);
					File.WriteString(str);
					// point1
					str.Format(_T("      vertex %e %e %e\n"), stltri.vs[1][0], stltri.vs[1][1], stltri.vs[1][2]);
					File.WriteString(str);
					// point 2
					str.Format(_T("      vertex %e %e %e\n"), stltri.vs[2][0], stltri.vs[2][1], stltri.vs[2][2]);
					File.WriteString(str);
					str.Format(_T("    endloop\n"));
					File.WriteString(str);
					str.Format(_T("  endfacet\n"));
					File.WriteString(str);
				}
				for (i_fct = 0; i_fct < facet->nquad; i_fct++)
				{
					I = facet->quads[i_fct][0];
					J = facet->quads[i_fct][1];
					K = facet->quads[i_fct][2];
					L = facet->quads[i_fct][3];
					stltri.vs[0][0] = facet->nodes[I].p[0];
					stltri.vs[0][1] = facet->nodes[I].p[1];
					stltri.vs[0][2] = facet->nodes[I].p[2];
					stltri.vs[1][0] = facet->nodes[J].p[0];
					stltri.vs[1][1] = facet->nodes[J].p[1];
					stltri.vs[1][2] = facet->nodes[J].p[2];
					stltri.vs[2][0] = facet->nodes[K].p[0];
					stltri.vs[2][1] = facet->nodes[K].p[1];
					stltri.vs[2][2] = facet->nodes[K].p[2];
					stltriCorrNormal(&stltri, 1e-10);
					str.Format(_T("  facet normal %e %e %e\n"), stltri.normal[0], stltri.normal[1], stltri.normal[2]);
					File.WriteString(str);
					str.Format(_T("    outer loop\n"));
					File.WriteString(str);
					// point0
					str.Format(_T("      vertex %e %e %e\n"), stltri.vs[0][0], stltri.vs[0][1], stltri.vs[0][2]);
					File.WriteString(str);
					// point1
					str.Format(_T("      vertex %e %e %e\n"), stltri.vs[1][0], stltri.vs[1][1], stltri.vs[1][2]);
					File.WriteString(str);
					// point 2
					str.Format(_T("      vertex %e %e %e\n"), stltri.vs[2][0], stltri.vs[2][1], stltri.vs[2][2]);
					File.WriteString(str);
					str.Format(_T("    endloop\n"));
					File.WriteString(str);
					str.Format(_T("  endfacet\n"));
					File.WriteString(str);

					stltri.vs[0][0] = facet->nodes[I].p[0];
					stltri.vs[0][1] = facet->nodes[I].p[1];
					stltri.vs[0][2] = facet->nodes[I].p[2];
					stltri.vs[1][0] = facet->nodes[K].p[0];
					stltri.vs[1][1] = facet->nodes[K].p[1];
					stltri.vs[1][2] = facet->nodes[K].p[2];
					stltri.vs[2][0] = facet->nodes[L].p[0];
					stltri.vs[2][1] = facet->nodes[L].p[1];
					stltri.vs[2][2] = facet->nodes[L].p[2];
					stltriCorrNormal(&stltri, 1e-10);
					str.Format(_T("  facet normal %e %e %e\n"), stltri.normal[0], stltri.normal[1], stltri.normal[2]);
					File.WriteString(str);
					str.Format(_T("    outer loop\n"));
					File.WriteString(str);
					// point0
					str.Format(_T("      vertex %e %e %e\n"), stltri.vs[0][0], stltri.vs[0][1], stltri.vs[0][2]);
					File.WriteString(str);
					// point1
					str.Format(_T("      vertex %e %e %e\n"), stltri.vs[1][0], stltri.vs[1][1], stltri.vs[1][2]);
					File.WriteString(str);
					// point 2
					str.Format(_T("      vertex %e %e %e\n"), stltri.vs[2][0], stltri.vs[2][1], stltri.vs[2][2]);
					File.WriteString(str);
					str.Format(_T("    endloop\n"));
					File.WriteString(str);
					str.Format(_T("  endfacet\n"));
					File.WriteString(str);
				}
				// 结束对 facet 数据写入
			}
		}
	}

	str.Format(_T("endsolid CADSTL\n"));
	File.WriteString(str);
	File.Close();

	return;
}
void CCADDoc::SetPropertySTL(short proper)
{
	STL *stl;

	for (stl = m_stls; stl != NULL; stl = stl->next)
	{
		if (stl->stt == stlSelected)
			stl->property = proper;
	}


	return ;

}
int CCADDoc::IsPropertySetAllStl()
{
	STL *stl;
	for (stl = m_stls; stl != NULL; stl = stl->next)
	{
		if (!(stl->property == FILLING_BODY ||
			stl->property == CAVITY ||
			stl->property == SKIN))
		{
			AfxMessageBox(_T("模型属性待设置！"));
			return 0;
		}
	}
	return 1;
}
//-------------------------------------------------------------
// CCADDoc 命令
void CCADDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	ClearCmdAll() ; // nt add 2017/9/1
	BOOL b_mLangFlag = GetLang_doc();
	LPTSTR pch ;
	CString szFilter ;
	CFileDialog dlg(TRUE);
	szFilter += "STL文件 (.STL)|*.stl|";
	szFilter += "模型文件 (.SWK)|*.swk|";
    szFilter += "所有文件 (*.*)|*.*|";
    pch = szFilter.GetBuffer(0);
    while ((pch = _tcschr(pch, '|')) != NULL)
		*pch++ = '\0';
    dlg.m_ofn.lpstrFilter = szFilter ;
    dlg.m_ofn.lpstrTitle = _T("数据文件") ;
	dlg.m_ofn.nFilterIndex = 3 ;
	if( dlg.DoModal() == IDOK )
	{
		CString title = dlg.GetFileTitle() ;
		CString pathName = dlg.GetPathName() ;
		if( pathName.Find(_T(".STL")) >= 0 ||
			pathName.Find(_T(".stl")) >= 0 )
		{
			STL* pSTL = stlCreate(10000, 100) ;
			if( stlLoad(pSTL, pathName.GetBuffer(0)) != 1 )
			{
				stlFree(pSTL) ;
				pSTL = NULL ;
				if (!b_mLangFlag)
					AfxMessageBox(_T("打开文件错误!")) ;
				else
					MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Open file error!"), _T("CAD"), MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			}
			else
			{
				if( fabs(m_unit-1.) > MIN_DBL ) // 若单位不是mm
					stlScale(pSTL, m_unit) ; // 转换单位
				stlCalAttribOfTris(pSTL, 1.e-5) ; // nt add 2017/3/16!!!

				// smf modify 2022/07/19
				if (GetNumOfSTL() < 1)
				{
					m_v[0] = -0.5*(pSTL->box.min[0] + pSTL->box.max[0]);
					m_v[1] = -0.5*(pSTL->box.min[1] + pSTL->box.max[1]);
					m_v[2] = /*m_ssp.h*/-pSTL->box.min[2]; // nt modify 2017/9/5
				}
				stlMove(pSTL, m_v);

				if (pSTL->box.min[0] < -0.5*m_ds[0] ||
					pSTL->box.max[0] > 0.5*m_ds[0] ||
					pSTL->box.min[1] < -0.5*m_ds[1] ||
					pSTL->box.max[1] > 0.5*m_ds[1] ||
					pSTL->box.max[2] > m_ds[2])
				{
					if (!b_mLangFlag)
						AfxMessageBox(_T("STL模型超过工作台范围!"));
					else
						MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("STL model is beyond the scope of the workbench!"), _T("CAD"), MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
					//AfxMessageBox(_T("STL model is beyond the scope of the workbench!"));
				}
				Add(pSTL) ;
				if (!b_mLangFlag)
					cadPromptStr(_T("打开文件成功")) ;
				else
					cadPromptStr(_T("Open file successfully"));
				int nSTL = GetNumOfSTL() ;
				if( nSTL > 1 )
				{
					CRCreate* pR = new CRCreate(pSTL) ;
					m_rs.Add(pR) ;
					SetModifiedFlag(TRUE) ;
				}
				else
				{
					EmptyUndo() ;
					SetPathName(pathName) ;
					SetTitle(title) ; // nt add 2017/9/1
				}
				Redraw() ;
			}
		}
		else
		if( pathName.Find(_T(".SWK")) >= 0 || // nt add 2017/9/1
			pathName.Find(_T(".swk")) >= 0 )
		{
			if( m_stls != NULL &&
				IsModified() == TRUE &&
				AfxMessageBox(_T("继续将清除现有模型？"), MB_YESNO) == IDNO )
				return ;
			EmptyUndo() ; // nt add 2017/9/2
			DelAllSTL() ;
			CFile file ;
			if( file.Open(pathName, CFile::modeRead) )
			{
				CArchive ar(&file, CArchive::load) ;
				Serialize(ar) ;
			}
			else
			{
				if (!b_mLangFlag)
					AfxMessageBox(_T("打开文件错误!"));
				else
					MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Open file error!"), _T("CAD"), MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
				//AfxMessageBox(_T("Open file error!"));
			}
			if (!b_mLangFlag)
				cadPromptStr(_T("打开文件成功")) ;
			else
				cadPromptStr(_T("Open file successfully"));
			SetPathName(pathName) ;
			CollisionDetect() ;
			SetTitle(title) ;
			Update() ;
			Redraw() ;
		}
	}

	return ;
}

BOOL CCADDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	BOOL b_mLangFlag = GetLang_doc() ;
	if( _tcsstr(lpszPathName, _T(".stl")) != NULL ||
		_tcsstr(lpszPathName, _T(".Stl")) != NULL ||
		_tcsstr(lpszPathName, _T(".STL")) != NULL)
	{
		STL* pSTL = stlCreate(10000, 100) ;
		CString pathName = lpszPathName ;
		if( stlLoad(pSTL, pathName.GetBuffer(0)) != 1 )
		{
			stlFree(pSTL) ;
			pSTL = NULL ;
			if (!b_mLangFlag)
				AfxMessageBox(_T("打开文件错误!")) ;
			else
				MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Open file error!"), _T("CAD"), MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		}
		else
		{
			if( fabs(m_unit-1.) > MIN_DBL ) // 若单位不是mm
				stlScale(pSTL, m_unit) ; // 转换单位
			stlCalAttribOfTris(pSTL, 1.e-5) ; // nt add 2017/3/16!!!
											  // nt add 2017/6/30
			VEC3D v ;
			v[0] = -0.5*(pSTL->box.min[0]+pSTL->box.max[0]) ;
			v[1] = -0.5*(pSTL->box.min[1]+pSTL->box.max[1]) ;
			v[2] = /*m_ssp.h*/-pSTL->box.min[2] ; // nt modify 2017/9/5
			stlMove(pSTL, v) ;
			// smf 2021/12/30 注释掉
			//if (pSTL->box.min[0] < -0.5*m_ds[0] ||
			//	pSTL->box.max[0] > 0.5*m_ds[0] ||
			//	pSTL->box.min[1] < -0.5*m_ds[1] ||
			//	pSTL->box.max[1] > 0.5*m_ds[1] ||
			//	pSTL->box.max[2] > m_ds[2])
			//{
			//	if (!b_mLangFlag)
			//		AfxMessageBox(_T("STL模型超过工作台范围!"));
			//	else
			//		MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("STL model is beyond the scope of the workbench!"), _T("CAD"), MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			//	//AfxMessageBox(_T("STL model is beyond the scope of the workbench!"));
			//}
			// end
			Add(pSTL) ;
			if (!b_mLangFlag)
				cadPromptStr(_T("打开文件成功")) ;
			else
				cadPromptStr(_T("Open file successfully"));
			int nSTL = GetNumOfSTL() ;
			if( nSTL > 1 )
			{
				CRCreate* pR = new CRCreate(pSTL) ;
				m_rs.Add(pR) ;
				SetModifiedFlag(TRUE) ;
			}
			else
			{
				EmptyUndo() ;
				SetPathName(pathName) ;
				CString title ;
				fileGetTitle(pathName, title) ;
				SetTitle(title) ; // nt add 2017/9/1
			}
			//Redraw() ;
		}
	}
	else
	if( _tcsstr(lpszPathName, _T(".swk")) != NULL ||
		_tcsstr(lpszPathName, _T(".Swk")) != NULL ||
		_tcsstr(lpszPathName, _T(".SWK")) != NULL )
	{
		if (!CDocument::OnOpenDocument(lpszPathName))
			return FALSE;
	}

	// TODO:  在此添加您专用的创建代码

	return TRUE;
}

//  2022/08/17 smf add: 输出Sline2的信息
void CCADDoc::OutPutSline2(ZB2 * zb2)
{
	CString filePath;
	efpGet(filePath); // 运行文件目录
	filePath += _T("\\..\\Works\\OutPutZB3.txt");
	FILE* fp = nullptr;
	_tfopen_s(&fp, filePath.GetBuffer(0), _T("w"));
	if (fp)
	{

		for (int ii = 0; ii < zb2->nx; ii++)
		{
			for (int jj = 0; jj < zb2->ny; jj++)
			{
				fprintf(fp, "/////////////////////////////////////////////\n");
				fprintf(fp, "[%d, %d]\t%d\t\n", ii, jj, zb2->lines[ii][jj].n);
				ISEGM * segment = zb2->lines[ii][jj].segms;
				for (int kk = 0; kk < zb2->lines[ii][jj].n; kk++)
				{
					fprintf(fp, "[%d, %d]\t%d\t%d\t%f\t%f\n", ii, jj, kk, segment->facetId, segment->z1, segment->z2);
					segment = segment->next;
				}
			}
		}
		fclose(fp);
	}
	fp = nullptr;
}

void CCADDoc::OutPutCB(CB * cb, float z)
{
	CString filePath;
	efpGet(filePath); // 运行文件目录
	filePath += _T("\\..\\Works\\OutPutCB.txt");
	FILE* fp = nullptr;
	_tfopen_s(&fp, filePath.GetBuffer(0), _T("w"));
	if (fp)
	{
		double center[3] = { 0., 0., 0. }; 
		CSEGM* segm = NULL;
		CELL *cell = NULL;
		CLINE* line = NULL;
		int kk = (z - cb->zmin) / cb->w;
		for (int i = 0; i < cb->nx; i++)
		{
			for (int j = 0; j < cb->ny; j++)
			{
				CSEGM * segm = cb->lines[i][j].segms;
				line = cbGetCLine(cb, i, j);
				if (line)
				{
					memcpy(center, line->p, sizeof(PNT2D));
					segm = line->segms;
					while (segm)
					{
						if (kk >= segm->k1 && kk <= segm->k2)
						{
							center[2] = cb->zmin + cb->w*(kk + 0.5);
							break;
						}
						segm = segm->next;
					}

					cell = line->cells;
					while (cell)
					{
						if (cell->k == kk)
						{
							int num = 0;
							for (int ii = 0; ii < 4; ii++)
							{
								if (cell->flags[ii])
								{
									if (cell->begin[ii][0] > 375 ||
										cell->begin[ii][1] > 375 ||
										cell->end[ii][0] > 375 || 
										cell->end[ii][1] > 375)
									{
										fprintf(fp, "第%d根杆：[%lf, %lf, %lf], [%lf, %lf, %lf]", ii, 
											cell->begin[ii][0], cell->begin[ii][1], cell->begin[ii][2], 
											cell->end[ii][0], cell->end[ii][1], cell->end[ii][2]);
									}
									num++;
								}
							}
							break;
						}
						cell = cell->next;
					}
				}

			}
		}
		fclose(fp);
	}
	fp = nullptr;
}

void CCADDoc::FindErrorCB(CB * cb)
{
	CString filePath;
	efpGet(filePath); // 运行文件目录
	filePath += _T("\\..\\Works\\FindErrorCB.txt");
	FILE* fp = nullptr;
	_tfopen_s(&fp, filePath.GetBuffer(0), _T("w"));
	if (fp)
	{
		double center[3] = { 0., 0., 0. };
		CSEGM* segm = NULL;
		CELL *cell = NULL;
		CLINE* line = NULL;
		int kk = 0;
		for (kk = 0; kk < cb->nz; kk++)
		{
			for (int i = 0; i < cb->nx; i++)
			{
				for (int j = 0; j < cb->ny; j++)
				{
					CSEGM * segm = cb->lines[i][j].segms;
					line = cbGetCLine(cb, i, j);
					if (line)
					{
						memcpy(center, line->p, sizeof(PNT2D));
						segm = line->segms;
						while (segm)
						{
							if (kk >= segm->k1 && kk <= segm->k2)
							{
								center[2] = cb->zmin + cb->w*(kk + 0.5);
								break;
							}
							segm = segm->next;
						}

						cell = line->cells;
						while (cell)
						{
							if (cell->k == kk)
							{
								int num = 0;
								for (int ii = 0; ii < 4; ii++)
								{
									if (cell->flags[ii])
									{
										if ((cell->begin[ii][0] * cell->begin[ii][0] + cell->begin[ii][1] * cell->begin[ii][1]) > 375 * 375 ||
											(cell->end[ii][0] * cell->end[ii][0] + cell->end[ii][1] * cell->end[ii][1]) > 375 * 375)
										{
											fprintf(fp, "[%d, %d, %d]\n第%d根杆：[%lf, %lf, %lf], [%lf, %lf, %lf]\n", i, j, cell->k, ii,
												cell->begin[ii][0], cell->begin[ii][1], cell->begin[ii][2],
												cell->end[ii][0], cell->end[ii][1], cell->end[ii][2]);
										}
										num++;
									}
								}
								break;
							}
							cell = cell->next;
						}
					}

				}
			}
		}
		fclose(fp);
	}
	fp = nullptr;
}

void CCADDoc::OnFileSave()
{
	ClearCmdAll() ;

	CString pathName = GetPathName() ;
	if( pathName.Find(_T(".SWK")) >= 0 ||
		pathName.Find(_T(".swk")) >= 0 )
	{
		OnSaveDocument(pathName) ;
		EmptyUndo() ; // nt add 2017/9/2
	}
	else
		OnFileSaveAs() ;

	return ;
}

// nt modify 2019/4/13
void CCADDoc::OnFileSaveAs()
{
	ClearCmdAll() ;

	if ( m_stls == NULL ) // no stl model
	{
		if( !GetLang_doc() )
			AfxMessageBox(_T("无模型!"));
		else
			AfxMessageBox(_T("no model!"));
		return ;
	}

	CString szFilter ;
	BOOL b_mLangFlag = GetLang_doc() ;
	if (!b_mLangFlag)
		szFilter = _T("模型 (.SWK)|*.swk|STL模型 (.STL)|*.stl||");
	else
		szFilter = _T("Model (.SWK)|*.swk|STL model (.STL)|*.stl||") ;
	CString title = GetTitle() ;
	int index = title.Find('.') ;
	CString str = title.Left(index) ;
	CFileDialog dlg(FALSE, _T(".SWK"), str, OFN_HIDEREADONLY, szFilter) ;
	
again:
	if( dlg.DoModal() == IDOK )
	{
		CString pathName = dlg.GetPathName() ;
		if( pathName.Find(_T(".SWK")) < 0 &&
			pathName.Find(_T(".swk")) < 0 )
			pathName += _T(".swk") ;
		CFileFind find ;
		if( find.FindFile(pathName) )
		{
			int ret ;
			if (!b_mLangFlag)
				ret = AfxMessageBox(_T("是否覆盖原文件?"), MB_YESNOCANCEL) ;
			else
				ret = AfxMessageBox(_T("Overwrite the original file?"), MB_YESNOCANCEL) ;
			if( ret == IDCANCEL )
				return ;
			else
			if( ret == IDNO )
				goto again ;
		}
		CString ext = dlg.GetFileExt() ;
		if( ext == _T("SWK") ||
			ext == _T("swk") )
		{
			OnSaveDocument(pathName) ;
			EmptyUndo() ; // nt add 2017/9/2
			SetPathName(pathName) ; // nt add 2017/9/1
			title = dlg.GetFileTitle() ;
			SetTitle(title) ;
		}
		else
		if( ext == _T("STL") ||
			ext == _T("stl") )
			ExportSTL(pathName) ;
	}

	return ;
}

void CCADDoc::OnFileNew()
{
	BOOL b_mLangFlag = GetLang_doc();
	if( !b_mLangFlag  && (AfxMessageBox(_T("清除所有模型？"), MB_YESNO) == IDYES ))	
	{
		ClearCmdAll() ;

		// delete STL model
		DelAllSTL() ;
		EmptyUndo() ; // nt add 2017/9/2
		Update() ; // nt add 2017/8/27

		// redraw
		CCADView* pView = GetView() ;
		if( pView )
			pView->OnViewFull() ;
	}
	//if (b_mLangFlag && (AfxMessageBox(_T("Clear all models?"), MB_YESNO) == IDYES))
	
	if (b_mLangFlag && (MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Clear all models?"), _T("CAD"), MB_YESNO | MB_ICONINFORMATION, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)) == IDYES))
	{
		ClearCmdAll();

		// delete STL model
		DelAllSTL();
		EmptyUndo(); // nt add 2017/9/2
		Update(); // nt add 2017/8/27

				  // redraw
		CCADView* pView = GetView();
		if (pView)
			pView->OnViewFull();
	}

	return ;
}
//--------------------------------------------------------------
void CCADDoc::OnEditUndo()
{
	int i, n = (int)m_rs.GetSize() ;
	double scale ;
	VEC3D v ;
	TRANS t ;
	CRSize* pRSize = NULL ;
	CRHeight* pRHeight = NULL ;
	CRDir* pRDir = NULL ;
	CRMakeSS* pRMakeSS = NULL ;
	CRDelSS* pRDelSS = NULL ;
	STL* stl = NULL ;
	SS* ss = NULL ;

	if( n > 0 )
	{
		CRecord* pR = (CRecord*)m_rs[n-1] ;
		m_rs.RemoveAt(n-1) ;
		if( pR )
		{
			int type = pR->GetType() ;
			stl = pR->GetSTL() ;
			switch(type)
			{
			case rtRCreate:
				Remove(stl) ;
				stlFree(stl) ;
				pR->SetSTL(NULL) ;
				break ;
			case rtRDelete:
				Add(stl) ;
				pR->SetSTL(NULL) ;
				break ;
			case rtRCut:
				Add(stl) ;
				pR->SetSTL(NULL) ;
				m_copy = NULL ;
				break ;
			case rtRCopy:
				m_copy = NULL ;
				break ;
			case rtRSize:
				pRSize = (CRSize*)pR ;
				if( pRSize->m_scale < MIN_DBL )
					break ;
				scale = 100./pRSize->m_scale ;
				stlScale2(stl, pRSize->m_center, scale) ;
				break ;
			case rtRHeight:
				pRHeight = (CRHeight*)pR ;
				v[0] = 0. ;
				v[1] = 0. ;
				v[2] = pRHeight->m_z ;
				ss = pRHeight->m_ss ;
				stlReleaseSS(stl) ;
				stlMove(stl, v) ;
				stl->ss = ss ;
				break ;
			case rtRDir:
				pRDir = (CRDir*)pR ;
				for( i = pRDir->m_n-1 ; i >= 0 ; i-- )
				{
					t = pRDir->m_ts[i] ;
					if( t.type == 0 )
					{
						t.v[0] = -t.v[0] ;
						t.v[1] = -t.v[1] ;
						t.v[2] = -t.v[2] ;
						stlMove(stl, t.v) ;
					}
					else
					if( t.type == 1 )
						stlRotate(stl, t.p, t.v, -t.a) ;
					else
					if( t.type == 2 )
						stlScale2(stl, t.p, 1/t.a) ;
				}
				stlCalBox(stl) ;
				break ;
			case rtRMakeSS:
				pRMakeSS = (CRMakeSS*)pR ;
				stlReleaseSS(stl) ; // free pRMakeSS->m_created
				stl->ss = pRMakeSS->m_old ;
				pRMakeSS->m_created = NULL ;
				pRMakeSS->m_old = NULL ;
				break ;
			case rtRDelSS:
				pRDelSS = (CRDelSS*)pR ;
				stl->ss = pRDelSS->m_ss ;
				pRDelSS->m_ss = NULL ;
				break ;
			default:
				break ; // error
			}
			delete pR ;
		}

		Update() ;
		Redraw() ;
	}

	if( m_rs.GetSize() < 1 ||
		m_stls == NULL )
		SetModifiedFlag(FALSE) ;

	return ;
}

void CCADDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( m_rs.GetSize() > 0 )
		pCmdUI->Enable(TRUE) ;
	else
		pCmdUI->Enable(FALSE) ;
}

void CCADDoc::OnEditSize()
{
	int n ;
	STL* stl = NULL ;
	CSet* pSet = new CSet() ;

	InitCmd(ID_EDIT_SIZE) ;

	n = GetAllOfSelectedSTL2(pSet) ; // 收集选中的STL
	if( n > 0 )
	{
		stl = (STL*)pSet->GetObj(0) ;
		if( stl )
		{
			CSizeDlg* pDlg = new CSizeDlg() ;
			pDlg->Init(stl, this) ;
			pDlg->Create(IDD_SIZEDLG) ;
			// nt add 2017/8/23
			CCADView* pView = GetView() ;
			RECT rect ;
			pView->GetWindowRect(&rect) ;
			pDlg->SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE) ;
			// end
			pDlg->ShowWindow(SW_SHOW) ;
			pDlg->EnableUndo(FALSE) ; // nt add 2017/8/24
			m_pDlg = pDlg ;
		}
	}

	delete pSet ;

	return;
}

void CCADDoc::OnEditDir()
{
	int n ;
	STL* stl = NULL ;
	CSet* pSet = new CSet() ;

	InitCmd(ID_EDIT_DIR) ;

	n = GetAllOfSelectedSTL2(pSet) ; // 收集选中的STL
	if( n > 0 )
	{
		stl = (STL*)pSet->GetObj(0) ;
		if( stl )
		{
			//stl->stt = stlNormal ; // nt add 2017/8/26 注释掉
			CDirDlg* pDlg = new CDirDlg() ;
			pDlg->Init(stl, this) ;
			pDlg->Create(IDD_DIRDLG) ;
			// nt add 2017/8/23
			CCADView* pView = GetView() ;
			RECT rect ;
			pView->GetWindowRect(&rect) ;
			pDlg->SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE) ;
			// end
			pDlg->ShowWindow(SW_SHOW) ;
			m_pDlg = pDlg ;
			Redraw() ;
		}
	}

	delete pSet ;

	return;
}

// nt add 2017/8/22
void CCADDoc::OnEditHeight()
{
	int i, n = 0 ;
	VEC3D v ;
	STL* stl = NULL ;
	CSet* pSet = new CSet() ;
	BOOL b_mLangFlag = GetLang_doc();
	InitCmd(ID_EDIT_DIR) ;

	n = GetAllOfSelectedSTL2(pSet) ; // 收集选中的STL
	if (n == 0)
	{
		if (!b_mLangFlag)
			cadPromptStr(_T("没有STL模型!"));
		else
			cadPromptStr(_T("No STL model!"));
	}
	else
	{
		if (!b_mLangFlag)
			cadPromptStr(_T("将所选STL模型置于工作台上"));
		else
			cadPromptStr(_T("Place the selected STL model on the workbench"));
	}
	v[0] = 0. ;

	v[1] = 0. ;
	for( i = 0 ; i < n ; i++ )
	{
		stl = (STL*)pSet->GetObj(i) ;
		if( stl )
		{
			stlCalBox(stl) ;
			v[2] = -stl->box.min[2] ;
			// nt add 2017/9/3
			CRHeight* pRHeight = new CRHeight(stl, -v[2], (SS*)(stl->ss)) ;
			m_rs.Add(pRHeight) ;
			stl->ss = NULL ;
			// end
			stlReleaseSS(stl) ;
			stlMove(stl, v) ;
		}
	}
	delete pSet ;

	if( n > 0 )
	{
		Update() ; // nt add 2017/8/23
		Redraw() ;
	}

	return;
}

// nt add 2017/8/22
void CCADDoc::OnEditDelSupport()
{
	int i, n, m = 0 , base ;
	CSet* pSet = new CSet() ;
	STL* stl ;
	BOOL b_mLangFlag = GetLang_doc();
	InitCmd(ID_EDIT_DELSUPPORT) ;

	n = GetAllOfSelectedSTL2(pSet) ; // 取拾取的STL模型
	for( i = 0 ; i < n ; i++ )
	{
		stl = (STL*)pSet->GetObj(i) ;
		if( stl->ss )
			m++ ;
	}
	if( m < 1 )
	{
		if (!b_mLangFlag)
			cadPromptStr(_T("没有带支撑的STL模型!"));
		else
			cadPromptStr(_T("No STL model with support!"));
		delete pSet ;
		return ;
	}
	if (!b_mLangFlag)	
		cadPromptStr(_T("保留了所有手动生成的支撑")) ;
	else
		cadPromptStr(_T("All manually generated supports are retained"));

	if (!b_mLangFlag)
	{
		if (AfxMessageBox(_T("清除自动生成的支撑后是否保留底座？"), MB_YESNO) == IDYES)
			base = 1;
		else
			base = 0;
	}
	if(b_mLangFlag)
	{
		if (MessageBoxEx(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("Keep the base after clearing the automatically generated support?"), _T("CAD"), MB_ICONEXCLAMATION | MB_YESNO, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)) == IDYES)
			base = 1;
		else
			base = 0;
	}
	
	for( i = 0 ; i < n ; i++ )
	{
		stl = (STL*)pSet->GetObj(i) ;
		//stlReleaseAuto(stl, base) ;
		if( stl->ss ) // nt add 2017/9/4
		{
			SS* old = ssCopy((SS*)stl->ss) ;
			stlReleaseAuto(stl, base) ;
			CRMakeSS* pRMakeSS = new CRMakeSS(stl, (SS*)stl->ss, old) ;
			m_rs.Add(pRMakeSS) ;
			SetModifiedFlag(TRUE) ;
		}
	}

	if( n > 0 )
		Redraw() ;

	delete pSet ;

	return ;
}

void CCADDoc::OnEditSupport()
{
	CCADView* pView = GetView() ;
	CSet* pSet = new CSet() ;
	STL* stl ;
	CEditSupport* pEdit = NULL ;

	InitCmd(ID_EDIT_SUPPORT) ;

	if( GetAllOfSelectedSTL(pSet) > 0 ) // 取拾取的STL模型
	{
		stl = (STL*)pSet->GetObj(0) ;
		pEdit = new CEditSupport(this, pView, stl, &m_ssp) ;
		m_pCmd = pEdit ;
		cadPromptClear() ;
		Redraw() ;
	}

	delete pSet ;

	return;
}

// nt add 2017/6/8
void CCADDoc::OnEditRod()
{
	CCADView* pView = GetView() ;
	CSet* pSet = new CSet() ;
	STL* stl ;
	CEditRod* pEdit = NULL ;

	InitCmd(ID_EDIT_SUPPORT) ;

	if( GetAllOfSelectedSTL2(pSet) > 0 ) // 取拾取的STL模型
	{
		stl = (STL*)pSet->GetObj(0) ;
		pEdit = new CEditRod(this, pView, stl, m_ssp.r2) ;
		m_pCmd = pEdit ;
		cadPromptClear() ;
		Redraw() ;
	}

	delete pSet ;

	return;
}

// 只cut第一个选中的STL模型
void CCADDoc::OnEditCut()
{
	int n, refresh = 0 ;
	STL* stl = NULL ;
	CSet* pSet = new CSet() ;
	BOOL b_mLangFlag = GetLang_doc();
	InitCmd(ID_EDIT_CUT) ;

	n = GetAllOfSelectedSTL(pSet) ; // 收集选中的STL
	if( n > 0 )
	{
		stl = (STL*)pSet->GetObj(0) ;
		if( stl )
		{
			Remove(stl) ;
			refresh = 1 ;
			m_copy = stl ;
			// nt add 2017/9/2
			CRCut* pRCut = new CRCut(stl) ;
			m_rs.Add(pRCut) ;
			SetModifiedFlag(TRUE) ;
			// end
		}
	}
	if (n > 1)
	{
		if (!b_mLangFlag)
			cadPromptStr(_T("只剪切一个选中的STL模型!"));
		else
			cadPromptStr(_T("Cut only one selected STL model!"));
	}

	delete pSet ;

	if( refresh )
		Redraw() ;

	return ;
}

// nt add 2019/7/10
void CCADDoc::OnEditSTLMaterial()
{
	int i, n ;
	STL* stl = NULL ;
	CSet* pSet = new CSet() ;
	BOOL b_mLangFlag = GetLang_doc();
	InitCmd(ID_EDIT_STLMATERIAL) ;

	n = GetAllOfSelectedSTL(pSet) ; // 收集选中的STL
	for( i = 0 ; i < n ; i++ )
	{
		stl = (STL*)pSet->GetObj(i) ;
		stl->material = m_parameter.material ;
	}
	if (n > 0)
	{
		SetModifiedFlag(TRUE) ;
		if (!b_mLangFlag)
			cadPromptStr(_T("STL模型的材料属性设置为当前材料"));
		else
			cadPromptStr(_T("Set the STL model's material attribute with current material"));
	}

	delete pSet ;

	return ;
}

// 只copy第一个选中的STL模型
void CCADDoc::OnEditCopy()
{
	int n, refresh = 0 ;
	STL* stl = NULL ;
	CSet* pSet = new CSet() ;
	BOOL b_mLangFlag = GetLang_doc();
	InitCmd(ID_EDIT_COPY) ;

	n = GetAllOfSelectedSTL(pSet) ; // 收集选中的STL
	if( n > 0 )
	{
		stl = (STL*)pSet->GetObj(0) ;
		if( stl )
		{
			refresh = 1 ;
			m_copy = stl ;
			// nt add 2017/9/2
			CRCopy* pRCopy = new CRCopy(stl) ;
			m_rs.Add(pRCopy) ;
			// end
		}
	}
	if (n > 1)
	{
		if (!b_mLangFlag)
			cadPromptStr(_T("只拷贝一个选中的STL模型!"));
		if (b_mLangFlag)
			cadPromptStr(_T("Copy only one selected STL model!"));
	}
	delete pSet ;

	if( refresh )
		Redraw() ;

	return ;
}

void CCADDoc::OnEditPaste()
{
	InitCmd(ID_EDIT_PASTE) ;

	if( m_copy )
	{
		PNT3D insertPos ;
		stlGetCenter(m_copy, insertPos) ;
		CCADView* pView = GetView() ;
		CPaste* pPaste = new CPaste(this, pView, insertPos, m_copy) ;
		m_pCmd = pPaste ;
		CollisionDetect() ; // nt add 2017/7/3
		cadPromptClear() ;
	}

	return;
}

// nt add 2017/9/3
void CCADDoc::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( m_copy )
		pCmdUI->Enable(TRUE) ;
	else
		pCmdUI->Enable(FALSE) ;
}

void CCADDoc::OnEditDelete()
{
	int i, n, refresh = 0 ;
	STL* stl = NULL ;
	CSet* pSet = NULL ;

	// nt modify 2017/9/6
	//if( m_pCmd ||
	//	m_pDlg )
	//	return ;
	ClearCmdAll() ;

	InitCmd(ID_EDIT_DELETE) ;

	pSet = new CSet() ;

	n = GetAllOfSelectedSTL(pSet) ;
	for( i = 0 ; i < n ; i++ )
	{
		stl = (STL*)pSet->GetObj(i) ;
		if( stl )
		{
			Remove(stl) ;
			//stlFree(stl) ;
			// nt add 2017/9/2
			CRDelete* pRDelete = new CRDelete(stl) ;
			m_rs.Add(pRDelete) ;
			SetModifiedFlag(TRUE) ;
			// end
			stl = NULL ;
			refresh = 1 ;
		}
	}

	delete pSet ;

	if( refresh )
		Redraw() ;

	return ;
}

void CCADDoc::OnEditSelectAll()
{
	int refresh = 0 ;
	STL* stl ;

	InitCmd(ID_EDIT_SELECTALL) ;

	for( stl = m_stls ; stl != NULL ; stl = stl->next )
	{
		if( stl->stt != stlSelected )
		{
			stl->stt = stlSelected ;
			refresh = 1 ;
		}
	}

	if( refresh )
		Redraw() ;

	return;
}

void CCADDoc::OnEditCancelSel()
{
	int refresh = 0 ;
	STL* stl ;

	InitCmd(ID_EDIT_CANCELSEL) ;

	for( stl = m_stls ; stl != NULL ; stl = stl->next )
	{
		if( stl->stt == stlSelected )
		{
			stl->stt = stlNormal ;
			refresh = 1 ;
		}
	}

	if( refresh )
		Redraw() ;

	return;
}

void CCADDoc::OnInsertSupport()
{
	InitCmd(ID_INSERT_SUPPORT) ;

	CollisionDetect() ; // nt add 2017/5/21

	CSSDlg dlg ;
	dlg.m_r = m_ssp.r ;
	dlg.m_density = 0.01*m_ssp.sDensity ;
	dlg.DoModal() ;

	return ;
}

void CCADDoc::OnInsertLayout()
{
	InitCmd(ID_INSERT_LAYOUT) ;

	// nt add 2017/9/5
	CCADView* pView = cadGetView() ;
	if( pView )
		pView->ViewUp() ;

	CLayoutDlg* pDlg = new CLayoutDlg() ;
	pDlg->Init(this) ;
	pDlg->Create(IDD_LAYOUTDLG) ;
	// nt add 2017/8/23
	RECT rect ;
	pView->GetWindowRect(&rect) ;
	pDlg->SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE) ;
	// end
	pDlg->ShowWindow(SW_SHOW) ;
	m_pDlg = pDlg ;

	return;
}
//-------------------------------------------------------------
// nt rewrite 2017/5/11
void CCADDoc::OnPrintSet()
{

	CPrinterSetupDlg dlg ;
	dlg.m_li = m_li ;
	dlg.m_lt = m_lt ;
	dlg.m_color = m_color ;
	dlg.m_h = m_h ;
	dlg.m_cur = m_parameter ;
	dlg.m_constH = m_constH ;
	if( dlg.DoModal() == IDOK )
	{
		m_parameter = dlg.m_cur ;
		m_ds[0] = m_parameter.L ;
		m_ds[1] = m_parameter.W ;
		m_ds[2] = m_parameter.H ;
		m_li = dlg.m_li ;
		m_lt = dlg.m_lt ;
		m_color = dlg.m_color ;
		m_h = dlg.m_h ;
		m_constH = dlg.m_constH ;
		Update() ; // nt add 2017/9/7
		Redraw() ;
	}

	return;
}

/*void CCADDoc::OnPrintInit()
{
	// 初始化打印机，为打印做准备
	
	return;
}*/

void CCADDoc::OnPrint()
{
	InitCmd(ID_PRINT) ;

	CollisionDetect() ; // nt add 2017/5/21

	CPrintDlg dlg ;
	dlg.DoModal() ;

	return ;
}

void CCADDoc::OnPrintPreview()
{
	return;
}

void CCADDoc::OnHelpShortCutKeys()
{
	CHotKey dlg;
	dlg.DoModal();
	return;
}

void CCADDoc::OnHelpSupport()              // HelpSupport
{
	ClearCmdAll() ;
	CString Path_support = GetModuleDir_doc();
	//CFileDialog dlg(TRUE);
	Path_support = Path_support + _T("\\User manual.pdf");
	ShellExecute(NULL, _T("open"), Path_support,	NULL, NULL, SW_SHOWNORMAL);
	
	//"..\..\Works\support.txt"
	/*
	CString szFilter = _T("打印数据 (.P_F)|*.p_f|") ;
	szFilter += _T("所有文件 (*.*)|*.*|") ;
	LPTSTR pch = szFilter.GetBuffer(0);
	while((pch = _tcschr(pch, '|')) != NULL)
		*pch++ = '\0' ;
	CFileDialog dlg(TRUE) ;
	dlg.m_ofn.lpstrFilter = szFilter ;
	if( dlg.DoModal() == IDOK )
	{
		CString pathName = dlg.GetPathName() ;
		if( pathName.Find(_T(".P_F")) >= 0 ||
			pathName.Find(_T(".p_f")) >= 0 )
		{
			PF* pf = pfCreate2() ;
			pfLoad(pf, pathName) ;
			LAYER* layer ;
			for( layer = pf->layers ; layer ; layer = layer->next )
			{
				layerSave2(layer, 
					       pf->nx, 
					       pf->ny, 
					       pathName) ;
			}
			pfFree(pf) ;
		}
	}*/

	return ;
}

void CCADDoc::OnHelpFeedback()      //FeedBack
{
	InitCmd(ID_HELP_FEEDBACK);
	FeedbackDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	
	
	/*FeedbackDlg *pDlg = new FeedbackDlg();
	pDlg->Init(this);
	pDlg->Create(IDD_FEEDBACK_DLG);

	CLayoutDlg* pDlg = new CLayoutDlg();
	pDlg->Init(this);
	pDlg->Create(IDD_LAYOUTDLG);*/

	/*CFile file;
	CString filepath, filename;
	filepath = _T("D://3DPrint/FeedBack/"); //文件位置
	filename = "feedback.txt";  //文件名
	filename = filepath + filename;  //文件路径
	if (file.Open(filename, CFile::modeReadWrite | CFile::modeNoTruncate))
	{

		if (file.Seek(0, CFile::begin) == file.Seek(0, CFile::end))
		{
			file.SeekToEnd();
			file.Write("\xff\xfe", 2); // 设置Unicode编码文件头 
			file.Write("You can send your suggestions to us!", strlen("You can send your suggestions to us!")); //写入文件
			file.Close();  //关闭文件
		}
	}*/

	/*//新建文档
	//以下一一种建立并写入TXT文件的方式   
	CFile file;
	CString filepath, filename;
	filepath = _T("D://3DPrint/FeedBack/"); //文件位置
	filename = "feedback.txt";  //文件名
	filename = filepath + filename;  //文件路径
	if (file.Open(filename, CFile::modeReadWrite | CFile::modeNoTruncate))
	{
		
		if (file.Seek(0, CFile::begin) == file.Seek(0, CFile::end))
		{
			file.SeekToEnd();
			file.Write("You can send your suggestions to us!", strlen("You can send your suggestions to us!")); //写入文件
			file.Close();  //关闭文件
		}
		else
		{
			file.Open(filename, CFile::modeReadWrite | CFile::modeNoTruncate);
			file.SeekToEnd();
			file.Write("\n", strlen("\n")); //写入文件
			file.Close();
		}
	}
	else
	{

	}*/

	
	return;
}
void CCADDoc::OnButtonDef()
{
	int n;
	STL* stl = NULL;
	CSet* pSet = new CSet();

	InitCmd(ID_EDIT_DIR);

	n = GetAllOfSelectedSTL2(pSet); // 收集选中的STL
	if (n > 0)
	{
		stl = (STL*)pSet->GetObj(0);
		if (stl)
		{
		}
	}

	delete pSet;

	return;

	//MessageBox(NULL, (LPCTSTR)L"error", (LPCTSTR)L"错误", MB_ICONHAND);

}


//--------------------------------------------------------------
CCADDoc* cadGetDoc()
{
	CMainFrame* pMF = (CMainFrame*)AfxGetApp()->m_pMainWnd ;  
	CCADView* pView = (CCADView*)pMF->GetActiveView() ;   
	CCADDoc* pDoc = pView->GetDocument() ;

	return pDoc ;
}
///////////////////////////////////////////////////////////////

