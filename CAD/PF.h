#pragma once

////////////////////////////////////////////////////////////////
typedef struct _layer LAYER ;
struct _layer
{
	LAYER* next ;
	int iLayer ;
	double h ; // 层厚
	int li1 ; // 光强1
	double lt1 ; // 光照时间1
	int li2 ; // 光强2
	double lt2 ; // 光照时间2
	double t1 ; // 预留参数
	double t2 ; // 预留参数
	int size ;
	uchar* data ; // 一个像素4个bits
} ;

LAYER* layerCreate(int iLayer, double h, int li1, double lt1, int li2, double lt2, int nx, int ny) ;
LAYER* layerCreate2(int n) ;
void layerFree(LAYER* layer) ;
uchar layerGet(LAYER* layer, int index) ; // index = j*nx+i, 从下至上，从左到右
void layerSet(LAYER* layer, int index, uchar c) ;
void layerLoad(LAYER* layer, CArchive& ar) ;
void layerSave(LAYER* layer, CArchive& ar) ;
//--------------------------------------------------------------
typedef struct _pf PF ; // printer file
struct _pf
{
	PARAMETER parameter ; // 输入参数
	// 
	double vol ; // 体积
	double time ; // 预估打印时间
	COLORREF color ;
	double h ; // 层高
	double lt1 ; // 曝光时间, s
	int li1 ; // 曝光强度, 0-100%
	double lt2 ; // 曝光时间, s
	int li2 ; // 曝光强度, 0-100%
	int constH ; // 1=等层厚,0=变层厚
	int nx ;
	int ny ;
	int nLayer ; // 总层数
	LAYER* layers ;
	LAYER* last ; // 最后一层
} ;

PF* pfCreate(PARAMETER* parameter, double vol, double time, COLORREF color, double h, double lt1, int li1, double lt2, int li2, int constH, int nx, int ny) ;
PF* pfCreate2() ;
void pfFree(PF* pf) ;
int pfAddLayer(PF* pf, uchar* data) ;
int pfAddLayer2(PF* pf, LAYER* layer) ;
int pfLoad(PF* pf, CString& fileName) ;
int pfSave(PF* pf, CString& fileName) ;
int pfLoad2(PF* pf, CString& fileName) ;
int pfSave2(PF* pf, CString& fileName, int n) ;
////////////////////////////////////////////////////////////////
