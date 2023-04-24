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

// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "gl\gl.h"
#include "gl\glu.h"
#include "VirtualKeys.h"
typedef unsigned _int64 UINT64 ; // nt add 2021/6/5
//--------------------------------------------------------------
#include "VI_API.h"
#include "GP_API.h"
#include "RBall.h"
#include "STL.h"
#include "Tools.h"
#include "Set.h"
#include "SetInt.h"
#include "Dlg.h"
#include "ZB.h"
#include "SS.h"
#include "Parameter.h"
#include "PF.h"
#include "CB.h" // nt add 2021/6/6
#include "ZB2.h" // nt add 2017/5/29
#include "PArray.h" // nt add 2017/5/31
#include "Record.h" // nt add 2017/9/2
#include "GridModel.h" // smf add 2023/04/24
#include <afxwin.h>
#include <afxwin.h>
#include <afxcontrolbars.h>
//--------------------------------------------------------------
void cadPromptUpdate() ;
void cadPromptAttach(TCHAR c) ;
void cadPromptAttach(CString& str) ;
void cadPromptGet(CString& str) ;
void cadPromptClear() ;
void cadPromptStr(CString& str) ;
void cadPromptStr(TCHAR* text) ;
void cadPromptPt(double p[3]) ;
void cadPromptPt2D(double p[2]) ;



