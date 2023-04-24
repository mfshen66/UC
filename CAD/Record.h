/*	2010/4/9 nt
 *	header file of class CRecord
 */
#ifndef _Record_h_
#define _Record_h_

///////////////////////////////////////////////////////////////
typedef CObArray COA ;
struct GridModel;
//--------------------------------------------------------------
// 记录系统的建模过程：
// (1)生成图元
// (2)删除图元
// (3)修改图元
//--------------------------------------------------------------
// CRecord的类型
enum ERecordType
{
	rtRDelete = 0,
	rtRCreate = 1,
	rtRCut = 2,
	rtRCopy = 3,
	rtRSize = 4,
	rtRHeight = 5,
	rtRDir = 6,
	rtRMakeSS = 7,
	rtRDelSS = 8, // delete all
	rtREditSupport = 9,
	rtREditRod = 10
} ;
//--------------------------------------------------------------
class CRecord : public CObject
{
public:
	STL* m_stl ;
	GridModel* m_Grid;
public:
	CRecord() ;
	~CRecord() ;

	virtual int GetType() = 0 ;
	STL* GetSTL() ;
	void SetSTL(STL* stl) ;
	GridModel* GetGridModel();
	void SetGridModel(GridModel* pGM);
} ;
//--------------------------------------------------------------
class CRCreate : public CRecord
{
public:
public:
	CRCreate(STL* stl) ;
	CRCreate(GridModel* pGM) ;
	~CRCreate() ;

	int GetType() ;
} ;
//--------------------------------------------------------------
class CRDelete : public CRecord
{
public:
public:
	CRDelete(STL* stl) ;
	~CRDelete() ;

	int GetType() ;
} ;
//--------------------------------------------------------------
class CRCut : public CRecord
{
public:
public:
	CRCut(STL* stl) ;
	~CRCut() ;

	int GetType() ;
} ;
//--------------------------------------------------------------
class CRCopy : public CRecord
{
public:
public:
	CRCopy(STL* stl) ;
	~CRCopy() ;

	int GetType() ;
} ;
//--------------------------------------------------------------
class CRSize : public CRecord
{
public:
	PNT3D m_center ;
	double m_scale ;
public:
	CRSize(STL* stl, PNT3D center, double scale) ;
	~CRSize() ;

	int GetType() ;
} ;
//--------------------------------------------------------------
class CRHeight : public CRecord
{
public:
	double m_z ;
	SS* m_ss ;
public:
	CRHeight(STL* stl, double z, SS* ss) ;
	~CRHeight() ;

	int GetType() ;
} ;
//--------------------------------------------------------------
class CRDir : public CRecord
{
public:
	int m_n ;
	TRANS* m_ts ;
public:
	CRDir(STL* stl, int n, TRANS* ts) ;
	~CRDir() ;

	int GetType() ;
} ;
//--------------------------------------------------------------
class CRMakeSS : public CRecord
{
public:
	SS* m_created ;
	SS* m_old ;
public:
	CRMakeSS(STL* stl, SS* created, SS* old) ;
	~CRMakeSS() ;

	int GetType() ;
} ;
//--------------------------------------------------------------
class CRDelSS : public CRecord
{
public:
	SS* m_ss ;
public:
	CRDelSS(STL* stl, SS* ss) ;
	~CRDelSS() ;

	int GetType() ;
} ;
///////////////////////////////////////////////////////////////

#endif
