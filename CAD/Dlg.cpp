#include "stdafx.h"

///////////////////////////////////////////////////////////////
CDlg::CDlg::CDlg()
{
}

CDlg::CDlg(UINT nIDTemplate, CWnd *pParent)
	: CDialogEx(nIDTemplate, pParent)
{
}

CDlg::~CDlg()
{
}

int CDlg::OnLBD(UINT nFlags, double p[3], double v[3], void* pVI)
{
	return 1 ;
}

int CDlg::OnMM(UINT nFlags, double p[3], double v[3], void* pVI)
{
	return 1 ;
}

int CDlg::OnRBD(UINT nFlags, double p[3], double v[3], void* pVI)
{
	return 1 ;
}

int CDlg::OnLBU(UINT nFlags, double p[3], double v[3], void* pVI)
{
	return 1 ;
}

int CDlg::OnReturn()
{
	return 1 ;
}

void CDlg::Notify(int msg)
{
}

void CDlg::Draw(void* pVI)
{
}
///////////////////////////////////////////////////////////////