// CHotKey.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "CHotKey.h"
#include "afxdialogex.h"


// CHotKey 对话框

IMPLEMENT_DYNAMIC(CHotKey, CDialogEx)

CHotKey::CHotKey(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HOTKEY, pParent)
{

}

CHotKey::~CHotKey()
{
}

void CHotKey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHotKey, CDialogEx)
END_MESSAGE_MAP()


// CHotKey 消息处理程序
