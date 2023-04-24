#include "stdafx.h"
#include "GridModel.h"
//#include "STL.h"
#include <vector>

using namespace std;
//==============================================================
/*	1997/4/22 nt
 *	在2D平面上判断原点是否在一个三角形内
 *	return IDIN: in ; IDOUT: not in; IDDEGE, degenerate
 */
//int mathChkOriInTri2D(double p1[2],
//	double p2[2],
//	double p3[2],
//	double tol)
//{
//	int ks[3], i = 0;
//	double xs[6];
//
//	if ((p1[0] > tol && p2[0] > tol && p3[0] > tol) ||
//		(p1[0] < -tol && p2[0] < -tol && p3[0] < -tol) ||
//		(p1[1] > tol && p2[1] > tol && p3[1] > tol) ||
//		(p1[1] < -tol && p2[1] < -tol && p3[1] < -tol))
//		return IDOUT;
//
//	ks[0] = fabs(p1[1]) < tol ? 0 : p1[1] > 0.0 ? 1 : -1;
//	ks[1] = fabs(p2[1]) < tol ? 0 : p2[1] > 0.0 ? 1 : -1;
//	ks[2] = fabs(p3[1]) < tol ? 0 : p3[1] > 0.0 ? 1 : -1;
//	if (ks[0] == 0) xs[i++] = p1[0];
//	if (ks[1] == 0) xs[i++] = p2[0];
//	if (ks[2] == 0) xs[i++] = p3[0];
//	if (i >= 3)
//		return IDDEGE;
//	if (ks[0] * ks[1] < 0)
//		xs[i++] = p1[0] + p1[1] * (p2[0] - p1[0]) / (p1[1] - p2[1]);
//	if (ks[1] * ks[2] < 0)
//		xs[i++] = p2[0] + p2[1] * (p3[0] - p2[0]) / (p2[1] - p3[1]);
//	if (ks[2] * ks[0] < 0)
//		xs[i++] = p3[0] + p3[1] * (p1[0] - p3[0]) / (p3[1] - p1[1]);
//
//	if (i == 2)
//	{
//		if (xs[0] * xs[1] < 0.0 ||
//			fabs(xs[0]) < tol ||
//			fabs(xs[1]) < tol)
//			return IDIN;
//	}
//
//	return IDOUT;
//}

/*	1997/5/5 nt
 *	计算一个点相对于2D三角形的重心坐标
 */
//int mathCalTri2DGCP(PNT2D p1,
//	PNT2D p2,
//	PNT2D p3,
//	PNT2D p,
//	PNT3D gc)
//{
//	double a11, a12, a21, a22, delta;
//
//	a11 = p1[0] - p3[0];
//	a12 = p2[0] - p3[0];
//	a21 = p1[1] - p3[1];
//	a22 = p2[1] - p3[1];
//	if (fabs(delta = a11 * a22 - a12 * a21) < 1e-20)
//		return ERUNSUC;
//	gc[0] = ((p3[1] - p[1])*a12 - (p3[0] - p[0])*a22) / delta;
//	gc[1] = ((p3[0] - p[0])*a21 - (p3[1] - p[1])*a11) / delta;
//	gc[2] = 1.0 - gc[0] - gc[1];
//
//	return ERSUCSS;
//}

/*	2016/8 nt
 *  将p投影到三角形上，再按二维情况计算重心坐标
 */
int mathCalTriGCPX(PNT3D p1, PNT3D p2, PNT3D p3, VEC3D normal, PNT3D p, PNT3D gc)
{
	PNT2D q1, q2, q3, q;
	PNT3D O;
	VEC3D v, X, Y;

	O[0] = (p1[0] + p2[0] + p3[0]) / 3;
	O[1] = (p1[1] + p2[1] + p3[1]) / 3;
	O[2] = (p1[2] + p2[2] + p3[2]) / 3;
	mathGetXYFromZ(normal, X, Y);

	mathGetVec(O, p1, v);
	q1[0] = mathOProduct(X, v); // to local coordinate frame lf
	q1[1] = mathOProduct(Y, v);

	mathGetVec(O, p2, v);
	q2[0] = mathOProduct(X, v); // to local coordinate frame lf
	q2[1] = mathOProduct(Y, v);

	mathGetVec(O, p3, v);
	q3[0] = mathOProduct(X, v); // to local coordinate frame lf
	q3[1] = mathOProduct(Y, v);

	mathGetVec(O, p, v);
	q[0] = mathOProduct(X, v); // to local coordinate frame lf
	q[1] = mathOProduct(Y, v);

	mathCalTri2DGCP(q1, q2, q3, q, gc);

	return ERSUCSS;
}

/*  1997/2/8 nt
 *	计算点与线段的最近距离和与该点最近的线段上点的参数,参数在0.0,1.0之间
 */
double mathGetPntSegmNDT(PNT3D p, PNT3D begin, PNT3D end, double* pt)
{
	VEC3D v1, v2;
	double A, B, C;

	*pt = 0.0;
	v1[0] = end[0] - begin[0];
	v1[1] = end[1] - begin[1];
	v1[2] = end[2] - begin[2];
	v2[0] = begin[0] - p[0];
	v2[1] = begin[1] - p[1];
	v2[2] = begin[2] - p[2];
	A = v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2];
	B = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	C = v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2];
	if (A > 1e-20)
	{
		pt[0] = -B / A;
		if (pt[0] > 1.0)
		{
			C += (A + 2.0*B);
			pt[0] = 1.0;
		}
		else
			if (pt[0] > 0.0)
				C += B * pt[0];
			else
				pt[0] = 0.0;
	}

	return sqrt(C);
}

// 线段：from begin to end
// *perp = 1 : 最近点与p的连线垂直与线段; *perp = 0, 表示不垂直
// 是否垂直的判断依据：垂足是否在线段上，容差为tol
double mathGetPntSegmNDTPerp(PNT3D p, PNT3D begin, PNT3D end, double tol, double* pt, int* perp)
{
	VEC3D v1, v2;
	double A, B, C;

	*pt = 0.;
	*perp = 1;
	v1[0] = end[0] - begin[0];
	v1[1] = end[1] - begin[1];
	v1[2] = end[2] - begin[2];
	v2[0] = begin[0] - p[0];
	v2[1] = begin[1] - p[1];
	v2[2] = begin[2] - p[2];
	A = v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2];
	B = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	C = v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2];
	if (A > 1e-20)
	{
		pt[0] = -B / A;
		if (pt[0] > 1.)
		{
			C += (A + 2.*B);
			if ((pt[0] - 1.)*sqrt(A) > tol)
				*perp = 0;
			pt[0] = 1.0;
		}
		else
			if (pt[0] >= 0.)
				C += B * pt[0];
			else // pt[0] < 0.
			{
				if (-pt[0] * sqrt(A) > tol)
					*perp = 0;
				pt[0] = 0.;
			}
	}

	return sqrt(C);
}

int mathChkPntSegmX(PNT3D p, PNT3D begin, PNT3D end, double tol)
{
	VEC3D v1, v2;
	double A, B, C, t;

	if (mathMIN(begin[0], end[0]) > p[0] + tol || //chk box intersection
		mathMAX(begin[0], end[0]) + tol < p[0] ||
		mathMIN(begin[1], end[1]) > p[1] + tol ||
		mathMAX(begin[1], end[1]) + tol < p[1] ||
		mathMIN(begin[2], end[2]) > p[2] + tol ||
		mathMAX(begin[2], end[2]) + tol < p[2])
		return IDOUT;

	v1[0] = end[0] - begin[0];
	v1[1] = end[1] - begin[1];
	v1[2] = end[2] - begin[2];
	v2[0] = begin[0] - p[0];
	v2[1] = begin[1] - p[1];
	v2[2] = begin[2] - p[2];
	A = v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2];
	B = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	C = v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2];
	if (A > 1e-20)
	{
		t = -B / A;
		if (t > 1.0)
			C += (A + 2.0*B);
		else
			if (t > 0.0)
				C += B * t;
	}

	C = sqrt(fabs(C));
	if (C < tol)
		return IDIN;
	else
		return IDOUT;
}

int mathGetTriNormal(PNT3D p1, PNT3D p2, PNT3D p3, VEC3D normal)
{
	int k;
	VEC3D v1, v2;

	for (k = 0; k < 3; k++)
	{
		v1[k] = p2[k] - p1[k];
		v2[k] = p3[k] - p2[k];
	}
	mathVProduct(v1, v2, normal);
	if (mathUniVec(normal, 1e-50) != ERSUCSS)
		return ERUNSUC;

	return ERSUCSS;
}
//==============================================================
FList GetFaceOfTwoEdge(EList e1, EList e2)
{
	FRelated r1, r2;

	for (r1 = e1->FaceUsed; r1; r1 = r1->FNext)
	{
		for (r2 = e2->FaceUsed; r2; r2 = r2->FNext)
			if (r1->Face == r2->Face)
				return r1->Face;
	}

	return NULL;
}

double rand01()
{
	return (double)rand() / RAND_MAX;
}

//==============================================================

// 点iPoint是否是三角形iFace的顶点
BOOL GridModel::IsPointAVertex(STLPNT3D iPoint, FaceList* iFace, int &oIndex)
{
	for (int i = 0; i < 3; i++)
	{
		if (mathSquareDist(iPoint, iFace->VertexUsed[i]->Coord) <= TOLSQUARELENGTH)
		{
			oIndex = i;
			return true;
		}
	}
	oIndex = -1;
	return false;
}

BOOL GridModel::IsPointOnEdge(STLPNT3D iPoint, EList iEdge)
{
	STLVECTOR v;
	STLPNT3D p;
	double dis = mathDist(iPoint, iEdge->VertexUsed[0]->Coord);
	v = iEdge->VertexUsed[1]->Coord - iEdge->VertexUsed[0]->Coord;
	v = vectorNormalize(v);
	p = iEdge->VertexUsed[0]->Coord + dis * v;

	if (mathIsCoincidentPoint(iPoint, p))
		return true;
	else
		return false;
}

// smf add 2022/12/13
// 判断点是否在三角形内
BOOL GridModel::IsPointInTriangle(STLPNT3D iPoint, FList iTri)
{
	BOOL rc = IDOUT;

	PNT3D p, pviot;
	VEC3D normal;
	memcpy(p, &iPoint, sizeof(STLPNT3D));
	memcpy(pviot, &(iTri->VertexUsed[0]->Coord), sizeof(STLPNT3D));
	memcpy(normal, &(iTri->NormalVector), sizeof(STLVECTOR));
	double d = 0.;
	if ((d = mathDistPntPln(p, pviot, normal)) >= TOLLENGTH) // 点与三角形不共面
	{
		return IDOUT;
	}
	else
	{
		STLVECTOR ap, bp, cp, ab, bc, ca;
		ap = iPoint - iTri->VertexUsed[0]->Coord;
		ap = vectorNormalize(ap);
		bp = iPoint - iTri->VertexUsed[1]->Coord;
		bp = vectorNormalize(bp);
		cp = iPoint - iTri->VertexUsed[2]->Coord;
		cp = vectorNormalize(cp);
		ab = iTri->VertexUsed[1]->Coord - iTri->VertexUsed[0]->Coord;
		ab = vectorNormalize(ab);
		bc = iTri->VertexUsed[2]->Coord - iTri->VertexUsed[1]->Coord;
		bc = vectorNormalize(bc);
		ca = iTri->VertexUsed[0]->Coord - iTri->VertexUsed[2]->Coord;
		ca = vectorNormalize(ca);

		double mixed_product1 = vectorNormalize(ab ^ ap) * iTri->NormalVector;
		double mixed_product2 = vectorNormalize(bc ^ bp) * iTri->NormalVector;
		double mixed_product3 = vectorNormalize(ca ^ cp) * iTri->NormalVector;

		if ((fabs(mixed_product1) <= TOLANGLE ||
			fabs(mixed_product2) <= TOLANGLE ||
			fabs(mixed_product3) <= TOLANGLE) ||
			mixed_product1 * mixed_product2 >= 0 &&
			mixed_product2 * mixed_product3 >= 0)
		{
			return IDIN;
		}
		else
		{
			return IDOUT;
		}
	}
}

BOOL GridModel::IsSegmInTriangle(STLPNT3D iBegin, STLPNT3D iEnd, FList iTri)
{
	if (IsPointInTriangle(iBegin, iTri) &&
		IsPointInTriangle(iEnd, iTri))
	{
		return true;
	}
	return false;
}

double CalVariance(double * iArray, int iSize)
{
	int variance = 0.;
	int average = 0.;

	if (iSize > 2)
	{
		for (size_t i = 0; i < iSize; i++)
		{
			average += iArray[i];
		}
		average = average * 1. / iSize;

		for (size_t i = 0; i < 3; i++)
		{
			variance += (average - iArray[i])*(average - iArray[i]);
		}
		variance = variance * 0.5;
	}
	else
	{
		variance = 0.;
	}
	return variance;
}

//==============================================================
VList VertexList::SchMinVert(double p[3], double& min)
{
	VList v, V = NULL;
	FList f;
	FRelated r;

	for (r = FaceUsed; r; r = r->FNext)
	{
		f = r->Face;
		if (f)
		{
			v = f->SchMinVert(p, min);
			if (v)
				V = v;
		}
	}

	return V;
}
//==============================================================
FaceList::FaceList()
{
}

FaceList::~FaceList()
{
}

int FaceList::GetType()
{
	return 60;
}

int FaceList::GetNumOfCV(FList f)
{
	int i, j, n = 0;

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (VertexUsed[i] == f->VertexUsed[j])
			{
				n++;
				break;
			}
		}
	}

	return n;
}

int FaceList::IsAdj(FList f)
{
	return GetNumOfCV(f) == 2 ? 1 : 0;
}

int FaceList::IsAdjVertex(FList f)
{
	return GetNumOfCV(f) == 1 ? 1 : 0;
}

void FaceList::GetVertPos(int k, double p[3])
{
	memcpy(p, &VertexUsed[k % 3]->Coord.x, sizeof(double[3]));

	return;
}

void FaceList::GetPos(double gc[3], double p[3])
{
	p[0] = gc[0] * VertexUsed[0]->Coord.x +
		gc[1] * VertexUsed[1]->Coord.x +
		gc[2] * VertexUsed[2]->Coord.x;
	p[1] = gc[0] * VertexUsed[0]->Coord.y +
		gc[1] * VertexUsed[1]->Coord.y +
		gc[2] * VertexUsed[2]->Coord.y;
	p[2] = gc[0] * VertexUsed[0]->Coord.z +
		gc[1] * VertexUsed[1]->Coord.z +
		gc[2] * VertexUsed[2]->Coord.z;

	return;
}

FList FaceList::GetAdj(int i, int j)
{
	int k;
	FRelated r;
	EList e;

	for (k = 0; k < 3; k++)
	{
		e = EdgeUsed[k];
		if ((e->VertexUsed[0] == VertexUsed[i] &&
			e->VertexUsed[1] == VertexUsed[j]) ||
			(e->VertexUsed[0] == VertexUsed[j] &&
				e->VertexUsed[1] == VertexUsed[i]))
		{
			for (r = e->FaceUsed; r; r = r->FNext)
			{
				if (r->Face != this)
					return r->Face;
			}
		}
	}

	return NULL;
}

EList FaceList::GetEdge(int i, int j)
{
	int k;
	EList e;

	for (k = 0; k < 3; k++)
	{
		e = EdgeUsed[k];
		if ((e->VertexUsed[0] == VertexUsed[i % 3] &&
			e->VertexUsed[1] == VertexUsed[j % 3]) ||
			(e->VertexUsed[0] == VertexUsed[j % 3] &&
				e->VertexUsed[1] == VertexUsed[i % 3]))
			return e;
	}

	return NULL; // error
}

VList FaceList::SchMinVert(double p[3], double& min)
{
	int k, I = -1;
	double d;

	for (k = 0; k < 3; k++)
	{
		if (VertexUsed[k]->flag == 0) // unused only
		{
			d = mathDist(&VertexUsed[k]->Coord.x, p);
			if (min > d)
			{
				min = d;
				I = k;
			}
		}
	}

	return I < 0 ? NULL : VertexUsed[I];
}

void FaceList::GetBarycenter(double center[3])
{
	center[0] = (VertexUsed[0]->Coord.x + VertexUsed[1]->Coord.x + VertexUsed[2]->Coord.x) / 3;
	center[1] = (VertexUsed[0]->Coord.y + VertexUsed[1]->Coord.y + VertexUsed[2]->Coord.y) / 3;
	center[2] = (VertexUsed[0]->Coord.z + VertexUsed[1]->Coord.z + VertexUsed[2]->Coord.z) / 3;

	return;
}

void FaceList::Prj(double p[3], double prj_p[3])
{
	int k;
	double gc[3], s = 0.;

	mathCalTriGCPX(&VertexUsed[0]->Coord.x,
		&VertexUsed[1]->Coord.x,
		&VertexUsed[2]->Coord.x,
		&NormalVector.x,
		p,
		gc);
	for (k = 0; k < 3; k++)
	{
		if (gc[k] < 0.)
			gc[k] = 0.;
		s += gc[k];
	}
	if (s < 1e-50)
	{
		memcpy(prj_p, p, sizeof(double[3]));
		return; // error
	}
	for (k = 0; k < 3; k++)
		gc[k] /= s;

	prj_p[0] = gc[0] * VertexUsed[0]->Coord.x +
		gc[1] * VertexUsed[1]->Coord.x +
		gc[2] * VertexUsed[2]->Coord.x;
	prj_p[1] = gc[0] * VertexUsed[0]->Coord.y +
		gc[1] * VertexUsed[1]->Coord.y +
		gc[2] * VertexUsed[2]->Coord.y;
	prj_p[2] = gc[0] * VertexUsed[0]->Coord.z +
		gc[1] * VertexUsed[1]->Coord.z +
		gc[2] * VertexUsed[2]->Coord.z;

	return;
}

// 输入：
//		pivot:平面上一点
//		normal:平面法矢
//		p:三角形内或其边上一点
//		dir:单位方向，指向测地线的终点
//		tol:点重合容差
// 输出：
//		q是三角形边上一点或三角形顶点
//		i==j表示q就是顶点i
//		i!=j表示q在边上，该边的顶点为i和j
// 功能:
//		平面切三角形得到三角形边上一点q，保证q-p与dir同向
// return 0=not found q, 1=found q
int FaceList::PlaneCut(double pivot[3],
	double normal[3],
	double p[3],
	double dir[3],
	double tol,
	double q[3],
	int& i,
	int& j)
{
	int k, m;
	double* vs[3], hs[3], h, d1, d2, ps[3][3], Q[3], max = 0.0; // tol

	i = j = -1;

	for (k = 0; k < 3; k++)
	{
		vs[k] = &VertexUsed[k]->Coord.x;
		hs[k] = mathDistPntPlnSide(vs[k], pivot, normal); // original use p
	}

	for (k = 0; k < 3; k++)
	{
		m = k < 2 ? k + 1 : 0;
		if (fabs(hs[k]) < tol &&
			fabs(hs[m]) < tol)
		{
			d1 = mathOProduct2(dir, p, vs[k]);
			d2 = mathOProduct2(dir, p, vs[m]);
			if (d1 >= d2 && d1 > 0.) // nt modify 2022/7/8 old tol
			{
				memcpy(q, vs[k], sizeof(double[3]));
				i = j = k;
				return 1;
			}
			if (d2 >= d1 && d2 > 0.) // nt modify 2022/7/8 old tol
			{
				memcpy(q, vs[m], sizeof(double[3]));
				i = j = m;
				return 1;
			}
			return 0;
		}
	}

	for (k = 0; k < 3; k++)
		mathPrjPntPln(vs[k], pivot, normal, ps[k]); // original use p
	for (k = 0; k < 3; k++)
	{
		m = k < 2 ? k + 1 : 0;
		if (hs[k] * hs[m] < 0.)
		{
			h = hs[k] - hs[m];
			mathMixPnt(ps[k], ps[m], -hs[m] / h, hs[k] / h, Q);
			h = mathOProduct2(dir, p, Q);
			if (max <= h)
			{
				// 原来必须要求交点与dir同向, 即初始 max = 0
				memcpy(q, Q, sizeof(double[3]));
				i = k;
				j = m;
				max = h;
			}
		}
	}

	for (k = 0; k < 3; k++)
	{
		if (fabs(hs[k]) < tol)
		{
			h = mathOProduct2(dir, p, vs[k]);
			if (max <= h)
			{
				memcpy(q, vs[k], sizeof(double[3]));
				i = j = k;
				return 1;
			}
		}
	}

	return i < 0 ? 0 : 1;
}

// 返回：
//		-1=退化
//		0=not in triangle
//		1=in interior
//		2=on edge
//		3=coincident with vertex
int FaceList::IsIn(double p[3], double tol)
{
	double *A, *B, *C, q[3], d, AB[3], AC[3], v[3], center[3], s, t;

	GetBarycenter(center);
	mathPrjPntPln(p, center, &NormalVector.x, q);
	d = mathDist(p, q);
	if (d > tol)
		return 0;

	A = &VertexUsed[0]->Coord.x;
	B = &VertexUsed[1]->Coord.x;
	C = &VertexUsed[2]->Coord.x;
	if (mathDist(p, A) < tol || // p is coincident with one vertex
		mathDist(p, B) < tol ||
		mathDist(p, C) < tol)
		return 3;

	if (mathChkPntSegmX(p, A, B, tol) == IDIN || // p is on one edge
		mathChkPntSegmX(p, B, C, tol) == IDIN ||
		mathChkPntSegmX(p, C, A, tol) == IDIN)
		return 2;

	mathGetVec(A, B, AB);
	mathGetVec(A, C, AC);
	mathVProduct(AB, &NormalVector.x, v); // v perpendicular to AB
	d = mathOProduct(AC, v);
	if (fabs(d) < 1e-50)
		return -1; // degenerate
	t = mathOProduct2(v, A, p) / d;
	if (t < 0. || t > 1.)
		return 0;
	mathVProduct(AC, &NormalVector.x, v); // v perpendicular to AC
	d = mathOProduct(AB, v);
	if (fabs(d) < 1e-50)
		return -1; // degenerate
	s = mathOProduct2(v, A, p) / d;
	if (s < 0. || s > 1.)
		return 0;
	if (s + t < 1.)
		return 1;
	else
		return 0;
}

int FaceList::IsIn2(double p[3], double tol, double prj_p[3])
{
	double center[3], q[3];

	GetBarycenter(center);
	mathPrjPntPln(p, center, &NormalVector.x, q);
	if (prj_p)
		memcpy(prj_p, q, sizeof(double[3]));

	return IsIn(q, tol);
}

int FaceList::Draw(void* pVI, int drawMode)
{
	return 1;
}

int FaceList::Draw2(void* pVI, int drawMode)
{
	MATERIAL material;
	if (drawMode == dmNormal)
		materialGet2(&material, 9);
	else
		if (drawMode == dmPrompt)
			materialGet2(&material, 1);
		else
			if (drawMode == dmSelect)
				materialGet2(&material, 41);
	materialSet(&material);

	viLighting(pVI, 1);
	viAddDefaultLight(pVI);
	glBegin(GL_TRIANGLES);
	glNormal3dv(&NormalVector.x);
	glVertex3dv(&VertexUsed[0]->Coord.x);
	glNormal3dv(&NormalVector.x);
	glVertex3dv(&VertexUsed[1]->Coord.x);
	glNormal3dv(&NormalVector.x);
	glVertex3dv(&VertexUsed[2]->Coord.x);
	glEnd();
	viLighting(pVI, 0);

	return 1;
}
//==============================================================
GridModel::GridModel()
{
	VRoot = NULL;
	EHead = NULL;
	FHead = NULL;
	VerNum = 0;
	EdgeNum = 0;
	FaceNum = 0;
	stl_xmin = 1e100;
	stl_ymin = 1e100;
	stl_zmin = 1e100;
	stl_xmax = -1e100;
	stl_ymax = -1e100;
	stl_zmax = -1e100;
	TraverseNum = 0;
	MeshResulttag = 0;
	// nt add 2022/6/17
}

void GridModel::InitFlagAll()
{
	EList e;

	for (e = EHead; e; e = e->ENext)
	{
		if (e->VertexUsed[0])
			e->VertexUsed[0]->flag = 0;
		if (e->VertexUsed[1])
			e->VertexUsed[1]->flag = 0;
	}

	return;
}

void GridModel::Draw(void* pVI, int drawMode)
{
	MATERIAL material;
	if (drawMode == dmNormal)
		materialGet2(&material, 9);
	else
		if (drawMode == dmPrompt)
			materialGet2(&material, 1);
		else
			if (drawMode == dmSelect)
				materialGet2(&material, 41);
	materialSet(&material);

	FList f;
	glBegin(GL_TRIANGLES);
	for (f = FHead; f; f = f->FNext)
	{
		glNormal3dv(&f->NormalVector.x);
		glVertex3dv(&f->VertexUsed[0]->Coord.x);
		glNormal3dv(&f->NormalVector.x);
		glVertex3dv(&f->VertexUsed[1]->Coord.x);
		glNormal3dv(&f->NormalVector.x);
		glVertex3dv(&f->VertexUsed[2]->Coord.x);
	}
	glEnd();

	return;
}

FList GridModel::Pick(PNT3D pos,
	VEC3D dir,
	double r,
	PNT3D p, // 输入输出参数
	double* pz, // 输入输出参数
	PNT3D gc)
{
	int k;
	double z;
	PNT2D ps[3], ori = { 0.,0. };
	PNT3D q;
	VEC3D X, Y, v;
	BOX3D box;
	FList f, picked = NULL;

	box.min[0] = stl_xmin;
	box.min[1] = stl_ymin;
	box.min[2] = stl_zmin;
	box.max[0] = stl_xmax;
	box.max[1] = stl_ymax;
	box.max[2] = stl_zmax;


	if (mathChkBoxLinInt(&box, pos, dir, r) == IDINT)
	{
		mathGetXYFromZ(dir, X, Y);
		for (f = FHead; f; f = f->FNext)
		{
			for (k = 0; k < 3; k++)
			{
				v[0] = f->VertexUsed[k]->Coord.x - pos[0];
				v[1] = f->VertexUsed[k]->Coord.y - pos[1];
				v[2] = f->VertexUsed[k]->Coord.z - pos[2];
				ps[k][0] = v[0] * X[0] + v[1] * X[1] + v[2] * X[2];
				ps[k][1] = v[0] * Y[0] + v[1] * Y[1] + v[2] * Y[2];
			}
			if (mathChkOriInTri2D(ps[0], ps[1], ps[2], r) == IDIN)
			{
				mathCalTri2DGCP(ps[0], ps[1], ps[2], ori, gc);
				q[0] = f->VertexUsed[0]->Coord.x*gc[0] +
					f->VertexUsed[1]->Coord.x*gc[1] +
					f->VertexUsed[2]->Coord.x*gc[2];
				q[1] = f->VertexUsed[0]->Coord.y*gc[0] +
					f->VertexUsed[1]->Coord.y*gc[1] +
					f->VertexUsed[2]->Coord.y*gc[2];
				q[2] = f->VertexUsed[0]->Coord.z*gc[0] +
					f->VertexUsed[1]->Coord.z*gc[1] +
					f->VertexUsed[2]->Coord.z*gc[2];
				v[0] = q[0] - pos[0];
				v[1] = q[1] - pos[1];
				v[2] = q[2] - pos[2];
				z = v[0] * dir[0] + v[1] * dir[1] + v[2] * dir[2];
				if (*pz > z)
				{
					memcpy(p, q, sizeof(PNT3D));
					picked = f;
					*pz = z;
				}
			}
		}
	}

	return picked;
}

// nt add 2022/7/9
// 找被射线穿过的邻面
FList FaceList::GetAdjRayPass(double p[3], double dir[3], double tol)
{
	int k, n;
	double v1[3], v2[3], q[3], d;
	VList v;
	EList e;
	FList F;
	FRelated r;

	n = IsIn(p, tol);
	if (n == 0)
	{
		for (k = 0; k < 3; k++)
		{
			e = EdgeUsed[k];
			for (r = e->FaceUsed; r; r = r->FNext)
			{
				F = r->Face;
				if (F != this)
				{
					F->Prj(p, q);
					if (F->IsIn(q, tol) > 0)
						return F;
				}
			}
		}
	}
	else
		if (n == 2) // p on edge
		{
			for (k = 0; k < 3; k++)
			{
				e = EdgeUsed[k];
				for (r = e->FaceUsed; r; r = r->FNext)
				{
					F = r->Face;
					if (F != this &&
						F->IsIn(p, tol) > 0)
						return F;
				}
			}
		}
		else
			if (n == 3) // p on vertex
			{
				for (k = 0; k < 3; k++)
				{
					v = VertexUsed[k];
					if (mathDist(p, &v->Coord.x) < tol)
					{
						for (r = v->FaceUsed; r; r = r->FNext)
						{
							F = r->Face;
							if (F != this)
							{
								if (F->VertexUsed[0] == v)
								{
									mathGetVec(p, &F->VertexUsed[1]->Coord.x, v1);
									mathGetVec(p, &F->VertexUsed[2]->Coord.x, v2);
								}
								else
									if (F->VertexUsed[1] == v)
									{
										mathGetVec(p, &F->VertexUsed[0]->Coord.x, v1);
										mathGetVec(p, &F->VertexUsed[2]->Coord.x, v2);
									}
									else
									{
										mathGetVec(p, &F->VertexUsed[0]->Coord.x, v1);
										mathGetVec(p, &F->VertexUsed[1]->Coord.x, v2);
									}
								mathVProduct(v1, dir, v1);
								mathVProduct(v2, dir, v2);
								d = mathOProduct(v1, v2);
								if (d <= 0.)
									return F;
							}
						}
					}
				}
			}
			else
				return this; // p在三角形内或外(error)

	return NULL;
}

// Check if all member of vec is val
static int checkAllEqualVal(double* vec, int n, double val)
{
	int ret = 1;
	for (int i = 0; i < n; i++) {
		if (vec[i] != val) {
			ret = 0;
			return ret;
		}
	}
	return ret;
}

BOOL GridModel::IsPointOnBoundary(STLPNT3D iPoint, FList iSupport)
{
	//if (!MTIPathOriginList::IsPointInTriangle(iPoint, iSupport))
	//	return false; // 不在三角形内
	int index = -1;

	if (IsPointAVertex(iPoint, iSupport, index))
	{
		VList vertex = iSupport->VertexUsed[index];
		for (size_t i = 0; i < vertex->FaceNum; i++)
		{
			FList face = vertex->FaceUsed[i].Face;
			for (size_t j = 0; j < 3; j++)
			{
				EList edge = face->EdgeUsed[j];
				if (edge->FaceNum == 1 && IsPointOnEdge(iPoint, edge))
				{
					return true; // 是顶点，且该顶点所在的边没有邻面
				}
			}
		}
	}
	else
	{
		for (size_t i = 0; i < 3; i++)
		{
			EList edge = iSupport->EdgeUsed[i];

			if (IsPointOnEdge(iPoint, edge))
			{
				if (edge->FaceNum == 1)
				{
					return true; // 在第i条边上，且该边没有邻面
				}
			}
		}
	}

	return false;
}

//==============================================================

FList GridModel::stlGetFacetFromNum(int N)
{
	FList f = FHead;
	for (int i = 1; i <= FaceNum; i++)
	{
		if (f->FaceNO == N)return f;
		else if (f->FNext != NULL)f = f->FNext;
	}
	return f;
}

BOOL GridModel::stlCreateSTLTriangle(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3)
{
	STLVECTOR v1, v2, nv;

	v1 = stlTwoVectorMinus(p2, p1);
	if (!v1.norm()) {
		return FALSE;
	}

	v2 = stlTwoVectorMinus(p3, p2);
	if (!v2.norm()) {
		return FALSE;
	}


	nv = stlTwoVectorProduct(v1, v2);
	if (!nv.norm()) {
		STLVECTOR v3 = stlTwoVectorMinus(p1, p3);
		if (!v3.norm()) {
			return FALSE;
		}
		nv = stlTwoVectorProduct(v3, v1);
		if (!nv.norm()) {
			return FALSE;
		}
	}

	stlCreateFace(&nv, &p1, &p2, &p3, -1);

	return TRUE;
}

FList GridModel::stlCreateFace(STLVECTOR *nv, STLPNT3D *p1, STLPNT3D *p2, STLPNT3D *p3, int tNum)
{
	FList p, temp;

	if (fabs(p1->x - p2->x) < EPS06 && fabs(p1->y - p2->y) < EPS06 && fabs(p1->z - p2->z) < EPS06 ||
		fabs(p1->x - p3->x) < EPS06 && fabs(p1->y - p3->y) < EPS06 && fabs(p1->z - p3->z) < EPS06 ||
		fabs(p3->x - p2->x) < EPS06 && fabs(p3->y - p2->y) < EPS06 && fabs(p3->z - p2->z) < EPS06) {
		return NULL;
	}

	FaceNum++;
	p = stlFaceAlloc();
	//默认p1,p2,p3,nv呈右手法则
	/*p->NormalVector.x=nv->x;
	p->NormalVector.y=nv->y;
	p->NormalVector.z=nv->z;*/
	// nt add 2022/7/9
	mathGetTriNormal(&p1->x, &p2->x, &p3->x, &p->NormalVector.x);
	if (mathOProduct(&nv->x, &p->NormalVector.x) < 0.)
	{
		p->NormalVector.x = -p->NormalVector.x;
		p->NormalVector.y = -p->NormalVector.y;
		p->NormalVector.z = -p->NormalVector.z;
	}
	// end

	if (tNum == -1)p->FaceNO = FaceNum;
	else p->FaceNO = tNum;

	p->VertexUsed[0] = stlCreateVertex(&VRoot, p1);
	p->VertexUsed[1] = stlCreateVertex(&VRoot, p2);
	p->VertexUsed[2] = stlCreateVertex(&VRoot, p3);

	p->EdgeUsed[0] = stlCreateEdge(p->VertexUsed[0], p->VertexUsed[1]);
	p->EdgeUsed[1] = stlCreateEdge(p->VertexUsed[1], p->VertexUsed[2]);
	p->EdgeUsed[2] = stlCreateEdge(p->VertexUsed[2], p->VertexUsed[0]);

	// 处理 VERTEC 上面的相邻关系
	stlAddFace2Vertex(p, p->VertexUsed[0]);
	stlAddFace2Vertex(p, p->VertexUsed[1]);
	stlAddFace2Vertex(p, p->VertexUsed[2]);

	p->EdgeUsed[0]->FaceCircleTag = 0;
	p->EdgeUsed[1]->FaceCircleTag = 0;
	p->EdgeUsed[2]->FaceCircleTag = 0;

	// 处理 EDGE 上面的相邻关系
	stlAddFace2Edge(p, p->EdgeUsed[1]);
	stlAddFace2Edge(p, p->EdgeUsed[0]);
	stlAddFace2Edge(p, p->EdgeUsed[2]);

	if (FHead == NULL) {
		FHead = p;
	}
	else {
		temp = FHead;
		FHead = p;
		FHead->FNext = temp;
	}

	return p;
}

FList GridModel::stlCreateFace(STLVECTOR *nv, STLPNT3D *p1, STLPNT3D *p2, STLPNT3D *p3, STLVECTOR *v1, STLVECTOR *v2, STLVECTOR *v3, int tNum)
{
	FList p, temp;

	if (fabs(p1->x - p2->x) < EPS06 && fabs(p1->y - p2->y) < EPS06 && fabs(p1->z - p2->z) < EPS06 ||
		fabs(p1->x - p3->x) < EPS06 && fabs(p1->y - p3->y) < EPS06 && fabs(p1->z - p3->z) < EPS06 ||
		fabs(p3->x - p2->x) < EPS06 && fabs(p3->y - p2->y) < EPS06 && fabs(p3->z - p2->z) < EPS06) {
		return NULL;
	}

	FaceNum++;
	p = stlFaceAlloc();
	//默认p1,p2,p3,nv呈右手法则
	/*p->NormalVector.x=nv->x;
	p->NormalVector.y=nv->y;
	p->NormalVector.z=nv->z;*/
	// nt add 2022/7/9
	mathGetTriNormal(&p1->x, &p2->x, &p3->x, &p->NormalVector.x);
	if (mathOProduct(&nv->x, &p->NormalVector.x) < 0.)
	{
		p->NormalVector.x = -p->NormalVector.x;
		p->NormalVector.y = -p->NormalVector.y;
		p->NormalVector.z = -p->NormalVector.z;
	}
	// end

	if (tNum == -1)p->FaceNO = FaceNum;
	else p->FaceNO = tNum;

	p->VertexUsed[0] = stlCreateVertex(&VRoot, p1, v1);
	p->VertexUsed[1] = stlCreateVertex(&VRoot, p2, v2);
	p->VertexUsed[2] = stlCreateVertex(&VRoot, p3, v3);

	p->EdgeUsed[0] = stlCreateEdge(p->VertexUsed[0], p->VertexUsed[1]);
	p->EdgeUsed[1] = stlCreateEdge(p->VertexUsed[1], p->VertexUsed[2]);
	p->EdgeUsed[2] = stlCreateEdge(p->VertexUsed[2], p->VertexUsed[0]);

	// 处理 VERTEC 上面的相邻关系
	stlAddFace2Vertex(p, p->VertexUsed[0]);
	stlAddFace2Vertex(p, p->VertexUsed[1]);
	stlAddFace2Vertex(p, p->VertexUsed[2]);

	p->EdgeUsed[0]->FaceCircleTag = 0;
	p->EdgeUsed[1]->FaceCircleTag = 0;
	p->EdgeUsed[2]->FaceCircleTag = 0;

	// 处理 EDGE 上面的相邻关系
	stlAddFace2Edge(p, p->EdgeUsed[1]);
	stlAddFace2Edge(p, p->EdgeUsed[0]);
	stlAddFace2Edge(p, p->EdgeUsed[2]);

	if (FHead == NULL) {
		FHead = p;
	}
	else {
		temp = FHead;
		FHead = p;
		FHead->FNext = temp;
	}

	return p;
}

FList GridModel::stlFaceAlloc()
{
	FList p;

	//p = (FList)malloc(sizeof(struct FaceList));
	p = new FaceList(); // nt add 2022/6/15
	if (p == NULL) {
		exit(1);
	}
	p->FCircleTag = 0.0;
	p->FaceNO = -1;
	p->FNext = NULL;
	return p;
}

// nt modify 2022/6/15
VList GridModel::stlCreateVertex(VList *root, STLPNT3D *pt)
{
	STLPNT3D q;

	q.x = /*0.001**/pt->x;
	q.y = /*0.001**/pt->y;
	q.z = /*0.001**/pt->z;

	if (q.x < stl_xmin)
		stl_xmin = (double)(q.x);
	if (q.x > stl_xmax)
		stl_xmax = (double)(q.x);
	if (q.y < stl_ymin)
		stl_ymin = (double)(q.y);
	if (q.y > stl_ymax)
		stl_ymax = (double)(q.y);
	if (q.z < stl_zmin)
		stl_zmin = (double)(q.z);
	if (q.z > stl_zmax)
		stl_zmax = (double)(q.z);

	return stlSortTree(&q, root);
}

VList GridModel::stlCreateVertex(VList *root, STLPNT3D *pt, STLVECTOR *vt)
{
	STLPNT3D q;

	q.x = /*0.001**/pt->x;
	q.y = /*0.001**/pt->y;
	q.z = /*0.001**/pt->z;

	if (q.x < stl_xmin)
		stl_xmin = (double)(q.x);
	if (q.x > stl_xmax)
		stl_xmax = (double)(q.x);
	if (q.y < stl_ymin)
		stl_ymin = (double)(q.y);
	if (q.y > stl_ymax)
		stl_ymax = (double)(q.y);
	if (q.z < stl_zmin)
		stl_zmin = (double)(q.z);
	if (q.z > stl_zmax)
		stl_zmax = (double)(q.z);

	return stlSortTree(&q, vt, root);
}

VList GridModel::stlSortTree(STLPNT3D *pt, VList *t)  //二叉排序树
{
	VList	 p, p1;  //p作为遍历节点使用,p1作为返回指针使用 
	int	   Bool;
	if (*t == NULL)  //树为空
	{
		*t = p1 = stlVertexAlloc();
		p1->Coord = *pt;
		VerNum++;
	}
	else   //树不为空
	{
		p = *t;  //p作为遍历节点使用
		Bool = 1;
		do
		{
			if (pt->z < p->Coord.z)   //如果需要插入的z值小于当前节点的z值,插入左子树
			{
				if (p->Lchild != NULL)  //左子树不空
				{
					p = p->Lchild;
				}
				else
				{
					p1 = stlVertexAlloc();
					p1->Coord = *pt;
					p->Lchild = p1;
					VerNum++;
					Bool = 0;
				}
			}
			else if (pt->z > p->Coord.z)
			{
				if (p->Rchild != NULL)
				{
					p = p->Rchild;
				}
				else
				{
					p1 = stlVertexAlloc();
					p1->Coord = *pt;
					p->Rchild = p1;
					VerNum++;
					Bool = 0;
				}
			}
			else
			{
				if (pt->x == p->Coord.x && pt->y == p->Coord.y)  //完全相同
				{
					p1 = p;
					Bool = 0;
				}
				else   //插入左子树
				{
					if (p->Lchild != NULL)
					{
						p = p->Lchild;
					}
					else
					{
						p1 = stlVertexAlloc();
						p1->Coord = *pt;
						p->Lchild = p1;
						VerNum++;
						Bool = 0;
					}
				}
			}
		} while (Bool);
	}
	return p1;
}

VList GridModel::stlSortTree(STLPNT3D *pt, STLVECTOR *vt, VList *t)
{
	VList	 p, p1;  //p作为遍历节点使用,p1作为返回指针使用 
	int	   Bool;
	if (*t == NULL)  //树为空
	{
		*t = p1 = stlVertexAlloc();
		p1->Coord = *pt;
		p1->NormalVector = *vt;
		VerNum++;
	}
	else   //树不为空
	{
		p = *t;  //p作为遍历节点使用
		Bool = 1;
		do
		{
			if (pt->z < p->Coord.z)   //如果需要插入的z值小于当前节点的z值,插入左子树
			{
				if (p->Lchild != NULL)  //左子树不空
				{
					p = p->Lchild;
				}
				else
				{
					p1 = stlVertexAlloc();
					p1->Coord = *pt;
					p->Lchild = p1;
					VerNum++;
					Bool = 0;
				}
			}
			else if (pt->z > p->Coord.z)
			{
				if (p->Rchild != NULL)
				{
					p = p->Rchild;
				}
				else
				{
					p1 = stlVertexAlloc();
					p1->Coord = *pt;
					p->Rchild = p1;
					VerNum++;
					Bool = 0;
				}
			}
			else
			{
				if (pt->x == p->Coord.x && pt->y == p->Coord.y)  //完全相同
				{
					p1 = p;
					Bool = 0;
				}
				else   //插入左子树
				{
					if (p->Lchild != NULL)
					{
						p = p->Lchild;
					}
					else
					{
						p1 = stlVertexAlloc();
						p1->Coord = *pt;
						p->Lchild = p1;
						VerNum++;
						Bool = 0;
					}
				}
			}
		} while (Bool);
	}
	return p1;
}

VList GridModel::stlVertexAlloc()
{
	VList p;
	p = (VList)malloc(sizeof(struct VertexList));
	if (p == NULL)
	{
		exit(1);
	}
	p->Lchild = NULL;
	p->Rchild = NULL;
	p->FaceUsed = NULL;
	p->FaceNum = 0;
	return p;
}
EList GridModel::stlCreateEdge(VList p1, VList p2)
{
	EList   e, e_new;
	FRelated fr1, fr2;
	int    i, j, k;
	fr1 = p1->FaceUsed;
	fr2 = p2->FaceUsed;
	if (fr1 != NULL && fr2 != NULL)
	{
		// 两个点均关联了一些面, 握手遍历
		for (i = 0; i < p1->FaceNum; i++)
		{
			fr2 = p2->FaceUsed;
			for (j = 0; j < p2->FaceNum; j++)
			{
				// 两个点关联到同一张面
				if (fr1->Face == fr2->Face) //如果两个点相关联的面相同
				{
					// 将该面的三个边取出来, 如果与 p1 p2 组成的边相同, 直接返回该边
					for (k = 0; k < 3; k++)
					{
						e = fr1->Face->EdgeUsed[k];
						if (e->VertexUsed[0] == p2 && e->VertexUsed[1] == p1)
							return e;
						else if (e->VertexUsed[0] == p1 && e->VertexUsed[1] == p2)
							return e;
					}
				}
				fr2 = fr2->FNext;
			}
			fr1 = fr1->FNext;
		}
	}
	e_new = stlEdgeAlloc();
	e_new->VertexUsed[0] = p1;
	e_new->VertexUsed[1] = p2;
	e = EHead;
	EHead = e_new;
	EHead->ENext = e;
	EdgeNum++;
	return EHead;
}

EList GridModel::stlEdgeAlloc()
{
	EList p;
	p = (EList)malloc(sizeof(struct EdgeList));
	if (p == NULL)
	{
		exit(1);
	}
	p->ENext = NULL;
	p->FaceUsed = NULL;
	p->FaceNum = 0;
	p->FaceCircleTag = 0;
	p->FCircleTag = 0.0;
	return p;
}

void GridModel::stlAddFace2Vertex(FList f, VList v)
{
	FRelated fr, fnew;
	fnew = (FRelated)malloc(sizeof(struct FaceRelated));
	if (fnew == NULL)
	{
		exit(1);
	}
	fnew->Face = f;
	if (v->FaceUsed == NULL)
	{
		v->FaceUsed = fnew;
		fnew->FNext = NULL;
	}
	else
	{
		fr = v->FaceUsed;
		v->FaceUsed = fnew;
		fnew->FNext = fr;
	}
	(v->FaceNum)++;
}

void GridModel::stlAddFace2Edge(FList f, EList e)
{
	FRelated fr, fnew;
	fnew = (FRelated)malloc(sizeof(struct FaceRelated));
	if (fnew == NULL)
	{
		exit(1);
	}
	fnew->Face = f;
	if (e->FaceUsed == NULL)
	{
		e->FaceUsed = fnew;
		fnew->FNext = NULL;
	}
	else
	{
		fr = e->FaceUsed;
		e->FaceUsed = fnew;
		fnew->FNext = fr;
	}
	(e->FaceNum)++;
}

BOOL GridModel::stlCompareTwo3DPoint(STLPNT3D p3d1, STLPNT3D p3d2, double epsdis)
{
	BOOL equaled = FALSE;
	if (fabs(p3d1.x - p3d2.x) < epsdis)
	{
		if (fabs(p3d1.y - p3d2.y) < epsdis)
		{
			if (fabs(p3d1.z - p3d2.z) < epsdis)
			{
				equaled = TRUE;
			}
		}
	}
	return equaled;
}

double GridModel::stlDistanceTwoPoints(STLPNT3D p1, STLPNT3D p2)
{
	double dd = (p1 - p2).norm();
	return dd;
}

int GridModel::stlCompareTriangleVertex(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3, STLPNT3D q1, STLPNT3D q2, STLPNT3D q3)
{
	STLPNT3D  v1, v2, v3;
	v1 = (p1 - q1);
	v2 = (p2 - q1);
	v3 = (p3 - q1);
	int  flag = 0;
	if (v1.norm() < 0.1) {
		flag++;
		v2 = (p2 - q2);
		v3 = (p3 - q3);
		if (v1.norm() < 0.1) {
			flag++;
			if (v1.norm() < 0.1) {
				flag++;
			}
		}
		else {
			v2 = (p3 - q2);
			v3 = (p2 - q3);
			if (v2.norm() < 0.1) {
				flag++;
				if (v3.norm() < 0.1) {
					flag++;
				}
			}
		}
	}
	else if (v2.norm() < 0.1) {
		flag++;
		v1 = (p1 - q2);
		v3 = (p3 - q3);
		if (v1.norm() < 0.1) {
			flag++;
			if (v3.norm() < 0.1) {
				flag++;
			}
		}
		else {
			v1 = p3 - p2;
			v3 = p1 - p3;
			if (v1.norm() < 0.1) {
				flag++;
				if (v3.norm() < 0.1) {
					flag++;
				}
			}
		}
	}
	else if (v3.norm() < 0.1) {
		flag++;
		v1 = (p1 - q2);
		v2 = (p2 - q3);
		if (v2.norm() < 0.1) {
			flag++;
			if (v1.norm() < 0.1) {
				flag++;
			}
		}
		else {
			v1 = (p1 - q3);
			v2 = (p2 - q2);
			if (v2.norm() < 0.1) {
				flag++;
				if (v1.norm() < 0.1) {
					flag++;
				}
			}
		}
	}
	return flag;
}
STLPNT3D GridModel::stlTwoVectorPlus(STLPNT3D p1, STLPNT3D p2)
{
	STLPNT3D PTemp = p1;
	PTemp.x = p1.x + p2.x;	PTemp.y = p1.y + p2.y;	PTemp.z = p1.z + p2.z;
	return PTemp;
}
STLPNT3D GridModel::stlTwoVectorMinus(STLPNT3D p1, STLPNT3D p2)
{
	STLPNT3D PTemp = p1;
	PTemp.x = p1.x - p2.x;	PTemp.y = p1.y - p2.y;	PTemp.z = p1.z - p2.z;
	return PTemp;
}
STLPNT3D GridModel::stlTwoVectorMultiply(STLPNT3D p1, double l1)
{
	STLPNT3D PTemp = p1;
	PTemp.x = p1.x * l1;	PTemp.y = p1.y * l1;	PTemp.z = p1.z * l1;
	return PTemp;
}
STLPNT3D GridModel::stlTwoVectorDivide(STLPNT3D p1, double l1)
{
	STLPNT3D PTemp = PBreak;
	if (fabs(l1) > 1.e-9)
	{
		PTemp.x = p1.x / l1;	PTemp.y = p1.y / l1;	PTemp.z = p1.z / l1;
	}
	return PTemp;
}

double GridModel::triCalArea(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3)
{
	STLPNT3D V1, V2, nv;
	V1 = stlTwoVectorMinus(p2, p1);
	V2 = stlTwoVectorMinus(p3, p2);
	nv = stlTwoVectorProduct(V1, V2);
	double  l1 = nv.norm();
	return 0.5 * l1;
}

double GridModel::CalDistofPtAndFace(STLPNT3D p0, FList f)
{
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;
	x1 = f->VertexUsed[2]->Coord.x - f->VertexUsed[1]->Coord.x;
	x2 = f->VertexUsed[1]->Coord.x - f->VertexUsed[0]->Coord.x;
	y1 = f->VertexUsed[2]->Coord.y - f->VertexUsed[1]->Coord.y;
	y2 = f->VertexUsed[1]->Coord.y - f->VertexUsed[0]->Coord.y;
	z1 = f->VertexUsed[2]->Coord.z - f->VertexUsed[1]->Coord.z;
	z2 = f->VertexUsed[1]->Coord.z - f->VertexUsed[0]->Coord.z;
	STLPNT3D A = CreateMTIPoint(x1, y1, z1);
	STLPNT3D B = CreateMTIPoint(x2, y2, z2);
	STLPNT3D n = stlTwoVectorProduct(A, B);
	x3 = p0.x - f->VertexUsed[2]->Coord.x;
	y3 = p0.y - f->VertexUsed[2]->Coord.y;
	z3 = p0.z - f->VertexUsed[2]->Coord.z;
	STLPNT3D Q = CreateMTIPoint(x3, y3, z3);
	STLPNT3D n2 = vectorNormalize(n);
	double s = fabs(n2 * Q);
	return s;
}

FList GridModel::FindClosetFace(STLPNT3D p0, double tol)
{

	FList f = FHead;
	for (int i = 1; i <= FaceNum; i++)
	{
		if (CalDistofPtAndFace(p0, f) < tol&& Is_in_triangle(p0, f, tol) == 1)
			return f;
		else if (f->FNext != NULL)f = f->FNext;
	}
	return f;
}

//判断点的投影是不是在三角形内
int GridModel::Is_in_triangle(STLPNT3D p0, FList f, double tol)
{
	double s1 = CalDistofPtAndFace(p0, f);
	VList v1, v2, v3;
	v1 = f->VertexUsed[1];
	v2 = f->VertexUsed[2];
	v3 = f->VertexUsed[0];
	STLPNT3D q1, q2, q3;
	q1 = v1->Coord;
	q2 = v2->Coord;
	q3 = v3->Coord;
	double a0, a1, a2, a3;
	a0 = triCalArea(q1, q2, q3);
	a1 = triCalArea(p0, q2, q3);
	a2 = triCalArea(q1, p0, q3);
	a3 = triCalArea(q1, q2, p0);
	if (fabs(a0 - a1 - a2 - a3) < tol)return 1;
	else return 0;
}

double cal2STLPNT3D_DIST(STLPNT3D p1, STLPNT3D p2)
{
	double s = (p1.x - p2.x) * (p1.x - p2.x)
		+ (p1.y - p2.y) * (p1.y - p2.y)
		+ (p1.z - p2.z) * (p1.z - p2.z);
	return sqrt(fabs(s));
}

STLVECTOR GridModel::CalAllVertexNormalVector(STLPNT3D p0, double tol)
{
	FList f = FindClosetFace(p0, tol);
	STLVECTOR n = f->NormalVector;
	return n;
}

int GridModel::stlDealInputFile(char* pathName)
{
	int flag = 0, rc = 999;
	FILE *file;

	char buf[300];
	errno_t err;
	err = fopen_s(&file, pathName, "rb");
	if (strstr(pathName, "stl") != NULL || 
		strstr(pathName, "STL") != NULL)
	{
		flag = 1;
		fread(buf, 1, 255, file);
		char* c;
		for (int i = 0; i < 255; i++)
		{
			c = strstr(buf + i, "facet normal");  //search substring "facet normal"
			if (c != NULL)
			{
				flag = 2;   //find "facet normal", then flag =2,stl文件为字符文件
				break;
			}
		}
	}
	else if (strstr(pathName, "dat") != NULL)
	{
		flag = 3;
	}
	fclose(file);
	if (flag != 0)
	{
		if (flag == 1)
		{
			rc = stlReadSTLBinFile(pathName); //二进制stl文件
		}
		else if (flag == 2)
		{
			rc = stlReadStlFile(pathName);   //字符stl文件
		}
		else if (flag == 3)
		{
			stlReadDatFile(pathName);		//有限元网格Dat文件
		}
	}
	return rc;
}
//void GridModel::stlReadFile(char *filename)
//{
//	int flag = 0;
//	FILE *file;
//
//	char buf[300];
//	errno_t err;
//	err = fopen_s(&file, filename, "rb");
//	int FileEx = 1;
//	if (err != 0) {
//		MessageBox(NULL, L"文件不存在，请检查输入条件。", L"提示", MB_OK);
//		FileEx = 0;
//	}
//	if (strstr(filename, "stl") != NULL)
//	{
//		flag = 1;
//		fread(buf, 1, 255, file);
//		char* c;
//		for (int i = 0; i < 255; i++)
//		{
//			c = strstr(buf + i, "facet normal");  //search substring "facet normal"
//			if (c != NULL)
//			{
//				flag = 2;   //find "facet normal", then flag =2,stl文件为字符文件
//				break;
//				file = NULL;
//			}
//		}
//	}
//	else if (strstr(filename, "dat") != NULL)
//	{
//		flag = 3;
//	}
//	fclose(file);
//	file = NULL;
//
//	if (flag != 0)
//	{
//		if (flag == 1)
//		{
//			stlReadSTLBinFile(filename); //二进制stl文件
//		}
//		else if (flag == 2)
//		{
//			stlReadStlFile(filename);   //字符stl文件
//		}
//		else if (flag == 3)
//		{
//			stlReadDatFile(filename);		//有限元网格Dat文件
//		}
//		TraverseNum = 0;
//		//GetNormaVectorOnVertex(VRoot);//获取顶点处法向 nt comment 2022/6/14
//	}
//}

int GridModel::stlReadSTLBinFile(char *filename)
{
	FILE *stlfile;

	STLVECTOR nv;
	STLPNT3D  p1, p2, p3;

	union
	{
		long int i4;
		char   c[4];
		float   r;
	} buf;

	errno_t err;

	err = fopen_s(&stlfile, filename, "rb");
	if (err != 0) {
		return err;
	}

	fseek(stlfile, 0L, SEEK_SET); //move point to the beginning
	if (stlfile == NULL)
	{
		printf("Error: STL file open failed\n");
		exit(1);
	}

	int n = 0, rc;
	char buffer[82];
	rc = (int)fread_s(buffer, 80, sizeof(char), 80, stlfile);
	if (rc != 80)
	{
		fclose(stlfile);
		return rc;
	}
	rc = (int)fread_s(&n, 4, sizeof(char), 4, stlfile);
	if (rc != 4 ||
		n < 1 ||
		n > 100000000) // nt add 2017/4/2
	{
		fclose(stlfile);
		return rc;
	}

	int NumberTotal = n;  //total number of facet
	//int step = (int)(NumberTotal / 20);

	//char str[133];
	//strcpy_s(str, "正在打开模型, 请等待");

	for (int i = 0; i < NumberTotal; i++)
	{

		stlRead4Bytes(stlfile, &buf.c[0]);  //get the vector of the facet    
		nv.x = buf.r;
		stlRead4Bytes(stlfile, &buf.c[0]);
		nv.y = buf.r;
		stlRead4Bytes(stlfile, &buf.c[0]);
		nv.z = buf.r;

		stlRead4Bytes(stlfile, &buf.c[0]);  //get the points of the facet    
		p1.x = buf.r;
		stlRead4Bytes(stlfile, &buf.c[0]);
		p1.y = buf.r;
		stlRead4Bytes(stlfile, &buf.c[0]);
		p1.z = buf.r;

		stlRead4Bytes(stlfile, &buf.c[0]);
		p2.x = buf.r;
		stlRead4Bytes(stlfile, &buf.c[0]);
		p2.y = buf.r;
		stlRead4Bytes(stlfile, &buf.c[0]);
		p2.z = buf.r;

		stlRead4Bytes(stlfile, &buf.c[0]);
		p3.x = buf.r;
		stlRead4Bytes(stlfile, &buf.c[0]);
		p3.y = buf.r;
		stlRead4Bytes(stlfile, &buf.c[0]);
		p3.z = buf.r;

		char c = getc(stlfile);
		c = getc(stlfile);

		STLPNT3D vp1, vp2;
		vp1 = CreateMTIPoint();
		vp2 = nv;
		nv = stlTwoVectorMinus(vp2, vp1);
		nv.norm();
		stlCreateFace(&nv, &p1, &p2, &p3, -1);   //生成一个三角片面,nv为法向量,p为三个顶点坐标

	}
	fclose(stlfile);
	stlfile = NULL;

	return 0;
}
//{
//
//	if (fp)
//	{
//		int i, n = 0, rc;
//		char buf[82];
//		STLTRI tri;
//
//		rc = (int)fread_s(buf, 80, sizeof(char), 80, fp);
//		if (rc != 80)
//		{
//			fclose(fp);
//			return 0;
//		}
//		rc = (int)fread_s(&n, 4, sizeof(char), 4, fp);
//		if (rc != 4 ||
//			n < 1 ||
//			n > 100000000) // nt add 2017/4/2
//		{
//			fclose(fp);
//			return 0;
//		}
//		for (i = 0; i < n; i++)
//		{
//			rc = stltriLoadBinary(&tri, fp); // 循环读入STLTRI
//			if (rc == 1)
//			{
//				if (stlAddTri(pSTL, &tri) != 1) // 加入
//				{
//					fclose(fp);
//					return 0;
//				}
//			}
//			else
//				break;
//		}
//		fclose(fp);
//		if (rc == 0)
//			return 0;
//		else
//			return 1;
//	}
//	else
//		return 0;
//}
void GridModel::stlRead4Bytes(FILE *stlfile, char *c)
{
	c[0] = getc(stlfile);
	c[1] = getc(stlfile);
	c[2] = getc(stlfile);
	c[3] = getc(stlfile);
}
int GridModel::stlReadStlFile(char *file)
{
	FILE		*stlfile;
	STLPNT3D  p1, p2, p3, nv;
	STLVECTOR v1 = { 0, 0, 0 }, v2 = { 0, 0, 0 }, v3 = { 0, 0, 0 };
	int			tttNum;
	char    str[256]; // nt modify 2022/7/8 old 80
	char    *ret, *ip;

	int rc = fopen_s(&stlfile, file, "r");
	if (rc != 0) {
		printf("Error: STL file open failed\n");
		exit(1);
	}
	MeshResulttag = 0;
label:
	ret = fgets(str, 256, stlfile);
	if (ret != NULL) {
		if (strstr(str, "##") != NULL)
		{
			//ip = strstr(str,"%%");
			sscanf_s(str, "%s", &MeshResult[MeshResulttag], 256);
			MeshResulttag++;
		}
		if (strstr(str, "facet normal") != NULL) {
			ip = strstr(str, "normal") + 6;
			sscanf_s(ip, "%le %le %le", &nv.x, &nv.y, &nv.z);
			if (strstr(str, "FaceNum = ") != NULL)
			{
				ip = strstr(str, "FaceNum = ") + 10;
				sscanf_s(ip, "%d", &tttNum);
			}
			else
				tttNum = -1;

			while (1)
			{
				ret = fgets(str, 256, stlfile);
				if (strstr(str, "outer loop") != NULL) // nt add 2022/7/8
					break;
			}
			ret = fgets(str, 256, stlfile);
			ip = strstr(str, "vertex") + 6;
			sscanf_s(ip, "%le %le %le", &p1.x, &p1.y, &p1.z);
			ip = strstr(str, "V_Normal");
			if (ip)
			{
				ip += 8;
				sscanf_s(ip, "%le %le %le", &v1.x, &v1.y, &v1.z);
			}

			ret = fgets(str, 256, stlfile);
			ip = strstr(str, "vertex") + 6;
			sscanf_s(ip, "%le %le %le", &p2.x, &p2.y, &p2.z);
			ip = strstr(str, "V_Normal");
			if (ip)
			{
				ip += 8;
				sscanf_s(ip, "%le %le %le", &v2.x, &v2.y, &v2.z);
			}

			ret = fgets(str, 256, stlfile);
			ip = strstr(str, "vertex") + 6;
			sscanf_s(ip, "%le %le %le", &p3.x, &p3.y, &p3.z);
			ip = strstr(str, "V_Normal");
			if (ip)
			{
				ip += 8;
				sscanf_s(ip, "%le %le %le", &v3.x, &v3.y, &v3.z);
			}

			stlCreateFace(&nv, &p1, &p2, &p3, &v1, &v2, &v3, tttNum);
			goto label;
		}
		else if (strstr(str, "endsolid") != NULL) {
			fclose(stlfile);
			stlfile = NULL;
			return 0;
		}
		else {
			goto label;
		}
	}
	stlfile = NULL;
	return 1;
}

void GridModel::stlReadDatFile(char *file)
{
	FILE		*datfile;
	STLPNT3D  p1, p2, p3, nv;
	char    str[80];
	char    *ret, *ip;
	int     temp[5], temp1;
	STLPNT3D *PTemp;
	int i = 1;

	//获取点数,确定点数组大小
	if (fopen_s(&datfile, file, "r") != 0) {
		printf("Error: dat file open failed\n");
		exit(1);
	}
label2:
	ret = fgets(str, 80, datfile);
	if (ret != NULL) {
		if (strstr(str, "GRID*") != NULL) {
			ret = fgets(str, 80, datfile);
			ret = fgets(str, 80, datfile);
			i++;
			goto label2;
		}
		else if (strstr(str, "ELEMENTS") != NULL)
			fclose(datfile);
		else
			goto label2;
	}

	fclose(datfile);

	PTemp = new STLPNT3D[i];

	//重新打开文档,开始拓扑关系建立
	if (fopen_s(&datfile, file, "r") != 0) {
		printf("Error: dat file open failed\n");
		exit(1);
	}

	i = 1;
label1:
	ret = fgets(str, 80, datfile);
	if (ret != NULL) {
		if (strstr(str, "GRID*") != NULL) {

			ip = strstr(str, "GRID*") + 5;
			sscanf_s(ip, "%d %le %le", &temp1, &p1.x, &p1.y);
			ret = fgets(str, 80, datfile);
			ret = fgets(str, 80, datfile);
			sscanf_s(ip, "%d %le", &temp1, &p1.z);
			PTemp[i] = p1;

			i++;

			goto label1;

		}
		else if (strstr(str, "ELEMENTS") != NULL)
			goto labell;
		else
			goto label1;
	}

labell:
	ret = fgets(str, 80, datfile);
	if (ret != NULL)
	{
		if (strstr(str, "CTRIA3") != NULL)//从面开始拓扑入手
		{
			ip = strstr(str, "CTRIA3") + 6;
			sscanf_s(ip, "%d %d %d %d %d", &temp[0], &temp[1], &temp[2], &temp[3], &temp[4]);

			p1 = PTemp[temp[2]];
			p2 = PTemp[temp[3]];
			p3 = PTemp[temp[4]];

			STLPNT3D l1, l2;
			l1 = (p1 - p2);
			l2 = (p1 - p3);
			nv = (l1 ^ l2);
			nv.norm();
			nv = stlOPPNormalVector(nv);
			stlCreateFace(&nv, &p1, &p2, &p3, -1);
			goto labell;
		}
		else if (strstr(str, "ENDDATA") != NULL) {
			fclose(datfile);

			delete PTemp;
			PTemp = NULL;

			return;
		}
		else {
			goto labell;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Function: 计算两向量叉积
// Input:		 L1  L2  两向量
// Return:  叉积
///////////////////////////////////////////////////////////////////////////////////////
STLPNT3D  GridModel::stlTwoVectorProduct(STLPNT3D L1, STLPNT3D L2)
{
	STLPNT3D P = PBreak;	// ?
	P.x = L1.y * L2.z - L1.z * L2.y;
	P.y = L1.z * L2.x - L1.x * L2.z;
	P.z = L1.x * L2.y - L1.y * L2.x;
	return P;
}
STLPNT3D GridModel::stlOPPNormalVector(STLPNT3D p1)
{
	p1 = stlTwoVectorMinus(CreateMTIPoint(), p1);
	return p1;
}
STLPNT3D GridModel::CreateMTIPoint()//创建默认点
{
	STLPNT3D pTemp;
	pTemp = CreateMTIPoint(0., 0., 0.);
	return pTemp;
}
STLPNT3D GridModel::CreateMTIPoint(double ix, double iy, double iz)//从xyz创点
{
	STLPNT3D pTemp;
	pTemp.x = ix;	pTemp.y = iy;	pTemp.z = iz;
	return pTemp;
}
STLPNT3D GridModel::CreateMTIPoint(double p1[])//从数组创点
{
	STLPNT3D pTemp;
	pTemp = CreateMTIPoint(p1[0], p1[1], p1[2]);
	return pTemp;
}
STLPNT3D GridModel::CreateMTIPoint(double p1[], int iNbStart)//从数组创点
{
	STLPNT3D pTemp;
	pTemp = CreateMTIPoint(p1[iNbStart + 0], p1[iNbStart + 1], p1[iNbStart + 2]);
	return pTemp;
}

STLVECTOR vectorNormalize(STLVECTOR& iVector)
{
	STLVECTOR vector_normal = { 0. };
	double norm = iVector.x * iVector.x + iVector.y * iVector.y + iVector.z * iVector.z;
	if (norm <= TOLLENGTH)
		return vector_normal; // too short!
	if (fabs(norm - 1.) < TOLANGLE)
		return iVector; // 已是单位向量，不需要单位化
	norm = 1. / sqrt(norm);
	vector_normal.x = iVector.x * norm;
	vector_normal.y = iVector.y * norm;
	vector_normal.z = iVector.z * norm;

	return vector_normal;
}

STLVECTOR operator-(STLPNT3D& iEnd, STLPNT3D& iBegin)
{
	STLVECTOR v;
	v.x = iEnd.x - iBegin.x;
	v.y = iEnd.y - iBegin.y;
	v.z = iEnd.z - iBegin.z;
	return v;
}

STLVECTOR operator-(STLVECTOR& iVector)
{
	STLVECTOR oVector;
	oVector.x = -iVector.x;
	oVector.y = -iVector.y;
	oVector.z = -iVector.z;
	return oVector;
}

STLVECTOR operator+(STLVECTOR& iVector1, STLVECTOR& iVector2)
{
	STLVECTOR v;
	v.x = iVector1.x + iVector2.x;
	v.y = iVector1.y + iVector2.y;
	v.z = iVector1.z + iVector2.z;
	return v;
}


STLVECTOR operator*(double iScalar, STLVECTOR& iVector)
{
	STLVECTOR v;
	v.x = iScalar * iVector.x;
	v.y = iScalar * iVector.y;
	v.z = iScalar * iVector.z;
	return v;
}

double operator*(STLVECTOR& iVector1, STLVECTOR& iVector2)
{
	double res = 0.;
	res = iVector1.x * iVector2.x + iVector1.y * iVector2.y + iVector1.z * iVector2.z;
	return res;
}

STLVECTOR operator^(STLVECTOR& iVectorU, STLVECTOR& iVectorV)
{
	VEC3D v1, v2, rtvec;
	memcpy(v1, &iVectorU, sizeof(PNT3D));
	memcpy(v2, &iVectorV, sizeof(PNT3D));

	mathVProduct(v1, v2, rtvec);

	STLVECTOR vector_w;
	memcpy(&vector_w, rtvec, sizeof(PNT3D));

	return vector_w;
}

// 线段与平面求交
// smf add 2022/9/06
int mathSegmentIntPln(PNT3D iPntOfSegment1, PNT3D iPntOfSegment2,  // 线段的两个端点
	PNT3D iPntOnPlane, VEC3D iNormPlane,  // 平面的法矢及平面上一点
	double iTolLength, double iTolAngle,  // 长度容差及角度容差
	PNT3D* oPntsOfIntersection) // 交点数组的指针
{
	int num_intersection_pnts = -1; // 初始化

	double distToPlane1 = mathDistPntPlnSide(iPntOfSegment1, iPntOnPlane, iNormPlane);
	double distToPlane2 = mathDistPntPlnSide(iPntOfSegment2, iPntOnPlane, iNormPlane);

	if (distToPlane1 * distToPlane2 > TOLSQUARELENGTH) // 线段在平面的一侧，无交
	{
		num_intersection_pnts = 0;
		return num_intersection_pnts;
	}

	if (fabs(distToPlane1) < TOLLENGTH &&
		fabs(distToPlane2) < TOLLENGTH) // 线段在平面上，两个交点
	{
		num_intersection_pnts = 2;
		memcpy(oPntsOfIntersection[0], iPntOfSegment1, sizeof(PNT3D));
		memcpy(oPntsOfIntersection[1], iPntOfSegment2, sizeof(PNT3D));
		return num_intersection_pnts;
	}

	PNT3D p;
	VEC3D v, v1;
	if (mathGetVecUnit(iPntOfSegment1, iPntOfSegment2, v1) == 0)
	{
		num_intersection_pnts = 0; // 两点十分靠近，0个交点？？？
		return num_intersection_pnts;
	}
	double acos_alpha = 0.;
	double dist = 0.;
	acos_alpha = mathOProduct(v1, iNormPlane);
	if (acos_alpha < 0) // 线段方向向量与平面法线夹角，并判断是否同向
	{
		v1[0] *= -1;
		v1[1] *= -1;
		v1[2] *= -1;
		memcpy(p, iPntOfSegment1, sizeof(PNT3D));
		dist = distToPlane1;
	}
	else
	{
		memcpy(p, iPntOfSegment2, sizeof(PNT3D));
		dist = distToPlane2;
	}
	memcpy(v, v1, sizeof(VEC3D));

	num_intersection_pnts = 1;

	acos_alpha = fabs(acos_alpha);
	double tmp = dist / acos_alpha;
	for (size_t i = 0; i < 3; i++)
	{
		oPntsOfIntersection[0][i] = p[i] - v[i] * tmp; // 计算线-面交点坐标
	}

	return num_intersection_pnts;
}

int mathPlnIntTri(
	PNT3D iPntOfTri1, PNT3D iPntOfTri2, PNT3D iPntOfTri3, // 三角形的三个顶点
	PNT3D iPntOnPlane, VEC3D iNormPlane,  // 平面上一点及平面的法矢
	double iTolLength, double iTolAngle,  // 长度容差及角度容差
	PNT3D* oPntsOfIntersection, // 交点数组的指针
	int* oIndexOfIntEdge, int& oNumOfIntEdge) // 与平面相交的边的序号及数量
{
	for (size_t i = 0; i < 3; i++)
	{
		oIndexOfIntEdge[i] = -1;
	}
	oNumOfIntEdge = 0;
	int num_intersection_pnts = 0; // 初始化

	PNT3D iPntsOfTri[3];
	memcpy(iPntsOfTri[0], iPntOfTri1, sizeof(PNT3D));
	memcpy(iPntsOfTri[1], iPntOfTri2, sizeof(PNT3D));
	memcpy(iPntsOfTri[2], iPntOfTri3, sizeof(PNT3D));

	double distToPlane[3];
	for (size_t i = 0; i < 3; i++)
		distToPlane[i] = mathDistPntPlnSide(iPntsOfTri[i], iPntOnPlane, iNormPlane);

	// 判断三点是否都在平面上
	if (fabs(distToPlane[0]) <= iTolLength &&
		fabs(distToPlane[1]) <= iTolLength &&
		fabs(distToPlane[2]) <= iTolLength)
	{
		num_intersection_pnts = 3; // 三点都在平面上
		memcpy(oPntsOfIntersection[0], iPntOfTri1, sizeof(PNT3D));
		oIndexOfIntEdge[0] = 0;
		memcpy(oPntsOfIntersection[1], iPntOfTri2, sizeof(PNT3D));
		oIndexOfIntEdge[0] = 1;
		memcpy(oPntsOfIntersection[2], iPntOfTri3, sizeof(PNT3D));
		oIndexOfIntEdge[0] = 2;
		return num_intersection_pnts;
	}

	// 判断三点是否在平面同侧
	if ((distToPlane[0] < -iTolLength &&
		distToPlane[1] < -iTolLength &&
		distToPlane[2] < -iTolLength) ||
		(distToPlane[0] > iTolLength &&
			distToPlane[1] > iTolLength &&
			distToPlane[2] > iTolLength))
	{
		num_intersection_pnts = 0; // 三点在平面同侧，无交
		return num_intersection_pnts;
	}
	for (int i = 0; i < 3; i++)
	{
		VEC3D v;
		int num_int_pnts = 0;
		if (num_int_pnts = mathGetVecUnit(iPntsOfTri[i], iPntsOfTri[(i + 1) % 3], v) == 0) // 线段长度过短
		{
			continue;
		}

		PNT3D int_pnts[2] = {};
		// 线段(三角形的边)与平面求交
		num_int_pnts = mathSegmentIntPln(iPntsOfTri[i], iPntsOfTri[(i + 1) % 3],
			iPntOnPlane, iNormPlane,  // 平面上一点及平面的法矢
			iTolLength, iTolAngle,  // 长度容差及角度容差
			int_pnts); // 交点数组

//检查是否有重合的交点，去重
		for (size_t j = 0; j < num_int_pnts; j++)
		{
			// 只要有交，就保留边序号
			oIndexOfIntEdge[oNumOfIntEdge] = i;
			oNumOfIntEdge++;
			if (num_intersection_pnts == 0)
			{
				memcpy(oPntsOfIntersection[0], int_pnts[j], sizeof(PNT3D));
				num_intersection_pnts++;
			}
			else
			{
				int k = 0;
				for (k = 0; k < num_intersection_pnts; k++)
				{
					if (mathSquareDist(int_pnts[j], oPntsOfIntersection[k]) <= TOLSQUARELENGTH) // 是重合点
					{
						k = 0;
						break;
					}
				}
				if (k == num_intersection_pnts) // 不是重合点
				{
					// 保存不重合的交点
					memcpy(oPntsOfIntersection[num_intersection_pnts], int_pnts[j], sizeof(PNT3D));
					num_intersection_pnts++;
				}
			}
		}
	}
	return num_intersection_pnts;
}

int mathPlnIntTri(
	STLPNT3D iSTLPntOfTri1, STLPNT3D iSTLPntOfTri2, STLPNT3D iSTLPntOfTri3, // 三角形的三个顶点
	STLPNT3D iSTLPntOnPlane, STLVECTOR iSTLNormPlane,  // 平面的法矢及平面上一点
	double iTolLength, double iTolAngle,  // 长度容差及角度容差
	STLPNT3D* oSTLPntsOfIntersection, // 交点数组的指针
	int* oIndexOfIntEdge, int& oNumOfIntEdge) // 与平面相交的边的序号及数量
{
	PNT3D iPntOfTri1, iPntOfTri2, iPntOfTri3, // 三角形的三个顶点
		iPntOnPlane;
	VEC3D iNormPlane;  // 平面的法矢及平面上一点
	PNT3D oPntsOfIntersection[3];

	memcpy(iPntOfTri1, &iSTLPntOfTri1, sizeof(PNT3D));
	memcpy(iPntOfTri2, &iSTLPntOfTri2, sizeof(PNT3D));
	memcpy(iPntOfTri3, &iSTLPntOfTri3, sizeof(PNT3D));
	memcpy(iPntOnPlane, &iSTLPntOnPlane, sizeof(PNT3D));
	memcpy(iNormPlane, &iSTLNormPlane, sizeof(PNT3D));
	int res = mathPlnIntTri(
		iPntOfTri1, iPntOfTri2, iPntOfTri3, // 三角形的三个顶点
		iPntOnPlane, iNormPlane,  // 平面的法矢及平面上一点
		iTolLength, iTolAngle,  // 长度容差及角度容差
		oPntsOfIntersection,  // 交点数组的指针
		oIndexOfIntEdge, oNumOfIntEdge); // 与平面相交的边的序号
	memcpy(oSTLPntsOfIntersection, oPntsOfIntersection, 3 * sizeof(PNT3D));
	return res;
}

int mathPlnIntTri(
	FList iTriangle, // 三角形
	STLPNT3D iPntOnPlane, STLVECTOR iNormPlane,  // 平面的法矢及平面上一点
	double iTolLength, double iTolAngle,  // 长度容差及角度容差
	STLPNT3D* oPntsOfIntersection,  // 交点数组的指针
	int* oIndexOfIntEdge, int& oNumOfIntEdge) // 与平面相交的边的序号及数量
{
	return mathPlnIntTri(
		iTriangle->VertexUsed[0]->Coord, iTriangle->VertexUsed[1]->Coord, iTriangle->VertexUsed[2]->Coord, // 三角形的三个顶点
		iPntOnPlane, iNormPlane,  // 平面的法矢及平面上一点
		iTolLength, iTolAngle,  // 长度容差及角度容差
		oPntsOfIntersection,  // 交点数组的指针
		oIndexOfIntEdge, oNumOfIntEdge);// 与平面相交的边的序号
}

double mathSquareDist(PNT3D p1, PNT3D p2)
{
	double squareDist = 0.;
	for (size_t i = 0; i < 3; i++)
	{
		double delta = p1[i] - p2[i];
		squareDist += delta * delta;
	}
	return squareDist;
}

double mathSquareDist(STLPNT3D p1, STLPNT3D p2)
{
	double squareDist = 0.;
	double delta_x = p1.x - p2.x;
	double delta_y = p1.y - p2.y;
	double delta_z = p1.z - p2.z;
	squareDist = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
	return squareDist;
}

// 2022/12/01 smf add 
// 缩短或延伸一条线段。iDist可为负值，iDist < 0 缩短；iDist > 0 延伸。
// iBeginOrEnd: 0-起点；1-终点。
// 返回值：0-线段过短，不可缩短；1-成功。
int mathShortenOrLengthenSegmByDist(STLPNT3D &ioBegin, STLPNT3D &ioEnd, int iBeginOrEnd, double iDist)
{
	double length = mathDist(ioBegin, ioEnd);
	if (iDist < 0 && length < fabs(iDist))
	{
		return 0; // 线段过短。
	}

	STLVECTOR segm_uni = vectorNormalize(ioEnd - ioBegin);

	if (iBeginOrEnd) // 从终点
	{
		ioEnd = ioEnd + iDist * segm_uni;
	}
	else // 从起点
	{
		ioBegin = ioBegin - iDist * segm_uni;
	}

	return 1;
}

BOOL mathIsCoincidentPoint(STLPNT3D iPoint1, STLPNT3D iPoint2)
{
	double d = mathSquareDist(iPoint1, iPoint2);
	if (d <= TOLSQUARELENGTH)
		return true;
	else
		return false;
}

double mathDist(STLPNT3D p1, STLPNT3D p2)
{
	double dist = 0.;
	double delta_x = p1.x - p2.x;
	double delta_y = p1.y - p2.y;
	double delta_z = p1.z - p2.z;
	dist = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
	dist = sqrt(dist);
	return dist;
}

void mathPrjPntPln(STLPNT3D iPoint, STLPNT3D iPointOfPlane, STLVECTOR iNormalOfPlane, STLPNT3D& oProjectPoint)
{
	PNT3D p, pivot, prj_p;
	VEC3D normal;
	memcpy(p, &iPoint, 3 * sizeof(double));
	memcpy(pivot, &iPointOfPlane, 3 * sizeof(double));
	memcpy(normal, &iNormalOfPlane, 3 * sizeof(double));

	mathPrjPntPln(p, pivot, normal, prj_p);

	memcpy(&oProjectPoint, prj_p, 3 * sizeof(double));
}

void mathPrjVecPln(STLVECTOR iVector, STLVECTOR iNormal, STLVECTOR &oPrjVec)
{
	VEC3D v, normal, prj_v;

	memcpy(v, &iVector, sizeof(STLVECTOR));
	memcpy(normal, &iNormal, sizeof(STLVECTOR));

	mathPrjVecPln(v, normal, prj_v);

	memcpy(&oPrjVec, prj_v, sizeof(STLVECTOR));
	oPrjVec = vectorNormalize(oPrjVec);
}

/*  1997/12/6 nt
 *  直线与圆柱求交
 *	返回IDNOINT,无交点;返回IDTAN,切于一点;返回IDINT,交于两点;返回IDOVERL,重合
 *	nt modify 2003/2/18
 */
int mathIntLinCyl(PNT3D begin,
	VEC3D dir,
	PNT3D pivot,
	VEC3D axis,
	double r,
	double tol,
	double ang,
	PNT3D intpt1,
	PNT3D intpt2)
{
	int rt_value = IDTAN, k;
	double d, h, c;
	PNT3D localBegin;
	VEC3D e1, e2, localDir;

	mathGetXYFromZ(axis, e1, e2);
	mathTransWorldPnt3DByOXYZ(pivot, e1, e2, axis, begin, localBegin);
	mathTransWorldVec3DByXYZ(e1, e2, axis, dir, localDir);
	d = sqrt(localDir[0] * localDir[0] + localDir[1] * localDir[1]);
	if (d < ang) // paralell to cylinder axis
	{
		d = sqrt(localBegin[0] * localBegin[0] + localBegin[1] * localBegin[1]);
		if (r - tol < d && d < r + tol)
			return IDOVERL;
		else
			return IDNOINT;
	}
	else
	{
		h = fabs(localBegin[0] * localDir[1] - localBegin[1] * localDir[0]) / d;
		if (h > r + tol) // no intersection
			return IDNOINT;
		c = -(localBegin[0] * localDir[0] + localBegin[1] * localDir[1]) / d;
		intpt1[0] = localBegin[0] + localDir[0] * c / d; // origin project to line 2d
		intpt1[1] = localBegin[1] + localDir[1] * c / d; // nt add 2003/2/18 : d!!!
		intpt1[2] = localBegin[2];
		memcpy(intpt2, intpt1, sizeof(PNT3D));
		k = fabs(localDir[0]) > fabs(localDir[1]) ? 0 : 1;
		if (h < r - tol) // two intersection points,otherwise tangential contact
		{
			d = sqrt(r*r - h * h) / d;
			intpt1[0] -= (d*localDir[0]); // intersect point of circle/line 2d
			intpt1[1] -= (d*localDir[1]);
			intpt2[0] += (d*localDir[0]); // intersect point of circle/line 2d
			intpt2[1] += (d*localDir[1]);
			rt_value = IDINT;
		}
		intpt1[2] += (localDir[2] * (intpt1[k] - localBegin[k]) / localDir[k]);
		intpt2[2] += (localDir[2] * (intpt2[k] - localBegin[k]) / localDir[k]);
		mathTransLocalPnt3DByOXYZ(pivot, e1, e2, axis, intpt1, intpt1);
		mathTransLocalPnt3DByOXYZ(pivot, e1, e2, axis, intpt2, intpt2);
		return rt_value;
	}
}

/*  2002/3/8 nt
 *  transform world_vector 3D to local_vector 3D which is defined
 *	in local_frame
 */
void mathTransWorldVec3DByXYZ(VEC3D e1,
	VEC3D e2,
	VEC3D e3,
	VEC3D world_vector,
	VEC3D local_vector)
{
	int k;
	VEC3D v;
	double local_invse[9];

	for (k = 0; k < 3; k++)
	{
		local_invse[3 * k] = e1[k];
		local_invse[3 * k + 1] = e2[k];
		local_invse[3 * k + 2] = e3[k];
	}
	for (k = 0; k < 3; k++)
		v[k] = world_vector[0] * local_invse[k] +
		world_vector[1] * local_invse[k + 3] +
		world_vector[2] * local_invse[k + 6];
	local_vector[0] = v[0];
	local_vector[1] = v[1];
	local_vector[2] = v[2];
}

/*  2002/3/7 nt
 *	transform local_point 3D to world_point 3D which is defined in
 *	local frame with origin,X,Y,Z as it's origin,X,Y,Z axis
 *	nt modify 2003/2/16
 */
void mathTransLocalPnt3DByOXYZ(PNT3D origin,
	VEC3D e1,
	VEC3D e2,
	VEC3D e3,
	PNT3D local_point,
	PNT3D world_point)
{
	PNT3D p;

	p[0] = origin[0] + local_point[0] * e1[0] + local_point[1] * e2[0] + local_point[2] * e3[0];
	p[1] = origin[1] + local_point[0] * e1[1] + local_point[1] * e2[1] + local_point[2] * e3[1];
	p[2] = origin[2] + local_point[0] * e1[2] + local_point[1] * e2[2] + local_point[2] * e3[2];
	world_point[0] = p[0];
	world_point[1] = p[1];
	world_point[2] = p[2];

	return;
}

/*  2002/3/28 nt
 *  calculate the intersection of cylinder/segment, consider the overlaping case
 *	return IDINT, IDNOINT
 *	t1, t2 are in [0.0, 1.0]. if begin coincident with p1(p2), t1(t2) is 0.0, if
 *	end is conincident with p1(p2), t1(t2) is 1.0
 */
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
	double* pt2)
{
	int rt_value;
	double d;
	VEC3D dir;

	// 1.
	*pn = 0;

	// 2.
	dir[0] = end[0] - begin[0];
	dir[1] = end[1] - begin[1];
	dir[2] = end[2] - begin[2];
	if ((d = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2])) < MIN_DBL)
	{
		mathMidPnt(begin, end, p1);
		if (fabs(mathDistPntLin(p1, pivot, axis) - r) < tol)
		{
			*pt1 = 0.5;
			*pn = 1;
			return IDINT;
		}
		return IDNOINT;
	}
	dir[0] /= d;
	dir[1] /= d;
	dir[2] /= d;

	// 3.
	rt_value = mathIntLinCyl(begin, dir, pivot, axis, r, tol, MIN_ANG, p1, p2);
	if (rt_value == IDINT || rt_value == IDTAN || rt_value == IDOVERL)
	{
		if ((d = mathGetPntSegmNDT(p1, begin, end, pt1)) < tol)
			*pn = 1;
	}
	if (rt_value == IDINT)
	{
		if ((d = mathGetPntSegmNDT(p2, begin, end, pt2)) < tol)
		{
			(*pn)++;
			if (*pn == 1)
			{
				memcpy(p1, p2, sizeof(PNT3D));
				*pt1 = *pt2;
			}
		}
	}

	return *pn > 0 ? IDINT : IDNOINT;
}

// smf add 2023/02/24
// 共面线段的交点计算
int mathSegmIntSegmInFace(STLPNT3D iBegin1, STLPNT3D iEnd1, STLPNT3D iBegin2, STLPNT3D iEnd2, double iTol, STLPNT3D& oIntPoint)
{
	STLVECTOR v1, v2, v_b1_b2;
	v1 = iEnd1 - iBegin1;
	v2 = iEnd2 - iBegin2;
	v_b1_b2 = iBegin2 - iBegin1;

	STLVECTOR unit_v1, unit_v2;
	unit_v1 = vectorNormalize(v1);
	unit_v2 = vectorNormalize(v2);

	double theta1 = unit_v1 * unit_v2;
	if (fabs(theta1 - 1) < TOLANGLE ||
		fabs(theta1 + 1) < TOLANGLE)
	{
		// 平行或共线

		// 判断是否在线段上
		if (mathIsPointInSegm(iBegin1, iBegin2, iEnd2, iTol) &&
			mathIsPointInSegm(iEnd1, iBegin2, iEnd2, iTol))
		{
			// 两点都在线段内
			return 2;
		}
	}

	double det_xy, det_yz, det_xz;

	det_xy = mathDet22(v1.x, -v2.x, v1.y, -v2.y);
	det_yz = mathDet22(v1.y, -v2.y, v1.z, -v2.z);
	det_xz = mathDet22(v1.x, -v2.x, v1.z, -v2.z);

	double t1 = -1000, t2 = -1000;
	if (det_xy < -iTol ||
		det_xy > iTol)
	{
		double det_1 = mathDet22(v_b1_b2.x, -v2.x, v_b1_b2.y, -v2.y);
		t1 = det_1 / det_xy;
		double det_2 = mathDet22(v1.x, v_b1_b2.x, v1.y, v_b1_b2.y);
		t2 = det_2 / det_xy;
	}
	else
		if (det_yz < -iTol ||
			det_yz > iTol)
		{
			double det_1 = mathDet22(v_b1_b2.y, -v2.y, v_b1_b2.z, -v2.z);
			t1 = det_1 / det_yz;
			double det_2 = mathDet22(v1.y, v_b1_b2.y, v1.z, v_b1_b2.z);
			t2 = det_2 / det_yz;
		}
		else
			if (det_xz < -iTol ||
				det_xz > iTol)
			{
				double det_1 = mathDet22(v_b1_b2.x, -v2.x, v_b1_b2.z, -v2.z);
				t1 = det_1 / det_xz;
				double det_2 = mathDet22(v1.x, v_b1_b2.x, v1.z, v_b1_b2.z);
				t2 = det_2 / det_xz;
			}

	if (t1 >= 0. && t1 <= 1. &&
		t2 >= 0. && t2 <= 1.) // 交点在线段上
	{
		oIntPoint = iBegin1 + t1 * v1;
		return IDINT;
	}
	return IDNOINT;
}

BOOL mathIsPointInSegm(STLPNT3D iPoint, STLPNT3D iBegin, STLPNT3D iEnd, double iTol)
{
	double bp, pe, be;
	bp = mathDist(iBegin, iPoint);
	pe = mathDist(iPoint, iEnd);
	be = mathDist(iBegin, iEnd);
	if (fabs(bp + pe - be) <= iTol)
	{
		return true;
	}
	return false;
}

STLPNT3D mathMidPoint(STLPNT3D iPoint1, STLPNT3D iPoint2)
{
	STLPNT3D mid_point;
	mid_point.x = 0.5 * (iPoint1.x + iPoint2.x);
	mid_point.y = 0.5 * (iPoint1.y + iPoint2.y);
	mid_point.z = 0.5 * (iPoint1.z + iPoint2.z);
	return mid_point;
}

BOOLEAN mathIntSegmCyl(
	STLVECTOR iSegmBegin, STLVECTOR iSegmEnd,	//线段的两个端点
	STLVECTOR iCylBegin, STLVECTOR iCylEnd,		// 圆柱轴线的起止点
	double iRadius,								// 圆柱半径
	double iTol,								// 容差 
	int &oNumIntPnts,							// 交点个数
	STLPNT3D *oIntPnts)						// 交点坐标
{
	PNT3D segm_begin, segm_end, cyl_begin, cyl_end, p1, p2;
	memcpy(segm_begin, &iSegmBegin, sizeof(STLPNT3D));
	memcpy(segm_end, &iSegmEnd, sizeof(STLPNT3D));
	memcpy(cyl_begin, &iCylBegin, sizeof(STLPNT3D));
	memcpy(cyl_end, &iCylEnd, sizeof(STLPNT3D));
	VEC3D axis;
	mathGetVecUnit(cyl_begin, cyl_end, axis);
	double pt1, pt2;
	int pn;
	oNumIntPnts = 0;
	if (mathIntSegmCyl(segm_begin, segm_end, cyl_begin, axis, iRadius, iTol, &pn, p1, p2, &pt1, &pt2) == IDINT)
	{
		if (!mathIsPointOnCylinderExtension(p1, cyl_begin, cyl_end, iRadius))
		{
			memcpy(&oIntPnts[oNumIntPnts], p1, sizeof(STLPNT3D));
			oNumIntPnts++;
		}
		if (pn == 2)
		{
			if (!mathIsPointOnCylinderExtension(p2, cyl_begin, cyl_end, iRadius))
			{
				memcpy(&oIntPnts[oNumIntPnts], p2, sizeof(STLPNT3D));
				oNumIntPnts++;
			}
		}
	}
	return (oNumIntPnts > 0) ? 1 : 0;
}

BOOLEAN mathIsPointOnCylinderExtension(PNT3D iPoint, PNT3D iCylBegin, PNT3D iCylEnd, double iRadius)
{
	VEC3D v1, v2;
	mathGetVecUnit(iCylBegin, iPoint, v1);
	mathGetVecUnit(iCylEnd, iPoint, v2);

	if (mathOProduct(v1, v2) > 0)
		return false;
	else
		return true;
}


// smf add 2022/12/10
// 根据三角形顶点的信息，找到三角形边上值等于给定值的一点
// ioIndex-等值顶点的索引
// 应该只返回0，1或2：
// 0-三角形内部没有等值点；
// 1-三角形的一个顶点为等值点，需遍历该顶点周围的所有三角形
// 2-三角形边上有两个等值点
int mathCalPointsByTriangleVertexInfo(STLPNT3D iVertex1, STLPNT3D iVertex2, STLPNT3D iVertex3, double iInfo[3], double iRef, double iTol, STLPNT3D ioPoint[3], int* ioIndex)
{
	ioIndex[0] = -1;
	ioIndex[1] = -1;
	ioIndex[2] = -1;
	STLPNT3D iVertex[3];
	int t = sizeof(STLPNT3D);
	memcpy(iVertex, &iVertex1, t);
	memcpy(iVertex + 1, &iVertex2, t);
	memcpy(iVertex + 2, &iVertex3, t);

	bool equal[3] = { false, false, false };
	int rc = 0;

	for (size_t i = 0; i < 3; i++)
	{
		if (fabs(iInfo[i] - iRef) < iTol)
		{
			equal[i] = true;
			ioIndex[i] = i;
			memcpy(ioPoint + rc, &iVertex + i, t); // 顶点值为给定值
			rc++;
		}
		else
			equal[i] = false;
	}

	//如果只有0个或1个顶点是等值点
	if (rc == 1 || rc == 0)
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (equal[i] || equal[(i + 1) % 3])
				continue;
			else
			{
				if ((iRef >= iInfo[i]) && (iRef <= iInfo[(i + 1) % 3]) ||
					(iRef >= iInfo[(i + 1) % 3]) && (iRef <= iInfo[i]))
				{
					double tmp = (iRef - iInfo[i]) / (iInfo[(i + 1) % 3] - iInfo[i]);
					STLVECTOR delta_v = iVertex[(i + 1) % 3] - iVertex[i];
					ioPoint[rc] = iVertex[i] + tmp * delta_v; // 线性插值
					ioIndex[rc] = i;
					rc++;
				}
			}
		}
	}

	return rc;
}

BOOL mathIsVectorDuringTwoVectors(STLVECTOR iVector1, STLVECTOR iVector2, STLVECTOR iVector)
{
	STLVECTOR v1 = vectorNormalize(iVector1 ^ iVector), v2 = vectorNormalize(iVector ^ iVector2);
	double theta1 = acos(iVector1 * iVector), theta2 = acos(iVector2 * iVector);
	if (v1 * v2 >= 0 && (theta1 + theta2 < PI + TOLANGLE))
		return true;
	else
		return false;
}
