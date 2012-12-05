
#pragma once

#ifndef MyExt_h__
#define MyExt_h__



//------------------------------------------------------------------------------
// ���ַ�����ԴID�����ַ��������ʧ�ܾͷ���NULL���򷵻��ַ���
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


// ֻ�ж�����MY_EXT_ERR_URL������ShowError��������ʵFooter
// #define MY_EXT_ERR_URL	_TEXT("http://hi.baidu.com/new/kingsollyu")

// �Զ��� CTaskDialogEx �࣬��Ҫʵ�ֵ������
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
		__in LPCTSTR szCaption   = _TEXT("��ʾ"),
		__in LPCWSTR pszResource = TD_INFORMATION_ICON,
		__in LPCTSTR lpszFooter  = _TEXT("") );



	static CString   GetRunPath();


	static DWORD LoadLnkFile(
		__in  CString strLnkPath,			// ��ݷ�ʽ�ļ�·��
		__out CString& strFilePath );		// ����·��



	static DWORD WirteRegString( 
		__in HKEY hKey,						// ����ֵ
		__in LPCTSTR szPath,				// ��·��
		__in LPCTSTR szKeyName,				// д������
		__in LPCTSTR szData );				// д������



	static DWORD ReadRegString( 
		__out LPWSTR szData,
		__in HKEY hKey,						// ����ֵ
		__in LPCTSTR szSubKey,				// ��·��
		__in LPCTSTR szKeyName = _TEXT(""),	// ��ȡ����
		__in DWORD dwDataLen = MAX_PATH );	// ���峤��


	static CString GetTempFilePath(
		__in const LPCTSTR  lpszPrefixString = _TEXT("sl_")); // ��ʱ�ļ���ǰ׺


	// �����ҳԴ��
	static CString GetWebSrcCode(
		__in const LPCTSTR lpczUrl,			// Ҫ��õĵ�ַ
		__in const LPCTSTR lpszSave);		// ��Դ�ļ������ڱ����ļ�


	// ���ļ��м���ͼ��
	static HICON LoadIcon( 
		__in LPCTSTR szDllPath ,			// ӵ��ͼ���ļ���·��
		__in  UINT nIndex );				// ͼ�������ֵ


	// ��鵱ǰ���е�����Ƿ�Ϊ����Ա
	static BOOL isAdministrator();

	// �����ı���ճ����
	static void SetClipboardData(
		__in const CString strText );
	
	// �ж�ϵͳ�Ƿ�Ϊ64λϵͳ
	static BOOL Is64Bit_OS();
};

#endif // MyExt_h__
