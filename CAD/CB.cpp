#include "StdAfx.h"
#include "CB.h"

////////////////////////////////////////////////////////////////
int cellblkInit(CELLBLK* blk, int max)
{
	if( blk )
	{
		blk->max = max ;
		blk->n = 0 ;
		if( max > 0 )
		{
			blk->cells = (CELL*)malloc(max*sizeof(CELL)) ;
			if( blk->cells == NULL )
				return 0 ;
		}
		else
			blk->cells = NULL ;
	}

	return 1 ;
}

void cellblkEmpty(CELLBLK* blk)
{
	if( blk )
	{
		if( blk->cells )
			free(blk->cells) ;
	}

	return ;
}


CELL* cellCreate()
{
	CELL *cell = NULL;
	cell = (CELL *)malloc(sizeof(CELL));
	cell->next = NULL;
	cell->k = -1;
	return cell;
}
void cellFree(CELL * cell)
{
	if (cell)
	{
		free(cell);
	}
	return;
}
//==============================================================
HULL * hullCreate()
{
	HULL *hull = NULL;
	hull = (HULL *)malloc(sizeof(HULL));
	hull->pn = 0;
	hull->fn = 0;

	return hull;
}


int hullChkPnt(HULL * hull, PNT3D p, double tol)
{
	int i, n = hull->fn;
	VEC3D v;
	FACE *tri;
	tol = hull->tol;
	for (i = 0; i < n; i++)
	{
		tri = &hull->tri[i];
		if (tri->flag)
		{
			mathGetVec(tri->A, p, v);
			int chkf = faceChkPntSide(p, tri, tol);
			if (chkf == 1//�����������
				/*|| (chkf == -1 && triChkPnt(tri, p, tol) != IDIN)*/)
				return 0;
		}
		else
			return -1;
	}
	return 1;
}
int hullCutLine(HULL * hull, PNT3D p1, PNT3D p2, PNT3D IP1, PNT3D IP2)
{
	VEC3D dir, v1;
	PNT3D interp1, interp2;
	double tol = hull->tol;
	int InPNum = 0;
	mathGetVec(p1, p2, v1);
	PNT3D points[MAXFACENUM * 2];
	int pn = 0, ipn;
	if (mathGetVecUnit(p1, p2, dir) == 0)
		return -1;//�����߶ι���
	FACE *tri;
	int n = hull->fn, i, j, k;

	for (i = 0; i < n; i++)
	{
		tri = &(hull->tri[i]);
		if (fabs(mathOProduct(dir, tri->normal)) < tol)//
		{
			if (faceChkPntSide(p1, tri, tol) != -1)//����ƽ���Ҳ���ƽ����
			{
				continue;
			}
		}
		else//��ƽ���ϻ�һ�����
		{
			if (triIntLin(tri, p1, v1, tol, &InPNum, interp1, interp2) != IDNOINT)//�н���
			{
				for (k = 0; k < 3; k++)//һ������
				{
					points[pn][k] = interp1[k];
				}
				pn++;
				if (InPNum == 2)//��������
				{
					for (k = 0; k < 3; k++)
					{
						points[pn][k] = interp2[k];
					}
					pn++;
				}
			}
		}
	}

	//����ϲ�
	if (pn >= 1)
	{
		ipn = 1;
		for (i = 0; i < pn; i++)
		{
			for (j = i + 1; j < pn; j++)
			{
				mathGetVec(points[i], points[j], v1);
				if (mathVecLen(v1) < tol)
					continue;
				else
				{
					swap(points[j], points[i + 1]);
					ipn = 2;
					break;
				}
			}
		}
	}
	else
	{
		return IDNOINT;
	}
	if (ipn)
	{
		for (k = 0; k < 3; k++)//һ������
			IP1[k] = points[0][k];
	}
	else
	{
		for (k = 0; k < 3; k++)//һ������
		{
			IP1[k] = 0;
			IP2[k] = 0;
		}
	}
	if (ipn > 1)
		for (k = 0; k < 3; k++)//һ������
			IP2[k] = points[1][k];
	
	return ipn;
}
int hullMake(HULL * hull)
{
	if (hull->pn < 4)
	{
		return 0;
	}
	int fail = 1, pn = hull->pn, fn = hull->fn;
	int i = 0, j = 0;
	double tol = hull->tol, d, s;
	VEC3D v1, v2, v3, norm;

	//ȡǰ�ĸ��㹹��������
	for (i = 1; i < pn; i++)
	{
		mathGetVec(hull->points[0], hull->points[i], v1);
		if (mathVecLen(v1) > tol)
		{
			swap(hull->points[1], hull->points[i]);// ȷ��p0, p1������
			fail = 0;
			break;
		}
	}
	if (fail)
		return 0;
	for (i = 2; i < pn; i++)
	{
		s = triCalArea(hull->points[0], hull->points[1], hull->points[i]);
		if (s > tol)
		{
			swap(hull->points[2], hull->points[i]);// ȷ��p0, p1, p2������
			fail = 0;
			break;
		}
	}
	if (fail)
		return 0;
	triGetNormal(hull->points[0], hull->points[1], hull->points[2], INF_DBL, norm);//������
	for (i = 3; i < pn; i++)
	{
		d = fabs(mathDistPntPlnSide(hull->points[i], hull->points[0], norm));//��
		if (d > tol)
		{
			swap(hull->points[3], hull->points[i]);// ȷ��p0, p1, p2, p3������
			fail = 0;
			break;
		}
	}
	if (fail)
		return 0;

	FACE face;
	int count = 0;
	hull->fn = count;
	for (i = 0; i < 4; i++)
	{
		face.a = (i + 1) % 4;
		face.b = (i + 2) % 4;
		face.c = (i + 3) % 4;
		for (j = 0; j < 3; j++)
		{
			face.A[j] = hull->points[face.a][j];
			face.B[j] = hull->points[face.b][j];
			face.C[j] = hull->points[face.c][j];
		}
		face.flag = 1;
		mathGetVec(face.A, face.B, v1);
		mathGetVec(face.A, face.C, v2);
		mathGetVec(face.A, hull->points[i], v3);
		mathVProduct(v1, v2, norm);
		mathUniVec(norm, tol);
		if (mathOProduct(norm, v3) > tol)
		{
			swap(face.B, face.C);//���������⣺A��B��C
			swap(face.b, face.c);
			for (int k = 0; k < 3; k++)
			{
				face.normal[k] = -norm[k];
			}
		}
		else
		{
			for (int k = 0; k < 3; k++)
			{
				face.normal[k] = norm[k];
			}
		}
		faceCreatBox(&face);
		hull->pBelongf[face.a][face.b] = hull->pBelongf[face.b][face.c] = hull->pBelongf[face.c][face.a] = hull->fn;//��¼��������
		hull->tri[count++] = face;
		hull->fn = count;
	}

	for (i = 4; i < hull->pn; i++)
	{
		if (hullChkPnt(hull, hull->points[i], tol) == 1)//���ڵ�ǰ�������ڲ�
		{
			break;
		}
		for (j = 0; j < hull->fn; j++)
		{
			if (hull->tri[j].flag)
			{
				int chkf = faceChkPntSide(hull->points[i], &(hull->tri[j]), tol);
				if (chkf == 1//������j������
					/*|| (chkf == -1 && triChkPnt(&(hull->tri[j]), hull->points[i], tol) != IDIN)*/)
				{
					hullUpdate(hull, j, i, chkf);//���¶�����
				}
			}
			else
			{
				continue;
			}
		}
	}

	//ɾȥ�����ն������ϵ���
	int temp = hull->fn;
	for (i = hull->fn - 1; i >= 0; i--)
	{
		if (hull->tri[i].flag == 0)
		{

			for (j = i + 1; j < hull->fn; j++)
			{
				hull->pBelongf[hull->tri[j].a][hull->tri[j].b] --;
				hull->pBelongf[hull->tri[j].b][hull->tri[j].c] --;
				hull->pBelongf[hull->tri[j].c][hull->tri[j].a] --;
				hull->tri[j - 1] = hull->tri[j];
			}
			hull->fn--;
		}
	}
	return 1;
}
void hullUpdate(HULL * hull, int fi, int pi, int chkf)
{
	//if (chkf != -1)
	//{
	hull->tri[fi].flag = false;//ɾ����ǰ��
//}

//����µ���
	hullDeal(hull, pi, hull->tri[fi].b, hull->tri[fi].a);
	hullDeal(hull, pi, hull->tri[fi].c, hull->tri[fi].b);
	hullDeal(hull, pi, hull->tri[fi].a, hull->tri[fi].c);
}

void hullDeal(HULL * hull, int pi, int fpa, int fpb)
{
	int fi = hull->pBelongf[fpa][fpb];
	FACE newf, *face;
	double tol = hull->tol;
	face = &(hull->tri[fi]);

	if (face->flag)
	{
		int chkf = faceChkPntSide(hull->points[pi], face, tol);
		if (chkf == 1
			/*||(chkf == -1 && triChkPnt(face, hull->points[pi], tol) != IDIN)*/)
		{
			hullUpdate(hull, fi, pi, chkf);
		}
		else
		{
			newf.a = fpb;
			newf.b = fpa;
			newf.c = pi;
			for (int i = 0; i < 3; i++)
			{
				newf.A[i] = hull->points[newf.a][i];
				newf.B[i] = hull->points[newf.b][i];
				newf.C[i] = hull->points[newf.c][i];
			}
			faceCreatBox(&newf);
			VEC3D norm;
			triGetNormal(newf.A, newf.B, newf.C, tol, norm);
			mathUniVec(norm, tol);
			for (int j = 0; j < hull->fn; j++)
			{
				if (hull->tri[j].flag)
				{
					VEC3D v;
					mathGetVec(newf.A, hull->tri[j].A, v);
					if (mathOProduct(v, norm) > tol)//��ǰ�������еĵ���ƽ������
					{
						swap(newf.B, newf.C);
						swap(newf.b, newf.c);
						mathRevVec(norm);
					}
					else if (fabs(mathOProduct(v, norm)) < tol)//��ѡ�����µ�ƽ����
						continue;
					for (int k = 0; k < 3; k++)
					{
						newf.normal[k] = norm[k];
					}
					break;
				}
			}
			face->flag = 1;
			hull->pBelongf[newf.a][newf.b] = hull->pBelongf[newf.b][newf.c] = hull->pBelongf[newf.c][newf.a] = hull->fn;
			hull->tri[hull->fn++] = newf;
		}
	}
	return;
}

void hullFree(HULL * hull)
{
	if (hull)
		free(hull);
	return;
}

void hullDivision(HULL * hull, HULL * hull1, HULL * hull2, PNT3D p1, PNT3D p2, short type)
{
	PNT3D p11, p22, p33;
	VEC3D v1, v2, v;
	if (type == 0x80 || type == 0x81)
	{
		p11[0] = p1[0];
		p11[1] = p1[1]; 
		p11[2] = 0;

		p22[0] = p2[0];
		p22[1] = p2[1];
		p22[2] = 0;
	}
	else if (type == 0x82 || type == 0x83)
	{
		p11[0] = p1[0];
		p11[1] = 0;
		p11[2] = p1[2];

		p22[0] = p2[0];
		p22[1] = 0;
		p22[2] = p2[2];
	}
	else if (type == 0x84 || type == 0x85)
	{
		p11[0] = p1[0];
		p11[1] = p1[1];
		p11[2] = p1[2];

		p22[0] = 0;
		p22[1] = p2[1];
		p22[2] = p2[2];
	}
	mathGetVecUnit(p11, p22, v1);
	//PNT3D *points = new PNT3D[hull->pn];
	int i1 = 0, i2 = 0;
	for (int i = 0; i < hull->pn; i++)
	{
		if ( type == 0x80 || type == 0x81)
		{
			p33[0] = hull->points[i][0];
			p33[1] = hull->points[i][1];
			p33[2] = 0;
		}
		else if ( type == 0x82 || type == 0x83 )
		{
			p33[0] = hull->points[i][0];
			p33[1] = 0;
			p33[2] = hull->points[i][2];
		}
		else if ( type == 0x84 || type == 0x85 )
		{
			p33[0] = 0;
			p33[1] = hull->points[i][1];
			p33[2] = hull->points[i][2];
		}
		mathGetVecUnit(p11, p33, v2);
		mathVProductUnit(v1, v2, v);
		for (int j = 0; j < 2; j++)
		{
			if (v[j] > MIN_DBL)
			{
				memcpy(hull1->points[i1], hull->points[i], sizeof(PNT3D));
				i1++;
			}
			else if (v[j] < -MIN_DBL)
			{
				memcpy(hull2->points[i1], hull->points[i], sizeof(PNT3D));
				i2++;
			}
		}
	}
	hull1->pn = i1;
	hull2->pn = i2;
}

//==============================================================
CSEGM* csegmCreate(int k1, int k2)
{
	CSEGM* segm = NULL ;

	segm = (CSEGM*)malloc(sizeof(CSEGM)) ;
	if( segm == NULL )
		return NULL ;
	segm->next = NULL ;
	segm->k1 = k1 ;
	segm->k2 = k2 ;

	return segm ;
}

void csegmFree(CSEGM* segm)
{
	if( segm )
		free(segm) ;

	return ;
}
//==============================================================
void clineInit(CLINE* line, double x, double y)
{
	if( line )
	{
		line->p[0] = x ;
		line->p[1] = y ;
		line->p[2] = 0. ;
		line->n = 0 ;
		line->segms = NULL ;
		line->m = 0 ;
		line->cells = NULL ;
	}

	return ;
}

void clineFreeSegmAll(CLINE* line)
{
	CSEGM* temp ;

	if( line == NULL )
		return ;
	while(line->segms)
	{
		temp = line->segms->next ;
		csegmFree(line->segms) ;
		line->segms = temp ;
	}

	return ;
}

void clineAddSegm(CLINE* line, CSEGM* segm)
{
	segm->next = line->segms ;
	line->segms = segm ;

	return ;
}

void clineAddCell(CLINE* line, CELL* cell)
{
	cell->next = line->cells ;
	line->cells = cell ;

	return ;
}
//==============================================================
// w : cell�ı߳�
// r1 : �ɵİ뾶
// r2 : ������İ뾶
CB* cbCreate(BOX3D* box, 
	         double w,
			 double r1,
			 double r2,
	         double tol,
			 double ang)
{
	int i, j ;
	double x, y ;
	SLINE* line = NULL ;
	CB* cb = NULL ;

	cb = (CB*)malloc(sizeof(CB)) ;
	if( cb == NULL )
		return NULL ;
	cb->blks = NULL ;
	cb->lines = NULL ;
	cb->xmin = box->min[0] ;
	cb->xmax = box->max[0] ;
	cb->ymin = box->min[1] ;
	cb->ymax = box->max[1] ;
	cb->zmin = box->min[2] ;
	cb->zmax = box->max[2] ;
	cb->w = w ;
	cb->tol = tol ;
	cb->ang = ang ;

	cb->max = CB_MAX_BLK ;
	cb->nBlk = 0 ;
	cb->blks = (CELLBLK*)malloc(cb->max*sizeof(CELLBLK)) ; // ����, 0...max-1
	if( cb->blks == NULL )
		goto Exit ;
	for( i = 0 ; i < cb->max ; i++ )
		cellblkInit(&(cb->blks[i]), 0) ; // û�з���CELL

	cb->nx = (int)ceil((cb->xmax-cb->xmin)/w) ;
	cb->ny = (int)ceil((cb->ymax-cb->ymin)/w) ;
	cb->nz = (int)ceil((cb->zmax-cb->zmin)/w) ;
	cb->lines = (CLINE**)malloc(cb->nx*sizeof(CLINE*)) ; // cb->nx
	if( cb->lines == NULL )
		goto Exit ;
	for( i = 0 ; i < cb->nx ; i++ )
		cb->lines[i] = NULL ;
	for( i = 0 ; i < cb->nx ; i++ )
	{
		x = cb->xmin+w*(0.5+i) ;
		cb->lines[i] = (CLINE*)malloc(cb->ny*sizeof(CLINE)) ; // cb->ny
		if( cb->lines[i] == NULL )
			goto Exit ;
		for( j = 0 ; j < cb->ny ; j++ )
		{
			y = cb->ymin+w*(0.5+j) ;
			clineInit(&(cb->lines[i][j]), x, y)  ;
		}
	}

	cb->r1 = r1 ;
	cb->r2 = r2 ;

	return cb ;
Exit:
	if( cb )
	{
		if( cb->lines )
		{
			for( i = 0 ; i < cb->nx ; i++ )
			{
				if( cb->lines[i] )
					free(cb->lines[i]) ;
			}
			free(cb->lines) ;
		}
		if( cb->blks )
			free(cb->blks) ;

		free(cb) ;
	}

	return NULL ;
}

void cbFree(CB* cb)
{
	int i, j ;

	if( cb )
	{
		if( cb->lines )
		{
			for( i = 0 ; i < cb->nx ; i++ )
			{
				for( j = 0 ; j < cb->ny ; j++ )
					clineFreeSegmAll(&(cb->lines[i][j])) ;
				free(cb->lines[i]) ;
			}
			free(cb->lines) ;
		}

		for( i = 0 ; i < cb->max ; i++ )
			cellblkEmpty(&(cb->blks[i])) ;
		free(cb->blks) ;

		free(cb) ;
	}

	return ;
}
//--------------------------------------------------------------
double cbGetSpacing(CB* cb)
{
	return cb->w ;
}

int cbGetNumOfSLineX(CB* cb)
{
	return cb->nx ;
}

int cbGetNumOfSLineY(CB* cb)
{
	return cb->ny ;
}

// ȡ��i�е�j�е�ɨ����
CLINE* cbGetCLine(CB* cb, int i, int j)
{
	if( i < 0 ||
		i >= cb->nx ||
		j < 0 ||
		j >= cb->ny )
		return NULL ;

	return &(cb->lines[i][j]) ;
}

void cbResI(CB* cb, int& i)
{
	if( i < 0 )
		i = 0 ;
	else
	if( i > cb->nx-1 )
		i = cb->nx-1 ;

	return ;
}

void cbResJ(CB* cb, int& j)
{
	if( j < 0 )
		j = 0 ;
	else
	if( j > cb->ny-1 )
		j = cb->ny-1 ;

	return ;
}

CELL* cbAllocCell(CB* cb)
{
	int iBlk, n ;

	if( cb &&
		cb->blks )
	{
		// ����δ�õ�CELL�ռ�
		iBlk = cb->nBlk-1 ;
		if( iBlk >= 0 )
		{
			n = cb->blks[iBlk].n ;
			if( n < cb->blks[iBlk].max )
			{
				(cb->blks[iBlk].n)++ ;
				return &(cb->blks[iBlk].cells[n]) ; // �������ڴ�
			}
		}

		// û��δ�õ�CELL�ռ�
		(cb->nBlk)++ ;
		if( cb->nBlk <= cb->max )
		{
			iBlk = cb->nBlk-1 ;
			if( cellblkInit(&(cb->blks[iBlk]), BLK_MAX_CELL) != 1 )
				return NULL ;
			cb->blks[iBlk].n = 1 ;
			return &(cb->blks[iBlk].cells[0]) ;
		}
		else
			return NULL ; //???
	}
	else
		return NULL ; // error
}
bool cbDeleteLastCell(CB * cb)
{
	int max = cb->max;
	if (cb->blks)
	{
		if (cb->blks[cb->nBlk - 1].n == 1)
		{
			(cb->blks[cb->nBlk - 1].n)--;
			(cb->nBlk)--;
		}
		else
			(cb->blks[cb->nBlk - 1].n)--;
		return true;
	}
	else
		return false;
}
//--------------------------------------------------------------
/*	2021/6/4 nt
 *	Fill with CELL
 */
int cbFill(CB* cb, STL* stl, PRG* pPrg, STL* m_stls, double w)
{
	if (stl->property == FILLING_BODY)
	{
		int i, j, k, k1, k2, k3, *fs = NULL;
		double dd, x, y, z, tol, ang, s_tol = 0.01, tol_len = 1e-3;// smf add 2022/07/18: tol_len��ͳһ�滻cbFill�еĳ����ݲ�
		SLINE* scanZ = NULL, *scanX = NULL, *scanY = NULL;
		CSEGM* segm = NULL;
		CLINE* line = NULL;
		ZB* zy = NULL, *xz = NULL, *xy = NULL; // deep dir = x, y, z

		PNT3D cPnt, cellPoints[4][2];

		CELL *cell;//smf add 2021/7/5
		IPOINT *p = NULL;

		HULL *hull, *hull1, *hull2;
		hull = hullCreate();
		hull1 = hullCreate();
		hull2 = hullCreate();

		if (cb == NULL ||
			stl == NULL)
			return 0;
		cb->w = w;
		pPrg->ws[pPrg->i] /= 4;
		tol = cb->tol;
		ang = cb->ang;
		hull->tol = tol_len;

		// create z-buffer of [xmin,xmax]x[ymin,ymax]
		xy = zbCreate2(cb->xmin, cb->ymin, cb->nx + 1, cb->ny + 1, cb->w, tol, ang);

		zbSlice(xy, stl, pPrg);

		stlSwapYZ2(stl);
		//stlSwapXZ2(stl);
		// create y-buffer of [xmin,xmax]x[zmin,zmax]
		xz = zbCreate2(cb->xmin, cb->zmin, cb->nx + 1, cb->nz + 1, cb->w, tol, ang);
		zbSlice(xz, stl, pPrg);//��
		//stl->zb2 = xz;
		stlSwapYZ2(stl);

		stlSwapXZ2(stl);
		// create x-buffer of [ymin,ymax]x[zmin,zmax]
		zy = zbCreate2(cb->zmin, cb->ymin, cb->nz + 1, cb->ny + 1, cb->w, tol, ang);
		zbSlice(zy, stl, pPrg);
		stlSwapXZ2(stl);

		fs = new int[cb->nz];
		dd = 1. / (cb->nz*cb->ny);

		int *fc = NULL;
		fc = new int[8];

		for (i = 0; i < cb->nx; i++)
		{
			x = cb->xmin + i * cb->w;
			for (j = 0; j < cb->ny; j++)
			{
				y = cb->ymin + j * cb->w;
				line = cbGetCLine(cb, i, j);
				for (k = 0; k < cb->nz; k++) // initialize fs[]
				{
					z = cb->zmin + k * cb->w;
					fs[k] = 0;
					int kk = 0;
					int count = 0;
					int pn = 0;
					double ds[12] = { 0. };
					// check 4 scanlines
					for (k1 = 0; k1 < 2; k1++)
					{
						for (k2 = 0; k2 < 2; k2++)
						{
							fc[kk] = 1;
							fc[kk + 4] = 1;
							scanZ = zbGetSLine(xy, i + k1, j + k2);//Z����ɨ��

							//smf add 2021/6/16
							//�����Ķ�����0-7���ж����ж������ģ�͵�λ�����
							if (slineChkPt(scanZ, z, cb->tol) != 1)// ���
							{
								fc[kk] = 0;
								count++;
							}
							else
							{
								hull->points[pn][0] = x + k1 * cb->w;//�ڲ����ȱ���
								hull->points[pn][1] = y + k2 * cb->w;
								hull->points[pn][2] = z;
								pn++;
								hull->pn = pn;
							}
							if (slineChkPt(scanZ, z + cb->w, cb->tol) != 1)// �յ�
							{
								fc[kk + 4] = 0;
								count++;//ͳ�Ʋ����ڲ��Ķ�����
							}
							else
							{
								hull->points[pn][0] = x + k1 * cb->w;//�ڲ����ȱ���
								hull->points[pn][1] = y + k2 * cb->w;
								hull->points[pn][2] = z + cb->w;
								pn++;
								hull->pn = pn;
							}
							kk++;

						}

						//if( fs[k] == 0 )
						//	break ;
					}
					//Z����
					if (fc[0] && fc[4])
						ds[0] = cb->w;
					if (fc[1] && fc[5])
						ds[1] = cb->w;
					if (fc[2] && fc[6])
						ds[2] = cb->w;
					if (fc[3] && fc[7])
						ds[3] = cb->w;
					//Y����
					if (fc[0] && fc[1])
						ds[4] = cb->w;
					if (fc[2] && fc[3])
						ds[6] = cb->w;
					if (fc[4] && fc[5])
						ds[5] = cb->w;
					if (fc[6] && fc[7])
						ds[7] = cb->w;
					//X����
					if (fc[0] && fc[2])
						ds[8] = cb->w;
					if (fc[1] && fc[3])
						ds[9] = cb->w;
					if (fc[4] && fc[6])
						ds[10] = cb->w;
					if (fc[5] && fc[7])
						ds[11] = cb->w;

					//����fc[]��ֵ���������������жϾ���λ���ڲ����߽绹���ⲿ��
					if (count == 8)
						fs[k] = 0;//������ȫ�����ڲ�
					else if (count == 0)
						fs[k] = 1;//������ȫ���ڲ�
					else
					{
						fs[k] = 2;//�����ڱ߽���
						//�����ĸ��˳�
						//Z����
						for (k1 = 0; k1 < 2; k1++)
						{
							for (k2 = 0; k2 < 2; k2++)
							{
								if (fabs(ds[k1 * 2 + k2] - cb->w) < tol_len)
									continue;
								else
								{
									scanZ = zbGetSLine(xy, i + k1, j + k2);
									p = scanZ->points;
									for (k3 = 0; k3 < scanZ->n; k3++)//����Z���򽻵�
									{
										if (z - tol_len <= p->z &&
											z + cb->w + tol_len >= p->z)//�ҵ���ǰ�����еĽ���
										{
											ds[k1 * 2 + k2] = p->z - z;//����ý���
											if (fabs(ds[k1 * 2 + k2]) < tol_len
												&& slineChkPt(scanZ, z + 0.5 * cb->w, tol_len) == 1)
											{
												ds[k1 * 2 + k2] = cb->w;
											}
											break;
										}
										p = p->next;
									}
									/*û���ҵ���Ӧ����
									��ø˳�����Ϊ0������Ϊ1*/
									if (k3 == scanZ->n)
									{
										if (slineChkPt(scanZ, z + 0.5 * cb->w, tol_len) == 1)
											ds[k1 * 2 + k2] = cb->w;
										else
											ds[k1 * 2 + k2] = 0.;
									}
								}
							}
						}

						//Y���򣨽���Y��Z��
						for (k1 = 0; k1 < 2; k1++)
						{
							for (k2 = 0; k2 < 2; k2++)
							{
								if (fabs(ds[4 + k1 * 2 + k2] - cb->w) < tol_len)
									continue;
								else
								{
									scanY = zbGetSLine(xz, i + k1, k + k2);
									p = scanY->points;
									for (k3 = 0; k3 < scanY->n; k3++)//����Y���򽻵�
									{
										if (y - tol_len <= p->z &&
											y + cb->w + tol_len >= p->z)//�ҵ���ǰ�����еĽ���
										{
											ds[4 + k1 * 2 + k2] = p->z - y;//����ý���
											if (fabs(ds[4 + k1 * 2 + k2]) < tol_len
												&& slineChkPt(scanY, y + 0.5 * cb->w, tol_len) == 1)
											{
												ds[4 + k1 * 2 + k2] = cb->w;
											}
											break;
										}
										p = p->next;
									}
									/*û���ҵ���Ӧ����
									��ø˳�����Ϊ0������Ϊ1*/
									if (k3 == scanY->n)
									{
										if (slineChkPt(scanY, y + 0.5 * cb->w, tol_len) == 1)
											ds[4 + k1 * 2 + k2] = cb->w;
										else
											ds[4 + k1 * 2 + k2] = 0.;
									}
								}
							}
						}

						//X���򣨽���X��Z��
						for (k1 = 0; k1 < 2; k1++)
						{
							for (k2 = 0; k2 < 2; k2++)
							{
								if (fabs(ds[8 + k1 * 2 + k2] - cb->w) < tol_len)
									continue;
								else
								{
									scanX = zbGetSLine(zy, k + k1, j + k2);
									p = scanX->points;
									for (k3 = 0; k3 < scanX->n; k3++)//����Z���򽻵�
									{
										if (x - tol_len <= p->z &&
											x + cb->w + tol_len >= p->z)//�ҵ���ǰ�����еĽ���
										{
											ds[8 + k1 * 2 + k2] = p->z - x;//����ý���
											if (fabs(ds[8 + k1 * 2 + k2]) < tol_len
												&& slineChkPt(scanX, x + 0.5 * cb->w, tol_len) == 1)
											{
												ds[8 + k1 * 2 + k2] = cb->w;
											}
											break;
										}
										p = p->next;
									}
									/*û���ҵ���Ӧ����
									��ø˳�����Ϊ0������Ϊ1*/
									if (k3 == scanX->n)
									{
										if (slineChkPt(scanX, x + 0.5 * cb->w, tol_len) == 1)
											ds[8 + k1 * 2 + k2] = cb->w;
										else
											ds[8 + k1 * 2 + k2] = 0.;
									}
								}
							}
						}

						if ((ds[0] < s_tol && ds[1] < s_tol && ds[2] < s_tol && ds[3] < s_tol) ||
							(ds[4] < s_tol && ds[5] < s_tol && ds[6] < s_tol && ds[7] < s_tol) ||
							(ds[8] < s_tol && ds[9] < s_tol && ds[10] < s_tol && ds[11] < s_tol))//��������λ��ģ�ͱ߽�
						{
							continue;
						}
						else
						{
							cell = cbAllocCell(cb);

							//����
							if (fc[0] && fc[1] && fc[2] && fc[3])
								//cell->type = 21;
								cell->type = 0x80;
							else if (fc[4] && fc[5] && fc[6] && fc[7])
								//cell->type = 22;
								cell->type = 0x81;
							else if (fc[0] && fc[1] && fc[4] && fc[5])
								//cell->type = 23;
								cell->type = 0x82;
							else if (fc[2] && fc[3] && fc[6] && fc[7])
								//cell->type = 24;
								cell->type = 0x83;
							else if (fc[0] && fc[2] && fc[4] && fc[6])
								//cell->type = 25;
								cell->type = 0x84;
							else if (fc[1] && fc[3] && fc[5] && fc[7])
								//cell->type = 26;
								cell->type = 0x85;
							else if (fc[0] && fc[4])
								//cell->type = 9;
								cell->type = 0x40;
							else if (fc[1] && fc[5])
								//cell->type = 10;
								cell->type = 0x41;
							else if (fc[2] && fc[6])
								//cell->type = 11;
								cell->type = 0x42;
							else if (fc[3] && fc[7])
								//cell->type = 12;
								cell->type = 0x43;
							else if (fc[0] && fc[1])
								//cell->type = 13;
								cell->type = 0x44;
							else if (fc[3] && fc[2])
								//cell->type = 14;
								cell->type = 0x45;
							else if (fc[7] && fc[6])
								//cell->type = 15;
								cell->type = 0x46;
							else if (fc[4] && fc[5])
								//cell->type = 16;
								cell->type = 0x47;
							else if (fc[0] && fc[2])
								//cell->type = 17;
								cell->type = 0x48;
							else if (fc[1] && fc[3])
								//cell->type = 18;
								cell->type = 0x49;
							else if (fc[4] && fc[6])
								//cell->type = 19;
								cell->type = 0x4A;
							else if (fc[5] && fc[7])
								//cell->type = 20;
								cell->type = 0x4B;
							else if (fc[0])
								//cell->type = 1;
								cell->type = 0x20;
							else if (fc[1])
								//cell->type = 2;
								cell->type = 0x21;
							else if (fc[2])
								//cell->type = 3;
								cell->type = 0x22;
							else if (fc[3])
								//cell->type = 4;
								cell->type = 0x23;
							else if (fc[4])
								//cell->type = 5;
								cell->type = 0x24;
							else if (fc[5])
								//cell->type = 6;
								cell->type = 0x25;
							else if (fc[6])
								//cell->type = 7;
								cell->type = 0x26;
							else if (fc[7])
								//cell->type = 8;
								cell->type = 0x27;

							kk = 0;
							for (k1 = 0; k1 < 2; k1++)
							{
								for (k2 = 0; k2 < 2; k2++, kk++)
								{
									if (fabs(ds[kk]) < tol_len || fabs(ds[kk] - cb->w) < tol_len)
										continue;
									else
									{
										hull->points[pn][0] = x + k1 * cb->w;//����
										hull->points[pn][1] = y + k2 * cb->w;
										hull->points[pn][2] = z + ds[kk];
										pn++;
										hull->pn = pn;
									}
								}
							}

							for (k1 = 0; k1 < 2; k1++)
							{
								for (k2 = 0; k2 < 2; k2++, kk++)
								{
									if (fabs(ds[kk]) < tol_len || fabs(ds[kk] - cb->w) < tol_len)
										continue;
									else
									{
										hull->points[pn][0] = x + k1 * cb->w;//����
										hull->points[pn][1] = y + ds[kk];
										hull->points[pn][2] = z + k2 * cb->w;
										pn++;
										hull->pn = pn;
									}
								}
							}

							for (k1 = 0; k1 < 2; k1++)
							{
								for (k2 = 0; k2 < 2; k2++, kk++)
								{
									if (fabs(ds[kk]) < tol_len || fabs(ds[kk] - cb->w) < tol_len)
										continue;
									else
									{
										hull->points[pn][0] = x + ds[kk];//����
										hull->points[pn][1] = y + k2 * cb->w;
										hull->points[pn][2] = z + k1 * cb->w;
										pn++;
										hull->pn = pn;
									}
								}
							}
							if (cell->type & 0x60)	// ���ڵڶ����������
							{

								hullMake(hull);

								//�Խ���
								/////////////////////////////////////////////////
								cellPoints[0][0][0] = x;
								cellPoints[0][0][1] = y;
								cellPoints[0][0][2] = z;
								cellPoints[0][1][0] = x + cb->w;
								cellPoints[0][1][1] = y + cb->w;
								cellPoints[0][1][2] = z + cb->w;
								/////////////////////////////////////////////////
								cellPoints[1][0][0] = cellPoints[0][1][0];
								cellPoints[1][0][1] = cellPoints[0][0][1];
								cellPoints[1][0][2] = cellPoints[0][0][2];
								cellPoints[1][1][0] = cellPoints[0][0][0];
								cellPoints[1][1][1] = cellPoints[0][1][1];
								cellPoints[1][1][2] = cellPoints[0][1][2];
								/////////////////////////////////////////////////
								cellPoints[2][0][0] = cellPoints[0][1][0];
								cellPoints[2][0][1] = cellPoints[0][1][1];
								cellPoints[2][0][2] = cellPoints[0][0][2];
								cellPoints[2][1][0] = cellPoints[0][0][0];
								cellPoints[2][1][1] = cellPoints[0][0][1];
								cellPoints[2][1][2] = cellPoints[0][1][2];
								/////////////////////////////////////////////////
								cellPoints[3][0][0] = cellPoints[0][0][0];
								cellPoints[3][0][1] = cellPoints[0][1][1];
								cellPoints[3][0][2] = cellPoints[0][0][2];
								cellPoints[3][1][0] = cellPoints[0][1][0];
								cellPoints[3][1][1] = cellPoints[0][0][1];
								cellPoints[3][1][2] = cellPoints[0][1][2];
								//��
								for (int ii = 0; ii < 4; ii++)
								{
									//cell->flags[ii] = hullCutLine(hull, cellPoints[ii][0], cellPoints[ii][1], cell->begin[ii], cell->end[ii]);

									if (hullCutLine(hull, cellPoints[ii][0], cellPoints[ii][1], cell->begin[ii], cell->end[ii]) == 0)
									{
										cell->flags[ii] = false;
									}
									else
										cell->flags[ii] = true;
								}


							}
							else
								if (cell->type & 0x80) // ���ڵ�һ����
								{

									hullMake(hull);

									//�Խ���
									/////////////////////////////////////////////////
									cellPoints[0][0][0] = x;
									cellPoints[0][0][1] = y;
									cellPoints[0][0][2] = z;
									cellPoints[0][1][0] = x + cb->w;
									cellPoints[0][1][1] = y + cb->w;
									cellPoints[0][1][2] = z + cb->w;
									/////////////////////////////////////////////////
									cellPoints[1][0][0] = cellPoints[0][1][0];
									cellPoints[1][0][1] = cellPoints[0][0][1];
									cellPoints[1][0][2] = cellPoints[0][0][2];
									cellPoints[1][1][0] = cellPoints[0][0][0];
									cellPoints[1][1][1] = cellPoints[0][1][1];
									cellPoints[1][1][2] = cellPoints[0][1][2];
									/////////////////////////////////////////////////
									cellPoints[2][0][0] = cellPoints[0][1][0];
									cellPoints[2][0][1] = cellPoints[0][1][1];
									cellPoints[2][0][2] = cellPoints[0][0][2];
									cellPoints[2][1][0] = cellPoints[0][0][0];
									cellPoints[2][1][1] = cellPoints[0][0][1];
									cellPoints[2][1][2] = cellPoints[0][1][2];
									/////////////////////////////////////////////////
									cellPoints[3][0][0] = cellPoints[0][0][0];
									cellPoints[3][0][1] = cellPoints[0][1][1];
									cellPoints[3][0][2] = cellPoints[0][0][2];
									cellPoints[3][1][0] = cellPoints[0][1][0];
									cellPoints[3][1][1] = cellPoints[0][0][1];
									cellPoints[3][1][2] = cellPoints[0][1][2];
									//��
									for (int ii = 0; ii < 4; ii++)
									{
										//cell->flags[ii] = hullCutLine(hull, cellPoints[ii][0], cellPoints[ii][1], cell->begin[ii], cell->end[ii]);

										if (hullCutLine(hull, cellPoints[ii][0], cellPoints[ii][1], cell->begin[ii], cell->end[ii]) == 0)
										{
											cell->flags[ii] = false;
										}
										else
											cell->flags[ii] = true;
									}

								}
							short proper = Undefined;// smf add 2022/07/18
							//���ĵ�
							mathGetMidPnt(cellPoints[0][0], cellPoints[0][1], cPnt);

							if (hullChkPnt(hull, cPnt, tol) != 0)//���ĵ㲻���ⲿ
								cell->flagC = true;
							else
							{
								cell->flagC = false; // ���ĵ㲻���ڲ�

								// �������STL������
								proper = propertyOfClosestSTL(cPnt, m_stls, stl);// smf modify 2022/07/18
								if (proper == CAVITY)
								{
									// ɾ����ǰ����
									cbDeleteLastCell(cb);
									continue;
								}
							}
							cell->k = k;//��λ
							clineAddCell(line, cell);//����ɨ������

							line->m = line->m + 1;
						}
					}
				}

				for (k1 = 0; k1 < cb->nz - 1; k1++)
				{
					if (fs[k1] == 1)
					{
						for (k2 = k1 + 1; k2 < cb->nz; k2++)
						{
							if (fs[k2] != 1)
								break;
						}
						// ��line���������������ڲ�CELL, all fs[k]=1
						segm = csegmCreate(k1, k2 - 1);
						clineAddSegm(line, segm);
						k1 = k2;
					}
				}

				if (pPrg)
					pPrg->c += (dd*pPrg->ws[pPrg->i]);
			}
		}

		delete[]fs;
		fs = NULL;

		delete[]fc;
		fc = NULL;

		hullFree(hull);
		hullFree(hull1);
		hullFree(hull2);

		return 1;
	}
	return 0;
}

int cbDraw(CB* cb, void* pVI)
{
	int i, j, k ;
	PNT3D min, max;
	CSEGM* segm = NULL ;
	CLINE* line = NULL ;
	CELL *cell = NULL;

	glLineWidth(1.f) ;
	glColor3ub(0, 0, 0) ;
	for( i = 0 ; i < cb->nx ; i++ )
	{
		min[0] = cb->xmin+cb->w*i ;
		max[0] = min[0]+cb->w ;
		for( j = 0 ; j < cb->ny ; j++ )
		{
			min[1] = cb->ymin+cb->w*j ;
			max[1] = min[1]+cb->w ;
			line = cbGetCLine(cb, i, j) ;
			if( line )
			{
				//����������Ԫ
				segm = line->segms ;
				while(segm)
				{
					for( k = segm->k1 ; k <= segm->k2 ; k++ )
					{
						min[2] = cb->zmin+cb->w*k ;
						max[2] = min[2]+cb->w ;
						drawCube3(min, max) ;
					}
					segm = segm->next ;
				}

				//���Ƶ�������
				cell = line->cells;
				while (cell)
				{
					for (int ii = 0; ii < 4; ii++)
					{
						if (cell->flags[ii])
						{
							drawCell(cell->begin[ii], cell->end[ii]);
						}
					}
					cell = cell->next;
				}
			}
		}
	}

	// smf add 2022/08/10
	// ���Ʋ��Ծ���
	i = -1;
	j = -1;
	k = -1;
	if ( getCellPosition(i, j, k) )
	{
		if (cell = findTestCell(cb, i, j, k))
		{
			drawTestCell(cell, cb);
		}
	}

	return 1 ;
}

// nt add 2021/12/26
// ������ֻ�ڴ洢ʱ��
void cbInitCellIndexAll(CB* cb)
{
	int iBlk, iCell ;
	CELLBLK* blk ;
	CELL* cell ;

	if( !cb )
		return ;

	for( iBlk = 0 ; iBlk < cb->nBlk ; iBlk++ )
	{
		blk = &(cb->blks[iBlk]) ;
		for( iCell = 0 ; iCell < blk->n ; iCell++ )
		{
			cell = &(blk->cells[iCell]) ;
			cell->iBlk = iBlk ;
			cell->iCell = iCell ;
		}
	}

	return ;
}

int cbSerialize(CB* cb, CArchive& ar)
{
	int i, j, k, n, m, iBlk, iCell, max ;
	CELLBLK* blk ;
	CSEGM* segm, *temp ;
	CLINE* line ;

	if( cb == NULL )
		return 0 ;

	if( ar.IsStoring() )
	{
		ar.Write(cb, sizeof(CB)) ;

		cbInitCellIndexAll(cb) ;
		ar << cb->nBlk;
		for( iBlk = 0 ; iBlk < cb->nBlk ; iBlk++ )
		{
			blk = &(cb->blks[iBlk]) ;
			ar << blk->max ;
			ar << blk->n ;
			ar.Write(blk->cells, sizeof(CELL)*blk->n) ;
			for( iCell = 0 ; iCell < blk->n ; iCell++ )
			{
				if( blk->cells[iCell].next )
				{
					i = blk->cells[iCell].next->iBlk ;
					j = blk->cells[iCell].next->iCell ;
				}
				else
					i = j = -1 ;
				ar << i << j ;
			}
		}

		for( i = 0 ; i < cb->nx ; i++ )
		{
			for( j = 0 ; j < cb->ny ; j++ )
			{
				line = cbGetCLine(cb, i, j) ;
				if( line )
				{
					ar.Write(line, sizeof(CLINE)) ;
					n = 0 ;
					segm = line->segms ;
					while(segm)
					{
						n++ ;
						segm = segm->next ;
					}
					ar << n ; // number of CSEGM
					segm = line->segms ;
					while(segm)
					{
						ar << segm->k1 ;
						ar << segm->k2 ;
						segm = segm->next ;
					}
					ar << line->m ; // number of CELL
					if ( line->m > 0 )
					{
						ar << line->cells->iBlk;
						ar << line->cells->iCell;
					}
				}
			}
		}
	}
	else
	{
		ar.Read(cb, sizeof(CB)) ;
		ar >> cb->nBlk;
		cb->blks = (CELLBLK*)malloc(cb->nBlk * sizeof(CELLBLK)) ; // blk����, 0...nBlk-1
		if( cb->blks == NULL )
			return 0 ;
		for( i = 0 ; i < cb->nBlk; i++ )
			cellblkInit(&(cb->blks[i]), 0) ; // û�з���CELL
		for( iBlk = 0 ; iBlk < cb->nBlk ; iBlk++ )
		{
			blk = &(cb->blks[iBlk]) ;
			ar >> max ;
			if( cellblkInit(blk, max) != 1 )
				return 0 ;
			ar >> blk->n ;
			ar.Read(blk->cells, sizeof(CELL)*blk->n) ;
			for( iCell = 0 ; iCell < blk->n ; iCell++ )
			{
				ar >> i >> j;
				if ( !((i == -1) && (j == -1)) )
					blk->cells[iCell].next = &(cb->blks[i].cells[j]) ;
				else
					blk->cells[iCell].next = NULL ;
			}
		}

		cb->lines = (CLINE**)malloc(cb->nx*sizeof(CLINE*)) ; // cb->nx
		if( cb->lines == NULL )
			return 0 ;
		for( i = 0 ; i < cb->nx ; i++ )
		{
			cb->lines[i] = (CLINE*)malloc(cb->ny*sizeof(CLINE)) ; // cb->ny
			if( cb->lines[i] == NULL )
				return 0 ;
			for( j = 0 ; j < cb->ny ; j++ )
			{
				line = cbGetCLine(cb, i, j) ;
				if( line )
				{
					ar.Read(line, sizeof(CLINE)) ;
					ar >> n ; // number of CSEGM
					segm = NULL ;
					for( k = 0 ; k < n ; k++ )
					{
						temp = csegmCreate(0, 0) ;
						ar >> temp->k1 ;
						ar >> temp->k2 ;
						if( segm == NULL )
							line->segms = temp ;
						else
							segm->next = temp ;
						segm = temp ; // last CSEGM
					}
					ar >> m ;
					line->m = m ;
					if (m > 0)
					{
						ar >> iBlk;
						ar >> iCell;
						line->cells = &(cb->blks[iBlk].cells[iCell]);
					}
					else
						line->cells = NULL ;
				}
			}
		}
	}

	return 1 ;
}

// smf modify 2022/07/18
int propertyOfClosestSTL(PNT3D p, STL * m_stls, STL * iCurrentSTL)
{
	STL* closestSTL = NULL;
	int properOfSTL = -1;
	double l = 0., lmin = 0.;
	STLBLK *blk;
	STLTRI *tri;
	int i, j;
	for (STL* stl = m_stls; stl; stl = stl->next)
	{
		if (stl->property == FILLING_BODY ||
			stl == iCurrentSTL)					// smf add 2022/07/18������������ǰ������ж�
			continue;
		for (i = 0; i < stl->nBlk; i++)
		{
			blk = &(stl->blks[i]);
			for (j = 0; j < stl->blks->nTri; j++)
			{
				tri = &(blk->tris[j]);
				if ( isPntAboveTri(p, tri) )
				{
					l = fabs(mathDistPntPln(p, tri->vs[0], tri->normal));
					if (l < lmin)
					{
						lmin = l;
						properOfSTL = stl->property;
					}
				}
			}
		}
	}
	return properOfSTL;
}
int isPntAboveTri(PNT3D p, STLTRI* tri)
{
	double l = mathDistPntPln(p, tri->vs[0], tri->normal);
	PNT3D p1;
	for (int i = 0; i < 3; i++)
		p1[i] = p[i] - l * tri->normal[i];
	return isPntInTri(p1, tri, MIN_DBL);
}
int isPntInTri(PNT3D p, STLTRI * tri, double tol)
{
	double s = 0.;
	for (int i = 0; i < 3; i++)
		s += mathCalTriArea(tri->vs[i], tri->vs[(i + 1) % 2], p);
	double tem = mathCalTriArea(tri->vs[0], tri->vs[1], tri->vs[2]);
	if (fabs(s - tem) <= tol)
		return 1;
	else
		return 0;
}

int getCellPosition(int &i, int &j, int &k)
{
	CString filePath;
	efpGet(filePath); // �����ļ�Ŀ¼
	filePath += _T("\\..\\Works\\TestCell.txt"); 
	FILE* fp = nullptr;
	_tfopen_s(&fp, filePath.GetBuffer(0), _T("r+t"));
	if (fp)
	{
		//int n = 0;
		//fscanf(fp, "%d", &n);
		//for (int ii = 0; ii < n; ii++)
		//{
			fscanf(fp, "%d %d %d", &i, &j, &k);
		//}
		fclose(fp);
		fp = nullptr;
		if (i >= 0 && j >= 0 && k >= 0)
			return 1;
		AfxMessageBox(L"TestCell�������");
		return 0;
	}
	AfxMessageBox(L"���ļ�TestCell.txtʧ�ܣ�");
	return 0;
}

CELL * findTestCell(CB * cb, int i, int j, int k)
{
	CELL *cell = nullptr;
	for (int ii = 0; ii < cb->lines[i][j].m; ii++)
	{
		if (cb->lines[i][j].cells[ii].k == k)
		{
			cell = &(cb->lines[i][j].cells[ii]);
			return cell;
		}
	}
	return nullptr;
}

CELL * findTestCell(CB * cb, double x, double y, double z)
{
	int i = (int)((x - cb->xmin) / cb->w);
	int j = (int)((y - cb->ymin) / cb->w);
	int k = (int)((z - cb->zmin) / cb->w);

	CELL *cell = findTestCell(cb, i, j, k);

	return cell;
}

CELL * findTestCell(CB * cb, int iBlk, int iCell)
{
	CELL *cell = nullptr;
	if (iBlk >= 0 && iBlk < cb->nBlk)
	{
		if (iCell >= 0 && iCell < cb->blks[iBlk].n)
		{
			cell = &(cb->blks[iBlk].cells[iCell]);
			return cell;
		}
		AfxMessageBox(L"iCell����������Χ��");
		return nullptr;
	}
	else
	{
		AfxMessageBox(L"iBlk����������Χ��");
		return nullptr;
	}
}

// smf add 2022/08/09
// �������
void drawTestCube(CB * cb, int i, int j, int k)
{
	double w = cb->w;
	double min[3] = { cb->xmin + i * w, cb->ymin + j * w , cb->zmin + k * w };
	double max[3] = { cb->xmin + (i + 1) * w, cb->ymin + (j + 1) * w , cb->zmin + (k + 1) * w };

	glLineWidth(3.0f);
	glColor3f(0., 0., 1.);
	glBegin(GL_LINES);

	glVertex3dv(min);
	glVertex3d(min[0], min[1], max[2]);

	glVertex3dv(min);
	glVertex3d(min[0], max[1], min[2]);

	glVertex3dv(min);
	glVertex3d(max[0], min[1], min[2]);

	glVertex3d(max[0], max[1], min[2]);
	glVertex3dv(max);

	glVertex3d(max[0], min[1], max[2]);
	glVertex3dv(max);

	glVertex3d(min[0], max[1], max[2]);
	glVertex3dv(max);

	glVertex3d(min[0], min[1], max[2]);
	glVertex3d(max[0], min[1], max[2]);

	glVertex3d(min[0], min[1], max[2]);
	glVertex3d(min[0], max[1], max[2]);

	glVertex3d(min[0], max[1], min[2]);
	glVertex3d(max[0], max[1], min[2]);

	glVertex3d(min[0], max[1], min[2]);
	glVertex3d(min[0], max[1], max[2]);

	glVertex3d(max[0], min[1], min[2]);
	glVertex3d(max[0], max[1], min[2]);

	glVertex3d(max[0], min[1], min[2]);
	glVertex3d(max[0], min[1], max[2]);

	glEnd();
}

int drawTestCell(CELL * iCell, CB* cb)
{
	// ����
	glLineWidth(6.f);
	glColor3f(1.0, 0., 0.);

	glBegin(GL_LINES);

	for (int ii = 0; ii < 4; ii++)
	{
		if (iCell->flags[ii])
		{
			glVertex3dv(iCell->begin[ii]);
			glVertex3dv(iCell->end[ii]);
		}
	}
	glEnd();

	int i = (int)((iCell->begin[0][0] - cb->xmin) / cb->w);
	int j = (int)((iCell->begin[0][1] - cb->ymin) / cb->w);
	int k = iCell->k;
	// �������
	drawTestCube(cb, i, j, k);
	return 0;
}


////////////////////////////////////////////////////////////////