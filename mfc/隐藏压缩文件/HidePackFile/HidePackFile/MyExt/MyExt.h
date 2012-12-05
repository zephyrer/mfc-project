
#pragma once

#ifndef MyExt_h__
#define MyExt_h__



//------------------------------------------------------------------------------
// 从字符串资源ID加载字符串，如果失败就返回NULL否则返回字符串
// #define LOADSTRING(x) CString(MAKEINTRESOURCE(x))
//------------------------------------------------------------------------------
inline LPCTSTR LOADSTRING(int nID)
{
	const int MAX_BUFF = 500;

#ifdef UNICODE
	static TCHAR szChar[MAX_BUFF] = {0};
#else
	static char szChar[MAX_BUFF] = {0};
#endif // UNICODE

	int nCopy = LoadString(NULL,nID,szChar,MAX_BUFF);

	if ( nCopy != 0) return szChar;
	return NULL;
};


// 只有定义了MY_EXT_ERR_URL都会在ShowError函数中现实Footer
// #define MY_EXT_ERR_URL	_TEXT("http://hi.baidu.com/new/kingsollyu")

// 自定义 CTaskDialogEx 类，主要实现点击连接
class CTaskDialogEx : public CXTPTaskDialog
{
public:
	CTaskDialogEx(CWnd* pWndParent)
		: CXTPTaskDialog(pWndParent)
	{

	}
protected:
	virtual void OnHyperlinkClicked(LPCTSTR pszURL)
	{
		ShellExecute(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), _T("open"),pszURL, NULL, NULL, SW_SHOWNORMAL);
	}
};


class CMyExt
{
public:
	CMyExt(void);
	virtual ~CMyExt(void);


private:
	CString m_strErrURL;

public:
	CString GetErrURL() const 		{ return m_strErrURL; }
	void 	SetErrURL(CString val)	{ m_strErrURL = val;  }

	static CString GetErrorMsg( __in const DWORD dwError );

	static LONG ShowError(
		__in LPCTSTR szErr,
		__in int nErr = -1,
		__in TASKDIALOG_COMMON_BUTTON_FLAGS nButton = TDCBF_OK_BUTTON,
		__in LPCTSTR szCaption   = _TEXT("提示"),
		__in LPCWSTR pszResource = TD_INFORMATION_ICON,
		__in LPCTSTR lpszFooter  = _TEXT("") );



	static CString   GetRunPath();


	static DWORD LoadLnkFile(
		__in  CString strLnkPath,			// 快捷方式文件路径
		__out CString& strFilePath );		// 返回路径



	static DWORD WirteRegString( 
		__in HKEY hKey,						// 主键值
		__in LPCTSTR szPath,				// 子路径
		__in LPCTSTR szKeyName,				// 写入名称
		__in LPCTSTR szData );				// 写入内容



	static DWORD ReadRegString( 
		__out LPWSTR szData,
		__in HKEY hKey,						// 主键值
		__in LPCTSTR szSubKey,				// 子路径
		__in LPCTSTR szKeyName = _TEXT(""),	// 读取名称
		__in DWORD dwDataLen = MAX_PATH );	// 缓冲长度


	static CString GetTempFilePath(
		__in const LPCTSTR  lpszPrefixString = _TEXT("sl_")); // 临时文件的前缀


	// 获得网页源码
	static CString GetWebSrcCode(
		__in const LPCTSTR lpczUrl,			// 要获得的地址
		__in const LPCTSTR lpszSave);		// 将源文件保存在本地文件


	// 从文件中加载图标
	static HICON LoadIcon( 
		__in LPCTSTR szDllPath ,			// 拥有图标文件的路径
		__in  UINT nIndex );				// 图标的索引值


	// 检查当前运行的身份是否为管理员
	static BOOL isAdministrator();

	// 设置文本到粘贴板
	static void SetClipboardData(
		__in const CString strText );
	
	// 判断系统是否为64位系统
	static BOOL Is64Bit_OS();
};

#endif // MyExt_h__
