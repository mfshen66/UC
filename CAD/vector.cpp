#include "stdafx.h"
#include "vector.h"

////////////////////////////////////////////////////////////////

double mathGetDist(PNT3D p1, PNT3D p2)
{
	double l = 0.;
	for (int i = 0; i < 3; i++)
		l += pow((p1[i] - p2[i]), 2);
	return sqrt(l);
}

void mathGetVec2D(PNT2D p1, PNT2D p2, VEC2D v)//向量
{
	v[0] = p2[0] - p1[0];
	v[1] = p2[1] - p1[1];

	return;
}

//double mathVecLen(VEC3D v)//向量的模
//{
//	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
//}

//double mathOProduct(VEC3D v1, VEC3D v2)//点积
//{
//	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
//}

//void mathVProduct(VEC3D v1, VEC3D v2, VEC3D rtvec)//叉积
//{
//	VEC3D vector;
//
//	vector[0] = v1[1] * v2[2] - v1[2] * v2[1];
//	vector[1] = v1[2] * v2[0] - v1[0] * v2[2];
//	vector[2] = v1[0] * v2[1] - v1[1] * v2[0];
//
//	rtvec[0] = vector[0];
//	rtvec[1] = vector[1];
//	rtvec[2] = vector[2];
//
//	return;
//}


void mathMixPnt(PNT3D p1, PNT3D p2, double w1, double w2, PNT3D mix_p)
{
	mix_p[0] = w1 * p1[0] + w2 * p2[0];
	mix_p[1] = w1 * p1[1] + w2 * p2[1];
	mix_p[2] = w1 * p1[2] + w2 * p2[2];

	return;
}

double mathMProduct(VEC3D v1, VEC3D v2, VEC3D v3)
{
	VEC3D vector;

	mathVProduct(v1, v2, vector);

	return mathOProduct(vector, v3);
}

void mathGetMidPnt(PNT3D p1, PNT3D p2, PNT3D mid)
{
	mid[0] = 0.5 * (p1[0] + p2[0]);
	mid[1] = 0.5 * (p1[1] + p2[1]);
	mid[2] = 0.5 * (p1[2] + p2[2]);

	return;
}

int mathGetVecUnit(PNT3D p1, PNT3D p2, VEC3D v)
{
	v[0] = p2[0] - p1[0];
	v[1] = p2[1] - p1[1];
	v[2] = p2[2] - p1[2];

	return mathUniVec(v, MIN_DBL);
}

int mathGetVecUnit2D(PNT2D p1, PNT2D p2, VEC2D v)
{
	v[0] = p2[0] - p1[0];
	v[1] = p2[1] - p1[1];

	return mathUniVec(v, MIN_DBL);
}


double mathGetAngle(VEC3D v1, VEC3D v2, double min_len)
{
	double d1, d2, angle;
	VEC3D unit_v1, unit_v2;

	d1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2]);
	d2 = sqrt(v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2]);
	if (d1 < min_len ||
		d2 < min_len)
		return -1.0;
	unit_v1[0] = v1[0] / d1;
	unit_v1[1] = v1[1] / d1;
	unit_v1[2] = v1[2] / d1;
	unit_v2[0] = v2[0] / d2;
	unit_v2[1] = v2[1] / d2;
	unit_v2[2] = v2[2] / d2;
	angle = mathGetAngleUnit(unit_v1, unit_v2);

	return angle;
}


//int mathUniVec(VEC3D v, double min_len)
//{
//	double len;
//
//	len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
//	if (len < min_len)
//	{
//		v[0] = 0.;
//		v[1] = 0.;
//		v[2] = 0.;
//		return ERUNSUC;
//	}
//	else
//	{
//		v[0] /= len;
//		v[1] /= len;
//		v[2] /= len;
//		return ERSUCSS;
//	}
//}


//double mathGetAngleUnit(VEC3D v1, VEC3D v2)
//{
//	double d1, d2;
//	PNT3D prj_p;
//
//	d1 = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
//	prj_p[0] = d1 * v1[0];
//	prj_p[1] = d1 * v1[1];
//	prj_p[2] = d1 * v1[2];
//	d2 = sqrt((v2[0] - prj_p[0])*(v2[0] - prj_p[0]) +
//		(v2[1] - prj_p[1])*(v2[1] - prj_p[1]) +
//		(v2[2] - prj_p[2])*(v2[2] - prj_p[2]));
//
//	return atan2(d2, d1);
//}


void mathRevVec(VEC3D v)
{
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}

int mathVProductUnit(VEC3D v1, VEC3D v2, VEC3D rtvec)
{
	VEC3D vector;

	vector[0] = v1[1] * v2[2] - v1[2] * v2[1];
	vector[1] = v1[2] * v2[0] - v1[0] * v2[2];
	vector[2] = v1[0] * v2[1] - v1[1] * v2[0];
	if (mathUniVec(vector, MIN_DBL) != ERSUCSS)
		return ERUNSUC;

	rtvec[0] = vector[0];
	rtvec[1] = vector[1];
	rtvec[2] = vector[2];

	return ERSUCSS;
}