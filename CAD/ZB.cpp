#include "StdAfx.h"

////////////////////////////////////////////////////////////////
/*	2017/3/9 nt
 *  �����������Ƭ�����ڵģ����ʼ��da, ������1�����򣬷���0
 */
int daMake(DA* da,
	       STLTRI* tri1, 
	       STLTRI* tri2, 
	       double tol, 
	       double ang)
{
	int i = -1, j = -1 ;
	PNT3D p, q ;

	if( da == NULL ||
		tri1 == NULL ||
		tri2 == NULL )
		return 0 ;
	da->a = -1. ;

	// �����������ҵ����ߵ�һ�Ա�
	for( i = 0 ; i < 3 ; i++ )
	{
		for( j = 0 ; j < 3 ; j++ )
		{
			if( mathDist(tri1->vs[(i+1)%3], tri2->vs[(j+1)%3]) < tol &&
				mathDist(tri1->vs[(i+2)%3], tri2->vs[(j+2)%3]) < tol )
			{
				mathMidPnt(tri1->vs[(i+1)%3], tri2->vs[(j+1)%3], p) ;
				mathMidPnt(tri1->vs[(i+2)%3], tri2->vs[(j+2)%3], q) ;
				goto Found ;
			}
			if( mathDist(tri1->vs[(i+1)%3], tri2->vs[(j+2)%3]) < tol &&
				mathDist(tri1->vs[(i+2)%3], tri2->vs[(j+1)%3]) < tol )
			{
				mathMidPnt(tri1->vs[(i+1)%3], tri2->vs[(j+2)%3], p) ;
				mathMidPnt(tri1->vs[(i+2)%3], tri2->vs[(j+1)%3], q) ;
				goto Found ;
			}
		}
	}

	return 0 ; // error δ�ҵ��غϵĹ�����

Found:
	mathGetVec(p, q, da->e) ;
	if( mathUniVec(da->e, MIN_LEN) != ERSUCSS )
		return 0 ; // �������˻�
	mathMidPnt(p, q, da->p) ;
	memcpy(da->p1, tri1->vs[i], sizeof(PNT3D)) ;
	memcpy(da->n1, tri1->normal, sizeof(VEC3D)) ;
	memcpy(da->p2, tri2->vs[j], sizeof(PNT3D)) ;
	memcpy(da->n2, tri2->normal, sizeof(VEC3D)) ;

	return 1 ;
}

/*	2017/3/8 nt
 *	
 */
int paMake(PA* pa,
	       int n, 
	       IPOINT** array, 
	       double tol, 
	       double ang)
{
	int i, j, *adjs ;
	DA* da ;

	pa->n = 0 ;
	if( n < 2 )
		return 0 ; // error
	if( n > pa->max )
		paAlloc(pa, n) ;
	adjs = (int*)malloc(n*sizeof(int)) ;
	for( i = 0 ; i < n ; i++ )
		adjs[i] = 0 ;
	
	for( i = 0 ; i < n-1 ; i++ ) // ���һ�����ô���
	{
		for( j = i+1 ; j < n ; j++ )
		{
			if( adjs[i] >= 2 ) // ����������IPOINT�������
				break ;
			da = &(pa->das[pa->n]) ;
			if( adjs[j] < 2 &&
				daMake(da, 
					   array[i]->tri, 
					   array[j]->tri, 
					   tol,
					   ang) == 1 )
			{
				(pa->n)++ ;
				(adjs[i])++ ;
				(adjs[j])++ ;
			}
		}
		if( adjs[i] != 2 )
		{
			free(adjs) ;
			return 0 ; // û�ҵ��������ڵ��棬����ǲ����
		}
	}
	free(adjs) ;

	return 1 ;
}
//==============================================================
void ipointInit(IPOINT* point)
{
	if( point )
	{
		point->next = NULL ;
		point->tri = NULL ;
		point->z = 0. ;
		point->attrib = 0 ;
	}

	return ;
}

void ipointInit2(IPOINT* point, double z, int attrib, STLTRI* tri)
{
	if( point )
	{
		point->next = NULL ;
		point->z = z ;
		point->attrib = attrib ;
		point->tri = tri ;
	}

	return ;
}

int ipointGetAttribID(IPOINT* point, int np)
{
	if( np == -1 )
	{
		if( point->attrib&IPOINT_ON1 )
			return IDON ; // ɨ����z-�ڱ���
		else
		if( point->attrib&IPOINT_IN1 )
			return IDIN ; // ɨ����z-���ڲ�
		else
		if( point->attrib&IPOINT_OUT1 )
			return IDOUT ; // ɨ����z-���ⲿ
	}
	else // np == 1
	{
		if( point->attrib&IPOINT_ON2 )
			return IDON ; // ɨ����z+�ڱ���
		else
		if( point->attrib&IPOINT_IN2 )
			return IDIN ; // ɨ����z+���ڲ�
		else
		if( point->attrib&IPOINT_OUT2 )
			return IDOUT ; // ɨ����z+���ⲿ
	}

	return -1 ;
}

void ipointSetAttribID(IPOINT* point, int np, int attribID)
{
	if( np == -1 )
	{
		if( attribID == IDON )
			point->attrib |= IPOINT_ON1 ; // ɨ����z-�ڱ���
		else
		if( attribID == IDIN )
			point->attrib |= IPOINT_IN1 ; // ɨ����z-���ڲ�
		else
		if( attribID == IDOUT )
			point->attrib |= IPOINT_OUT1 ; // ɨ����z-���ⲿ
	}
	else // np == 1
	{
		if( attribID == IDON )
			point->attrib |= IPOINT_ON2 ; // ɨ����z+�ڱ���
		else
		if( attribID == IDIN )
			point->attrib |= IPOINT_IN2 ; // ɨ����z+���ڲ�
		else
		if( attribID == IDOUT )
			point->attrib |= IPOINT_OUT2 ; // ɨ����z+���ⲿ
	}

	return ;
}

/*	2017/3/3 nt
 *	�����Ƕ������Ƭ�Ĺ�������
 */
int ipointClassifyVertex(int n, 
	                     IPOINT** array, 
	                     double tol, 
	                     double ang)
{
	int i, attribID ;
	VEC3D ray = {0., 0., 0.} ;
	PA* pa = NULL ;

	pa = paCreate(PA_MAX_DA) ;
	if( paMake(pa, // ����IPOINT���鴴������ǽṹ
	           n, 
	           array, 
	           tol, 
	           ang) != 1 )
	{
		paFree(pa) ;
		return 0 ; // error
	}

	for( i = -1 ; i < 2 ; i += 2 )
	{
		ray[2] = (double)i ;
		attribID = paRayClassify(pa, ray, ang) ;
		ipointSetAttribID(array[0], i, attribID) ;
	}

	for( i = 1 ; i < n ; i++ )
		array[i]->tri = NULL ;
	paFree(pa) ;

	return 1 ;
}

/*	2017/3/3 nt
 *	��������������Ƭ�ı߽���
 */
int ipointClassifyEdge(IPOINT* point1, 
	                   IPOINT* point2, 
	                   double tol, 
	                   double ang)
{
	int attribID ;
	VEC3D ray = {0., 0., 0.} ;
	DA da ;

	// nt add 2017/3/18
	if( (point1->attrib&IPOINT_ON1) && // ���غ����
		(point2->attrib&IPOINT_ON2) )
	{
		point1->attrib |= IPOINT_ON2 ;
		point2->tri = NULL ;
		return 1 ;
	}
	if( (point1->attrib&IPOINT_ON2) && // ���غ����
		(point2->attrib&IPOINT_ON1) )
	{
		point1->attrib |= IPOINT_ON1 ;
		point2->tri = NULL ;
		return 1 ;
	}

	if( daMake(&da, // ��������IPOINT��������ǽṹ
	           point1->tri, 
	           point2->tri, 
	           tol, 
	           ang) != 1 ||
		daCalStdXY(&da) != 1 )
		return 0 ; // error

	if( (point1->attrib&IPOINT_ON1) || // �Ѿ��غ���
		(point2->attrib&IPOINT_ON1) )
		attribID = IDON ;
	else
	{
		ray[2] = -1. ;
		attribID = daRayClassify(&da, ray, ang) ;
	}
	ipointSetAttribID(point1, -1, attribID) ;

	if( (point1->attrib&IPOINT_ON2) || // �Ѿ��غ���
		(point2->attrib&IPOINT_ON2) )
		attribID = IDON ;
	else
	{
		ray[2] = 1. ;
		attribID = daRayClassify(&da, ray, ang) ;
	}
	ipointSetAttribID(point1, 1, attribID) ;

	point2->tri = NULL ;

	return 1 ;
}

/*	2017/3/3 nt
 *	������һ������Ƭ�ڲ�
 */
int ipointClassifyFace(IPOINT* point, double tol, double ang)
{
	STLTRI* tri ;

	if( point == NULL ||
		point->tri == NULL )
		return 0 ;
	tri = point->tri ;
	//if( fabs(tri->normal[2]) < ang ) // Ӧ�ò���������������
	if( (tri->attrib&STLTRI_PARALLEL_Z) == 1 )
	{
		point->attrib |= IPOINT_ON1 ;
		point->attrib |= IPOINT_ON2 ;
	}
	else
	if( tri->normal[2] > 0. )
	{
		point->attrib |= IPOINT_IN1 ;
		point->attrib |= IPOINT_OUT2 ;
	}
	else
	{
		point->attrib |= IPOINT_OUT1 ;
		point->attrib |= IPOINT_IN2 ;
	}

	return 1 ;
}

/*	2017/3/1 nt
 *  ע��array�еĽ������غϵ�
 */
int ipointClassify(int n, IPOINT** array, double tol, double ang)
{
	int i ;

	if( n < 1 ||
		array == NULL )
		return 0 ; // error
	for( i = 0 ; i < n ; i++ )
	{
		if( array[i] == NULL ||
			array[i]->tri == NULL )
			return 0 ; // error
	}

	if( array[0]->attrib&IPOINT_FACE ) // һ������������Ƭ�ڲ������
	{
		if( n != 1 )
			return 0 ; // error
		return ipointClassifyFace(array[0], tol, ang) ;
	}
	else
	if( array[0]->attrib&IPOINT_EDGE ) // ��������������Ƭ�ı߽���
	{
		if( n != 2 )
			return 0 ; // error
		return ipointClassifyEdge(array[0], array[1], tol, ang) ;
	}
	else
	if( array[0]->attrib&IPOINT_VERTEX ) // �������������Ƭ�Ķ����غ�
	{
		if( n < 3 )
			return 0 ; // error
		return ipointClassifyVertex(n, array, tol, ang) ;
	}

	return 1 ;
}
//==============================================================
int blkInit(BLK* blk, int max)
{
	if( blk )
	{
		blk->max = max ;
		blk->n = 0 ;
		if( max > 0 )
		{
			blk->points = (IPOINT*)malloc(max*sizeof(IPOINT)) ;
			if( blk->points == NULL )
				return 0 ;
		}
		else
			blk->points = NULL ;
	}

	return 1 ;
}

void blkEmpty(BLK* blk)
{
	if( blk )
	{
		if( blk->points )
			free(blk->points) ;
	}

	return ;
}
//==============================================================
void slineInit(SLINE* line, PNT3D p)
{
	if( line &&
		p )
	{
		line->flags = 0 ;
		line->n = 0 ;
		memcpy(line->p, p, sizeof(PNT3D)) ;
		line->points = NULL ;
	}

	return ;
}

void slineInit2(SLINE* line, double x, double y)
{
	if( line )
	{
		line->flags = 0 ;
		line->n = 0 ;
		line->p[0] = x ;
		line->p[1] = y ;
		line->p[2] = 0. ;
		line->points = NULL ;
	}

	return ;
}

void slineAdd(SLINE* line, IPOINT* point)
{
	if( line &&
		point )
	{
		point->next = line->points ;
		line->points = point ;
		(line->n)++ ;
	}

	return ;
}

// ��ɨ�����е�IPOINT���������У����ı�IPOINT->next��line->points��ָ��
// ע��IPOINT��line->points�еĴ�������array�е���ȫ������ͬ!!!!!!!!!!!
int slineArray(SLINE* line, int nSize, IPOINT** array)
{
	int n = 0 ;
	IPOINT* point ;

	for( point = line->points ; point ; point = point->next )
	{
		if( n > nSize )
			return -1 ; // error
		array[n] = point ;
		n++ ;
	}
	if( line->n != n )
		return -1 ; // error

	return n ;
}

// ��array�е�tri��Ϊ�յ�IPOINT������line->points��, 
// ���ı�IPOINT��˳��
// array�е�triΪ�յ�IPOINT��������line->points��
int slineMerge(SLINE* line, int n, IPOINT** array)
{
	int i, attribID1, attribID2 ;
	IPOINT* point ;

	if( line == NULL ||
		n < 1 ||
		array == NULL )
		return 1 ;

	ipointSetAttribID(array[0], -1, IDOUT) ;
	ipointSetAttribID(array[n-1], 1, IDOUT) ;

	/*for( i = 0 ; i < n ; i++ )
	{
		attribID1 = ipointGetAttribID(array[i], -1) ;
		attribID2 = ipointGetAttribID(array[i], 1) ;
		if( attribID1 == attribID2 )
			array[i]->tri = NULL ;
	}*/

	line->n = 0 ;
	line->points = NULL ;
	for( i = n-1 ; i >= 0 ; i-- )
	{
		if( array[i]->tri )
		{
			array[i]->next = line->points ;
			line->points = array[i] ;
			(line->n)++ ;
		}
	}

	for( point = line->points ; point && point->next ; )
	{
		attribID1 = ipointGetAttribID(point, 1) ;
		attribID2 = ipointGetAttribID(point->next, -1) ;
		if( (attribID1 == IDOUT && attribID2 == IDIN) ||
			(attribID1 == IDIN && attribID2 == IDOUT) )
		{
			line->flags = 1 ; // nt add 2017/3/16
			return 0 ;
		}
		point = point->next ;
	}

	return 1 ;
}

// �Խ���������򣬰�xֵ��С����Ĵ���
// ��ʱʹ��array
int slineSort(SLINE* line, int nSize, IPOINT** array)
{
	int i, j, n = 0 ;
	IPOINT* point ;

	if( (n = slineArray(line, nSize, array)) < 0 )
		return 0 ; // error
	for( i = 0 ; i < n-1 ; i++ ) // array[]�Ӵ�С����
	{
		for( j = i+1 ; j < n ; j++ )
		{
			if( array[i]->z < array[j]->z )
			{
				point = array[i] ;
				array[i] = array[j] ;
				array[j] = point ;
			}
		}
	}
	line->points = NULL ;
	for( i = 0 ; i < n ; i++ ) // points��z��С��������
	{
		array[i]->next = line->points ;
		line->points = array[i] ;
	}

	return 1 ;
}

// ��������õĽ�����з��࣬���ж��ڽ��㴦����������
// ��ʱʹ��array
// ע������ǰ�zֵ��С�����ź�!!!!!!!!!!!!!!!!!!!!!!!
int slineClassify(SLINE* line, 
				  double tol, 
	              double ang, 
	              int nSize, 
	              IPOINT** array)
{
	int i, j, n, m ;
	double d ;

	n = line->n ;
	if( n <= 0 ) // �޽���
		return 1 ;
	if( n == 1 )
		return 1 ; // error, ���滹Ҫ����line->flagsΪ1
	if( (n = slineArray(line, nSize, array)) < 0 ||
		n != line->n )
		return 0 ; // error
	
	// һ�����
	if( line->n%2 == 0 ) // ��ż��������
	{
		for( i = 0 ; i < n ; i++ )
		{
			if( (array[i]->attrib&IPOINT_FACE) == 0 )
				break ;
		}
		if( i >= n ) // ����һ�������Ҳ�Ǽ����
		{
			for( i = 0 ; i < n ; i++ )
			{
				if( i%2 == 0 )
				{
					array[i]->attrib |= IPOINT_OUT1 ; // �����
					array[i]->attrib |= IPOINT_IN2 ; // ���ڲ�
				}
				else
				{
					array[i]->attrib |= IPOINT_IN1 ; // ���ڲ�
					array[i]->attrib |= IPOINT_OUT2 ; // �����
				}
			}
			return 1 ; // success
		}
	}

	// �����������
	for( i = 0 ; i < n ; i += m )
	{
		m = 1 ; // ��array[i]�غϵĽ���������1��ʾһ��array[i]
		for( j = i+1 ; j < n ; j++ )
		{
			d = fabs(array[j]->z-array[i]->z) ;
			if( d < tol )
				m++ ;
			else
				break ;
		}
		if( ipointClassify(m, array+i, tol, ang) != 1 )
			line->flags = 1 ; // set error flag
	}

	return 1 ;
}

/*  2017/3/14 nt
 *  �ж���������֮��ɨ���ߵľֲ����ԣ�
 *  ����0=out,1=in,2=on
 */
int slineChkAttrib(IPOINT* point1, IPOINT* point2)
{
	if( point1 &&
		point2 )
	{
		if( point1->z < point2->z )
		{
			if( point1->attrib&IPOINT_IN2 || // ��ΪֻҪ��һ����in����in
				point2->attrib&IPOINT_IN1 )
				return 1 ; // interior
			else
			if( point1->attrib&IPOINT_ON2 &&
				point2->attrib&IPOINT_ON1 )
				return 2 ; // on boundary
			else
				return 0 ; // outside
		}
		else
		{
			if( point2->attrib&IPOINT_IN2 || // ��ΪֻҪ��һ����in����in
				point1->attrib&IPOINT_IN1 )
				return 1 ; // interior
			else
			if( point2->attrib&IPOINT_ON2 &&
				point1->attrib&IPOINT_ON1 )
				return 2 ; // on boundary
			else
				return 0 ; // outside
		}
	}
	else
		return -1 ; // error
}

// ע������ǰ�zֵ��С�����ź�!!!
double slineCalLen(SLINE* line)
{
	double d = 0. ;
	IPOINT* point1, *point2 ;

	for( point1 = line->points ; point1 ; )
	{
		point2 = point1->next ;
		if( slineChkAttrib(point1, point2) > 0 )
			d += fabs(point2->z-point1->z) ;
		point1 = point2 ;
	}

	return d ;
}

double slineCalLen2(SLINE* line)
{
	int rt ;
	double d = 0., s ;
	IPOINT* point1, *point2 ;

	for( point1 = line->points ; point1 ; )
	{
		point2 = point1->next ;
		rt = slineChkAttrib(point1, point2) ;
		if( rt > 0 )
		{
			s = fabs(point2->z-point1->z) ;
			if( rt == 2 ) // on�����
				s *= 0.5 ;
			d += s ;
		}
		point1 = point2 ;
	}

	return d ;
}

// nt add 2017/3/21
int slineChkPt(SLINE* line, double z, double tol)
{
	IPOINT* point1, *point2 ;

	for( point1 = line->points ; point1 ; )
	{
		if( z < point1->z-tol )
			return 0 ; // not on
		point2 = point1->next ;
		if( point2 )
		{
			if( point1->z-tol < z &&
				z < point2->z+tol &&
				slineChkAttrib(point1, point2) > 0 )
				return 1 ; // on
		}
		point1 = point2 ;
	}

	return 0 ; // not on
}
//==============================================================
ZB* zbCreate(BOX3D* box, 
	         double w,
	         double h,
	         double tol,
			 double ang)
{
	int i, j ;
	double x, y ;
	SLINE* line = NULL ;
	ZB* zb = NULL ;

	zb = (ZB*)malloc(sizeof(ZB)) ;
	if( zb == NULL )
		return NULL ;
	zb->blks = NULL ;
	zb->lines = NULL ;
	zb->pixels = NULL ;
	zb->xmin = box->min[0]-w ;
	zb->xmax = box->max[0]+w ;
	zb->ymin = box->min[1]-w ;
	zb->ymax = box->max[1]+w ;
	zb->zmin = box->min[2] ;
	zb->zmax = box->max[2] ;
	zb->w = w ; // ��࣬x��y�����ϵ�ɨ���ߵļ��
	zb->h = h ; // ���, z�����ϵĲ�ĺ��
	zb->tol = tol ;
	zb->ang = ang ;

	zb->max = ZB_MAX_BLK ;
	zb->nBlk = 0 ;
	zb->blks = (BLK*)malloc(zb->max*sizeof(BLK)) ; // ����, 0...max-1
	if( zb->blks == NULL )
		goto Exit ;
	for( i = 0 ; i < zb->max ; i++ )
		blkInit(&(zb->blks[i]), 0) ; // û�з���IPOINT

	zb->nx = (int)ceil((zb->xmax-zb->xmin)/w)+1 ;
	zb->ny = (int)ceil((zb->ymax-zb->ymin)/w)+1 ;
	zb->lines = (SLINE**)malloc(zb->nx*sizeof(SLINE*)) ; // zb->nx
	if( zb->lines == NULL )
		goto Exit ;
	for( i = 0 ; i < zb->nx ; i++ )
		zb->lines[i] = NULL ;
	for( i = 0 ; i < zb->nx ; i++ )
	{
		x = zb->xmin+w*i ;
		zb->lines[i] = (SLINE*)malloc(zb->ny*sizeof(SLINE)) ; // zb->ny
		if( zb->lines[i] == NULL )
			goto Exit ;
		for( j = 0 ; j < zb->ny ; j++ )
		{
			y = zb->ymin+w*j ;
			slineInit2(&(zb->lines[i][j]), x, y)  ;
		}
	}
	zb->maxIP = 0 ;

	zb->nDege = 0 ;
	zb->nOpen = 0 ;
	zb->nMem = 0 ;
	zb->nOther = 0 ;

	// nt add 2017/3/21
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
					free(zb->lines[i]) ;
			}
			free(zb->lines) ;
		}
		if( zb->blks )
			free(zb->blks) ;
		if( zb->pixels )
			free(zb->pixels) ;
		free(zb) ;
	}

	return NULL ;
}

// nt add 2021/6/4
ZB* zbCreate2(double xmin, 
			  double ymin, 
			  int nx, 
			  int ny, 
	          double w,
	          double tol,
			  double ang)
{
	int i, j ;
	double x, y ;
	SLINE* line = NULL ;
	ZB* zb = NULL ;

	zb = (ZB*)malloc(sizeof(ZB)) ;
	if( zb == NULL )
		return NULL ;
	zb->blks = NULL ;
	zb->lines = NULL ;
	zb->pixels = NULL ;
	zb->xmin = xmin ;
	zb->xmax = xmin+w*(nx-1) ;
	zb->ymin = ymin ;
	zb->ymax = ymin+w*(ny-1) ;
	zb->zmin = 0. ;
	zb->zmax = MAX_DBL ;
	zb->w = w ; // ��࣬x��y�����ϵ�ɨ���ߵļ��
	zb->h = 0. ; // ���, z�����ϵĲ�ĺ��
	zb->tol = tol ;
	zb->ang = ang ;

	zb->max = ZB_MAX_BLK ;
	zb->nBlk = 0 ;
	zb->blks = (BLK*)malloc(zb->max*sizeof(BLK)) ; // ����, 0...max-1
	if( zb->blks == NULL )
		goto Exit ;
	for( i = 0 ; i < zb->max ; i++ )
		blkInit(&(zb->blks[i]), 0) ; // û�з���IPOINT

	zb->nx = nx ;
	zb->ny = ny ;
	zb->lines = (SLINE**)malloc(zb->nx*sizeof(SLINE*)) ; // zb->nx
	if( zb->lines == NULL )
		goto Exit ;
	for( i = 0 ; i < zb->nx ; i++ )
		zb->lines[i] = NULL ;
	for( i = 0 ; i < zb->nx ; i++ )
	{
		x = zb->xmin+w*i ;
		zb->lines[i] = (SLINE*)malloc(zb->ny*sizeof(SLINE)) ; // zb->ny
		if( zb->lines[i] == NULL )
			goto Exit ;
		for( j = 0 ; j < zb->ny ; j++ )
		{
			y = zb->ymin+w*j ;
			slineInit2(&(zb->lines[i][j]), x, y)  ;
		}
	}
	zb->maxIP = 0 ;

	zb->nDege = 0 ;
	zb->nOpen = 0 ;
	zb->nMem = 0 ;
	zb->nOther = 0 ;

	// nt add 2017/3/21
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
					free(zb->lines[i]) ;
			}
			free(zb->lines) ;
		}
		if( zb->blks )
			free(zb->blks) ;
		if( zb->pixels )
			free(zb->pixels) ;
		free(zb) ;
	}

	return NULL ;
}

void zbFree(ZB* zb)
{
	int i ;

	if( zb )
	{
		if( zb->lines )
		{
			for( i = 0 ; i < zb->nx ; i++ )
				free(zb->lines[i]) ;
			free(zb->lines) ;
		}

		for( i = 0 ; i < zb->max ; i++ )
			blkEmpty(&(zb->blks[i])) ;
		free(zb->blks) ;
		if( zb->pixels )
			free(zb->pixels) ;
		free(zb) ;
	}

	return ;
}
//--------------------------------------------------------------
double zbGetSpacing(ZB* zb)
{
	return zb->w ;
}

double zbGetThickness(ZB* zb)
{
	return zb->h ;
}

int zbGetNumOfSlineX(ZB* zb)
{
	return zb->nx ;
}

int zbGetNumOfSLineY(ZB* zb)
{
	return zb->ny ;
}

// ȡ��i�е�j�е�ɨ����
SLINE* zbGetSLine(ZB* zb, int i, int j)
{
	if( i < 0 ||
		i >= zb->nx ||
		j < 0 ||
		j >= zb->ny )
		return NULL ;

	return &(zb->lines[i][j]) ;
}

void zbResI(ZB* zb, int& i)
{
	if( i < 0 )
		i = 0 ;
	else
	if( i > zb->nx-1 )
		i = zb->nx-1 ;

	return ;
}

void zbResJ(ZB* zb, int& j)
{
	if( j < 0 )
		j = 0 ;
	else
	if( j > zb->ny-1 )
		j = zb->ny-1 ;

	return ;
}

IPOINT* zbAllocIPoint(ZB* zb)
{
	int iBlk, n ;

	if( zb &&
		zb->blks )
	{
		// ����δ�õ�IPOINT�ռ�
		iBlk = zb->nBlk-1 ;
		if( iBlk >= 0 )
		{
			n = zb->blks[iBlk].n ;
			if( n < zb->blks[iBlk].max )
			{
				(zb->blks[iBlk].n)++ ;
				return &(zb->blks[iBlk].points[n]) ; // �������ڴ�
			}
		}

		// û��δ�õ�IPOINT�ռ�
		(zb->nBlk)++ ;
		if( zb->nBlk <= zb->max )
		{
			iBlk = zb->nBlk-1 ;
			if( blkInit(&(zb->blks[iBlk]), BLK_MAX_IPOINT) != 1 )
			{
				(zb->nMem)++ ;
				return NULL ;
			}
			zb->blks[iBlk].n = 1 ;
			return &(zb->blks[iBlk].points[0]) ;
		}
		else
			return NULL ; //???
	}
	else
		return NULL ; // error
}

// i: i_scanline along x axis
// j: j_scanline along y axis
int zbAdd(ZB* zb, int i, int j, double z, int attrib, STLTRI* tri)
{
	IPOINT* point = NULL ;
	SLINE* line = NULL ;

	line = zbGetSLine(zb, i, j) ;
	if( line == NULL )
		return 0 ;
	point = zbAllocIPoint(zb) ;
	if( point == NULL )
		return 0 ;
	ipointInit2(point, z, attrib, tri) ;
	slineAdd(line, point) ;
	if( zb->maxIP < line->n ) // update zb->maxIP when add a IPOINT
		zb->maxIP = line->n ;

	return 1 ;
}
//--------------------------------------------------------------
// ����ɨ�������ӵ�����������ͬһ������Ƭ�ϵ����
int zbOnTri(ZB* zb, int i, int j)
{
	IPOINT* point1 = NULL, *point2 = NULL ;
	SLINE* line = NULL ;

	line = zbGetSLine(zb, i, j) ;
	if( line == NULL ||
		line->n < 2 )
		return 0 ;
	point1 = line->points ;
	point2 = point1->next ;
	if( point1->tri == point2->tri )
	{
		if( fabs(point1->tri->normal[2]) < PI1/180000 )
		{
			if( point1->z < point2->z )
			{
				point1->attrib |= IPOINT_ON2 ;
				point2->attrib |= IPOINT_ON1 ;
			}
			else
			{
				point1->attrib |= IPOINT_ON1 ;
				point2->attrib |= IPOINT_ON2 ;
			}
		}
		else
		{
			if( point2->attrib&IPOINT_VERTEX )
				point2->attrib -= IPOINT_VERTEX ;
			if( point2->attrib&IPOINT_EDGE )
				point2->attrib -= IPOINT_EDGE ;
			point2->attrib |= IPOINT_FACE ;
			line->points = point2 ; // ȥ������point1
			point1->tri = NULL ; // ���Ͻ���point1
		}
		return 1 ;
	}
	else
		return 0 ;
}

/*	2017/2/21 nt
 *	STLTRI/scan_line intersection
 *	�и�ƽ��ƽ����YZƽ�棨��ֱ��X�ᣩ
 */
int zbSliceSTLTriYZ(ZB* zb, double xmin, double xmax, STLTRI* tri)
{
	int i, j, n, i1, i2, j1, j2, as[2], js[2], attrib ;
	double x, y, z, dx = 0., dy = 0. ;
	PNT3D ps[2] ;

	i1 = (int)ceil((xmin-zb->tol-zb->xmin)/zb->w) ; // (...)/w <= i1
	i2 = (int)floor((xmax+zb->tol-zb->xmin)/zb->w) ; // i2 <= (...)/w
	if( fabs(tri->normal[2]) > MIN_DBL )
	{
		dx = tri->normal[0]/tri->normal[2] ;
		dy = tri->normal[1]/tri->normal[2] ;
	}
	for( i = i1 ; i <= i2 ; i++ ) // ��x��������и�ƽ��
	{
		x = zb->xmin+zb->w*i ;
		n = triCut(tri->vs[0], 
			       tri->vs[1], 
			       tri->vs[2], 
			       0,
			       x, 
			       zb->tol,
	               ps,
				   as) ; // Ӧ��û�з���3�����
		js[0] = -2000000000 ;
		js[1] = -2000000000 ;
		if( n == 1 ||
			n == 2 )
		{
			j1 = (int)ceil((ps[0][1]-zb->tol-zb->ymin)/zb->w) ;
			y = zb->ymin+zb->w*j1 ;
			if( fabs(y-ps[0][1]) < zb->tol ) // ��(x,y,0)ɨ���߹�ps[0]
			{
				if( zbAdd(zb, i, j1, ps[0][2], as[0], tri) == 0 )
					return 0 ;
				js[0] = j1 ;
				j1++ ;
			}
		}
		if( n == 2 )
		{
			j2 = (int)floor((ps[1][1]+zb->tol-zb->ymin)/zb->w) ;
			y = zb->ymin+zb->w*j2 ;
			if( fabs(y-ps[1][1]) < zb->tol ) // ��(x,y,0)ɨ���߹�ps[1]
			{
				if( zbAdd(zb, i, j2, ps[1][2], as[1], tri) == 0 )
					return 0 ;
				js[1] = j2 ;
				j2-- ;
			}

			if( js[0] >= 0 && js[0] == js[1] ) // ɨ����������Ƭ�غ�
				zbOnTri(zb, i, j) ;
			else
			{
				if( as[0] == IPOINT_VERTEX && 
					as[1] == IPOINT_VERTEX )
					attrib = IPOINT_EDGE ;
				else
					attrib = IPOINT_FACE ;
				for( j = j1 ; j <= j2 ; j++ ) // tri->normal[2]!=0.
				{
					y = zb->ymin+zb->w*j ;
					z = tri->vs[0][2]+ // ɨ����/����Ƭ��, ���ڲ���
						dx*(tri->vs[0][0]-x)+
						dy*(tri->vs[0][1]-y) ;
					if( zbAdd(zb, i, j, z, attrib, tri) == 0 )
						return 0 ;
				}
			}
		}
	}

	return 1 ;
}

/*	2017/2/21 nt
 *	STLTRI/scan_line intersection
 *	�и�ƽ��ƽ����XZƽ�棨��ֱ��Y�ᣩ
 */
int zbSliceSTLTriXZ(ZB* zb, double ymin, double ymax, STLTRI* tri)
{
	int i, j, n, i1, i2, j1, j2, as[2], is[2], attrib ;
	double x, y, z, dx = 0., dy = 0. ;
	PNT3D ps[2] ;

	j1 = (int)ceil((ymin-zb->tol-zb->ymin)/zb->w) ; // (...)/w <= j1
	j2 = (int)floor((ymax+zb->tol-zb->ymin)/zb->w) ; // j2 <= (...)/w
	if( fabs(tri->normal[2]) > MIN_DBL )
	{
		dx = tri->normal[0]/tri->normal[2] ;
		dy = tri->normal[1]/tri->normal[2] ;
	}
	for( j = j1 ; j <= j2 ; j++ ) // ��y��������и�ƽ��
	{
		y = zb->ymin+zb->w*j ;
		n = triCut(tri->vs[0], 
			       tri->vs[1], 
			       tri->vs[2], 
			       1,
			       y, 
			       zb->tol,
	               ps,
				   as) ; // Ӧ��û�з���3�����
		is[0] = -2000000000 ;
		is[1] = -2000000000 ;
		if( n == 1 ||
			n == 2 )
		{
			i1 = (int)ceil((ps[0][0]-zb->tol-zb->xmin)/zb->w) ;
			x = zb->xmin+zb->w*i1 ;
			if( fabs(x-ps[0][0]) < zb->tol ) // ��(x,y,0)ɨ���߹�ps[0]
			{
				if( zbAdd(zb, i1, j, ps[0][2], as[0], tri) == 0 )
					return 0 ;
				is[0] = i1 ;
				i1++ ;
			}
		}
		if( n == 2 )
		{
			i2 = (int)floor((ps[1][0]+zb->tol-zb->xmin)/zb->w) ;
			x = zb->xmin+zb->w*i2 ;
			if( fabs(x-ps[1][0]) < zb->tol ) // ��(x,y,0)ɨ���߹�ps[1]
			{
				if( zbAdd(zb, i2, j, ps[1][2], as[1], tri) == 0 )
					return 0 ;
				is[1] = i2 ;
				i2-- ;
			}

			if( is[0] >= 0 && is[0] == is[1] ) // ɨ����������Ƭ�غ�
				zbOnTri(zb, i, j) ;
			else
			{
				if( as[0] == IPOINT_VERTEX &&
					as[1] == IPOINT_VERTEX )
					attrib = IPOINT_EDGE ;
				else
					attrib = IPOINT_FACE ;
				for( i = i1 ; i <= i2 ; i++ ) // tri->normal[2]!=0.
				{
					x = zb->xmin+zb->w*i ;
					z = tri->vs[0][2]+ // ɨ����/����Ƭ��, ���ڲ���
						dx*(tri->vs[0][0]-x)+
						dy*(tri->vs[0][1]-y) ;
					if( zbAdd(zb, i, j, z, attrib, tri) == 0 )
						return 0 ;
				}
			}
		}
	}

	return 1 ;
}

/*	2017/2/21 nt
 *	STLTRI/scan_line intersection
 */
int zbSliceSTLTri(ZB* zb, STLTRI* tri)
{
	double xmin, xmax, ymin, ymax, xx, yy ;

	xmin = mathMIN2(tri->vs[0][0], tri->vs[1][0], tri->vs[2][0]) ;
	xmax = mathMAX2(tri->vs[0][0], tri->vs[1][0], tri->vs[2][0]) ;
	ymin = mathMIN2(tri->vs[0][1], tri->vs[1][1], tri->vs[2][1]) ;
	ymax = mathMAX2(tri->vs[0][1], tri->vs[1][1], tri->vs[2][1]) ;
	xx = xmax-xmin ;
	yy = ymax-ymin ;
	if( xx < zb->tol &&
		yy < zb->tol )
	{
		(zb->nDege)++ ;
		return 1 ; // ����Ƭ�˻�
	}
	if( xx > yy )
		return zbSliceSTLTriYZ(zb, xmin, xmax, tri) ;
	else
		return zbSliceSTLTriXZ(zb, ymin, ymax, tri) ;
}

/*	2017/2/21 nt
 *	STL/scan_line intersection
 *	nt add PRG 2017/9/10
 */
int zbSlice(ZB* zb, STL* stl, PRG* pPrg)
{
	int i, j ;
	double dd ;
	STLBLK* blk = NULL ;
	STLTRI* tri = NULL ;
	SLINE* line = NULL ;
	IPOINT** array = NULL ;

	if( zb == NULL ||
		stl == NULL )
		return 0 ;

	for( i = 0 ; i < stl->nBlk ; i++ )
	{
		blk = &(stl->blks[i]) ;
		for( j = 0 ; j < blk->nTri ; j++ )
		{
			tri = &(blk->tris[j]) ;
			if( tri )
			{
				if( zbSliceSTLTri(zb, tri) == 0 )
					return 0 ;
			}
		}
	}
	if( zb->maxIP < 2 )
		return 0 ; // error

	dd = 1./(zb->nx*zb->ny) ; // nt add 2017/9/10, nt modify 2021/6/4 ��()
	array = (IPOINT**)malloc(zb->maxIP*sizeof(IPOINT*)) ;
	for( i = 0 ; i < zb->nx ; i++ )
	{
		for( j = 0 ; j < zb->ny ; j++ )
		{
			line = zbGetSLine(zb, i, j) ;
			if( line )
			{
				if( slineSort(line, zb->maxIP, array) != 1 ||
					slineClassify(line, // �Խ����������Է���
						          zb->tol, 
						          zb->ang, 
						          zb->maxIP, 
						          array) != 1 )
				{
					free(array) ;
					return 0 ;
				}
				if( slineMerge(line, line->n, array) == 0 )
				{
					//slineDump(line, "d:\\1.txt") ;
					(zb->nOpen)++ ;
				}
			}

			// nt add 2017/9/10
			if( pPrg )
				pPrg->c += (dd*pPrg->ws[pPrg->i]) ;
		}
	}
	free(array) ;

	return 1 ;
}

int zbDraw(ZB* zb, void* pVI)
{
	int i, j ;
	PNT3D p, q ;
	SLINE* line ;
	IPOINT* point1, *point2 ;

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
			line = zbGetSLine(zb, i, j) ;
			for( point1 = line->points ; point1 ; )
			{
				point2 = point1->next ;
				if( slineChkAttrib(point1, point2) > 0 )
				{
					p[2] = point1->z ; // ȡz����ֵ
					q[2] = point2->z ; // ȡz����ֵ
					glVertex3dv(p) ;
					glVertex3dv(q) ;
				}
				point1 = point2 ;
			}
		}
	}
	glEnd() ;

	return 1 ;
}

int zbDraw2(ZB* zb, void* pVI)
{
	int i, j ;
	PNT3D p ;
	SLINE* line ;
	IPOINT* point ;

	glPointSize(1.f) ;
	glColor3ub(0, 0, 0) ;
	glBegin(GL_POINTS) ;
	for( i = 0 ; i < zb->nx ; i++ )
	{
		p[0] = zb->xmin+zb->w*i ;
		for( j = 0 ; j < zb->ny ; j++ )
		{
			p[1] = zb->ymin+zb->w*j ;
			line = zbGetSLine(zb, i, j) ;
			for( point = line->points ; point ; point = point->next )
			{
				p[2] = point->z ;
				glVertex3dv(p) ;
			}
		}
	}
	glEnd() ;

	return 1 ;
}

int zbDrawZ(ZB* zb, void* pVI)
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

int zbCut(ZB* zb, double z)
{
	int i, j, k ;
	SLINE* line ;
	
	k = 0 ;
	for( j = 0 ; j < zb->ny ; j++ )
	{
		for( i = 0 ; i < zb->nx ; i++ )
		{
			line = zbGetSLine(zb, i, j) ;
			if( slineChkPt(line, z, zb->tol) == 1 )
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

int zbGray(ZB* zb, int grays[4])
{
	uchar p, c ; // p:previous, c:current
	int i, j, k ;
	
	if( zb->cut )
	{
		k = 0 ;
		for( j = 0 ; j < zb->ny ; j++ )
		{
			p = 0 ; // initial value
			c = 0 ; // initial value
			for( i = 0 ; i < zb->nx ; i++ )
			{
				c = zb->pixels[k] ;
				if( p == 0 && c == 255 )
				{
					if( i >= 2 )
						zb->pixels[k-2] = (uchar)(255*grays[3]/100) ;
					if( i >= 1 )
						zb->pixels[k-1] = (uchar)(255*grays[2]/100) ;
					zb->pixels[k] = (uchar)(255*grays[1]/100) ;
					if( i+1 < zb->nx )
						zb->pixels[k+1] = (uchar)(255*grays[0]/100) ;
				}
				else
				if( p == 255 && c == 0 )
				{
					if( i >= 2 )
						zb->pixels[k-2] = (uchar)(255*grays[0]/100) ;
					if( i >= 1 )
						zb->pixels[k-1] = (uchar)(255*grays[1]/100) ;
					zb->pixels[k] = (uchar)(255*grays[2]/100) ;
					if( i+1 < zb->nx )
						zb->pixels[k+1] = (uchar)(255*grays[3]/100) ;
				}
				p = c ;
				k++ ;
			}
		}

		for( i = 0 ; i < zb->nx ; i++ )
		{
			p = 0 ; // initial value
			c = 0 ; // initial value
			for( j = 0 ; j < zb->ny ; j++ )
			{
				k = j*zb->nx+i ;
				c = zb->pixels[k] ;
				if( p == 0 && c == 255 )
				{
					if( j >= 2 )
						zb->pixels[(j-2)*zb->nx+i] = (uchar)(255*grays[3]/100) ;
					if( j >= 1 )
						zb->pixels[(j-1)*zb->nx+i] = (uchar)(255*grays[2]/100) ;
					zb->pixels[k] = (uchar)(255*grays[1]/100) ;
					if( j+1 < zb->ny )
						zb->pixels[(j+1)*zb->nx+i] = (uchar)(255*grays[0]/100) ;
				}
				else
				if( p == 255 && c == 0 )
				{
					if( j >= 2 )
						zb->pixels[(j-2)*zb->nx+i] = (uchar)(255*grays[0]/100) ;
					if( j >= 1 )
						zb->pixels[(j-1)*zb->nx+i] = (uchar)(255*grays[1]/100) ;
					zb->pixels[k] = (uchar)(255*grays[2]/100) ;
					if( j+1 < zb->ny )
						zb->pixels[(j+1)*zb->nx+i] = (uchar)(255*grays[3]/100) ;
				}
				p = c ;
				k++ ;
			}
		}
	}

	return 1 ;
}

uchar zbGetPixel(ZB* zb, int i, int j)
{
	//return zb->pixels[zb->ny*j+i] ;
	return zb->pixels[zb->nx*j+i] ; // nt add 2021/6/4
}
////////////////////////////////////////////////////////////////