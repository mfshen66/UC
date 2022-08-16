// 2017/1/7 nt
// ��ת�� : ������ͼ������Ľ�����ת

#pragma once

////////////////////////////////////////////////////////////////
typedef struct _rball RBALL ;
struct _rball
{
	int n ; // ��1/4Բ���ֳ�n��
	PNT3D center ; // ����
	double r ; // �뾶
	double k ; // h = k*r k < 1, һ��ȡ0.05
	double a ; // �Ƕ���k����
	float w1 ; // 1/8��߽���߿�һ��ȡ1.f
	float w2 ; // ������������ߵ��߿�һ��ȡ2.f
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
// axis = 'Z'��ʾ����XYƽ���ϵ�Բ����
void rballDrawBand(RBALL* rball, char axis) ;
// 0=δʰȡ��,1=ʰȡ��XYƽ���Բ, 2=YZƽ���Բ, 3=ZXƽ���Բ, 4=ʰȡ����
int rballPick(RBALL* rball, PNT3D p, VEC3D v, double r) ;
////////////////////////////////////////////////////////////////
