// 2017/1/7 nt
// 旋转球 : 用于视图或物体的交互旋转

#pragma once

////////////////////////////////////////////////////////////////
typedef struct _rball RBALL ;
struct _rball
{
	int n ; // 将1/4圆弧分成n段
	PNT3D center ; // 球心
	double r ; // 半径
	double k ; // h = k*r k < 1, 一般取0.05
	double a ; // 角度由k决定
	float w1 ; // 1/8球边界的线宽，一般取1.f
	float w2 ; // 整个球的轮廓线的线宽，一般取2.f
	FACET* xyTorus ;
	FACET* yzTorus ;
	FACET* zxTorus ;
	FNODE* nodes ;
} ;

RBALL* rballCreate(int n, double k, float w1, float w2) ;
void rballFree(RBALL* rball) ;
void rballSetCenterR(RBALL* rball, PNT3D center, double r) ;
void rballDrawBall(RBALL* rball, VEC3D v) ;
void rballDrawArc(RBALL* rball, VEC3D v) ;
// axis = 'Z'表示绘制XY平面上的圆环带
void rballDrawBand(RBALL* rball, char axis) ;
// 0=未拾取到,1=拾取到XY平面大圆, 2=YZ平面大圆, 3=ZX平面大圆, 4=拾取到球
int rballPick(RBALL* rball, PNT3D p, VEC3D v, double r) ;
////////////////////////////////////////////////////////////////
