#include <stdafx.h>
#include "MyExt.h"

#include <wininet.h>
#pragma comment(lib, "wininet.lib") 

typedef void (WINAPI *LPFN_PGNSI)(LPSYSTEM_INFO);

CMyExt::CMyExt(void)
{
}

CMyExt::~CMyExt(void)
{
}

CString CMyExt::GetErrorMsg( __in const DWORD dwError )
{
	LPVOID lpMsgBuf;

	BOOL isOk = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,  
		NULL,dwError,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0, NULL );  

	if (isOk != FALSE && (&lpMsgBuf != NULL))
		return CString ((LPTSTR)lpMsgBuf); 


	return CString ((LPTSTR)lpMsgBuf);
}

LONG CMyExt::ShowError( __in LPCTSTR szErr, __in int nErr /*= -1*/, __in TASKDIALOG_COMMON_BUTTON_FLAGS nButton /*= TDCBF_OK_BUTTON*/, __in LPCTSTR szCaption /*= _TEXT("����")*/, __in LPCWSTR pszResource /*= TD_ERROR_ICON*/, __in LPCTSTR lpszFooter /*= _TEXT("")*/ )
{
	CString strErr(L"");


	CTaskDialogEx taskDlg(AfxGetApp()->GetMainWnd());
	taskDlg.EnableHyperlinks(TRUE);

	taskDlg.SetCommonButtons(nButton);
#ifdef MY_EXT_ERR_URL
		CString strFooter;
		strFooter.Format(_TEXT("��γ���������������<a href=\"%s\">����</a>���Է�����Ϣ��"),MY_EXT_ERR_URL);
		taskDlg.SetFooter(strFooter);
		taskDlg.SetFooterIcon(TD_QUESTION_ICON);
#endif	

	taskDlg.SetWindowTitle(szCaption);
	taskDlg.SetMainIcon((TCHAR*)pszResource);
	taskDlg.SetMainInstruction(szCaption);
	taskDlg.SetContent(szErr);
	if (nErr != -1 || lpszFooter != L"") 
	{
		strErr.Format(_TEXT("���ţ�0x%04X\n�ı���%s"),nErr,GetErrorMsg(nErr)),SetLastError(nErr);
		if (lpszFooter != L"") strErr = lpszFooter;
		taskDlg.SetExpandedInformation(strErr);
		taskDlg.SetExpandedControlText(_TEXT("�ر���ϸ��Ϣ"));
		taskDlg.SetCollapsedControlText(_TEXT("����ϸ��Ϣ"));
	}

	return taskDlg.DoModal();
}
DWORD CMyExt::LoadLnkFile( __in CString strLnkPath,__out CString& strFilePath )
{
	HRESULT hres;
	IShellLink* psl;
	WIN32_FIND_DATA wfd;
	WCHAR szGotPath[MAX_PATH];
	hres = CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(VOID**)&psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;
		hres = psl->QueryInterface(IID_IPersistFile,(VOID**)&ppf);
		if (SUCCEEDED(hres))
		{
			hres = ppf->Load(strLnkPath.GetString(),STGM_READ);
			if (SUCCEEDED(hres))
			{
				int nIconIndex = 0 ;
				hres = psl->GetPath(szGotPath,MAX_PATH,(WIN32_FIND_DATA*)&wfd,SLGP_RAWPATH);

				// ��ȡ��ݷ�ʽ�Ĵ򿪷�ʽ
				if (SUCCEEDED(hres))
				{
					strFilePath.Format(_TEXT("%s"),szGotPath);
				}
			}
		}
		ppf->Release();
	}
	return hres;
}

DWORD CMyExt::WirteRegString( __in HKEY hKey,__in LPCTSTR szPath,__in LPCTSTR szKeyName,__in LPCTSTR szData )
{
	HKEY hkeyHander = NULL;
	DWORD dwErrCode = NULL ;
	__try
	{
		dwErrCode = RegCreateKeyEx(hKey,szPath,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hkeyHander,NULL);
		if ( dwErrCode != ERROR_SUCCESS ) __leave ;
		dwErrCode = RegSetValueEx(hkeyHander,szKeyName,NULL,REG_SZ,(const BYTE*)szData,MAX_PATH);
		if ( dwErrCode != ERROR_SUCCESS ) __leave ;
	}
	__finally
	{
		if (hkeyHander != NULL) RegCloseKey(hkeyHander);
		hkeyHander = NULL;
	}
	if (dwErrCode != ERROR_SUCCESS && dwErrCode >= 0x300) dwErrCode = ERROR_SUCCESS ;

	return dwErrCode ;
}

DWORD CMyExt::ReadRegString( __out LPWSTR szData,__in HKEY hKey,__in LPCTSTR szSubKey,__in LPCTSTR szKeyName /*= L""*/,__in DWORD dwDataLen /*= MAX_PATH*/ )
{
	HKEY hKeyHander = NULL ; 
	int  nRetCode   = NULL ;

	__try
	{
		nRetCode =  RegOpenKeyEx(hKey,szSubKey,NULL,KEY_WOW64_32KEY|KEY_QUERY_VALUE,&hKeyHander);
		if  ( ERROR_SUCCESS != nRetCode )
		{
			// LPCTSTR str = L"";
			// wprintf_s(str,L"��ע��� %s ��ֵ����",szSubKey);
			// ShowError(str,nRetCode);	
			__leave;
		}

		nRetCode = RegQueryValueEx(hKeyHander,szKeyName,NULL,NULL,(LPBYTE)szData,&dwDataLen);
		if  ( ERROR_SUCCESS != nRetCode )
		{
			// LPCTSTR str = L"" ;
			// wprintf_s(str,L"��ȡע��� %s ��ֵ����",szKeyName);
			// ShowError(str,nRetCode);	
			__leave;
		}
	}
	__finally
	{
		if (hKeyHander != NULL)
		{
			RegCloseKey(hKeyHander);
			hKeyHander = NULL;
		}
	}
	return nRetCode;
}

CString CMyExt::GetTempFilePath( __in const LPCTSTR lpszPrefixString )
{
	CString szTempPath,szTempfile; 
	GetTempPath(_MAX_PATH, szTempPath.GetBufferSetLength(_MAX_PATH)); 
	szTempPath.ReleaseBuffer();
	GetTempFileName(szTempPath.GetString(),lpszPrefixString,0,szTempfile.GetBufferSetLength(_MAX_PATH)); 
	szTempfile.ReleaseBuffer();
	return CString(szTempfile);
}

CString CMyExt::GetWebSrcCode(const LPCTSTR lpczUrl,const LPCTSTR lpszSave)
{

	CString strWebCode = _TEXT("");

	HINTERNET hSession = InternetOpen(_TEXT("zwt"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET hURL = InternetOpenUrl(hSession, lpczUrl, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (hURL != NULL)
		{
			char Temp[1024] = {0};
			ULONG Number = 1;

			if (lpszSave != NULL)
			{
				FILE *stream;
				fopen_s(&stream, CStringA(lpszSave), "wb" );

				if( stream != NULL )
				{
					while (Number > 0)
					{
						InternetReadFile(hURL, Temp, 1024 - 1, &Number);
						strWebCode = strWebCode +  CString(Temp);
						fwrite(Temp, sizeof (char), Number , stream);
					}
					fclose( stream );
				}
			} 
			else
			{			
				while (Number > 0)
				{
					InternetReadFile(hURL, Temp, 1024 - 1, &Number);
					strWebCode = strWebCode +  CString(Temp);
				}
			}
			InternetCloseHandle(hURL);
			hURL = NULL;
		}

		InternetCloseHandle(hSession);
		hSession = NULL;
	}
	return strWebCode;
}

BOOL CMyExt::isAdministrator()
{
	BOOL bIsElevated = FALSE;
	HANDLE hToken    = NULL;
	UINT16 uWinVer   = LOWORD(GetVersion());
	uWinVer = MAKEWORD(HIBYTE(uWinVer),LOBYTE(uWinVer));
	if (uWinVer < 0x0600)
		return TRUE;
	if (OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken))
	{
		struct{
			DWORD TokenIsElevated;
		}te;
		DWORD dwReturnLength = 0;
		if (GetTokenInformation(hToken,(_TOKEN_INFORMATION_CLASS)20,&te,sizeof(te),&dwReturnLength))
		{
			if (dwReturnLength == sizeof(te))
				bIsElevated = te.TokenIsElevated;
		}
		CloseHandle(hToken);
	}
	return bIsElevated;
}

HICON CMyExt::LoadIcon( __in LPCTSTR szDllPath , __in UINT nIndex )
{
	HICON hIcon = NULL ;
	ExtractIconEx(szDllPath,nIndex,&hIcon,NULL,1);
	return hIcon;
}

CString CMyExt::GetRunPath()
{
	CString   sAppPath; 
	::GetModuleFileName(NULL,   sAppPath.GetBuffer(MAX_PATH),   MAX_PATH); 
	sAppPath.ReleaseBuffer(); 

	int   nLen     =   sAppPath.ReverseFind( '\\'); 
	sAppPath   =   sAppPath.Left(nLen) + _TEXT("\\"); 

	return   sAppPath;
}

void CMyExt::SetClipboardData(__in const CString strText )
{
	// �򿪼�����   
	if (::OpenClipboard(NULL) == FALSE || ::EmptyClipboard() == FALSE)    
	{   
		CMyExt::ShowError(L"�򿪻���ռ��а����",GetLastError());   
		return;   
	} 
	HGLOBAL hMen;   

	// ����ȫ���ڴ�    
	hMen = GlobalAlloc(GMEM_MOVEABLE, ((strText.GetLength())+1)*sizeof(CString));    
	if (hMen == NULL)   
	{   
		CMyExt::ShowError(L"����ȫ���ڴ����",GetLastError());     
		// �رռ��а�    
		CloseClipboard();   
		return;         
	}  

	// �����ݿ�����ȫ���ڴ���   
	// ��ס�ڴ���    
	LPTSTR lpStr = (LPTSTR)GlobalLock(hMen);    

	// �ڴ渴��   
	memcpy(lpStr, strText.GetString(), ((strText.GetLength())+1)*sizeof(CString));    
	// �ͷ���    
	GlobalUnlock(hMen);   

	// ���ڴ��е����ݷŵ����а���   
	::SetClipboardData(CF_UNICODETEXT, hMen);   
	CloseClipboard();
}


BOOL CMyExt::Is64Bit_OS()
{
	BOOL bRetVal = FALSE;
	SYSTEM_INFO si = { 0 };
	LPFN_PGNSI pGNSI = (LPFN_PGNSI) GetProcAddress(GetModuleHandle(_TEXT("kernel32.dll")), "GetNativeSystemInfo");
	
	if (pGNSI == NULL)
		return FALSE;

	pGNSI(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || 
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
	{
		bRetVal = TRUE;
	}
	else
	{
		// 32 λ����ϵͳ
		//_tprintf(_T("is 32 bit OS\r\n"));
	}
	return bRetVal;
}