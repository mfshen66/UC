// 2017/1/20 nt
// Set file : 用于int对象的存贮

#pragma once

#define SET_MAX_SIZE 64

////////////////////////////////////////////////////////////////
class CSetInt
{
private:
	int m_nSize ; // m_objs数组的实际大小
	int m_nInt ;
	int* m_ints ;
public:
	CSetInt() ;
	CSetInt(int nSize) ;
	~CSetInt() ;

	int AddInt(int Int) ; // 0=error, 1=success
	void RemoveLast() ;
	int GetNumOfInt() ;
	int GetInt(int i) ;
} ;
////////////////////////////////////////////////////////////////
