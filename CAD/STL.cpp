#include "StdAfx.h"

////////////////////////////////////////////////////////////////
void stltriInit(STLTRI* pT)
{
	int k ;

	if( pT )
	{
		for( k = 0 ; k < 3 ; k++ )
		{
			pT->normal[k] = 0. ;
			pT->vs[0][k]  = 0. ;
			pT->vs[1][k]  = 0. ;
			pT->vs[2][k]  = 0. ;
		}
		pT->attrib = 0 ; // nt add 2017/3/17
	}

	return ;
}

// nt add 2017/3/26
void stltriGetBox2D(STLTRI* pT, BOX2D* box)
{
	int k ;

	for( k = 0 ; k < 2 ; k++ )
	{
		if( pT->vs[0][k] < pT->vs[1][k] ) // x0 < x1
		{
			if( pT->vs[0][k] < pT->vs[2][k] ) // x0 < x2
			{
				box->min[k] = pT->vs[0][k] ;
				box->max[k] = mathMAX(pT->vs[1][k], pT->vs[2][k]) ;
			}
			else // x2 < x0 < x1
			{
				box->min[k] = pT->vs[2][k] ;
				box->max[k] = pT->vs[1][k] ;
			}
		}
		else // x1 < x0
		{
			if( pT->vs[1][k] < pT->vs[2][k] ) // x1 < x2
			{
				box->min[k] = pT->vs[1][k] ;
				box->max[k] = mathMAX(pT->vs[0][k], pT->vs[2][k]) ;
			}
			else // x0 > x1 > x2
			{
				box->min[k] = pT->vs[2][k] ;
				box->max[k] = pT->vs[0][k] ;
			}
		}
	}

	return ;
}

// nt add 2017/3/26
void stltriGetBox(STLTRI* pT, BOX3D* box)
{
	int k ;

	for( k = 0 ; k < 3 ; k++ )
	{
		if( pT->vs[0][k] < pT->vs[1][k] ) // x0 < x1
		{
			if( pT->vs[0][k] < pT->vs[2][k] ) // x0 < x2
			{
				box->min[k] = pT->vs[0][k] ;
				box->max[k] = mathMAX(pT->vs[1][k], pT->vs[2][k]) ;
			}
			else // x2 < x0 < x1
			{
				box->min[k] = pT->vs[2][k] ;
				box->max[k] = pT->vs[1][k] ;
			}
		}
		else // x1 < x0
		{
			if( pT->vs[1][k] < pT->vs[2][k] ) // x1 < x2
			{
				box->min[k] = pT->vs[1][k] ;
				box->max[k] = mathMAX(pT->vs[0][k], pT->vs[2][k]) ;
			}
			else // x0 > x1 > x2
			{
				box->min[k] = pT->vs[2][k] ;
				box->max[k] = pT->vs[0][k] ;
			}
		}
	}

	return ;
}

/* nt add 2016/12/28, 根据三角形重心坐标得到一个点
 */
void stltriGetPt(STLTRI* pT, double gc[3], PNT3D p)
{
	p[0] = gc[0]*pT->vs[0][0]+gc[1]*pT->vs[1][0]+gc[2]*pT->vs[2][0] ;
	p[1] = gc[0]*pT->vs[0][1]+gc[1]*pT->vs[1][1]+gc[2]*pT->vs[2][1] ;
	p[2] = gc[0]*pT->vs[0][2]+gc[1]*pT->vs[1][2]+gc[2]*pT->vs[2][2] ;

	return ;
}

/*	2016/12/27 nt 校正法矢使法矢模长为1或0
 */
int stltriCorrNormal(STLTRI* pT, double tol)
{
	VEC3D n ;

	if( pT )
	{
		if( mathGetTriNormal(pT->vs[0], 
				             pT->vs[1], 
				             pT->vs[2],
				             tol, 
				             n) == ERSUCSS )
		{
			if( pT->normal[0]*n[0]+
				pT->normal[1]*n[1]+
				pT->normal[2]*n[2] > 0. )
				memcpy(pT->normal, n, sizeof(VEC3D)) ;
			else
			{
				pT->normal[0] = -n[0] ;
				pT->normal[1] = -n[1] ;
				pT->normal[2] = -n[2] ;
			}
		}
		else
		{
			pT->normal[0] = 0. ;
			pT->normal[1] = 0. ;
			pT->normal[2] = 0. ;
		}

		return 1 ;
	}
	else
		return 0 ;
}

//	2008/6/5 read STL file
int stltriLoadBinary(STLTRI* pT, FILE* fp)
{
	if( pT &&
		fp )
	{
		int i = 0, k = 0, n ;
		char buf[2] ;
		float p[3]/*, d*/ ;

		pT->attrib = 0 ; // nt add 2017/3/16
		n = (int)fread_s(p, sizeof(float)*3, sizeof(float), 3, fp) ;
		//d = sqrtf(fabsf(p[0]*p[0]+p[1]*p[1]+p[2]*p[2])) ;
		if( n != 3 )
			return 0 ;
		//if( (d > 0.000001f && d < 0.999999f) ||
		//	d > 1.000001f )
		//	return 0 ;
		for( k = 0 ; k < 3 ; k++ )
			pT->normal[k] = (double)p[k] ;
		for( i = 0 ; i < 3 ; i++ )
		{
			n = (int)fread_s(p, sizeof(float)*3, sizeof(float), 3, fp) ;
			if( n != 3 )
				return 0 ;
			for( k = 0 ; k < 3 ; k++ )
				pT->vs[i][k] = (double)p[k] ;
		}
		n = (int)fread_s(buf, sizeof(char)*2, sizeof(char), 2, fp) ;
		if( n != 2 )
			return 0 ;
		// nt modify 2017/3/20 都要校正法矢
		//if( d > 1.000001f || // 法矢模长不等于1
		//	d < 0.999999f )
			stltriCorrNormal(pT, MIN_DBL) ;

		return 1 ;
	}
	else
		return 0 ;
}

int stltriLoadASCII(STLTRI* pT, FILE* fp)
{
	if( pT &&
		fp )
	{
		int i = 0, k = 0 ;
		char buf[128], *str ;
		//double d ;

		pT->attrib = 0 ; // nt add 2017/3/16
		while(1)
		{
			if( fgets(buf, 128, fp) == NULL )
				return 2 ; // end of the file
			if( (str = strstr(buf, "normal")) != NULL )
			{
				if( sscanf_s(str+6, 
						     "%lf %lf %lf",
						     &(pT->normal[0]),
						     &(pT->normal[1]),
						     &(pT->normal[2])) != 3 )
					return 0 ;
				k++ ;
			}
			else
			if( (str = strstr(buf, "vertex")) != NULL )
			{
				if( sscanf_s(str+6, 
						     "%lf %lf %lf",
						     &(pT->vs[i][0]),
						     &(pT->vs[i][1]),
						     &(pT->vs[i][2])) != 3 )
					return 0 ;
				i++ ;
			}
			else
			if( (str = strstr(buf, "endfacet")) != NULL )
				break ;
			else
			if( (str = strstr(buf, "solid")) != NULL ||
				(str = strstr(buf, "loop")) != NULL )
				;
			else
				break ;
		}
		if( k == 1 &&
			i == 3 )
		{
			// nt modify 2017/3/20 都要校正法矢
			//d = sqrt(fabs(pT->normal[0]*pT->normal[0]+
			//	          pT->normal[1]*pT->normal[1]+
			//	          pT->normal[2]*pT->normal[2])) ;
			//if( d > 1.000001 || // 法矢模长不等于1
			//	d < 0.999999 )
				stltriCorrNormal(pT, MIN_DBL) ;
			return 1 ;
		}
		else
			return 0 ;
	}
	else
		return 0 ;
}

// nt add 2017/7/13
// 用k指定顶点的序号, k=0,1,2
// 把三角形的序号为k的角投影到XY平面，计算其角度
double stltriCalAngleXY(STLTRI* pT, int k)
{
	double a ;
	VEC2D v1, v2 ;

	v1[0] = pT->vs[(k+1)%3][0]-pT->vs[k][0] ;
	v1[1] = pT->vs[(k+1)%3][1]-pT->vs[k][1] ;
	v2[0] = pT->vs[(k+2)%3][0]-pT->vs[k][0] ;
	v2[1] = pT->vs[(k+2)%3][1]-pT->vs[k][1] ;
	if( mathUniVec2D(v1, MIN_DBL) == ERUNSUC ||
		mathUniVec2D(v2, MIN_DBL) == ERUNSUC )
		return 0. ;
	a = mathGetAngle2DUnit(v1, v2) ;

	return a ;
}

// nt add 2017/9/1
int stltriIsDege(STLTRI* pT, double tol)
{
	int k ;
	double d ;

	for( k = 0 ; k < 3 ; k++ )
	{
		d = mathDistPntLinEx(pT->vs[k], pT->vs[(k+1)%3], pT->vs[(k+2)%3]) ;
		if( d < tol )
			return 1 ; // dege
	}

	return 0 ;
}
//--------------------------------------------------------------
int stlblkAllocTris(STLBLK* pB, int maxNumOfTri)
{
	if( pB &&
		pB->tris == NULL &&
		maxNumOfTri > 0 )
	{
		pB->tris = new STLTRI[maxNumOfTri] ;
		return 1 ;
	}
	else
		return 0 ;
}

int stlblkFreeTris(STLBLK* pB)
{
	if( pB &&
		pB->tris )
	{
		delete [](pB->tris) ;
		return 1 ;
	}
	else
		return 0 ;
}

int stlblkCalBox(STLBLK* pB)
{
	int i, k ;
	STLTRI* pT ;

	if( pB )
	{
		mathClnBox(&(pB->box)) ;
		for( i = 0 ; i < pB->nTri ; i++ )
		{
			pT = &(pB->tris[i]) ;
			if( pT )
			{
				for( k = 0 ; k < 3 ; k++ )
				{
					mathEnlargeBox(pT->vs[k], &(pB->box)) ;
				}
			}
		}

		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2017/9/1
int stlblkSerialize(STLBLK* pB, CArchive& ar)
{
	if( pB == NULL )
		return 0 ;

	if( ar.IsStoring() )
	{
		ar << pB->nTri ;
		if( pB->nTri > 0 )
			ar.Write(pB->tris, sizeof(STLTRI)*pB->nTri) ;
		ar.Write(&(pB->box), sizeof(BOX3D)) ;
	}
	else
	{
		ar >> pB->nTri ;
		if( pB->nTri > 0 )
		{
			pB->tris = new STLTRI[pB->nTri] ;
			ar.Read(pB->tris, sizeof(STLTRI)*pB->nTri) ;
		}
		else
			pB->tris = NULL ;
		ar.Read(&(pB->box), sizeof(BOX3D)) ;
	}

	return 1 ;
}
//--------------------------------------------------------------
STL* stlCreate(int maxNumOfBlk, int maxNumOfTri)
{
	if( maxNumOfBlk < 1 ||
		maxNumOfTri < 1 )
		return NULL ;

	STL* pSTL = new STL() ;
	pSTL->next = NULL ; // nt add 2016/12/23
	pSTL->maxNumOfBlk = maxNumOfBlk ;
	pSTL->maxNumOfTri = maxNumOfTri ;
	pSTL->nBlk = 0 ;
	pSTL->blks = new STLBLK[maxNumOfBlk] ;
	for( int i = 0 ; i < maxNumOfBlk ; i++ )
	{
		pSTL->blks[i].nTri = 0 ;
		pSTL->blks[i].tris = NULL ;
		mathClnBox(&(pSTL->blks[i].box)) ;
	}
	mathClnBox(&(pSTL->box)) ;
	pSTL->stt = stlNormal ;
	pSTL->p[0] = 0. ;
	pSTL->p[0] = 0. ;
	pSTL->p[0] = 0. ;
	pSTL->vol = 0. ; // nt add 2017/5/14
	stltriInit(&(pSTL->tri)) ;
	pSTL->zb = NULL ; // nt add 2017/5/26
	pSTL->ss = NULL ; // nt add 2017/5/26
	pSTL->zb2 = NULL ; // nt add 2017/5/26
	pSTL->vs = NULL ; // nt add 2017/7/9

	// nt add 2021/12/26
	pSTL->cb = NULL ;
	pSTL->zb3 = NULL ;

	return pSTL ;
}

// nt add 2017/9/1
STL* stlCreate2()
{
	return new STL() ;
}

int stlFree(STL* pSTL)
{
	if( pSTL )
	{
		for( int i = 0 ; i < pSTL->nBlk ; i++ )
		{
			stlblkFreeTris(&(pSTL->blks[i])) ;
		}
		delete [](pSTL->blks) ;
		if( pSTL->zb ) // nt add 2017/5/26
			zbFree((ZB*)(pSTL->zb)) ;
		if( pSTL->ss ) // nt add 2017/5/26
			ssFree((SS*)(pSTL->ss)) ;
		if( pSTL->zb2 ) // nt add 2017/8/22 原来注释掉了
			zb2Free((ZB2*)(pSTL->zb2)) ;
		// nt add 2017/7/9
		if( pSTL->vs )
		{
			VER* u, *v = pSTL->vs ;
			while(v)
			{
				u = v->next ;
				verFree(v) ;
				v = u ;
			}
		}

		// nt add 2021/12/26
		if( pSTL->cb )
			cbFree((CB*)(pSTL->cb)) ;
		if( pSTL->zb3 )
			zb2Free((ZB2*)(pSTL->zb3)) ;

		delete pSTL ;
		return 1 ;
	}
	else
		return 0 ;
}

int stlReleaseSS(STL* pSTL)
{
	if( pSTL )
	{
		if( pSTL->zb )
		{
			zbFree((ZB*)(pSTL->zb)) ;
			pSTL->zb = NULL ;
		}
		if( pSTL->ss )
		{
			ssFree((SS*)(pSTL->ss)) ;
			pSTL->ss = NULL ;
		}
		if( pSTL->zb2 )
		{
			zb2Free((ZB2*)(pSTL->zb2)) ;
			pSTL->zb2 = NULL ;
		}
		return 1 ;
	}
	else
		return 0 ;
}
//-------------------------------------------------------------
STL* stlCopy(STL* pSTL)
{
	int i, nTri ;
	STL* pCopy ;
	
	if( pSTL == NULL )
		return NULL ;
	pCopy = new STL() ;
	pCopy->next = NULL ;
	pCopy->maxNumOfBlk = pSTL->nBlk ;
	pCopy->maxNumOfTri = pSTL->nBlk>0?pSTL->blks[0].nTri:100 ;
	pCopy->nBlk = pSTL->nBlk ;
	pCopy->blks = new STLBLK[pSTL->nBlk] ;
	for( i = 0 ; i < pSTL->nBlk ; i++ )
	{
		nTri = pSTL->blks[i].nTri ;
		pCopy->blks[i].nTri = nTri ;
		pCopy->blks[i].tris = new STLTRI[nTri] ;
		memcpy(pCopy->blks[i].tris, 
			  pSTL->blks[i].tris, 
			  sizeof(STLTRI)*nTri) ;
		pCopy->blks[i].box = pSTL->blks[i].box ;
	}
	pCopy->box = pSTL->box ;
	pCopy->stt = stlNormal ;
	pCopy->p[0] = 0. ;
	pCopy->p[0] = 0. ;
	pCopy->p[0] = 0. ;
	pCopy->vol = pSTL->vol ; // nt add 2017/5/14
	stltriInit(&(pCopy->tri)) ;
	pCopy->zb = NULL ; // nt add 2017/5/26
	pCopy->ss = NULL ; // nt add 2017/5/26
	pCopy->zb2 = NULL ; // nt add 2017/5/26
	pCopy->vs = NULL ; // nt add 2017/7/9

	// nt add 2021/12/26
	pCopy->cb = NULL ;
	pCopy->zb3 = NULL ;

	return pCopy ;
}

int stlEmpty(STL* pSTL)
{
	int i ;
	STLBLK* pB ;

	if( pSTL )
	{
		for( i = 0 ; i < pSTL->nBlk ; i++ )
		{
			pB = &(pSTL->blks[i]) ;
			if( pB )
			{
				mathClnBox(&(pB->box)) ;
				pB->nTri = 0 ;
				if( pB->tris ) // nt add 2017/1/3
				{
					delete []pB->tris ;
					pB->tris = NULL ;
				}
			}
		}
		mathClnBox(&(pSTL->box)) ;
		pSTL->nBlk = 0 ;

		return 1 ;
	}
	else
		return 0 ;
}

int stlGetNumOfTri(STL* pSTL)
{
	if( pSTL )
	{
		if( pSTL->nBlk < 1 )
			return 0 ;
		else
			return pSTL->maxNumOfTri*(pSTL->nBlk-1)+
			       pSTL->blks[pSTL->nBlk-1].nTri ;
	}
	else
		return 0 ;
}

int stlGetTri(STL* pSTL, int i, STLTRI* pT)
{
	if( pSTL &&
		i >= 0 &&
		pT )
	{
		int nBlk = (i+1)/pSTL->maxNumOfTri ; // 占满nBlk个块
		int nTri = (i+1)%pSTL->maxNumOfTri ; // 占满nBlk个块后还剩余nTri个三角形
		if( nTri > 0 ) // 还要多占一块的一部分
			nBlk++ ; // 总共nBlk个块
		else
			nTri = pSTL->maxNumOfTri ;
		if( nBlk > pSTL->nBlk )
				return 0 ;
		*pT = pSTL->blks[nBlk-1].tris[nTri-1] ;
		return 1 ;
	}
	else
		return 0 ;
}

// return 0: error
int stlAddTri(STL* pSTL, STLTRI* pT)
{
	if( pSTL &&
		pT )
	{
		if( pSTL->nBlk < 1 || // 若STL为空, 先分配空间
			pSTL->blks[pSTL->nBlk-1].nTri >= pSTL->maxNumOfTri )
		{
			if( pSTL->nBlk >= pSTL->maxNumOfBlk )
				return 0 ;
			stlblkAllocTris(&(pSTL->blks[pSTL->nBlk]), pSTL->maxNumOfTri) ;
			(pSTL->nBlk)++ ;
		}

		STLBLK* pB = &(pSTL->blks[pSTL->nBlk-1]) ; // 取最后一个块
		pB->tris[pB->nTri] = *pT ;
		(pB->nTri)++ ;
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2017/5/26
void* stlGetZB(STL* pSTL)
{
	return pSTL->zb ;
}

// nt add 2017/5/26
void* stlGetSS(STL* pSTL)
{
	return pSTL->ss ;
}

// nt add 2017/5/26
void* stlGetZB2(STL* pSTL)
{
	return pSTL->zb2 ;
}

// 平移
int stlMove(STL* pSTL, double v[3])
{
	int i, j, k ;
	STLBLK* pB ;

	if( pSTL )
	{
		for( i = 0 ; i < pSTL->nBlk ; i++ )
		{
			pB = &(pSTL->blks[i]) ;
			if( pB )
			{
				for( j = 0 ; j < pB->nTri ; j++ )
				{
					for( k = 0 ; k < 3 ; k++ )
					{
						pB->tris[j].vs[k][0] += v[0] ;
						pB->tris[j].vs[k][1] += v[1] ;
						pB->tris[j].vs[k][2] += v[2] ;
					}
				}
				// calculate box, nt add 2016/12/31
				mathMoveBox(&(pB->box), v) ;
			}
		}
		// calculate box, nt add 2016/12/31
		mathMoveBox(&(pSTL->box), v) ;

		// nt add 2017/7/3
		if( pSTL->zb )
		{
			zbFree((ZB*)pSTL->zb) ;
			pSTL->zb = NULL ;
		}
		if( pSTL->ss )
			ssMove((SS*)pSTL->ss, v) ;
		if( pSTL->zb2 )
		{
			zb2Free((ZB2*)pSTL->zb2) ;
			pSTL->zb2 = NULL ;
		}
		// end

		return 1 ;
	}
	else
		return 0 ;
}

// 旋转
int stlRotate(STL* pSTL, double pivot[3], double axis[3], double a)
{
	int i, j, k ;
	PNT3D p ;
	VEC3D normal ;
	STLBLK* pB ;

	if( pSTL )
	{
		for( i = 0 ; i < pSTL->nBlk ; i++ )
		{
			pB = &(pSTL->blks[i]) ;
			if( pB )
			{
				for( j = 0 ; j < pB->nTri ; j++ )
				{
					mathRotVec(axis, pivot, a, pB->tris[j].normal, normal) ;
					memcpy(pB->tris[j].normal, normal, sizeof(double[3])) ;
					for( k = 0 ; k < 3 ; k++ )
					{
						mathRotPnt(axis, pivot, a, pB->tris[j].vs[k], p) ;
						memcpy(pB->tris[j].vs[k], p, sizeof(double[3])) ;
					}
				}
				// calculate box, nt add 2016/12/31
				mathRotateBox(&(pB->box), pivot, axis, a) ;
			}
		}
		// calculate box, nt add 2016/12/31
		mathRotateBox(&(pSTL->box), pivot, axis, a) ;

		// nt add 2017/7/3
		if( pSTL->zb )
		{
			zbFree((ZB*)pSTL->zb) ;
			pSTL->zb = NULL ;
		}
		if( pSTL->ss )
		{
			if( fabs(axis[0]) < MIN_ANG &&
				fabs(axis[1]) < MIN_ANG )
				ssRotate((SS*)pSTL->ss, pivot, axis, a) ;
			else
			{
				ssFree((SS*)pSTL->ss) ;
				pSTL->ss = NULL ;
			}
		}
		if( pSTL->zb2 )
		{
			zb2Free((ZB2*)pSTL->zb2) ;
			pSTL->zb2 = NULL ;
		}
		// end

		return 1 ;
	}
	else
		return 0 ;
}

int stlRotate2(STL* pSTL, double pivot[3], double axis[3], double a)
{
	int i, j, k ;
	PNT3D p ;
	VEC3D normal ;
	STLBLK* pB ;

	if( pSTL )
	{
		for( i = 0 ; i < pSTL->nBlk ; i++ )
		{
			pB = &(pSTL->blks[i]) ;
			if( pB )
			{
				for( j = 0 ; j < pB->nTri ; j++ )
				{
					mathRotVec(axis, pivot, a, pB->tris[j].normal, normal) ;
					memcpy(pB->tris[j].normal, normal, sizeof(double[3])) ;
					for( k = 0 ; k < 3 ; k++ )
					{
						mathRotPnt(axis, pivot, a, pB->tris[j].vs[k], p) ;
						memcpy(pB->tris[j].vs[k], p, sizeof(double[3])) ;
					}
				}
				// calculate box, nt add 2016/12/31
				//mathRotateBox(&(pB->box), pivot, axis, a) ;
			}
		}
		// calculate box, nt add 2016/12/31
		//mathRotateBox(&(pSTL->box), pivot, axis, a) ;

		// nt add 2017/7/3
		if( pSTL->zb )
		{
			zbFree((ZB*)pSTL->zb) ;
			pSTL->zb = NULL ;
		}
		if( pSTL->ss )
		{
			if( fabs(axis[0]) < MIN_ANG &&
				fabs(axis[1]) < MIN_ANG )
				ssRotate((SS*)pSTL->ss, pivot, axis, a) ;
			else
			{
				ssFree((SS*)pSTL->ss) ;
				pSTL->ss = NULL ;
			}
		}
		if( pSTL->zb2 )
		{
			zb2Free((ZB2*)pSTL->zb2) ;
			pSTL->zb2 = NULL ;
		}
		// end

		return 1 ;
	}
	else
		return 0 ;
}

// 比例缩放
// nt modify 2016/12/31 : 以center为中心进行缩放
int stlScale(STL* pSTL, double scale)
{
	int i, j, k ;
	STLBLK* pB ;

	if( pSTL )
	{
		for( i = 0 ; i < pSTL->nBlk ; i++ )
		{
			pB = &(pSTL->blks[i]) ;
			if( pB )
			{
				for( j = 0 ; j < pB->nTri ; j++ )
				{
					for( k = 0 ; k < 3 ; k++ )
					{
						pB->tris[j].vs[k][0] *= scale ;
						pB->tris[j].vs[k][1] *= scale ;
						pB->tris[j].vs[k][2] *= scale ;
					}
				}
				// calculate box, nt add 2016/12/31
				mathScaleBox(&(pB->box), scale) ;
			}
		}
		// calculate box, nt add 2016/12/31
		mathScaleBox(&(pSTL->box), scale) ;

		// nt add 2017/7/3
		if( pSTL->zb )
		{
			zbFree((ZB*)pSTL->zb) ;
			pSTL->zb = NULL ;
		}
		if( pSTL->ss )
		{
			ssFree((SS*)pSTL->ss) ;
			pSTL->ss = NULL ;
		}
		if( pSTL->zb2 )
		{
			zb2Free((ZB2*)pSTL->zb2) ;
			pSTL->zb2 = NULL ;
		}
		// end

		return 1 ;
	}
	else
		return 0 ;
}

int stlScale2(STL* pSTL, PNT3D center, double scale)
{
	int i, j, k ;
	double d ;
	STLBLK* pB ;

	if( pSTL )
	{
		for( i = 0 ; i < pSTL->nBlk ; i++ )
		{
			pB = &(pSTL->blks[i]) ;
			if( pB )
			{
				for( j = 0 ; j < pB->nTri ; j++ )
				{
					for( k = 0 ; k < 3 ; k++ )
					{
						d = pB->tris[j].vs[k][0]-center[0] ;
						pB->tris[j].vs[k][0] = center[0]+d*scale ;
						d = pB->tris[j].vs[k][1]-center[1] ;
						pB->tris[j].vs[k][1] = center[1]+d*scale ;
						d = pB->tris[j].vs[k][2]-center[2] ;
						pB->tris[j].vs[k][2] = center[2]+d*scale ;
					}
				}
				// calculate box, nt add 2016/12/31
				mathScaleBox2(&(pB->box), center, scale) ;
			}
		}
		// calculate box, nt add 2016/12/31
		mathScaleBox2(&(pSTL->box), center, scale) ;

		// nt add 2017/7/3
		if( pSTL->zb )
		{
			zbFree((ZB*)pSTL->zb) ;
			pSTL->zb = NULL ;
		}
		if( pSTL->ss )
		{
			ssFree((SS*)pSTL->ss) ;
			pSTL->ss = NULL ;
		}
		if( pSTL->zb2 )
		{
			zb2Free((ZB2*)pSTL->zb2) ;
			pSTL->zb2 = NULL ;
		}
		// end

		// nt add 2017/8/23
		pSTL->vol *= (scale*scale*scale) ;

		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2016/12/22
int stlCalBox(STL* pSTL)
{
	int i ;
	STLBLK* pB ;

	mathClnBox(&(pSTL->box)) ;
	if( pSTL )
	{
		for( i = 0 ; i < pSTL->nBlk ; i++ )
		{
			pB = &(pSTL->blks[i]) ;
			if( pB )
			{
				stlblkCalBox(pB) ;
				mathCalBoxUnion(&(pB->box), &(pSTL->box), &(pSTL->box)) ;
			}
		}

		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2017/9/30
int stlCalBox2(STL* pSTL, BOX3D* box)
{
	if( pSTL == NULL ||
		box == NULL )
		return 0 ;
	
	stlCalBox(pSTL) ;
	*box = pSTL->box ;
	if( pSTL->ss )
	{
		SS* ss = (SS*)(pSTL->ss) ;
		ssCalBox(ss) ;
		mathCalBoxUnion(box, &(ss->box), box) ;
	}

	return 1 ;
}

// nt add 2017/10/1 1=collision, 0=collision free
// 未处理一个模型完全在另一个模型内部的情况
int stlCollision(STL* pSTL1, STL* pSTL2)
{
	int i, j, s, t ;
	double tol = 1.e-3 ;
	STLBLK* pB1, *pB2 ;
	STLTRI* pT1, *pT2 ;

	if( pSTL1 == NULL ||
		pSTL2 == NULL )
		return 0 ;

	if( mathChkBoxInt(&(pSTL1->box), &(pSTL2->box), tol) == IDINT )
	{
		for( i = 0 ; i < pSTL1->nBlk ; i++ )
		{
			pB1 = &(pSTL1->blks[i]) ;
			for( j = 0 ; j < pB1->nTri ; j++ )
			{
				pT1 = &(pB1->tris[j]) ;
				if( mathChkBoxPnt(&(pSTL2->box), pT1->vs[0], tol) == IDIN ||
					mathChkBoxPnt(&(pSTL2->box), pT1->vs[1], tol) == IDIN ||
					mathChkBoxPnt(&(pSTL2->box), pT1->vs[2], tol) == IDIN )
				{
					for( s = 0 ; s < pSTL2->nBlk ; s++ )
					{
						pB2 = &(pSTL2->blks[s]) ;
						for( t = 0 ; t < pB2->nTri ; t++ )
						{
							pT2 = &(pB2->tris[t]) ;
							if( triInt(pT1->vs[0],
									   pT1->vs[1],
									   pT1->vs[2],
									   pT2->vs[0],
									   pT2->vs[1],
									   pT2->vs[2]) )
								return 1 ;
						}
					}
				}
			}
		}
	}

	return 0 ;
}

// nt add 2016/12/31
int stlGetBox(STL* pSTL, BOX3D* box)
{
	if( pSTL &&
		box )
	{
		*box = pSTL->box ;
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2016/12/31
int stlGetCenter(STL* pSTL, PNT3D center)
{
	if( pSTL &&
		center )
	{
		center[0] = 0.5*(pSTL->box.min[0]+pSTL->box.max[0]) ;
		center[1] = 0.5*(pSTL->box.min[1]+pSTL->box.max[1]) ;
		center[2] = 0.5*(pSTL->box.min[2]+pSTL->box.max[2]) ;

		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2017/3/13
double stlCalVol(STL* pSTL)
{
	int i, j ;
	double vol = 0., h, area ;
	STLBLK* pB ;
	STLTRI* pT ;

	if( pSTL )
	{
		for( i = 0 ; i < pSTL->nBlk ; i++ )
		{
			pB = &(pSTL->blks[i]) ;
			for( j = 0 ; j < pB->nTri ; j++ )
			{
				pT = &(pB->tris[j]) ;
				h = (pT->vs[0][2]+pT->vs[0][2]+pT->vs[0][2])/3 ;
				area = (pT->vs[1][0]-pT->vs[0][0])*(pT->vs[0][1]+pT->vs[1][1])+
					   (pT->vs[2][0]-pT->vs[1][0])*(pT->vs[1][1]+pT->vs[2][1])+
					   (pT->vs[0][0]-pT->vs[2][0])*(pT->vs[2][1]+pT->vs[0][1]) ;
				if( pT->normal[2] > 0. )
					vol += fabs(area)*h ;
				else
					vol -= fabs(area)*h ;
			}
		}
		pSTL->vol = 0.5*fabs(vol) ; // nt add 2017/5/14

		return pSTL->vol ; //0.5*fabs(vol) ;
	}
	else
		return 0. ;
}
//-------------------------------------------------------------
STLTRI* stlPick(STL* pSTL, 
			   PNT3D pos, 
	           VEC3D dir,
	           double r, 
	           PNT3D p, // 输入输出参数
	           double* pz) // 输入输出参数
{
	int i, j, k ;
	double gc[3], z ;
	PNT2D ps[3], ori={0.,0.} ;
	PNT3D q ;
	VEC3D X, Y, v ;
	STLBLK* pB ;
	STLTRI* pT, *pM = NULL ;

	if( pSTL )
	{
		if( mathChkBoxLinInt(&(pSTL->box), pos, dir, r) == IDINT )
		{
			mathGetXYFromZ(dir, X, Y) ;
			for( i = 0 ; i < pSTL->nBlk ; i++ )
			{
				pB = &(pSTL->blks[i]) ;
				if( pB )
				{
					if( mathChkBoxLinInt(&(pB->box), pos, dir, r) == IDINT )
					{
						for( j = 0 ; j < pB->nTri ; j++ )
						{
							pT = &(pB->tris[j]) ;
							for( k = 0 ; k < 3 ; k++ )
							{
								v[0] = pT->vs[k][0]-pos[0] ;
								v[1] = pT->vs[k][1]-pos[1] ;
								v[2] = pT->vs[k][2]-pos[2] ;
								ps[k][0] = v[0]*X[0]+v[1]*X[1]+v[2]*X[2] ;
								ps[k][1] = v[0]*Y[0]+v[1]*Y[1]+v[2]*Y[2] ;
							}
							if( mathChkOriInTri2D(ps[0], ps[1], ps[2], r) == IDIN )
							{
								mathCalTri2DGCP(ps[0], ps[1], ps[2], ori, gc) ;
								stltriGetPt(pT, gc, q) ;
								v[0] = q[0]-pos[0] ;
								v[1] = q[1]-pos[1] ;
								v[2] = q[2]-pos[2] ;
								z = v[0]*dir[0]+v[1]*dir[1]+v[2]*dir[2] ;
								if( *pz > z )
								{
									memcpy(p, q, sizeof(PNT3D)) ;
									pM = pT ;
									*pz = z ;
								}
							}
						}
					}
				}
			}
		}

		return pM ;
	}
	else
		return NULL ;
}
//-------------------------------------------------------------
int stlDraw(STL* pSTL, void* pVI)
{
	MATERIAL m_material ;
	STLBLK* pB ;
	STLTRI* pT ;

	if( pSTL &&
		pVI )
	{
		if( pSTL->stt == stlPrompting )
			materialGet(&m_material, "Prompt") ;
		else
		if( pSTL->stt == stlSelected )
			materialGet(&m_material, "Select") ;
		else
			//materialGet(&m_material, "PolishedBronze") ;
			materialGet(&m_material, "Sky2") ;
		materialSet(&m_material) ;
		
		glBegin(GL_TRIANGLES) ;
		for( int i = 0 ; i < pSTL->nBlk ; i++ )
		{
			pB = &(pSTL->blks[i]) ;
			for( int j = 0 ; j < pB->nTri ; j++ )
			{
				pT = &(pB->tris[j]) ;
				glNormal3dv(pT->normal) ;
				glVertex3dv(pT->vs[0]) ;
				glNormal3dv(pT->normal) ;
				glVertex3dv(pT->vs[1]) ;
				glNormal3dv(pT->normal) ;
				glVertex3dv(pT->vs[2]) ;
			}
		}
		glEnd() ;

		// nt add 2017/7/9
		if( pSTL->vs )
		{
			PNT3D ori = {0., 0., 0.} ;
			FACET* facet = facetSphere(ori, 0.1, 8, 8) ;
			if( facet == NULL )
				return 0 ;
			VER* v ;
			for( v = pSTL->vs ; v ; v = v->next )
			{
				glPushMatrix() ;
				glTranslated(v->p[0], v->p[1], v->p[2]) ;
				glVertexPointer(3, GL_DOUBLE, sizeof(FNODE), facet->nodes[0].p) ;
				glNormalPointer(GL_DOUBLE, sizeof(FNODE), facet->nodes[0].normal) ;
				glDrawElements(GL_TRIANGLES, facet->ntria*3, GL_UNSIGNED_INT, facet->trias) ;
				glPopMatrix() ;
			}
			facetFree(facet) ;
		}
		// end

		return 1 ;
	}
	else
		return 0 ;
}

int stlDraw2(STL* pSTL, void* pVI)
{
	STLBLK* pB ;
	STLTRI* pT ;

	if( pSTL &&
		pVI )
	{
		for( int i = 0 ; i < pSTL->nBlk ; i++ )
		{
			pB = &(pSTL->blks[i]) ;
			for( int j = 0 ; j < pB->nTri ; j++ )
			{
				pT = &(pB->tris[j]) ;
				glBegin(GL_LINE_LOOP) ;
					glVertex3dv(pT->vs[0]) ;
					glVertex3dv(pT->vs[1]) ;
					glVertex3dv(pT->vs[2]) ;
				glEnd() ;
			}
		}

		return 1 ;
	}
	else
		return 0 ;
}
//-------------------------------------------------------------
int stlLoadBinary(STL* pSTL, TCHAR* pathName)
{
	if( pSTL &&
		pathName )
	{
		FILE* fp ;
		_tfopen_s(&fp, pathName, _T("r+b")) ;
		if( fp )
		{
			int i, n = 0, rc ;
			char buf[82] ;
			STLTRI tri ;

			rc = (int)fread_s(buf, 80, sizeof(char), 80, fp) ;
			if( rc != 80 )
			{
				fclose(fp) ;
				return 0 ;
			}
			rc = (int)fread_s(&n, 4, sizeof(char), 4, fp) ;
			if( rc != 4 ||
				n < 1 ||
				n > 100000000 ) // nt add 2017/4/2
			{
				fclose(fp) ;
				return 0 ;
			}
			for( i = 0 ; i < n ; i++ )
			{
				rc = stltriLoadBinary(&tri, fp) ; // 循环读入STLTRI
				if( rc == 1 )
				{
					if( stlAddTri(pSTL, &tri) != 1 ) // 加入
					{
						fclose(fp) ;
						return 0 ;
					}
				}
				else
					break ;
			}
			fclose(fp) ;
			if( rc == 0 )
				return 0 ;
			else
				return 1 ;
		}
		else
			return 0 ;
	}
	else
		return 0 ;
}

// pSTL必须是用stlCreate()刚创建好的
int stlLoadASCII(STL* pSTL, TCHAR* pathName)
{
	if( pSTL &&
		pathName )
	{
		FILE* fp ;
		_tfopen_s(&fp, pathName, _T("r+t")) ;
		if( fp )
		{
			int rc = 1 ;
			STLTRI tri ;
			while(1)
			{
				rc = stltriLoadASCII(&tri, fp) ; // 循环读入STLTRI
				if( rc == 1 )
				{
					if( stlAddTri(pSTL, &tri) != 1 ) // 加入
					{
						fclose(fp) ;
						return 0 ;
					}
				}
				else
					break ;
			}
			fclose(fp) ;
			if( rc == 0 )
				return 0 ;
			else
				return 1 ;
		}
		else
			return 0 ;
	}
	else
		return 0 ;
}

int stlLoad(STL* pSTL, TCHAR* pathName)
{
	if( pSTL )
	{
		if( stlLoadBinary(pSTL, pathName) == 0 )
		{
			stlEmpty(pSTL) ; // nt add 2016/12/26
			if( stlLoadASCII(pSTL, pathName) == 0 )
				return 0 ;
		}
		stlCalBox(pSTL) ; // nt add 2016/12/26

		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2017/3/16
int stlCalAttribOfTris(STL* pSTL, double tol)
{
	int i, j, I, J, K ;
	double d ;
	VEC2D v ;
	STLBLK* pB ;
	STLTRI* pT ;

	if( pSTL )
	{
		for( i = 0 ; i < pSTL->nBlk ; i++ )
		{
			pB = &(pSTL->blks[i]) ;
			for( j = 0 ; j < pB->nTri ; j++ )
			{
				pT = &(pB->tris[j]) ;
				for( I = 0 ; I < 3 ; I++ )
				{
					J = (I+1)%3 ;
					K = (I+2)%3 ;
					v[0] = pT->vs[K][0]-pT->vs[J][0] ;
					v[1] = pT->vs[K][1]-pT->vs[J][1] ;
					d = sqrt(v[0]*v[0]+v[1]*v[1]) ;
					if( d < tol )
					{
						pT->attrib |= STLTRI_PARALLEL_Z ;
						break ;
					}
					v[0] /= d ;
					v[1] /= d ;
					d = -v[1]*(pT->vs[I][0]-pT->vs[J][0])+
						 v[0]*(pT->vs[I][1]-pT->vs[J][1]) ;
					if( fabs(d) < tol )
					{
						pT->attrib |= STLTRI_PARALLEL_Z ;
						break ;
					}
				}
			}
		}

		return 1 ;
	}
	else
		return 0 ;
}
//==============================================================
VER* verCreate(int flags, PNT3D p, VEC3D normal, double d, double a)
{
	VER* ver = (VER*)malloc(sizeof(VER)) ;
	if( ver )
	{
		ver->next = NULL ;
		ver->flags = flags ; // nt add 2017/7/11
		memcpy(ver->p, p, sizeof(PNT3D)) ;
		memcpy(ver->normal, normal, sizeof(VEC3D)) ;
		ver->d = d ;
		ver->a = a ; // nt add 2017/7/13
	}

	return ver ;
}

void verFree(VER* ver)
{
	if( ver )
		free(ver) ;

	return ;
}

VER* versMatch(VER* vers, PNT3D p, double d, double tol)
{
	VER* v = NULL ;

	for( v = vers ; v ; v = v->next )
	{
		if( fabs(v->d-d) < tol &&
			mathDist(v->p, p) < tol )
			break ;
	}

	return v ;
}
//==============================================================
// nt add 2017/7/7
// 寻找STL模型中所有向下的尖点
// nt modify 2017/7/14 加了w进行过滤
int stlCalDownwardSharpPts(STL* pSTL, double tol, double w)
{
	int i, j, k, n = 100000, I, flags ;
	double d, maxD, dd = 0., a, min ;
	PNT3D p ;
	VEC3D normal ;
	VER** vers = NULL, *vs = NULL, *u, *v, *t ;
	STLBLK* pB ;
	STLTRI* pT ;

	if( pSTL == NULL )
		return 0 ; // error
	v = pSTL->vs ; // 首先释放上一次生成的VERs
	while(v)
	{
		u = v->next ;
		verFree(v) ;
		v = u ;
	}
	pSTL->vs = NULL ;
	vers = (VER**)malloc(n*sizeof(VER*)) ;
	if( vers == NULL )
		return 0 ; // error
	for( i = 0 ; i < n ; i++ )
		vers[i] = NULL ;
	for( k = 0 ; k < 3 ; k++ )
	{
		if( fabs(pSTL->box.min[k]) > fabs(pSTL->box.max[k]) )
			d = pSTL->box.min[k] ;
		else
			d = pSTL->box.max[k] ;
		dd += d*d ;
	}
	maxD = sqrt(dd)+0.01 ;
	dd = maxD/n ; // 每一段的长度

	for( i = 0 ; i < pSTL->nBlk ; i++ )
	{
		pB = &(pSTL->blks[i]) ;
		for( j = 0 ; j < pB->nTri ; j++ )
		{
			pT = &(pB->tris[j]) ;
			if( stltriIsDege(pT, 0.001) == 1 ) // nt add 2017/11/1
				continue ;
			memcpy(normal, pT->normal, sizeof(VEC3D)) ;
			for( k = 0 ; k < 3 ; k++ )
			{
				a = stltriCalAngleXY(pT, k) ;
				memcpy(p, pT->vs[k], sizeof(PNT3D)) ;
				flags = 0 ;
				if( p[2] > pT->vs[(k+1)%3][2] || // 非向下尖点
					p[2] > pT->vs[(k+2)%3][2] )
					flags = 1 ;
				d = sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]) ;
				I = (int)(d/dd) ;
				if( I >= 0 && I < n ) // I在合理的范围
				{
					v = versMatch(vers[I], p, d, tol) ;
					if( v == NULL )
					{
						if( I < n-1 && fabs(d-(I+1)*dd) < tol )
							v = versMatch(vers[I+1], p, d, tol) ;
						else
						if( I > 0 && fabs(d-I*dd) < tol )
							v = versMatch(vers[I-1], p, d, tol) ;
					}

					if( v == NULL ) // 如果vers数组中没发现该顶点
					{
						v = vers[I] ;
						vers[I] = verCreate(flags, p, normal, d, 0.) ;
						vers[I]->next = v ;
						v = vers[I] ; // nt add 2017/7/13
					}
					else
					if( v->flags == 0 ) // 如果vers数组中有该顶点
					{
						if( flags == 1 ) // 该顶点不是向下的尖点
							v->flags = 1 ;
						else
						if( v->normal[2] > normal[2] )
							memcpy(v->normal, normal, sizeof(VEC3D)) ;
					}
					v->a += a ; // nt add 2017/7/13
				}
			}
		}
	}

	for( I = 0 ; I < n ; I++ ) // 收集vers中向下尖点
	{
		v = vers[I] ;
		while(v)
		{
			u = v->next ;
			if( v->flags == 0 && // 如果v是向下的尖点
				fabs(v->a-PI2) < 0.001 ) // ?????
			{
				v->next = pSTL->vs ;
				pSTL->vs = v ;
			}
			else // 如果v不是向下的尖点
				verFree(v) ;
			v = u ;
		}
	}
	free(vers) ; // free array of vers
	vers = NULL ;

	// nt add 2017/7/14 过滤
	u = pSTL->vs ;
	while(u)
	{
		v = u->next ;
		if( v == NULL )
			break ;
		min = MAX_DBL ;
		for( t = pSTL->vs ; t ; t = t->next )
		{
			if( t == v )
				break ;
			if( fabs(t->p[2]-v->p[2]) < 0.001 ) // ?要求z坐标相同
			{
				d = mathDist2D(t->p, v->p) ;
				if( min > d )
					min = d ;
			}
		}
		if( min < w )
		{
			u->next = v->next ;
			verFree(v) ;
		}
		else
			u = v ;
	}

	return 1 ;
}

// nt add 2017/7/14
// 计算点p到STL模型的所有向下的尖点的最近距离，该距离指XY平面上的
double stlCalDistFromSharpPts(STL* pSTL, PNT2D p)
{
	double d, min = MAX_DBL ;
	VER* v = NULL ;

	for( v = pSTL->vs ; v ; v = v->next )
	{
		d = mathDist2D(v->p, p) ;
		if( min > d )
			min = d ;
	}

	return min ;
}

// nt add 2017/8/22
int stlReleaseAuto(STL* pSTL, int base)
{
	if( pSTL )
	{
		if( pSTL->zb )
		{
			zbFree((ZB*)pSTL->zb) ;
			pSTL->zb = NULL ;
		}
		if( pSTL->ss )
			ssReleaseAuto((SS*)pSTL->ss, base) ;
		if( pSTL->zb2 )
		{
			zb2Free((ZB2*)pSTL->zb2) ;
			pSTL->zb2 = NULL ;
		}
	}

	return 1 ;
}

// nt add 2017/9/1
// nt modify 2019/7/10
int stlSerialize(STL* pSTL, CArchive& ar)
{
	int i ;

	if( pSTL == NULL )
		return 0 ;

	if( ar.IsStoring() )
	{
		//ar.Write(pSTL, sizeof(STL)) ;
		ar << pSTL->maxNumOfBlk
		   << pSTL->maxNumOfTri
		   << pSTL->nBlk
		   << pSTL->stt
		   << pSTL->p[0]
		   << pSTL->p[1]
		   << pSTL->p[2]
		   << pSTL->vol
		   << pSTL->material ; // nt add 2019/7/10
		ar.Write(&pSTL->box, sizeof(BOX3D)) ; // nt add 2019/7/10

		// nt add 2021/12/26
		if( pSTL->cb )
			ar << 1 ;
		else
			ar << 0 ;
		cbSerialize((CB*)pSTL->cb, ar) ;
	}
	else
	{
		//ar.Read(pSTL, sizeof(STL)) ;
		ar >> pSTL->maxNumOfBlk
		   >> pSTL->maxNumOfTri
		   >> pSTL->nBlk
		   >> pSTL->stt
		   >> pSTL->p[0]
		   >> pSTL->p[1]
		   >> pSTL->p[2]
		   >> pSTL->vol
		   >> pSTL->material ; // nt add 2019/7/10
		ar.Read(&pSTL->box, sizeof(BOX3D)) ; // nt add 2019/7/10
		pSTL->next = NULL ;
		if( pSTL->nBlk > 0 )
			pSTL->blks = new STLBLK[pSTL->nBlk] ;
		else
			pSTL->blks = NULL ;
		pSTL->zb = NULL ;
		pSTL->zb2 = NULL ;
		pSTL->vs = NULL ;
		//ar << pSTL->material ; // nt add 2019/7/10 nt modify 2021/12/26

		// nt add 2021/12/26
		pSTL->zb3 = NULL ;
		ar >> i ;
		if( i == 1 )
		{
			pSTL->cb = new CB() ;
			cbSerialize((CB*)pSTL->cb, ar) ;
		}
	}

	for( i = 0 ; i < pSTL->nBlk ; i++ )
		stlblkSerialize(&(pSTL->blks[i]), ar) ;

	if( pSTL->ss )
	{
		if( ar.IsStoring() == FALSE )
			pSTL->ss = ssCreate() ;
		ssSerialize((SS*)(pSTL->ss), ar) ;
	}

	return 1 ;
}
//--------------------------------------------------------------
// nt add 2021/6/4
void stlSwapXZ(STL* pSTL)
{
	int i, j, k ;
	double d ;
	STLBLK* blk = NULL ;
	STLTRI* tri = NULL ;

	if( pSTL == NULL )
		return ;

	for( i = 0 ; i < pSTL->nBlk ; i++ )
	{
		blk = &(pSTL->blks[i]) ;
		for( j = 0 ; j < blk->nTri ; j++ )
		{
			tri = &(blk->tris[j]) ;
			if( tri )
			{
				d = tri->normal[0] ;
				tri->normal[0] = tri->normal[2] ;
				tri->normal[2] = d ;
				for( k = 0 ; k < 2 ; k++ )
				{
					d = tri->vs[k][0] ;
					tri->vs[k][0] = tri->vs[k][2] ;
					tri->vs[k][2] = d ;
				}
			}
		}
	}
	
	return ;
}

// nt add 2021/6/4
void stlSwapYZ(STL* pSTL)
{
	int i, j, k ;
	double d ;
	STLBLK* blk = NULL ;
	STLTRI* tri = NULL ;

	if( pSTL == NULL )
		return ;

	for( i = 0 ; i < pSTL->nBlk ; i++ )
	{
		blk = &(pSTL->blks[i]) ;
		for( j = 0 ; j < blk->nTri ; j++ )
		{
			tri = &(blk->tris[j]) ;
			if( tri )
			{
				d = tri->normal[1] ;
				tri->normal[1] = tri->normal[2] ;
				tri->normal[2] = d ;
				for( k = 0 ; k < 2 ; k++ )
				{
					d = tri->vs[k][1] ;
					tri->vs[k][1] = tri->vs[k][2] ;
					tri->vs[k][2] = d ;
				}
			}
		}
	}
	
	return ;
}

// smf add 2021/7/20
void stlSwapXZ2(STL * pSTL)
{
	int i, j, k;
	double d;
	STLBLK* blk = NULL;
	STLTRI* tri = NULL;

	if (pSTL == NULL)
		return;

	for (i = 0; i < pSTL->nBlk; i++)
	{
		blk = &(pSTL->blks[i]);
		for (j = 0; j < blk->nTri; j++)
		{
			tri = &(blk->tris[j]);
			if (tri)
			{
				d = tri->normal[0];
				tri->normal[0] = tri->normal[2];
				tri->normal[2] = d;
				for (k = 0; k < 3; k++)
				{
					d = tri->vs[k][0];
					tri->vs[k][0] = tri->vs[k][2];
					tri->vs[k][2] = d;
				}
			}
		}
	}
	d = pSTL->box.min[0];
	pSTL->box.min[0] = pSTL->box.min[2];
	pSTL->box.min[2] = d;
	d = pSTL->box.max[0];
	pSTL->box.max[0] = pSTL->box.max[2];
	pSTL->box.max[2] = d;
	d = pSTL->p[0];
	pSTL->p[0] = pSTL->p[2];
	pSTL->p[2] = d;

	return;
}

// smf add 2021/7/20
void stlSwapYZ2(STL * pSTL)
{
	int i, j, k;
	double d;
	STLBLK* blk = NULL;
	STLTRI* tri = NULL;

	if (pSTL == NULL)
		return;

	for (i = 0; i < pSTL->nBlk; i++)
	{
		blk = &(pSTL->blks[i]);
		for (j = 0; j < blk->nTri; j++)
		{
			tri = &(blk->tris[j]);
			if (tri)
			{
				d = tri->normal[1];
				tri->normal[1] = tri->normal[2];
				tri->normal[2] = d;
				for (k = 0; k < 3; k++)
				{
					d = tri->vs[k][1];
					tri->vs[k][1] = tri->vs[k][2];
					tri->vs[k][2] = d;
				}
			}
		}
	}
	d = pSTL->box.min[1];
	pSTL->box.min[1] = pSTL->box.min[2];
	pSTL->box.min[2] = d;
	d = pSTL->box.max[1];
	pSTL->box.max[1] = pSTL->box.max[2];
	pSTL->box.max[2] = d;
	d = pSTL->p[1];
	pSTL->p[1] = pSTL->p[2];
	pSTL->p[2] = d;
	for (k = 0; k < 3; k++)
	{
		d = pSTL->tri.vs[k][1];
		pSTL->tri.vs[k][1] = pSTL->tri.vs[k][2];
		pSTL->tri.vs[k][2] = d;
	}
	d = pSTL->tri.normal[1];
	pSTL->tri.normal[1] = pSTL->tri.normal[2];
	pSTL->tri.normal[2] = d;
	
	return;
}

// smf add 2021/7/21
void stlSwapXY2(STL * pSTL)
{
	int i, j, k;
	double d;
	STLBLK* blk = NULL;
	STLTRI* tri = NULL;

	if (pSTL == NULL)
		return;

	for (i = 0; i < pSTL->nBlk; i++)
	{
		blk = &(pSTL->blks[i]);
		for (j = 0; j < blk->nTri; j++)
		{
			tri = &(blk->tris[j]);
			if (tri)
			{
				d = tri->normal[0];
				tri->normal[0] = tri->normal[1];
				tri->normal[1] = d;
				for (k = 0; k < 2; k++)
				{
					d = tri->vs[k][0];
					tri->vs[k][0] = tri->vs[k][1];
					tri->vs[k][1] = d;
				}
			}
		}
	}
	d = pSTL->box.min[0];
	pSTL->box.min[0] = pSTL->box.min[1];
	pSTL->box.min[1] = d;
	d = pSTL->box.max[0];
	pSTL->box.max[0] = pSTL->box.max[1];
	pSTL->box.max[1] = d;
	d = pSTL->p[0];
	pSTL->p[0] = pSTL->p[1];
	pSTL->p[1] = d;

	return;
}
////////////////////////////////////////////////////////////////