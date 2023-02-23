//#pragma once

#ifndef _Triangle_h_
#define _Triangle_h_

using namespace std;

////////////////////////////////////////////////////////////////
#define INF_DBL 1e-10
//--------------------------------------------------------------
typedef struct _face FACE;
struct _face
{
	int a, b, c;	//三个点的编号
	PNT3D A, B, C;	//三个点坐标
	BOX3D box;		//包围盒
	bool flag;		//是否为最终多面体的面
	VEC3D normal;
};
int faceChkPntSide(PNT3D p, FACE *tri, double tol);
void faceCreatBox(FACE *tri);
int boxChkLinInt2(BOX3D *box, PNT3D begin, VEC3D dir, double tol);
int triIntLin(FACE* pT,
	PNT3D begin,
	VEC3D dir,
	double tol,
	int* pn,
	PNT3D intpnt1,
	PNT3D intpnt2);
int triIntSegm(FACE* pT, PNT3D begin, PNT3D end, double tol, int &pn, PNT3D intpnt1, PNT3D intpnt2);
//--------------------------------------------------------------
double triCalArea(PNT3D p1, PNT3D p2, PNT3D p3);
int triGetNormal(PNT3D p1, PNT3D p2, PNT3D p3, double tol, VEC3D normal);
double mathDistPntPlnSide(PNT3D p, PNT3D pivot, VEC3D normal);
int triChkPnt(FACE *pT, PNT3D p, double tol);
int triChkPnt2(FACE *pT, PNT3D p, double tol);//同一个面上
int boxChkPnt(BOX3D *box, PNT3D p, double tol);
double triCalArea(PNT3D p1, PNT3D p2, PNT3D p3);

//double mathDistPntPln(PNT3D p, PNT3D pivot, VEC3D normal);
void mathComp3doubles(double a, double b, double c, double &min, double &max);

bool mathIsCoinsidentPoint(PNT3D iP1, PNT3D iP2, double iTol);
#endif