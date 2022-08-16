#include "StdAfx.h"
#include "Parameter.h"
#include "PF.h"

////////////////////////////////////////////////////////////////
LAYER* layerCreate(int iLayer, 
	               double h, 
	               int li1, 
	               double lt1, 
	               int li2,
	               double lt2,
	               int nx, 
	               int ny)
{
	LAYER* layer = (LAYER*)malloc(sizeof(LAYER)) ;
	if( layer == NULL )
		return NULL ;
	layer->next = NULL ;
	layer->iLayer = iLayer ;
	layer->h = h ;
	layer->li1 = li1 ;
	layer->lt1 = lt1 ;
	layer->li2 = li2 ;
	layer->lt2 = lt2 ;
	layer->t1 = 0. ;
	layer->t2 = 0. ;
	layer->size = (nx*ny+1)/2 ;
	layer->data = (uchar*)malloc(layer->size*sizeof(uchar)) ;
	if( layer->data == NULL )
	{
		free(layer) ;
		return NULL ;
	}
	// nt add 2017/5/15
	for( int i = 0 ; i < layer->size ; i++ )
		layer->data[i] = 0 ;

	return layer ;
}

LAYER* layerCreate2(int n)
{
	LAYER* layer = (LAYER*)malloc(sizeof(LAYER)) ;
	if( layer == NULL )
		return NULL ;
	layer->next = NULL ;
	layer->iLayer = 0 ;
	layer->h = 0. ;
	layer->li1 = 0 ;
	layer->lt1 = 0. ;
	layer->li2 = 0 ;
	layer->lt2 = 0. ;
	layer->size = (n+1)/2 ;
	layer->data = (uchar*)malloc(layer->size*sizeof(uchar)) ;
	if( layer->data == NULL )
	{
		free(layer) ;
		return NULL ;
	}
	// nt add 2017/5/15
	for( int i = 0 ; i < layer->size ; i++ )
		layer->data[i] = 0 ;

	return layer ;
}

void layerFree(LAYER* layer)
{
	if( layer )
	{
		if( layer->data )
			free(layer->data) ;
		free(layer) ;
	}

	return ;
}

// index = j*nx+i, 从下至上，从左到右
uchar layerGet(LAYER* layer, int index)
{
	uchar c ;
	int s, t ;

	s = index/2 ;
	t = index%2 ;
	c = layer->data[s] ;
	if( t == 1 )
		return c&15 ; // 取低4bits
	else
		return c/16 ; // 取高4bits
}

void layerSet(LAYER* layer, int index, uchar c)
{
	uchar old ;
	int s, t ;

	s = index/2 ;
	t = index%2 ;
	old = layer->data[s] ;
	if( t == 1 )
	{
		old = (layer->data[s]&240) ;
		layer->data[s] = old+c ; // 存贮在低4bits
	}
	else
	{
		old = (layer->data[s]&15) ;
		layer->data[s] = (c<<4)+old ; // 高4bits
	}

	return ;
}

void layerLoad(LAYER* layer, CArchive& ar)
{
	if( layer )
	{
		ar >> layer->iLayer
		   >> layer->h
		   >> layer->li1
		   >> layer->lt1
		   >> layer->li2
		   >> layer->lt2
		   >> layer->t1
		   >> layer->t2
		   >> layer->size ;
		if( layer->size > 0 )
		{
			layer->data = (uchar*)malloc(sizeof(uchar)*layer->size) ;
			ar.Read(layer->data, sizeof(uchar)*layer->size) ;
		}
		else
			layer->data = NULL ;
	}

	return ;
}

void layerSave(LAYER* layer, CArchive& ar)
{
	if( layer )
	{
		ar << layer->iLayer
		   << layer->h
		   << layer->li1
		   << layer->lt1
		   << layer->li2
		   << layer->lt2
		   << layer->t1
		   << layer->t2
		   << layer->size ;
		if( layer->size > 0 )
			ar.Write(layer->data, sizeof(uchar)*layer->size) ;
	}

	return ;
}
//--------------------------------------------------------------
PF* pfCreate(PARAMETER* parameter, 
	         double vol, 
	         double time, 
			 COLORREF color,
	         double h, 
	         double lt1, 
	         int li1, 
	         double lt2, 
	         int li2, 
	         int constH, 
	         int nx, 
	         int ny)
{
	PF* pf = (PF*)malloc(sizeof(PF)) ;
	if( pf == NULL )
		return NULL ;
	if( parameter )
		pf->parameter = *parameter ;
	else
		parameterInit(&(pf->parameter)) ;
	pf->vol = vol ;
	pf->time = time ;
	pf->color = color ;
	pf->h = h ;
	pf->lt1 = lt1 ;
	pf->li1 = li1 ;
	pf->lt2 = lt2 ;
	pf->li2 = li2 ;
	pf->constH = constH ;
	pf->nx = nx ;
	pf->ny = ny ;
	pf->nLayer = 0 ;
	pf->layers = NULL ;
	pf->last = NULL ;

	return pf ;
}

PF* pfCreate2()
{
	PF* pf = (PF*)malloc(sizeof(PF)) ;
	if( pf == NULL )
		return NULL ;
	parameterInit(&(pf->parameter)) ;
	pf->vol = 0. ;
	pf->time = 0. ;
	pf->color = 0 ;
	pf->h = 0. ;
	pf->lt1 = 0. ;
	pf->li1 = 0 ;
	pf->lt2 = 0. ;
	pf->li2 = 0 ;
	pf->constH = 1 ;
	pf->nx = 0 ;
	pf->ny = 0 ;
	pf->nLayer = 0 ;
	pf->layers = NULL ;
	pf->last = NULL ;

	return pf ;
}

void pfFree(PF* pf)
{
	LAYER* layer, *next ;

	if( pf )
	{
		layer = pf->layers ;
		while(layer)
		{
			next = layer->next ;
			layerFree(layer) ;
			layer = next ;
		}
		free(pf) ;
	}

	return ;
}

// 输入的数据中一个像素占一个uchar
int pfAddLayer(PF* pf, uchar* data)
{
	uchar c ;
	int i, j, n, li1, li2 ;
	double lt1, lt2 ;
	LAYER* layer ;

	if( pf == NULL ||
		data == NULL )
		return 0 ;
	li1 = pf->parameter.lils[0] ; // ???????????????还得有个算法算一下
	lt1 = pf->parameter.ltls[0] ; // ?????????????????
	li2 = pf->parameter.lis[0] ;
	lt2 = pf->parameter.lts[0] ;
	layer = layerCreate(pf->nLayer, pf->h, li1, lt1, li2, lt2, pf->nx, pf->ny) ;
	if( layer == NULL )
		return 0 ;
	n = pf->nx*pf->ny ;
	for( i = j = 0 ; i < n ; i++ )
	{
		c = data[i] ; // c应该<=15
		if( j%2 == 0 )
			layer->data[j] = c ; // 保存在data[j]的低4bits
		else
		{
			layer->data[j] += (c<<4) ;
			j++ ;
		}
	}
	if( pf->last == NULL )
		pf->layers = layer ;
	else
		pf->last->next = layer ;
	pf->last = layer ;
	(pf->nLayer)++ ;

	return 1 ;
}

int pfAddLayer2(PF* pf, LAYER* layer)
{
	if( pf == NULL ||
		layer == NULL )
		return 0 ;

	if( pf->last == NULL )
		pf->layers = layer ;
	else
		pf->last->next = layer ;
	pf->last = layer ;
	(pf->nLayer)++ ;

	return 1 ;
}

int pfLoad(PF* pf, CString& fileName)
{
	int i, n ;
	LAYER* layer ;
	CFile file ;

	if( file.Open(fileName, CFile::CFile::modeRead) )
	{
		CArchive ar(&file, CArchive::load) ;
		ar.Read(&pf->parameter, sizeof(PARAMETER)) ;
		ar >> pf->vol
		   >> pf->time
		   >> pf->color
		   >> pf->h
		   >> pf->lt1
		   >> pf->li1
		   >> pf->lt2
		   >> pf->li2
		   >> pf->constH
		   >> pf->nx
		   >> pf->ny
		   >> pf->nLayer ;
		n = pf->nx*pf->ny ;
		for( i = 0 ; i < pf->nLayer ; i++ )
		{
			layer = layerCreate2(n) ;
			if( layer == NULL )
				return 0 ;
			layerLoad(layer, ar) ;
			if( pf->last == NULL )
				pf->layers = layer ;
			else
				pf->last->next = layer ;
			pf->last = layer ;
		}
		ar.Close() ;
		file.Close() ;
		return 1 ;
	}
	else
		return 0 ;
}

int pfSave(PF* pf, CString& fileName)
{
	LAYER* layer ;
	CFile file ;

	if( file.Open(fileName, CFile::modeCreate | CFile::modeWrite) )
	{
		CArchive ar(&file, CArchive::store) ;
		ar.Write(&pf->parameter, sizeof(PARAMETER)) ;
		ar << pf->vol
		   << pf->time
		   << pf->color
		   << pf->h
		   << pf->lt1
		   << pf->li1
		   << pf->lt2
		   << pf->li2
		   << pf->constH
		   << pf->nx
		   << pf->ny
		   << pf->nLayer ;
		for( layer = pf->layers ; layer ; layer = layer->next )
			layerSave(layer, ar) ;
		ar.Close() ;
		file.Close() ;
		return 1 ;
	}
	else
		return 0 ;
}

int pfLoad2(PF* pf, CString& fileName)
{
	int i, k, m, n ;
	LAYER* layer ;
	CFile file ;
	CString temp ;

	if( file.Open(fileName, CFile::modeRead) )
	{
		CArchive ar(&file, CArchive::load) ;
		ar.Read(&pf->parameter, sizeof(PARAMETER)) ;
		ar >> pf->vol
		   >> pf->time
		   >> pf->color
		   >> pf->h
		   >> pf->lt1
		   >> pf->li1
		   >> pf->lt2
		   >> pf->li2
		   >> pf->constH
		   >> pf->nx
		   >> pf->ny
		   >> pf->nLayer ;
		ar.Close() ;
		file.Close() ;
		return 1 ;
	}
	else
		return 0 ;

	k = 0 ;
	n = pf->nx*pf->ny ;
	while(m<pf->nLayer)
	{
		temp.Format(_T("%d"), k) ;
		temp = fileName+temp ;
		if( file.Open(temp, CFile::modeRead) )
		{
			CArchive ar(&file, CArchive::store) ;
			ar >> m ;
			for( i = 0 ; i < m ; i++ )
			{
				layer = layerCreate2(n) ;
				if( layer == NULL )
					return 0 ;
				layerLoad(layer, ar) ;
				if( pf->last == NULL )
					pf->layers = layer ;
				else
					pf->last->next = layer ;
				pf->last = layer ;
			}
			ar.Close() ;
			file.Close() ;
		}
		else
			return 0 ;
	}
	if( pf->nLayer != n )
		return 0 ;

	return 1 ;
}

int pfSave2(PF* pf, CString& fileName, int n)
{
	int i, k, m ;
	LAYER* layer ;
	CFile file ;
	CString temp ;

	if( file.Open(fileName, CFile::modeCreate | CFile::modeWrite) )
	{
		CArchive ar(&file, CArchive::store) ;
		ar.Write(&pf->parameter, sizeof(PARAMETER)) ;
		ar << pf->vol
		   << pf->time
		   << pf->color
		   << pf->h
		   << pf->lt1
		   << pf->li1
		   << pf->lt2
		   << pf->li2
		   << pf->constH
		   << pf->nx
		   << pf->ny
		   << pf->nLayer ;
		ar.Close() ;
		file.Close() ;
		return 1 ;
	}
	else
		return 0 ;

	k = 0 ;
	layer = pf->layers ;
	while(layer)
	{
		temp.Format(_T("%d"), k) ;
		temp = fileName+temp ;
		if( file.Open(temp, CFile::modeCreate | CFile::modeWrite) )
		{
			CArchive ar(&file, CArchive::store) ;
			m = pf->nLayer-k*n ; // 还剩下的LAYER总数
			m = m>n?n:m ;
			ar << m ;
			for( i = 0 ; i < m ; i++ )
			{
				layerSave(layer, ar) ;
				layer = layer->next ;
				if( layer == NULL )
					break ;
			}
			ar.Close() ;
			file.Close() ;
		}
		else
			return 0 ;
		k++ ;
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////