#include "StdAfx.h"

////////////////////////////////////////////////////////////////
CSet::CSet()
{
	m_nSize = SET_MAX_SIZE ;
	m_nObj = 0 ;
	m_objs = new void*[m_nSize] ;
}

CSet::CSet(int nSize)
{
	m_nSize = nSize ;
	m_nObj = 0 ;
	m_objs = new void*[m_nSize] ;
}

CSet::~CSet()
{
	if( m_objs )
		delete []m_objs ;
}
//--------------------------------------------------------------
int CSet::AddObj(void* pObj)
{
	if( m_nObj > m_nSize ) // 应该不会出现这种情况
		return 0 ; // error
	else
	if( m_nObj == m_nSize ) // 原有的buffer已经满了
	{
		m_nSize *= 2 ;
		void** buf = new void*[m_nSize] ;
		memcpy(buf, m_objs, sizeof(void*)*m_nObj) ;
		delete []m_objs ;
		m_objs = buf ;
	}

	m_objs[m_nObj] = pObj ;
	m_nObj++ ;

	return 1 ;
}

int CSet::GetNumOfObj()
{
	return m_nObj ;
}

void* CSet::GetObj(int i)
{
	if( i < m_nObj )
		return m_objs[i] ;
	else
		return NULL ;
}

void CSet::Clear()
{
	m_nObj = 0 ;
}
////////////////////////////////////////////////////////////////