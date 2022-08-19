/*	GP_API.h
 *	2002/8/19 nt
 */
#ifndef _GP_API_h_
#define _GP_API_h_

#ifdef GP_EXPORTS
#define GP_API __declspec(dllexport)
#else
#define GP_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

///////////////////////////////////////////////////////////////
// (1) error code : only used as function directly return value
#define ERNOERR 100 // no error
#define ERSUCSS 1 // success
#define ERUNSUC 0 // error

#define ERMEM -1 // memoery error
#define ERINPUT -2 // input error
#define ERDEVGN -3 // devergent
#define ERDEGE -4 // degenerative
#define ERSINGU -5 // singular: the case which is unablbe to handle
#define ERNOTON -6 // not on the curve or surface
//-------------------------------------------------------------
// (2) identification code
#define IDSOLU0	0 // 0 solution
#define IDSOLU1	1 // 1 solution
#define IDSOLU2	2 // 2 solution
#define IDSOLU3	3 // 3 solution
#define IDSOLU4	4 // 4 solution
#define IDINF 100 // infinite

#define IDNOINT 0 // no intersect
#define IDINT 1 // intersect
#define IDPARAL 2 // parallel
#define IDTAN 3 // tangent
#define IDOVERL 4 // overlap
#define IDNOPLN 5 // not on same plane
#define IDPERPN	6 // perpendicular
#define IDNOOVR 7 // not overlap

#define IDUNSYM 0 // unsymmetric
#define IDSYM 1 // symmetric

#define IDHOR 0 // horizental
#define IDVER 1 // vertical

#define IDNONE 0 // none
#define IDUNIQ 1 // unique
#define IDMULTI 2 // multiple

#define IDPERIO 0 // periodic
#define IDCLOSED 1 // close
#define IDOPEN 2 // open

#define IDBEGIN 0 // begin
#define IDEND 1 // end

#define IDCW 0 // clockwise
#define IDACW 1 // anti-clockwise

#define IDOUT 0 // out
#define IDIN 1 // in
#define IDON 2 // boundary
#define IDLEFT 3 // left 
#define IDRIGHT 4 // right

#define IDDEVGN	0 // devergent
#define IDCNVGN	1 // convergente

#define IDDIFF 0 // differant
#define IDSAME 1 // same

#define IDU	0 // u
#define IDV	1 // v

#define IDUNFND	0 // unfind
#define IDFOUND	1 // found

#define IDMINOR	0 // minor
#define IDMAJOR	1 // major

#define IDAPPX1	1 // approximate by 1 deg
#define IDAPPX2	2 // approximate by 2 deg 
#define IDAPPX3	3 // approximate by 3 deg

#define IDINVISI 0 // invisible
#define IDVISI 1 // visible

#define IDG0 0 // G0
#define IDG1 1 // G1
#define IDG2 2 // G2
#define IDGF 120 // free

#define ID_NO 0 // no
#define ID_YES 1 // yes

#define IDREAD 0 // read from file
#define IDWRITE 1 // write to file

#define IDWIRE 0
#define IDHDLINE 1
#define IDINGRAY 2
#define IDSHADE 3

#define IDNORMAL 0
#define IDPROMPT 1
#define IDSELECT 2
#define IDACTIVE 3
#define IDERAZE 4

#define IDAPPX 0
#define IDEXACT 1

#define IDDEGE 800 // degenarative
#define IDREGU 801 // regular

#define IDINVAL	802 // invalid
#define IDVALID	803 // valid
//-------------------------------------------------------------
// (3) functions : error handle
GP_API int envGetError() ; // get last error
GP_API void envSetError(int error) ; // set last error
//-------------------------------------------------------------
// (4) functions : allocate and free memory
GP_API void envGetMemInfo(int memInfo[3]) ; // get memory info
GP_API void* envAlloc(int size) ; // allocate memory
GP_API void envFree(void* mem) ; // free memory
//-------------------------------------------------------------
//	(5) draw
/*	2001/11/17 nt
 *	draw function type
 */
enum EDrawFunType
{
	dfPt = 0, // ��
	dfLin = 1, // �߶�
	dfPoly = 2, // ����
	dfTris = 3, // ���������
	dfQuas = 4 // ����ı���
} ;
GP_API int regDrawPt(int (*drawPt)(double p[3], int color, double size)) ;
GP_API int regDrawLin(int (*drawLin)(double p1[3], double p2[3], int color, double width)) ;
GP_API int regDrawPoly(int (*drawPoly)(int np, double (*ps)[3], int color, double width)) ;
GP_API int regDrawTris(int (*drawTris)(int np, double (*ps)[3], double (*ns)[3], int skipsize, int nt, int (*ts)[3], int color)) ;
GP_API int regDrawQuas(int (*drawQuas)(int np, double (*ps)[3], double (*ns)[3], int skipsize, int nq, int (*qs)[4], int color)) ;
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// 2022/08/16 add: MAX_FLT
#define MAX_FLT 10e+6
//	(1) macro
// nt modify 2010/5/24
#define MAX_DBL 10e+10
#define MIN_DBL 1.e-10
#define MIN_LEN 1.e-5 // 1.e-8
#define MIN_ANG	1.e-11
#define MIN_RAD 1.e-3 // 1.e-6

#define PI1 3.1415926535897932 // PI
#define PI2 6.2831853071795864 // PI*2
#define PI1_2 1.5707963267948966 // PI/2

#define SR2_2 0.7071067811865475 // square_root(2)/2

#define mathABS(x) ((x)<0?-(x):(x))
#define mathSIGN(d) (((d)<-INF_DBL)?(-1.0):((d)>INF_DBL)?1.0:0.0)
#define mathMIN(x,y) (((x)>(y))?(y):(x))
#define mathMAX(x,y) (((x)>(y))?(x):(y))
#define mathMIN2(a,b,c) ((a)<(b)?((a)<(c)?(a):(c)):((b)<(c)?(b):(c)))
#define mathMAX2(a,b,c) ((a)>(b)?((a)>(c)?(a):(c)):((b)>(c)?(b):(c)))
#define ACOS(d) (d<-0.999999999999?PI1:(d>0.999999999999?0.:acos(d)))

// point
typedef double PNT2D[2] ; // point  type defined in 2 dimension
typedef double PNT3D[3] ; // point  type defined in 3 dimension
typedef double PNT4D[4] ; // point  type defined in 4 dimension
// vector
typedef double VEC2D[2] ; // vector type defined in 2 dimension
typedef double VEC3D[3] ; // vector type defined in 3 dimension
typedef double VEC4D[4] ; // point  type defined in 4 dimension
// array
typedef double *STR1D ; // double array
typedef double (*STR2D)[2] ; // pt array defined in 2 dimension
typedef double (*STR3D)[3] ; // pt array defined in 3 dimension
typedef double (*STR4D)[4] ; // pt array defined in 4 dimension
/*-----------------------------------------------------------*/
//	(2) point
GP_API double mathDist2D(PNT2D p1, PNT2D p2) ;
GP_API void mathRotPnt2D(PNT2D pivot, double angle, PNT2D p, PNT2D rot_p) ;
GP_API double mathDist(PNT3D p1, PNT3D p2) ;
GP_API void mathRotPnt(VEC3D axis, PNT3D pivot, double angle, PNT3D p, PNT3D rot_p) ;
GP_API void mathMidPnt2D(PNT2D p1, PNT2D p2, PNT2D mid_p) ;
GP_API void mathMidPnt(PNT2D p1, PNT2D p2, PNT2D mid_p) ;
GP_API void mathMirPnt(PNT3D pivot, VEC3D normal, PNT3D p, PNT3D mir_p) ;
/*-----------------------------------------------------------*/
//	(3) vector
GP_API void mathGetVec(PNT3D p1, PNT3D p2, VEC3D v) ;
GP_API int mathUniVec2D(VEC2D v, double min_len) ;
GP_API void mathRotVec2D(double angle, VEC2D v, VEC2D rot_v) ;
GP_API double mathVecLen2D(VEC2D v) ;
GP_API double mathOProduct2D(VEC2D vec1, VEC2D vec2) ;
GP_API void mathRotVec2D(double angle, VEC2D v, VEC2D rot_v) ;
GP_API void mathVecAdd2D(double w1, VEC2D v1, double w2, VEC2D v2, VEC2D v) ; // nt add 2011/9/2
GP_API double mathGetAngle2DUnit(VEC2D v1, VEC2D v2) ;
GP_API int mathUniVec(VEC3D v, double min_len) ;
GP_API double mathVecLen(VEC3D v) ;
GP_API double mathOProduct(VEC3D v1, VEC3D v2) ;
GP_API void mathVProduct(VEC3D v1, VEC3D v2, VEC3D rtvec) ;
GP_API double mathGetAngleUnit(VEC3D v1, VEC3D v2) ;
GP_API double mathGetRAngleXZ(VEC3D e1, VEC3D e3, VEC3D v) ;
GP_API void mathRotVec(VEC3D axis, PNT3D pivot, double angle, VEC3D v, VEC3D rot_v) ;
GP_API void mathGetXYFromZ(VEC3D Z, VEC3D X, VEC3D Y) ;
GP_API int mathIsParallel(VEC3D v1, VEC3D v2, double min_ang) ;
GP_API int mathIsPerpendicular(VEC3D v1, VEC3D v2, double min_ang) ;
GP_API void mathMirVec(PNT3D pivot, VEC3D normal, VEC3D v, VEC3D mir_v) ;
GP_API double mathCalTriArea(PNT3D p1, PNT3D p2, PNT3D p3) ;
GP_API double mathOProduct2(VEC3D v, PNT3D b, PNT3D e) ; // nt add 2010, (v, e-b)
GP_API void mathVecAdd(double w1, VEC3D v1, double w2, VEC3D v2, VEC3D v) ; // nt add 2011/9/2
GP_API void mathInitVec(VEC3D v, char xyzFlag) ;
GP_API void mathInitVec2(VEC3D v, char xyzFlag, int posNeg) ;
GP_API void mathVec2D(double v[2], double x, double y) ;
GP_API void mathVec(double v[3], double x, double y, double z) ;
GP_API double mathGetAngleXAcw2D(VEC2D v) ;
GP_API void mathMixVec(VEC3D v1, VEC3D v2, double w1, double w2, VEC3D v) ;
/*-----------------------------------------------------------*/
//	(4) line
GP_API double mathDistPntLin2D(PNT2D p, PNT2D begin, VEC2D dir) ;
GP_API double mathDistPntLin2DEx(PNT2D p, PNT2D begin, PNT2D end) ;
GP_API double mathDistPntLin(PNT3D p, PNT3D begin, VEC3D dir) ;
GP_API void mathPrjPntLin2D(PNT2D p, PNT2D begin, VEC2D dir, PNT2D prj_p) ;
GP_API int mathIntLin2D(PNT2D begin1, VEC2D dir1, PNT2D begin2, VEC2D dir2, double tol, double ang, PNT2D intpt) ;
GP_API void mathEvalLin(PNT3D begin, VEC3D dir, double u, PNT3D p) ;
GP_API double mathDistPntLin(PNT3D p, PNT3D begin, VEC3D dir) ;
GP_API double mathDistPntLinEx(PNT3D p, PNT3D begin, PNT3D end) ;
GP_API void mathPrjPntLin(PNT3D p, PNT3D begin, VEC3D dir, PNT3D prj_p) ;
GP_API int mathIntLin(PNT3D begin1, VEC3D dir1, PNT3D begin2, VEC3D dir2, double tol, double ang, PNT3D intpt1, PNT3D intpt2) ;
/*-----------------------------------------------------------*/
//	(5) plane
GP_API void mathEvalPln(PNT3D pivot, VEC3D X, VEC3D Y, double u, double v, PNT3D p) ;
GP_API double mathDistPntPln(PNT3D p, PNT3D pivot, VEC3D normal) ;
GP_API void mathPrjPntPln(PNT3D p, PNT3D pivot, VEC3D normal, PNT3D prj_p) ;
GP_API void mathPrjVecPln(VEC3D v, VEC3D normal, VEC3D prj_v) ;
GP_API int mathIntLinPln(PNT3D begin, VEC3D dir, PNT3D pivot, VEC3D normal, double tol, double ang, PNT3D intpt) ;
GP_API int mathIntPln(PNT3D pivot1, VEC3D normal1, PNT3D pivot2, VEC3D normal2, double tol, double ang, PNT3D intpt, VEC3D dir) ;
/*-----------------------------------------------------------*/
//	(6) segment
//	2D�߶�
typedef struct _line_sg2d LIN2D ; // struct of line segment 2d
struct _line_sg2d
{
    LIN2D*      next ;
    PNT2D       p1, p2 ; // begin and end point
} ;

//  3D�߶�
typedef struct _line_sg3d LIN3D ; // struct of line segment 2d
struct _line_sg3d
{
    LIN3D*      next ;
    PNT3D       p1, p2 ; // begin and end point
} ;

GP_API int mathIntSegm2D(PNT2D b1, PNT2D e1, PNT2D b2, PNT2D e2, double tol, double ang, PNT2D p1, PNT2D p2) ;
GP_API int mathIntSegm(PNT3D b1, PNT3D e1, PNT3D b2, PNT3D e2, double tol, double ang, PNT3D p1, PNT3D p2) ;
GP_API LIN2D* mathCreLin2D(PNT2D p1, PNT2D p2) ;
GP_API LIN3D* mathCreLin3D(PNT3D p1, PNT3D p2) ;
/*-----------------------------------------------------------*/
//	(7) rectangular coordinate frame
typedef struct _rect_fm2d RFRM2D ; // rectangle coor_frame 2d
struct _rect_fm2d				   
{
    VEC2D       X, Y ; // 2d unit vectors of x, y axis
	PNT2D		O ; // origin
    double      scale ; // scale > 0.0
} ;

typedef struct _rect_fm3d RFRAME ; // rectangle coor_frame 3d
struct _rect_fm3d
{
    VEC3D       X, Y, Z ; // 3d unit vectors of x, y, z axis
	PNT3D		O ; // origin
    double      scale ; // scale > 0.0
} ;

GP_API void mathInitRFrame(RFRAME* pLF) ;
GP_API void mathMoveRFrame(VEC3D translateVector, RFRAME* pLF) ;
GP_API void mathRotateRFrame(PNT3D p, VEC3D axis, double angle, RFRAME* pLF) ;
GP_API void mathGetRFrameOfQuadrant(int i, RFRAME* pLF) ;
GP_API void mathInitMatrix16(PNT3D O, VEC3D X, VEC3D Y, VEC3D Z, double matrix[16]) ;
GP_API void mathTransLocalPnt3D(RFRAME* local_frame, PNT3D local_point, PNT3D world_point) ;
GP_API void mathTransWorldPnt3D(RFRAME* local_frame, PNT3D world_point, PNT3D local_point) ;
GP_API void mathTransLocalVec3D(RFRAME* local_frame, VEC3D local_vector, VEC3D world_vector) ;
GP_API void mathTransWorldVec3D(RFRAME* local_frame, VEC3D world_vector, VEC3D local_vector) ;
GP_API void mathTransWorldPnt3DByOXYZ(PNT3D origin, VEC3D e1, VEC3D e2, VEC3D e3, PNT3D world_point, PNT3D local_point) ;
/*-----------------------------------------------------------*/
//	(8) box
typedef struct _box2d BOX2D ; // structure of box defined in 2d
struct _box2d
{
    double      min[2], max[2] ;
} ;

typedef struct _box3d BOX3D ; // structure of box defined in 3d
struct _box3d
{
    double      min[3], max[3] ;
} ;

typedef BOX2D BOXUV ; // surface parameter u and v bounding box

GP_API void mathClnBox(BOX3D* box) ;
GP_API void mathEnlargeBox(PNT3D p, BOX3D* box) ;
GP_API int mathChkBox2DSegmInt(BOX2D* box, PNT2D p1, PNT2D p2, double tol) ;
GP_API int mathChkBox2DInt(BOX2D* box1, BOX2D* box2, double tol) ;
GP_API void mathCalBoxUnion(BOX3D* box1, BOX3D* box2, BOX3D* box) ;
GP_API int mathChkBoxLinInt(BOX3D* box, PNT3D p, VEC3D dir, double tol) ;
GP_API int mathChkBoxInt(BOX3D* box1, BOX3D* box2, double tol) ;
GP_API void mathTransLocalBox2D(RFRAME* local_frame, BOX2D* local_box2d, BOX3D* world_box3d) ;
GP_API void mathTransLocalBox(RFRAME* local_frame, BOX3D* local_box3d, BOX3D* world_box3d) ;
GP_API void mathTransWorldBox(RFRAME* local_frame, BOX3D* world_box3d, BOX3D* local_box3d) ;
GP_API int mathChkBoxPnt(BOX3D* box, PNT3D p, double tol) ;
GP_API void mathMoveBox(BOX3D* box, VEC3D v) ;
GP_API void mathRotateBox(BOX3D* box, PNT3D pivot, VEC3D axis, double a) ;
GP_API void mathScaleBox(BOX3D* box, double scale) ;
GP_API void mathScaleBox2(BOX3D* box, PNT3D center, double scale) ;
/*-----------------------------------------------------------*/
// (9)
/*	2001/9/1 nt
 *	type of intersect point
 */
enum EIntPtType
{
	ipSimple = 0,
	ipTangent = 1,
	ipOverlap = 2
} ;

/*	2001/9/1 nt
 *	intersect point
 */
typedef struct _inter_pnt INTPT ; // intersect point
struct _inter_pnt
{
	INTPT*		prev ;
	INTPT*		next ;
	int			type ; // IntPtType
	int			flag1 ; // -1 : parameter decrease dir, 0 : unkown, 1 : parameter increase, 2 : double dir
	int			flag2 ; // -1, 0, 1, 2
	PNT3D		p ; // intersect position
	VEC3D		tvec ; // unit tangent vector of intersect curve
	double		t ;
	PNT2D		uv ; // parameter u (and v) of curve1(or surface1)
	PNT2D		st ; // parameter s (and t) of curve2(or surface2)
	VEC2D		uvdir ;
	VEC2D		stdir ;
	double		step ;
	double		a ; // nt add 2002/5/26
} ;

GP_API INTPT* intptCreate(int type, PNT3D p, VEC3D tvec, double u, double v, double s, double t) ;
GP_API int intptFree(INTPT* intpt) ;
/*-----------------------------------------------------------*/
// (10)
/*	2001/8/27 nt
 *	parametric interval
 */
typedef struct _interval INTERVAL ; // param interval for curve, surface
struct _interval // t1 <= t2
{
	double		t1 ; // a sufficient negative large number indicate infinite
	double		t2 ; // a sufficient positive large number indicate infinite
} ;
/*-----------------------------------------------------------*/
// (11)
/*	2001/8/28 nt
 *	a string of point and related information
 */
typedef struct _str_of_pt STRPT ; // string of pt and related info
struct _str_of_pt
{
	int			np ; // number of point
	PNT3D*		ps ; // [np], array of point
	VEC3D*		vs ; // [np], array of unit vector
	double*		ts ; // [np], array of parameter t
	double*		as ; // [np], array of attribute
} ;

#define STRPT_PT 1
#define STRPT_VEC 2
#define STRPT_PARAM 4
#define STRPT_ATTRIB 8
GP_API STRPT* strptCreate(int np, int flags) ;
GP_API int strptFree(STRPT* strpt) ;
GP_API int strptCalBox(STRPT* strpt, double tol, BOX3D* box) ;
GP_API int strptDraw(STRPT* strpt, int color, double width) ;
GP_API int strptPick(STRPT* strpt, PNT3D pos, VEC3D dir, double scope, double* pz, double* pu, int* picked) ;
/*-----------------------------------------------------------*/
// (12)
/*  1997/6/30 nt
 *  pick information
 */
typedef struct _pick_info PINFO ;
struct _pick_info // ʰȡ��Ϣ
{
    int type ; // entity type
    double pos[3] ; // mouse position(relative to model coordinate frame)
    double dir[3] ; // view dir(from eye to object)
    double r ; // picking radius

	int	ot ; // object type
    void* obj ; // picked object
    double u ;
	double v ; // u, v parameter
	double z ; // depth from pos to entity, (-)--->(+),dir:--->
	int att ; // 0:region,1=line, 2=point, nt add 2011/7/6
} ;

/*	2001/11/17 nt
 *	Math_PInfo.c
 */
GP_API PINFO* pinfoCreate(int type, PNT3D pos, VEC3D dir) ;
GP_API int pinfoFree(PINFO* pinfo) ;
/*-----------------------------------------------------------*/
// (13)
/*	2001/11/17 nt
 *	facet node
 */
typedef struct _facetnode FNODE ;
struct _facetnode
{
    PNT3D       p ; // space position
    PNT2D       uv ; // parameter space position
    VEC3D       normal ; // space normal
    double      attrib ; // attribute
} ;

/*	2001/11/17 nt
 *	facet
 */
typedef struct _facet FACET ; // facet used in surface or face display
struct _facet
{
    int         nnode ; // number of FNODE
    FNODE*		nodes ; // array of FNODE
    int         ntria ; // number of triangle
    int			(*trias)[3] ; // array of triangles
    int			nquad ; // number of quadrilateral
    int			(*quads)[4] ; // array of quadrilaterals
} ;

GP_API FACET* facetCreate(int nnode, int ntria, int nquad) ;
GP_API int facetFree(FACET* facet) ;
GP_API int facetCalBox(FACET* facet, BOX3D* box) ;
GP_API int facetDraw(FACET* facet, int drawMode, int color) ;
GP_API int facetCalZDepth(FNODE* nodes, int ns[3], PNT3D gc, double* pz, double* pu, double* pv) ;
GP_API int facetPick(FACET* facet, PNT3D pos, VEC3D dir, double scope, double* pz, double* pu, double* pv, int* picked) ;
/*-----------------------------------------------------------*/
// (14)
/*	2001/8/27 nt
 *	parametric local property:endpoint type
 */
enum EEndpType
{
	epInfinite = 0,
	epExtendable = 1,
	epFixed = 2,
	epDegenerate = 3,
} ;

/*	2001/8/27 nt
 *	parametric global property:periodic type
 */
enum EPeriType
{
	peOpen = 0,
	peClosed = 1,
	pePeriodic = 2
} ;

/*	2001/8/27 nt
 *	parametric global property:parametric type
 */
enum EParaType
{
	paLinear = 0,
	paCircular = 1,
	paOthers = 3
} ;

/*	2001/8/27 nt
 *	parametric global property:geometric continuous type
 */
enum EContType
{
	cnOrder0 = 0,
	cnOrder1 = 1,
	cnOrder2 = 2,
	cnOrderN = 3
} ;

/*	2001/8/27 nt
 *	param
 */
typedef struct _param PARAM ;
struct _param
{
	INTERVAL	interval ; // define basic range of parameter value
	int			endpType[2] ; // EndpType
	int			periType ; // PeriType
	int			paraType ; // ParaType
	int			contType ; // ContType
} ;
GP_API PARAM* paramCreate() ;
GP_API int paramFree(PARAM* param) ;
GP_API int paramInit(PARAM* param) ;
/*-----------------------------------------------------------*/
//	(15) ����
/*  1997/2/1 nt
 *  virture bidirection chain
 */
typedef struct _bdchain BDCHN ; // a base class for bidir chain
struct _bdchain
{                   
    BDCHN*		prev ;
	BDCHN*		next ;
} ;

/*  1997/2/1 nt
 *  virture single direction chain
 */
typedef struct _sdchain SDCHN ; // a base class for single chain
struct _sdchain
{                   
    SDCHN*		next ;
} ;

/*  1997/10/6 nt
 *  ��ָ������
 */
typedef struct _vpcarrier VPCARRIER ; // void pointer carrier
struct _vpcarrier
{
    VPCARRIER*  next ;
    void*       pointer ;
} ;

GP_API void mathFreBDChns(void* list_head) ;
GP_API void* mathInsBDChnsAtHead(void* list_stri, void* list_head) ;
GP_API void* mathInsBDChnsAtTail(void* list_stri, void* list_head) ;
GP_API void* mathDelItemInBDChns(void* item, void* head) ;
GP_API void* mathInsItemInBDChns(void* item, void* head) ;
GP_API int mathGetNumOfItemInBDChns(void* head) ;
GP_API void* mathRevBDChns(void* head) ;
GP_API void mathFreSDChns(void* list_head) ;
GP_API void* mathInsSDChnsAtHead(void* list_stri, void* list_head) ;
GP_API void* mathInsSDChnsAtTail(void* list_stri, void* list_head) ;
GP_API void* mathDelItemInSDChns(void* item, void* head) ;
GP_API void* mathInsItemInSDChns(void* item, void* head) ;
GP_API int mathGetNumOfItemInSDChns(void* head) ;
/*-----------------------------------------------------------*/
/*	2003/5/9 nt
 *	point cloud
 */
typedef struct _pcloud PCLOUD ; // a group of points in 3d space
struct _pcloud
{
	int			np ; // number of point
	PNT3D*		ps ; // points
} ;

/*
 *	Math_PCloud.c
 */
GP_API PCLOUD* pcloudCreate(int n) ;
GP_API int pcloudFree(PCLOUD* pcloud) ;
GP_API PCLOUD* pcloudCopy(PCLOUD* pcloud) ;
GP_API int pcloudMove(PCLOUD* pcloud, VEC3D v) ;
GP_API int pcloudRotate(PCLOUD* pcloud, PNT3D pivot, VEC3D axis, double angle) ;
GP_API int pcloudMirror(PCLOUD* pcloud, PNT3D pivot, VEC3D normal) ;
GP_API int pcloudToLocal(PCLOUD* pcloud, RFRAME* pLF) ;
GP_API int pcloudToWorld(PCLOUD* pcloud, RFRAME* pLF) ;
GP_API int pcloudScale(PCLOUD* pcloud, double scale) ;
GP_API int pcloudCalBox(PCLOUD* pcloud, double tol, BOX3D* box) ;
GP_API int pcloudDraw(PCLOUD* pcloud, int color, double size) ;
/*-----------------------------------------------------------*/
//	others
GP_API int mathEvalTriContLin(int dim, double* p1, double* p2, double* p3, double a1, double a2, double a3, double attTol, double tol, double* cp1, double* cp2 ) ;
GP_API int mathGetTriNormal(PNT3D p1, PNT3D p2, PNT3D p3, double tol, VEC3D normal) ;
GP_API int mathChkPntSegm(PNT3D p, PNT3D begin, PNT3D end, double tol) ;
GP_API void triGetVertexOfMinXYZ(int xyzID, PNT3D v1, PNT3D v2, PNT3D v3, PNT3D p) ;
GP_API void triGetVertexOfMaxXYZ(int xyzID, PNT3D v1, PNT3D v2, PNT3D v3, PNT3D p) ;
GP_API int angleRayClassify(double a, double ray, double ang) ;
GP_API int planeRayClassify(VEC3D normal, VEC3D ray, double ang) ;
GP_API int triIntZ(PNT3D vs[3], VEC3D normal, PNT3D p, double tol, double zMinMax[2]) ;
GP_API int triIntZ2(PNT3D vs[3], VEC3D normal, PNT3D p, double tol, double* pz) ;
//--------------------------------------------------------------
//	(28) circular arc
typedef struct _arc ARC ; // circular arc defined in local frame
struct _arc
{
	RFRAME		lf ; // local frame
	double		r ; // radius
	double		a ; // angle
} ;

/*
 *	Math_Arc.c
 */
GP_API ARC* mathCreArc(double radius, double angle) ;
GP_API void mathFreArc(ARC* arc) ;
GP_API int mathEvalArcPos(ARC* arc, double u, PNT3D pos) ;
GP_API int mathEvalArcDer1O(ARC* arc, double u, PNT3D pos, VEC3D du) ;
GP_API int mathEvalArcDer2O(ARC* arc, double u, PNT3D pos, VEC3D du, VEC3D duu) ;
GP_API void mathEvalArcEndp(ARC* arc, PNT3D b, PNT3D e) ;
GP_API double mathCalArcArcLen(ARC* arc) ;
GP_API int mathRevsArc(ARC* arc) ;
GP_API void mathCalArcBox(ARC* arc, BOX3D* box) ;
GP_API int mathDiscArc(ARC* arc, double tol, int* pn, double (**ps)[3]) ;
GP_API int mathGenArc(PNT3D begin, PNT3D center, VEC3D normal, double angle, ARC** parc) ;
GP_API int mathGenArcBy3P(PNT3D p1, PNT3D p2, PNT3D p3, ARC** parc) ;
GP_API int mathGenArcBy2PCen(int flag, PNT3D p1, PNT3D p2, PNT3D center, VEC3D normal, ARC** parc) ;
GP_API int mathGetPntArcNP(ARC* arc, PNT3D p, PNT3D nearest_p, double* pu, double* pd) ;
GP_API int mathGetPntCirNP(RFRAME* pLF, double r, PNT3D p, PNT3D nearest_p, double* pu) ;
GP_API int mathChkPntArc(ARC* arc, PNT3D p, double tol) ;
GP_API int mathIsCirTL(RFRAME* pLF, double r, PNT3D begin, VEC3D dir, double tol, double ang) ;
GP_API int mathGetArcImp(double r, PNT2D origin, double qs[6]) ;
GP_API int mathIntCirLin2D(double r, PNT2D begin, VEC2D dir, double tol, PNT2D intpt1, PNT2D intpt2) ;
GP_API int mathIntCirLin(RFRAME* pLF, double r, PNT3D begin, VEC3D dir, double tol, PNT3D intpt1, PNT3D intpt2, int* pn) ;
GP_API int mathIntCirPln(RFRAME* pLF, double r, PNT3D pivot, VEC3D normal, double tol, double ang, PNT3D intpt1, PNT3D intpt2, int* pn) ;
GP_API int mathIntCir(RFRAME* pLF1, double r1, RFRAME* pLF2, double r2, double tol, double ang, int* pn, PNT3D intpt1, PNT3D intpt2) ;
GP_API int mathMakeArc(ARC* arc, PNT3D center, VEC3D normal, PNT3D p1, PNT3D p2, PNT3D p) ; // nt add 2010
GP_API int mathGetPntEllNP2D(double a, double b, PNT2D p, PNT2D nearest, double* pu, double* pd) ;
//==============================================================
// �����ӿ�
GP_API int mathChkPntSegm(PNT3D p, PNT3D begin, PNT3D end, double tol) ;
GP_API int mathSolHomoEqua22(double a11, double a12, double a21, double a22, double tol, double rs[2]) ;
GP_API int mathSolHomoEqua(double a, double b, double c, double tol, double solus[2]) ;
GP_API int mathSolQuadEqua(double a, double b, double c, double tol, double solus[2]) ;
GP_API int mathMatxInvs(int row, int col, double* matrix, double* inverse) ;
GP_API int mathOptNormalOfPts(int np, STR3D ps, PNT3D center, VEC3D normal) ;
GP_API int mathOptNormalOfVecs(int nv, STR3D vs, VEC3D normal) ; // nt add 2006/7/25
GP_API int mathPickSegm(PNT3D begin, PNT3D end, PNT3D p, VEC3D viewdir, double scope) ;
GP_API int mathChkOriInTri2D(PNT2D p1, PNT2D p2, PNT2D p3, double tol) ;
GP_API int mathCalTri2DGCP(PNT2D p1, PNT2D p2, PNT2D p3, PNT2D p, PNT3D gc) ;
GP_API int mathGetTriNormal(PNT3D p1, PNT3D p2, PNT3D p3, double tol, VEC3D normal) ;
GP_API double trimValue(double value, int pre) ;
GP_API double trimValue2(double value, int pre) ;
GP_API int triCut(PNT3D A, PNT3D B, PNT3D C, int i, double xyz, double tol, PNT3D ps[2], int as[2]) ;
//--------------------------------------------------------------
typedef struct _da DA ; // dihedral angle
struct _da
{
	PNT3D p ; // ����Ǳ���һ��
	VEC3D e ; // ����Ǳߵĵ�λʸ��
	PNT3D p1 ;
	VEC3D n1 ;
	PNT3D p2 ;
	VEC3D n2 ;

	VEC3D X ;
	VEC3D Y ;
	double a ;
} ;

GP_API DA* daCreate() ;
GP_API void daFree(DA* da) ;
GP_API void daRevEdge(DA* da) ;
GP_API int daCalStdXY(DA* da) ;
GP_API int daRayClassify(DA* da, VEC3D ray, double ang) ;
//--------------------------------------------------------------
#define PA_MAX_DA 512

typedef struct _pa PA ; // polyhedral angle
struct _pa
{
	int max ;
	int n ;
	DA* das ;
} ;

GP_API PA* paCreate(int max) ;
GP_API int paFree(PA* pa) ;
GP_API int paAlloc(PA* pa, int max) ;
GP_API int paRayClassify(PA* pa, VEC3D ray, double ang) ;
//--------------------------------------------------------------
// nt add 2011/5/17
GP_API void m33Zero(double A[3][3]) ; // aij = 0.
GP_API void m33Identify(double A[3][3]) ; // // ����Ϊ��λ��
GP_API void m33Rev(double A[3][3]) ; // A = -A
GP_API void m33Mult(double A[3][3], double B[3][3], double C[3][3]) ; // C = A*B
GP_API void m33MultVec(double A[3][3], double v[3], double B[3]) ; // B = A*v, v��������
GP_API void m33VecMult(double v[3], double A[3][3], double B[3]) ; // B = v*A, v��������
GP_API void m33Plus(double A[3][3], double B[3][3], double C[3][3]) ; // C = A+B
GP_API void m33Minus(double A[3][3], double B[3][3], double C[3][3]) ; // C = A-B
GP_API void m33Trans(double A[3][3], double B[3][3]) ; // B = A'
GP_API void m33Scalar(double A[3][3], double s) ; // A = A*s
GP_API double m33MAbs(double A[3][3]) ; // return max{|aij|}
GP_API double m33Tr(double A[3][3]) ; // a11+a22+a33
GP_API double m33Det(double A[3][3]) ; // |A|
GP_API int m33Inv(double A[3][3], double B[3][3]) ; // ������������
GP_API int m33SVD(double A[3][3], double U[3][3], double d[3], double V[3][3]) ; // A = U*d*V, dΪ�Խ���
GP_API int m33Eig(double A[3][3], double eigval[3], double eigvec[3][3]) ; // ��3x3���������������������
GP_API int m33Eig2(double A[3][3], double eigval[3], double eigvec[3][3]) ; // ��3x3�Գƾ��������������������
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//	(1) ��������
/*	1991 nt
 *	BSCUR
 */
typedef struct _bspln_cur BSCUR ; // NURBS curve definition
struct _bspln_cur
{
    int         ord_u ; // order of u parameter
	int			cnt_u ; // number of control pnt of u parameter
    double		(*u_p)[4] ; // control points
    double*		u_k ; // knot vector
} ;
typedef BSCUR** STRCV ; // a string of curve
GP_API BSCUR* bscurCreate(int ord_u, int cnt_u) ;
GP_API void bscurFree(BSCUR* curv) ;
GP_API BSCUR* bscurCopy(BSCUR* curv) ;
GP_API int bscurRevsDir(BSCUR* curv) ;
GP_API int bscurSaveASCII(BSCUR* curv, FILE* fp) ;
GP_API BSCUR* bscurLoadASCII(FILE* fp) ;
GP_API int bscurSaveBinary(BSCUR* curv, FILE* fp) ;
GP_API BSCUR* bscurLoadBinary(FILE* fp) ;
GP_API int bscurSaveAr(BSCUR* curv, void* ar, int (*arWrite)(void* ar, void* buf, int size)) ;
GP_API BSCUR* bscurLoadAr(void* ar, int (*arRead)(void* ar, void* buf, int size)) ;
GP_API int bscurEvalPos(BSCUR* curv, double u, PNT3D pos) ;
GP_API int bscurEvalDer1O(BSCUR* curv, double u, PNT3D pos, VEC3D du) ;
GP_API int bscurEvalDer2O(BSCUR* curv, double u, PNT3D pos, VEC3D du, VEC3D duu) ;
GP_API int bscurEvalDer3O(BSCUR* curv, double u, PNT3D pos, VEC3D du, VEC3D duu, VEC3D duuu) ; // nt add 2013/4/7
GP_API int bscurEvalCur(BSCUR* curv, double u, PNT3D pos, double* pcur) ;
GP_API int bscurEvalCurEx(BSCUR* curve, double t, VEC3D refv, PNT3D pos, double* pcur) ; // nt add 2011/1/28
GP_API int bscurGetMaxiStepLin(BSCUR* curv, double tol, double* pstep) ;
GP_API int bscurIntpPs(int n, STR3D string, int ord_u, BSCUR** pcurv) ;
GP_API int bscurIntpPsET(int n, STR3D string, VEC3D tanvec1, VEC3D tanvec2, int ord_u, BSCUR** pcurv) ;
GP_API int bzcIntpPts(int n, STR3D ps, BSCUR** pbez) ; // nt add 2011/8/15
GP_API int bscurExtdByLin(BSCUR* curv, int idbe, double arclen, BSCUR** pextd_curv) ;
GP_API int bscurExtdByArc(BSCUR* curv, int idbe, double radius, double arclen, BSCUR** pextd_curv) ;
GP_API int bscurCreFixArcLen(PNT3D pos, VEC3D du, VEC3D duu, double arclen, BSCUR** pcurv) ;
GP_API int bscurExtdByCurv(BSCUR* curv, int idbe, double arclen, BSCUR** pextd_curv) ;
GP_API int bscurJoinG1(BSCUR* curv1, BSCUR* curv2, double tol, double maxDev[3]) ;
GP_API int bscurModfG1(BSCUR* curv, int idbe, VEC3D tanv) ;
GP_API int bscurModfG2(BSCUR* curv, int idbe, double cuvt) ;
GP_API int bscurMakePeriodic(BSCUR* curv) ;
GP_API int bscurAppx(BSCUR* curv, int np, STR3D ps, int bndcond, double tol, double* pd) ;
GP_API int bscurSmth(BSCUR* curv, int bndcond, double tol, double* pd) ;
GP_API int bscurCompByPrj(BSCUR* primary, BSCUR* auxiliary, RFRAME* pLF, BSCUR** pcurv) ;
GP_API int bscurSnapPnt(BSCUR* curv, PNT3D p, double tol, PNT3D curv_p, double* pu, double* pd) ;
GP_API double bscurCalArcLenEx(BSCUR* curv, INTERVAL* interval, double tol) ; // nt add 2011/6/14
GP_API double bscurCalArcLen(BSCUR* curv) ; // nt add 2011/6/14
GP_API int bscurCreArc(ARC* arc, BSCUR** pcurv) ;
GP_API void bscurScale(BSCUR* curv, double scale) ;
GP_API int bscurGetENormalVec(BSCUR* nurbs, VEC3D normal) ; // nt add 2011/1/28
// ����ֵ�����߲������ϵ�һ��������, flagָ����������:
// 1=|Pu|^2, 2=|Puu|^2, 3=|Puuu|^3, 4=C^2, 5=|dC/du|^2, C��ʾ����, 6=fabs((duu,duuu)), (,)��ʾʸ���ĵ��, nt add 2012/9/20
// rtol������ݲһ���ȡ0.01��0.0001
GP_API double bscurCalEnergy(BSCUR* nurbs, INTERVAL* interval, int flag, double rtol, double tol) ; // nt add 2010/12
GP_API double bscurCalEnergy2(BSCUR* nurbs, INTERVAL* interval, int flag, double tol) ; // nt add 2010/12
GP_API int bscurCalEnergyAll(BSCUR* nurbs, INTERVAL* interval, double rtol, double tol, double es[]) ; // nt add 2011/6/20
GP_API int bscurMakeCPConvex(BSCUR* nurbs, int nItt) ; // nt add 2011/6/20
GP_API int bscurIttMinDist(BSCUR* c1, BSCUR* c2, double* pu1, double* pu2, double tol, PNT3D p1, PNT3D p2, double* pmin) ; // nt add 2011/6/24
GP_API int bscurIttMinDist2(BSCUR* c1, BSCUR* c2, double* pu1, double* pu2, double tol, PNT3D p1, PNT3D p2, double* pmin) ; // nt add 2011/7/14
GP_API int bscurIttMinDistAlongDir(BSCUR* c1, BSCUR* c2, double* pu1, double* pu2, VEC3D dir, double tol, PNT3D p1, PNT3D p2, double* pmin) ; // nt add 2011/6/24
GP_API int bscurCalBox2(BSCUR* nurbs, INTERVAL* interval, BOX3D* box) ; // nt add 2011/6/27
GP_API int bscurCalUB1(BSCUR* bc, double* ub) ; // ��������һ�׵���ģ���Ͻ�, nt add 2014/8/13
GP_API int bscurCalUB2(BSCUR* bc, double* ub) ; // �������߶��׵���ģ���Ͻ�, nt add 2014/8/13
//-------------------------------------------------------------
//	(2) ��������
/*	1991 nt
 *	BSSUR
 */
typedef struct _bspln_sur BSSUR ; // NURBS surf definition
struct _bspln_sur
{
    int         ord_u ; // order of u parameter
	int			ord_v ; // order of v parameter
    int         cnt_u ; // number of control pnt of u parameter
	int			cnt_v ; // number of control pnt of v parameter
    double		(**uv_p)[4] ; // control pnt of cnt_u x cnt_v
    double*		u_k ; // knot vector of u parameter
	double*		v_k ; // knot vector of v parameter
} ;
typedef BSSUR** STRSF ; // a string of surface
GP_API BSSUR* bssurCreate(int ord_u, int cnt_u, int ord_v, int cnt_v) ;
GP_API void bssurFree(BSSUR* surf) ;
GP_API BSSUR* bssurCopy(BSSUR* surf) ;
GP_API int bssurRevsDir(BSSUR* surf, int flag) ;
GP_API int bssurSaveASCII(BSSUR* surf, FILE* fp) ;
GP_API BSSUR* bssurLoadASCII(FILE* fp) ;
GP_API int bssurSaveBinary(BSSUR* surf, FILE* fp) ;
GP_API BSSUR* bssurLoadBinary(FILE* fp) ;
GP_API int bssurSaveAr(BSSUR* surf, void* ar, int (*arWrite)(void* ar, void* buf, int size)) ;
GP_API BSSUR* bssurLoadAr(void* ar, int (*arRead)(void* ar, void* buf, int size)) ;
GP_API int bssurEvalPos(BSSUR* surf, double u, double v, PNT3D pos) ;
GP_API int bssurEvalDer1O(BSSUR* surf, double u, double v, PNT3D pos, VEC3D du, VEC3D dv) ;
GP_API int bssurEvalDer2O(BSSUR* surf, double u, double v, PNT3D pos, VEC3D du, VEC3D dv, VEC3D duu, VEC3D duv, VEC3D dvv) ;
GP_API int bssurEvalNormal(BSSUR* surf, double u, double v, PNT3D pos, VEC3D normal) ;
GP_API int bssurEvalKH(BSSUR* surf, double u, double v, PNT3D pos, VEC3D du, VEC3D dv, VEC3D normal, double* pK, double* pH) ; // nt add 2011/6/17
// ����flag��������S(u,v)�ֲ�����ĳ�ּ�����
// flag�ĺ�������:
// 1=|Su|+|Sv|, 
// 2=|Suu|+|Suv|+|Svv|,
// 3=|Suuu|+|Svvv|
// 4=|k1|+|k2|, k1,k2�������� 
// 5=|K|, K�Ǹ�˹���� 
// 6=|H|, H��ƽ������
// 7=|dH/du|+|dH/dv|
// 8=fabs((Suu,Suuu))+fabs((Svv,Svvv)), nt add 2012/9/20
// 9=Max(fabs((Suu,Suuu)),fabs((Svv,Svvv))), nt add 2013/7/7
GP_API double bssurEvalEnergy(BSSUR* nurbs, double u, double v, int flag, double d) ; // nt add 2011/6/18
GP_API int bssurGetMaxiStepLin(BSSUR* surf, double tol, double* pstep_u, double* pstep_v) ;
GP_API int bssurCreSphere(RFRAME* pLF, double r, BSSUR** psurf) ;
GP_API int bssurCreCylinder(RFRAME* pLF, double r, double h, BSSUR** psurf) ;
GP_API int bssurCreCone(RFRAME* pLF, double r, double h, BSSUR** psurf) ;
GP_API int bssurCreTorus(RFRAME* pLF, double r1, double r2, BSSUR** psurf) ;
GP_API int bssurCrePntArray(int cnt_u, int cnt_v, double (**array)[3], BSSUR** psurf) ;
GP_API int bssurCreRule(BSCUR* curv1, BSCUR* curv2, BSSUR** psurf) ;
GP_API int bssurCreSkin(BSCUR* bnd1, BSCUR* bnd2, BSCUR* bnd3, BSCUR* bnd4, double tol, BSSUR** psurf) ;
GP_API int bssurCreSection(int n, BSCUR** sectcurvs, STR1D p_u, int ord_u, double tol, BSSUR** psurf) ;
GP_API int bssurCreBSweep(int nprofile, STRCV profiles, int nspine, STRCV spines, double tol, BSSUR** psurf) ;
GP_API int bssurCrePiping(BSCUR* spine, double radius, double tol, BSSUR** psurf) ;
GP_API int bssurCreConnect(BSSUR* surf1, BSSUR* surf2, double tol, double weit, BSSUR** psurf) ;
GP_API int bzsCreLoft(int n, STRCV sections, BSSUR** pbez) ; // nt add 2011/7/19
GP_API int bssurJoin(int idgx, BSSUR* surf1, BSSUR* surf2, double tol, double maxDev[3]) ;
GP_API int bssurInqGCD(BSSUR* surf1, BSSUR* surf2, double tol, double gcd[4]) ;
GP_API int bssurAppx(BSSUR* surf, int np, STR3D ps, int bndcond, double tol, double sc, double* pd) ;
GP_API int bssurAppx2(BSSUR* surf, int np, STR3D ps, STR2D uvs, int bndcond, double tol, double sc, double* pd) ; // nt add 2006/7/6
GP_API int bssurAppxZ(BSSUR* surf, int np, STR3D ps, STR2D uvs, int bndcond, double tol, double sc, double* pd) ; // nt add 2006/7/28
GP_API int bssurSnap(BSSUR* surf, PNT3D p, double tol, PNT3D surf_p, double* pu, double* pv, double* pd) ;
GP_API double bssurCalAreaEx(BSSUR* surf, BOXUV* boxuv, double tol) ; // nt add 2011/6/15
GP_API double bssurCalArea(BSSUR* surf) ; // nt add 2011/6/15
GP_API double bssurCalEnergy(BSSUR* nurbs, BOXUV* boxuv, int flag, double rtol, double tol) ; // nt add 2011/6/18
GP_API double bssurCalEnergy2(BSSUR* nurbs, BOXUV* boxuv, int flag, double tol) ; // nt add 2011/6/18
GP_API int bssurCalEnergyAll(BSSUR* nurbs, BOXUV* boxuv, double rtol, double tol, double es[8]) ; // nt add 2011/6/20
GP_API int bssurCalBox2(BSSUR* nurbs, BOXUV* boxuv, BOX3D* box) ; // nt add 2011/6/27
GP_API int bssurCalUB1(BSSUR* bs, double ubs[2]) ; // ��������һ�׵���ģ���Ͻ�, nt add 2014/8/14
GP_API int bssurCalUB2(BSSUR* bs, double ubs[3]) ; // ����������׵���ģ���Ͻ�, nt add 2014/8/14
//	mesh patch
typedef struct _meshpatch MPATCH ; // for construct surface over curve mesh
struct _meshpatch // structure of mesh patch
{
	int			ncurve ;
	STRCV		curves ; // each curves[i] will not be free by mpatchFree()
	int			npatch ;
	STRSF		patchs ; // each patchs[i] will not be free by mpatchFree()
} ;

/*
 *	Mesh_Patch.c
 */
GP_API MPATCH* mpatchCreate(int ncurve, STRCV curves, int npatch, STRSF patchs) ;
GP_API int mpatchFree(MPATCH* mpatch) ;
GP_API int mpatchBuild(MPATCH* mpatch, int idgx, double tol) ;
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//	(1) current precision for geometric modeling
GP_API double gpGetTol() ;
GP_API void gpSetTol(double tol) ;
GP_API double gpGetAng() ;
GP_API void gpSetAng(double ang) ;
GP_API void gpGetPre(double pre[3]) ;
GP_API void gpSetPre(double pre[3]) ;
GP_API void gpGetTra(double tra[3]) ;
GP_API void gpSetTra(double Tra[3]) ;
GP_API int gpGetTes(double tes[3]) ; // nt add 2010/5/19
GP_API int gpSetTes(double tes[3]) ; // nt add 2010/5/19
GP_API void gpGetDsc(double dsc[3]) ; // nt add 2003/2/13
GP_API void gpSetDsc(double dsc[3]) ; // nt add 2003/2/13
//-------------------------------------------------------------
//	(2) ��������Ԫ�صĻ���
/*	2001/8/10 nt
 *	entity type
 */
enum EEntityType
{
	etVertex = 0, // vertex
	etEdge = 1, // edge
	etFin = 2, // fin
	etLoop = 3, // loop
	etFace = 4, // face
	etShell = 5, // shell
	etBody = 6, // body
	etPoint3D = 7, // point3d
	etLine = 8, // line
	etPlane = 9, // plane
	etCircle = 10, // circle
	etCylinder = 11, // cylinder
	etEllipse = 12, // ellipse
	etCone = 13, // cone
	etBCurve = 14, // b-spline curve
	etSphere = 15, // sphere
	etIntCur = 16, // intersect curve
	etTorus = 17, // torus
	etPCurve = 18, // parameter curve
	etRulSur = 19, // rule surface
	etHelix = 20, // helix curve
	etRevSur = 21, // revolve surface
	etSpiral = 22, // spiral curve
	etBSurface = 23, // b-spline surface
	etIsoCur = 24, // iso curve
	etSweep = 25, // sweep surface
	etBound = 26, // boundary curve
	etRBSur = 27, // rolling ball surface
	etTrmCur = 28, // trim curve
	etOffset = 29, // offset surface
	etOffCur = 30 // offset curve
} ;

typedef void ENTITY ;

// �������ʵ�ֵĽӿ�
GP_API ENTITY* entityAlloc(int et) ;
GP_API int entityGetType(ENTITY* entity) ; // return entity type
GP_API int entityGetId(ENTITY* entity) ; // return entity id
GP_API int entityGetTag(ENTITY* entity) ;
GP_API int entityIs(ENTITY* entity, int type) ; // ID_NO:is not;ID_YES:is
GP_API int entityIsGeometry(ENTITY* entity) ; // ID_NO:is not;ID_YES:is
GP_API int entityIsPoint(ENTITY* entity) ; // ID_NO:is not;ID_YES:is
GP_API int entityIsCurve(ENTITY* entity) ; // ID_NO:is not;ID_YES:is
GP_API int entityIsSurface(ENTITY* entity) ; // ID_NO:is not;ID_YES:is
GP_API int entityIsTopology(ENTITY* entity) ; // ID_NO:is not;ID_YES:is
// ��Ҫ����ʵ�ֵĽӿ�
GP_API int entityFree(ENTITY* entity) ;
//-------------------------------------------------------------
typedef void GEOMETRY ;
GEOMETRY* geometryCopy(GEOMETRY* geometry) ; // nt add 2002/12/23
GP_API int geometryMove(GEOMETRY* geometry, VEC3D v) ; // nt add 2002/12/24
GP_API int geometryRotate(GEOMETRY* geometry, PNT3D pivot, VEC3D axis, double angle) ; // nt add 2002/12/24
GP_API int geometryMirror(GEOMETRY* geometry, PNT3D pivot, VEC3D normal) ;
GP_API int geometryToWorld(GEOMETRY* geometry, RFRAME* pLF) ; // from local to world, nt add 2002/12/24
GP_API int geometryToLocal(GEOMETRY* geometry, RFRAME* pLF) ; // from world to local, nt add 2002/12/24
GP_API int geometryScale(GEOMETRY* geometry, double scale) ; // nt add 2002/12/24
//-------------------------------------------------------------
//	(3) curve
typedef void CURVE ;

// ��ѯ�ӿ�
GP_API int curveGetInterval(CURVE* curve, INTERVAL* interval) ;
GP_API int curveGetParam(CURVE* curve, PARAM* param) ;
GP_API int curveGetDisc(CURVE* curve, STRPT** disc) ; // nt modify 2002/8/15
GP_API BSCUR* curveGetNurbs(CURVE* curve) ;
GP_API CURVE* curveCopy(CURVE* curve) ;
// �ֲ�����ӿ�
GP_API int curveEvalPos(CURVE* curve, double t, PNT3D pos) ;
GP_API int curveEvalDer1O(CURVE* curve, double t, PNT3D pos, VEC3D dt) ;
GP_API int curveEvalDer2O(CURVE* curve, double t, PNT3D pos, VEC3D dt, VEC3D dtt) ;
GP_API int curveEvalTangent(CURVE* curve, double t, PNT3D pos, VEC3D tangent) ;
GP_API int curveEvalCur(CURVE* curve, double t, PNT3D pos, VEC3D T, VEC3D N, VEC3D B, double* pcur) ;
GP_API double curveEvalEnergy(CURVE* curve, double u, int flag, double d) ; // nt add 2013/4/5
// ȫ�ּ���ӿ�
GP_API int curveIsIn(CURVE* curve, INTERVAL* interval, double t, int* pyesno) ;
GP_API int curveSnap(CURVE* curve, PNT3D p, PNT3D nearest_p, double* pt) ;
GP_API int curveSnap2(CURVE* curve, PNT3D p, double tol, PNT3D nearest_p, double* pt) ; // nt add 2010/3/24
GP_API int curveDisc(CURVE* curve, INTERVAL* interval, double tol, double ang, double chd, int flags, STRPT** strpt) ;
GP_API int curveCalArcLen(CURVE* curve, INTERVAL* interval, double tol, double* parclen) ;
GP_API int curveCalBox(CURVE* curve, INTERVAL* interval, BOX3D* box) ;
GP_API int curveCalInterval(CURVE* curve, PNT3D p1, VEC3D tvec1, PNT3D p2, VEC3D tvec2, INTERVAL* interval) ;
GP_API int curveIttPln(CURVE* curve, PNT3D pivot, VEC3D normal, double tol, double ang, PNT3D pos, double* pu, int* tan) ;
GP_API int curveItt(CURVE* curve1, CURVE* curve2, double tol, double ang, PNT3D pos, double* pt1, double* pt2, int* tan) ;
GP_API int curveIttInflectionPt2(CURVE* curve, INTERVAL* intv, double tol, PNT3D p, double* pt) ; // nt add 2011/6/20
GP_API int curveCalInflectionPts(CURVE* curve, INTERVAL* interval, double tol, STRPT** strpt) ; // nt add 2011/1
GP_API int curveCalMinRads(CURVE* curve, INTERVAL* interval, double tol, double dTol, double dAng, double dChd, int* inf, STRPT** strpt) ; // nt add 2014/6/19
GP_API int curveCalMinRads2(CURVE* curve, INTERVAL* interval, STRPT** strpt) ; // nt add 2014/6/19
GP_API int curveIttOnOffCur(CURVE* curve1, double* pt1, CURVE* curve2, double* pt2, double r, double tol, PNT3D p) ;
// NURBS�ƽ��ӿ�
GP_API int curveToNurbs(CURVE* curve, INTERVAL* interval, BSCUR** nurbs, int* pexact) ;
// ��˳
GP_API int curveFair(CURVE* curve, INTERVAL* interval, int bndCond, double sc, double tol, int bezier, BSCUR** pnurbs, double* pd) ; // nt add 2011/2/12
GP_API int curvePolish(CURVE* curve, int bndCond, double tol, int bezier, BSCUR** pnurbs, double* pd) ; // nt add 2013/6/12
// ƽ����ת
GP_API int curveMove(CURVE* curve, VEC3D v) ; // nt add 2002/12/24
// ��
GP_API int curveInt(CURVE* curve1, INTERVAL* interval1, CURVE* curve2, INTERVAL* interval2, INTPT** intpts) ;
// serialize
GP_API int curveSave(CURVE* curve, void* ar, int (*arWrite)(void* ar, void* buf, int size)) ;
GP_API int curveLoad(CURVE* curve, void* ar, int (*arRead)(void* ar, void* buf, int size)) ;
/*-----------------------------------------------------------*/
GP_API CURVE* lineCreate(PNT3D p, VEC3D dir) ;
GP_API CURVE* circleCreate(RFRAME* pLF, double r) ;
GP_API CURVE* ellipseCreate(RFRAME* pLF, double r1, double r2) ;
GP_API CURVE* bcurveCreate(BSCUR* nurbs) ;
GP_API CURVE* offcurCreate(CURVE* curve, VEC3D refdir, double dist) ;
//-------------------------------------------------------------
//	(4) surface
/*	2001/9/1 nt
 *	type of intersect curve
 */
enum EIntCurType
{
	icSimple = 0,
	icTangent = 1
} ;

/*	2001/9/1 nt
 *	intersect curve
 */
typedef struct _inter_cur INTCV ; // intersect curve
struct _inter_cur
{
	INTCV*		prev ;
	INTCV*		next ;
	int			type ; // IntCurType
	CURVE*		curve ;
	INTERVAL	interval ;
} ;

GP_API INTCV* intcvCreate(CURVE* curve, INTERVAL* interval) ;
GP_API int intcvFree(INTCV* intcv) ;
/*-----------------------------------------------------------*/
typedef void SURFACE ;

// ��ѯ�ӿ�
GP_API int surfaceGetBoxUV(SURFACE* surface, BOXUV* boxuv) ;
GP_API int surfaceGetBoxUVEx(SURFACE* surface, BOXUV* boxuv) ;
GP_API BSSUR* surfaceGetNurbs(SURFACE* surface) ;
GP_API int surfaceGetParam(SURFACE* surface, PARAM* param_u, PARAM* param_v) ;
GP_API int surfaceGetConicInfo(SURFACE* surface, RFRAME* pLF, double* pD1, double* pD2) ;
// �ֲ�����ӿ�
GP_API int surfaceEvalPos(SURFACE* surface, double u, double v, PNT3D pos) ;
GP_API int surfaceEvalDer1O(SURFACE* surface, double u, double v, PNT3D pos, VEC3D du, VEC3D dv) ;
GP_API int surfaceEvalDer2O(SURFACE* surface, double u, double v, PNT3D pos, VEC3D du, VEC3D dv, VEC3D duu, VEC3D duv, VEC3D dvv) ;
GP_API int surfaceEvalNormal(SURFACE* surface, double u, double v, PNT3D pos, VEC3D normal) ;
GP_API int surfaceEvalDerNor(SURFACE* surface, double u, double v, PNT3D pos, VEC3D du, VEC3D dv, VEC3D normal) ; // nt add 2010/12/30
GP_API int surfaceEvalCur(SURFACE* surface, double u, double v, PNT3D pos, VEC3D normal, VEC3D pri_dir1, VEC3D pri_dir2, double* pcur1, double* pcur2) ;
GP_API int surfaceGetDegeInfo(SURFACE* surface, BOXUV* boxuv, int* pn, PNT3D ps[2], PNT2D uvs[4], VEC3D ns[2]) ; // nt modify 2003/3/28
// ȫ�ּ���ӿ�
GP_API int surfaceSnap(SURFACE* surface, PNT3D p, PNT3D nearest_p, double* pu, double* pv) ;
GP_API int surfaceSnap2(SURFACE* surface, PNT3D p, double tol, PNT3D nearest_p, double* pu, double* pv) ; // nt add 2010/3/24
GP_API int surfaceIttIsoNormal(SURFACE* surface1, SURFACE* surface2, double tol, double ang, PNT2D uv, PNT2D st, PNT3D p1, PNT3D p2) ;
GP_API int surfaceCalBox(SURFACE* surface, BOXUV* boxuv, BOX3D* box) ;
GP_API int surfaceCalDevG0(SURFACE* surface, CURVE* curve, INTERVAL* interval, double* pe) ; // nt add 2011/6/20
GP_API int surfaceCalDevG1(SURFACE* surface1, SURFACE* surface2, CURVE* curve, INTERVAL* interval, double* pe) ; // nt add 2011/6/20
// NURBS�ƽ��ӿ�
GP_API int surfaceToNurbs(SURFACE* surface, BOXUV* boxuv, BSSUR** nurbs, int* pexact) ;
// �Ⱦ�ӿ�
GP_API int surfaceMakeOffset(SURFACE* surface, double d, SURFACE** offset) ;
// ��
GP_API int surfaceIntCurve(SURFACE* surface, BOXUV* boxuv, CURVE* curve, INTERVAL* interval, INTPT** intpts) ; // nt modify 2003/4/27
GP_API int verifyAgbIntCur(SURFACE* surface, BOXUV* boxuv, CURVE* curve, INTERVAL* interval, double step, INTPT** intpts) ;
GP_API int surfaceInt(SURFACE* surface1, BOXUV* boxuv1, SURFACE* surface2, BOXUV* boxuv2, BOX3D* box, INTPT** intpts, INTCV** intcvs) ;
// serialize
GP_API int surfaceSave(SURFACE* surface, void* ar, int (*arWrite)(void* ar, void* buf, int size)) ;
GP_API int surfaceLoad(SURFACE* surface, void* ar, int (*arRead)(void* ar, void* buf, int size)) ;
// ����
GP_API int surfaceItt(SURFACE* surface1, SURFACE* surface2, double tol, INTPT* intpt) ;
GP_API int surfaceIttSurPln(SURFACE* surface1, SURFACE* surface2, PNT3D pivot, VEC3D normal, double tol, PNT3D p, double uv[2], double st[2], VEC3D n1, VEC3D n2) ;
GP_API int surfaceIttCur(SURFACE* surface, BOXUV* boxuv, CURVE* curve, INTERVAL* interval, double tol, PNT3D p, PNT2D uv, double* pt) ;
GP_API int surfaceGetIsoCurve(SURFACE* surface, int uvflag, double uv, CURVE** curve, INTERVAL* interval, PNT3D dege_p) ; // nt add 2002/12/24
// ����
GP_API int surfaceFair(SURFACE* surface, BOXUV* boxuv, int bndCond, double sc, double tol, int bezier, BSSUR** pnurbs, double* pd) ; // nt add 2013/6/20
GP_API int surfacePolish(SURFACE* surface, int bndCond, double tol, int bezier, BSSUR** pnurbs, double* pd) ; // nt add 2013/6/20
/*-----------------------------------------------------------*/
GP_API SURFACE* planeCreate(RFRAME* pLF) ;
GP_API SURFACE* cylinderCreate(RFRAME* pLF, double r) ;
GP_API SURFACE* coneCreate(RFRAME* pLF, double tanHalfAng) ;
GP_API SURFACE* sphereCreate(RFRAME* pLF, double r) ;
GP_API SURFACE* torusCreate(RFRAME* pLF, double r1, double r2) ;
GP_API SURFACE* bsurfaceCreate(BSSUR* nurbs) ;
GP_API SURFACE* rulsurCreate(CURVE* profile, VEC3D dir) ;
GP_API SURFACE* revsurCreate(CURVE* profile, PNT3D p, VEC3D axis) ;
GP_API SURFACE* sweepCreate(CURVE* profile, CURVE* spine, RFRAME* pLF) ;
GP_API SURFACE* offsetCreate(SURFACE* underlying, double d) ;
GP_API SURFACE* comsurCreate(SURFACE* underlying, BSSUR* nurbs) ; // nt add 2011/6/28
///////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif