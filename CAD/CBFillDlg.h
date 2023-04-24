#pragma once
#include "afxeditbrowsectrl.h"

// CBFillDlg 对话框

class CBFillDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBFillDlg)

public:
	CBFillDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBFillDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBFILLDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	PRG m_prg;

	int m_cell_type;		// 晶格构型
	double m_r_edge;	// 杆半径
	double m_r_vertex;	// 节点半径
	int m_shape;		// 截面形状
	struct m_cell_size	// 晶格单元尺寸
	{
		double l;		// 长
		double w;		// 宽
		double h;		// 高
	}m_size;

	struct m_cell_seed	// 晶格单元种子
	{
		PNT3D pos;		// 位置
		double rot[3];	// 旋转角
	}m_trans_seed;

	/* 移除选项
	m_remove_opt & 2 非零, 删除悬臂杆
	m_remove_opt & 1 非零, 删除断开的晶格单元
	*/
	int m_remove_opt;

	afx_msg void OnClickedApply();

	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
