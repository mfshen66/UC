#include "StdAfx.h"

////////////////////////////////////////////////////////////////
RBALL* rballCreate(int n, double k, float w1, float w2)
{
	double r1, r2 ;
	VEC3D v1, v2, X = {1., 0., 0.},
		          Y = {0., 1., 0.},
		          Z = {0., 0., 1.} ;
	RBALL* rball ;

	rball = new RBALL() ;
	rball->n = n ;
	rball->w1 = w1 ;
	rball->w2 = w2 ;
	rball->center[0] = 0. ;
	rball->center[1] = 0. ;
	rball->center[2] = 0. ;
	rball->r = 1. ;
	rball->k = k ;
	mathVec(v1, sqrt(fabs(1-2*k*k)), k, k) ;
	mathVec(v2, sqrt(fabs(1-k*k)), k, 0.) ;
	rball->a = mathGetAngleUnit(v1, v2) ;

	r1 = rball->r*(rball->k+1.) ;
	r2 = rball->r*rball->k ;
	rball->xyTorus = facetTorus(rball->center, Z, r1, r2, 20, 4) ;
	rball->yzTorus = facetTorus(rball->center, X, r1, r2, 20, 4) ;
	rball->zxTorus = facetTorus(rball->center, Y, r1, r2, 20, 4) ;
	rball->nodes = new FNODE[rball->xyTorus->nnode] ;

	return rball ;
}

void rballFree(RBALL* rball)
{
	if( rball )
	{
		if( rball->xyTorus )
			facetFree(rball->xyTorus) ;
		if( rball->yzTorus )
			facetFree(rball->yzTorus) ;
		if( rball->zxTorus )
			facetFree(rball->zxTorus) ;
		if( rball->nodes )
			delete []rball->nodes ;
		delete rball ;
	}

	return ;
}

void rballSetCenterR(RBALL* rball, PNT3D center, double r)
{
	if( rball )
	{
		memcpy(rball->center, center, sizeof(PNT3D)) ;
		rball->r = r ;
	}

	return ;
}

// v是观察方向，单位矢量
void rballDrawBall(RBALL* rball, VEC3D v)
{
	int i, n ;
	double x1, y1, x2, y2, step, ca, sa ;
	PNT3D p ;
	VEC3D X, Y ;

	n = 4*rball->n ;
	step = PI2/n ;
	ca = cos(step) ;
	sa = sin(step) ;
	x1 = rball->r ;
	y1 = 0. ;
	mathGetXYFromZ(v, X, Y) ;
	mathEvalLin(rball->center, X, x1, p) ;
	glBegin(GL_TRIANGLES) ;
		for( i = 0 ; i < n ; i++ )
		{
			glNormal3dv(v) ;
			glVertex3dv(p) ;
			x2 = x1*ca-y1*sa ;
			y2 = y1*ca+x1*sa ;
			x1 = x2 ;
			y1 = y2 ;
			p[0] = rball->center[0]+X[0]*x1+Y[0]*y1 ;
			p[1] = rball->center[1]+X[1]*x1+Y[1]*y1 ;
			p[2] = rball->center[2]+X[2]*x1+Y[2]*y1 ;
			glNormal3dv(v) ;
			glVertex3dv(p) ;
			glNormal3dv(v) ;
			glVertex3dv(rball->center) ;
		}
	glEnd() ;

	return ;
}

void rballDrawArc(RBALL* rball, VEC3D v)
{
	int iQ ;
	double matrix[16], h ;
	PNT3D O ;
	VEC3D X = {1., 0., 0.},
		  Y = {0., 1., 0.},
		  Z = {0., 0., 1.} ;
	RFRAME lf ;

	h = rball->k*rball->r ;
	glLineWidth(rball->w1) ;
	for( iQ = 0 ; iQ < 8 ; iQ++ )
	{
		mathGetRFrameOfQuadrant(iQ, &lf) ;
		memcpy(lf.O, rball->center, sizeof(PNT3D)) ;
		mathInitMatrix16(lf.O, lf.X, lf.Y, lf.Z, matrix) ;
		glPushMatrix() ;
		glMultMatrixd(matrix) ;
		O[0] = 0. ;
		O[1] = 0. ;
		O[2] = h ;
		drawArc2(O, X, Y, rball->r, rball->a, rball->n) ; // XY
		O[0] = 0. ;
		O[1] = h ;
		O[2] = 0. ;
		drawArc2(O, Z, X, rball->r, rball->a, rball->n) ; // ZX
		O[0] = h ;
		O[1] = 0. ;
		O[2] = 0. ;
		drawArc2(O, Y, Z, rball->r, rball->a, rball->n) ; // YZ
		glPopMatrix() ;
	}

	glLineWidth(rball->w2) ;
	mathGetXYFromZ(v, lf.X, lf.Y) ;
	drawArc(lf.O, lf.X, lf.Y, rball->r, PI2, 4*rball->n) ;
	h = rball->r*(2*rball->k+1.) ;
	drawArc(lf.O, lf.X, lf.Y, h, PI2, 4*rball->n) ;

	return ;
}

// axis = 'Z'表示绘制XY平面上的圆环带
// axis = 'X'表示绘制YZ平面上的圆环带
// axis = 'Y'表示绘制ZX平面上的圆环带
void rballDrawBand(RBALL* rball, char axis)
{
	int i ;
	FACET* facet ;

	if( axis == 'X' )
		facet = rball->yzTorus ;
	else
	if( axis == 'Y' )
		facet = rball->zxTorus ;
	else
		facet = rball->xyTorus ;

	if( facet )
	{
		for( i = 0 ; i < facet->nnode ; i++ )
		{
			rball->nodes[i] = facet->nodes[i] ;
			rball->nodes[i].p[0] = facet->nodes[i].p[0]*rball->r+rball->center[0] ;
			rball->nodes[i].p[1] = facet->nodes[i].p[1]*rball->r+rball->center[1] ;
			rball->nodes[i].p[2] = facet->nodes[i].p[2]*rball->r+rball->center[2] ;
		}
		glVertexPointer(3, GL_DOUBLE, sizeof(FNODE), rball->nodes[0].p) ;
		glNormalPointer(GL_DOUBLE, sizeof(FNODE), rball->nodes[0].normal) ;
		glDrawElements(GL_TRIANGLES, facet->ntria*3, GL_UNSIGNED_INT, facet->trias) ;
	}

	return ;
}

// 0=未拾取到,1=拾取到XY平面大圆, 2=YZ平面大圆, 3=ZX平面大圆, 4=拾取到球
int rballPick(RBALL* rball, PNT3D p, VEC3D v, double r)
{
	int k ;
	double d, R ;
	PNT3D q, Q ;
	VEC3D axis ;

	if( rball )
	{
		mathPrjPntLin(rball->center, p, v, q) ; // q是投影点
		d = mathDist(rball->center, q) ;
		R = rball->r ;
		if( d > R+r )
			return 0 ; // 未拾取到
		if( d < R ) // 注意：q是拾取射线与球朝向p点（即视点）一侧的交点
			mathEvalLin(q, v, -sqrt(fabs((R+d)*(R-d))), q) ;

		// 拾取XY、YZ、ZX平面上的大圆
		for( k = 1 ; k < 4 ; k++ ) // 以下拾取算法:近似计算直线/圆最近点
		{
			mathInitVec(axis, k==1?'Z':(k==2?'X':'Y')) ;
			mathPrjPntPln(q, rball->center, axis, Q) ;
			d = mathDist(rball->center, Q) ;
			if( d > MIN_LEN )
			{ // 在下面代码中Q在大圆上, d是矢量Q-q在大圆平面上的投影长度
				Q[0] = rball->center[0]+R*(Q[0]-rball->center[0])/d ;
				Q[1] = rball->center[1]+R*(Q[1]-rball->center[1])/d ;
				Q[2] = rball->center[2]+R*(Q[2]-rball->center[2])/d ;
				d = sqrt((Q[0]-q[0])*(Q[0]-q[0])*fabs(1.-axis[0])+
					     (Q[1]-q[1])*(Q[1]-q[1])*fabs(1.-axis[1])+
					     (Q[2]-q[2])*(Q[2]-q[2])*fabs(1.-axis[2])) ;
				if( d < r )
					return k ;
			}
		}

		return 4 ;
	}
	else
		return -1 ;
}
////////////////////////////////////////////////////////////////