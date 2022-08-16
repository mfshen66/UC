// 2017/2/19 nt
// z-buffer for slicing algorithm of STL model
#ifndef _ZB_h_
#define _ZB_h_

////////////////////////////////////////////////////////////////
#define IPOINT_VERTEX 0x0001
#define IPOINT_EDGE 0x0002
#define IPOINT_FACE 0x0004
#define IPOINT_ON1 0x0008 // -z
#define IPOINT_IN1 0x0010 // -z 16
#define IPOINT_OUT1 0x0020 // -z 32
#define IPOINT_ON2 0x0040 // z+ 64
#define IPOINT_IN2 0x0080 // z+ 128
#define IPOINT_OUT2 0x0100 // z+ 256

typedef struct _ipoint IPOINT ; // intersection point
struct _ipoint
{
	IPOINT* next ;
	STLTRI* tri ;
	double z ;
	int attrib ; // ��ʾ������Ƭ�ڲ����ߡ�������, �Լ�ɨ�����������������
} ;

void ipointInit(IPOINT* point) ;
void ipointInit2(IPOINT* point, double z, int attrib, STLTRI* tri) ;
int ipointGetAttribID(IPOINT* point, int np) ;
void ipointSetAttribID(IPOINT* point, int np, int attribID) ;
int ipointClassifyVertex(int n, IPOINT** array, double tol, double ang) ;
int ipointClassifyEdge(IPOINT* point1, IPOINT* point2, double tol, double ang) ;
int ipointClassifyFace(IPOINT* point, double tol, double ang) ;
int ipointClassify(int n, IPOINT** array, double tol, double ang) ;
//--------------------------------------------------------------
int daMake(DA* da, STLTRI* tri1, STLTRI* tri2, double tol, double ang) ; // nt add 2017/3/18
int paMake(PA* pa, int n, IPOINT** array, double tol, double ang) ;
//--------------------------------------------------------------
#define BLK_MAX_IPOINT 100000

typedef struct _blk BLK ;
struct _blk
{
	int max ; // BLK_MAX_IPOINT
	int n ; // 0<=n<=max
	IPOINT* points ; // ����, 0...max-1
} ;

int blkInit(BLK* blk, int max) ;
void blkEmpty(BLK* blk) ;
//--------------------------------------------------------------
typedef struct _sline SLINE ; // scan_line, direction=(0,0,1)
struct _sline
{
	int flags ; // 0=normal,1=classify error
	PNT3D p ; // p[2] = 0.
	int n ; // IPOINT����
	IPOINT* points ; // ����
} ;

void slineInit(SLINE* line, PNT3D p) ;
void slineInit2(SLINE* line, double x, double y) ;
void slineAdd(SLINE* line, IPOINT* point) ; // ������������һ������
int slineArray(SLINE* line, int nSize, IPOINT** array) ; // ��ɨ�����е�IPOINT����������
int slineMerge(SLINE* line, int n, IPOINT** array) ;
int slineSort(SLINE* line, int nSize, IPOINT** array) ; // �����н���������򣬰�zֵ��С����Ĵ���
int slineClassify(SLINE* line, double tol, double ang, int nSize, IPOINT** array) ; // ��������õĽ�����з��࣬���ж��ڽ��㴦����������
int slineChkAttrib(IPOINT* point1, IPOINT* point2) ;
double slineCalLen(SLINE* line) ;
double slineCalLen2(SLINE* line) ;
int slineChkPt(SLINE* line, double z, double tol) ; // nt add 2017/3/21
//--------------------------------------------------------------
typedef unsigned char uchar ;
#define ZB_MAX_BLK 100000

typedef struct _zb ZB ; // z-bufferģ��
struct _zb
{
	double xmin ; // x������С
	double xmax ; // x�������
	double ymin ; // y������С
	double ymax ; // y�������
	double zmin ; // z������С
	double zmax ; // z�������
	double w ; // ��࣬x��y�����ϵ�ɨ���ߵļ��
	double h ; // ���, z�����ϵĲ�ĺ��
	double tol ;
	double ang ;

	int max ; // ZB_MAX_BLK
	int nBlk ;
	BLK* blks ; // ����

	int nx ; // ��nx��
	int ny ; // ��ny��
	SLINE** lines ;
	int maxIP ;

	// error records
	int nDege ;
	int nOpen ;
	int nMem ;
	int nOther ;

	// pixel buffer
	int cut ;
	double z ;
	uchar* pixels ;
} ;

ZB* zbCreate(BOX3D* box, double w, double h, double tol, double ang) ;
ZB* zbCreate2(double xmin, double ymin, int nx, int ny, double w, double tol, double ang) ; // nt add 2021/6/4
void zbFree(ZB* zb) ;
double zbGetSpacing(ZB* zb) ;
double zbGetThickness(ZB* zb) ;
int zbGetNumOfSLineX(ZB* zb) ;
int zbGetNumOfSLineY(ZB* zb) ;
SLINE* zbGetSLine(ZB* zb, int i, int j) ; // ȡ��i�е�j�е�ɨ����
void zbResI(ZB* zb, int& i) ;
void zbResJ(ZB* zb, int& j) ;
IPOINT* zbAllocIPoint(ZB* zb) ;
int zbAdd(ZB* zb, int i, int j, double z, int attrib, STLTRI* tri) ; // ���ӽ���
int zbOnTri(ZB* zb, int i, int j) ;
int zbSliceSTLTriYZ(ZB* zb, double xmin, double xmax, STLTRI* tri) ;
int zbSliceSTLTriXZ(ZB* zb, double ymin, double ymax, STLTRI* tri) ;
int zbSliceSTLTri(ZB* zb, STLTRI* tri) ;
int zbSlice(ZB* zb, STL* stl, PRG* pPrg) ; // STL/scan_line intersection
int zbDraw(ZB* zb, void* pVI) ; // draw lines
int zbDraw2(ZB* zb, void* pVI) ; // draw points
int zbDrawZ(ZB* zb, void* pVI) ; // draw points on z-plane
int zbCut(ZB* zb, double z) ; // nt add 2017/3/21
int zbGray(ZB* zb, int grays[4]) ; // nt add 2017/5/16
uchar zbGetPixel(ZB* zb, int i, int j) ;
////////////////////////////////////////////////////////////////

#endif