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
	int n ; // ISEGM����
	ISEGM* segms ; // ����
} ;

void sline2Init(SLINE2* line, PNT3D p) ;
void sline2Init2(SLINE2* line, double x, double y) ;
void sline2FreeSegmAll(SLINE2* line) ;
int sline2Add(SLINE2* line, double z, UINT64 facetId) ;
int sline2ChkPt(SLINE2* line, double z, double tol) ;
//--------------------------------------------------------------
typedef struct _zb2 ZB2 ; // z-bufferģ��
struct _zb2
{
	double xmin ; // x������С
	double xmax ; // x�������
	double ymin ; // y������С
	double ymax ; // y�������
	double zmin ; // z������С
	double zmax ; // z�������
	double w ; // ��࣬x��y�����ϵ�ɨ���ߵļ��
	double h ; // ���
	double tol ;
	double ang ;

	UINT64 facetId ;
	int nx ; // ��nx��
	int ny ; // ��ny��
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
SLINE2* zb2GetSLine(ZB2* zb, int i, int j) ; // ȡ��i�е�j�е�ɨ����
void zb2ResI(ZB2* zb, int& i) ;
void zb2ResJ(ZB2* zb, int& j) ;
int zb2Add(ZB2* zb, int i, int j, double z) ; // ���ӽ���
int zb2SliceTriYZ(ZB2* zb, double xmin, double xmax, PNT3D A, PNT3D B, PNT3D C, VEC3D normal) ;
int zb2SliceTriXZ(ZB2* zb, double ymin, double ymax, PNT3D A, PNT3D B, PNT3D C, VEC3D normal) ;
int zb2SliceTri(ZB2* zb, PNT3D A, PNT3D B, PNT3D C) ;
int zb2SliceFacet(ZB2* zb, FACET* facet) ; 
int zb2SliceSS(ZB2* zb, SS* ss) ; 
int zb2Slice(ZB2* zb, STL* stl) ; // STL��ss/scan_line intersection
int zb2SliceCB(ZB2* zb, CB* cb, PRG* pPrg) ; // nt add 2021/6/5
int zb2Draw(ZB2* zb, void* pVI) ; // draw lines
int zb2Draw2(ZB2* zb, void* pVI) ; // draw points
int zb2DrawZ(ZB2* zb, void* pVI) ; // draw points on z-plane
int zb2Cut(ZB2* zb, double z) ;
uchar zb2GetPixel(ZB2* zb, int i, int j) ;
////////////////////////////////////////////////////////////////

#endif