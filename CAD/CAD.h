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

// CAD.h : CAD 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CCADApp:
// 有关此类的实现，请参阅 CAD.cpp
//


class CCADApp : public CWinAppEx
{
public:
	CCADApp();

	BOOL g_bRestart = FALSE;
	void SetLang(BOOL bEnglish);
	void SetStart(BOOL bStart);
	void SetFirst(BOOL bFirst);
	BOOL GetLang(void);
	BOOL GetStart(void);
	BOOL GetFirst(void);
	HINSTANCE m_hLangDLL;
	BOOL g_bEnglish = FALSE;
	BOOL g_bFirst = FALSE;
	BOOL g_bStart = FALSE;
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnChinese();
	//afx_msg void OnEnglish();
	
	afx_msg void OnBnClickedChinese();
	afx_msg void OnBnClickedEnglish();
	afx_msg void OnSwLan();
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);
};

extern CCADApp theApp;