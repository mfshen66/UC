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
//	int k ;				//z�����λ��
//	PNT3D begin[4];		//�˵����
//	PNT3D end[4];		//�˵��յ�
//	bool flags[4];		//�Ƿ��и�
//	int flagC;			//�����Ƿ����������
//} ;
//
//CELL* cellCreate();
//void cellFree(CELL* cell);
//--------------------------------------------------------------

// smf add 2021/7/21
#define MAXFACENUM 50
typedef struct _convexHull HULL;// ͹������
struct _convexHull
{
	PNT3D points[MAXFACENUM];		// ͹������Ķ���
	int pn;					// ����ĸ���
	FACE tri[MAXFACENUM];	// ͹���������
	int fn;					// ��ĸ���
	double tol;
	int pBelongf[MAXFACENUM][MAXFACENUM];	//������������ϵ
};

HULL* hullCreate();
int hullChkPnt(HULL *hull, PNT3D p, double tol); // �ж�һ�����Ƿ��ڵ�ǰ�������ڣ�1-�ڶ������ڣ�0-�ڶ�������
int hullCutLine(HULL *hull, PNT3D p1, PNT3D p2, PNT3D IP1, PNT3D IP2);//����ֵ��-1-�߶ι��̣�0-�߶����ڲ�;1-һ�����㣻2-��������
int hullMake(HULL *hull);
void hullUpdate(HULL *hull, int fi, int pi, int chkf);
void hullDeal(HULL *hull, int pi, int fpa, int fpb);
void hullFree(HULL *hull);
void hullDivision(HULL *hull, HULL *hull1, HULL *hull2, PNT3D p1, PNT3D p2, short type); // �������ʷ�
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
	int n ; // RSEGM����
	CSEGM* segms ; // ����
	int m ;
	CELL* cells ; // λ�ڱ����CELL��ɵ�����, ����CSEGM�ͷ�!!!!!!
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
	CELL* cells ; // ����, 0...max-1
} ;

int cellblkInit(CELLBLK* blk, int max) ;
void cellblkEmpty(CELLBLK* blk) ;
//--------------------------------------------------------------
#define CB_MAX_BLK 100000

typedef struct _cb CB ; // cell-bufferģ��
struct _cb
{
	double xmin ; // x������С
	double xmax ; // x�������
	double ymin ; // y������С
	double ymax ; // y�������
	double zmin ; // z������С
	double zmax ; // z�������
	double w ; // cell�ı߳�
	double tol ;
	double ang ;

	int max ; // CB_MAX_BLK
	int nBlk ;
	CELLBLK* blks ; // ����

	// cell[i][j][k]�����½�����=(xmin+i*w,ymin+j*w,zmin+k*w)
	int nx ; // CELL��nx�У�xmax-xmin=w*nx
	int ny ; // CELL��ny�У�ymax-ymin=w*ny
	int nz ; // CELL��nz�ߣ�zmax-zmin=w*nz
	CLINE** lines ; // ����xy������ͬ��CELL���һ��SLINE3
	double r1 ; // �˵İ뾶
	double r2 ; // ������İ뾶
} ;

CB* cbCreate(BOX3D* box, double w, double r1, double r2, double tol, double ang) ;
void cbFree(CB* cb) ;
double cbGetSpacing(CB* cb) ;
int cbGetNumOfSLineX(CB* cb) ;
int cbGetNumOfSLineY(CB* cb) ;
CLINE* cbGetCLine(CB* cb, int i, int j) ; // ȡ��i�е�j�е�ɨ����
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

// 2022/08/10 smf add: ���Խӿ�
int getCellPosition(int &i, int &j, int &k);// �������cell�Ŀռ�λ��

// �ҵ����Ե�cell��ָ��
CELL* findTestCell(CB *cb, int i, int j, int k);
CELL* findTestCell(CB *cb, double x, double y, double z);
CELL* findTestCell(CB *cb, int iBlk, int iCell);

void drawTestCube(CB *cb, int i, int j, int k);// ���Ʋ���cell�İ�Χ��
void drawTestCell(CELL *iCell, CB* cb); // ���Ʋ���cell

////////////////////////////////////////////////////////////////

#endif