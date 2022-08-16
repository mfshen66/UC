#pragma once

////////////////////////////////////////////////////////////////
typedef struct _layer LAYER ;
struct _layer
{
	LAYER* next ;
	int iLayer ;
	double h ; // ���
	int li1 ; // ��ǿ1
	double lt1 ; // ����ʱ��1
	int li2 ; // ��ǿ2
	double lt2 ; // ����ʱ��2
	double t1 ; // Ԥ������
	double t2 ; // Ԥ������
	int size ;
	uchar* data ; // һ������4��bits
} ;

LAYER* layerCreate(int iLayer, double h, int li1, double lt1, int li2, double lt2, int nx, int ny) ;
LAYER* layerCreate2(int n) ;
void layerFree(LAYER* layer) ;
uchar layerGet(LAYER* layer, int index) ; // index = j*nx+i, �������ϣ�������
void layerSet(LAYER* layer, int index, uchar c) ;
void layerLoad(LAYER* layer, CArchive& ar) ;
void layerSave(LAYER* layer, CArchive& ar) ;
//--------------------------------------------------------------
typedef struct _pf PF ; // printer file
struct _pf
{
	PARAMETER parameter ; // �������
	// 
	double vol ; // ���
	double time ; // Ԥ����ӡʱ��
	COLORREF color ;
	double h ; // ���
	double lt1 ; // �ع�ʱ��, s
	int li1 ; // �ع�ǿ��, 0-100%
	double lt2 ; // �ع�ʱ��, s
	int li2 ; // �ع�ǿ��, 0-100%
	int constH ; // 1=�Ȳ��,0=����
	int nx ;
	int ny ;
	int nLayer ; // �ܲ���
	LAYER* layers ;
	LAYER* last ; // ���һ��
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
