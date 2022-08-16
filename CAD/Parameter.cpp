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

	// 打印机名称含型号
	_tcscpy_s(parameter->printer, 64, _T("SWCD-50-64X40")) ;
	
	// 材料名称含型号
	_tcscpy_s(parameter->material, 64, _T("Al2O3-01")) ;

	// x、y、z方向上的工作台的范围
	parameter->L = 256. ; // mm
	parameter->W = 160. ; // mm
	parameter->H = 200. ; // mm

	// 分辨率
	parameter->PL = 1280 ; // pixel
	parameter->PW = 800 ; // pixel

	parameter->P0 = 0 ;
	parameter->Dx = 1 ;

	// 层厚参数
	// 第一层切片厚度T0 μm 浮点	
	// 最小层厚T1 12.5 μm 浮点	
	// 层厚T2 25 μm 浮点	
	// 层厚T3 50 μm 浮点	
	// 层厚T4 100 μm 浮点	
	// 最大层厚T5 200 μm 浮点 最多有5个厚度
	parameter->ts[0] = 20. ;
	parameter->ts[1] = 12.5 ;
	parameter->ts[2] = 25. ;
	parameter->ts[3] = 50. ;
	parameter->ts[4] = 100. ;
	parameter->ts[5] = 200. ;

	// lis[i]对应ts[i], 曝光光强 整型 范围：0~100
	for( k = 0 ; k < 6 ; k++ )
		parameter->lis[k] = 90 ;

	// nt add 2017/6/27
	for( k = 0 ; k < 5 ; k++ )
		parameter->lils[k] = 90 ;

	// lts[i]对应ts[i], 曝光时间 s 浮点
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

	parameter->w1max = 60. ; // 一号电机转速（高速）rpm 浮点
	parameter->w1min = 1. ; // 一号电机转速（低速）rpm 浮点
	parameter->w1p = 1. ; // 一号电机拔起速度 rpm 浮点
	parameter->L1 = 1. ; // 一号电机变速起始距离 mm 浮点
	parameter->L2 = 0.05 ; // 一号电机变速结束距离 mm 浮点
	parameter->L3 = 3. ; // 一号电机打印过程上升停止距离 mm 浮点
	parameter->H0 = 150. ; // 打印平台缺省高度 mm 浮点 初始化、暂停高度
	parameter->w2max = 100. ; // 二号电机转速（高速）rpm 浮点
	parameter->w2min = 5. ; // 二号电机转速（低速）rpm 浮点
	parameter->beta = 450. ; // 二号电机转动角 deg 浮点

	parameter->LDdelta = 10. ; // mm
	parameter->LD0 = 100. ; // mm
	parameter->LNF = 0.9 ;
	parameter->LF = 112. ; // mm
	parameter->LENx = 8. ; // mm
	parameter->LENy = 4.5 ; // mm
	parameter->n = 1.5 ;

	// int 像素点1灰度Pix1 0~100（百分数）
	parameter->grays[0] = 100 ;
	parameter->grays[1] = 80 ;
	parameter->grays[2] = 60 ;
	parameter->grays[3] = 40 ;

	// nt add 2017/6/1
	parameter->wp = 500 ;

	// nt add 2017/8/20
	parameter->tmc = 10. ; // mm
	parameter->ratio = 7.5 ; // 无单位

	// double 预留待定
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

	// 像素矩形x向边长为dx, y向边长为dy, 一般dx=dy
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
// 用参数的最小最大范围限制参数
// 返回-1=出错
// 返回0=没有发生超出最小最大范围的情况
// 返回n=发生了n次超出最小最大范围的情况,n>0
int parameterResByLimits(PARAMETER* parameter)
/*{
	// 遍历所有有最小最大范围的参数，将此参数限制在最小最大范围内
	// ...
	int k, n = 0 ;

	double Parameter[59];
	Parameter[0]=parameter->L ; // mm
	Parameter[1]=parameter->W ; // mm
	Parameter[2]=parameter->H ; // mm

								// 分辨率
	Parameter[3]=parameter->PL ; // pixel
	Parameter[4]=parameter->PW ; // pixel

	Parameter[5]=parameter->P0 ;
	Parameter[6]=parameter->Dx ;

	// 层厚参数
	// 第一层切片厚度T0 μm 浮点	
	// 最小层厚T1 12.5 μm 浮点	
	// 层厚T2 25 μm 浮点	
	// 层厚T3 50 μm 浮点	
	// 层厚T4 100 μm 浮点	
	// 最大层厚T5 200 μm 浮点 最多有5个厚度
	for( k = 0 ; k < 6 ; k++ )
		Parameter[7+k]=parameter->ts[k] ;

	// lis[i]对应ts[i], 曝光光强 整型 范围：0~100
	for( k = 0 ; k < 6 ; k++ )
		Parameter[13+k]=parameter->lis[k] ;

	// nt add 2017/6/27
	for( k = 0 ; k < 5 ; k++ )
		Parameter[19+k]=parameter->lils[k] ;

	// lts[i]对应ts[i], 曝光时间 s 浮点
	for( k = 0 ; k < 6 ; k++ )
		Parameter[24+k]=parameter->lts[k] ;


	for( k = 0 ; k < 5 ; k++ )// nt add 2017/6/27
		Parameter[30+k]=parameter->ltls[k] ;


	Parameter[35]=parameter->w1max ; // 一号电机转速（高速）rpm 浮点
	Parameter[36]=parameter->w1min ; // 一号电机转速（低速）rpm 浮点
	Parameter[37]=parameter->w1p ; // 一号电机拔起速度 rpm 浮点
	Parameter[38]=parameter->L1 ; // 一号电机变速起始距离 mm 浮点
	Parameter[39]=parameter->L2 ; // 一号电机变速结束距离 mm 浮点
	Parameter[40]=parameter->L3 ; // 一号电机打印过程上升停止距离 mm 浮点
	Parameter[41]=parameter->H0 ; // 打印平台缺省高度 mm 浮点 初始化、暂停高度
	Parameter[42]=parameter->w2max ; // 二号电机转速（高速）rpm 浮点
	Parameter[43]=parameter->w2min ; // 二号电机转速（低速）rpm 浮点
	Parameter[44]=parameter->beta ; // 二号电机转动角 deg 浮点

	Parameter[45]=parameter->LDdelta ; // mm
	Parameter[46]=parameter->LD0 ; // mm
	Parameter[47]=parameter->LNF ;
	Parameter[48]=parameter->LF ; // mm
	Parameter[49]=parameter->LENx ; // mm
	Parameter[50]=parameter->LENy ; // mm
	Parameter[51]=parameter->n ;

	for( k = 0 ; k < 4 ; k++ )// int 像素点1灰度Pix1 0~100（百分数）
		Parameter[52+k]=parameter->grays[k] ;


	// nt add 2017/6/1
	Parameter[56]=parameter->wp ;

	// nt add 2017/8/20
	Parameter[57]=parameter->tmc ; // mm
	Parameter[58]=parameter->ratio ; // 无单位

	int num[100];
	double min[100];//存储范围下界的数组
	double max[100];//存储范围上界的数组
	ifstream inFile;
	int count;
	CString filePath ;
	efpGet(filePath) ; // 运行文件目录
	filePath += _T("\\Libs\\参数范围.txt") ;
	//inFile.open("e:\\参数范围.txt");//存储范围的文档的路径
	inFile.open(filePath);

	for (count = 0; count<59; count++)    //读数据 
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

								// 分辨率
	parameter->PL=(int)(Parameter[3]+0.5) ; // pixel
	parameter->PW=(int)(Parameter[4]+0.5) ; // pixel

	parameter->P0=(int)(Parameter[5]+0.5) ;
	parameter->Dx=(int)(Parameter[6]+0.5) ;

	// 层厚参数
	// 第一层切片厚度T0 μm 浮点	
	// 最小层厚T1 12.5 μm 浮点	
	// 层厚T2 25 μm 浮点	
	// 层厚T3 50 μm 浮点	
	// 层厚T4 100 μm 浮点	
	// 最大层厚T5 200 μm 浮点 最多有5个厚度
	for( k = 0 ; k < 6 ; k++ )
		parameter->ts[0]=Parameter[7+k] ;

	// lis[i]对应ts[i], 曝光光强 整型 范围：0~100
	for( k = 0 ; k < 6 ; k++ )
		parameter->lis[k]=(int)(Parameter[13+k]+0.5) ;

	// nt add 2017/6/27
	for( k = 0 ; k < 5 ; k++ )
		parameter->lils[k]=(int)(Parameter[19+k]+0.5) ;

	// lts[i]对应ts[i], 曝光时间 s 浮点
	for( k = 0 ; k < 6 ; k++ )
		parameter->lts[0]=(int)(Parameter[24+k]+0.5) ;


	for( k = 0 ; k < 5 ; k++ )// nt add 2017/6/27
		parameter->ltls[0]=(int)(Parameter[30+k]+0.5) ;


	parameter->w1max=Parameter[35] ; // 一号电机转速（高速）rpm 浮点
	parameter->w1min=Parameter[36] ; // 一号电机转速（低速）rpm 浮点
	parameter->w1p=Parameter[37] ; // 一号电机拔起速度 rpm 浮点
	parameter->L1=Parameter[38] ; // 一号电机变速起始距离 mm 浮点
	parameter->L2=Parameter[39] ; // 一号电机变速结束距离 mm 浮点
	parameter->L3=Parameter[40] ; // 一号电机打印过程上升停止距离 mm 浮点
	parameter->H0=Parameter[41] ; // 打印平台缺省高度 mm 浮点 初始化、暂停高度
	parameter->w2max=Parameter[42] ; // 二号电机转速（高速）rpm 浮点
	parameter->w2min=Parameter[43] ; // 二号电机转速（低速）rpm 浮点
	parameter->beta=Parameter[44] ; // 二号电机转动角 deg 浮点

	parameter->LDdelta=Parameter[45] ; // mm
	parameter->LD0=Parameter[46] ; // mm
	parameter->LNF=Parameter[47] ;
	parameter->LF=Parameter[48] ; // mm
	parameter->LENx=Parameter[49] ; // mm
	parameter->LENy=Parameter[50] ; // mm
	parameter->n=Parameter[51] ;

	for( k = 0 ; k < 4 ; k++ )// int 像素点1灰度Pix1 0~100（百分数）
		parameter->grays[k]=(int)(Parameter[52+k]+0.5) ;


	// nt add 2017/6/1
	parameter->wp=(int)(Parameter[56]+0.5) ;

	// nt add 2017/8/20
	parameter->tmc=Parameter[57] ; // mm
	parameter->ratio=Parameter[58] ; // 无单位

	return n ;
}*/
{
	// 遍历所有有最小最大范围的参数，将此参数限制在最小最大范围内
	// ...
	int n = 0 ;

	int k=0;
	double Parameter[59];
	Parameter[0]=parameter->L ; // mm
	Parameter[1]=parameter->W ; // mm
	Parameter[2]=parameter->H ; // mm

								// 分辨率
	Parameter[3]=parameter->PL ; // pixel
	Parameter[4]=parameter->PW ; // pixel

	Parameter[5]=parameter->P0 ;
	Parameter[6]=parameter->Dx ;

	// 层厚参数
	// 第一层切片厚度T0 μm 浮点	
	// 最小层厚T1 12.5 μm 浮点	
	// 层厚T2 25 μm 浮点	
	// 层厚T3 50 μm 浮点	
	// 层厚T4 100 μm 浮点	
	// 最大层厚T5 200 μm 浮点 最多有5个厚度
	for( k = 0 ; k < 6 ; k++ )
		Parameter[7+k]=parameter->ts[k] ;

	// lis[i]对应ts[i], 曝光光强 整型 范围：0~100
	for( k = 0 ; k < 6 ; k++ )
		Parameter[13+k]=parameter->lis[k] ;

	// nt add 2017/6/27
	for( k = 0 ; k < 5 ; k++ )
		Parameter[19+k]=parameter->lils[k] ;

	// lts[i]对应ts[i], 曝光时间 s 浮点
	for( k = 0 ; k < 6 ; k++ )
		Parameter[24+k]=parameter->lts[k] ;


	for( k = 0 ; k < 5 ; k++ )// nt add 2017/6/27
		Parameter[30+k]=parameter->ltls[k] ;


	Parameter[35]=parameter->w1max ; // 一号电机转速（高速）rpm 浮点
	Parameter[36]=parameter->w1min ; // 一号电机转速（低速）rpm 浮点
	Parameter[37]=parameter->w1p ; // 一号电机拔起速度 rpm 浮点
	Parameter[38]=parameter->L1 ; // 一号电机变速起始距离 mm 浮点
	Parameter[39]=parameter->L2 ; // 一号电机变速结束距离 mm 浮点
	Parameter[40]=parameter->L3 ; // 一号电机打印过程上升停止距离 mm 浮点
	Parameter[41]=parameter->H0 ; // 打印平台缺省高度 mm 浮点 初始化、暂停高度
	Parameter[42]=parameter->w2max ; // 二号电机转速（高速）rpm 浮点
	Parameter[43]=parameter->w2min ; // 二号电机转速（低速）rpm 浮点
	Parameter[44]=parameter->beta ; // 二号电机转动角 deg 浮点

	Parameter[45]=parameter->LDdelta ; // mm
	Parameter[46]=parameter->LD0 ; // mm
	Parameter[47]=parameter->LNF ;
	Parameter[48]=parameter->LF ; // mm
	Parameter[49]=parameter->LENx ; // mm
	Parameter[50]=parameter->LENy ; // mm
	Parameter[51]=parameter->n ;

	for( k = 0 ; k < 4 ; k++ )// int 像素点1灰度Pix1 0~100（百分数）
		Parameter[52+k]=parameter->grays[k] ;


	// nt add 2017/6/1
	Parameter[56]=parameter->wp ;

	// nt add 2017/8/20
	Parameter[57]=parameter->tmc ; // mm
	Parameter[58]=parameter->ratio ; // 无单位

	int num[100];
	double min[100];//存储范围下界的数组
	double max[100];//存储范围上界的数组
	ifstream inFile;
	int count;

	CString filePath ;
	efpGet(filePath) ; // 运行文件目录
	filePath += _T("\\Libs\\参数范围.txt") ;
	//inFile.open("e:\\参数范围.txt");//存储范围的文档的路径
	inFile.open(filePath);

	for (count = 0; count<59; count++)    //读数据 
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

								// 分辨率
	parameter->PL=(int)(Parameter[3]+0.5) ; // pixel
	parameter->PW=(int)(Parameter[4]+0.5) ; // pixel

	parameter->P0=(int)(Parameter[5]+0.5) ;
	parameter->Dx=(int)(Parameter[6]+0.5) ;

	// 层厚参数
	// 第一层切片厚度T0 μm 浮点	
	// 最小层厚T1 12.5 μm 浮点	
	// 层厚T2 25 μm 浮点	
	// 层厚T3 50 μm 浮点	
	// 层厚T4 100 μm 浮点	
	// 最大层厚T5 200 μm 浮点 最多有5个厚度
	for( k = 0 ; k < 6 ; k++ )
		parameter->ts[k]=Parameter[7+k] ;

	// lis[i]对应ts[i], 曝光光强 整型 范围：0~100
	for( k = 0 ; k < 6 ; k++ )
		parameter->lis[k]=(int)(Parameter[13+k]+0.5) ;

	// nt add 2017/6/27
	for( k = 0 ; k < 5 ; k++ )
		parameter->lils[k]=(int)(Parameter[19+k]+0.5) ;

	// lts[i]对应ts[i], 曝光时间 s 浮点
	for( k = 0 ; k < 6 ; k++ )
		parameter->lts[k]=Parameter[24+k] ;


	for( k = 0 ; k < 5 ; k++ )// nt add 2017/6/27
		parameter->ltls[k]=Parameter[30+k] ;


	parameter->w1max=Parameter[35] ; // 一号电机转速（高速）rpm 浮点
	parameter->w1min=Parameter[36] ; // 一号电机转速（低速）rpm 浮点
	parameter->w1p=Parameter[37] ; // 一号电机拔起速度 rpm 浮点
	parameter->L1=Parameter[38] ; // 一号电机变速起始距离 mm 浮点
	parameter->L2=Parameter[39] ; // 一号电机变速结束距离 mm 浮点
	parameter->L3=Parameter[40] ; // 一号电机打印过程上升停止距离 mm 浮点
	parameter->H0=Parameter[41] ; // 打印平台缺省高度 mm 浮点 初始化、暂停高度
	parameter->w2max=Parameter[42] ; // 二号电机转速（高速）rpm 浮点
	parameter->w2min=Parameter[43] ; // 二号电机转速（低速）rpm 浮点
	parameter->beta=Parameter[44] ; // 二号电机转动角 deg 浮点

	parameter->LDdelta=Parameter[45] ; // mm
	parameter->LD0=Parameter[46] ; // mm
	parameter->LNF=Parameter[47] ;
	parameter->LF=Parameter[48] ; // mm
	parameter->LENx=Parameter[49] ; // mm
	parameter->LENy=Parameter[50] ; // mm
	parameter->n=Parameter[51] ;

	for( k = 0 ; k < 4 ; k++ )// int 像素点1灰度Pix1 0~100（百分数）
		parameter->grays[k]=(int)(Parameter[52+k]+0.5) ;


	// nt add 2017/6/1
	parameter->wp=(int)(Parameter[56]+0.5) ;

	// nt add 2017/8/20
	parameter->tmc=Parameter[57] ; // mm
	parameter->ratio=Parameter[58] ; // 无单位


	return n ;
}

int parameterLoad(PARAMETER* parameter, CString& fileName)
{
	CFile file ;

	if( file.Open(fileName, CFile::CFile::modeRead) )
	{
		CArchive ar(&file, CArchive::load) ;
		ar.Read(parameter, sizeof(PARAMETER)) ;

		/*// 仅用于读旧的参数表
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