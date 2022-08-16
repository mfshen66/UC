// 2008/6/5 nt
// interfaces of loading STL file
#ifndef _STL_h_
#define _STL_h_

////////////////////////////////////////////////////////////////
#define STLTRI_DEGE			1
#define STLTRI_PARALLEL_Z	2

#define Undefined			0	// 未指定的实体类型 smf add 2022/07/18
#define FILLING_BODY		1	// 填充体
#define CAVITY				2	// 空腔
#define SKIN				3	// 蒙皮
typedef struct _stltri STLTRI ;
struct _stltri
{
	double normal[3] ;
	double vs[3][3] ;
	int attrib ; // nt add 2017/3/16
} ;

void stltriInit(STLTRI* pT) ;
void stltriGetBox2D(STLTRI* pT, BOX2D* box) ; // nt add 2017/3/26
void stltriGetBox(STLTRI* pT, BOX3D* box) ; // nt add 2017/3/26
void stltriGetPt(STLTRI* pT, double gc[3], PNT3D p) ; // nt add 2016/12/28
int stltriCorrNormal(STLTRI* pT, double tol) ; // nt add 2016/12/27
int stltriLoadBinary(STLTRI* pT, FILE* fp) ;
int stltriLoadASCII(STLTRI* pT, FILE* fp) ;
double stltriCalAngleXY(STLTRI* pT, int k) ; // nt add 2017/7/13
int stltriIsDege(STLTRI* pT, double tol) ; // nt add 2017/9/1
//--------------------------------------------------------------
typedef struct _stlblk STLBLK ;
struct _stlblk
{
	int nTri ;
	STLTRI* tris ;
	BOX3D box ; // nt add 2016/12/22
} ;

int stlblkAllocTris(STLBLK* pB, int maxNumOfTri) ;
int stlblkFreeTris(STLBLK* pB) ;
int stlblkCalBox(STLBLK* pB) ; // nt add 2016/12/22
int stlblkSerialize(STLBLK* pB, CArchive& ar) ; // nt add 2017/9/1
//--------------------------------------------------------------
// nt add 2017/7/7
typedef struct _ver VER ;
struct _ver
{
	VER* next ;
	int flags ; // 1=非向下的尖点
	PNT3D p ;
	VEC3D normal ;
	double d ; // 到原点的距离
	double a ; // 在xy平面上的投影角度之和
} ;

VER* verCreate(int flags, PNT3D p, VEC3D normal, double d, double a) ;
void verFree(VER* ver) ;
//--------------------------------------------------------------
typedef enum _stlstt STLSTT ;
enum _stlstt
{
	stlNormal = 0,
	stlPrompting = 1,
	stlSelected = 2
} ;

typedef struct _stl STL ;
struct _stl
{
	STL* next ; // nt add 2016/1/23
	int maxNumOfBlk ;
	int maxNumOfTri ;
	int nBlk ;
	STLBLK* blks ;
	BOX3D box ; // nt add 2016/12/22

	int stt ; // status, nt add 2016/12/28
	PNT3D p ; // 拾取时捕捉的点
	STLTRI tri ; // 被拾取的三角片的copy
	double vol ; // nt add 2017/5/14 mm^3

	// nt add 2017/5/26
	void* zb ;
	void* ss ;
	void* zb2 ;

	void* cb ; // nt add 2021/6/6 cell-buffer
	void* zb3 ; // nt add 2021/6/6 scan-line z-buffer for all cells

	// nt add 2017/7/9
	VER* vs ; // 尖点链表

	// nt add 2019/7/10
	CString material ;

	// smf add 2021/10/30
	short property;
} ;

STL* stlCreate(int maxNumOfBlk, int maxNumOfTri) ;
STL* stlCreate2() ; // nt add 2017/9/1
int stlFree(STL* pSTL) ;
int stlReleaseSS(STL* pSTL) ; // nt add 2017/8/22
STL* stlCopy(STL* pSTL) ;
int stlEmpty(STL* pSTL) ; // nt add 2016/12/26
int stlGetNumOfTri(STL* pSTL) ;
int stlGetTri(STL* pSTL, int i, STLTRI* pT) ;
int stlAddTri(STL* pSTL, STLTRI* pT) ; // return 0: error
void* stlGetZB(STL* pSTL) ; // nt add 2017/5/26
void* stlGetSS(STL* pSTL) ; // nt add 2017/5/26
void* stlGetZB2(STL* pSTL) ; // nt add 2017/5/26
int stlMove(STL* pSTL, VEC3D v) ; // 平移
int stlRotate(STL* pSTL, PNT3D pivot, VEC3D axis, double a) ; // 旋转
int stlRotate2(STL* pSTL, PNT3D pivot, VEC3D axis, double a) ; // 旋转
int stlScale(STL* pSTL, double scale) ; // 比例缩放
int stlScale2(STL* pSTL, PNT3D center, double scale) ; // 比例缩放
int stlCalBox(STL* pSTL) ; // nt add 2016/12/22
int stlCalBox2(STL* pSTL, BOX3D* box) ; // nt add 2017/9/30
int stlCollision(STL* pSTL1, STL* pSTL2) ; // nt add 2017/10/1 1=collision
int stlGetBox(STL* pSTL, BOX3D* box) ; // nt add 2016/12/31
int stlGetCenter(STL* pSTL, PNT3D center) ; // nt add 2016/12/31
double stlCalVol(STL* pSTL) ; // nt add 2017/3/13
STLTRI* stlPick(STL* pSTL, PNT3D pos, VEC3D dir, double r, PNT3D p, double* pz) ;
int stlDraw(STL* pSTL, void* pVI) ;
int stlDraw2(STL* pSTL, void* pVI) ; // nt add 2017/3/31
int stlLoadBinary(STL* pSTL, TCHAR* pathName) ;
int stlLoadASCII(STL* pSTL, TCHAR* pathName) ;
int stlLoad(STL* pSTL, TCHAR* pathName) ;
int stlCalAttribOfTris(STL* pSTL, double tol) ; // nt add 2017/3/16
int stlCalDownwardSharpPts(STL* pSTL, double tol, double w) ; // nt add 2017/7/7
double stlCalDistFromSharpPts(STL* pSTL, PNT2D p) ; // nt add 2017/7/14
int stlReleaseAuto(STL* pSTL, int base) ; // nt add 2017/8/22
int stlSerialize(STL* pSTL, CArchive& ar) ; // nt add 2017/9/1
void stlSwapXZ(STL* pSTL); // nt add 2021/6/4
void stlSwapYZ(STL* pSTL); // nt add 2021/6/4
void stlSwapXZ2(STL* pSTL); // smf add 2021/7/20
void stlSwapYZ2(STL* pSTL); // smf add 2021/7/20
void stlSwapXY2(STL* pSTL); // smf add 2021/7/20
//--------------------------------------------------------------
typedef struct _stlpck STLPCK ;
struct _stlpck
{
	PNT3D pos ;
	VEC3D dir ;
	double r ;

	PNT3D p ;
	double z ;
	STLTRI* tri ;
	STL* stl ;
} ;
////////////////////////////////////////////////////////////////

#endif