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

// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









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
#include <afxwin.h>
#include <afxwin.h>
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



