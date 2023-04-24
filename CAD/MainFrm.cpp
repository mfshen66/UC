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

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "CAD.h"
#include "MFCVisualManagerOffice20072.h"
#include "MainFrm.h"
#include "SplashDlg.h" // nt add 2017/5/5

#include "CADDoc.h" // nt add 2017/5/14
#include "CADView.h" // nt add 2017/8/28
#include "Splash.h" // nt add 2017/5/30



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
	ON_COMMAND(ID_SLIDER1, &CMainFrame::OnSlider1)
	ON_COMMAND(ID_COMBO_DEF, &CMainFrame::OnComboDef)
//	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

CMFCRibbonStatusBarPane* CMainFrame::GetPane(int index)
{
	if( index < 1 )
		return (CMFCRibbonStatusBarPane*)m_wndStatusBar.GetElement(0) ;
	else
		return (CMFCRibbonStatusBarPane*)m_wndStatusBar.GetExElement(0) ;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	
	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	CMFCRibbonQuickAccessToolBar* pQAToolbar = m_wndRibbonBar.GetQuickAccessToolbar();
	pQAToolbar->RemoveAll();
	
	// nt add 2017/5/14
	CMFCRibbonBaseElement* pObj ;
	CMFCRibbonEdit* pEdit ;
	pObj = m_wndRibbonBar.FindByID(ID_EDIT1) ;
	pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pObj) ;
	if( pEdit )
		pEdit->SetTextAlign(ES_RIGHT) ;
	pObj = m_wndRibbonBar.FindByID(ID_EDIT2) ;
	pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pObj) ;
	if( pEdit )
		pEdit->SetTextAlign(ES_RIGHT) ;
	pObj = m_wndRibbonBar.FindByID(ID_EDIT3) ;
	pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pObj) ;
	if( pEdit )
		pEdit->SetTextAlign(ES_RIGHT) ;
	pObj = m_wndRibbonBar.FindByID(ID_EDIT4) ;
	pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pObj) ;
	if( pEdit )
		pEdit->SetTextAlign(ES_RIGHT) ;
	pObj = m_wndRibbonBar.FindByID(ID_EDIT5) ;
	pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pObj) ;
	if( pEdit )
		pEdit->SetTextAlign(ES_RIGHT) ;

	CMFCRibbonComboBox *pCmbox;
	pCmbox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_DEF));

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);


	// nt add 2017/8/28
	pObj = m_wndRibbonBar.FindByID(ID_SLIDER1) ;
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pObj) ;
	if( pSlider )
	{
		pSlider->SetRange(0, 0) ;
		pSlider->SetZoomIncrement(1) ;
	}
	
	// nt add 2017/5/5
	/*CSplashDlg* dlg = new CSplashDlg(this) ;
	dlg->Create(IDD_SPLASH, this) ; // 创建对话框
	dlg->CenterWindow() ;
	dlg->ShowWindow(SW_SHOW) ; // 显示对话框
	dlg->UpdateWindow() ;
	Sleep(500); // 画面显示停留时间，单位为毫秒
	dlg->DestroyWindow(); // 销毁对话框
	delete dlg ;*/

	// nt add 2017/5/30
	CSplash *pSplash = new CSplash() ;
	pSplash->Create(this, 
		            NULL, 
		            500, 
		            CSS_FADE|CSS_CENTERSCREEN|CSS_SHADOW) ;
	pSplash->SetBitmap(IDB_SPLASH, 255, 0, 255) ;
	pSplash->Show() ;

	// smf add 2021/11/03
	CMFCRibbonCategory *pCategory = m_wndRibbonBar.GetCategory(1);
	CMFCRibbonPanel *pPanel = pCategory->GetPanel(3);
	m_wndRibbonBar.SetRedraw(FALSE);
	CClientDC dc(&m_wndRibbonBar);
	CFont *pOldFont = dc.SelectObject(m_wndRibbonBar.GetFont());
	//pPanel->Add(new CMFCRibbonButton(ID_BUTTON_DEF, _T("模型属性"), 18, 18));
	pPanel->Add(new CMFCRibbonComboBox(ID_COMBO_DEF, 1, 50, _T("属性"), 18));
	CMFCRibbonComboBox *pCombox = (CMFCRibbonComboBox* )m_wndRibbonBar.FindByID(ID_COMBO_DEF);
	pCombox->AddItem(_T("未指定"), 0);
	pCombox->AddItem(_T("填充体"), 1);
	pCombox->AddItem(_T("空腔"), 2);
	pCombox->AddItem(_T("蒙皮"), 0);
	pCombox->SetEditText(_T("未指定"));

	pPanel->Add(new CMFCRibbonButton(IDC_BUTTON_CBFILL, _T("晶格填充"), -1, -1));
	CMFCRibbonButton* pButton = (CMFCRibbonButton*)m_wndRibbonBar.FindByID(IDC_BUTTON_CBFILL);

	for (int i = 0; i < pCategory->GetPanelCount(); i++)
	{
		pPanel = pCategory->GetPanel(i);
		CRect rc = pPanel->GetRect();
		pPanel->RecalcWidths(&dc, rc.Height());
	}
	pCategory->ReposPanels(&dc);
	dc.SelectObject(pOldFont);
	m_wndRibbonBar.Invalidate();
	m_wndRibbonBar.SetRedraw(TRUE);
	m_wndRibbonBar.RedrawWindow();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = ID_VIEW_APPLOOK_WINDOWS_7;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice20072::SetStyle(CMFCVisualManagerOffice20072::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice20072::SetStyle(CMFCVisualManagerOffice20072::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice20072::SetStyle(CMFCVisualManagerOffice20072::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice20072::SetStyle(CMFCVisualManagerOffice20072::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice20072));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMainFrame::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 强制关闭“打印预览”模式
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}

void CMainFrame::OnSlider1()
{
	// TODO: 在此添加命令处理程序代码
	CCADDoc* pDoc = cadGetDoc() ;
	CCADView* pView = cadGetView() ;
	if( pDoc == NULL ||
		pView == NULL )
		return ;
	pDoc->ClearCmdAll() ;

	CMFCRibbonBaseElement* pObj = m_wndRibbonBar.FindByID(ID_SLIDER1) ;
	CMFCRibbonEdit* pEdit ;
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, pObj) ;
	if( pSlider )
	{
		int nPos = pSlider->GetPos() ;
		if( nPos > pDoc->m_nLayer )
			nPos = pDoc->m_nLayer ;
		pDoc->m_iLayer = nPos ;
		pObj = m_wndRibbonBar.FindByID(ID_EDIT5) ;
		pEdit = DYNAMIC_DOWNCAST(CMFCRibbonEdit, pObj) ;
		if( pEdit )
		{
			CString str ;
			str.Format(_T("%d"), nPos) ;
			pEdit->SetEditText(str) ;
		}
	}

	int n = pDoc->GetNumOfSTL() ;
	if( n < 1 )
	{
		cadPromptStr(_T("无STL模型!")) ;
		return ;
	}

	float z = (float)(pDoc->m_iLayer*pDoc->GetZMax()/pDoc->m_nLayer) ; // 2022/08/16 smf modify: 强转double to float
	pDoc->CutAll(z) ;
	pView->m_drawSection = 1 ;
	pView->m_z = z ;
	pView->Invalidate(FALSE) ;
	//pView->m_drawSection = 0 ;

	return ;
}

void CMainFrame::OnComboDef()
{
	CCADDoc* pDoc = cadGetDoc();
	CCADView* pView = cadGetView();
	if (pDoc == NULL ||
		pView == NULL)
		return;
	pDoc->ClearCmdAll();

	CMFCRibbonComboBox* pCombo = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_DEF));
	int n = (int)pCombo->GetCount();
	int nIndex = pCombo->GetCurSel();
	pDoc->SetPropertySTL(nIndex);

	return;
}

//==============================================================
// nt add 2015/11/2
// prompting string, nt add 2009/10/21
static CString prompting ; // nt add 2009/10/21
// prompting flag, nt add 2009/12/15, 解决提示和键盘输入的冲突问题
static int pf ; // =1:prompting, =0:not prompting
//--------------------------------------------------------------
// nt add 2009/10/21
// nt modify 2015/8/5
void cadPromptUpdate()
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd() ;
	if( pMainWnd == NULL ) // nt add 2017/9/6
		return ;
	CMFCRibbonStatusBarPane* pPane = pMainWnd->GetPane(0) ;
	if( pPane )
	{
		pPane->SetText(prompting) ;
		pPane->Redraw() ;
	}

	return ;
}

void cadPromptAttach(TCHAR c)
{
	TCHAR s[2] ;
	s[0] = c ;
	s[1] = _T('\0') ;
	// nt add 2009/12/15
	if( pf == 1 )
		prompting = _T("") ;
	pf = 0 ;
	// end
	prompting += s ;
	cadPromptUpdate() ;
	return ;
}

void cadPromptAttach(CString& str)
{
	prompting += str ;
	cadPromptUpdate() ;
	return ;
}

void cadPromptGet(CString& str)
{
	str = prompting ;
	return ;
}

void cadPromptClear()
{
	prompting = _T("") ;
	cadPromptUpdate() ;
	return ;
}
//--------------------------------------------------------------
/*	2002/9/27 nt
*	prompt str in statusbar
*/
void cadPromptStr(CString& str)
{
	prompting = str ; // nt add 2009/10/21
	cadPromptUpdate() ; // nt add 2009/10/21
	pf = 1 ; // nt add 2009/12/15

	return ;
}

void cadPromptStr(TCHAR* text)
{
	CString str ;
	str = text ;
	cadPromptStr(str) ;

	return ;
}

// nt modify 2015/8/5
// nt modify 2016/2/15
void cadPromptPt2D(double p[2])
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd() ;
	if( pMainWnd == NULL ) // nt add 2017/9/6
		return ;
	CMFCRibbonStatusBarPane* pPane = pMainWnd->GetPane(1) ;
	if( pPane )
	{
		CString str ;
		str.Format(_T("x=%5.2lf,y=%5.2lf"), p[0], p[1]) ;
		pPane->SetText(str) ;
		pPane->Redraw() ; 
	}

	return ;
}

void cadPromptPt(double p[3])
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd() ;
	if( pMainWnd == NULL ) // nt add 2017/9/6
		return ;
	CMFCRibbonStatusBarPane* pPane = pMainWnd->GetPane(1) ;
	if( pPane )
	{
		CString str ;
		str.Format(_T("x=%5.2lf,y=%5.2lf,z=%5.2lf"), p[0], p[1], p[2]) ;
		pPane->SetText(str) ;
		pPane->Redraw() ; 
	}

	return ;
}
//==============================================================
/*
void CMainFrame::OnClose()
{
	BOOL bStartFlag =GetStart_main();
	if (bStartFlag)

	{

		CString strFileName = _T("");

		GetModuleFileName(NULL, strFileName.GetBuffer(MAX_PATH), MAX_PATH);

		ShellExecute(NULL, _T(""), strFileName, NULL, NULL, SW_SHOWNORMAL);

		strFileName.ReleaseBuffer();

	}
	CFrameWndEx::OnClose();
}*/