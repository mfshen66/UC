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
STLVECTOR operator- (STLVECTOR& iVector); // ����ȡ��
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
	int IsIn2(double p[3], double tol, double prj_p[3]); // nt add 2022/7/9 p��ͶӰ��Face�ڣ����߽磩
	void Prj(double p[3], double prj_p[3]);
	void GetBarycenter(double center[3]);
	int Draw(void* pVI, int drawMode);
	int Draw2(void* pVI, int drawMode);
};

// ƽ������������
int mathPlnIntTri(
	PNT3D iPntOfTri1, PNT3D iPntOfTri2, PNT3D iPntOfTri3, // �����ε���������
	PNT3D iPntOnPlane, VEC3D iNormPlane,  // ƽ��ķ�ʸ��ƽ����һ��
	double iTolLength, double iTolAngle,  // �����ݲ�Ƕ��ݲ�
	PNT3D* oPntsOfIntersection, // ���������ָ��
	int* oIndexOfIntEdge, int& oNumOfIntEdge); // ��ƽ���ཻ�ıߵ���ż�����

int mathPlnIntTri(
	STLPNT3D iPntOfTri1, STLPNT3D iPntOfTri2, STLPNT3D iPntOfTri3, // �����ε���������
	STLPNT3D iPntOnPlane, STLVECTOR iNormPlane,  // ƽ��ķ�ʸ��ƽ����һ��
	double iTolLength, double iTolAngle,  // �����ݲ�Ƕ��ݲ�
	STLPNT3D* oPntsOfIntersection,  // ���������ָ��
	int* oIndexOfIntEdge, int& oNumOfIntEdge); // ��ƽ���ཻ�ıߵ���ż�����

int mathPlnIntTri(
	FList iTriangle, // ������
	STLPNT3D iPntOnPlane, STLVECTOR iNormPlane,  // ƽ��ķ�ʸ��ƽ����һ��
	double iTolLength, double iTolAngle,  // �����ݲ�Ƕ��ݲ�
	STLPNT3D* oPntsOfIntersection,  // ���������ָ��
	int* oIndexOfIntEdge, int& oNumOfIntEdge); // ��ƽ���ཻ�ıߵ���ż�����

BOOLEAN mathIsPointOnCylinderExtension(PNT3D iPoint, PNT3D iCylBegin, PNT3D iCylEnd, double iRadius);
// ������ƽ������
double mathSquareDist(PNT3D p1, PNT3D p2);
double mathSquareDist(STLPNT3D p1, STLPNT3D p2);

// 2022/12/01 smf add 
// ָ���߶εĶ˵�
enum SEGMENT
{
	Begin = 0,
	End = +1
};
// 2022/12/01 smf add 
// ���̻�����һ���߶Ρ�iDist��Ϊ��ֵ��iDist < 0 ���̣�iDist > 0 ���졣
// iBeginOrEnd: 0-��㣻1-�յ㡣
// ����ֵ��0-�߶ι��̣��������̣�1-�ɹ���
int mathShortenOrLengthenSegmByDist(STLPNT3D &ioBegin, STLPNT3D &ioEnd, int iBeginOrEnd, double iDist);

BOOL mathIsCoincidentPoint(STLPNT3D iPoint1, STLPNT3D iPoint2);
// �����ľ���
double mathDist(STLPNT3D p1, STLPNT3D p2);

// ��ͶӰ��������
void mathPrjPntPln(STLPNT3D iPoint, STLPNT3D iPointOfPlane, STLVECTOR iNormalOfPlane, STLPNT3D& oProjectPoint);
// ����ͶӰ��������
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
// �����߶εĽ������
int mathSegmIntSegmInFace(STLPNT3D iBegin1, STLPNT3D iEnd1, STLPNT3D iBegin2, STLPNT3D iEnd2, double iTol, STLPNT3D& oIntPoint);

BOOL mathIsPointInSegm(STLPNT3D iPoint, STLPNT3D iBegin, STLPNT3D iEnd, double iTol);

STLPNT3D mathMidPoint(STLPNT3D iPoint1, STLPNT3D iPoint2); // �е�

// smf add 2022/12/10
// ���������ζ������Ϣ���ҵ������α���ֵ���ڸ���ֵ��һ��
// ioIndex-��ֵ���������
// Ӧ��ֻ����0��1��2��
// 0-�������ڲ�û�е�ֵ�㣻
// 1-�����ε�һ������Ϊ��ֵ�㣬������ö�����Χ������������
// 2-�����α�����������ֵ��
int mathCalPointsByTriangleVertexInfo(
	STLPNT3D iVertex1, STLPNT3D iVertex2, STLPNT3D iVertex3,
	double iInfo[3], // �����ζ��㴦����Ϣ��ֵ��
	double iRef, // �����Ĳο�ֵ
	double iTol, // �ݲ�
	STLPNT3D ioPoint[3], // ��ֵ��
	int* ioIndex); // ��ֵ���������

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
	int I; // λ������, I<1��ʾ���point pair����Ч�ģ�������
	double t; // [0., 1.)
	double d; // geodesic distance
	int perp; // nt add 2022/7/4 1=��ֱ, 0=����ֱ
};

struct GridModel {
	GridModel* Next;
	VList   VRoot;		// ����������ڵ�
	EList   EHead;		// ��������ͷ
	FList   FHead;		// ��������ͷ
	int  VerNum;		// ����
	int  EdgeNum;		// ����
	int  FaceNum;		// ����
	double stl_xmin;	// ��Χ�з�Χ, �ɶ�������
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
	// ���˹�ϵ����
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
	BOOL stlCompareTwo3DPoint(STLPNT3D p3d1, STLPNT3D p3d2, double epsdis);	// �������ƶȱȽ�
	int stlCompareTriangleVertex(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3, STLPNT3D q1, STLPNT3D q2, STLPNT3D q3); // ����Ƭ���, �����غϵ���

	STLPNT3D stlTwoVectorPlus(STLPNT3D p1, STLPNT3D p2);		// ��������� p1+p2
	STLPNT3D stlTwoVectorMinus(STLPNT3D p1, STLPNT3D p2);	// ��������� p1-p2
	STLPNT3D stlTwoVectorMultiply(STLPNT3D p1, double l1);	// �����ӳ� p1*l1
	STLPNT3D stlTwoVectorDivide(STLPNT3D p1, double l1);		// �������� p1/l1

	// add by wjq, ���������ķ�ʸ
	double triCalArea(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3);
	double CalDistofPtAndFace(STLPNT3D p0, FList f);
	FList FindClosetFace(STLPNT3D p0, double tol);
	int Is_in_triangle(STLPNT3D p0, FList f, double tol);
	STLVECTOR CalAllVertexNormalVector(STLPNT3D p1, double tol);	// add by wjq, ���㷨ʸ

	//20170825 ADD DAT�ļ�����
	int stlDealInputFile(char* pathName);
	void stlReadFile(char *file);
	int stlReadSTLBinFile(char *file);
	int stlReadStlFile(char *file);
	void stlRead4Bytes(FILE *stlfile, char *c);
	void stlReadDatFile(char *file);

	FList stlGetFacetFromNum(int N);	// ��ȡ���Ϊ N ����Ƭ
	//void GetNormaVectorOnVertex(VList p) ;
	double stlDistanceTwoPoints(STLPNT3D p1, STLPNT3D p2);	// ��������䳤

	STLPNT3D stlTwoVectorProduct(STLPNT3D v1, STLPNT3D v2);			// ����������
	STLPNT3D stlOPPNormalVector(STLPNT3D v);						// ���ص� v ����Ӧ���෴ʸ��
	STLPNT3D CreateMTIPoint();										// ����Ĭ�ϵ� (0.0,0.0,0.0)
	STLPNT3D CreateMTIPoint(double ix, double iy, double iz);			// �� xyz ����
	STLPNT3D CreateMTIPoint(double p1[]);							// �����鴴��
	STLPNT3D CreateMTIPoint(double p1[], int iNbStart);				// �������ĳλ�ô�����

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
#define MAXSIZE 20 /* �洢�ռ��ʼ������ */

typedef int Status;
typedef int SElemType; /* SElemType���͸���ʵ������������������Ϊint */


/* ��ջ�ṹ */
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