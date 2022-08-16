// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// CAD.h : CAD Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CCADApp:
// �йش����ʵ�֣������ CAD.cpp
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
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
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