// 2017/5/31 nt
// pixels array
#ifndef _PArray_h_
#define _PArray_h_

////////////////////////////////////////////////////////////////
typedef struct _parray PARRAY ;
struct _parray
{
	double xmin ;
	double xmax ;
	double ymin ;
	double ymax ;
	double w ; // ��࣬x��y�����ϵ�ɨ���ߵļ��
	int nx ; // ��nx��
	int ny ; // ��ny��
	int size ;
	uchar* pixels ;
} ;

PARRAY* parrayCreate(int size, double w) ;
void parrayFree(PARRAY* parray) ;
int parrayMergeZBZB2(PARRAY* parray, ZB* zb, ZB2* zb2) ;
int parrayMergeZB2ZB2(PARRAY* parray, ZB2* zb1, ZB2* zb2) ; // nt add 2021/12/29
uchar parrayGetPixel(PARRAY* parray, int i, int j) ;
void parraySetPixel(PARRAY* parray, int i, int j, uchar pixel) ;
int parrayGray(PARRAY* parray, int grays[4]);
int parrayGray1(PARRAY* parray, int grays[4]);
////////////////////////////////////////////////////////////////

#endif