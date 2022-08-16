// CPropertyDefine.cpp: 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "CPropertyDefineDlg.h"
#include "afxdialogex.h"


// CPropertyDefine 对话框

IMPLEMENT_DYNAMIC(CPropertyDefineDlg, CDialog)

CPropertyDefineDlg::CPropertyDefineDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PROPERTY_DEFINE, pParent)
{

}

CPropertyDefineDlg::~CPropertyDefineDlg()
{
}

void CPropertyDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropertyDefineDlg, CDialog)
END_MESSAGE_MAP()


// CPropertyDefine 消息处理程序
