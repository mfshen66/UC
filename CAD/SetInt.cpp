#include "StdAfx.h"

////////////////////////////////////////////////////////////////
CSetInt::CSetInt()
{
	m_nSize = SET_MAX_SIZE ;
	m_nInt = 0 ;
	m_ints = new int[m_nSize] ;
}

CSetInt::CSetInt(int nSize)
{
	m_nSize = nSize ;
	m_nInt = 0 ;
	m_ints = new int[m_nSize] ;
}

CSetInt::~CSetInt()
{
	if( m_ints )
		delete []m_ints ;
}
//--------------------------------------------------------------
int CSetInt::AddInt(int Int)
{
	if( m_nInt > m_nSize ) // 应该不会出现这种情况
		return 0 ; // error
	else
	if( m_nInt == m_nSize ) // 原有的buffer已经满了
	{
		m_nSize *= 2 ;
		int* buf = new int[m_nSize] ;
		memcpy(buf, m_ints, sizeof(int)*m_nInt) ;
		delete []m_ints ;
		m_ints = buf ;
	}

	m_ints[m_nInt] = Int ;
	m_nInt++ ;

	return 1 ;
}

void CSetInt::RemoveLast()
{
	if( m_nInt > 0 )
		m_nInt-- ;

	return ;
}

int CSetInt::GetNumOfInt()
{
	return m_nInt ;
}

int CSetInt::GetInt(int i)
{
	if( i < m_nInt )
		return m_ints[i] ;
	else
		return NULL ;
}
////////////////////////////////////////////////////////////////