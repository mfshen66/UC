#include "StdAfx.h"
#include "CB.h" // nt add 2021/6/5
#include "ZB2.h"

////////////////////////////////////////////////////////////////
void isegmInit(ISEGM* segm)
{
	if( segm )
	{
		segm->next = NULL ;
		segm->facetId = 0 ;
		segm->z1 = -MAX_DBL ;
		segm->z2 = -MAX_DBL ;
	}

	return ;
}

void isegmInit2(ISEGM* segm, double z, UINT64 facetId)
{
	if( segm )
	{
		segm->next = NULL ;
		segm->facetId = facetId ;
		segm->z1 = -MAX_DBL ;
		segm->z2 = z ;
	}

	return ;
}

int isegmMerge(ISEGM* segm, double z)
{
	if( segm )
	{
		if( segm->z2 < 1-MAX_DBL )
			return 0 ;
		if( segm->z1 < 1-MAX_DBL )
		{
			segm->z1 = mathMIN(segm->z2, z) ;
			segm->z2 = mathMAX(segm->z2, z) ;
		}
		else
		{
			segm->z1 = mathMIN(segm->z1, z) ;
			segm->z2 = mathMAX(segm->z2, z) ;
		}
		return 1 ;
	}

	return 0 ;
}
//==============================================================
void sline2Init(SLINE2* line, PNT3D p)
{
	if( line &&
		p )
	{
		line->n = 0 ;
		memcpy(line->p, p, sizeof(PNT3D)) ;
		line->segms = NULL ;
	}

	return ;
}

void sline2Init2(SLINE2* line, double x, double y)
{
	if( line )
	{
		line->n = 0 ;
		line->p[0] = x ;
		line->p[1] = y ;
		line->p[2] = 0. ;
		line->segms = NULL ;
	}

	return ;
}

void sline2FreeSegmAll(SLINE2* line)
{
	ISEGM* segm, *next ;

	segm = line->segms ;
	while(segm)
	{
		next = segm->next ;
		free(segm) ;
		segm = next ;
	}

	return ;
}

int sline2Add(SLINE2* line, double z, UINT64 facetId)
{
	if( line )
	{
		if( line->segms && // 对于ISEGM需要进行合并的情况
			line->segms->facetId == facetId )
			isegmMerge(line->segms, z) ;
		else
		{
			ISEGM* segm = (ISEGM*)malloc(sizeof(ISEGM)) ;
			if( segm == NULL )
				return 0 ;
			isegmInit2(segm, z, facetId) ;
			segm->next = line->segms ;
			line->segms = segm ;
			(line->n)++ ;
		}
		return 1 ;
	}
	else
		return 0 ;
}

// return 1=in, 0=out
int sline2ChkPt(SLINE2* line, double z, double tol)
{
	ISEGM* segm ;

	for( segm = line->segms ; segm ; segm = segm->next )
	{
		if( segm->z1 > -1. &&
			segm->z1-tol < z &&
			z < segm->z2+tol )
			return 1 ;
	}

	return 0 ;
}
//==============================================================
ZB2* zb2Create(BOX3D* box, 
	           double w,
	           double h,
	           double tol,
			   double ang)
{
	int i, j ;
	double x, y ;
	SLINE2* line = NULL ;
	ZB2* zb = NULL ;

	zb = (ZB2*)malloc(sizeof(ZB2)) ;
	if( zb == NULL )
		return NULL ;
	zb->lines = NULL ;
	zb->pixels = NULL ;
	zb->xmin = box->min[0] ;
	zb->xmax = box->max[0] ;
	zb->ymin = box->min[1] ;
	zb->ymax = box->max[1] ;
	zb->zmin = box->min[2] ;
	zb->zmax = box->max[2] ;
	zb->w = w ; // 间距，x和y方向上的扫描线的间距
	zb->h = h ; // 层厚, z方向上的层的厚度
	zb->tol = tol ;
	zb->ang = ang ;

	zb->facetId = 0 ;
	zb->nx = (int)ceil((zb->xmax-zb->xmin)/w)+1 ;
	zb->ny = (int)ceil((zb->ymax-zb->ymin)/w)+1 ;
	zb->lines = (SLINE2**)malloc(zb->nx*sizeof(SLINE2*)) ; // zb->nx
	if( zb->lines == NULL )
		goto Exit ;
	for( i = 0 ; i < zb->nx ; i++ )
		zb->lines[i] = NULL ;
	for( i = 0 ; i < zb->nx ; i++ )
	{
		x = zb->xmin+w*i ;
		zb->lines[i] = (SLINE2*)malloc(zb->ny*sizeof(SLINE2)) ; // zb->ny
		if( zb->lines[i] == NULL )
			goto Exit ;
		for( j = 0 ; j < zb->ny ; j++ )
		{
			y = zb->ymin+w*j ;
			sline2Init2(&(zb->lines[i][j]), x, y)  ;
		}
	}

	zb->cut = 0 ;
	zb->z = -MAX_DBL ;
	zb->pixels = (uchar*)malloc(zb->nx*zb->ny*sizeof(uchar)) ;

	return zb ;
Exit:
	if( zb )
	{
		if( zb->lines )
		{
			for( i = 0 ; i < zb->nx ; i++ )
			{
				if( zb->lines[i] )
				{
					for( j = 0 ; j < zb->ny ; j++ )
						sline2FreeSegmAll(&(zb->lines[i][j])) ;
					free(zb->lines[i]) ;
				}
			}
			free(zb->lines) ;
		}
		if( zb->pixels )
			free(zb->pixels) ;
		free(zb) ;
	}

	return NULL ;
}

void zb2Free(ZB2* zb)
{
	int i, j ;

	if( zb )
	{
		if( zb->lines )
		{
			for( i = 0 ; i < zb->nx ; i++ )
			{
				if( zb->lines[i] )
				{
					for( j = 0 ; j < zb->ny ; j++ )
						sline2FreeSegmAll(&(zb->lines[i][j])) ;
					free(zb->lines[i]) ;
				}
			}
			free(zb->lines) ;
		}

		if( zb->pixels )
			free(zb->pixels) ;
		free(zb) ;
	}

	return ;
}
//--------------------------------------------------------------
double zb2GetSpacing(ZB2* zb)
{
	return zb->w ;
}

double zb2GetThickness(ZB2* zb)
{
	return zb->h ;
}

int zb2GetNumOfSLineX(ZB2* zb)
{
	return zb->nx ;
}

int zb2GetNumOfSLineY(ZB2* zb)
{
	return zb->ny ;
}

// 取第i行第j列的扫描线
SLINE2* zb2GetSLine(ZB2* zb, int i, int j)
{
	if( i < 0 ||
		i >= zb->nx ||
		j < 0 ||
		j >= zb->ny )
		return NULL ;

	return &(zb->lines[i][j]) ;
}

void zb2ResI(ZB2* zb, int& i)
{
	if( i < 0 )
		i = 0 ;
	else
	if( i > zb->nx-1 )
		i = zb->nx-1 ;

	return ;
}

void zb2ResJ(ZB2* zb, int& j)
{
	if( j < 0 )
		j = 0 ;
	else
	if( j > zb->ny-1 )
		j = zb->ny-1 ;

	return ;
}

// i: i_scanline along x axis
// j: j_scanline along y axis
int zb2Add(ZB2* zb, int i, int j, double z)
{
	SLINE2* line = NULL ;

	line = zb2GetSLine(zb, i, j) ;
	if( line == NULL )
		return 0 ;
	if( sline2Add(line, z, zb->facetId) != 1 )
		return 0 ; // memory error

	return 1 ;
}
//--------------------------------------------------------------
/*	2017/5/28 nt
 *	TRI/scan_line intersection
 *	切割平面平行于YZ平面（垂直于X轴）
 *	|normal[2]|>0
 */
int zb2SliceTriYZ(ZB2* zb, 
	              double xmin, 
	              double xmax, 
	              PNT3D A, 
	              PNT3D B, 
	              PNT3D C,
	              VEC3D normal)
{
	int i, j, n, i1, i2, j1, j2, as[2] ;
	double x, y, z, dx = 0., dy = 0. ;
	PNT3D ps[2] ;

	i1 = (int)ceil((xmin-zb->tol-zb->xmin)/zb->w) ; // (...)/w <= i1
	i2 = (int)floor((xmax+zb->tol-zb->xmin)/zb->w) ; // i2 <= (...)/w
	dx = normal[0]/normal[2] ;
	dy = normal[1]/normal[2] ;
	for( i = i1 ; i <= i2 ; i++ ) // 沿x方向遍历切割平面
	{
		x = zb->xmin+zb->w*i ;
		n = triCut(A, B, C, 0, x, zb->tol, ps, as) ; // 无返回3的情况
		if( n == 1 ||
			n == 2 )
		{
			j1 = (int)ceil((ps[0][1]-zb->tol-zb->ymin)/zb->w) ;
			y = zb->ymin+zb->w*j1 ;
			if( fabs(y-ps[0][1]) < zb->tol ) // 过(x,y,0)扫描线过ps[0]
			{
				if( zb2Add(zb, i, j1, ps[0][2]) == 0 )
					return 0 ;
				j1++ ;
			}
		}
		if( n == 2 )
		{
			j2 = (int)floor((ps[1][1]+zb->tol-zb->ymin)/zb->w) ;
			y = zb->ymin+zb->w*j2 ;
			if( fabs(y-ps[1][1]) < zb->tol ) // 过(x,y,0)扫描线过ps[1]
			{
				if( zb2Add(zb, i, j2, ps[1][2]) == 0 )
					return 0 ;
				j2-- ;
			}

			for( j = j1 ; j <= j2 ; j++ ) // tri->normal[2]!=0.
			{
				y = zb->ymin+zb->w*j ;
				z = A[2]+dx*(A[0]-x)+dy*(A[1]-y) ;
				if( zb2Add(zb, i, j, z) == 0 )
					return 0 ;
			}
		}
	}

	return 1 ;
}

/*	2017/5/28 nt
 *	STLTRI/scan_line intersection
 *	切割平面平行于XZ平面（垂直于Y轴）
 *	|normal[2]|>0
 */
int zb2SliceTriXZ(ZB2* zb, 
	              double ymin, 
	              double ymax, 
	              PNT3D A, 
	              PNT3D B, 
	              PNT3D C,
	              VEC3D normal)
{
	int i, j, n, i1, i2, j1, j2, as[2] ;
	double x, y, z, dx = 0., dy = 0. ;
	PNT3D ps[2] ;

	j1 = (int)ceil((ymin-zb->tol-zb->ymin)/zb->w) ; // (...)/w <= j1
	j2 = (int)floor((ymax+zb->tol-zb->ymin)/zb->w) ; // j2 <= (...)/w
	dx = normal[0]/normal[2] ;
	dy = normal[1]/normal[2] ;
	for( j = j1 ; j <= j2 ; j++ ) // 沿y方向遍历切割平面
	{
		y = zb->ymin+zb->w*j ;
		n = triCut(A, B, C, 1, y, zb->tol, ps, as) ; // 无返回3的情况
		if( n == 1 ||
			n == 2 )
		{
			i1 = (int)ceil((ps[0][0]-zb->tol-zb->xmin)/zb->w) ;
			x = zb->xmin+zb->w*i1 ;
			if( fabs(x-ps[0][0]) < zb->tol ) // 过(x,y,0)扫描线过ps[0]
			{
				if( zb2Add(zb, i1, j, ps[0][2]) == 0 )
					return 0 ;
				i1++ ;
			}
		}
		if( n == 2 )
		{
			i2 = (int)floor((ps[1][0]+zb->tol-zb->xmin)/zb->w) ;
			x = zb->xmin+zb->w*i2 ;
			if( fabs(x-ps[1][0]) < zb->tol ) // 过(x,y,0)扫描线过ps[1]
			{
				if( zb2Add(zb, i2, j, ps[1][2]) == 0 )
					return 0 ;
				i2-- ;
			}

			for( i = i1 ; i <= i2 ; i++ ) // tri->normal[2]!=0.
			{
				x = zb->xmin+zb->w*i ;
				z = A[2]+dx*(A[0]-x)+dy*(A[1]-y) ;
				if( zb2Add(zb, i, j, z) == 0 )
					return 0 ;
			}
		}
	}

	return 1 ;
}

int zb2SliceTri(ZB2* zb, PNT3D A, PNT3D B, PNT3D C)
{
	double xmin, xmax, ymin, ymax, xx, yy ;
	VEC3D normal ;

	mathGetTriNormal(A, B, C, MIN_DBL, normal) ;
	if( fabs(normal[2]) < zb->ang )
		return 1 ;
	xmin = mathMIN2(A[0], B[0], C[0]) ;
	xmax = mathMAX2(A[0], B[0], C[0]) ;
	ymin = mathMIN2(A[1], B[1], C[1]) ;
	ymax = mathMAX2(A[1], B[1], C[1]) ;
	xx = xmax-xmin ;
	yy = ymax-ymin ;
	if( xx < zb->tol ||
		yy < zb->tol )
		return 1 ; // 三角片退化
	if( xx > yy )
		return zb2SliceTriYZ(zb, xmin, xmax, A, B, C, normal) ;
	else
		return zb2SliceTriXZ(zb, ymin, ymax, A, B, C, normal) ;
}

int zb2SliceFacet(ZB2* zb, FACET* facet)
{
	int i ;

	if( zb == NULL ||
		facet == NULL )
		return 0 ;
	(zb->facetId)++ ;

	for( i = 0 ; i < facet->ntria ; i++ )
	{
		zb2SliceTri(zb, 
			        facet->nodes[facet->trias[i][0]].p,
			        facet->nodes[facet->trias[i][1]].p,
			        facet->nodes[facet->trias[i][2]].p) ;
	}

	for( i = 0 ; i < facet->nquad ; i++ )
	{
		zb2SliceTri(zb, 
			        facet->nodes[facet->quads[i][0]].p,
			        facet->nodes[facet->quads[i][1]].p,
			        facet->nodes[facet->quads[i][2]].p) ;
		zb2SliceTri(zb, 
			        facet->nodes[facet->quads[i][2]].p,
			        facet->nodes[facet->quads[i][3]].p,
			        facet->nodes[facet->quads[i][0]].p) ;
	}

	return 1 ;
}

int zb2SliceSS(ZB2* zb, SS* ss)
{
	int i ;
	SUPPORT* support ;
	ROD* rod ;

	if( zb == NULL ||
		ss == NULL )
		return 0 ;

	for( support = ss->supports ; support ; support = support->next )
	{
		for( i = 0 ; i < SUPPORT_MAXNUMFACET ; i++ )
		{
			if( support->facets[i] )
			{
				if( zb2SliceFacet(zb, support->facets[i]) != 1 )
					return 0 ;
			}
		}
	}

	// nt add 2017/6/9
	for( rod = ss->rods ; rod ; rod = rod->next )
	{
		if( rod->facet )
		{
			if( zb2SliceFacet(zb, rod->facet) != 1 )
				return 0 ;
		}
	}

	if( ss->base &&
		ss->base->facet )
	{
		if( zb2SliceFacet(zb, ss->base->facet) != 1 )
			return 0 ;
	}

	return 1 ;
}

int zb2Slice(ZB2* zb, STL* stl)
{
	SS* ss ;

	if( zb == NULL ||
		stl == NULL )
		return 0 ;
	ss = (SS*)(stl->ss) ;	

	return zb2SliceSS(zb, ss) ;
}

// nt add 2021/6/5
int zb2SliceCB(ZB2* zb, CB* cb, PRG* pPrg)
{
	int i, j, k, I, nFacet, nBFacet;
	double dd, center[3] = {0., 0., 0.} ;
	CSEGM* segm = NULL ;
	CELL *cell = NULL;
	CLINE* line = NULL ;
	FACET* facets[5], *Facets[5], *bfacets[5], *bFacets[5];

	if( zb == NULL ||
		cb == NULL )
		return 0 ;

	nFacet = facetsCell(center, cb->w, cb->r1, cb->r2, 6, facets) ;
	nFacet = facetsCell(center, cb->w, cb->r1, cb->r2, 6, Facets) ;

	dd = 1./(cb->nx*cb->ny) ;
	for( i = 0 ; i < cb->nx ; i++ )
	{
		for( j = 0 ; j < cb->ny ; j++ )
		{
			line = cbGetCLine(cb, i, j) ;
			if( line )
			{
				memcpy(center, line->p, sizeof(PNT2D)) ;
				segm = line->segms ;
				while(segm)
				{
					for( k = segm->k1 ; k <= segm->k2 ; k++ )
					{
						center[2] = cb->zmin+cb->w*(k+0.5) ;
						facetsCopyMove(nFacet, facets, center, Facets) ;
						for( I = 0 ; I < nFacet ; I++ )
							zb2SliceFacet(zb, Facets[I]) ;
					}
					segm = segm->next ;
				}

				cell = line->cells;
				while (cell)
				{
					nBFacet = facetsBoundCell(center, cell, cb->r1, cb->r2, 6, bfacets);
					nBFacet = facetsBoundCell(center, cell, cb->r1, cb->r2, 6, bFacets);
					for (I = 0; I < nBFacet; I++)
						zb2SliceFacet(zb, bFacets[I]);
					cell = cell->next;
				}
			}

			if( pPrg )
				pPrg->c += (dd*pPrg->ws[pPrg->i]) ;
		}
	}

	for( I = 0 ; I < nFacet ; I++ )
	{
		facetFree(facets[I]) ;
		facetFree(Facets[I]) ;
	}

	return 1 ;
}
//--------------------------------------------------------------
int zb2Draw(ZB2* zb, void* pVI)
{
	int i, j ;
	PNT3D p, q ;
	SLINE2* line ;
	ISEGM* segm ;

	glBegin(GL_LINES) ;
	glLineWidth(1.f) ;
	glColor3ub(0, 0, 0) ;
	for( i = 0 ; i < zb->nx ; i++ )
	{
		p[0] = zb->xmin+zb->w*i ;
		q[0] = p[0] ;
		for( j = 0 ; j < zb->ny ; j++ )
		{
			p[1] = zb->ymin+zb->w*j ;
			q[1] = p[1] ;
			line = zb2GetSLine(zb, i, j) ;
			for( segm = line->segms ; segm ; segm = segm->next )
			{
				if( segm->z1 > 1-MAX_DBL )
				{
					p[2] = segm->z1 ; // 取z坐标值
					q[2] = segm->z2 ; // 取z坐标值
					glVertex3dv(p) ;
					glVertex3dv(q) ;
				}
			}
		}
	}
	glEnd() ;

	return 1 ;
}

int zb2Draw2(ZB2* zb, void* pVI)
{
	int i, j ;
	PNT3D p ;
	SLINE2* line ;
	ISEGM* segm ;

	glPointSize(1.f) ;
	glColor3ub(0, 0, 0) ;
	glBegin(GL_POINTS) ;
	for( i = 0 ; i < zb->nx ; i++ )
	{
		p[0] = zb->xmin+zb->w*i ;
		for( j = 0 ; j < zb->ny ; j++ )
		{
			p[1] = zb->ymin+zb->w*j ;
			line = zb2GetSLine(zb, i, j) ;
			for( segm = line->segms ; segm ; segm = segm->next )
			{
				if( segm->z1 > 1-MAX_DBL )
				{
					p[2] = segm->z1 ;
					glVertex3dv(p) ;
					p[2] = segm->z2 ;
					glVertex3dv(p) ;
				}
			}
		}
	}
	glEnd() ;

	return 1 ;
}

int zb2DrawZ(ZB2* zb, void* pVI)
{
	int i, j, k ;
	PNT3D p ;
	
	if( zb->cut )
	{
		p[2] = zb->z ;
		glPointSize(2.5f) ;
		//glColor3ub(255, 0, 255) ;
		glBegin(GL_POINTS) ;
		k = 0 ;
		for( j = 0 ; j < zb->ny ; j++ )
		{
			p[1] = zb->ymin+zb->w*j ;
			for( i = 0 ; i < zb->nx ; i++ )
			{
				p[0] = zb->xmin+zb->w*i ;
				if( zb->pixels[k] > 0 )
					glVertex3dv(p) ;
				k++ ;
			}
		}
		glEnd() ;
	}

	return 1 ;
}
//--------------------------------------------------------------
int zb2Cut(ZB2* zb, double z)
{
	int i, j, k ;
	SLINE2* line ;
	
	k = 0 ;
	for( j = 0 ; j < zb->ny ; j++ )
	{
		for( i = 0 ; i < zb->nx ; i++ )
		{
			line = zb2GetSLine(zb, i, j) ;
			if( sline2ChkPt(line, z, zb->tol) == 1 )
				zb->pixels[k] = 255 ;
			else
				zb->pixels[k] = 0 ;
			k++ ;
		}
	}
	zb->cut = 1 ;
	zb->z = z ;

	return 1 ;
}

uchar zb2GetPixel(ZB2* zb, int i, int j)
{
	//return zb->pixels[zb->ny*j+i] ;
	return zb->pixels[zb->nx*j+i] ; // nt add 2021/6/5
}
////////////////////////////////////////////////////////////////