/*	2010/4/9 nt
 *	implementation file class CRecord
 */
#include "StdAfx.h"

////////////////////////////////////////////////////////////////
//	2010/4/9 nt
//	CRecord
CRecord::CRecord()
{
	m_stl = NULL ;
}

CRecord::~CRecord()
{
}

STL* CRecord::GetSTL()
{
	return m_stl ;
}

void CRecord::SetSTL(STL* stl)
{
	m_stl = stl ;
}
//--------------------------------------------------------------
CRCreate::CRCreate(STL* stl)
{
	m_stl = stl ;
}

CRCreate::~CRCreate()
{
}

int CRCreate::GetType()
{
	return rtRCreate ;
}
//--------------------------------------------------------------
CRDelete::CRDelete(STL* stl)
{
	m_stl = stl ;
}

CRDelete::~CRDelete()
{
	if( m_stl )
		stlFree(m_stl) ;
	m_stl = NULL ;
}

int CRDelete::GetType()
{
	return rtRDelete ;
}
//--------------------------------------------------------------
CRCut::CRCut(STL* stl)
{
	m_stl = stl ;
}

CRCut::~CRCut()
{
	if( m_stl )
		stlFree(m_stl) ;
	m_stl = NULL ;
}

int CRCut::GetType()
{
	return rtRCut ;
}
//--------------------------------------------------------------
CRCopy::CRCopy(STL* stl)
{
	m_stl = stl ;
}

CRCopy::~CRCopy()
{
}

int CRCopy::GetType()
{
	return rtRCopy ;
}
//--------------------------------------------------------------
CRSize::CRSize(STL* stl, PNT3D center, double scale)
{
	m_stl = stl ;
	memcpy(m_center, center, sizeof(PNT3D)) ;
	m_scale = scale ;
}

CRSize::~CRSize()
{
}

int CRSize::GetType()
{
	return rtRSize ;
}
//--------------------------------------------------------------
CRHeight::CRHeight(STL* stl, double z, SS* ss)
{
	m_stl = stl ;
	m_z = z ;
	m_ss = ss ;
}

CRHeight::~CRHeight()
{
	if( m_ss )
		ssFree(m_ss) ;
}

int CRHeight::GetType()
{
	return rtRHeight ;
}
//--------------------------------------------------------------
CRDir::CRDir(STL* stl, int n, TRANS* ts)
{
	m_stl = stl ;
	m_n = n ;
	if( n > 0 )
	{
		m_ts = new TRANS[n] ;
		memcpy(m_ts, ts, sizeof(TRANS)*n) ;
	}
	else
		m_ts = NULL ;
}

CRDir::~CRDir()
{
	if( m_ts )
		delete []m_ts ;
}

int CRDir::GetType()
{
	return rtRDir ;
}
//--------------------------------------------------------------
CRMakeSS::CRMakeSS(STL* stl, SS* created, SS* old)
{
	m_stl = stl ;
	m_created = created ;
	m_old = old ;
}

CRMakeSS::~CRMakeSS()
{
	if( m_old )
		ssFree(m_old) ;
}

int CRMakeSS::GetType()
{
	return rtRMakeSS ;
}
//--------------------------------------------------------------
CRDelSS::CRDelSS(STL* stl, SS* ss)
{
	m_stl = stl ;
	m_ss = ss ;
}

CRDelSS::~CRDelSS()
{
	if( m_ss )
		ssFree(m_ss) ;
}

int CRDelSS::GetType()
{
	return rtRDelSS ;
}
////////////////////////////////////////////////////////////////
