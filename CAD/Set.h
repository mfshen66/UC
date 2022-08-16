// 2016/12/30 nt
// Set file : ����һ��ָ�����Ĵ���

#pragma once

#define SET_MAX_SIZE 64

////////////////////////////////////////////////////////////////
class CSet
{
private:
	int m_nSize ; // m_objs�����ʵ�ʴ�С
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
