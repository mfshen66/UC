#pragma once

////////////////////////////////////////////////////////////////
// ��λ������������
/*typedef struct _parameterold PARAMETEROLD ;
struct _parameterold
{
	TCHAR printer[64] ; // ��ӡ�����ƺ��ͺ�
	TCHAR material[64] ; // �������ƺ��ͺ�
	double L ; // ��ӡ����X��ߴ�L
	double W ; // ��ӡ����Y��ߴ�W
	double H ; // ��ӡ����Z��ߴ�H
	int PL ; // ��ӡX��ֱ���PL
	int PW ; // ��ӡY��ֱ���PW
	int P0 ; // ���Ͷ��ԭ�㶨��P0 0/1/2/3/4
	int Dx ; // ���Ͷ��X'�᷽����Dx 1/-1/2/-2
	// ��һ����Ƭ���T0 ��m ����	
	// ��С���T1 12.5 ��m ����	
	// ���T2 25 ��m ����	
	// ���T3 50 ��m ����	
	// ���T4 100 ��m ����	
	// �����T5 200 ��m ���� �����5�����
	double ts[6] ; // ������
	int lis[6] ; // lis[i]��Ӧts[i], �ع��ǿ ���� ��Χ��0~100
	double lts[6] ; // lts[i]��Ӧts[i], �ع�ʱ�� s ����
	double w1max ; // һ�ŵ��ת�٣����٣�rpm ����
	double w1min ; // һ�ŵ��ת�٣����٣�rpm ����
	double w1p ; // һ�ŵ�������ٶ� rpm ����
	double L1 ; // һ�ŵ��������ʼ���� mm ����
	double L2 ; // һ�ŵ�����ٽ������� mm ����
	double L3 ; // һ�ŵ����ӡ��������ֹͣ���� mm ����
	double H0 ; // ��ӡƽ̨ȱʡ�߶� mm ���� ��ʼ������ͣ�߶�
	double w2max ; // ���ŵ��ת�٣����٣�rpm ����
	double w2min ; // ���ŵ��ת�٣����٣�rpm ����
	double beta ; // ���ŵ��ת���� deg ����
	int grays[4] ; // int ���ص�1�Ҷ�Pix1 0~100���ٷ�����
	int wp ; // �Ҷ���С�ɴ���߶ȣ���m, ���ͣ�500��nt add 2017/6/1
	double tps[5] ; // double Ԥ������
} ;*/

typedef struct _parameterold PARAMETEROLD ;
struct _parameterold
{
	TCHAR printer[64] ; // ��ӡ�����ƺ��ͺ�
	TCHAR material[64] ; // �������ƺ��ͺ�
	double L ; // ��ӡ����X��ߴ�L
	double W ; // ��ӡ����Y��ߴ�W
	double H ; // ��ӡ����Z��ߴ�H
	int PL ; // ��ӡX��ֱ���PL
	int PW ; // ��ӡY��ֱ���PW
	int P0 ; // ���Ͷ��ԭ�㶨��P0 0/1/2/3/4
	int Dx ; // ���Ͷ��X'�᷽����Dx 1/-1/2/-2
	// ��һ����Ƭ���T0 ��m ����	
	// ��С���T1 12.5 ��m ����	
	// ���T2 25 ��m ����	
	// ���T3 50 ��m ����	
	// ���T4 100 ��m ����	
	// �����T5 200 ��m ���� �����5�����
	double ts[6] ; // ������
	int lis[6] ; // lis[i]��Ӧts[i], �ع��ǿ ���� ��Χ��0~100
	int lils[5] ;
	double lts[6] ; // lts[i]��Ӧts[i], �ع�ʱ�� s ����
	double ltls[5] ;
	double w1max ; // һ�ŵ��ת�٣����٣�rpm ����
	double w1min ; // һ�ŵ��ת�٣����٣�rpm ����
	double w1p ; // һ�ŵ�������ٶ� rpm ����
	double L1 ; // һ�ŵ��������ʼ���� mm ����
	double L2 ; // һ�ŵ�����ٽ������� mm ����
	double L3 ; // һ�ŵ����ӡ��������ֹͣ���� mm ����
	double H0 ; // ��ӡƽ̨ȱʡ�߶� mm ���� ��ʼ������ͣ�߶�
	double w2max ; // ���ŵ��ת�٣����٣�rpm ����
	double w2min ; // ���ŵ��ת�٣����٣�rpm ����
	double beta ; // ���ŵ��ת���� deg ����
	int grays[4] ; // int ���ص�1�Ҷ�Pix1 0~100���ٷ�����
	int wp ; // �Ҷ���С�ɴ���߶ȣ���m, ���ͣ�500��nt add 2017/6/1
	double LDdelta ; // mm
	double LD0 ; // mm
	double LNF ;
	double LF ; // mm
	double LENx ; // mm
	double LENy ; // mm
	double n ;
	double tps[5] ; // double Ԥ������
} ;

typedef struct _parameter PARAMETER ;
struct _parameter
{
	TCHAR printer[64] ; // ��ӡ�����ƺ��ͺ�
	TCHAR material[64] ; // �������ƺ��ͺ�
	double L ; // ��ӡ����X��ߴ�L
	double W ; // ��ӡ����Y��ߴ�W
	double H ; // ��ӡ����Z��ߴ�H
	int PL ; // ��ӡX��ֱ���PL
	int PW ; // ��ӡY��ֱ���PW
	int P0 ; // ���Ͷ��ԭ�㶨��P0 0/1/2/3/4
	int Dx ; // ���Ͷ��X'�᷽����Dx 1/-1/2/-2
	// ��һ����Ƭ���T0 ��m ����	
	// ��С���T1 12.5 ��m ����	
	// ���T2 25 ��m ����	
	// ���T3 50 ��m ����	
	// ���T4 100 ��m ����	
	// �����T5 200 ��m ���� �����5�����
	double ts[6] ; // ������
	int lis[6] ; // lis[i]��Ӧts[i], �ع��ǿ ���� ��Χ��0~100
	int lils[5] ; // Tiһ���ع��ǿLIiL, i=1,2,3,4,5
	double lts[6] ; // lts[i]��Ӧts[i], �ع�ʱ�� s ����
	double ltls[5] ;
	double w1max ; // һ�ŵ��ת�٣����٣�rpm ����
	double w1min ; // һ�ŵ��ת�٣����٣�rpm ����
	double w1p ; // һ�ŵ�������ٶ� rpm ����
	double L1 ; // һ�ŵ��������ʼ���� mm ����
	double L2 ; // һ�ŵ�����ٽ������� mm ����
	double L3 ; // һ�ŵ����ӡ��������ֹͣ���� mm ����
	double H0 ; // ��ӡƽ̨ȱʡ�߶� mm ���� ��ʼ������ͣ�߶�
	double w2max ; // �Ϻ�ת�٣����٣�rpm ����
	double w2min ; // �Ϻ�ת�٣����٣�rpm ����
	double beta ; // ���ŵ��ת���� deg ����
	int grays[4] ; // int ���ص�1�Ҷ�Pix1 0~100���ٷ�����
	int wp ; // �Ҷ���С�ɴ���߶ȣ���m, ���ͣ�500��nt add 2017/6/1
	double LDdelta ; // mm ��е���
	double LD0 ; // mm ��դ�ߵ�ǰλ��
	double LNF ; // �������ϵ��
	double LF ; // mm Ͷ�����LF
	double LENx ; // mm �����X���LENx
	double LENy ; // mm �����Y���LENy
	double n ; // �Ϻ�������n
	double tmc ; // �Ϻк��TMC nt add 2017/8/19
	double ratio ; // �Ϻг��ּ��ٱ� nt add 2017/8/19
	double tps[5] ; // double Ԥ������
	int tis[5] ; // int Ԥ������ nt add 2017/8/20
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