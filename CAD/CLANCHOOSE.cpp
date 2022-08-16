// CLANCHOOSE.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "CLANCHOOSE.h"
#include "afxdialogex.h"


// CLANCHOOSE 对话框

IMPLEMENT_DYNAMIC(CLANCHOOSE, CDialogEx)

CLANCHOOSE::CLANCHOOSE(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_START, pParent)
{

}

CLANCHOOSE::~CLANCHOOSE()
{
}

void CLANCHOOSE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLANCHOOSE, CDialogEx)
END_MESSAGE_MAP()


// CLANCHOOSE 消息处理程序
