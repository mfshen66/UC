#pragma once

///////////////////////////////////////////////////////////////
typedef struct _cell CELL; // unit cell
struct _cell
{
	CELL* next;
	unsigned char type; // 8+12+6=26
	int k;
	PNT3D begin[4];		//杆的起点
	PNT3D end[4];		//杆的终点
	bool flags[4];		//是否有杆
	bool flagC;			//晶胞是否包含中心球
	int iBlk ; // nt add 2021/12/26
	int iCell ; // nt add 2021/12/26
};

CELL* cellCreate();
void cellFree(CELL* cell);
//--------------------------------------------------------------
void fileGetName(CString& pathName, CString& fileName) ;
void fileGetTitle(CString& pathName, CString& title) ;
double roundOff(double d, int nAfterZeroPoint) ;
//--------------------------------------------------------------
void drawCell(double min[3], double max[3]);
void drawCube(double min[3], double max[3]) ;
void drawCube2(double min[3], double max[3]) ;
void drawCube3(double min[3], double max[3]) ;
void drawSphere(PNT3D center, double r, int nu, int nv) ;
void drawCylinder(PNT3D center, VEC3D Z, double r, double h, int nu, int nv) ;
void drawCone(PNT3D center, VEC3D Z, double r, double h, int nu, int nv) ;
void drawTorus(PNT3D center, VEC3D Z, double r1, double r2, int nu, int nv) ;
void drawArc(PNT3D O, VEC3D X, VEC3D Y, double r, double a, int n) ;
void drawArc2(PNT3D O, VEC3D X, VEC3D Y, double r, double a, int n) ;
void drawUmbrella(PNT3D O, VEC3D X, VEC3D Y, VEC3D Z, double r, double h, int n) ;
void drawArrowWithDisk(PNT3D O, VEC3D Z, double r1, double r2, double h, int n) ;
void drawMove(PNT3D O, VEC3D X, VEC3D Y, double w, double h, COLORREF c) ;
void drawZoom(PNT3D O, VEC3D X, VEC3D Y, double w, double h, COLORREF c) ;
void drawZoom2(PNT3D O, VEC3D X, VEC3D Y, double w, COLORREF c) ;
//-------------------------------------------------------------
FACET* facetCreate2(SURFACE* surface, BOX2D* boxuv, int nu, int nv) ;
FACET* facetCreate3(int nu) ;
FACET* facetCreate4(int nu) ;
FACET* facetCylinder(PNT3D center, VEC3D Z, double r, double h, int nu, int nv) ;
FACET* facetCylinder2(PNT3D p1, PNT3D p2, double r, int nu) ;
FACET* facetCylinder3(PNT3D p1, PNT3D p2, double r, int nu) ;
FACET* facetCone(PNT3D center, VEC3D Z, double r, double h, int nu, int nv) ;
FACET* facetCone2(PNT3D p1, PNT3D p2, double r1, double r2, int nu) ;
FACET* facetCone3(PNT3D p1, PNT3D p2, double r1, double r2, int nu) ;
FACET* facetSphere(PNT3D center, double r, int nu, int nv) ;
FACET* facetTorus(PNT3D center, VEC3D Z, double r1, double r2, int nu, int nv) ;
int facetsBoundCell(PNT3D center, CELL* cell, double r1, double r2, int nu, FACET* facets[5]); // smf add 2021/8/20
int facetsCell(PNT3D center, double w, double r1, double r2, int nu, FACET* facets[5]) ; // nt add 2021/6/6
void facetMove(FACET* facet, VEC3D v) ;
void facetRotate(FACET* facet, PNT3D pivot, VEC3D axis, double a) ;
void facetsCopyMove(int nFacet, FACET** facets1, VEC3D v, FACET** facets2) ;
void facetDraw0(FACET* facet) ;
void facetDraw1(FACET* facet) ;
void facetDraw2(FACET* facet) ;
double facetGetMinZ(FACET* facet) ;
FACET* facetCopy(FACET* facet) ;
int facetSerialize(FACET* facet, CArchive& ar) ;
//-------------------------------------------------------------
int mathPrjPntPlnEx(PNT3D p, VEC3D normal, PNT2D decomp) ;
int mathDecVecXY(PNT3D p, PNT3D q, VEC3D normal, VEC2D decomp) ;
//--------------------------------------------------------------
// nt add 2009/10/21 : 从字符串中取数值
int strToNums(TCHAR* str, int& i) ; // nt add 2015/9/13
int strToNums(TCHAR* str, double& d) ; // nt add 2015/9/13
int strToNums(TCHAR* str, int& i1, int& i2) ; // nt add 2015/9/13
int strToNums(TCHAR* str, double& d1, double& d2) ; // nt add 2015/9/13
int strToNums(TCHAR* str, int& i, double& d) ; // nt add 2015/9/13
int strToNums(TCHAR* str, double& d, int& i) ; // nt add 2015/9/13
int strToNums(TCHAR* str, double& d1, double& d2, double& d3) ; // nt add 2015/9/13
int strToNums(TCHAR* str, double& d1, double& d2, int& i) ; // nt add 2015/9/13
int strToNums(CString& str, int& i) ;
int strToNums(CString& str, double& d) ;
int strToNums(CString& str, int& i1, int& i2) ;
int strToNums(CString& str, double& d1, double& d2) ;
int strToNums(CString& str, int& i, double& d) ;
int strToNums(CString& str, double& d, int& i) ;
int strToNums(CString& str, double& d1, double& d2, double& d3) ;
int strToNums(CString& str, double& d1, double& d2, int& i) ;
//--------------------------------------------------------------
typedef struct _trans TRANS ;
struct _trans
{
	int type ; // 0=平移,1=旋转,2=比例缩放
	double a ; // 旋转角或比例
	PNT3D p ; // 旋转中心或比例缩放中心
	VEC3D v ; // 平移量或旋转轴
} ;
//--------------------------------------------------------------
void efpGet(CString& efp) ;
//--------------------------------------------------------------
void cstrWrite(CString& str, CArchive& ar) ;
void cstrRead(CString& str, CArchive& ar) ;
//--------------------------------------------------------------
int comboLoad(CString& filePath, CComboBox& combo) ;
//--------------------------------------------------------------
typedef struct _prg PRG ; // nt add 2017/9/9
struct _prg
{
	double n ;
	double* ws ; // ws[0]+ws[1]+...+ws[n-1] = 1. ;
	int i ; // 当前正在更新的index
	double c ; // 0. to 1.
	int finished ; // 1=finished
} ;

PRG* prgCreate() ;
void prgFree(PRG* pPrg) ;
void prgReleaseBuf(PRG* pPrg) ;
void prgInit(PRG* pPrg, int n) ;
int prgGetPercent(PRG* pPrg) ;
void prgFinish(PRG* pPrg) ;
//--------------------------------------------------------------
int triInt(PNT3D V0, PNT3D V1, PNT3D V2, PNT3D U0, PNT3D U1, PNT3D U2) ;
//--------------------------------------------------------------
// smf add 2023/02/21
// 两共面的线段求交
int mathSegmIntSegmInFace(PNT3D iBegin1, PNT3D iEnd1, PNT3D iBegin2, PNT3D iEnd2, double iTol, PNT3D oIntPoint);

double mathDet22(double a11, double a12, double a21, double a22); // |A|
///////////////////////////////////////////////////////////////