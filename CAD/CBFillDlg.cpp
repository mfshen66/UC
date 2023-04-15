// CBFillDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CADDoc.h"
#include "CAD.h"
#include "CBFillDlg.h"
#include "afxdialogex.h"
#include "CB.h"

#ifndef MIN_CELL_SIZE
// 最短边长
#define MIN_CELL_SIZE 0.2
#endif // !MIN_CELL_SIZE

#ifndef MAX_CELL_SIZE
// 最短边长
#define MAX_CELL_SIZE 1000.
#endif // !MAX_CELL_SIZE

#ifndef PLAT_RANGE
// 工作台范围
#define PLAT_RANGE 10000.
#endif // !PLAT_RANGE

// CBFillDlg 对话框

IMPLEMENT_DYNAMIC(CBFillDlg, CDialogEx)

CBFillDlg::CBFillDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBFILLDLG, pParent)
{
	m_cell_type = 0;
	m_size.l = 0.;
	m_size.w = 0.;
	m_size.h = 0.;
	m_r_edge = 0.;
	m_r_vertex = 0;
	m_shape = 0;
	m_remove_opt = 0;
	m_trans_seed.pos[0] = 0.;
	m_trans_seed.pos[1] = 0.;
	m_trans_seed.pos[2] = 0.;
	m_trans_seed.rot[0] = 0.;
	m_trans_seed.pos[1] = 0.;
	m_trans_seed.pos[2] = 0.;
	m_prg.ws = NULL;
}

CBFillDlg::~CBFillDlg()
{
}

void CBFillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	int isSquare = -1;
	DDX_Check(pDX, IDC_CHECK_SQUARE, isSquare);
	if (isSquare)
	{
		double l = 0.;
		DDX_Text(pDX, IDC_EDIT_CELLW, l);
		DDV_MinMaxDouble(pDX, l, MIN_CELL_SIZE, DBL_MAX);
		m_size.l = l;
		m_size.w = l;
		m_size.h = l;
	}
	else
	{
		DDX_Text(pDX, IDC_EDIT_CELL_L, m_size.l);
		DDV_MinMaxDouble(pDX, m_size.l, MIN_CELL_SIZE, MAX_CELL_SIZE);
		DDX_Text(pDX, IDC_EDIT_CELL_W, m_size.w);
		DDV_MinMaxDouble(pDX, m_size.w, MIN_CELL_SIZE, MAX_CELL_SIZE);
		DDX_Text(pDX, IDC_EDIT_CELL_H, m_size.h);
		DDV_MinMaxDouble(pDX, m_size.h, MIN_CELL_SIZE, MAX_CELL_SIZE);
	}
	DDX_CBIndex(pDX, IDC_COMBO_CELLTYPE, m_cell_type);
	DDX_Text(pDX, IDC_EDIT_RADIAS_V, m_r_edge);
	DDX_Text(pDX, IDC_EDIT_RADIAS_V, m_r_vertex);
	DDX_CBIndex(pDX, IDC_COMBO_SHAPE, m_shape);
	int remove_cantilever = 0, remove_independent = 0;
	DDX_Check(pDX, IDC_CHECK_REMOVE_CANTILEVER, remove_cantilever);
	DDX_Check(pDX, IDC_CHECK_REMOVE_INDEPENDENT, remove_independent);
	m_remove_opt = (remove_cantilever << 1) + remove_independent;

	int isDirNeeded = -1;
	DDX_Check(pDX, IDC_CHECK_PRJDIR, isDirNeeded);
	if (isDirNeeded)
	{
		// 位置
		DDX_Text(pDX, IDC_EDIT_CELL_X, m_trans_seed.pos[0]);
		DDV_MinMaxDouble(pDX, m_trans_seed.pos[0], -PLAT_RANGE, PLAT_RANGE);
		DDX_Text(pDX, IDC_EDIT_CELL_Y, m_trans_seed.pos[1]);
		DDV_MinMaxDouble(pDX, m_trans_seed.pos[1], -PLAT_RANGE, PLAT_RANGE);
		DDX_Text(pDX, IDC_EDIT_CELL_Z, m_trans_seed.pos[2]);
		DDV_MinMaxDouble(pDX, m_trans_seed.pos[2], -PLAT_RANGE, PLAT_RANGE);

		// 旋转角
		DDX_Text(pDX, IDC_EDIT_CELL_RX, m_trans_seed.rot[0]);
		DDV_MinMaxDouble(pDX, m_trans_seed.rot[0], -180, 180);
		DDX_Text(pDX, IDC_EDIT_CELL_RY, m_trans_seed.rot[1]);
		DDV_MinMaxDouble(pDX, m_trans_seed.rot[1], -180, 180);
		DDX_Text(pDX, IDC_EDIT_CELL_RZ, m_trans_seed.rot[2]);
		DDV_MinMaxDouble(pDX, m_trans_seed.rot[2], -180, 180);
	}
}

BEGIN_MESSAGE_MAP(CBFillDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CBFillDlg::OnClickedApply)
END_MESSAGE_MAP()


// CBFillDlg 消息处理程序

// 应用
void CBFillDlg::OnClickedApply()
{
	UpdateData(TRUE);

	CCADDoc* pDoc = cadGetDoc();
	if (pDoc == NULL)
		return;
	int n = pDoc->GetNumOfSTL();
	if (n < 1)
	{
		AfxMessageBox(_T("无STL模型!"));
		return;
	}
	if (pDoc->IsPropertySetAllStl())
	{
		pDoc->CBFill(m_cell_type, m_size.l, m_size.w, m_size.h, m_shape, m_r_edge, m_r_vertex, m_remove_opt, &m_prg);
	}

	return;
}