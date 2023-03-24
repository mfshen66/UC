#include "stdafx.h"
#include "Triangle.h"
#include "vector.h"

////////////////////////////////////////////////////////////////


int faceChkPntSide(PNT3D p, FACE * tri, double tol)
{
	VEC3D v;
	mathGetVec(tri->A, p, v);
	double t = mathOProduct(v, tri->normal);
	if (t > tol)//在正侧
		return 1;
	if (fabs(t) <= tol)//在面上
		return -1;
	return 0;
}

void faceCreatBox(FACE * tri)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		mathComp3doubles(tri->A[i], tri->B[i], tri->C[i], tri->box.min[i], tri->box.max[i]);
	}
}

int boxChkLinInt2(BOX3D * box, PNT3D begin, VEC3D dir, double tol)
{
	PNT3D end;
	int i;
	for (i = 0; i < 3; i++)
	{
		end[i] = begin[i] + dir[i];
		if (begin[i] > box->min[i] + tol
			&& begin[i] < box->max[i] - tol
			&& end[i] > box->min[i] + tol
			&& end[i] < box->max[i] - tol)
		{
			continue;
		}
		else
			break;
	}
	if (i == 3)
		return IDINT;
	else
		return IDOUT;
}

int triIntLin(FACE* pT,
	PNT3D begin,
	VEC3D dir,
	double tol,
	int* pn,
	PNT3D intpnt1,
	PNT3D intpnt2)
{
	double d, dA, dB, dC, w1, w2;
	PNT3D p;
	VEC3D v;

	*pn = 0; // initial value
	if (boxChkLinInt2(&(pT->box), begin, dir, tol) == IDINT)
		return IDNOINT;
	if (fabs(mathOProduct(pT->normal, dir)) > SR2_2) // 夹角小于45°
	{
		if (mathIntLinPln(begin, // line/plane inter
			dir,
			pT->A,
			pT->normal,
			tol,
			MIN_ANG,
			p) == IDINT)
		{
			if (triChkPnt2(pT, p, tol) == IDIN)
			{
				memcpy(intpnt1, p, sizeof(PNT3D));
				*pn = 1;
				return IDINT; // intersection point
			}
			else
				return IDNOINT; // no intersection
		}
		else
			return IDNOINT; // error
	}
	else
	{
		if (mathVProductUnit(pT->normal, dir, v) != ERSUCSS)
			return IDNOINT;
		dA = mathDistPntPln(pT->A, begin, v); // A to perpendicular plane
		dB = mathDistPntPln(pT->B, begin, v); // B to per_planeular plane
		dC = mathDistPntPln(pT->C, begin, v); // C to per_planeular plane
		if ((dA > tol && dB > tol && dC > tol) || // 排除同侧的情况
			(dA < -tol && dB < -tol && dC < -tol))
			return IDNOINT;
		else
		{
			if (fabs(dA) < MIN_DBL &&
				fabs(dB) < MIN_DBL)
			{
				memcpy(intpnt1, pT->A, sizeof(PNT3D));
				memcpy(intpnt2, pT->B, sizeof(PNT3D));
				*pn = 2;
			}
			else
				if (fabs(dB) < MIN_DBL &&
					fabs(dC) < MIN_DBL)
				{
					memcpy(intpnt1, pT->B, sizeof(PNT3D));
					memcpy(intpnt2, pT->C, sizeof(PNT3D));
					*pn = 2;
				}
				else
					if (fabs(dC) < MIN_DBL &&
						fabs(dA) < MIN_DBL)
					{
						memcpy(intpnt1, pT->C, sizeof(PNT3D));
						memcpy(intpnt2, pT->A, sizeof(PNT3D));
						*pn = 2;
					}
					else
					{
						if (dA*dB < 0.)
						{
							w1 = fabs(dA);
							w2 = fabs(dB);
							d = w1 + w2;
							w1 /= d;
							w2 /= d;
							mathMixPnt(pT->A, pT->B, w2, w1, (*pn) ? intpnt2 : intpnt1);
							(*pn)++;
						}
						if (dB*dC < 0.)
						{
							w1 = fabs(dB);
							w2 = fabs(dC);
							d = w1 + w2;
							w1 /= d;
							w2 /= d;
							mathMixPnt(pT->B, pT->C, w2, w1, (*pn) ? intpnt2 : intpnt1);
							(*pn)++;
						}
						if (dC*dA < 0.)
						{
							w1 = fabs(dC);
							w2 = fabs(dA);
							d = w1 + w2;
							w1 /= d;
							w2 /= d;
							mathMixPnt(pT->C, pT->A, w2, w1, (*pn) ? intpnt2 : intpnt1);
							(*pn)++;
						}
						if ((*pn) > 2)
							(*pn) = 2;
					}
			if ((*pn) > 0)
			{
				mathPrjPntLin(intpnt1, begin, dir, p); // 交点投影到直线上
				w1 = mathDist(intpnt1, p); // distance
				memcpy(intpnt1, p, sizeof(PNT3D));
			}
			else
				w1 = MAX_DBL;
			if ((*pn) > 1)
			{
				mathPrjPntLin(intpnt2, begin, dir, p); // 交点投影到直线上
				w2 = mathDist(intpnt2, p); // distance
				memcpy(intpnt2, p, sizeof(PNT3D));
			}
			else
				w2 = MAX_DBL;
			if (w1 > tol)
			{
				if ((*pn) > 1)
					memcpy(intpnt1, intpnt2, sizeof(PNT3D));
				(*pn)--;
			}
			if (w2 > tol)
				(*pn)--;
			return (*pn) > 0 ? IDINT : IDNOINT;
		}
	}
}

int triIntSegm(FACE * pT, PNT3D begin, PNT3D end, double tol, int & pn, PNT3D intpnt1, PNT3D intpnt2)
{
	pn = 0;
	int size_of_pnt = sizeof(PNT3D);
	//if (triChkPnt2(pT, begin, tol) == IDIN)
	//{
	//	memcpy(intpnt1, begin, size_of_pnt);
	//	pn++;
	//}
	//if (triChkPnt2(pT, end, tol) == IDIN)
	//{
	//	memcpy(intpnt2, end, size_of_pnt);
	//	pn++;
	//}

	VEC3D v;
	mathGetVec(begin, end, v);

	double d_begin, d_end;
	d_begin = mathDistPntPlnSide(begin, pT->A, pT->normal);
	d_end = mathDistPntPlnSide(end, pT->A, pT->normal);

	if (d_begin * d_end > tol)
	{
		// 线段在三角形同侧，无交
		pn = 0;
		return (pn > 0) ? IDINT : IDNOINT;
	}
	else if (fabs(d_begin * d_end) <= tol) // 线段在三角形上
	{
		PNT3D p_tmp[3];
		bool is_int[3] = { 0, 0, 0 };
		is_int[0] = mathSegmIntSegmInFace(begin, end, pT->A, pT->B, tol, p_tmp[0]);
		is_int[1] = mathSegmIntSegmInFace(begin, end, pT->B, pT->C, tol, p_tmp[1]);
		is_int[2] = mathSegmIntSegmInFace(begin, end, pT->A, pT->C, tol, p_tmp[2]);

		for (size_t i = 0; i < 3; i++)
		{
			if (is_int[i])
			{
				if (pn == 0)
				{
					memcpy(intpnt1, p_tmp[i], size_of_pnt);
					pn++;
				}
				else
				{
					if (!mathIsCoinsidentPoint(intpnt1, p_tmp[i], tol))
					{
						memcpy(intpnt2, p_tmp[i], size_of_pnt);
						pn++;
						return (pn > 0) ? IDINT : IDNOINT;
					}
				}
			}
		}
		return (pn > 0) ? IDINT : IDNOINT;
	}
	else //	在异侧
	{
		double w1 = fabs(d_begin);
		double w2 = fabs(d_end);
		double sum_d = w1 + w2;
		for (int i = 0; i < 3; i++)
		{
			intpnt1[i] = begin[i] + w1 / sum_d * v[i];
		}
		pn = 1;
		return (pn > 0) ? IDINT : IDNOINT;
	}
}

double triCalArea(PNT3D p1, PNT3D p2, PNT3D p3)//三角形面积
{
	VEC3D v1, v2, normal;
	double angle = 0., angtol = 1.e-3;

	mathGetVec(p1, p2, v1);
	mathGetVec(p1, p3, v2);
	angle = mathGetAngle(v1, v2, MIN_DBL);
	if (fabs(angle) < angtol
		|| fabs(angle - PI1) < angtol)
		return 0.;//三点共线
	mathVProduct(v1, v2, normal);
	return 0.5*mathVecLen(normal);
}

int triGetNormal(PNT3D p1, PNT3D p2, PNT3D p3, double tol, VEC3D normal)
{
	int k;
	VEC3D v1, v2;

	//if (mathDistPntLinEx(p1, p2, p3) < tol || // nt add 2001/11/28
	//	mathDistPntLinEx(p2, p3, p1) < tol ||
	//	mathDistPntLinEx(p3, p1, p2) < tol)
	//	return ERUNSUC;
	for (k = 0; k < 3; k++)
	{
		v1[k] = p2[k] - p1[k];
		v2[k] = p3[k] - p2[k];
	}
	mathVProduct(v1, v2, normal);
	if (mathUniVec(normal, INF_DBL) != ERSUCSS)
		return ERUNSUC;

	return ERSUCSS;
}

double mathDistPntPlnSide(PNT3D p, PNT3D pivot, VEC3D normal)
{	/* normal is assumed to be unit vector */
	return	normal[0] * (p[0] - pivot[0]) +
		normal[1] * (p[1] - pivot[1]) +
		normal[2] * (p[2] - pivot[2]);
}

int triChkPnt(FACE *pT, PNT3D p, double tol)
{
	if (boxChkPnt(&(pT->box), p, tol) == IDIN)
	{
		if (fabs(mathDistPntPln(p, pT->A, pT->normal)) < tol)
		{
			VEC3D v;
			double d, dAB, dBC, dCA;
			// distance of p to AB
			mathGetVec(pT->A, pT->B, v);
			if (mathUniVec(v, MIN_DBL) != ERSUCSS)
				return IDOUT; // error
			mathVProduct(pT->normal, v, v);
			d = mathDistPntPln(pT->C, pT->A, v); // distance of pT->C to AB
			if (d < 0.)
			{
				v[0] = -v[0];
				v[1] = -v[1];
				v[2] = -v[2];
			}
			dAB = mathDistPntPln(p, pT->A, v); // distance of p to AB
			if (dAB < -tol)
				return IDOUT;
			// distance of p to BC
			mathGetVec(pT->B, pT->C, v);
			if (mathUniVec(v, MIN_DBL) != ERSUCSS)
				return IDOUT; // error
			mathVProduct(pT->normal, v, v);
			d = mathDistPntPln(pT->A, pT->B, v); // distance of pT->A to BC
			if (d < 0.)
			{
				v[0] = -v[0];
				v[1] = -v[1];
				v[2] = -v[2];
			}
			dBC = mathDistPntPln(p, pT->B, v); // distance of p to BC
			if (dBC < -tol)
				return IDOUT;
			// distance of p to CA
			mathGetVec(pT->C, pT->A, v);
			if (mathUniVec(v, MIN_DBL) != ERSUCSS)
				return IDOUT; // error
			mathVProduct(pT->normal, v, v);
			d = mathDistPntPln(pT->B, pT->C, v); // distance of pT->B to CA
			if (d < 0.)
			{
				v[0] = -v[0];
				v[1] = -v[1];
				v[2] = -v[2];
			}
			dCA = mathDistPntPln(p, pT->C, v); // distance of p to CA
			if (dCA < -tol)
				return IDOUT;
			// p在三角形内部, 或在某条边上
			if ((dAB > 0. && dBC > 0. && dCA > 0.) ||
				(dAB < tol && mathChkPntSegm(p, pT->A, pT->B, tol) == IDIN) ||
				(dBC < tol && mathChkPntSegm(p, pT->B, pT->C, tol) == IDIN) ||
				(dCA < tol && mathChkPntSegm(p, pT->C, pT->A, tol) == IDIN))
				return IDIN;
			else
				return IDOUT;
		}
		else
			return IDOUT;
	}
	else
		return IDOUT;
}

int triChkPnt2(FACE * pT, PNT3D p, double tol)
{
	double s, s1, s2, s3;
	s = triCalArea(pT->A, pT->B, pT->C);
	s1 = triCalArea(p, pT->A, pT->B);
	s2 = triCalArea(p, pT->B, pT->C);
	s3 = triCalArea(p, pT->C, pT->A);
	if (fabs(s - s1 - s2 - s3) < tol)
		return IDIN;
	else
		return IDOUT;
}

int boxChkPnt(BOX3D * box, PNT3D p, double tol)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		if (p[i] < box->min[i] - tol
			|| p[i] > box->max[i] + tol)
			return IDOUT;
	}
	return IDIN;
}


//double mathDistPntPln(PNT3D p, PNT3D pivot, VEC3D normal)
//{	/* normal is assumed to be unit vector */
//	return normal[0] * (p[0] - pivot[0]) +
//		normal[1] * (p[1] - pivot[1]) +
//		normal[2] * (p[2] - pivot[2]);
//}

void mathComp3doubles(double a, double b, double c, double & min, double & max)
{
	if (a >= b)
	{
		max = a;
		min = b;
	}
	else
	{
		min = a;
		max = b;
	}
	if (c > max)
		max = c;
	if (c < min)
		min = c;
	return;
}

bool mathIsCoinsidentPoint(PNT3D iP1, PNT3D iP2, double iTol)
{
	if (mathDist(iP1, iP2) <= iTol)
	{
		return true;
	}
	return false;
}
