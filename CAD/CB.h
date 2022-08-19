// 2021/6/1 nt
// z-buffer of unit cells for STL model
#ifndef _ZB3_h_
#define _ZB3_h_
#include <algorithm>
#include <vector>

#include "Triangle.h"
#include "vector.h"
using namespace std;

////////////////////////////////////////////////////////////////
//typedef struct _cell CELL ; // unit cell
//struct _cell
//{
//	CELL* next ;
//	int type ;			// 8+12+6=26
//	int k ;				//z方向的位置
//	PNT3D begin[4];		//杆的起点
//	PNT3D end[4];		//杆的终点
//	bool flags[4];		//是否有杆
//	int flagC;			//晶胞是否包含中心球
//} ;
//
//CELL* cellCreate();
//void cellFree(CELL* cell);
//--------------------------------------------------------------

// smf add 2021/7/21
#define MAXFACENUM 50
typedef struct _convexHull HULL;// 凸多面体
struct _convexHull
{
	PNT3D points[MAXFACENUM];		// 凸多面体的顶点
	int pn;					// 顶点的个数
	FACE tri[MAXFACENUM];	// 凸多面体的面
	int fn;					// 面的个数
	double tol;
	int pBelongf[MAXFACENUM][MAXFACENUM];	//点和面的所属关系
};

HULL* hullCreate();
int hullChkPnt(HULL *hull, PNT3D p, double tol); // 判断一个点是否在当前多面体内：1-在多面体内，0-在多面体外
int hullCutLine(HULL *hull, PNT3D p1, PNT3D p2, PNT3D IP1, PNT3D IP2);//返回值：-1-线段过短；0-线段在内部;1-一个交点；2-两个交点
int hullMake(HULL *hull);
void hullUpdate(HULL *hull, int fi, int pi, int chkf);
void hullDeal(HULL *hull, int pi, int fpa, int fpb);
void hullFree(HULL *hull);
void hullDivision(HULL *hull, HULL *hull1, HULL *hull2, PNT3D p1, PNT3D p2, short type); // 多面体剖分
//--------------------------------------------------------------
typedef struct _csegm CSEGM ; // interior cell segment
struct _csegm
{
	CSEGM* next ;
	int k1 ; // min
	int k2 ; // max
} ;

CSEGM* csegmCreate(int k1, int k2) ;
void csegmFree(CSEGM* segm) ;
//--------------------------------------------------------------
typedef struct _cline CLINE ; // scan_line, direction=(0,0,1)
struct _cline
{
	PNT3D p ; // p[2] = 0.
	int n ; // RSEGM总数
	CSEGM* segms ; // 链表
	int m ;
	CELL* cells ; // 位于表面的CELL组成的链表, 不用CSEGM释放!!!!!!
} ;

void clineInit(CLINE* line, double x, double y) ;
void clineFreeSegmAll(CLINE* line) ;
void clineAddSegm(CLINE* line, CSEGM* segm) ;
void clineAddCell(CLINE* line, CELL* cell) ;
int clineChkPt(CLINE* line, double z, double tol) ;
//--------------------------------------------------------------
#define BLK_MAX_CELL 100000

typedef struct _cellblk CELLBLK ;
struct _cellblk
{
	int max ; // BLK_MAX_IPOINT
	int n ; // 0<=n<=max
	CELL* cells ; // 数组, 0...max-1
} ;

int cellblkInit(CELLBLK* blk, int max) ;
void cellblkEmpty(CELLBLK* blk) ;
//--------------------------------------------------------------
#define CB_MAX_BLK 100000

typedef struct _cb CB ; // cell-buffer模型
struct _cb
{
	double xmin ; // x坐标最小
	double xmax ; // x坐标最大
	double ymin ; // y坐标最小
	double ymax ; // y坐标最大
	double zmin ; // z坐标最小
	double zmax ; // z坐标最大
	double w ; // cell的边长
	double tol ;
	double ang ;

	int max ; // CB_MAX_BLK
	int nBlk ;
	CELLBLK* blks ; // 数组

	// cell[i][j][k]的左下角坐标=(xmin+i*w,ymin+j*w,zmin+k*w)
	int nx ; // CELL共nx行，xmax-xmin=w*nx
	int ny ; // CELL共ny列，ymax-ymin=w*ny
	int nz ; // CELL共nz高，zmax-zmin=w*nz
	CLINE** lines ; // 中心xy坐标相同的CELL组成一条SLINE3
	double r1 ; // 杆的半径
	double r2 ; // 中心球的半径
} ;

CB* cbCreate(BOX3D* box, double w, double r1, double r2, double tol, double ang) ;
void cbFree(CB* cb) ;
double cbGetSpacing(CB* cb) ;
int cbGetNumOfSLineX(CB* cb) ;
int cbGetNumOfSLineY(CB* cb) ;
CLINE* cbGetCLine(CB* cb, int i, int j) ; // 取第i行第j列的扫描线
void cbResI(CB* cb, int& i) ;
void cbResJ(CB* cb, int& j) ;
CELL* cbAllocCell(CB* cb);
bool cbDeleteLastCell(CB* cb);
void cbInitCellIndexAll(CB* cb) ; // nt add 2021/12/26
int cbFill(CB* cb, STL* stl, PRG* pPrg, STL* m_stls, double w = 2.0) ;
int cbDraw(CB* cb, void* pVI) ; // draw lines
int cbSerialize(CB* cb, CArchive& ar) ; // nt add 2021/12/26
int propertyOfClosestSTL(PNT3D p, STL* m_stls, STL * iCurrentSTL);
int isPntAboveTri(PNT3D p, STLTRI* tri);
int isPntInTri(PNT3D p, STLTRI* tri, double tol);

// 2022/08/10 smf add: 测试接口
int getCellPosition(int &i, int &j, int &k);// 输入测试cell的空间位置

// 找到测试的cell的指针
CELL* findTestCell(CB *cb, int i, int j, int k);
CELL* findTestCell(CB *cb, double x, double y, double z);
CELL* findTestCell(CB *cb, int iBlk, int iCell);

void drawTestCube(CB *cb, int i, int j, int k);// 绘制测试cell的包围盒
void drawTestCell(CELL *iCell, CB* cb); // 绘制测试cell

////////////////////////////////////////////////////////////////

#endif