#pragma once

class CDlg : public CDialogEx
{
public:
	CDlg() ;
	CDlg(UINT nIDTemplate, CWnd *pParent = NULL) ;
	virtual ~CDlg() ;

	virtual int OnLBD(UINT nFlags, double p[3], double v[3], void* pVI) ;
	virtual int OnMM(UINT nFlags, double p[3], double v[3], void* pVI) ;
	virtual int OnRBD(UINT nFlags, double p[3], double v[3], void* pVI) ;
	virtual int OnLBU(UINT nFlags, double p[3], double v[3], void* pVI) ;
	virtual int OnReturn() ;
	virtual void Notify(int msg) ; // model redrawed
	virtual void Draw(void* pVI) ;
} ;