#include "StdAfx.h"
#include <fstream>
using namespace std ;

////////////////////////////////////////////////////////////////
PARAMETER* parameterCreate()
{
	return (PARAMETER*)malloc(sizeof(PARAMETER)) ;
}

void parameterFree(PARAMETER* parameter)
{
	if( parameter )
		free(parameter) ;

	return ;
}

void parameterInit(PARAMETER* parameter)
{
	int k ;

	// ��ӡ�����ƺ��ͺ�
	_tcscpy_s(parameter->printer, 64, _T("SWCD-50-64X40")) ;
	
	// �������ƺ��ͺ�
	_tcscpy_s(parameter->material, 64, _T("Al2O3-01")) ;

	// x��y��z�����ϵĹ���̨�ķ�Χ
	parameter->L = 256. ; // mm
	parameter->W = 160. ; // mm
	parameter->H = 200. ; // mm

	// �ֱ���
	parameter->PL = 1280 ; // pixel
	parameter->PW = 800 ; // pixel

	parameter->P0 = 0 ;
	parameter->Dx = 1 ;

	// ������
	// ��һ����Ƭ���T0 ��m ����	
	// ��С���T1 12.5 ��m ����	
	// ���T2 25 ��m ����	
	// ���T3 50 ��m ����	
	// ���T4 100 ��m ����	
	// �����T5 200 ��m ���� �����5�����
	parameter->ts[0] = 20. ;
	parameter->ts[1] = 12.5 ;
	parameter->ts[2] = 25. ;
	parameter->ts[3] = 50. ;
	parameter->ts[4] = 100. ;
	parameter->ts[5] = 200. ;

	// lis[i]��Ӧts[i], �ع��ǿ ���� ��Χ��0~100
	for( k = 0 ; k < 6 ; k++ )
		parameter->lis[k] = 90 ;

	// nt add 2017/6/27
	for( k = 0 ; k < 5 ; k++ )
		parameter->lils[k] = 90 ;

	// lts[i]��Ӧts[i], �ع�ʱ�� s ����
	parameter->lts[0] = 30. ;
	parameter->lts[1] = 5. ;
	parameter->lts[2] = 10. ;
	parameter->lts[3] = 15. ;
	parameter->lts[4] = 20. ;
	parameter->lts[5] = 25. ;

	// nt add 2017/6/27
	parameter->ltls[0] = 5. ;
	parameter->ltls[1] = 10. ;
	parameter->ltls[2] = 15. ;
	parameter->ltls[3] = 20. ;
	parameter->ltls[4] = 25. ;

	parameter->w1max = 60. ; // һ�ŵ��ת�٣����٣�rpm ����
	parameter->w1min = 1. ; // һ�ŵ��ת�٣����٣�rpm ����
	parameter->w1p = 1. ; // һ�ŵ�������ٶ� rpm ����
	parameter->L1 = 1. ; // һ�ŵ��������ʼ���� mm ����
	parameter->L2 = 0.05 ; // һ�ŵ�����ٽ������� mm ����
	parameter->L3 = 3. ; // һ�ŵ����ӡ��������ֹͣ���� mm ����
	parameter->H0 = 150. ; // ��ӡƽ̨ȱʡ�߶� mm ���� ��ʼ������ͣ�߶�
	parameter->w2max = 100. ; // ���ŵ��ת�٣����٣�rpm ����
	parameter->w2min = 5. ; // ���ŵ��ת�٣����٣�rpm ����
	parameter->beta = 450. ; // ���ŵ��ת���� deg ����

	parameter->LDdelta = 10. ; // mm
	parameter->LD0 = 100. ; // mm
	parameter->LNF = 0.9 ;
	parameter->LF = 112. ; // mm
	parameter->LENx = 8. ; // mm
	parameter->LENy = 4.5 ; // mm
	parameter->n = 1.5 ;

	// int ���ص�1�Ҷ�Pix1 0~100���ٷ�����
	parameter->grays[0] = 100 ;
	parameter->grays[1] = 80 ;
	parameter->grays[2] = 60 ;
	parameter->grays[3] = 40 ;

	// nt add 2017/6/1
	parameter->wp = 500 ;

	// nt add 2017/8/20
	parameter->tmc = 10. ; // mm
	parameter->ratio = 7.5 ; // �޵�λ

	// double Ԥ������
	for( k = 0 ; k < 5 ; k++ )
	{
		parameter->tps[k] = 0. ;
		parameter->tis[k] = 0 ; // nt add 2017/8/20
	}

	return ;
}

// nt add 2017/5/15
int parameter2PixelCoord(PARAMETER* parameter, 
	                     double x, 
	                     double y, 
	                     int& i, 
	                     int& j)
{
	int s, t ;
	double x0, y0, dx, dy, HL, HW ;

	i = 0 ;
	j = 0 ;

	x += 0.01 ;
	y += 0.01 ;

	HL = 0.5*parameter->L ;
	HW = 0.5*parameter->W ;
	if( x < -HL ||
		x > HL ||
		y < -HW ||
		y > HW )
		return 0 ;

	// ���ؾ���x��߳�Ϊdx, y��߳�Ϊdy, һ��dx=dy
	dx = parameter->L/parameter->PL ;
	dy = parameter->W/parameter->PW ;

	if( parameter->P0 == 0 ||
		parameter->P0 == 1 )
	{
		x0 = -HL ;
		y0 = -HW ;
	}
	else
	if( parameter->P0 == 2 )
	{
		x0 = HL ;
		y0 = -HW ;
	}
	else
	if( parameter->P0 == 3 )
	{
		x0 = HL ;
		y0 = HW ;
	}
	else
	if( parameter->P0 == 4 )
	{
		x0 = -HL ;
		y0 = HW ;
	}
	else
		return 0 ;

	s = (int)(fabs(x-x0)/dx+0.5) ;
	if( s >= parameter->PL )
		s = parameter->PL-1 ;
	t = (int)(fabs(y-y0)/dy+0.5) ;
	if( t >= parameter->PW )
		t = parameter->PW-1 ;

	if( abs(parameter->Dx) == 1 )
	{
		i = s ;
		j = t ;
	}
	else
	{
		i = t ;
		j = s ;
	}
	
	return 1 ;
}

// nt add 2017/6/3
int parameterMatch(PARAMETER* parameter, 
	               double h, 
	               int& li1, 
	               double& lt1, 
	               int& li2, 
	               double& lt2)
{
	int i, I = -1 ;
	double d, min = MAX_DBL ;
	li1 = 0 ;
	lt1 = 0. ;
	li2 = 0 ;
	lt2 = 0. ;
	for( i = 0 ; i < 6 ; i++ )
	{
		d = fabs(0.001*parameter->ts[i]-h) ;
		if( min > d )
		{
			min = d ;
			I = i ;
		}
	}
	if( I < 0 )
		I = 0 ;
	li2 = parameter->lis[I] ;
	lt2 = parameter->lts[I] ;
	if( I > 0 )
	{
		li1 = parameter->lils[I-1] ;
		lt1 = parameter->ltls[I-1] ;
	}
	else
	{
		li1 = parameter->lils[0] ;
		lt1 = parameter->ltls[0] ;
	}

	return I ;
}

// nt add 2017/8/20
// �ò�������С���Χ���Ʋ���
// ����-1=����
// ����0=û�з���������С���Χ�����
// ����n=������n�γ�����С���Χ�����,n>0
int parameterResByLimits(PARAMETER* parameter)
/*{
	// ������������С���Χ�Ĳ��������˲�����������С���Χ��
	// ...
	int k, n = 0 ;

	double Parameter[59];
	Parameter[0]=parameter->L ; // mm
	Parameter[1]=parameter->W ; // mm
	Parameter[2]=parameter->H ; // mm

								// �ֱ���
	Parameter[3]=parameter->PL ; // pixel
	Parameter[4]=parameter->PW ; // pixel

	Parameter[5]=parameter->P0 ;
	Parameter[6]=parameter->Dx ;

	// ������
	// ��һ����Ƭ���T0 ��m ����	
	// ��С���T1 12.5 ��m ����	
	// ���T2 25 ��m ����	
	// ���T3 50 ��m ����	
	// ���T4 100 ��m ����	
	// �����T5 200 ��m ���� �����5�����
	for( k = 0 ; k < 6 ; k++ )
		Parameter[7+k]=parameter->ts[k] ;

	// lis[i]��Ӧts[i], �ع��ǿ ���� ��Χ��0~100
	for( k = 0 ; k < 6 ; k++ )
		Parameter[13+k]=parameter->lis[k] ;

	// nt add 2017/6/27
	for( k = 0 ; k < 5 ; k++ )
		Parameter[19+k]=parameter->lils[k] ;

	// lts[i]��Ӧts[i], �ع�ʱ�� s ����
	for( k = 0 ; k < 6 ; k++ )
		Parameter[24+k]=parameter->lts[k] ;


	for( k = 0 ; k < 5 ; k++ )// nt add 2017/6/27
		Parameter[30+k]=parameter->ltls[k] ;


	Parameter[35]=parameter->w1max ; // һ�ŵ��ת�٣����٣�rpm ����
	Parameter[36]=parameter->w1min ; // һ�ŵ��ת�٣����٣�rpm ����
	Parameter[37]=parameter->w1p ; // һ�ŵ�������ٶ� rpm ����
	Parameter[38]=parameter->L1 ; // һ�ŵ��������ʼ���� mm ����
	Parameter[39]=parameter->L2 ; // һ�ŵ�����ٽ������� mm ����
	Parameter[40]=parameter->L3 ; // һ�ŵ����ӡ��������ֹͣ���� mm ����
	Parameter[41]=parameter->H0 ; // ��ӡƽ̨ȱʡ�߶� mm ���� ��ʼ������ͣ�߶�
	Parameter[42]=parameter->w2max ; // ���ŵ��ת�٣����٣�rpm ����
	Parameter[43]=parameter->w2min ; // ���ŵ��ת�٣����٣�rpm ����
	Parameter[44]=parameter->beta ; // ���ŵ��ת���� deg ����

	Parameter[45]=parameter->LDdelta ; // mm
	Parameter[46]=parameter->LD0 ; // mm
	Parameter[47]=parameter->LNF ;
	Parameter[48]=parameter->LF ; // mm
	Parameter[49]=parameter->LENx ; // mm
	Parameter[50]=parameter->LENy ; // mm
	Parameter[51]=parameter->n ;

	for( k = 0 ; k < 4 ; k++ )// int ���ص�1�Ҷ�Pix1 0~100���ٷ�����
		Parameter[52+k]=parameter->grays[k] ;


	// nt add 2017/6/1
	Parameter[56]=parameter->wp ;

	// nt add 2017/8/20
	Parameter[57]=parameter->tmc ; // mm
	Parameter[58]=parameter->ratio ; // �޵�λ

	int num[100];
	double min[100];//�洢��Χ�½������
	double max[100];//�洢��Χ�Ͻ������
	ifstream inFile;
	int count;
	CString filePath ;
	efpGet(filePath) ; // �����ļ�Ŀ¼
	filePath += _T("\\Libs\\������Χ.txt") ;
	//inFile.open("e:\\������Χ.txt");//�洢��Χ���ĵ���·��
	inFile.open(filePath);

	for (count = 0; count<59; count++)    //������ 
	{
		inFile >> num[count] >> min[count] >> max[count];
		if(count==40||count==41||count==50||count==51)
		{
			if(count==40||count==41||count==51)
			{
				min[count]=0;
				max[count]=Parameter[2];
			}

			if(count==50)
			{
				min[count]=-Parameter[2];
				max[count]=Parameter[2];
			}
		}
	}
	inFile.close();

	for(k=0;k<59;k++)
	{
		if(Parameter[k]<min[k]||Parameter[k]>max[k])
		{
			if(Parameter[k]<min[k])
			{
				Parameter[k]=min[k];
			}
			else
			{
				Parameter[k]=max[k];
			}
			n++;
		}
	}

	parameter->L=Parameter[0] ; // mm
	parameter->W=Parameter[1] ; // mm
	parameter->H=Parameter[2] ; // mm

								// �ֱ���
	parameter->PL=(int)(Parameter[3]+0.5) ; // pixel
	parameter->PW=(int)(Parameter[4]+0.5) ; // pixel

	parameter->P0=(int)(Parameter[5]+0.5) ;
	parameter->Dx=(int)(Parameter[6]+0.5) ;

	// ������
	// ��һ����Ƭ���T0 ��m ����	
	// ��С���T1 12.5 ��m ����	
	// ���T2 25 ��m ����	
	// ���T3 50 ��m ����	
	// ���T4 100 ��m ����	
	// �����T5 200 ��m ���� �����5�����
	for( k = 0 ; k < 6 ; k++ )
		parameter->ts[0]=Parameter[7+k] ;

	// lis[i]��Ӧts[i], �ع��ǿ ���� ��Χ��0~100
	for( k = 0 ; k < 6 ; k++ )
		parameter->lis[k]=(int)(Parameter[13+k]+0.5) ;

	// nt add 2017/6/27
	for( k = 0 ; k < 5 ; k++ )
		parameter->lils[k]=(int)(Parameter[19+k]+0.5) ;

	// lts[i]��Ӧts[i], �ع�ʱ�� s ����
	for( k = 0 ; k < 6 ; k++ )
		parameter->lts[0]=(int)(Parameter[24+k]+0.5) ;


	for( k = 0 ; k < 5 ; k++ )// nt add 2017/6/27
		parameter->ltls[0]=(int)(Parameter[30+k]+0.5) ;


	parameter->w1max=Parameter[35] ; // һ�ŵ��ת�٣����٣�rpm ����
	parameter->w1min=Parameter[36] ; // һ�ŵ��ת�٣����٣�rpm ����
	parameter->w1p=Parameter[37] ; // һ�ŵ�������ٶ� rpm ����
	parameter->L1=Parameter[38] ; // һ�ŵ��������ʼ���� mm ����
	parameter->L2=Parameter[39] ; // һ�ŵ�����ٽ������� mm ����
	parameter->L3=Parameter[40] ; // һ�ŵ����ӡ��������ֹͣ���� mm ����
	parameter->H0=Parameter[41] ; // ��ӡƽ̨ȱʡ�߶� mm ���� ��ʼ������ͣ�߶�
	parameter->w2max=Parameter[42] ; // ���ŵ��ת�٣����٣�rpm ����
	parameter->w2min=Parameter[43] ; // ���ŵ��ת�٣����٣�rpm ����
	parameter->beta=Parameter[44] ; // ���ŵ��ת���� deg ����

	parameter->LDdelta=Parameter[45] ; // mm
	parameter->LD0=Parameter[46] ; // mm
	parameter->LNF=Parameter[47] ;
	parameter->LF=Parameter[48] ; // mm
	parameter->LENx=Parameter[49] ; // mm
	parameter->LENy=Parameter[50] ; // mm
	parameter->n=Parameter[51] ;

	for( k = 0 ; k < 4 ; k++ )// int ���ص�1�Ҷ�Pix1 0~100���ٷ�����
		parameter->grays[k]=(int)(Parameter[52+k]+0.5) ;


	// nt add 2017/6/1
	parameter->wp=(int)(Parameter[56]+0.5) ;

	// nt add 2017/8/20
	parameter->tmc=Parameter[57] ; // mm
	parameter->ratio=Parameter[58] ; // �޵�λ

	return n ;
}*/
{
	// ������������С���Χ�Ĳ��������˲�����������С���Χ��
	// ...
	int n = 0 ;

	int k=0;
	double Parameter[59];
	Parameter[0]=parameter->L ; // mm
	Parameter[1]=parameter->W ; // mm
	Parameter[2]=parameter->H ; // mm

								// �ֱ���
	Parameter[3]=parameter->PL ; // pixel
	Parameter[4]=parameter->PW ; // pixel

	Parameter[5]=parameter->P0 ;
	Parameter[6]=parameter->Dx ;

	// ������
	// ��һ����Ƭ���T0 ��m ����	
	// ��С���T1 12.5 ��m ����	
	// ���T2 25 ��m ����	
	// ���T3 50 ��m ����	
	// ���T4 100 ��m ����	
	// �����T5 200 ��m ���� �����5�����
	for( k = 0 ; k < 6 ; k++ )
		Parameter[7+k]=parameter->ts[k] ;

	// lis[i]��Ӧts[i], �ع��ǿ ���� ��Χ��0~100
	for( k = 0 ; k < 6 ; k++ )
		Parameter[13+k]=parameter->lis[k] ;

	// nt add 2017/6/27
	for( k = 0 ; k < 5 ; k++ )
		Parameter[19+k]=parameter->lils[k] ;

	// lts[i]��Ӧts[i], �ع�ʱ�� s ����
	for( k = 0 ; k < 6 ; k++ )
		Parameter[24+k]=parameter->lts[k] ;


	for( k = 0 ; k < 5 ; k++ )// nt add 2017/6/27
		Parameter[30+k]=parameter->ltls[k] ;


	Parameter[35]=parameter->w1max ; // һ�ŵ��ת�٣����٣�rpm ����
	Parameter[36]=parameter->w1min ; // һ�ŵ��ת�٣����٣�rpm ����
	Parameter[37]=parameter->w1p ; // һ�ŵ�������ٶ� rpm ����
	Parameter[38]=parameter->L1 ; // һ�ŵ��������ʼ���� mm ����
	Parameter[39]=parameter->L2 ; // һ�ŵ�����ٽ������� mm ����
	Parameter[40]=parameter->L3 ; // һ�ŵ����ӡ��������ֹͣ���� mm ����
	Parameter[41]=parameter->H0 ; // ��ӡƽ̨ȱʡ�߶� mm ���� ��ʼ������ͣ�߶�
	Parameter[42]=parameter->w2max ; // ���ŵ��ת�٣����٣�rpm ����
	Parameter[43]=parameter->w2min ; // ���ŵ��ת�٣����٣�rpm ����
	Parameter[44]=parameter->beta ; // ���ŵ��ת���� deg ����

	Parameter[45]=parameter->LDdelta ; // mm
	Parameter[46]=parameter->LD0 ; // mm
	Parameter[47]=parameter->LNF ;
	Parameter[48]=parameter->LF ; // mm
	Parameter[49]=parameter->LENx ; // mm
	Parameter[50]=parameter->LENy ; // mm
	Parameter[51]=parameter->n ;

	for( k = 0 ; k < 4 ; k++ )// int ���ص�1�Ҷ�Pix1 0~100���ٷ�����
		Parameter[52+k]=parameter->grays[k] ;


	// nt add 2017/6/1
	Parameter[56]=parameter->wp ;

	// nt add 2017/8/20
	Parameter[57]=parameter->tmc ; // mm
	Parameter[58]=parameter->ratio ; // �޵�λ

	int num[100];
	double min[100];//�洢��Χ�½������
	double max[100];//�洢��Χ�Ͻ������
	ifstream inFile;
	int count;

	CString filePath ;
	efpGet(filePath) ; // �����ļ�Ŀ¼
	filePath += _T("\\Libs\\������Χ.txt") ;
	//inFile.open("e:\\������Χ.txt");//�洢��Χ���ĵ���·��
	inFile.open(filePath);

	for (count = 0; count<59; count++)    //������ 
	{
		inFile >> num[count] >> min[count] >> max[count];
		if(count==40||count==41||count==50||count==51)
		{
			if(count==40||count==41||count==51)
			{
				min[count]=0;
				max[count]=Parameter[2];
			}

			if(count==50)
			{
				min[count]=-Parameter[2];
				max[count]=Parameter[2];
			}
		}
	}
	inFile.close();

	for(k=0;k<59;k++)
	{
		if(Parameter[k]<min[k]||Parameter[k]>max[k])
		{
			if(Parameter[k]<min[k])
			{
				Parameter[k]=min[k];
			}
			else
			{
				Parameter[k]=max[k];
			}
			n++;
		}
	}

	parameter->L=Parameter[0] ; // mm
	parameter->W=Parameter[1] ; // mm
	parameter->H=Parameter[2] ; // mm

								// �ֱ���
	parameter->PL=(int)(Parameter[3]+0.5) ; // pixel
	parameter->PW=(int)(Parameter[4]+0.5) ; // pixel

	parameter->P0=(int)(Parameter[5]+0.5) ;
	parameter->Dx=(int)(Parameter[6]+0.5) ;

	// ������
	// ��һ����Ƭ���T0 ��m ����	
	// ��С���T1 12.5 ��m ����	
	// ���T2 25 ��m ����	
	// ���T3 50 ��m ����	
	// ���T4 100 ��m ����	
	// �����T5 200 ��m ���� �����5�����
	for( k = 0 ; k < 6 ; k++ )
		parameter->ts[k]=Parameter[7+k] ;

	// lis[i]��Ӧts[i], �ع��ǿ ���� ��Χ��0~100
	for( k = 0 ; k < 6 ; k++ )
		parameter->lis[k]=(int)(Parameter[13+k]+0.5) ;

	// nt add 2017/6/27
	for( k = 0 ; k < 5 ; k++ )
		parameter->lils[k]=(int)(Parameter[19+k]+0.5) ;

	// lts[i]��Ӧts[i], �ع�ʱ�� s ����
	for( k = 0 ; k < 6 ; k++ )
		parameter->lts[k]=Parameter[24+k] ;


	for( k = 0 ; k < 5 ; k++ )// nt add 2017/6/27
		parameter->ltls[k]=Parameter[30+k] ;


	parameter->w1max=Parameter[35] ; // һ�ŵ��ת�٣����٣�rpm ����
	parameter->w1min=Parameter[36] ; // һ�ŵ��ת�٣����٣�rpm ����
	parameter->w1p=Parameter[37] ; // һ�ŵ�������ٶ� rpm ����
	parameter->L1=Parameter[38] ; // һ�ŵ��������ʼ���� mm ����
	parameter->L2=Parameter[39] ; // һ�ŵ�����ٽ������� mm ����
	parameter->L3=Parameter[40] ; // һ�ŵ����ӡ��������ֹͣ���� mm ����
	parameter->H0=Parameter[41] ; // ��ӡƽ̨ȱʡ�߶� mm ���� ��ʼ������ͣ�߶�
	parameter->w2max=Parameter[42] ; // ���ŵ��ת�٣����٣�rpm ����
	parameter->w2min=Parameter[43] ; // ���ŵ��ת�٣����٣�rpm ����
	parameter->beta=Parameter[44] ; // ���ŵ��ת���� deg ����

	parameter->LDdelta=Parameter[45] ; // mm
	parameter->LD0=Parameter[46] ; // mm
	parameter->LNF=Parameter[47] ;
	parameter->LF=Parameter[48] ; // mm
	parameter->LENx=Parameter[49] ; // mm
	parameter->LENy=Parameter[50] ; // mm
	parameter->n=Parameter[51] ;

	for( k = 0 ; k < 4 ; k++ )// int ���ص�1�Ҷ�Pix1 0~100���ٷ�����
		parameter->grays[k]=(int)(Parameter[52+k]+0.5) ;


	// nt add 2017/6/1
	parameter->wp=(int)(Parameter[56]+0.5) ;

	// nt add 2017/8/20
	parameter->tmc=Parameter[57] ; // mm
	parameter->ratio=Parameter[58] ; // �޵�λ


	return n ;
}

int parameterLoad(PARAMETER* parameter, CString& fileName)
{
	CFile file ;

	if( file.Open(fileName, CFile::CFile::modeRead) )
	{
		CArchive ar(&file, CArchive::load) ;
		ar.Read(parameter, sizeof(PARAMETER)) ;

		/*// �����ڶ��ɵĲ�����
		ar.Read(parameter, sizeof(PARAMETEROLD)) ;
		parameter->tmc = 10. ;
		parameter->ratio = 7.5 ;
		for( int k = 0 ; k < 5 ; k++ )
		{
			parameter->tps[k] = 0. ;
			parameter->tis[k] = 0 ;
		}
		// end*/

		ar.Close() ;
		file.Close() ;
		return 1 ;
	}
	else
		return 0 ;
}

int parameterSave(PARAMETER* parameter, CString& fileName)
{
	CFile file ;

	if( file.Open(fileName, CFile::modeCreate | CFile::modeWrite) )
	{
		CArchive ar(&file, CArchive::store) ;
		ar.Write(parameter, sizeof(PARAMETER)) ;
		ar.Close() ;
		file.Close() ;
		return 1 ;
	}
	else
		return 0 ;
}


////////////////////////////////////////////////////////////////