#pragma once

////////////////////////////////////////////////////////////////
// 上位机软件输入参数
/*typedef struct _parameterold PARAMETEROLD ;
struct _parameterold
{
	TCHAR printer[64] ; // 打印机名称含型号
	TCHAR material[64] ; // 材料名称含型号
	double L ; // 打印区域X向尺寸L
	double W ; // 打印区域Y向尺寸W
	double H ; // 打印区域Z向尺寸H
	int PL ; // 打印X向分辨率PL
	int PW ; // 打印Y向分辨率PW
	int P0 ; // 光机投射原点定义P0 0/1/2/3/4
	int Dx ; // 光机投射X'轴方向定义Dx 1/-1/2/-2
	// 第一层切片厚度T0 μm 浮点	
	// 最小层厚T1 12.5 μm 浮点	
	// 层厚T2 25 μm 浮点	
	// 层厚T3 50 μm 浮点	
	// 层厚T4 100 μm 浮点	
	// 最大层厚T5 200 μm 浮点 最多有5个厚度
	double ts[6] ; // 层厚参数
	int lis[6] ; // lis[i]对应ts[i], 曝光光强 整型 范围：0~100
	double lts[6] ; // lts[i]对应ts[i], 曝光时间 s 浮点
	double w1max ; // 一号电机转速（高速）rpm 浮点
	double w1min ; // 一号电机转速（低速）rpm 浮点
	double w1p ; // 一号电机拔起速度 rpm 浮点
	double L1 ; // 一号电机变速起始距离 mm 浮点
	double L2 ; // 一号电机变速结束距离 mm 浮点
	double L3 ; // 一号电机打印过程上升停止距离 mm 浮点
	double H0 ; // 打印平台缺省高度 mm 浮点 初始化、暂停高度
	double w2max ; // 二号电机转速（高速）rpm 浮点
	double w2min ; // 二号电机转速（低速）rpm 浮点
	double beta ; // 二号电机转动角 deg 浮点
	int grays[4] ; // int 像素点1灰度Pix1 0~100（百分数）
	int wp ; // 灰度最小可处理尺度，μm, 整型，500，nt add 2017/6/1
	double tps[5] ; // double 预留待定
} ;*/

typedef struct _parameterold PARAMETEROLD ;
struct _parameterold
{
	TCHAR printer[64] ; // 打印机名称含型号
	TCHAR material[64] ; // 材料名称含型号
	double L ; // 打印区域X向尺寸L
	double W ; // 打印区域Y向尺寸W
	double H ; // 打印区域Z向尺寸H
	int PL ; // 打印X向分辨率PL
	int PW ; // 打印Y向分辨率PW
	int P0 ; // 光机投射原点定义P0 0/1/2/3/4
	int Dx ; // 光机投射X'轴方向定义Dx 1/-1/2/-2
	// 第一层切片厚度T0 μm 浮点	
	// 最小层厚T1 12.5 μm 浮点	
	// 层厚T2 25 μm 浮点	
	// 层厚T3 50 μm 浮点	
	// 层厚T4 100 μm 浮点	
	// 最大层厚T5 200 μm 浮点 最多有5个厚度
	double ts[6] ; // 层厚参数
	int lis[6] ; // lis[i]对应ts[i], 曝光光强 整型 范围：0~100
	int lils[5] ;
	double lts[6] ; // lts[i]对应ts[i], 曝光时间 s 浮点
	double ltls[5] ;
	double w1max ; // 一号电机转速（高速）rpm 浮点
	double w1min ; // 一号电机转速（低速）rpm 浮点
	double w1p ; // 一号电机拔起速度 rpm 浮点
	double L1 ; // 一号电机变速起始距离 mm 浮点
	double L2 ; // 一号电机变速结束距离 mm 浮点
	double L3 ; // 一号电机打印过程上升停止距离 mm 浮点
	double H0 ; // 打印平台缺省高度 mm 浮点 初始化、暂停高度
	double w2max ; // 二号电机转速（高速）rpm 浮点
	double w2min ; // 二号电机转速（低速）rpm 浮点
	double beta ; // 二号电机转动角 deg 浮点
	int grays[4] ; // int 像素点1灰度Pix1 0~100（百分数）
	int wp ; // 灰度最小可处理尺度，μm, 整型，500，nt add 2017/6/1
	double LDdelta ; // mm
	double LD0 ; // mm
	double LNF ;
	double LF ; // mm
	double LENx ; // mm
	double LENy ; // mm
	double n ;
	double tps[5] ; // double 预留待定
} ;

typedef struct _parameter PARAMETER ;
struct _parameter
{
	TCHAR printer[64] ; // 打印机名称含型号
	TCHAR material[64] ; // 材料名称含型号
	double L ; // 打印区域X向尺寸L
	double W ; // 打印区域Y向尺寸W
	double H ; // 打印区域Z向尺寸H
	int PL ; // 打印X向分辨率PL
	int PW ; // 打印Y向分辨率PW
	int P0 ; // 光机投射原点定义P0 0/1/2/3/4
	int Dx ; // 光机投射X'轴方向定义Dx 1/-1/2/-2
	// 第一层切片厚度T0 μm 浮点	
	// 最小层厚T1 12.5 μm 浮点	
	// 层厚T2 25 μm 浮点	
	// 层厚T3 50 μm 浮点	
	// 层厚T4 100 μm 浮点	
	// 最大层厚T5 200 μm 浮点 最多有5个厚度
	double ts[6] ; // 层厚参数
	int lis[6] ; // lis[i]对应ts[i], 曝光光强 整型 范围：0~100
	int lils[5] ; // Ti一次曝光光强LIiL, i=1,2,3,4,5
	double lts[6] ; // lts[i]对应ts[i], 曝光时间 s 浮点
	double ltls[5] ;
	double w1max ; // 一号电机转速（高速）rpm 浮点
	double w1min ; // 一号电机转速（低速）rpm 浮点
	double w1p ; // 一号电机拔起速度 rpm 浮点
	double L1 ; // 一号电机变速起始距离 mm 浮点
	double L2 ; // 一号电机变速结束距离 mm 浮点
	double L3 ; // 一号电机打印过程上升停止距离 mm 浮点
	double H0 ; // 打印平台缺省高度 mm 浮点 初始化、暂停高度
	double w2max ; // 料盒转速（高速）rpm 浮点
	double w2min ; // 料盒转速（低速）rpm 浮点
	double beta ; // 二号电机转动角 deg 浮点
	int grays[4] ; // int 像素点1灰度Pix1 0~100（百分数）
	int wp ; // 灰度最小可处理尺度，μm, 整型，500，nt add 2017/6/1
	double LDdelta ; // mm 机械零点
	double LD0 ; // mm 光栅尺当前位置
	double LNF ; // 光机能量系数
	double LF ; // mm 投射距离LF
	double LENx ; // mm 出光口X宽度LENx
	double LENy ; // mm 出光口Y宽度LENy
	double n ; // 料盒折射率n
	double tmc ; // 料盒厚度TMC nt add 2017/8/19
	double ratio ; // 料盒齿轮减速比 nt add 2017/8/19
	double tps[5] ; // double 预留待定
	int tis[5] ; // int 预留待定 nt add 2017/8/20
} ;

PARAMETER* parameterCreate() ;
void parameterFree(PARAMETER* parameter) ;
void parameterInit(PARAMETER* parameter) ;
int parameter2PixelCoord(PARAMETER* parameter, double x, double y, int& i, int& j)  ;
int parameterMatch(PARAMETER* parameter, double h, int& li1, double& lt1, int& li2, double& lt2) ; // nt add 2017/6/3
int parameterResByLimits(PARAMETER* parameter) ; // nt add 2017/8/20
int parameterLoad(PARAMETER* parameter, CString& fileName) ;
int parameterSave(PARAMETER* parameter, CString& fileName) ;
////////////////////////////////////////////////////////////////