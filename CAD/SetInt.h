// 2017/1/20 nt
// Set file : ����int����Ĵ���

#pragma once

#define SET_MAX_SIZE 64

////////////////////////////////////////////////////////////////
class CSetInt
{
private:
	int m_nSize ; // m_objs�����ʵ�ʴ�С
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
