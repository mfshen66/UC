/*	2000/8/19 nt
 *	header file of class doc command
 */
#pragma once

///////////////////////////////////////////////////////////////
/*	2002/9 nt
 *	doc command type
 */
enum EDocCmdType
{
	dctPaste = 0,
	dctEditSupport = 1,
	dctEditRod = 2
} ;
//-------------------------------------------------------------
class CPaste : public CCommand
{
private:
	PNT3D m_insertPos ;
	STL* m_stl ;
	PNT3D m_p ;

public:
	CPaste(CCADDoc* pDoc, CCADView* pView, PNT3D insertPos, STL* stl) ;
	~CPaste() ;

	void Notify(int msg) ;
	int GetID() ;
	int	OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int	OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int	OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnReturn() ;
	int	Cancel() ;
} ;
//--------------------------------------------------------------
// 2017/3/28 nt
class CEditSupport : public CCommand
{
private:
	SSP m_ssp ;
	STL* m_stl ;
	int m_pick ; // 1=在STL模型上找到了一点及法矢
	PNT3D m_p ;
	VEC3D m_normal ;
	int m_lastId ; // 最后生成的SUPPORT的Id
	int m_arrow ; // 1=绘制了箭头
	
	SUPPORT* m_old ; // 拾取到的已有的支撑
	int m_support ; // 1=绘制了m_old

	SS* m_copy ; // nt add 2017/9/4 old SS

public:
	CEditSupport(CCADDoc* pDoc, CCADView* pView, STL* stl, SSP* ssp) ;
	~CEditSupport() ;

	void Notify(int msg) ;
	int GetID() ;
	int	OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int	OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int	OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnReturn() ;
	int	Cancel() ;
	void DrawArrow(void* pVI) ;
	void ErazeArrow(void* pVI) ;
	void DrawSupport(void* pVI) ;
	void ErazeSupport(void* pVI) ;
	void DrawBall(PNT3D center, double r, void* pVI) ;
} ;
//--------------------------------------------------------------
// 2017/6/4 nt
typedef struct _sr SR ; // SUPPORT or ROD
struct _sr
{
	int type ; // 0=SUPPORT, 1=ROD
	void* entity ; // SUPPORT or ROD
	int drawed ; // 0=not draw, 1=drawed
	int drawMode ; // dmNormal, dmPrompt, dmSelect, dmEraze
	PNT3D p ;
} ;

void srInit(SR* sr) ;
void srInit2(SR* sr, int type, void* entity, PNT3D p) ;
void srDraw(SR* sr, void* pVI, int drawMode) ;
//--------------------------------------------------------------
class CEditRod : public CCommand
{
private:
	double m_r ;
	STL* m_stl ;

	SR m_selected ;
	SR m_prompting ;

	SS* m_copy ; // nt add 2017/9/4 old SS

public:
	CEditRod(CCADDoc* pDoc, CCADView* pView, STL* stl, double r) ;
	~CEditRod() ;

	SS* GetSS() ;
	int Make(SUPPORT* support1, SUPPORT* support2, PNT3D p1, PNT3D p2) ;
	void AddPromptEntity(int type, void* entity, PNT3D p) ;
	void Notify(int msg) ;
	int GetID() ;
	int	OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int	OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int	OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnReturn() ;
	int	Cancel() ;
	int Del() ;
} ;
///////////////////////////////////////////////////////////////
