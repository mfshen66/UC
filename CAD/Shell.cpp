#include "stdafx.h"
#include "Shell.h"

void runexe(CString ex)//CSY
{

	USES_CONVERSION;
	LPCWSTR path = A2CW(W2A(ex));
	char* szStr1 = "open";
	CString str1 = CString(szStr1);
	LPCWSTR wszClassName1 = A2CW(W2A(str1));
	str1.ReleaseBuffer();
	ShellExecute(NULL, wszClassName1, path, NULL, NULL, SW_SHOW);
	//LPSTR path = T2A(ex);
	//system(path);
	//LPCSTR path = (LPCSTR)ex.GetBuffer();
	//ShellExecute(NULL, _T("open"),_T("path"), NULL, NULL, SW_SHOWNORMAL);
	//rt = WinExec(path ,SW_SHOWDEFAULT);
	/*rt = WinExec(_T("1.txt"), SW_SHOWDEFAULT);//  返回2 没有找到路径。
	if (rt<32)
	{
	mess.Format(_T("%d"),rt);
	AfxMessageBox(mess);
	}*/

}

DWORD WINAPI DISPLAY1(LPVOID lpParameter)//CSY,新线程
{
	CString    sPath;
	CString    txtPath;//用来传递要传输的文件的位置
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);

	//	AfxMessageBox(sPath);
	CString TCPpath = _T("\\TCP1.exe");
	/*if (sPath == _T("E:\\实验室任务\\3DPrint2017-5-19toLBS\\3DPrint2017-5-19toLBS\\Debug"))
	AfxMessageBox(_T("1"));
	else
	AfxMessageBox(_T("0"));*/
	sPath = sPath + TCPpath;
	//CString path = _T("E:\\实验室任务\\3DPrint2017-5-19toLBS\\3DPrint2017-5-19toLBS\\Debug\\TCP.exe");
	/*for (int i = 0; i < 50; i++)
	{
	if (sPath[i] == path[i])
	;
	else
	AfxMessageBox(_T("错"));
	}*/

	//AfxMessageBox(sPath);
	//CString path = _T("notepad f:\\deng\\001.TXT");//ok
	runexe(sPath);
	//CString path9= _T( "E:\\1.txt");//ok
	//runexe(path9);
	return 0;
}