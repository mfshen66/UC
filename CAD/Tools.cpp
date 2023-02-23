// Tools.cpp : 工具函数的实现
#include "stdafx.h"
#include "DirectXCollision.h"
using namespace DirectX ;

///////////////////////////////////////////////////////////////
// 去掉路径，只提取文件名，带扩展名
void fileGetName(CString& pathName, CString& fileName)
{
	int i, m, n ;

	n = pathName.GetLength() ;
	for( m = -1, i = n-1 ; i >= 0 ; i-- )
	{
		if( pathName.GetAt(i) == _T('\\') )
		{
			m = i ;
			break ;
		}
	}
	fileName = pathName.Right(n-m-1) ;

	return ;
}

void fileGetTitle(CString& pathName, CString& title)
{
	int i, n ;
	CString fileName ;

	fileGetName(pathName, fileName) ;
	n = fileName.GetLength() ;
	for( i = n-1 ; i >= 0 ; i-- )
	{
		if( fileName.GetAt(i) == _T('.') )
		{
			title = fileName.Left(i) ;
			return ;
		}
	}
	title = fileName ;

	return ;
}

// draw cube in shading
double roundOff(double d, int nAfterZeroPoint)
{
	int i ;

	for( i = 0 ; i < nAfterZeroPoint ; i++ )
		d *= 10 ;
	if( d > 0. )
		d += 0.5 ;
	else
		d -= 0.5 ;
	i = (int)d ;
	d = (double)i ;
	for( i = 0 ; i < nAfterZeroPoint ; i++ )
		d /= 10 ;

	return d ;
}
//--------------------------------------------------------------
void drawCube(double min[3], double max[3])
{
	double X[3], Y[3], Z[3];

	X[0] = Y[1] = Z[2] = 1.;
	X[1] = Y[0] = Z[0] = 0.;
	X[2] = Y[2] = Z[1] = 0.;
	glBegin(GL_QUADS);
	glNormal3dv(Z);
	glVertex3d(min[0], min[1], min[2]); // 下底
	glNormal3dv(Z);
	glVertex3d(max[0], min[1], min[2]);
	glNormal3dv(Z);
	glVertex3d(max[0], max[1], min[2]);
	glNormal3dv(Z);
	glVertex3d(min[0], max[1], min[2]);

	glNormal3dv(Z);
	glVertex3d(min[0], min[1], max[2]); // 上底
	glNormal3dv(Z);
	glVertex3d(max[0], min[1], max[2]);
	glNormal3dv(Z);
	glVertex3d(max[0], max[1], max[2]);
	glNormal3dv(Z);
	glVertex3d(min[0], max[1], max[2]);

	glNormal3dv(Y);
	glVertex3d(min[0], min[1], min[2]); // 前侧
	glNormal3dv(Y);
	glVertex3d(max[0], min[1], min[2]);
	glNormal3dv(Y);
	glVertex3d(max[0], min[1], max[2]);
	glNormal3dv(Y);
	glVertex3d(min[0], min[1], max[2]);

	glNormal3dv(Y);
	glVertex3d(min[0], max[1], min[2]); // 后侧
	glNormal3dv(Y);
	glVertex3d(max[0], max[1], min[2]);
	glNormal3dv(Y);
	glVertex3d(max[0], max[1], max[2]);
	glNormal3dv(Y);
	glVertex3d(min[0], max[1], max[2]);

	glNormal3dv(X);
	glVertex3d(min[0], min[1], min[2]); // 左侧
	glNormal3dv(X);
	glVertex3d(min[0], max[1], min[2]);
	glNormal3dv(X);
	glVertex3d(min[0], max[1], max[2]);
	glNormal3dv(X);
	glVertex3d(min[0], min[1], max[2]);

	glNormal3dv(X);
	glVertex3d(max[0], min[1], min[2]); // 右侧
	glNormal3dv(X);
	glVertex3d(max[0], max[1], min[2]);
	glNormal3dv(X);
	glVertex3d(max[0], max[1], max[2]);
	glNormal3dv(X);
	glVertex3d(max[0], min[1], max[2]);
	glEnd();

	return;
}

// draw cube in wireframe
void drawCube2(double min[3], double max[3])
{
	glBegin(GL_LINE_LOOP);
	glVertex3d(min[0], min[1], min[2]); // 下底
	glVertex3d(max[0], min[1], min[2]);
	glVertex3d(max[0], max[1], min[2]);
	glVertex3d(min[0], max[1], min[2]);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3d(min[0], min[1], max[2]); // 上底
	glVertex3d(max[0], min[1], max[2]);
	glVertex3d(max[0], max[1], max[2]);
	glVertex3d(min[0], max[1], max[2]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3d(min[0], min[1], min[2]); // 侧棱
	glVertex3d(min[0], min[1], max[2]);
	glVertex3d(max[0], min[1], min[2]);
	glVertex3d(max[0], min[1], max[2]);
	glVertex3d(max[0], max[1], min[2]);
	glVertex3d(max[0], max[1], max[2]);
	glVertex3d(min[0], max[1], min[2]);
	glVertex3d(min[0], max[1], max[2]);
	glEnd();

	return;
}

// draw cube in wireframe
void drawCube3(double min[3], double max[3])
{
	/*glBegin(GL_LINE_LOOP);
	glVertex3dv(min); // 下底
	glVertex3d(max[0], min[1], min[2]);
	glVertex3d(max[0], max[1], min[2]);
	glVertex3d(min[0], max[1], min[2]);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3d(min[0], min[1], max[2]); // 上底
	glVertex3d(max[0], min[1], max[2]);
	glVertex3dv(max);
	glVertex3d(min[0], max[1], max[2]);
	glEnd();*/

	glBegin(GL_LINES);
	/*glVertex3dv(min); // 侧棱
	glVertex3d(min[0], min[1], max[2]);
	glVertex3d(max[0], min[1], min[2]);
	glVertex3d(max[0], min[1], max[2]);
	glVertex3d(max[0], max[1], min[2]);
	glVertex3dv(max);
	glVertex3d(min[0], max[1], min[2]);
	glVertex3d(min[0], max[1], max[2]);*/

	glVertex3dv(min); // 对角线
	glVertex3dv(max);
	glVertex3d(min[0], min[1], max[2]);
	glVertex3d(max[0], max[1], min[2]);
	glVertex3d(min[0], max[1], min[2]);
	glVertex3d(max[0], min[1], max[2]);
	glVertex3d(min[0], max[1], max[2]);
	glVertex3d(max[0], min[1], min[2]);
	glEnd();

	return;
}

void drawCell(double min[3], double max[3])
{
	glBegin(GL_LINES);

	glVertex3dv(min); // 对角线
	glVertex3dv(max);

	glEnd();

	return;

}
void drawSphere(PNT3D center, double r, int nu, int nv)
{
	FACET* facet = facetSphere(center, r, nu, nv) ;
	glVertexPointer(3, GL_DOUBLE, sizeof(FNODE), facet->nodes[0].p) ;
	glNormalPointer(GL_DOUBLE, sizeof(FNODE), facet->nodes[0].normal) ;
	glDrawElements(GL_TRIANGLES, facet->ntria*3, GL_UNSIGNED_INT, facet->trias) ;
	facetFree(facet) ;

	return ;
}

void drawCylinder(PNT3D center, VEC3D Z, double r, double h, int nu, int nv)
{
	FACET* facet = facetCylinder(center, Z, r, h, nu, nv) ;
	glVertexPointer(3, GL_DOUBLE, sizeof(FNODE), facet->nodes[0].p) ;
	glNormalPointer(GL_DOUBLE, sizeof(FNODE), facet->nodes[0].normal) ;
	glDrawElements(GL_TRIANGLES, facet->ntria*3, GL_UNSIGNED_INT, facet->trias) ;
	facetFree(facet) ;

	return ;
}

void drawCone(PNT3D center, VEC3D Z, double r, double h, int nu, int nv)
{
	FACET* facet = facetCone(center, Z, r, h, nu, nv) ;
	glVertexPointer(3, GL_DOUBLE, sizeof(FNODE), facet->nodes[0].p) ;
	glNormalPointer(GL_DOUBLE, sizeof(FNODE), facet->nodes[0].normal) ;
	glDrawElements(GL_TRIANGLES, facet->ntria*3, GL_UNSIGNED_INT, facet->trias) ;
	facetFree(facet) ;

	return ;
}

void drawTorus(PNT3D center, VEC3D Z, double r1, double r2, int nu, int nv)
{
	FACET* facet = facetTorus(center, Z, r1, r2, nu, nv) ;
	glVertexPointer(3, GL_DOUBLE, sizeof(FNODE), facet->nodes[0].p) ;
	glNormalPointer(GL_DOUBLE, sizeof(FNODE), facet->nodes[0].normal) ;
	glDrawElements(GL_TRIANGLES, facet->ntria*3, GL_UNSIGNED_INT, facet->trias) ;
	facetFree(facet) ;

	return ;
}

void drawArc(PNT3D O, VEC3D X, VEC3D Y, double r, double a, int n)
{
	int i ;
	double step, ca, sa, x1, y1, x2, y2 ;
	PNT3D p ;

	if( O == NULL ||
		X == NULL ||
		Y == NULL ||
		r < MIN_RAD ||
		a < MIN_ANG ||
		n < 1 )
		return ;
	
	step = a/n ;
	ca = cos(step) ;
	sa = sin(step) ;
	glBegin(GL_LINE_STRIP) ;
		for( i = 0, x1 = r, y1 = 0. ; i <= n ; i++ )
		{
			p[0] = O[0]+X[0]*x1+Y[0]*y1 ;
			p[1] = O[1]+X[1]*x1+Y[1]*y1 ;
			p[2] = O[2]+X[2]*x1+Y[2]*y1 ;
			glVertex3dv(p) ;
			x2 = x1*ca-y1*sa ;
			y2 = y1*ca+x1*sa ;
			x1 = x2 ;
			y1 = y2 ;
		}
	glEnd() ;

	return ;
}

// [a, PI1/2-a]
void drawArc2(PNT3D O, VEC3D X, VEC3D Y, double r, double a, int n)
{
	VEC3D Z, v1, v2 ;

	mathVProduct(X, Y, Z) ;
	mathUniVec(Z, MIN_DBL) ;
	mathRotVec(Z, O, a, X, v1) ;
	mathRotVec(Z, O, a, Y, v2) ;
	drawArc(O, v1, v2, r, PI1/2-2*a, n) ;

	return ;
}

void drawUmbrella(PNT3D O, VEC3D X, VEC3D Y, VEC3D Z, double r, double h, int n)
{
	int i ;
	double step, ca, sa, x1, y1, x2, y2 ;
	PNT3D p, apex ;

	if( O == NULL ||
		X == NULL ||
		Y == NULL ||
		r < MIN_RAD ||
		n < 1 )
		return ;
	
	step = PI2/n ;
	ca = cos(step) ;
	sa = sin(step) ;
	glBegin(GL_LINE_STRIP) ;
		for( i = 0, x1 = r, y1 = 0. ; i <= n ; i++ )
		{
			p[0] = O[0]+X[0]*x1+Y[0]*y1 ;
			p[1] = O[1]+X[1]*x1+Y[1]*y1 ;
			p[2] = O[2]+X[2]*x1+Y[2]*y1 ;
			glVertex3dv(p) ;
			x2 = x1*ca-y1*sa ;
			y2 = y1*ca+x1*sa ;
			x1 = x2 ;
			y1 = y2 ;
		}
	glEnd() ;

	mathEvalLin(O, Z, h, apex) ;
	glBegin(GL_LINES) ;
		for( i = 0, x1 = r, y1 = 0. ; i < n ; i++ )
		{
			p[0] = O[0]+X[0]*x1+Y[0]*y1 ;
			p[1] = O[1]+X[1]*x1+Y[1]*y1 ;
			p[2] = O[2]+X[2]*x1+Y[2]*y1 ;
			glVertex3dv(p) ;
			glVertex3dv(apex) ;
			x2 = x1*ca-y1*sa ;
			y2 = y1*ca+x1*sa ;
			x1 = x2 ;
			y1 = y2 ;
		}
	glEnd() ;

	return ;
}

// 箭头长为h, 底座半径为r1, 箭头半径为r2, 箭头长为r2
void drawArrowWithDisk(PNT3D O, 
	                   VEC3D Z, 
	                   double r1, 
	                   double r2,
	                   double h, 
	                   int n)
{
	PNT3D p ;
	VEC3D X, Y ;

	mathEvalLin(O, Z, h, p) ;
	mathGetXYFromZ(Z, X, Y) ;

	glBegin(GL_LINES) ;
		glVertex3dv(O) ;
		glVertex3dv(p) ;
	glEnd() ;

	drawArc(O, X, Y, r1, PI2, n) ;

	mathEvalLin(O, Z, h-r2, p) ;
	drawUmbrella(p, X, Y, Z, r2, 5*r2, n) ;

	return ;
}

// nt add 2017/1/16 draw a mark of view moving
// mark is a square, and its center is O
void drawMove(PNT3D O, VEC3D X, VEC3D Y, double w, double h, COLORREF c)
{
	int i, j ;
	double L, d = sqrt(2.) ;
	PNT3D p ;

	L = d*w/3-h/2 ;
	for( i = -1 ; i < 2 ; i += 2 )
	{
		for( j = -1 ; j < 2 ; j += 2 )
		{
			glColor3ub(GetRValue(c), GetGValue(c), GetBValue(c)) ;
			glBegin(GL_TRIANGLES) ;
				mathEvalPln(O, X, Y, i*h/d, 0., p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, 0., j*h/d, p) ;
				glVertex3dv(p) ;
				glVertex3dv(O) ;

				mathEvalPln(O, X, Y, i*w/2, j*w/6, p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, i*w/2, j*w/2, p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, i*w/6, j*w/2, p) ;
				glVertex3dv(p) ;
			glEnd() ;

			glBegin(GL_QUADS) ;
				mathEvalPln(O, X, Y, i*h/d, 0., p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, i*(h+L)/d, j*L/d, p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, i*L/d, j*(h+L)/d, p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, 0., j*h/d, p) ;
				glVertex3dv(p) ;
			glEnd() ;

			glColor3ub(0, 0, 0) ;
			glLineWidth(1.5f) ;
			glBegin(GL_LINE_STRIP) ;
				mathEvalPln(O, X, Y, i*h/d, 0., p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, i*(h+L)/d, j*L/d, p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, i*w/2, j*w/6, p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, i*w/2, j*w/2, p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, i*w/6, j*w/2, p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, i*L/d, j*(h+L)/d, p) ;
				glVertex3dv(p) ;
				mathEvalPln(O, X, Y, 0., j*h/d, p) ;
				glVertex3dv(p) ;
			glEnd() ;
		}
	}

	return ;
}

void drawZoom(PNT3D O, VEC3D X, VEC3D Y, double w, double h, COLORREF c)
{
	double d = sqrt(2.) ;
	PNT3D p ;

	glColor3ub(GetRValue(c), GetGValue(c), GetBValue(c)) ;
	glBegin(GL_TRIANGLES) ;
		mathEvalPln(O, X, Y, w/2, w/2, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, 0., w/2, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, w/2, 0., p) ;
		glVertex3dv(p) ;
	glEnd() ;

	glBegin(GL_QUADS) ;
		mathEvalPln(O, X, Y, w/4+h/(2*d), w/4-h/(2*d), p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, w/4-h/(2*d), w/4+h/(2*d), p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, -w/2, -w/2+h/d, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, -w/2+h/d, -w/2, p) ;
		glVertex3dv(p) ;
	glEnd() ;

	glColor3ub(0, 0, 0) ;
	glLineWidth(1.5f) ;
	glBegin(GL_LINE_LOOP) ;
		mathEvalPln(O, X, Y, w/2, w/2, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, 0., w/2, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, w/4-h/(2*d), w/4+h/(2*d), p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, -w/2, -w/2+h/d, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, -w/2+h/d, -w/2, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, w/4+h/(2*d), w/4-h/(2*d), p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, w/2, 0., p) ;
		glVertex3dv(p) ;
	glEnd() ;

	return ;
}

void drawZoom2(PNT3D O, VEC3D X, VEC3D Y, double w, COLORREF c)
{
	double d = sqrt(2.) ;
	PNT3D p ;

	glColor3ub(GetRValue(c), GetGValue(c), GetBValue(c)) ;
	glBegin(GL_TRIANGLES) ;
		mathEvalPln(O, X, Y, w/2, w/2, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, -w/4, w/2, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, w/2, -w/4, p) ;
		glVertex3dv(p) ;
	glEnd() ;

	glBegin(GL_QUADS) ;
		mathEvalPln(O, X, Y, w/4, 0., p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, 0., w/4, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, -w/2, -w/4, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, -w/4, -w/2, p) ;
		glVertex3dv(p) ;
	glEnd() ;

	glColor3ub(0, 0, 0) ;
	glLineWidth(1.5f) ;
	glBegin(GL_LINE_LOOP) ;
		mathEvalPln(O, X, Y, w/2, w/2, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, -w/4, w/2, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, 0., w/4, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, -w/2, -w/4, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, -w/4, -w/2, p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, w/4, 0., p) ;
		glVertex3dv(p) ;
		mathEvalPln(O, X, Y, w/2, -w/4, p) ;
		glVertex3dv(p) ;
	glEnd() ;

	return ;
}
//--------------------------------------------------------------
FACET* facetCreate2(SURFACE* surface, BOX2D* boxuv, int nu, int nv)
{
	int i, j, n ;
	double u, v ;
	FACET* facet = NULL ;

	if( surface == NULL ||
		nu < 1 ||
		nv < 1 )
		return NULL ;
	
	facet = facetCreate((nu+1)*(nv+1), nu*nv*2, 0) ;
	if( !facet )
		return NULL ; // error
	for( n = 0, i = 0 ; i <= nu ; i++ )
	{
		u = boxuv->min[0]+i*(boxuv->max[0]-boxuv->min[0])/nu ;
		for( j = 0 ; j <= nv ; j++ )
		{
			v = boxuv->min[1]+j*(boxuv->max[1]-boxuv->min[1])/nv ;
			facet->nodes[n].uv[0] = u ;
			facet->nodes[n].uv[1] = v ;
			facet->nodes[n].attrib = 0. ;
			surfaceEvalNormal(surface,
						      facet->nodes[n].uv[0],
						      facet->nodes[n].uv[1],
						      facet->nodes[n].p,
						      facet->nodes[n].normal) ;
			n++ ;
		}
	} // n = i*(nv+1)+j

	for( n = 0, i = 0 ; i < nu ; i++ )
	{
		for( j = 0 ; j < nv ; j++, n += 2 )
		{
			facet->trias[n][0] = i*(nv+1)+j ;
			facet->trias[n][1] = (i+1)*(nv+1)+j ;
			facet->trias[n][2] = (i+1)*(nv+1)+(j+1) ;

			facet->trias[n+1][0] = i*(nv+1)+j ;
			facet->trias[n+1][1] = (i+1)*(nv+1)+(j+1) ;
			facet->trias[n+1][2] = i*(nv+1)+(j+1) ;
		}
	}

	return facet ;
}

// nv = 2,注意此函数只处理u方向是周期的这种情况曲面
FACET* facetCreate3(int nu)
{
	int i ;
	FACET* facet = NULL ;
	
	facet = facetCreate(2*nu, 0, nu) ;
	if( !facet )
		return NULL ; // error

	for( i = 0 ; i < facet->nnode ; i++ )
	{
		facet->nodes[i].uv[0] = 0. ; // 初始化u
		facet->nodes[i].uv[1] = 0. ; // 初始化v
		facet->nodes[i].attrib = 0. ; // 初始化属性
	}

	for( i = 0 ; i < nu-1 ; i++ )
	{
		facet->quads[i][0] = i ;
		facet->quads[i][1] = i+1 ;
		facet->quads[i][2] = i+1+nu ;
		facet->quads[i][3] = i+nu ;
	}
	facet->quads[nu-1][0] = nu-1 ;
	facet->quads[nu-1][1] = 0 ;
	facet->quads[nu-1][2] = nu ;
	facet->quads[nu-1][3] = 2*nu-1 ;

	return facet ;
}

// nt add 2017/5/30
FACET* facetCreate4(int nu)
{
	int i ;
	FACET* facet = NULL ;
	
	facet = facetCreate(2*nu+2, 2*nu, nu) ;
	if( !facet )
		return NULL ; // error

	for( i = 0 ; i < facet->nnode ; i++ )
	{
		facet->nodes[i].uv[0] = 0. ; // 初始化u
		facet->nodes[i].uv[1] = 0. ; // 初始化v
		facet->nodes[i].attrib = 0. ; // 初始化属性
	}

	for( i = 0 ; i < nu ; i++ )
	{
		facet->trias[i][0] = 2*nu ;
		facet->trias[i][1] = (i+1)<nu?(i+1):0 ;
		facet->trias[i][2] = i ;

		facet->trias[i+nu][0] = 2*nu+1 ;
		facet->trias[i+nu][1] = nu+i ;
		facet->trias[i+nu][2] = (i+1)<nu?(nu+i+1):nu ;
	}

	for( i = 0 ; i < nu-1 ; i++ )
	{
		facet->quads[i][0] = i ;
		facet->quads[i][1] = i+1 ;
		facet->quads[i][2] = i+1+nu ;
		facet->quads[i][3] = i+nu ;
	}
	facet->quads[nu-1][0] = nu-1 ;
	facet->quads[nu-1][1] = 0 ;
	facet->quads[nu-1][2] = nu ;
	facet->quads[nu-1][3] = 2*nu-1 ;

	return facet ;
}

FACET* facetCylinder(PNT3D center, VEC3D Z, double r, double h, int nu, int nv)
{
	RFRAME lf ;
	BOXUV boxuv ;
	SURFACE* surface ;
	FACET* facet ;

	if( r < MIN_RAD ||
		h < MIN_LEN )
		return NULL ;

	memcpy(lf.Z, Z, sizeof(VEC3D)) ;
	mathGetXYFromZ(Z, lf.X, lf.Y) ;
	memcpy(lf.O, center, sizeof(PNT3D)) ;
	lf.scale = 1. ;
	surface = cylinderCreate(&lf, r) ;
	boxuv.min[0] = 0. ;
	boxuv.max[0] = PI2 ;
	boxuv.min[1] = 0. ;
	boxuv.max[1] = h ;
	facet = facetCreate2(surface, &boxuv, nu, nv) ;
	entityFree(surface) ;

	return facet ;
}

FACET* facetCylinder2(PNT3D p1, PNT3D p2, double r, int nu)
{
	int i ;
	double h, a ;
	VEC3D X, Y, Z ;
	FACET* facet = NULL ;

	mathGetVec(p1, p2, Z) ;
	h = mathDist(p1, p2) ;
	if( h < MIN_LEN )
		return NULL ;
	Z[0] /= h ;
	Z[1] /= h ;
	Z[2] /= h ;
	mathGetXYFromZ(Z, X, Y) ;

	facet = facetCreate3(nu) ;
	if( facet == NULL )
		return NULL ;
	mathEvalLin(p1, X, r, facet->nodes[0].p) ;
	memcpy(facet->nodes[0].normal, X, sizeof(VEC3D)) ;
	mathEvalLin(p2, X, r, facet->nodes[nu].p) ;
	memcpy(facet->nodes[nu].normal, X, sizeof(VEC3D)) ;

	a = PI2/nu ;
	for( i = 1 ; i < nu ; i++ )
	{
		mathRotPnt(Z, p1, i*a, facet->nodes[0].p, facet->nodes[i].p) ;
		mathRotVec(Z, p1, i*a, X, facet->nodes[i].normal) ;
		mathRotPnt(Z, p1, i*a, facet->nodes[nu].p, facet->nodes[i+nu].p) ;
		mathRotVec(Z, p1, i*a, X, facet->nodes[i+nu].normal) ;
	}

	return facet ;
}

// nt add 2017/5/30
FACET* facetCylinder3(PNT3D p1, PNT3D p2, double r, int nu)
{
	int i ;
	double h, a ;
	VEC3D X, Y, Z ;
	FACET* facet = NULL ;

	mathGetVec(p1, p2, Z) ;
	h = mathDist(p1, p2) ;
	if( h < MIN_LEN )
		return NULL ;
	Z[0] /= h ;
	Z[1] /= h ;
	Z[2] /= h ;
	mathGetXYFromZ(Z, X, Y) ;

	facet = facetCreate4(nu) ;
	if( facet == NULL )
		return NULL ;
	mathEvalLin(p1, X, r, facet->nodes[0].p) ;
	memcpy(facet->nodes[0].normal, X, sizeof(VEC3D)) ;
	mathEvalLin(p2, X, r, facet->nodes[nu].p) ;
	memcpy(facet->nodes[nu].normal, X, sizeof(VEC3D)) ;

	a = PI2/nu ;
	for( i = 1 ; i < nu ; i++ )
	{
		mathRotPnt(Z, p1, i*a, facet->nodes[0].p, facet->nodes[i].p) ;
		mathRotVec(Z, p1, i*a, X, facet->nodes[i].normal) ;
		mathRotPnt(Z, p1, i*a, facet->nodes[nu].p, facet->nodes[i+nu].p) ;
		mathRotVec(Z, p1, i*a, X, facet->nodes[i+nu].normal) ;
	}

	memcpy(facet->nodes[2*nu].p, p1, sizeof(PNT3D)) ;
	memcpy(facet->nodes[2*nu+1].p, p2, sizeof(PNT3D)) ;

	return facet ;
}

FACET* facetCone(PNT3D center, VEC3D Z, double r, double h, int nu, int nv)
{
	RFRAME lf ;
	BOXUV boxuv ;
	SURFACE* surface ;
	FACET* facet ;

	if( r < MIN_RAD ||
		h < MIN_LEN )
		return NULL ;

	memcpy(lf.Z, Z, sizeof(VEC3D)) ;
	mathGetXYFromZ(Z, lf.X, lf.Y) ;
	memcpy(lf.O, center, sizeof(PNT3D)) ;
	lf.scale = 1. ;
	surface = coneCreate(&lf, r/h) ;
	boxuv.min[0] = 0. ;
	boxuv.max[0] = PI2 ;
	boxuv.min[1] = 0. ;
	boxuv.max[1] = h ;
	facet = facetCreate2(surface, &boxuv, nu, nv) ;
	entityFree(surface) ;

	return facet ;
}

FACET* facetCone2(PNT3D p1, PNT3D p2, double r1, double r2, int nu)
{
	int i ;
	double h, a ;
	VEC3D X, Y, Z ;
	FACET* facet = NULL ;

	mathGetVec(p1, p2, Z) ;
	h = mathDist(p1, p2) ;
	if( h < MIN_LEN )
		return NULL ;
	Z[0] /= h ;
	Z[1] /= h ;
	Z[2] /= h ;
	mathGetXYFromZ(Z, X, Y) ;
	a = atan((r1-r2)/h) ;

	facet = facetCreate3(nu) ;
	if( facet == NULL )
		return NULL ;
	mathEvalLin(p1, X, r1, facet->nodes[0].p) ;
	mathMixVec(X, Z, cos(a), sin(a), Y) ;
	memcpy(facet->nodes[0].normal, Y, sizeof(VEC3D)) ;
	mathEvalLin(p2, X, r2, facet->nodes[nu].p) ;
	memcpy(facet->nodes[nu].normal, Y, sizeof(VEC3D)) ;

	a = PI2/nu ;
	for( i = 1 ; i < nu ; i++ )
	{
		mathRotPnt(Z, p1, i*a, facet->nodes[0].p, facet->nodes[i].p) ;
		mathRotVec(Z, p1, i*a, Y, facet->nodes[i].normal) ;
		mathRotPnt(Z, p1, i*a, facet->nodes[nu].p, facet->nodes[i+nu].p) ;
		mathRotVec(Z, p1, i*a, Y, facet->nodes[i+nu].normal) ;
	}

	return facet ;
}

// nt add 2017/5/30
FACET* facetCone3(PNT3D p1, PNT3D p2, double r1, double r2, int nu)
{
	int i ;
	double h, a ;
	VEC3D X, Y, Z ;
	FACET* facet = NULL ;

	mathGetVec(p1, p2, Z) ;
	h = mathDist(p1, p2) ;
	if( h < MIN_LEN )
		return NULL ;
	Z[0] /= h ;
	Z[1] /= h ;
	Z[2] /= h ;
	mathGetXYFromZ(Z, X, Y) ;
	a = atan((r1-r2)/h) ;

	facet = facetCreate4(nu) ;
	if( facet == NULL )
		return NULL ;
	mathEvalLin(p1, X, r1, facet->nodes[0].p) ;
	mathMixVec(X, Z, cos(a), sin(a), Y) ;
	memcpy(facet->nodes[0].normal, Y, sizeof(VEC3D)) ;
	mathEvalLin(p2, X, r2, facet->nodes[nu].p) ;
	memcpy(facet->nodes[nu].normal, Y, sizeof(VEC3D)) ;

	a = PI2/nu ;
	for( i = 1 ; i < nu ; i++ )
	{
		mathRotPnt(Z, p1, i*a, facet->nodes[0].p, facet->nodes[i].p) ;
		mathRotVec(Z, p1, i*a, Y, facet->nodes[i].normal) ;
		mathRotPnt(Z, p1, i*a, facet->nodes[nu].p, facet->nodes[i+nu].p) ;
		mathRotVec(Z, p1, i*a, Y, facet->nodes[i+nu].normal) ;
	}

	memcpy(facet->nodes[2*nu].p, p1, sizeof(PNT3D)) ;
	facet->nodes[2*nu].normal[0] = -Z[0] ; // nt add 2017/6/9
	facet->nodes[2*nu].normal[1] = -Z[1] ; // nt add 2017/6/9
	facet->nodes[2*nu].normal[2] = -Z[2] ; // nt add 2017/6/9
	memcpy(facet->nodes[2*nu+1].p, p2, sizeof(PNT3D)) ;
	memcpy(facet->nodes[2*nu+1].normal, Z, sizeof(VEC3D)) ; // nt add 2017/6/9

	return facet ;
}

FACET* facetSphere(PNT3D center, double r, int nu, int nv)
{
	RFRAME lf ;
	BOXUV boxuv ;
	SURFACE* surface ;
	FACET* facet ;

	if( r < MIN_RAD )
		return NULL ;

	mathInitRFrame(&lf) ;
	memcpy(lf.O, center, sizeof(PNT3D)) ;
	surface = sphereCreate(&lf, r) ;
	surfaceGetBoxUV(surface, &boxuv) ;
	facet = facetCreate2(surface, &boxuv, nu, nv) ;
	entityFree(surface) ;

	return facet ;
}

FACET* facetTorus(PNT3D center, VEC3D Z, double r1, double r2, int nu, int nv)
{
	RFRAME lf ;
	BOXUV boxuv ;
	SURFACE* surface ;
	FACET* facet ;

	if( r1 < MIN_RAD ||
		r2 < MIN_LEN )
		return NULL ;

	memcpy(lf.Z, Z, sizeof(VEC3D)) ;
	mathGetXYFromZ(Z, lf.X, lf.Y) ;
	memcpy(lf.O, center, sizeof(PNT3D)) ;
	lf.scale = 1. ;
	surface = torusCreate(&lf, r1, r2) ;
	surfaceGetBoxUV(surface, &boxuv) ;
	facet = facetCreate2(surface, &boxuv, nu, nv) ;
	entityFree(surface) ;

	return facet ;
}

int facetsBoundCell(PNT3D center, CELL * cell, double r1, double r2, int nu, FACET * facets[5])
{
	int i, nfacets = 0;
	for (i = 0; i < 4; i++)
	{
		if (cell->flags)
		{
			facets[nfacets++] = facetCylinder3(cell->begin[i], cell->end[i], r1, nu);
		}
	}
	if (cell->flagC)
	{
		facets[nfacets++] = facetSphere(center, r2, nu, nu / 2);
	}
	return nfacets;
}

/*int facetsCell(PNT3D center, double w, double r1, double r2, int nu, FACET* facets[17])
{
	int i, j, k ;
	double h, vs[2][2][2][3] ;

	h = 0.5*w ; // nt comment 2021/6/7 注意不能减掉了r1!!!
	for( i = 0 ; i < 2 ; i++ )
	{
		for( j = 0 ; j < 2 ; j++ )
		{
			for( k = 0 ; k < 2 ; k++ )
			{
				vs[i][j][k][0] = center[0]+(i?h:-h) ;
				vs[i][j][k][1] = center[1]+(j?h:-h) ;
				vs[i][j][k][2] = center[2]+(k?h:-h) ;
			}
		}
	}

	facets[0] = facetCylinder3(vs[0][0][0], vs[1][0][0], r1, nu) ;
	facets[1] = facetCylinder3(vs[1][0][0], vs[1][1][0], r1, nu) ;
	facets[2] = facetCylinder3(vs[1][1][0], vs[0][1][0], r1, nu) ;
	facets[3] = facetCylinder3(vs[0][1][0], vs[0][0][0], r1, nu) ;

	facets[4] = facetCylinder3(vs[0][0][1], vs[1][0][1], r1, nu) ;
	facets[5] = facetCylinder3(vs[1][0][1], vs[1][1][1], r1, nu) ;
	facets[6] = facetCylinder3(vs[1][1][1], vs[0][1][1], r1, nu) ;
	facets[7] = facetCylinder3(vs[0][1][1], vs[0][0][1], r1, nu) ;

	facets[8] = facetCylinder3(vs[0][0][0], vs[0][0][1], r1, nu) ;
	facets[9] = facetCylinder3(vs[1][0][0], vs[1][0][1], r1, nu) ;
	facets[10] = facetCylinder3(vs[1][1][0], vs[1][1][1], r1, nu) ;
	facets[11] = facetCylinder3(vs[0][1][0], vs[0][1][1], r1, nu) ;

	facets[12] = facetCylinder3(vs[0][0][0], vs[1][1][1], r1, nu) ;
	facets[13] = facetCylinder3(vs[0][0][1], vs[1][1][0], r1, nu) ;
	facets[14] = facetCylinder3(vs[0][1][0], vs[1][0][1], r1, nu) ;
	facets[15] = facetCylinder3(vs[0][1][1], vs[1][0][0], r1, nu) ;

	facets[16] = facetSphere(center, r2, nu, nu/2) ;

	for( i = 0 ; i < 17 ; i++ )
	{
		if( facets[i] == NULL )
			return -1 ;
	}

	return 17 ;
}*/
int facetsCell(PNT3D center, double w, double r1, double r2, int nu, FACET* facets[5])
{
	int i, j, k ;
	double h, vs[2][2][2][3] ;

	h = 0.5*w ;
	for( i = 0 ; i < 2 ; i++ )
	{
		for( j = 0 ; j < 2 ; j++ )
		{
			for( k = 0 ; k < 2 ; k++ )
			{
				vs[i][j][k][0] = center[0]+(i?h:-h) ;
				vs[i][j][k][1] = center[1]+(j?h:-h) ;
				vs[i][j][k][2] = center[2]+(k?h:-h) ;
			}
		}
	}

	facets[0] = facetCylinder3(vs[0][0][0], vs[1][1][1], r1, nu) ;
	facets[1] = facetCylinder3(vs[0][0][1], vs[1][1][0], r1, nu) ;
	facets[2] = facetCylinder3(vs[0][1][0], vs[1][0][1], r1, nu) ;
	facets[3] = facetCylinder3(vs[0][1][1], vs[1][0][0], r1, nu) ;

	facets[4] = facetSphere(center, r2, nu, nu/2) ;

	return 5 ;
}

void facetMove(FACET* facet, VEC3D v)
{
	for( int i = 0 ; i < facet->nnode ; i++ )
	{
		facet->nodes[i].p[0] += v[0] ;
		facet->nodes[i].p[1] += v[1] ;
		facet->nodes[i].p[2] += v[2] ;
	}

	return ;
}

void facetRotate(FACET* facet, PNT3D pivot, VEC3D axis, double a)
{
	PNT3D p ;
	VEC3D normal ;

	for( int i = 0 ; i < facet->nnode ; i++ )
	{
		mathRotPnt(axis, pivot, a, facet->nodes[i].p, p) ;
		memcpy(facet->nodes[i].p, p, sizeof(PNT3D)) ;
		mathRotVec(axis, pivot, a, facet->nodes[i].normal, normal) ;
		memcpy(facet->nodes[i].normal, normal, sizeof(PNT3D)) ;
	}

	return ;
}

// 注意这里不用copy法矢
void facetsCopyMove(int nFacet, FACET** facets1, VEC3D v, FACET** facets2)
{
	int i, j ;

	for( i = 0 ; i < nFacet ; i++ )
	{
		for( j = 0 ; j < facets1[i]->nnode ; j++ )
		{
			facets2[i]->nodes[j].p[0] = facets1[i]->nodes[j].p[0]+v[0] ;
			facets2[i]->nodes[j].p[1] = facets1[i]->nodes[j].p[1]+v[1] ;
			facets2[i]->nodes[j].p[2] = facets1[i]->nodes[j].p[2]+v[2] ;
		}
	}

	return ;
}

void facetDraw0(FACET* facet)
{
	int i ;

	if( facet == NULL )
		return ;

	glColor3ub(0, 0, 0) ;
	glPointSize(1.f) ;
	glBegin(GL_POINTS) ;
		for( i = 0 ; i < facet->nnode ; i++ )
			glVertex3dv(facet->nodes[i].p) ;
	glEnd() ;

	return ;
}

void facetDraw1(FACET* facet)
{
	int i, I, J, K, L ;

	if( facet == NULL )
		return ;

	glColor3ub(0, 0, 0) ;
	glLineWidth(1.f) ;
	
	for(i = 0 ; i < facet->ntria ; i++ )
	{
		I = facet->trias[i][0] ;
		J = facet->trias[i][1] ;
		K = facet->trias[i][2] ;
		glBegin(GL_LINE_LOOP) ;
			glVertex3dv(facet->nodes[I].p) ;
			glVertex3dv(facet->nodes[J].p) ;
			glVertex3dv(facet->nodes[K].p) ;
		glEnd() ;
	}

	for(i = 0 ; i < facet->nquad ; i++ )
	{
		I = facet->quads[i][0] ;
		J = facet->quads[i][1] ;
		K = facet->quads[i][2] ;
		L = facet->quads[i][3] ;
		glBegin(GL_LINE_LOOP) ;
			glVertex3dv(facet->nodes[I].p) ;
			glVertex3dv(facet->nodes[J].p) ;
			glVertex3dv(facet->nodes[K].p) ;
			glVertex3dv(facet->nodes[L].p) ;
		glEnd() ;
	}
	
	return ;
}

void facetDraw2(FACET* facet)
{
	glVertexPointer(3, GL_DOUBLE, sizeof(FNODE), facet->nodes[0].p) ;
	glNormalPointer(GL_DOUBLE, sizeof(FNODE), facet->nodes[0].normal) ;
	if( facet->trias )
		glDrawElements(GL_TRIANGLES, facet->ntria*3, GL_UNSIGNED_INT, facet->trias) ;
	if( facet->quads )
		glDrawElements(GL_QUADS, facet->nquad*4, GL_UNSIGNED_INT, facet->quads) ;

	return ;
}

double facetGetMinZ(FACET* facet)
{
	int i ;
	double zmin = MAX_DBL ;

	if( facet == NULL )
		return 0. ;

	for( i = 0 ; i < facet->nnode ; i++ )
	{
		if( zmin > facet->nodes[i].p[2] )
			zmin = facet->nodes[i].p[2] ;
	}
	
	return zmin ;
}

// nt add 2017/9/4
FACET* facetCopy(FACET* facet)
{
	FACET* copy = facetCreate(facet->nnode, facet->ntria, facet->nquad) ;
	if( facet->nodes )
		memcpy(copy->nodes, facet->nodes, sizeof(FNODE)*facet->nnode) ;
	if( facet->ntria )
		memcpy(copy->trias, facet->trias, sizeof(int[3])*facet->ntria) ;
	if( facet->nodes )
		memcpy(copy->quads, facet->quads, sizeof(int[4])*facet->nquad) ;

	return copy ;
}

// nt add 2017/8/31
int facetSerialize(FACET* facet, CArchive& ar)
{
	if( facet == NULL )
		return 0 ;

	if( ar.IsStoring() )
	{
		ar << facet->nnode
		   << facet->ntria
		   << facet->nquad ;
		if( facet->nnode > 0 )
			ar.Write(facet->nodes, sizeof(FNODE)*facet->nnode) ;
		if( facet->ntria > 0 )
			ar.Write(facet->trias, sizeof(int[3])*facet->ntria) ;
		if( facet->nquad > 0 )
			ar.Write(facet->quads, sizeof(int[4])*facet->nquad) ;
	}
	else
	{
		ar >> facet->nnode
		   >> facet->ntria
		   >> facet->nquad ;
		if( facet->nnode > 0 )
		{
			facet->nodes = (FNODE*)envAlloc(sizeof(FNODE)*facet->nnode) ;
			ar.Read(facet->nodes, sizeof(FNODE)*facet->nnode) ;
		}
		if( facet->ntria > 0 )
		{
			facet->trias = (int(*)[3])envAlloc(sizeof(int[3])*facet->ntria) ;
			ar.Read(facet->trias, sizeof(int[3])*facet->ntria) ;
		}
		if( facet->nquad > 0 )
		{
			facet->quads = (int(*)[4])envAlloc(sizeof(int[4])*facet->nquad) ;
			ar.Read(facet->quads, sizeof(int[4])*facet->nquad) ;
		}
	}

	return 1 ;
}
//-------------------------------------------------------------
/*  2017/1/18 nt
 *  该平面过原点，其法矢为normal, 将X、Y轴也投影到该平面上得到X'、Y',
 *  将p投影到该平面上，并相对于X'、Y'进行分解
 */
int mathPrjPntPlnEx(PNT3D p, VEC3D normal, PNT2D decomp)
{
	int rtx, rty ;
	PNT3D q, q1, q2 ;
	VEC3D X, Y ;
	RFRAME lf ;

	mathInitRFrame(&lf) ;
	mathPrjPntPln(p, lf.O, normal, q) ;
	mathPrjVecPln(lf.X, normal, X) ;
	mathPrjVecPln(lf.Y, normal, Y) ;
	rtx = mathUniVec(X, MIN_LEN) ;
	rty = mathUniVec(Y, MIN_LEN) ;
	if( rtx != ERSUCSS &&
		rty != ERSUCSS )
		return 0 ; // error
	if( rtx != ERSUCSS )
	{
		mathVProduct(Y, normal, X) ;
		mathUniVec(X, MIN_LEN) ;
	}
	if( rty != ERSUCSS )
	{
		mathVProduct(X, normal, Y) ;
		mathUniVec(Y, MIN_LEN) ;
	}
	
	if( mathIntLin(q, Y, lf.O, X, MIN_LEN, MIN_ANG, q1, q2) != IDINT )
		return 0 ;
	decomp[0] = q1[0]*X[0]+q1[1]*X[1]+q1[2]*X[2] ;

	if( mathIntLin(q, X, lf.O, Y, MIN_LEN, MIN_ANG, q1, q2) != IDINT )
		return 0 ;
	decomp[1] = q1[0]*Y[0]+q1[1]*Y[1]+q1[2]*Y[2] ;

	return 1 ;
}

// 见mathPrjPntPlnEx()
int mathDecVecXY(PNT3D p, PNT3D q, VEC3D normal, VEC2D decomp)
{
	PNT3D dP, dQ ;

	if( mathPrjPntPlnEx(p, normal, dP) == 0 ||
		mathPrjPntPlnEx(q, normal, dQ) == 0 )
		return 0 ;
	decomp[0] = dQ[0]-dP[0] ;
	decomp[1] = dQ[1]-dP[1] ;

	return 1 ;
}
//--------------------------------------------------------------
// nt add 2015/9/13
int strToNums(TCHAR* str, int& i)
{
	int k, n = (int)_tcslen(str) ;
	i = 0 ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == _T('-') ||
			(str[k] >= _T('0') &&
			str[k] <= _T('9')) )
		{
			if( _stscanf_s(str+k, _T("%d"), &i) == 1 )
				return 1 ;
			else
				return 0 ;
		}
	}
	return 0 ;
}

int strToNums(CString& str, int& i)
{
	return strToNums(str.GetBuffer(0), i) ;
}

// nt add 2015/9/13
int strToNums(TCHAR* str, double& d)
{
	int k = 0, n = (int)_tcslen(str) ;
	d = 0. ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == _T('-') ||
			str[k] == _T('.') ||
			(str[k] >= _T('0') &&
			str[k] <= _T('9')) )
		{
			if( _stscanf_s(str+k, _T("%lf"), &d) == 1 )
				return 1 ;
			else
				return 0 ;
		}
	}
	return 0 ;
}

int strToNums(CString& str, double& d)
{
	return strToNums(str.GetBuffer(0), d) ;
}

// nt add 2015/9/13
int strToNums(TCHAR* str, int& i1, int& i2)
{
	int k, n = (int)_tcslen(str), m = 0 ;
	i1 = 0 ;
	i2 = 0 ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == _T('-') ||
			(str[k] >= _T('0') &&
			str[k] <= _T('9')) )
		{
			if( m < 1 )
			{
				if( _stscanf_s(str+k, _T("%d"), &i1) != 1 )
					return 0 ;
			}
			else
			{
				if( _stscanf_s(str+k, _T("%d"), &i2) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 2 )
				return 2 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == _T(',') ||
					str[k] == _T(' ') )
					break ;
			}
		}
	}
	return m ;
}

int strToNums(CString& str, int& i1, int& i2)
{
	return strToNums(str.GetBuffer(0), i1, i2) ;
}

// nt add 2015/9/13
int strToNums(TCHAR* str, double& d1, double& d2)
{
	int k, n = (int)_tcslen(str), m = 0 ;
	d1 = 0. ;
	d2 = 0. ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == _T('-') ||
			str[k] == _T('.') ||
			(str[k] >= _T('0') &&
			str[k] <= _T('9')) )
		{
			if( m < 1 )
			{
				if( _stscanf_s(str+k, _T("%lf"), &d1) != 1 )
					return 0 ;
			}
			else
			{
				if( _stscanf_s(str+k, _T("%lf"), &d2) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 2 )
				return 2 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == _T(',') ||
					str[k] == _T(' ') )
					break ;
			}
		}
	}
	return m ;
}

int strToNums(CString& str, double& d1, double& d2)
{
	return strToNums(str.GetBuffer(0), d1, d2) ;
}

// nt add 2015/9/13
int strToNums(TCHAR* str, int& i, double& d)
{
	int k, n = (int)_tcslen(str), m = 0 ;
	i = 0 ;
	d = 0. ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == _T('-') ||
			(str[k] == _T('.') &&
			m == 1) ||
			(str[k] >= _T('0') &&
			str[k] <= _T('9')) )
		{
			if( m < 1 )
			{
				if( _stscanf_s(str+k, _T("%d"), &i) != 1 )
					return 0 ;
			}
			else
			{
				if( _stscanf_s(str+k, _T("%lf"), &d) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 2 )
				return 2 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == _T(',') ||
					str[k] == _T(' ') )
					break ;
			}
		}
	}
	return m ;
}

int strToNums(CString& str, int& i, double& d)
{
	return strToNums(str.GetBuffer(0), i, d) ;
}

// nt add 2015/9/13
int strToNums(TCHAR* str, double& d, int& i)
{
	int k, n = (int)_tcslen(str), m = 0 ;
	d = 0. ;
	i = 0 ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == _T('-') ||
			(str[k] == _T('.') &&
			m == 0) ||
			(str[k] >= _T('0') &&
			str[k] <= _T('9')) )
		{
			if( m < 1 )
			{
				if( _stscanf_s(str+k, _T("%lf"), &d) != 1 )
					return 0 ;
			}
			else
			{
				if( _stscanf_s(str+k, _T("%d"), &i) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 2 )
				return 2 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == _T(',') ||
					str[k] == _T(' ') )
					break ;
			}
		}
	}
	return m ;
}

int strToNums(CString& str, double& d, int& i)
{
	return strToNums(str.GetBuffer(0), d, i) ;
}

// nt add 2015/9/13
int strToNums(TCHAR* str, 
			  double& d1, 
			  double& d2, 
			  double& d3)
{
	int k, n = (int)_tcslen(str), m = 0 ;
	d1 = 0. ;
	d2 = 0. ;
	d3 = 0. ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == _T('-') ||
			str[k] == _T('.') ||
			(str[k] >= _T('0') &&
			str[k] <= _T('9')) )
		{
			if( m == 0 )
			{
				if( _stscanf_s(str+k, _T("%lf"), &d1) != 1 )
					return 0 ;
			}
			else
			if( m == 1 )
			{
				if( _stscanf_s(str+k, _T("%lf"), &d2) != 1 )
					return 0 ;
			}
			else
			{
				if( _stscanf_s(str+k, _T("%lf"), &d3) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 3 )
				return 3 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == _T(',') ||
					str[k] == _T(' ') )
					break ;
			}
		}
	}
	return m ;
}

int strToNums(CString& str, 
			  double& d1, 
			  double& d2, 
			  double& d3)
{
	return strToNums(str.GetBuffer(0), d1, d2, d3) ;
}

// nt add 2015/9/13
int strToNums(TCHAR* str, 
			  double& d1, 
			  double& d2, 
			  int& i)
{
	int k, n = (int)_tcslen(str), m = 0 ;
	d1 = 0. ;
	d2 = 0. ;
	i = 0 ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == _T('-') ||
			str[k] == _T('.') ||
			(str[k] >= _T('0') &&
			str[k] <= _T('9')) )
		{
			if( m == 0 )
			{
				if( _stscanf_s(str+k, _T("%lf"), &d1) != 1 )
					return 0 ;
			}
			else
			if( m == 1 )
			{
				if( _stscanf_s(str+k, _T("%lf"), &d2) != 1 )
					return 0 ;
			}
			else
			{
				if( _stscanf_s(str+k, _T("%d"), &i) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 3 )
				return 3 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == _T(',') ||
					str[k] == _T(' ') )
					break ;
			}
		}
	}
	return m ;
}

int strToNums(CString& str, 
			  double& d1, 
			  double& d2, 
			  int& i)
{
	return strToNums(str.GetBuffer(0), d1, d2, i) ;
}
//-------------------------------------------------------------
// nt modify 2015/9/14
void efpGet(CString& efp)
{
	TCHAR buf[512] ;

	HMODULE hModule = ::GetModuleHandle(NULL) ;
	::GetModuleFileName(hModule, buf, 512) ;
	for( int i = (int)_tcslen(buf)-1, n = 0 ; i >= 0 && n < 2 ; i-- )
	{
		if( buf[i] == _T('\\') )
			n++ ;
		buf[i] = _T('\0') ;
	}
	// nt add 2012/12/17
	int n = (int)_tcslen(buf) ;
	if( buf[n-1] == _T('\\') ||
		buf[n-1] == _T('.') )
	{
		buf[n-1] = _T('\0') ;
		buf[n-2] = _T('\0') ;
	}
	efp = buf ;

	return ;
}
//-------------------------------------------------------------
void cstrWrite(CString& str, CArchive& ar)
{
	int n = str.GetLength() ;
	ar << n ;
	if( n > 0 )
		ar.Write(str.GetBuffer(0), n*sizeof(char)) ;
}

void cstrRead(CString& str, CArchive& ar)
{
	int n ;
	ar >> n ;
	if( n > 0 )
	{
		char* buf = new char[n+1] ;
		ar.Read(buf, n*sizeof(char)) ;
		str = buf ;
		delete []buf ;
	}
}
//--------------------------------------------------------------
int comboLoad(CString& filePath, CComboBox& combo)
{
	int n = -1 ;
	char buf[128] ;
	CString text ;
	FILE* fp ;
	_tfopen_s(&fp, filePath, _T("r+t")) ;
	if( fp )
	{
		n = 0 ;
		while(1)
		{
			if( fgets(buf, 128, fp) )
			{
				text = buf ;
				combo.AddString(text) ;
				n++ ;
			}
			else
				break ; // end of the file
		}
		fclose(fp) ;
	}
	
	return n ;
}
//-------------------------------------------------------------
// nt add 2017/9/9
PRG* prgCreate()
{
	PRG* pPrg = new PRG() ;
	pPrg->n = 0 ;
	pPrg->ws = NULL ;
	pPrg->i = 0 ;
	pPrg->c = 0. ;
	pPrg->finished = 0 ;

	return pPrg ;
}

void prgFree(PRG* pPrg)
{
	if( pPrg )
	{
		if( pPrg->ws )
			delete []pPrg->ws ;
		delete pPrg ;
	}

	return ;
}

void prgReleaseBuf(PRG* pPrg)
{
	if( pPrg &&
		pPrg->ws )
	{
		delete []pPrg->ws ;
		pPrg->ws = NULL ;
	}

	return ;
}

void prgInit(PRG* pPrg, int n)
{
	if( pPrg )
	{
		if( n < 1 )
			pPrg->n = 0 ;
		else
		{
			if( pPrg->ws )
				delete []pPrg->ws ;
			pPrg->ws = new double[n] ;
			for( int i = 0 ; i < n ; i++ )
				pPrg->ws[i] = 0. ;
			pPrg->n = n ;
		}
		pPrg->finished = 0 ;
		pPrg->c = 0. ;
		pPrg->i = 0 ;
	}

	return ;
}

int prgGetPercent(PRG* pPrg)
{
	if( pPrg->finished == 1 )
		return 100 ;
	else
		return (int)(100*pPrg->c) ;
}

void prgFinish(PRG* pPrg)
{
	if( pPrg )
		pPrg->finished = 1 ;

	return ;
}
//--------------------------------------------------------------
// nt add 2017/10/1
int triInt(PNT3D A,
		   PNT3D B,
		   PNT3D C,
		   PNT3D P,
		   PNT3D Q,
		   PNT3D R)
{
	int k ;
	XMVECTOR ps[3], qs[3] ;

	for( k = 0 ; k < 3 ; k++ )
	{
		ps[0].m128_f32[k] = (float)A[k] ;
		ps[1].m128_f32[k] = (float)B[k] ;
		ps[2].m128_f32[k] = (float)C[k] ;
		qs[0].m128_f32[k] = (float)P[k] ;
		qs[1].m128_f32[k] = (float)Q[k] ;
		qs[2].m128_f32[k] = (float)R[k] ;
	}

	return (int)TriangleTests::Intersects(ps[0], ps[1], ps[2], qs[0], qs[1], qs[2]) ;
}
//--------------------------------------------------------------
// smf add 2023/02/21
int mathSegmIntSegmInFace(PNT3D iBegin1, PNT3D iEnd1, PNT3D iBegin2, PNT3D iEnd2, double iTol, PNT3D oIntPoint)
{
	VEC3D v1, v2, v_b1_b2;
	for (size_t i = 0; i < 3; i++)
	{
		v1[i] = iEnd1[i] - iBegin1[i];
		v2[i] = iEnd2[i] - iBegin2[i];
		v_b1_b2[i] = iBegin2[i] - iBegin1[i];
	}
	
	double det_xy, det_yz, det_xz;

	det_xy = mathDet22(v1[0], -v2[0], v1[1], -v2[1]);
	det_yz = mathDet22(v1[1], -v2[2], v1[2], -v2[2]);
	det_xz = mathDet22(v1[0], -v2[0], v1[2], -v2[2]);

	double t1 = -1000, t2 = -1000;
	if (det_xy < -iTol || 
		det_xy > iTol)
	{
		double det_1 = mathDet22(v_b1_b2[0], -v2[0], v_b1_b2[1], -v2[1]);
		t1 = det_1 / det_xy;
		double det_2 = mathDet22(v1[0], v_b1_b2[0], v1[1], v_b1_b2[1]);
		t2 = det_2 / det_xy;
	}
	else 
	if (det_yz < -iTol ||
		det_yz > iTol)
	{
		double det_1 = mathDet22(v_b1_b2[1], -v2[1], v_b1_b2[2], -v2[2]);
		t1 = det_1 / det_yz;
		double det_2 = mathDet22(v1[1], v_b1_b2[1], v1[2], v_b1_b2[2]);
		t2 = det_2 / det_yz;
	}
	else 
	if (det_xz < -iTol ||
		det_xz > iTol)
	{
		double det_1 = mathDet22(v_b1_b2[0], -v2[0], v_b1_b2[2], -v2[2]);
		t1 = det_1 / det_xz;
		double det_2 = mathDet22(v1[0], v_b1_b2[0], v1[2], v_b1_b2[2]);
		t2 = det_2 / det_xz;
	}

	if (t1 >= 0. && t1 <= 1. &&
		t2 >= 0. && t2 <= 1.) // 交点在线段上
	{
		for (int i = 0; i < 3; i++)
		{
			oIntPoint[i] = iBegin1[i] + t1 * v1[i];
		}
		return IDINT;
	}
	return IDNOINT;
}

// |A|
double mathDet22(double a11, double a12, double a21, double a22)
{
	double det = a11 * a22 - a12 * a21;
	return det;
}
///////////////////////////////////////////////////////////////