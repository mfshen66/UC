// ParameterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CAD.h"
#include "CADDoc.h" // nt add 2017/8/19
#include "ParameterDlg.h"
#include "afxdialogex.h"


// CParameterDlg 对话框

IMPLEMENT_DYNAMIC(CParameterDlg, CDialogEx)

CParameterDlg::CParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PARAMETERDLG, pParent)
{
	parameterInit(&m_parameter) ;
	m_pathName = _T("") ;
}

CParameterDlg::~CParameterDlg()
{
}

void CParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_grid);
}


BEGIN_MESSAGE_MAP(CParameterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CParameterDlg::OnClickedMfcbutton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CParameterDlg::OnClickedMfcbutton2)
	ON_BN_CLICKED(IDC_MFCBUTTON3, &CParameterDlg::OnClickedMfcbutton3)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

void CParameterDlg::InitProperty()
{
	CMFCPropertyGridProperty* pGroup, *pItem ;

	// 打印机
	pGroup = new CMFCPropertyGridProperty(_T("打印机")) ;
	pItem = new CMFCPropertyGridProperty(_T("打印机型号"), m_parameter.printer, _T("字符串")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("打印区域X向尺寸"), m_parameter.L, _T("浮点数")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("打印区域Y向尺寸"), m_parameter.W, _T("浮点数")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("打印区域Z向尺寸"), m_parameter.H, _T("浮点数")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("打印X向分辨率"), (long)m_parameter.PL, _T("整型")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("打印Y向分辨率"), (long)m_parameter.PW, _T("整型")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("光机投射原点定义"), (long)m_parameter.P0, _T("整型")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("光机投射X'轴方向定义"), (long)m_parameter.Dx, _T("整型")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// 材料
	pGroup = new CMFCPropertyGridProperty(_T("材料")) ;
	pItem = new CMFCPropertyGridProperty(_T("材料型号"), m_parameter.material, _T("字符串")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("第一层切片层厚t0"), m_parameter.ts[0], _T("μm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("最小层厚t1"), m_parameter.ts[1], _T("μm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t2"), m_parameter.ts[2], _T("μm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t3"), m_parameter.ts[3], _T("μm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t4"), m_parameter.ts[4], _T("μm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("最大层厚t5"), m_parameter.ts[5], _T("μm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t0曝光光强"), (long)m_parameter.lis[0], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t1曝光光强"), (long)m_parameter.lis[1], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t2曝光光强"), (long)m_parameter.lis[2], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t3曝光光强"), (long)m_parameter.lis[3], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t4曝光光强"), (long)m_parameter.lis[4], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t5曝光光强"), (long)m_parameter.lis[5], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	// nt add 2017/6/27
	pItem = new CMFCPropertyGridProperty(_T("层厚t1小光强曝光光强"), (long)m_parameter.lils[0], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t2小光强曝光光强"), (long)m_parameter.lils[1], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t3小光强曝光光强"), (long)m_parameter.lils[2], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t4小光强曝光光强"), (long)m_parameter.lils[3], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t5小光强曝光光强"), (long)m_parameter.lils[4], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	// end
	pItem = new CMFCPropertyGridProperty(_T("层厚t0曝光时间"), m_parameter.lts[0], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t1曝光时间"), m_parameter.lts[1], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t2曝光时间"), m_parameter.lts[2], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t3曝光时间"), m_parameter.lts[3], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t4曝光时间"), m_parameter.lts[4], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t5曝光时间"), m_parameter.lts[5], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	// nt add 2017/6/27
	pItem = new CMFCPropertyGridProperty(_T("层厚t1小光强曝光时间"), m_parameter.ltls[0], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t2小光强曝光时间"), m_parameter.ltls[1], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t3小光强曝光时间"), m_parameter.ltls[2], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t4小光强曝光时间"), m_parameter.ltls[3], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("层厚t5小光强曝光时间"), m_parameter.ltls[4], _T("s")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	// end
	m_grid.AddProperty(pGroup) ;

	// 一号电机
	pGroup = new CMFCPropertyGridProperty(_T("一号电机")) ;
	pItem = new CMFCPropertyGridProperty(_T("最高转速1"), m_parameter.w1max, _T("rpm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("最低转速1"), m_parameter.w1min, _T("rpm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("拔起速度"), m_parameter.w1p, _T("rpm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("变速起始距离"), m_parameter.L1, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("变速结束距离"), m_parameter.L2, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("打印过程上升停止距离"), m_parameter.L3, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// 二号电机
	pGroup = new CMFCPropertyGridProperty(_T("二号电机")) ;
	pItem = new CMFCPropertyGridProperty(_T("最高转速2"), m_parameter.w2max, _T("rpm")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("最低转速2"), m_parameter.w2min, _T("rmp")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("转动角度"), m_parameter.beta, _T("deg")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// 灰度渐变
	pGroup = new CMFCPropertyGridProperty(_T("灰度渐变")) ;
	pItem = new CMFCPropertyGridProperty(_T("像素点1灰度"), (long)m_parameter.grays[0], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("像素点2灰度"), (long)m_parameter.grays[1], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("像素点3灰度"), (long)m_parameter.grays[2], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("像素点4灰度"), (long)m_parameter.grays[3], _T("0~100%")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	// 高度
	pItem = new CMFCPropertyGridProperty(_T("打印平台缺省高度"), m_parameter.H0, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;

	// 灰度最小可处理尺度, nt add 2017/6/1
	pItem = new CMFCPropertyGridProperty(_T("灰度最小可处理尺度"), (long)m_parameter.wp, _T("μm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;

	// nt add 2017/6/27
	pItem = new CMFCPropertyGridProperty(_T("机械零点"), m_parameter.LDdelta, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("光栅尺当前位置"), m_parameter.LD0, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("光机能量系数"), m_parameter.LNF, _T("缺省值0.9")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("投射距离"), m_parameter.LF, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("出光口X宽度"), m_parameter.LENx, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("出光口Y宽度"), m_parameter.LENy, _T("mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("料盒折射率"), m_parameter.n, _T("缺省值1.5")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("料盒厚度"), m_parameter.tmc, _T("缺省值10.mm")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("料盒齿轮减速比"), m_parameter.ratio, _T("缺省值7.5")) ;
	pItem->AllowEdit(TRUE) ;
	m_grid.AddProperty(pItem) ;
	//end

	// 预留
	pGroup = new CMFCPropertyGridProperty(_T("预留")) ;
	pItem = new CMFCPropertyGridProperty(_T("浮点参数0"), m_parameter.tps[0], _T("浮点")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("浮点参数1"), m_parameter.tps[1], _T("浮点")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("浮点参数2"), m_parameter.tps[2], _T("浮点")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("浮点参数3"), m_parameter.tps[3], _T("浮点")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("浮点参数4"), m_parameter.tps[4], _T("浮点")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("整型参数0"), (long)m_parameter.tis[0], _T("整型")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("整型参数1"), (long)m_parameter.tis[1], _T("整型")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("整型参数2"), (long)m_parameter.tis[2], _T("整型")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("整型参数3"), (long)m_parameter.tis[3], _T("整型")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	pItem = new CMFCPropertyGridProperty(_T("整型参数4"), (long)m_parameter.tis[4], _T("整型")) ;
	pItem->AllowEdit(TRUE) ;
	pGroup->AddSubItem(pItem) ;
	m_grid.AddProperty(pGroup) ;

	return ;
}

// CParameterDlg 消息处理程序
BOOL CParameterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_grid.EnableHeaderCtrl(TRUE, _T("参数"), _T("值")) ;
	m_grid.SetVSDotNetLook(TRUE);
	m_grid.EnableDescriptionArea();
	m_grid.MarkModifiedProperties();
	HDITEM item ;
	item.cxy = 240 ;
	item.mask = HDI_WIDTH ;
	m_grid.GetHeaderCtrl().SetItem(0, &item) ;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 打开打印机材料参数文件
void CParameterDlg::OnClickedMfcbutton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE);
    dlg.m_ofn.lpstrFilter = _T("P_M文件(*.p_m)") ;
    dlg.m_ofn.lpstrTitle = _T("参数文件") ;
	CString filePath ;
	efpGet(filePath) ; // 运行文件目录
	filePath += _T("\\Libs\\P_M") ;
	dlg.m_ofn.lpstrInitialDir = filePath.GetBuffer(0) ;
	if( dlg.DoModal() == IDOK )
	{
		m_pathName = dlg.GetPathName() ;
		PARAMETER parameter ;
		if( parameterLoad(&parameter, m_pathName) == 1 )
		{
			m_parameter = parameter ;
			if( m_grid.GetPropertyCount() > 0 )
				ResetAllProperty() ;
			else
				InitProperty() ;
		}
		else
			AfxMessageBox(_T("打开文件错误!")) ;
	}

	return ;
}

// 保存打印机材料参数文件
void CParameterDlg::OnClickedMfcbutton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CCADDoc* pDoc = cadGetDoc() ; // nt add 2017/8/19
	if( m_pathName != _T("") )
	{
		parameterResByLimits(&m_parameter) ; // nt add 2017/8/20
		parameterSave(&m_parameter, m_pathName) ;
		// nt add 2017/8/19
		if( _tcscmp(m_parameter.printer, pDoc->m_parameter.printer) == 0 &&
			_tcscmp(m_parameter.material, pDoc->m_parameter.material) == 0 )
			pDoc->m_parameter = m_parameter ;
		// end
	}
	else
		AfxMessageBox(_T("先打开参数数据文件!")) ;
}

// 另存打印机材料参数文件
void CParameterDlg::OnClickedMfcbutton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pathName != _T("") )
	{
		CFileDialog dlg(FALSE) ;
		dlg.m_ofn.lpstrFilter = _T("P_M文件(*.p_m)") ;
		dlg.m_ofn.lpstrTitle = _T("参数文件") ;
		if( dlg.DoModal() == IDOK )
		{
			m_pathName = dlg.GetPathName() ;
			parameterResByLimits(&m_parameter) ; // nt add 2017/8/20
			parameterSave(&m_parameter, m_pathName) ;
		}
	}
	else
		AfxMessageBox(_T("先打开参数数据文件!")) ;

	return ;
}


void CParameterDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}

LRESULT CParameterDlg::OnPropertyChanged(WPARAM wParam,LPARAM lParam)
{
	CMFCPropertyGridProperty* pItem = (CMFCPropertyGridProperty*)lParam ;
	GetPropertyValue(pItem) ;

	return 0;
}

// 根据m_parameter从新给所有的property赋值
void CParameterDlg::ResetAllProperty()
{
	int i, j, m, n = m_grid.GetPropertyCount() ;
	CMFCPropertyGridProperty* pGroup, *pItem ;
	CString name ;
	for( i = 0 ; i < n ; i++ ) // 遍历第一层property
	{
		pGroup = m_grid.GetProperty(i) ;
		m = pGroup->GetSubItemsCount() ;
		for( j = 0 ; j < m ; j++ ) // 遍历第二层property
		{
			pItem = pGroup->GetSubItem(j) ;
			SetPropertyValue(pItem) ;
		}
		SetPropertyValue(pGroup) ;
	}

	return ;
}

// nt add 2017/5/9 从字符串中提取一个整数
int cstrGetInt(CString& str)
{
	int i, n = str.GetLength() ;
	CString buf ;

	for( i = 0 ; i < n ; i++ )
	{
		if( '0' <= str[i] && str[i] <= '9' )
			buf.AppendChar(str[i]) ;
	}
	if( buf.GetLength() > 0 )
		return _tstoi(buf) ;
	else
		return -1 ;
}

void CParameterDlg::GetPropertyValue(CMFCPropertyGridProperty* pProperty)
{
	CString name = pProperty->GetName() ;
	_variant_t v = pProperty->GetValue() ;
	int k ;
	if( name == _T("打印机型号") )
		_tcscpy_s(m_parameter.printer, 64, (CString)v) ;
	else
	if( name == _T("打印区域X向尺寸") )
		m_parameter.L = v ;
	else
	if( name == _T("打印区域Y向尺寸") )
		m_parameter.W = v ;
	else
	if( name == _T("打印区域Z向尺寸") )
		m_parameter.H = v ;
	else
	if( name == _T("打印X向分辨率") )
		m_parameter.PL = v ;
	else
	if( name == _T("打印Y向分辨率") )
		m_parameter.PW = v ;
	else
	if( name == _T("光机投射原点定义") )
		m_parameter.P0 = v ;
	else
	if( name == _T("光机投射X'轴方向定义") )
		m_parameter.Dx = v ;
	else
	if( name == _T("材料型号") )
		_tcscpy_s(m_parameter.material, 64, (CString)v) ;
	else
	if( name.Find(_T("层厚t")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 6 )
		{
			if( name.Find(_T("曝光光强")) >= 0 )
			{
				if( name.Find(_T("小")) >= 0 )
					m_parameter.lils[k-1] = v ;
				else
					m_parameter.lis[k] = v ;
			}
			else
			if( name.Find(_T("曝光时间")) >= 0 )
			{
				if( name.Find(_T("小")) >= 0 )
					m_parameter.ltls[k-1] = v ;
				else
					m_parameter.lts[k] = v ;
			}
			else
				m_parameter.ts[k] = v ;
		}
	}
	else
	if( name == _T("最高转速1") )
		m_parameter.w1max = v ;
	else
	if( name == _T("最低转速1") )
		m_parameter.w1min = v ;
	else
	if( name == _T("拔起速度") )
		m_parameter.w1p = v ;
	else
	if( name == _T("变速起始距离") )
		m_parameter.L1 = v ;
	else
	if( name == _T("变速结束距离") )
		m_parameter.L2 = v ;
	else
	if( name == _T("打印过程上升停止距离") )
		m_parameter.L3 = v ;
	else
	if( name == _T("最高转速2") )
		m_parameter.w2max = v ;
	else
	if( name == _T("最低转速2") )
		m_parameter.w2min = v ;
	else
	if( name == _T("转动角度") )
		m_parameter.beta = v ;
	else
	if( name.Find(_T("像素点")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 1 && k <= 4 )
			m_parameter.grays[k-1] = v ;
	}
	else
	if( name == _T("打印平台缺省高度") )
		m_parameter.H0 = v ;
	else
	if( name == _T("灰度最小可处理尺度") ) // nt add 2017/6/1
		m_parameter.wp = v ;
	else
	if( name == _T("机械零点") )
		m_parameter.LDdelta = v ;
	else
	if( name == _T("光栅尺当前位置") )
		m_parameter.LD0 = v ;
	else
	if( name == _T("光机能量系数") )
		m_parameter.LNF = v ;
	else
	if( name == _T("投射距离") )
		m_parameter.LF = v ;
	else
	if( name == _T("出光口X宽度") )
		m_parameter.LENx = v ;
	else
	if( name == _T("出光口Y宽度") )
		m_parameter.LENy = v ;
	else
	if( name == _T("料盒折射率") )
		m_parameter.n = v ;
	else
	if( name == _T("料盒厚度") ) // nt add 2017/8/20
		m_parameter.tmc = v ;
	else
	if( name == _T("料盒齿轮减速比") ) // nt add 2017/8/20
		m_parameter.ratio = v ;
	else
	if( name.Find(_T("浮点参数")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 5 )
			m_parameter.tps[k] = v ;
	}
	else
	if( name.Find(_T("整型参数")) >=0 ) // nt add 2017/8/20
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 5 )
			m_parameter.tis[k] = v ;
	}

	// nt add 2017/8/21
	if( parameterResByLimits(&m_parameter) > 0 )
		AfxMessageBox(_T("参数超出了取值范围!")) ;

	return ;
}

void CParameterDlg::SetPropertyValue(CMFCPropertyGridProperty* pProperty)
{
	CString name ;

	if( pProperty == NULL ||
		pProperty->GetSubItemsCount() > 0 )
		return ; // 不处理这种情况的property
	name = pProperty->GetName() ;

	int k ;
	if( name == _T("打印机型号") )
		pProperty->SetValue(m_parameter.printer) ;
	else
	if( name == _T("打印区域X向尺寸") )
		pProperty->SetValue(m_parameter.L) ;
	else
	if( name == _T("打印区域Y向尺寸") )
		pProperty->SetValue(m_parameter.W) ;
	else
	if( name == _T("打印区域Z向尺寸") )
		pProperty->SetValue(m_parameter.H) ;
	else
	if( name == _T("打印X向分辨率") )
		pProperty->SetValue((long)m_parameter.PL) ;
	else
	if( name == _T("打印Y向分辨率") )
		pProperty->SetValue((long)m_parameter.PW) ;
	else
	if( name == _T("光机投射原点定义") )
		pProperty->SetValue((long)m_parameter.P0) ;
	else
	if( name == _T("光机投射X'轴方向定义") )
		pProperty->SetValue((long)m_parameter.Dx) ;
	else
	if( name == _T("材料型号") )
		pProperty->SetValue(m_parameter.material) ;
	else
	if( name.Find(_T("层厚t")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 6 )
		{
			if( name.Find(_T("曝光光强")) >= 0 )
			{
				if( name.Find(_T("小")) >= 0 )
					pProperty->SetValue((long)m_parameter.lils[k-1]) ;
				else
					pProperty->SetValue((long)m_parameter.lis[k]) ;
			}
			else
			if( name.Find(_T("曝光时间")) >= 0 )
			{
				if( name.Find(_T("小")) >= 0 )
					pProperty->SetValue(m_parameter.ltls[k-1]) ;
				else
					pProperty->SetValue(m_parameter.lts[k]) ;
			}
			else
				pProperty->SetValue(m_parameter.ts[k]) ;
		}
	}
	else
	if( name == _T("最高转速1") )
		pProperty->SetValue(m_parameter.w1max) ;
	else
	if( name == _T("最低转速1") )
		pProperty->SetValue(m_parameter.w1min) ;
	else
	if( name == _T("拔起速度") )
		pProperty->SetValue(m_parameter.w1p) ;
	else
	if( name == _T("变速起始距离") )
		pProperty->SetValue(m_parameter.L1) ;
	else
	if( name == _T("变速结束距离") )
		pProperty->SetValue(m_parameter.L2) ;
	else
	if( name == _T("打印过程上升停止距离") )
		pProperty->SetValue(m_parameter.L3) ;
	else
	if( name == _T("最高转速2") )
		pProperty->SetValue(m_parameter.w2max) ;
	else
	if( name == _T("最低转速2") )
		pProperty->SetValue(m_parameter.w2min) ;
	else
	if( name == _T("转动角度") )
		pProperty->SetValue(m_parameter.beta) ;
	else
	if( name.Find(_T("像素点")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 1 && k <= 4 )
			pProperty->SetValue((long)m_parameter.grays[k-1]) ;
	}
	else
	if( name == _T("打印平台缺省高度") )
		pProperty->SetValue(m_parameter.H0) ;
	else
	if( name == _T("灰度最小可处理尺度") ) // nt add 2017/6/1
		pProperty->SetValue((long)m_parameter.wp) ;
	else
	if( name == _T("机械零点") )
		pProperty->SetValue(m_parameter.LDdelta) ;
	else
	if( name == _T("光栅尺当前位置") )
		pProperty->SetValue(m_parameter.LD0) ;
	else
	if( name == _T("光机能量系数") )
		pProperty->SetValue(m_parameter.LNF) ;
	else
	if( name == _T("投射距离") )
		pProperty->SetValue(m_parameter.LF) ;
	else
	if( name == _T("出光口X宽度") )
		pProperty->SetValue(m_parameter.LENx) ;
	else
	if( name == _T("出光口Y宽度") )
		pProperty->SetValue(m_parameter.LENy) ;
	else
	if( name == _T("料盒折射率") )
		pProperty->SetValue(m_parameter.n) ;
	else
	if( name == _T("料盒厚度") )
		pProperty->SetValue(m_parameter.tmc) ;
	else
	if( name == _T("料盒齿轮减速比") )
		pProperty->SetValue(m_parameter.ratio) ;
	else
	if( name.Find(_T("浮点参数")) >=0 )
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 5 )
			pProperty->SetValue(m_parameter.tps[k]) ;
	}
	else
	if( name.Find(_T("整型参数")) >=0 ) // nt add 2017/8/20
	{
		k = cstrGetInt(name) ;
		if( k >= 0 && k < 5 )
			pProperty->SetValue((long)m_parameter.tis[k]) ;
	}

	return ;
}
