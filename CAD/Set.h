// 2016/12/30 nt
// Set file : 用于一般指针对象的存贮

#pragma once

#define SET_MAX_SIZE 64

////////////////////////////////////////////////////////////////
class CSet
{
private:
	int m_nSize ; // m_objs数组的实际大小
	int m_nObj ;
	void** m_objs ;
public:
	CSet() ;
	CSet(int nSize) ;
	~CSet() ;

	int AddObj(void* pObj) ; // 0=error, 1=success
	int GetNumOfObj() ;
	void* GetObj(int i) ;
	void Clear() ;
} ;
////////////////////////////////////////////////////////////////
