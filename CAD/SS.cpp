#include "StdAfx.h"

////////////////////////////////////////////////////////////////

//==============================================================
/*	2017/3/23 nt
 *  base of support structure
 */
BASE* baseCreate(BOX3D* box, double a, double d)
{
	BASE* base = (BASE*)malloc(sizeof(BASE)) ;
	if( base == NULL )
		return NULL ;
	if( box == NULL )
		mathClnBox(&(base->box)) ;
	else
		base->box = *box ;
	if( a < PI1/180 )
		a = PI1/180 ;
	else
	if( a > PI1/180*85 )
		a = PI1/180*85 ;
	base->a = a ;
	base->d = d ;
	base->box.min[0] -= d ;
	base->box.min[1] -= d ;
	base->box.max[0] += d ;
	base->box.max[1] += d ;
	base->facet = NULL ;

	return base ;
}

// nt add 2017/9/1
BASE* baseCreate2()
{
	BASE* base = (BASE*)malloc(sizeof(BASE)) ;

	return base ;
}

void baseFree(BASE* base)
{
	if( base )
	{
		if( base->facet )
			facetFree(base->facet) ;
		free(base) ;
	}

	return ;
}

// nt add 2017/9/4
BASE* baseCopy(BASE* base)
{
	BASE* copy = baseCreate2() ;
	*copy = *base ;
	if( base->facet )
		copy->facet = facetCopy(base->facet) ;

	return copy ;
}

void fnodeSet(FNODE* node, PNT3D p, VEC3D normal)
{
	memcpy(node->p, p, sizeof(PNT3D)) ;
	memcpy(node->normal, normal, sizeof(PNT3D)) ;
	node->uv[0] = 0. ;
	node->uv[1] = 0. ;
	node->attrib = 0. ;

	return ;
}

/*	2017/3/23 nt
 *  要求base中已经设置好了box,a和d
 */
int baseMake(BASE* base)
{
	int i, j ;
	double w, d, h, offset ;
	PNT3D ps[2][2], qs[2][2] ;
	VEC3D normal ;
	FACET* facet = NULL ;

	w = base->box.max[0]-base->box.min[0] ; // x方向
	d = base->box.max[1]-base->box.min[1] ; // y方向
	h = base->box.max[2]-base->box.min[2] ; // z方向
	memcpy(ps[0][0], base->box.min, sizeof(PNT3D)) ;
	ps[0][0][2] = base->box.max[2] ;
	memcpy(ps[1][1], base->box.max, sizeof(PNT3D)) ;
	memcpy(ps[1][0], ps[0][0], sizeof(PNT3D)) ;
	ps[1][0][0] += w ;
	memcpy(ps[0][1], ps[0][0], sizeof(PNT3D)) ;
	ps[0][1][1] += d ;

	offset = h*tan(base->a) ;
	for( i = 0 ; i < 2 ; i++ )
	{
		for( j = 0 ; j < 2 ; j++ )
		{
			memcpy(qs[i][j], ps[i][j], sizeof(PNT3D)) ;
			qs[i][j][0] += i>0?-offset:offset ;
			qs[i][j][1] += j>0?-offset:offset ;
			qs[i][j][2] = base->box.min[2] ;
		}
	}

	facet = facetCreate(24, 0, 6) ;
	if( facet == NULL )
		return 0 ; // memory error

	// 上底面
	mathInitVec(normal, 'Z') ;
	fnodeSet(&(facet->nodes[0]), ps[0][0], normal) ;
	fnodeSet(&(facet->nodes[1]), ps[1][0], normal) ;
	fnodeSet(&(facet->nodes[2]), ps[1][1], normal) ;
	fnodeSet(&(facet->nodes[3]), ps[0][1], normal) ;
	facet->quads[0][0] = 0 ;
	facet->quads[0][1] = 1 ;
	facet->quads[0][2] = 2 ;
	facet->quads[0][3] = 3 ;

	// 下底面
	mathInitVec2(normal, 'Z', -1) ;
	fnodeSet(&(facet->nodes[4]), qs[0][0], normal) ;
	fnodeSet(&(facet->nodes[5]), qs[0][1], normal) ;
	fnodeSet(&(facet->nodes[6]), qs[1][1], normal) ;
	fnodeSet(&(facet->nodes[7]), qs[1][0], normal) ;
	facet->quads[1][0] = 4 ;
	facet->quads[1][1] = 5 ;
	facet->quads[1][2] = 6 ;
	facet->quads[1][3] = 7 ;
	 
	// 前
	normal[0] = 0. ;
	normal[1] = -cos(base->a) ;
	normal[2] = -sin(base->a) ;
	fnodeSet(&(facet->nodes[8]), qs[0][0], normal) ;
	fnodeSet(&(facet->nodes[9]), qs[1][0], normal) ;
	fnodeSet(&(facet->nodes[10]), ps[1][0], normal) ;
	fnodeSet(&(facet->nodes[11]), ps[0][0], normal) ;
	facet->quads[2][0] = 8 ;
	facet->quads[2][1] = 9 ;
	facet->quads[2][2] = 10 ;
	facet->quads[2][3] = 11 ;

	// 后
	normal[0] = 0. ;
	normal[1] = cos(base->a) ;
	normal[2] = -sin(base->a) ;
	fnodeSet(&(facet->nodes[12]), qs[0][1], normal) ;
	fnodeSet(&(facet->nodes[13]), ps[0][1], normal) ;
	fnodeSet(&(facet->nodes[14]), ps[1][1], normal) ;
	fnodeSet(&(facet->nodes[15]), qs[1][1], normal) ;
	facet->quads[3][0] = 12 ;
	facet->quads[3][1] = 13 ;
	facet->quads[3][2] = 14 ;
	facet->quads[3][3] = 15 ;

	// 左
	normal[0] = -cos(base->a) ;
	normal[1] = 0. ;
	normal[2] = -sin(base->a) ;
	fnodeSet(&(facet->nodes[16]), qs[0][0], normal) ;
	fnodeSet(&(facet->nodes[17]), ps[0][0], normal) ;
	fnodeSet(&(facet->nodes[18]), ps[0][1], normal) ;
	fnodeSet(&(facet->nodes[19]), qs[0][1], normal) ;
	facet->quads[4][0] = 16 ;
	facet->quads[4][1] = 17 ;
	facet->quads[4][2] = 18 ;
	facet->quads[4][3] = 19 ;

	// 右
	normal[0] = cos(base->a) ;
	normal[1] = 0. ;
	normal[2] = -sin(base->a) ;
	fnodeSet(&(facet->nodes[20]), qs[1][0], normal) ;
	fnodeSet(&(facet->nodes[21]), qs[1][1], normal) ;
	fnodeSet(&(facet->nodes[22]), ps[1][1], normal) ;
	fnodeSet(&(facet->nodes[23]), ps[1][0], normal) ;
	facet->quads[5][0] = 20 ;
	facet->quads[5][1] = 21 ;
	facet->quads[5][2] = 22 ;
	facet->quads[5][3] = 23 ;

	base->facet = facet ;

	return 1 ;
}

void baseDraw(BASE* base, void* pVI)
{
	VEC3D v ;

	// nt add 2017/8/24
	viGetViewDir(pVI, v) ;
	if( v[2] > 0. )
		return ;
	// end
	if( base &&
		base->facet )
		facetDraw2(base->facet) ;

	return ;
}

// nt add 2017/8/31
int baseSerialize(BASE* base, CArchive& ar)
{
	if( base == NULL )
		return 0 ;

	if( ar.IsStoring() )
		ar.Write(&base, sizeof(BASE)) ;
	else
		ar.Read(base, sizeof(BASE)) ;

	if( base->facet )
	{
		if( ar.IsStoring() == FALSE )
			base->facet = facetCreate(0, 0, 0) ;
		facetSerialize(base->facet, ar) ;
	}

	return 1 ;
}
//==============================================================
/*	2017/3/23 nt
 *  support
 */
SUPPORT* supportCreate(PNT3D p, 
	                   VEC3D normal, 
					   double h,
	                   double r1, 
	                   double r2,
	                   double r,
	                   double d)
{
	int k ;
	double h1, h2 ;
	PNT3D q ;
	SUPPORT* support ;

	mathEvalLin(p, normal, d, q) ;
	h1 = 0. ;
	h2 = q[2]-h ;
	support = (SUPPORT*)malloc(sizeof(SUPPORT)) ;
	if( support == NULL )
		return NULL ;
	memcpy(support->p, p, sizeof(PNT3D)) ;
	memcpy(support->normal, normal, sizeof(PNT3D)) ;
	support->h = h ;
	support->r1 = r1 ;
	support->h1 = h1 ;
	support->r2 = r2 ;
	support->h2 = h2 ;
	support->r = r ;
	support->d = d ;
	for( k = 0 ; k < SUPPORT_MAXNUMFACET ; k++ )
		support->facets[k] = NULL ;
	support->id = -1 ;
	support->del = 0 ;
	support->flag = 0 ; // nt add 2017/8/22
	support->z = h ; // nt add 2017/8/22
	support->no = 0 ;

	return support ;
}

// nt add 2017/6/4
SUPPORT* supportCreate2()
{
	SUPPORT* support ;
	support = (SUPPORT*)malloc(sizeof(SUPPORT)) ;
	if( support == NULL )
		return NULL ;
	support->h = 0. ;
	support->r1 = 0. ;
	support->h1 = 0. ;
	support->r2 = 0. ;
	support->h2 = 0. ;
	support->r = 0. ;
	support->d = 0. ;
	for( int k = 0 ; k < SUPPORT_MAXNUMFACET ; k++ )
		support->facets[k] = NULL ;
	support->id = -1 ;
	support->del = 0 ;
	support->flag = 0 ; // nt add 2017/8/22
	support->z = 0. ; // nt add 2017/8/22
	support->no = 0 ;

	return support ;
}

void supportFree(SUPPORT* support)
{
	int k ;

	if( support )
	{
		for( k = 0 ; k < SUPPORT_MAXNUMFACET ; k++ )
		{
			if( support->facets[k] )
				facetFree(support->facets[k]) ;
		}
		free(support) ;
	}

	return ;
}

// nt add 2017/9/4
SUPPORT* supportCopy(SUPPORT* support)
{
	SUPPORT* copy = supportCreate2() ;
	*copy = *support ;
	copy->next = NULL ;
	for(int i = 0 ; i < SUPPORT_MAXNUMFACET ; i++)
	{
		if( support->facets[i] )
			copy->facets[i] = facetCopy(support->facets[i]) ;
	}

	return copy ;
}

// nt add 2017/3/28
int supportGetId(SUPPORT* support)
{
	if( support )
		return support->id ;
	else
		return 0 ;
}

// nt add 2017/3/28
void supportSetId(SUPPORT* support, int id)
{
	if( support )
		support->id = id ;

	return ;
}

// nt add 2017/3/28
void supportMarkDel(SUPPORT* support)
{
	if( support )
		support->del = 1 ;

	return ;
}

// nt add 2017/3/28
int supportIsDel(SUPPORT* support)
{
	if( support &&
		support->del == 1 )
		return 1 ;
	else
		return 0 ;
}

// nt add 2017/3/28
int supportIsRaw(SUPPORT* support)
{
	if( support &&
		support->facets[0] == NULL &&
		support->facets[1] == NULL &&
		support->facets[2] == NULL &&
		support->facets[3] == NULL )
		return 1 ;
	else
		return 0 ;
}

/*	2017/3/24 nt
 *  要求support中已经设置好了基本参数
 *	nt modify 2017/5/30
 */
int supportMake(SUPPORT* support, int nu, int nv)
{
	PNT3D q, q1, q2 ;

	mathEvalLin(support->p, support->normal, support->d, q) ;
	q1[0] = q[0] ;
	q1[1] = q[1] ;
	q1[2] = q[2]-support->h1-support->h2 ;
	q2[0] = q[0] ;
	q2[1] = q[1] ;
	q2[2] = q[2]-support->h2 ;

	if( support->d > MIN_LEN )
	{
		support->facets[0] = facetCone3(q, support->p, support->r2, support->r, nu) ;
		support->facets[1] = facetSphere(q, support->r2, nu, nv) ;
		support->z = q[2] ; // nt add 2017/8/30
	}
	if( support->h2 > MIN_LEN )
	{
		support->facets[2] = facetCylinder3(q2, q, support->r2, nu) ;
		support->z = q2[2] ; // nt add 2017/8/30
	}
	if( support->h1 > MIN_LEN )
	{
		support->facets[3] = facetCone3(q1, q2, support->r1, support->r2, nu) ;
		support->z = q1[2] ; // nt add 2017/8/30
	}

	return 1 ;
}

/*	2017/3/28 nt
 *  要求support中已经设置好了基本参数
 *	from p[2] to z, p[2]>z
 *  nt rewrite 2017/6/4
 *  nt modify 2017/7/4
 */
int supportMake2(SUPPORT* support, double z, VEC3D normal, int nu, int nv)
{
	double d, h = 0. ;
	PNT3D q, q1, p1 ;
	VEC3D Z = {0., 0., 1.} ;

	// nt add 2017/6/9
	if( support->normal[2] > 0. )
		return 0 ;

	mathEvalLin(support->p, support->normal, support->d, q) ;
	memcpy(p1, support->p, sizeof(PNT2D)) ;
	p1[2] = z ;
	if( fabs(normal[2]) < MIN_ANG )
		goto ReCal ;
	memcpy(q1, q, sizeof(PNT2D)) ;
	q1[2] = p1[2]+((p1[0]-q1[0])*normal[0]+(p1[1]-q1[1])*normal[1])/normal[2] ;
	if( q1[2] < z ) // nt add 2017/7/14
		q1[2] = z ;
	h = mathDist2D(p1, q1) ;
	q1[2] += h ;
	if( q1[2]+2*support->r2 > q[2] )
		goto ReCal ;
	
	if( support->d > MIN_LEN )
	{
		support->facets[0] = facetCone3(q, support->p, support->r2, support->r, nu) ;
		support->facets[1] = facetSphere(q, support->r2, nu, nv) ;
		support->z = q[2] ; // nt add 2017/8/30
	}
	support->h2 = q[2]-q1[2] ;
	if( support->h2 > MIN_LEN )
	{
		support->facets[2] = facetCylinder3(q1, q, support->r2, nu) ;
		support->facets[3] = facetSphere(q1, support->r2, nu, nv) ;
		support->z = q1[2] ; // nt add 2017/8/30
	}
	support->facets[4] = facetCone3(p1, q1, support->r, support->r2, nu) ;

	return 1 ;
ReCal:
	d = mathDist(support->p, p1) ;
	if( d > 2.5*support->d )
	{
		mathEvalLin(support->p, Z, -support->d, q) ;
		mathEvalLin(p1, Z, support->d, q1) ;
		if( q[2] < 0. || // nt add 2017/7/14
			q1[2] < 0. )
			return 0 ;
		support->facets[0] = facetCone3(q, support->p, support->r2, support->r, nu) ;
		support->facets[1] = facetCylinder3(q1, q, support->r2, nu) ;
		support->facets[2] = facetCone3(p1, q1, support->r, support->r2, nu) ;
		support->z = q1[2] ; // nt add 2017/8/30
	}
	else
	{
		mathMidPnt(support->p, p1, q) ;
		//support->facets[0] = facetCone2(q, support->p, support->r2, support->r, nu) ;
		//support->facets[1] = facetCone2(q, p1, support->r2, support->r, nu) ;
		support->facets[0] = facetCone3(q, support->p, support->r2, support->r, nu) ;
		support->facets[1] = facetCone3(q, p1, support->r2, support->r, nu) ;
		support->z = q[2] ; // nt add 2017/8/30
	}

	return 1 ;
}

// nt add 2017/6/9 就是supportMake3()的z=0.的情况
int supportMake3(SUPPORT* support, double R, double H, double A, int nu, int nv)
{
	double R1 = R ;
	PNT3D q, q1, q2 ;

	// nt add 2017/6/9
	if( support->normal[2] > -MIN_ANG )
		return 0 ;

	mathEvalLin(support->p, support->normal, support->d, q) ;
	memcpy(q1, q, sizeof(PNT3D)) ;
	memcpy(q2, q, sizeof(PNT3D)) ;
	q1[2] = H ;
	q2[2] = 0. ;
	//if( q[2]-H < support->r2 )
	if( q[2] < support->r2 ) // nt modify 2017/7/11
		return 0 ;

	support->facets[0] = facetCone3(q, support->p, support->r2, support->r, nu) ;
	support->facets[1] = facetSphere(q, support->r2, nu, nv) ;
	support->facets[2] = facetCylinder3(q1, q, support->r2, nu) ;
	support->z = q1[2] ; // nt add 2017/8/30
	if( A < 89.999999 )
		R1 += H/tan(A*PI1/180) ;
	support->facets[3] = facetCone3(q1, q2, R1, R, nu) ;

	return 1 ;
}

void supportDraw(SUPPORT* support, void* pVI)
{
	int k ;

	if( support )
	{
		for( k = 0 ; k < SUPPORT_MAXNUMFACET ; k++ )
		{
			if( k > 2 ) // nt add 2017/9/30
			{
				VEC3D v ;
				viGetViewDir(pVI, v) ;
				if( v[2] > 0. )
					return ;
			}
			if( support->facets[k] )
				facetDraw2(support->facets[k]) ;
		}
	}
	
	return ;
}

// nt add 2017/6/4
void supportDraw2(SUPPORT* support, void* pVI)
{
	if( support )
	{
		PNT3D q ;
		mathEvalLin(support->p, support->normal, support->d, q) ;
		//glColor3ub(0, 0, 0) ;
		//glLineWidth(1.f) ;
		glBegin(GL_LINE_STRIP) ;
			glVertex3dv(support->p) ;
			glVertex3dv(q) ;
			// nt modify 2017/8/30, 适应有内部支撑的情况
			//q[2] = support->h ; // 0. ;
			//q[2] = facetGetMinZ(support->facets[2]) ;
			q[2] = support->z ;
			// end
			glVertex3dv(q) ;
		glEnd() ;
	}
	
	return ;
}

// nt add 2017/8/31
int supportSerialize(SUPPORT* support, CArchive& ar)
{
	int i ;

	if( support == NULL )
		return 0 ;

	if( ar.IsStoring() )
		ar.Write(support, sizeof(SUPPORT)) ;
	else
	{
		ar.Read(support, sizeof(SUPPORT)) ;
		support->next = NULL ;
	}

	for( i = 0 ; i < SUPPORT_MAXNUMFACET ; i++ )
	{
		if( support->facets[i] )
		{
			if( ar.IsStoring() == FALSE )
				support->facets[i] = facetCreate(0, 0, 0) ;
			facetSerialize(support->facets[i], ar) ;
		}
	}

	return 1 ;
}
//==============================================================
// nt add 2017/6/4
ROD* rodCreate(SUPPORT* support1, SUPPORT* support2, PNT3D p1, PNT3D p2, double r)
{
	ROD* rod = (ROD*)malloc(sizeof(ROD)) ;
	if( rod == NULL )
		return NULL ;
	rod->next = NULL ;
	rod->del = 0 ;
	rod->support1 = support1 ;
	rod->support2 = support2 ;
	memcpy(rod->p1, p1, sizeof(PNT3D)) ;
	memcpy(rod->p2, p2, sizeof(PNT3D)) ;
	rod->r = r ;
	//rod->facet = facetCylinder2(p1, p2, r, 8) ;
	rod->facet = facetCylinder3(p1, p2, r, 8) ;

	return rod ;
}

ROD* rodCreate2()
{
	ROD* rod = (ROD*)malloc(sizeof(ROD)) ;
	if( rod == NULL )
		return NULL ;
	rod->next = NULL ;
	rod->del = 0 ;
	rod->support1 = NULL ;
	rod->support2 = NULL ;
	rod->p1[0] = 0. ;
	rod->p1[1] = 0. ;
	rod->p1[2] = 0. ;
	rod->p2[0] = 0. ;
	rod->p2[1] = 0. ;
	rod->p2[2] = 0. ;
	rod->r = 0. ;
	rod->facet = NULL ;

	return rod ;
}

void rodFree(ROD* rod)
{
	if( rod )
	{
		if( rod->facet )
			facetFree(rod->facet) ;
		free(rod) ;
	}

	return ;
}

// nt add 2017/9/4
ROD* rodCopy(ROD* rod)
{
	ROD* copy = rodCreate2() ;
	*copy = *rod ;
	copy->next = NULL ;
	copy->support1 = NULL ;
	copy->support2 = NULL ;
	if( rod->facet )
		copy->facet = facetCopy(rod->facet) ;

	return copy ;
}

int rodIsDel(ROD* rod)
{
	if( rod &&
		rod->del == 1 )
		return 1 ;
	else
		return 0 ;
}

void rodDraw(ROD* rod, void* pVI)
{
	if( rod &&
		rod->facet )
		facetDraw2(rod->facet) ;
}

void rodDraw2(ROD* rod, void* pVI)
{
	if( rod )
	{
		glBegin(GL_LINES) ;
			glVertex3dv(rod->p1) ;
			glVertex3dv(rod->p2) ;
		glEnd() ;
	}

	return ;
}

// nt add 2017/8/31
int rodSerialize(ROD* rod, CArchive& ar, SUPPORT** array)
{
	int no ;

	if( rod == NULL )
		return 0 ;

	if( ar.IsStoring() )
	{
		ar.Write(rod, sizeof(ROD)) ;
		ar << rod->support1->no
		   << rod->support2->no ;
	}
	else
	{
		ar.Read(rod, sizeof(ROD)) ;
		rod->next = NULL ;
		if( rod->support1 == NULL ||
			rod->support2 == NULL ||
			array == NULL )
			return 0 ;
		ar >> no ;
		rod->support1 = array[no] ;
		ar >> no ;
		rod->support2 = array[no] ;
	}

	if( rod->facet )
	{
		if( ar.IsStoring() == FALSE )
			rod->facet = facetCreate(0, 0, 0) ;
		facetSerialize(rod->facet, ar) ;
	}

	return 1 ;
}
//==============================================================
ZP* zpCreate(double z, STLTRI* tri)
{
	ZP* zp = (ZP*)malloc(sizeof(ZP)) ;
	zp->next = NULL ;
	zp->z = z ;
	zp->tri = tri ;

	return zp ;
}

void zpFree(ZP* zp)
{
	if( zp )
		free(zp) ;

	return ;
}

ZM* zmCreate(BOX3D* box, double w)
{
	int i, n ;
	double d ;
	ZM* zm = NULL ;

	zm = (ZM*)malloc(sizeof(ZM)) ;
	if( zm == NULL )
		return NULL ;
	d = fabs(box->max[0]-box->min[0]) ; // x方向的长度范围
	zm->nx = (int)round(d/w) ;
	if( zm->nx < 1 )
		zm->nx = 1 ;
	zm->wx = d/zm->nx ;
	zm->xmin = box->min[0]+0.5*zm->wx ; // ???????????????
	d = fabs(box->max[1]-box->min[1]) ; // y方向的长度范围
	zm->ny = (int)round(d/w) ;
	if( zm->ny < 1 )
		zm->ny = 1 ;
	zm->wy = d/zm->ny ;
	zm->ymin = box->min[1]+0.5*zm->wy ; // ???????????????

	n = zm->nx*zm->ny ;
	zm->zs = (ZP**)malloc(n*sizeof(ZP*)) ;
	if( zm->zs == NULL )
	{
		free(zm) ;
		return 0 ; // error
	}
	for( i = 0 ; i < n ; i++ )
		zm->zs[i] = NULL ;

	return zm ;
}

void zmFree(ZM* zm)
{
	if( zm )
	{
		if( zm->zs )
		{
			int i, n = zm->nx*zm->ny ;
			ZP* zp, *temp ;
			for( i = 0 ; i < n ; i++ )
			{
				zp = zm->zs[i] ;
				while(zp)
				{
					temp = zp->next ;
					zpFree(zp) ;
					zp = temp ;
				}
			}
			free(zm->zs) ;
		}
		free(zm) ;
	}

	return ;
}

int zmSort(ZM* zm, double tol)
{
	int i, j, k, n, m, max = 0, I, J, M ;
	double d ;
	ZP* zp, **array = NULL ;

	if( zm == NULL )
		return 0 ;

	n = zm->nx*zm->ny ;
	for( i = 0 ; i < n ; i++ )
	{
		m = 0 ;
		for( zp = zm->zs[i] ; zp ; zp = zp->next )
			m++ ;
		if( max < m )
			max = m ;
	}
	if( max < 1 )
		return 1 ; // success
	array = (ZP**)malloc(max*sizeof(ZP*)) ;
	if( array == NULL )
		return 0 ; // error
	for( i = 0 ; i < n ; i++ )
	{
		m = 0 ;
		for( zp = zm->zs[i] ; zp ; zp = zp->next )
		{
			array[m] = zp ;
			m++ ;
		}
		if( m > 1 )
		{
			for( j = 0 ; j < m-1 ; j++ ) // 按z从小到大
			{
				for( k = j+1 ; k < m ; k++ )
				{
					if( array[j]->z > array[k]->z )
					{
						zp = array[j] ;
						array[j] = array[k] ;
						array[k] = zp ;
					}
				}
			}

			for( I = 0 ; I < m ; I += M )
			{
				M = 1 ; // 与array[I]重合的交点的总数值
				for( J = I+1 ; J < m ; J++ )
				{
					d = fabs(array[J]->z-array[I]->z) ;
					if( d < tol )
						M++ ;
					else
						break ;
				}

				// 对array[I]...array[I+M-1]共M个做排序
				for( j = I ; j < I+M-1 ; j++ )
				{
					for( k = j+1 ; k < I+M ; k++ )
					{
						if( array[j]->tri->normal[2] > 
							array[k]->tri->normal[2] )
						{
							zp = array[j] ;
							array[j] = array[k] ;
							array[k] = zp ;
						}
					}
				}
			}

			zm->zs[i] = array[0] ;
			for( j = 1 ; j < m ; j++ )
				array[j-1]->next = array[j] ;
			array[m-1]->next = NULL ;
		}
	}
	if( array )
		free(array) ;

	return 1 ;
}

// nt modify 2017/7/12 use ZP
int zmIntSTL(ZM* zm, STL* stl, double tol)
{
	int i, j, k, I, J, i1, i2, j1, j2 ;
	double zMinMax[2] ;
	PNT3D p ;
	BOX2D box ;
	STLBLK* blk = NULL ;
	STLTRI* tri = NULL ;
	ZP* zp = NULL ;

	if( zm == NULL ||
		stl == NULL )
		return 0 ; // error

	p[2] = 0. ;
	for( I = 0 ; I < stl->nBlk ; I++ )
	{
		blk = &(stl->blks[I]) ;
		for( J = 0 ; J < blk->nTri ; J++ )
		{
			tri = &(blk->tris[J]) ;
			if( tri )
			{
				stltriGetBox2D(tri, &box) ;
				i1 = (int)ceil((box.min[0]-tol-zm->xmin)/zm->wx) ;
				i2 = (int)floor((box.max[0]+tol-zm->xmin)/zm->wx) ;
				j1 = (int)ceil((box.min[1]-tol-zm->ymin)/zm->wy) ;
				j2 = (int)floor((box.max[1]+tol-zm->ymin)/zm->wy) ;
				for( i = i1 ; i <= i2 ; i++ )
				{
					p[0] = zm->xmin+i*zm->wx ;
					for( j = j1 ; j <= j2 ; j++ )
					{
						p[1] = zm->ymin+j*zm->wy ;
						k = j*zm->nx+i ;
						zMinMax[0] = MAX_DBL ; // nt modify 2017/7/12
						zMinMax[1] = MAX_DBL ;
						if( triIntZ(tri->vs, tri->normal, p, tol, zMinMax) == 1 )
						{
							zp = zpCreate(zMinMax[0], tri) ; // nt add 2017/7/12
							zp->next = zm->zs[k] ; // nt add 2017/7/12
							zm->zs[k] = zp ; // nt add 2017/7/12
						}
					}
				}
			}
		}
	}

	// nt add 2017/7/12
	zmSort(zm, tol) ;

	return 1 ;
}
//==============================================================
void sspInit(SSP* ssp, 
	         double a, 
	         double h, 
	         double w, 
	         double r, 
	         double d, 
	         double r1, 
	         double r2, 
	         double b, 
	         double tol)
{
	ssp->a = a ;
	ssp->h = h ;
	ssp->w = w ;
	ssp->r = r ;
	ssp->d = d ;
	ssp->r1 = r1 ;
	ssp->r2 = r2 ;
	ssp->b = b ;
	ssp->tol = tol ;
	// nt add 2017/5/11
	ssp->hPart = 5. ; // @总体底托底面到零件底面高度（距离，单位：mm）
	ssp->sDensity = 100 ; // @支撑密度:50-150, 50对应2*w，150对应0.5*w
	ssp->R = 2.5 ; // @支撑底托半径（单位：mm）（若支撑没有落到底托上）
	ssp->H = 2.5 ; // @支撑底托高度（单位：mm）（若支撑没有落到底托上）
	ssp->A = 45. ; // @支撑底托倾斜角（单位：deg，45°~90°）（若支撑没有落到底托上）
	ssp->coneA = 22.5 ; // 支撑顶端锥角度（单位：deg）
	ssp->minH = 25. ; // 支撑无横向连接最小高度（单位：mm）
	ssp->ts[0] = 0. ; // 预留参数
	ssp->ts[1] = 0. ; // 预留参数
	ssp->ts[2] = 0. ; // 预留参数
	ssp->ts[3] = 0. ; // 预留参数
	ssp->ts[4] = 0. ; // 预留参数

	return ;
}
//==============================================================
/*	2017/3/23 nt
 *  support structure
 */
SS* ssCreate()
{
	SS* ss = (SS*)malloc(sizeof(SS)) ;
	if( ss == NULL )
		return NULL ;
	ss->base = NULL ;
	ss->supports = NULL ;
	ss->rods = NULL ; // nt add 2017/6/4
	mathClnBox(&(ss->box)) ;
	ss->editing = 0 ; // nt add 2017/3/29

	return ss ;
}

// nt modify 2017/6/4
void ssFree(SS* ss)
{
	SUPPORT* support ;
	ROD* rod ; // nt add 2017/6/4

	if( ss )
	{
		if( ss->base )
			baseFree(ss->base) ;
		while(ss->supports)
		{
			support = ss->supports ;
			ss->supports = support->next ;
			supportFree(support) ;
		}
		// nt add 2017/6/4
		while(ss->rods)
		{
			rod = ss->rods ;
			ss->rods = rod->next ;
			rodFree(rod) ;
		}
		free(ss) ;
	}

	return ;
}

// nt add 2017/9/4
SS* ssCopy(SS* ss)
{
	SS* copy = ssCreate() ;
	*copy = *ss ;
	if( ss->base )
		copy->base = baseCopy(ss->base) ;
	int i, n = ssGetNumOfSupport(ss) ;
	SUPPORT** array = NULL, *support ;
	copy->supports = NULL ;
	if( n > 0 )
	{
		array = new SUPPORT*[n] ;
		support = ss->supports ;
		for( i = 0 ; i < n ; i++ )
		{
			support->no = i ;
			array[i] = supportCopy(support) ;
			if( i > 0 )
				array[i-1]->next = array[i] ;
			support = support->next ;
		}
		copy->supports = array[0] ;
	}
	ROD* rod, *temp ;
	copy->rods = NULL ;
	for( rod = ss->rods ; rod ; rod = rod->next )
	{
		temp = rodCopy(rod) ;
		temp->support1 = array[rod->support1->no] ;
		temp->support2 = array[rod->support2->no] ;
		temp->next = copy->rods ;
		copy->rods = temp ;
	}
	if( array )
		delete []array ;

	return copy ;
}

// nt add 2017/8/22 仅释放自动生成的支撑
void ssReleaseAuto(SS* ss, int base)
{
	SUPPORT* support, *head = NULL ;
	ROD* rod ;

	if( ss )
	{
		if( base == 0 &&
			ss->base )
		{
			baseFree(ss->base) ;
			ss->base = NULL ;
		}

		while(ss->supports)
		{
			support = ss->supports ;
			ss->supports = support->next ;
			if( support->flag == 1 )
				supportFree(support) ;
			else
			{
				support->next = head ;
				head = support ;
			}
		}
		ss->supports = head ;
		while(ss->rods)
		{
			rod = ss->rods ;
			ss->rods = rod->next ;
			rodFree(rod) ;
		}
		ss->rods = NULL ;
	}

	return ;
}

// nt add 2017/9/4 仅释放自动生成的支撑
BASE* ssReleaseAuto2(SS* ss, int base, CSet& delSupports, CSet& delRods)
{
	BASE* delBase = NULL ;
	SUPPORT* support, *head = NULL ;
	ROD* rod ;

	if( ss )
	{
		if( base == 0 &&
			ss->base )
		{
			//baseFree(ss->base) ;
			delBase = ss->base ;
			ss->base = NULL ;
		}

		while(ss->supports)
		{
			support = ss->supports ;
			ss->supports = support->next ;
			if( support->flag == 1 )
			{
				//supportFree(support) ;
				delSupports.AddObj(support) ;
			}
			else
			{
				support->next = head ;
				head = support ;
			}
		}
		ss->supports = head ;
		while(ss->rods)
		{
			rod = ss->rods ;
			ss->rods = rod->next ;
			//rodFree(rod) ;
			delRods.AddObj(rod) ;
		}
		ss->rods = NULL ;
	}

	return delBase ;
}

void ssBeginEdit(SS* ss)
{
	SUPPORT* support ;

	if( ss )
	{
		for( support = ss->supports ; support ; support = support->next )
			support->id = -1 ;
		ss->editing = 1 ;
	}

	return ;
}

// nt add 2017/6/7
void ssBeginEditRod(SS* ss)
{
	if( ss )
		ss->editing = 2 ;

	return ;
}

void ssEndEdit(SS* ss)
{
	if( ss )
		ss->editing = 0 ;

	return ;
}

int ssIsEditing(SS* ss)
{
	return ss->editing ;
}

int ssGetNumOfSupport(SS* ss)
{
	int n = 0 ;
	SUPPORT* support ;

	if( ss == NULL )
		return -1 ;
	
	for( support = ss->supports ; support ; support = support->next )
		n++ ;

	return n ;
}

SUPPORT* ssGetSupport(SS* ss, int i)
{
	int n = 0 ;
	SUPPORT* support ;

	if( ss == NULL )
		return NULL ;
	
	for( support = ss->supports ; support ; support = support->next )
	{
		if( i == n )
			return support ;
		n++ ;
	}

	return NULL ;
}

SUPPORT* ssGetSupportById(SS* ss, int id)
{
	SUPPORT* support ;

	if( ss == NULL )
		return NULL ;
	
	for( support = ss->supports ; support ; support = support->next )
	{
		if( support->id == id )
			return support ;
	}

	return NULL ;
}

// nt add 2017/10/1, 1=has valid support, 0=no
int ssHasValidSupport(SS* ss)
{
	SUPPORT* support ;

	if( ss == NULL )
		return 0 ;

	for( support = ss->supports ; support ; support = support->next )
	{
		if( supportIsDel(support) == 1 )
			continue ;
		return 1 ;
	}

	return 0 ;
}

// nt add 2017/7/11
// 注意：要求stl->vs已经计算出来
int ssMakeAtSharpPts(SS* ss, STL* stl, SSP* ssp)
{
	int i, j ;
	double z, a, b ;
	PNT3D p ;
	VEC3D normal ;
	VER* v ;
	STLBLK* blk = NULL ;
	STLTRI* tri = NULL ;
	SUPPORT* support ;

	if( ss == NULL ||
		stl == NULL ||
		ssp == NULL )
		return 0 ; // error
	b = ssp->b*PI1/180 ;

	// 生成支撑
	for( v = stl->vs ; v ; v = v->next )
	{
		if( v->flags == 0 && v->normal[2] < 0. )
		{
			a = ACOS(-v->normal[2]) ;
			if( a < b+MIN_ANG )
			{
				z = 0. ;
				mathVec(normal, 0., 0., 1.) ;
				memcpy(p, v->p, sizeof(PNT3D)) ;
				for( i = 0 ; i < stl->nBlk ; i++ )
				{
					blk = &(stl->blks[i]) ;
					for( j = 0 ; j < blk->nTri ; j++ )
					{
						tri = &(blk->tris[j]) ;
						if( tri->normal[2] > 0. && // ???????????
							triIntZ2(tri->vs, tri->normal, p, ssp->tol, &z) == 1 )
							memcpy(normal, tri->normal, sizeof(VEC3D)) ;
					}
				}
				if( normal[2] > MIN_DBL )
				{
					support = supportCreate(v->p, 
											v->normal, 
											ssp->h, 
											ssp->r1,
											ssp->r2, 
											ssp->r,
											ssp->d) ;
					if( z > ssp->h )
						supportMake2(support, z, normal, 8, 8) ;
					else
						//supportMake3(support, ssp->R, ssp->H, ssp->A, 8, 8) ;
						supportMake3(support, ssp->R, ssp->h, ssp->A, 8, 8) ;
					ssAddSupport(ss, support) ;
					support->flag = 1 ; // nt add 2017/8/22
				}
			}
		}
	}

	return 1 ;
}

/*	2017/3/26 nt
 *  生成STL模型的支撑结构
 *  输入参数：
 *		stl:STL模型
 *		ssp->h:底座厚度
 *		ssp->w:支撑间距
 *		ssp->r:接触点（球）半径
 *		ssp->d:
 *		ssp->r1:支撑圆柱半径
 *		ssp->r2:支撑圆台半径
 *		ssp->tol:
 *	输出参数:
 *		ss:支撑结构
 *	返回0=error,1=success
 *	nt modify 2017/7/11 handle sharp pts
 *  nt rewrite 2017/7/13
 *	nt modify 2017/8/22
 *  nt modify 2017/9/10 add PRG
 */
int ssMake(SS* ss, STL* stl, SSP* ssp, PRG* pPrg)
{
	int i, j, k ;
	double a, b, d, dd ;
	PNT3D p ;
	BOX3D box ;
	ZM* zm = NULL ;
	SUPPORT* support = NULL ;
	ZP* z1, *z2 ;

	if( ss == NULL ||
		stl == NULL ||
		ssp == NULL ||
		pPrg == NULL )
		return 0 ; // error
	stlGetBox(stl, &box) ;
	box.min[2] = 0. ;
	box.max[2] = ssp->h ;
	b = ssp->b*PI1/180 ; // nt add 2017/5/11

	// 创建底座
	//ss->base = baseCreate(&box, ssp->a*PI1/180, ssp->d+ssp->r1) ; // 底座
	ss->base = baseCreate(&box, ssp->a*PI1/180, ssp->d) ; // nt modify 2017/9/8
	if( ss->base == NULL )
		return 0 ; // error
	baseMake(ss->base) ;

	// nt add 2017/7/11
	stlCalDownwardSharpPts(stl, 0.001, ssp->w) ;
	ssMakeAtSharpPts(ss, stl, ssp) ;

	// 创建z矩阵并与STL模型求交
	zm = zmCreate(&box, ssp->w) ;
	if( zm == NULL )
		return 0 ; // error
	zmIntSTL(zm, stl, ssp->tol) ;

	// 根据z矩阵生成支撑
	dd = 1./zm->nx*zm->ny ; // nt add 2017/9/10
	for( i = 0 ; i < zm->nx ; i++ )
	{
		p[0] = zm->xmin+i*zm->wx ;
		for( j = 0 ; j < zm->ny ; j++ )
		{
			p[1] = zm->ymin+j*zm->wy ;
			k = j*zm->nx+i ;

			// nt add 2017/7/14 过滤掉距离尖点太近的点
			d = stlCalDistFromSharpPts(stl, p) ;
			if( d < 0.5*ssp->w )
				continue ;

			z1 = zm->zs[k] ;
			if( z1 &&
				z1->tri &&
				z1->tri->normal[2] < 0. )
			{
				a = ACOS(-z1->tri->normal[2]) ;
				if( a < b+MIN_ANG ) // nt modify 2017/5/11 old ssp->b
				{
					p[2] = z1->z ;
					support = supportCreate(p, 
											z1->tri->normal, 
											ssp->h, 
											ssp->r1,
											ssp->r2, 
											ssp->r,
											ssp->d) ;
					if( support == NULL )
					{
						zmFree(zm) ;
						return 0 ; // error
					}
					supportMake(support, 8, 8) ;
					ssAddSupport(ss, support) ;
					support->flag = 1 ; // nt add 2017/8/22
				}

				z2 = z1->next ;
				while(z2)
				{
					if( z1->tri->normal[2] > 0. && // 找到一个内部空腔
						z2->tri->normal[2] < 0. &&
						z2->z-z1->z > 2*ssp->d ) // ???? nt modify 2017/7/14
					{
						p[2] = z2->z ;
						support = supportCreate(p, 
												z2->tri->normal, 
												ssp->h, 
												ssp->r1,
												ssp->r2, 
												ssp->r,
												ssp->d) ;
						if( support == NULL )
						{
							zmFree(zm) ;
							return 0 ; // error
						}
						supportMake2(support, z1->z, z1->tri->normal, 8, 8) ;
						ssAddSupport(ss, support) ;
						support->flag = 1 ; // nt add 2017/8/22
					}
					z1 = z1->next ;
					z2 = z2->next ;
				}
			}

			// nt add 2017/9/10
			if( pPrg )
				pPrg->c += (dd*pPrg->ws[pPrg->i]) ;
		}
	}

	// 释放z矩阵
	zmFree(zm) ;

	// nt add 2017/7/14
	VER* u, *v = stl->vs ;
	while(v)
	{
		u = v->next ;
		verFree(v) ;
		v = u ;
	}
	stl->vs = NULL ;

	return 1 ;
}

void ssAddSupport(SS* ss, SUPPORT* support)
{
	if( ss &&
		support )
	{
		support->next = ss->supports ;
		ss->supports = support ;
	}

	return ;
}

int ssDelSupport(SS* ss, SUPPORT* support)
{
	SUPPORT* temp, *prev ;

	if( ss &&
		support )
	{
		prev = NULL ;
		for( temp = ss->supports ; temp ; temp = temp->next )
		{
			if( temp == support )
			{
				if( prev == NULL )
					ss->supports = temp->next ;
				else
					prev->next = temp->next ;

				ssDelRod2(ss, support) ; // nt add 2017/9/3
				supportFree(support) ;
				return 1 ;
			}
			prev = temp ;
		}
	}

	return 0 ;
}

// nt add 2017/9/4
int ssRemoveSupport(SS* ss, SUPPORT* support)
{
	SUPPORT* temp, *prev ;

	if( ss &&
		support )
	{
		prev = NULL ;
		for( temp = ss->supports ; temp ; temp = temp->next )
		{
			if( temp == support )
			{
				if( prev == NULL )
					ss->supports = temp->next ;
				else
					prev->next = temp->next ;

				return 1 ;
			}
			prev = temp ;
		}
	}

	return 0 ;
}

// nt add 2017/6/4
void ssAddRod(SS* ss, ROD* rod)
{
	if( ss &&
		rod )
	{
		rod->next = ss->rods ;
		ss->rods = rod ;
	}

	return ;
}

// nt add 2017/6/4
int ssDelRod(SS* ss, ROD* rod)
{
	ROD* temp, *prev ;

	if( ss &&
		rod )
	{
		prev = NULL ;
		for( temp = ss->rods ; temp ; temp = temp->next )
		{
			if( temp == rod )
			{
				if( prev == NULL )
					ss->rods = temp->next ;
				else
					prev->next = temp->next ;
				rodFree(rod) ;
				return 1 ;
			}
			prev = temp ;
		}
	}

	return 0 ;
}

// nt add 2017/6/4
int ssDelRod2(SS* ss, SUPPORT* support)
{
	int n = 0 ;
	ROD* temp, *prev ;

	if( ss &&
		support )
	{
		prev = NULL ;
		temp = ss->rods ;
		while(temp)
		{
			if( temp->support1 == support ||
				temp->support2 == support )
			{
				if( prev == NULL )
				{
					ss->rods = temp->next ;
					rodFree(temp) ;
					temp = ss->rods ;
				}
				else
				{
					prev->next = temp->next ;
					rodFree(temp) ;
					temp = prev->next ;
				}
				n++ ;
			}
			else
			{
				prev = temp ;
				temp = temp->next ;
			}
		}
	}

	return n ; // nt modify 2017/9/3 old return 0
}

// nt add 2017/9/1
int ssGetNumOfRod(SS* ss)
{
	int n = 0 ;
	ROD* rod ;

	if( ss == NULL )
		return -1 ;
	
	for( rod = ss->rods ; rod ; rod = rod->next )
		n++ ;

	return n ;
}

// nt add 2017/9/30
int ssCalBox(SS* ss)
{
	int k ;
	SUPPORT* support ;
	ROD* rod ;
	BOX3D box ;

	mathClnBox(&(ss->box)) ;
	if( ss->base )
	{
		if( ss->base->facet )
		{
			facetCalBox(ss->base->facet, &box) ;
			mathCalBoxUnion(&box, &(ss->box), &(ss->box)) ;
		}
	}

	for( support = ss->supports ; support ; support = support->next )
	{
		if( supportIsDel(support) == 1 )
			continue ;
		for( k = 0 ; k < SUPPORT_MAXNUMFACET ; k++ )
		{
			if( support->facets[k] )
			{
				facetCalBox(support->facets[k], &box) ;
				mathCalBoxUnion(&box, &(ss->box), &(ss->box)) ;
			}
		}
	}

	for( rod = ss->rods ; rod ; rod = rod->next )
	{
		if( rodIsDel(rod) == 1 )
			continue ;
		if( rod->facet )
		{
			facetCalBox(rod->facet, &box) ;
			mathCalBoxUnion(&box, &(ss->box), &(ss->box)) ;
		}
	}

	return 1 ;
}

// nt add 2017/7/3
void ssMove(SS* ss, VEC3D v)
{
	int i ;
	SUPPORT* support ;
	ROD* rod ;

	if( ss == NULL )
		return ;

	if( ss->base &&
		ss->base->facet )
	{
		facetMove(ss->base->facet, v) ;
		mathMoveBox(&(ss->base->box), v) ;
	}

	for( support = ss->supports ; support ; support = support->next )
	{
		for( i = 0 ; i < SUPPORT_MAXNUMFACET ; i++ )
		{
			if( support->facets[i] )
				facetMove(support->facets[i], v) ;
		}
	}

	for( rod = ss->rods ; rod ; rod = rod->next )
	{
		if( rod->facet )
			facetMove(rod->facet, v) ;
	}

	mathMoveBox(&(ss->box), v) ;

	return ;
}

// nt add 2017/7/3
void ssRotate(SS* ss, PNT3D pivot, VEC3D axis, double a)
{
	int i ;
	SUPPORT* support ;
	ROD* rod ;

	if( ss == NULL )
		return ;

	if( ss->base &&
		ss->base->facet )
	{
		facetRotate(ss->base->facet, pivot, axis, a) ;
		mathRotateBox(&(ss->base->box), pivot, axis, a) ;
	}

	for( support = ss->supports ; support ; support = support->next )
	{
		for( i = 0 ; i < SUPPORT_MAXNUMFACET ; i++ )
		{
			if( support->facets[i] )
				facetRotate(support->facets[i], pivot, axis, a) ;
		}
	}

	for( rod = ss->rods ; rod ; rod = rod->next )
	{
		if( rod->facet )
			facetRotate(rod->facet, pivot, axis, a) ;
	}

	mathRotateBox(&(ss->box), pivot, axis, a) ;

	return ;
}

// 全部绘制
void ssDraw(SS* ss, void* pVI)
{
	MATERIAL m_material ;
	SUPPORT* support ;
	ROD* rod ; // nt add 2017/6/4

	if( ss == NULL )
		return ;

	materialGet(&m_material, "BlackRubber2") ;
	materialSet(&m_material) ;
	if( ss->editing == 0 ) // 绘制完整的支撑结构
	{
		if( ss->base )
			baseDraw(ss->base, pVI) ;
		for( support = ss->supports ; support ; support = support->next )
		{
			if( supportIsDel(support) == 1 )
				continue ;
			supportDraw(support, pVI) ;
		}
		// nt add 2017/6/4
		for( rod = ss->rods ; rod ; rod = rod->next )
		{
			if( rodIsDel(rod) == 1 )
				continue ;
			rodDraw(rod, pVI) ;
		}
	}
	else // 绘制所有接触点
	if( ss->editing == 1 ) // nt add this line 2017/6/7
	{
		double r = 0.5 ;
		PNT3D ori = {0., 0., 0.} ;
		if( ss->supports )
			r = 2*ss->supports->r ;
		FACET* facet = facetSphere(ori, r, 8, 8) ;
		if( facet == NULL )
			return ;
		for( support = ss->supports ; support ; support = support->next )
		{
			if( supportIsDel(support) == 1 )
				continue ;
			glPushMatrix() ;
			glTranslated(support->p[0], support->p[1], support->p[2]) ;
			glVertexPointer(3, GL_DOUBLE, sizeof(FNODE), facet->nodes[0].p) ;
			glNormalPointer(GL_DOUBLE, sizeof(FNODE), facet->nodes[0].normal) ;
			glDrawElements(GL_TRIANGLES, facet->ntria*3, GL_UNSIGNED_INT, facet->trias) ;
			glPopMatrix() ;
		}
		facetFree(facet) ;
	}
	else
	if( ss->editing == 2 ) // nt add 2017/6/7
	{
		for( support = ss->supports ; support ; support = support->next )
		{
			if( supportIsDel(support) == 1 )
				continue ;
			supportDraw2(support, pVI) ;
		}
		for( rod = ss->rods ; rod ; rod = rod->next )
		{
			if( rodIsDel(rod) == 1 )
				continue ;
			rodDraw2(rod, pVI) ;
		}
	}

	return ;
}

/*	2017/3/28 nt
 *  拾取SUPPORT
 */
SUPPORT* ssPickSupport(SS* ss, 
			           PNT3D pos, 
	                   VEC3D dir,
	                   double r)
{
	double d ;
	SUPPORT* support ;

	if( ss )
	{
		for( support = ss->supports ; support ; support = support->next )
		{
			if( supportIsDel(support) == 1 )
				continue ;
			d = mathDistPntLin(support->p, pos, dir) ;
			if( d < r )
				return support ;
		}
	}

	return NULL ;
}

// nt add 2017/6/5
SUPPORT* ssPickSupport2(SS* ss, 
			            PNT3D pos, 
	                    VEC3D dir,
	                    double r,
	                    double z,
	                    PNT3D p)
{
	SUPPORT* support ;

	if( ss )
	{
		for( support = ss->supports ; support ; support = support->next )
		{
			if( supportIsDel(support) == 1 )
				continue ;
			double d ;
			PNT3D q, q1, q2 ;
			VEC3D Z = {0., 0., 1.} ;
			mathEvalLin(support->p, support->normal, support->d, q) ;
			mathIntLin(q, Z, pos, dir, MIN_LEN, MIN_ANG, q1, q2) ;
			d = mathDist(q1, q2) ;
			if( d < r &&
				//q1[2] > support->h-r && // nt 注释掉该行2017/8/30
				q1[2] > support->z-r && // nt add 2017/8/30
				q1[2] < q[2]+r )
			{
				d = (q1[0]-pos[0])*dir[0]+
					(q1[1]-pos[1])*dir[1]+
					(q1[2]-pos[2])*dir[2] ;
				if( z > d )
				{
					p[0] = q[0] ;
					p[1] = q[1] ;
					if( fabs(q1[2]-q[2]) < r ) // navigate
						p[2] = q[2] ;
					else
					//if( fabs(q1[2]-support->h) < r ) // navigate
					if( fabs(q1[2]-support->z) < r ) // navigate
						p[2] = support->z ; // nt modify 2017/8/30 old use ->h
					else
						p[2] = q1[2] ;
					return support ;
				}
			}
		}
	}

	return NULL ;
}

/*	2017/6/4 nt
 *  拾取ROD
 */
ROD* ssPickRod(SS* ss, 
			   PNT3D pos, 
	           VEC3D dir,
	           double r,
	           double z)
{
	ROD* rod, *best = NULL ;

	if( ss )
	{
		for( rod = ss->rods ; rod ; rod = rod->next )
		{
			if( rodIsDel(rod) == 1 )
				continue ;
			double d, L, Z ;
			PNT3D q1, q2 ;
			VEC3D v ;
			v[0] = rod->p2[0]-rod->p1[0] ;
			v[1] = rod->p2[1]-rod->p1[1] ;
			v[2] = rod->p2[2]-rod->p1[2] ;
			L = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
			if( L < MIN_LEN )
				continue ;
			v[0] /= L ;
			v[1] /= L ;
			v[2] /= L ;
			mathIntLin(rod->p1, v, pos, dir, MIN_LEN, MIN_ANG, q1, q2) ;
			d = mathDist(q1, q2) ;
			if( d < r )
			{
				d = (q1[0]-rod->p1[0])*v[0]+
					(q1[1]-rod->p1[1])*v[1]+
					(q1[2]-rod->p1[2])*v[2] ;
				Z = (q1[0]-pos[0])*dir[0]+
					(q1[1]-pos[1])*dir[1]+
					(q1[2]-pos[2])*dir[2] ;
				if( d > 0. &&
					d < L &&
					z > Z )
					return rod ;
			}
		}
	}

	return NULL ;
}

// nt add 2017/8/24
// 把merged中手动生成的支撑加到ss中，并这部分手动生成的还要rebuild
int ssMerge(SS* ss, SS* merged, STL* stl, SSP* ssp)
{
	SUPPORT* support, *copy ;

	if( ss == NULL ||
		merged == NULL ||
		ssp == NULL )
		return 0 ;

	for( support = merged->supports ; support ; support = support->next )
	{
		if( support->flag == 0 ) // 手动生成的
		{
			copy = supportCreate(support->p,
				                 support->normal,
				                 ssp->h,
				                 ssp->r1,
				                 ssp->r2,
				                 ssp->r,
				                 ssp->d) ;
			ssAddSupport(ss, copy) ;
		}
	}

	ssRebuildEx(ss, stl, ssp) ; // nt modify 2017/9/4

	return 1 ;
}

/*	2017/3/28 nt
 *  重建新生成的SUPPORT的FACET，删除已标记删除的（id<0）
 *	不用创建底座
 */
int ssRebuild(SS* ss, STL* stl, SSP* ssp)
{
	int i, j ;
	double z, a, b ;
	PNT3D p ;
	VEC3D normal ;
	STLBLK* blk = NULL ;
	STLTRI* tri = NULL ;
	SUPPORT* support, *prev ;

	if( ss == NULL ||
		stl == NULL ||
		ssp == NULL )
		return 0 ; // error
	b = ssp->b*PI1/180 ; // nt add 2017/6/9

	// 生成支撑
	for( support = ss->supports ; support ; support = support->next )
	{
		if( supportIsRaw(support) == 1 ) // support有待生成FACETs
		{
			a = ACOS(-support->normal[2]) ;
			if( a < b+MIN_ANG )
			{
				z = 0. ;
				mathVec(normal, 0., 0., 1.) ;
				memcpy(p, support->p, sizeof(PNT3D)) ;
				for( i = 0 ; i < stl->nBlk ; i++ )
				{
					blk = &(stl->blks[i]) ;
					for( j = 0 ; j < blk->nTri ; j++ )
					{
						tri = &(blk->tris[j]) ;
						if( tri->normal[2] > 0. && // ???????????
							triIntZ2(tri->vs, tri->normal, p, ssp->tol, &z) == 1 )
							memcpy(normal, tri->normal, sizeof(VEC3D)) ;
					}
				}
				if( normal[2] > MIN_DBL )
				{
					if( z > ssp->h )
						supportMake2(support, z, normal, 8, 8) ;
					else
						//supportMake3(support, ssp->R, ssp->H, ssp->A, 8, 8) ;
						supportMake3(support, ssp->R, ssp->h, ssp->A, 8, 8) ;
				}
			}
		}
	}

	prev = NULL ;
	for( support = ss->supports ; support ; )
	{
		if( supportIsDel(support) == 1 || // support已被标记为删除
			supportIsRaw(support) == 1 ) // 未能生成FACET的
		{
			if( prev )
				prev->next = support->next ;
			else
				ss->supports = support->next ;
			ssDelRod2(ss, support) ; // nt add 2017/9/3
			supportFree(support) ;
			support = prev ? prev->next : ss->supports ;
		}
		else
		{
			prev = support ;
			support = support->next ;
		}
	}

	return 1 ;
}

// nt rewrite 2017/9/4 only used in ssMerge()
// 不再删除标志为删除的support，因为没有这种support了
int ssRebuildEx(SS* ss, STL* stl, SSP* ssp)
{
	int i, j ;
	double z, a, b ;
	PNT3D p ;
	VEC3D normal ;
	STLBLK* blk = NULL ;
	STLTRI* tri = NULL ;
	SUPPORT* support ;

	if( ss == NULL ||
		stl == NULL ||
		ssp == NULL )
		return 0 ; // error
	b = ssp->b*PI1/180 ; // nt add 2017/6/9

	// 生成支撑
	for( support = ss->supports ; support ; support = support->next )
	{
		if( supportIsRaw(support) == 1 ) // support有待生成FACETs
		{
			a = ACOS(-support->normal[2]) ;
			if( a < b+MIN_ANG )
			{
				z = 0. ;
				mathVec(normal, 0., 0., 1.) ;
				memcpy(p, support->p, sizeof(PNT3D)) ;
				for( i = 0 ; i < stl->nBlk ; i++ )
				{
					blk = &(stl->blks[i]) ;
					for( j = 0 ; j < blk->nTri ; j++ )
					{
						tri = &(blk->tris[j]) ;
						if( tri->normal[2] > 0. && // ???????????
							triIntZ2(tri->vs, tri->normal, p, ssp->tol, &z) == 1 )
							memcpy(normal, tri->normal, sizeof(VEC3D)) ;
					}
				}
				if( normal[2] > MIN_DBL )
				{
					if( z > ssp->h )
						supportMake2(support, z, normal, 8, 8) ;
					else
						//supportMake3(support, ssp->R, ssp->H, ssp->A, 8, 8) ;
						supportMake3(support, ssp->R, ssp->h, ssp->A, 8, 8) ;
				}
			}
		}
	}

	return 1 ;
}

/*	2017/6/8 nt
 *  删除已标记删除的ROD
 */
int ssRebuild2(SS* ss)
{
	ROD* rod, *prev ;

	if( ss == NULL )
		return 0 ; // error

	prev = NULL ;
	for( rod = ss->rods ; rod ; )
	{
		if( rodIsDel(rod) == 1 ) // 已被标记为删除
		{
			if( prev )
				prev->next = rod->next ;
			else
				ss->rods = rod->next ;
			rodFree(rod) ;
			rod = prev ? prev->next : ss->rods ;
		}
		else
		{
			prev = rod ;
			rod = rod->next ;
		}
	}

	return 1 ;
}

// nt add 2017/8/31
int ssSerialize(SS* ss, CArchive& ar)
{
	int i, n ;
	SUPPORT* support, **array = NULL ;
	ROD* rod, *prev = NULL ;

	if( ss == NULL )
		return 0 ;

	if( ar.IsStoring() )
	{
		ar.Write(ss, sizeof(SS)) ;

		if( ss->base )
			baseSerialize(ss->base, ar) ;

		n = ssGetNumOfSupport(ss) ;
		ar << n ;
		i = 0 ;
		for( support = ss->supports ; support ; support = support->next )
		{
			support->no = i ;
			supportSerialize(support, ar) ;
			i++ ;
		}

		n = ssGetNumOfRod(ss) ;
		ar << n ;
		for( rod = ss->rods ; rod ; rod = rod->next )
			rodSerialize(rod, ar, NULL) ;
	}
	else
	{
		ar.Read(ss, sizeof(SS)) ;

		if( ss->base )
		{
			ss->base = baseCreate2() ;
			baseSerialize(ss->base, ar) ;
		}

		ar >> n ;
		if( n > 0 )
		{
			array = new SUPPORT*[n] ;
			for( i = 0 ; i < n ; i++ )
			{
				array[i] = supportCreate2() ;
				supportSerialize(array[i], ar) ;
				if( i > 0 )
					array[i-1]->next = array[i] ;
			}
			ss->supports = array[0] ;
		}

		ar >> n ;
		if( n > 0 )
		{
			prev = NULL ;
			for( i = 0 ; i < n ; i++ )
			{
				rod = rodCreate2() ;
				rodSerialize(rod, ar, array) ;
				if( prev )
					prev->next = rod ;
				else
					ss->rods = rod ;
				prev = rod ;
			}
		}

		delete []array ;
		array = NULL ;
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////