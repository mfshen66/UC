#ifndef _GRID_MODEL_h_
#define _GRID_MODEL_h_

#include "GP_API.h"
#include <vector>

#define EPS06 1e-6
#define EPS12 1e-12
#define EPS08 1e-8
//#define STLTRI 60

#define TOLLENGTH 1.e-3
#define TOLSQUARELENGTH 1.e-6
#define TOLANGLE 1.e-6

#define PI 3.14159265358979323846

#define INVERSQRT2 0.7071067811865475244

typedef struct {
	double   x, y, z;
	double inline norm() {
		return sqrt(x * x + y * y + z * z);
	};
} STLVECTOR;

typedef struct {
	double   x, y;
} POINT2D;

typedef STLVECTOR  STLPNT3D;
typedef struct VertexList   *VList;
typedef struct EdgeList     *EList;
typedef struct FaceList     *FList;
typedef struct FaceRelated  *FRelated;
typedef struct GridModel GM; // nt add 2022/6/15

// smf add 2022/09/07
STLVECTOR vectorNormalize(STLVECTOR& iVector);
STLVECTOR operator- (STLPNT3D& iEnd, STLPNT3D& iBegin);
STLVECTOR operator- (STLVECTOR& iVector); // 向量取反
STLVECTOR operator+ (STLVECTOR& iVector1, STLVECTOR& iVector2);
STLVECTOR operator* (double iScalar, STLVECTOR& iVector);
double operator*(STLVECTOR& iVector1, STLVECTOR& iVector2);
STLVECTOR operator^ (STLVECTOR& iVectorU, STLVECTOR& iVectorV);

struct VertexList {
	STLPNT3D     Coord;
	STLVECTOR    NormalVector;
	VList        Rchild;
	VList        Lchild;
	FRelated     FaceUsed;
	int          FaceNum;
	int			 flag; // nt add 2022/6/17

	VList SchMinVert(double p[3], double& min);
};

struct EdgeList {
	VList         VertexUsed[2];
	EList         ENext;
	FRelated      FaceUsed;
	int           FaceNum;
	double		  FCircleTag;
	int			  FaceCircleTag;
};

struct FaceList
{
	STLVECTOR   NormalVector;
	VList       VertexUsed[3];
	EList       EdgeUsed[3];
	FList       FNext;
	int         FaceNO;
	double		  FCircleTag;
	FaceList();
	~FaceList();
	int GetType();
	void GetVertPos(int k, double p[3]);
	void GetPos(double gc[3], double p[3]);
	int GetNumOfCV(FList f); // common vertex
	int IsAdj(FList f);
	int IsAdjVertex(FList f);
	FList GetAdj(int i, int j);
	FList GetAdjRayPass(double p[3], double dir[3], double tol); // nt add 2022/7/9
	EList GetEdge(int i, int j);
	VList SchMinVert(double p[3], double& min);
	int PlaneCut(double pivot[3], double normal[3], double p[3], double dir[3], double tol, double q[3], int& i, int& j);
	int IsIn(double p[3], double tol);
	int IsIn2(double p[3], double tol, double prj_p[3]); // nt add 2022/7/9 p的投影在Face内（含边界）
	void Prj(double p[3], double prj_p[3]);
	void GetBarycenter(double center[3]);
	int Draw(void* pVI, int drawMode);
	int Draw2(void* pVI, int drawMode);
};

// 平面与三角形求交
int mathPlnIntTri(
	PNT3D iPntOfTri1, PNT3D iPntOfTri2, PNT3D iPntOfTri3, // 三角形的三个顶点
	PNT3D iPntOnPlane, VEC3D iNormPlane,  // 平面的法矢及平面上一点
	double iTolLength, double iTolAngle,  // 长度容差及角度容差
	PNT3D* oPntsOfIntersection, // 交点数组的指针
	int* oIndexOfIntEdge, int& oNumOfIntEdge); // 与平面相交的边的序号及数量

int mathPlnIntTri(
	STLPNT3D iPntOfTri1, STLPNT3D iPntOfTri2, STLPNT3D iPntOfTri3, // 三角形的三个顶点
	STLPNT3D iPntOnPlane, STLVECTOR iNormPlane,  // 平面的法矢及平面上一点
	double iTolLength, double iTolAngle,  // 长度容差及角度容差
	STLPNT3D* oPntsOfIntersection,  // 交点数组的指针
	int* oIndexOfIntEdge, int& oNumOfIntEdge); // 与平面相交的边的序号及数量

int mathPlnIntTri(
	FList iTriangle, // 三角形
	STLPNT3D iPntOnPlane, STLVECTOR iNormPlane,  // 平面的法矢及平面上一点
	double iTolLength, double iTolAngle,  // 长度容差及角度容差
	STLPNT3D* oPntsOfIntersection,  // 交点数组的指针
	int* oIndexOfIntEdge, int& oNumOfIntEdge); // 与平面相交的边的序号及数量

BOOLEAN mathIsPointOnCylinderExtension(PNT3D iPoint, PNT3D iCylBegin, PNT3D iCylEnd, double iRadius);
// 两点间的平方距离
double mathSquareDist(PNT3D p1, PNT3D p2);
double mathSquareDist(STLPNT3D p1, STLPNT3D p2);

// 2022/12/01 smf add 
// 指代线段的端点
enum SEGMENT
{
	Begin = 0,
	End = +1
};
// 2022/12/01 smf add 
// 缩短或延伸一条线段。iDist可为负值，iDist < 0 缩短；iDist > 0 延伸。
// iBeginOrEnd: 0-起点；1-终点。
// 返回值：0-线段过短，不可缩短；1-成功。
int mathShortenOrLengthenSegmByDist(STLPNT3D &ioBegin, STLPNT3D &ioEnd, int iBeginOrEnd, double iDist);

BOOL mathIsCoincidentPoint(STLPNT3D iPoint1, STLPNT3D iPoint2);
// 两点间的距离
double mathDist(STLPNT3D p1, STLPNT3D p2);

// 点投影到三角形
void mathPrjPntPln(STLPNT3D iPoint, STLPNT3D iPointOfPlane, STLVECTOR iNormalOfPlane, STLPNT3D& oProjectPoint);
// 向量投影到三角形
void mathPrjVecPln(STLVECTOR iVector, STLVECTOR iNormal, STLVECTOR &oPrjVec);

void mathTransWorldVec3DByXYZ(VEC3D e1,
	VEC3D e2,
	VEC3D e3,
	VEC3D world_vector,
	VEC3D local_vector);

int mathIntLinCyl(PNT3D begin,
	VEC3D dir,
	PNT3D pivot,
	VEC3D axis,
	double r,
	double tol,
	double ang,
	PNT3D intpt1,
	PNT3D intpt2);

void mathTransLocalPnt3DByOXYZ(PNT3D origin,
	VEC3D e1,
	VEC3D e2,
	VEC3D e3,
	PNT3D local_point,
	PNT3D world_point);

int mathIntSegmCyl(PNT3D begin,
	PNT3D end,
	PNT3D pivot,
	VEC3D axis,
	double r,
	double tol,
	int* pn,
	PNT3D p1,
	PNT3D p2,
	double* pt1,
	double* pt2);

// smf add 2023/02/24
// 共面线段的交点计算
int mathSegmIntSegmInFace(STLPNT3D iBegin1, STLPNT3D iEnd1, STLPNT3D iBegin2, STLPNT3D iEnd2, double iTol, STLPNT3D& oIntPoint);

BOOL mathIsPointInSegm(STLPNT3D iPoint, STLPNT3D iBegin, STLPNT3D iEnd, double iTol);

STLPNT3D mathMidPoint(STLPNT3D iPoint1, STLPNT3D iPoint2); // 中点

// smf add 2022/12/10
// 根据三角形顶点的信息，找到三角形边上值等于给定值的一点
// ioIndex-等值顶点的索引
// 应该只返回0，1或2：
// 0-三角形内部没有等值点；
// 1-三角形的一个顶点为等值点，需遍历该顶点周围的所有三角形
// 2-三角形边上有两个等值点
int mathCalPointsByTriangleVertexInfo(
	STLPNT3D iVertex1, STLPNT3D iVertex2, STLPNT3D iVertex3,
	double iInfo[3], // 三角形顶点处的信息（值）
	double iRef, // 给定的参考值
	double iTol, // 容差
	STLPNT3D ioPoint[3], // 等值点
	int* ioIndex); // 等值顶点的索引

BOOL mathIsVectorDuringTwoVectors(STLVECTOR iVector1, STLVECTOR iVector2, STLVECTOR iVector);

const double INVSQRT2 = 0.70710678118654752440;
const double delta_l = 50.;

using namespace std;

struct FaceRelated abstract {
	FList     Face;
	FRelated  FNext;
};

typedef struct Pp PP; // point pair
struct Pp
{
	FList fs[2];
	double ps[2][3];
	int I; // 位置索引, I<1表示这个point pair是无效的，不能用
	double t; // [0., 1.)
	double d; // geodesic distance
	int perp; // nt add 2022/7/4 1=垂直, 0=不垂直
};

struct GridModel {
	GridModel* Next;
	VList   VRoot;		// 点二叉树根节点
	EList   EHead;		// 边链表链头
	FList   FHead;		// 面链表链头
	int  VerNum;		// 点数
	int  EdgeNum;		// 边数
	int  FaceNum;		// 面数
	double stl_xmin;	// 包围盒范围, 由读入点更新
	double stl_ymin;
	double stl_zmin;
	double stl_xmax;
	double stl_ymax;
	double stl_zmax;
	int TraverseNum;
	int MeshResulttag;
	char MeshResult[10][256];
	char PointName[100];
	STLPNT3D PBreak;
	double isag, istep, iangle, Plyangle, dval, gapValue;
	int TrailNumDum, FiberNum;
	int stlFaceTag;

	GridModel();
	BOOL GridModel::stlCreateSTLTriangle(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3);
	// 拓扑关系建立
	FList stlCreateFace(STLVECTOR *nv, STLPNT3D *p1, STLPNT3D *p2, STLPNT3D *p3, int tNum);
	FList stlCreateFace(STLVECTOR *nv, STLPNT3D *p1, STLPNT3D *p2, STLPNT3D *p3, STLVECTOR *v1, STLVECTOR *v2, STLVECTOR *v3, int tNum);
	FList stlFaceAlloc();
	VList stlCreateVertex(VList *root, STLPNT3D *pt);
	VList stlCreateVertex(VList *root, STLPNT3D *pt, STLVECTOR *vt);
	VList stlSortTree(STLPNT3D *pt, VList *t);
	VList stlSortTree(STLPNT3D *pt, STLVECTOR *vt, VList *t);
	VList stlVertexAlloc();
	EList stlCreateEdge(VList p1, VList p2);
	EList stlEdgeAlloc();
	void stlAddFace2Vertex(FList f, VList v);
	void stlAddFace2Edge(FList f, EList e);

	// 2016.09Add
	BOOL stlCompareTwo3DPoint(STLPNT3D p3d1, STLPNT3D p3d2, double epsdis);	// 两点相似度比较
	int stlCompareTriangleVertex(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3, STLPNT3D q1, STLPNT3D q2, STLPNT3D q3); // 三角片检测, 返回重合点数

	STLPNT3D stlTwoVectorPlus(STLPNT3D p1, STLPNT3D p2);		// 两向量求和 p1+p2
	STLPNT3D stlTwoVectorMinus(STLPNT3D p1, STLPNT3D p2);	// 两向量求差 p1-p2
	STLPNT3D stlTwoVectorMultiply(STLPNT3D p1, double l1);	// 向量延长 p1*l1
	STLPNT3D stlTwoVectorDivide(STLPNT3D p1, double l1);		// 向量缩短 p1/l1

	// add by wjq, 计算任意点的法矢
	double triCalArea(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3);
	double CalDistofPtAndFace(STLPNT3D p0, FList f);
	FList FindClosetFace(STLPNT3D p0, double tol);
	int Is_in_triangle(STLPNT3D p0, FList f, double tol);
	STLVECTOR CalAllVertexNormalVector(STLPNT3D p1, double tol);	// add by wjq, 计算法矢

	//20170825 ADD DAT文件处理
	int stlDealInputFile(char* pathName);
	void stlReadFile(char *file);
	int stlReadSTLBinFile(char *file);
	int stlReadStlFile(char *file);
	void stlRead4Bytes(FILE *stlfile, char *c);
	void stlReadDatFile(char *file);

	FList stlGetFacetFromNum(int N);	// 获取编号为 N 的面片
	//void GetNormaVectorOnVertex(VList p) ;
	double stlDistanceTwoPoints(STLPNT3D p1, STLPNT3D p2);	// 计算两点间长

	STLPNT3D stlTwoVectorProduct(STLPNT3D v1, STLPNT3D v2);			// 返回两点叉乘
	STLPNT3D stlOPPNormalVector(STLPNT3D v);						// 返回点 v 所对应的相反矢量
	STLPNT3D CreateMTIPoint();										// 创建默认点 (0.0,0.0,0.0)
	STLPNT3D CreateMTIPoint(double ix, double iy, double iz);			// 从 xyz 创点
	STLPNT3D CreateMTIPoint(double p1[]);							// 从数组创点
	STLPNT3D CreateMTIPoint(double p1[], int iNbStart);				// 从数组的某位置创建点

	void InitFlagAll();
	FList Prj(FList f1, double p1[3], FList f2, double p2[3], double tol, double ptOnLine[3], double ptOnFace[3]); // nt add 2022/7/8	
	FList Pick(PNT3D pos, VEC3D dir, double r, PNT3D p, double* pz, PNT3D gc);
	void Draw(void* pVI, int drawMode);

	// smf add 2023/01/31
	static BOOL IsPointOnBoundary(STLPNT3D iPoint, FList iSupport);

	static BOOL IsPointAVertex(STLPNT3D iPoint, FaceList* iFace, int &oIndex);
	static BOOL IsPointOnEdge(STLPNT3D iPoint, EList iEdge);
	static BOOL IsPointInTriangle(STLPNT3D iPoint, FList iTri);
	static BOOL IsSegmInTriangle(STLPNT3D iBegin, STLPNT3D iEnd, FList iTri);
};


// 2022/10/10 smf add

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define MAXSIZE 20 /* 存储空间初始分配量 */

typedef int Status;
typedef int SElemType; /* SElemType类型根据实际情况而定，这里假设为int */


/* 链栈结构 */
typedef struct StackNode
{
	SElemType data;
	struct StackNode* next;
}StackNode, *LinkStackPtr;


typedef struct
{
	LinkStackPtr top;
	int count;
}LinkStack;

#endif