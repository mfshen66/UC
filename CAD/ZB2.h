// 2017/5/27 nt
// z-buffer for SS of STL model
#ifndef _ZB2_h_
#define _ZB2_h_

////////////////////////////////////////////////////////////////
typedef struct _isegm ISEGM ; // intersection segment
struct _isegm
{
	ISEGM* next ;
	UINT64 facetId ;
	double z1 ; // min
	double z2 ; // max
} ;

void isegmInit(ISEGM* segm) ;
void isegmInit2(ISEGM* segm, double z, UINT64 facetId) ;
int isegmMerge(ISEGM* segm, double z) ;
//--------------------------------------------------------------
typedef struct _sline2 SLINE2 ; // scan_line, direction=(0,0,1)
struct _sline2
{
	PNT3D p ; // p[2] = 0.
	int n ; // ISEGM总数
	ISEGM* segms ; // 链表
} ;

void sline2Init(SLINE2* line, PNT3D p) ;
void sline2Init2(SLINE2* line, double x, double y) ;
void sline2FreeSegmAll(SLINE2* line) ;
int sline2Add(SLINE2* line, double z, UINT64 facetId) ;
int sline2ChkPt(SLINE2* line, double z, double tol) ;
//--------------------------------------------------------------
typedef struct _zb2 ZB2 ; // z-buffer模型
struct _zb2
{
	double xmin ; // x坐标最小
	double xmax ; // x坐标最大
	double ymin ; // y坐标最小
	double ymax ; // y坐标最大
	double zmin ; // z坐标最小
	double zmax ; // z坐标最大
	double w ; // 间距，x和y方向上的扫描线的间距
	double h ; // 层厚
	double tol ;
	double ang ;

	UINT64 facetId ;
	int nx ; // 共nx行
	int ny ; // 共ny列
	SLINE2** lines ;

	// pixel buffer
	int cut ;
	double z ;
	uchar* pixels ;
} ;

ZB2* zb2Create(BOX3D* box, double w, double h, double tol, double ang) ;
void zb2Free(ZB2* zb) ;
double zb2GetSpacing(ZB2* zb) ;
double zb2GetThickness(ZB2* zb) ;
int zb2GetNumOfSLineX(ZB2* zb) ;
int zb2GetNumOfSLineY(ZB2* zb) ;
SLINE2* zb2GetSLine(ZB2* zb, int i, int j) ; // 取第i行第j列的扫描线
void zb2ResI(ZB2* zb, int& i) ;
void zb2ResJ(ZB2* zb, int& j) ;
int zb2Add(ZB2* zb, int i, int j, double z) ; // 增加交点
int zb2SliceTriYZ(ZB2* zb, double xmin, double xmax, PNT3D A, PNT3D B, PNT3D C, VEC3D normal) ;
int zb2SliceTriXZ(ZB2* zb, double ymin, double ymax, PNT3D A, PNT3D B, PNT3D C, VEC3D normal) ;
int zb2SliceTri(ZB2* zb, PNT3D A, PNT3D B, PNT3D C) ;
int zb2SliceFacet(ZB2* zb, FACET* facet) ; 
int zb2SliceSS(ZB2* zb, SS* ss) ; 
int zb2Slice(ZB2* zb, STL* stl) ; // STL的ss/scan_line intersection
int zb2SliceCB(ZB2* zb, CB* cb, PRG* pPrg) ; // nt add 2021/6/5
int zb2Draw(ZB2* zb, void* pVI) ; // draw lines
int zb2Draw2(ZB2* zb, void* pVI) ; // draw points
int zb2DrawZ(ZB2* zb, void* pVI) ; // draw points on z-plane
int zb2Cut(ZB2* zb, double z) ;
uchar zb2GetPixel(ZB2* zb, int i, int j) ;
////////////////////////////////////////////////////////////////

#endif