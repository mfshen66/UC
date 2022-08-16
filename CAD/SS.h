// 2017/3/23 nt
// support structure for STL model
#ifndef _SS_h_
#define _SS_h_

////////////////////////////////////////////////////////////////
typedef struct _base BASE ; // ����
struct _base
{
	BOX3D box ;
	double a ; // б�ȣ���λ=����
	double d ; // ���쳤��
	FACET* facet ;
} ;

BASE* baseCreate(BOX3D* box, double a, double d) ;
BASE* baseCreate2() ; // nt add 2017/9/1
void baseFree(BASE* base) ;
BASE* baseCopy(BASE* base) ; // nt add 2017/9/4
int baseMake(BASE* base) ;
void baseDraw(BASE* base, void* pVI) ;
int baseSerialize(BASE* base, CArchive& ar) ; // nt add 2017/8/31
//--------------------------------------------------------------
#define SUPPORT_MAXNUMFACET 6
typedef struct _support SUPPORT ; // ֧��
struct _support
{
	SUPPORT* next ;
	int id ; // ����֧�ŵı༭
	int del ; // ����֧�ŵı༭
	int flag ; // 0=�ֶ����ɵģ�1=�Զ����ɵ�
	int no ; // nt add 2017/8/31 ���
	PNT3D p ; // �Ӵ���
	VEC3D normal ;
	double h ; // �����ĸ߶�
	double r1 ;
	double h1 ;
	double r2 ;
	double h2 ;
	double r ;
	double d ;
	double z ; // nt add 2017/8/30
	FACET* facets[SUPPORT_MAXNUMFACET] ;
} ;

SUPPORT* supportCreate(PNT3D p, VEC3D normal, double h, double r1, double r2, double r, double d) ;
SUPPORT* supportCreate2() ; // nt add 2017/6/4
void supportFree(SUPPORT* support) ;
SUPPORT* supportCopy(SUPPORT* support) ; // nt add 2017/9/4
int supportGetId(SUPPORT* support) ;
void supportSetId(SUPPORT* support, int id) ;
void supportMarkDel(SUPPORT* support) ;
int supportIsDel(SUPPORT* support) ;
int supportIsRaw(SUPPORT* support) ;
int supportMake(SUPPORT* support, int nu, int nv) ;
int supportMake2(SUPPORT* support, double z, VEC3D normal, int nu, int nv) ;
int supportMake3(SUPPORT* support, double R, double H, double A, int nu, int nv) ; // nt add 2017/6/9
void supportDraw(SUPPORT* support, void* pVI) ;
void supportDraw2(SUPPORT* support, void* pVI) ; // nt add 2017/6/4
int supportSerialize(SUPPORT* support, CArchive& ar) ; // nt add 2017/8/31
//--------------------------------------------------------------
// nt add 2017/6/4
typedef struct _rod ROD ; // ���
struct _rod
{
	ROD* next ;
	int del ;
	SUPPORT* support1 ;
	SUPPORT* support2 ;
	PNT3D p1 ;
	PNT3D p2 ;
	double r ;
	FACET* facet ;
} ;

ROD* rodCreate(SUPPORT* support1, SUPPORT* support2, PNT3D p1, PNT3D p2, double r) ;
ROD* rodCreate2() ;
void rodFree(ROD* rod) ;
ROD* rodCopy(ROD* rod) ; // nt add 2017/9/4
int rodIsDel(ROD* rod) ;
void rodDraw(ROD* rod, void* pVI) ;
void rodDraw2(ROD* rod, void* pVI) ;
int rodSerialize(ROD* rod, CArchive& ar, SUPPORT** array) ; // nt add 2017/8/31
//--------------------------------------------------------------
typedef struct _zp ZP ; // z-value point, nt add 2017/7/12
struct _zp
{
	ZP* next ;
	double z ;
	STLTRI* tri ;
} ;

typedef struct _zm ZM ; // z-values matrix, nt modify 2017/7/12
struct _zm
{
	double xmin ;
	double ymin ;
	double wx ;
	double wy ;
	int nx ; // i = 0,...,nx-1
	int ny ; // j = 0,...,ny-1
	ZP** zs ; // zs[j*nx+i]
} ;

ZM* zmCreate(BOX3D* box, double w) ;
void zmFree(ZM* zm) ;
int zmIntSTL(ZM* zm, STL* stl, double tol) ;
//--------------------------------------------------------------
//	֧�Žṹ����
typedef struct _ssp SSP ; // support structure parameters
struct _ssp
{
	double a ; // ����б��, ��λ�Ƕ�(2017-5-11��)��һ��ȡ45�ȣ�@������нǶ�
	double h ; // �����߶ȣ�@������и߶ȣ���λ��mm��
	double w ; // ���
	double r ; // ֧�ŵ�뾶��@֧�Ŷ���׶�Ӵ���뾶����λ��mm��
	double d ; // ��֧�ŵ��ط�ʸ���쳤�ȣ�@֧�Ŷ���׶�߶ȣ���λ��mm��
	double r1 ; // ֧�ŵײ�׶��뾶
	double r2 ; // ֧��Բ���뾶��@֧�Ű뾶����λ��mm��
	double b ; // ��������ǶȲ���֧��, ��λ�Ƕ�(2017-5-11��)
	double tol ; // �ݲ�
	// nt add 2017/5/11
	double hPart ; // @������е��浽�������߶ȣ����룬��λ��mm��
	int sDensity ; // @֧���ܶ�:50-150, 50��Ӧ2*w��150��Ӧ0.5*w
	double R ; // @֧�ŵ��а뾶����λ��mm������֧��û���䵽�����ϣ�
	double H ; // @֧�ŵ��и߶ȣ���λ��mm������֧��û���䵽�����ϣ�
	double A ; // @֧�ŵ�����б�ǣ���λ��deg��45��~90�㣩����֧��û���䵽�����ϣ�
	double coneA ; // ֧�Ŷ���׶�Ƕȣ���λ��deg��
	double minH ; // ֧���޺���������С�߶ȣ���λ��mm��
	double ts[5] ; // Ԥ������
} ;

void sspInit(SSP* ssp, double a, double h, double w, double r, double d, double r1, double r2, double b, double tol) ;
//--------------------------------------------------------------
typedef struct _ss SS ;
struct _ss
{
	int editing ; // 1=���ڱ༭
	BASE* base ; // ����Ϊ��
	SUPPORT* supports ;
	ROD* rods ; // nt add 2017/6/4
	BOX3D box ;
} ;

SS* ssCreate() ;
void ssFree(SS* ss) ;
SS* ssCopy(SS* ss) ; // nt add 2017/9/4
void ssReleaseAuto(SS* ss, int base) ; // nt add 2017/8/22
BASE* ssReleaseAuto2(SS* ss, int base, CSet& delSupports, CSet& delRods) ; // nt add 2017/9/4
void ssBeginEdit(SS* ss) ;
void ssBeginEditRod(SS* ss) ; // nt add 2017/6/7
void ssEndEdit(SS* ss) ;
int ssIsEditing(SS* ss) ;
int ssGetNumOfSupport(SS* ss) ;
SUPPORT* ssGetSupport(SS* ss, int i) ;
SUPPORT* ssGetSupportById(SS* ss, int id) ;
int ssHasValidSupport(SS* ss) ; // nt add 2017/10/1
int ssMakeAtSharpPts(SS* ss, STL* stl, SSP* ssp) ; // nt add 2017/7/11
int ssMake(SS* ss, STL* stl, SSP* ssp, PRG* pPrg) ; // nt add 2017/9/10
void ssAddSupport(SS* ss, SUPPORT* support) ;
int ssDelSupport(SS* ss, SUPPORT* support) ; // ����ss��ɾ��support, δ�ͷ���ռ�
int ssRemoveSupport(SS* ss, SUPPORT* support) ; // nt add 2017/9/4
void ssAddRod(SS* ss, ROD* rod) ; // nt add 2017/6/4
int ssDelRod(SS* ss, ROD* rod) ; // nt add 2017/6/4
int ssDelRod2(SS* ss, SUPPORT* support) ; // nt add 2017/6/4
int ssGetNumOfRod(SS* ss) ; // nt add 2017/9/1
int ssCalBox(SS* ss) ; // nt add 2017/9/30
void ssMove(SS* ss, VEC3D v) ; // nt add 2017/7/3
void ssRotate(SS* ss, PNT3D pivot, VEC3D axis, double a) ; // nt add 2017/7/3
void ssDraw(SS* ss, void* pVI) ;
SUPPORT* ssPickSupport(SS* ss, PNT3D pos, VEC3D dir, double r) ;
SUPPORT* ssPickSupport2(SS* ss, PNT3D pos, VEC3D dir, double r, double z, PNT3D p) ; // nt add 2017/6/5
ROD* ssPickRod(SS* ss, PNT3D pos, VEC3D dir, double r, double z) ;
int ssMerge(SS* ss, SS* merged, STL* stl, SSP* ssp) ; // nt add 2017/8/24
int ssRebuild(SS* ss, STL* stl, SSP* ssp) ;
int ssRebuildEx(SS* ss, STL* stl, SSP* ssp) ; // nt add 2017/9/4
int ssRebuild2(SS* ss) ; // nt add 2017/6/8
int ssSerialize(SS* ss, CArchive& ar) ; // nt add 2017/8/31
////////////////////////////////////////////////////////////////

#endif