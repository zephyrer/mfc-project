#include "StdAfx.h"
#include "WorkClass.h"

CWorkClass::CWorkClass(void)
{
}

CWorkClass::~CWorkClass(void)
{
}

UINT CWorkClass::ReadFile_( LPCTSTR lpszFilePath,CString& strText )
{
	CFile fileTxt(lpszFilePath,CFile::modeRead);
	unsigned short n2Temp(2);
	fileTxt.Read(&n2Temp,2);

	LONGLONG lFileSize = fileTxt.GetLength();

	if (n2Temp == 0xFEFF)
	{
		WCHAR* szText = new WCHAR[(UINT)lFileSize/2];
		memset(szText,'\0', sizeof(szText));

		int nReadSize = fileTxt.Read(szText,(UINT)lFileSize-2);
		szText[nReadSize/2]='\0';

		strText.Format(TEXT("%s"),szText);
	}else
	{
		char* szText = new char[(UINT)lFileSize+1];
		memset(szText,'\0', sizeof(szText));

		fileTxt.SeekToBegin();
		int nReadSize = fileTxt.Read(szText,(UINT)lFileSize);
		szText[nReadSize]='\0';

		strText = szText;
//		free(szText);
	}

	fileTxt.Close();

	return ERR_NONE;
}

BOOL CWorkClass::SaveFile_( LPCTSTR lpszFilePath,CString& strText,UINT nCode/*=FILE_ANSI*/ )
{
	CFile fileWirte(lpszFilePath,CFile::modeCreate|CFile::modeReadWrite);

	if (FILE_UNICODE == nCode)
	{
		WCHAR* sWchar=new WCHAR[strText.GetLength()];
		wcscpy(sWchar,(LPCWSTR)(LPCTSTR)strText);
		fileWirte.Write("\xff\xfe",2);
		fileWirte.Write(sWchar,wcslen(sWchar)*sizeof(WCHAR));
	}else if (FILE_ANSI == nCode)
	{

		wchar_t* wszString =strText.GetBuffer(strText.GetLength());
		//预转换，得到所需空间的大小，这次用的函数和上面名字相反
		int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
		//同上，分配空间要给'\0'留个空间
		char* szAnsi = new char[ansiLen + 1];
		//转换
		//unicode版对应的strlen是wcslen
		::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), szAnsi, ansiLen, NULL, NULL);
		//最后加上'\0'
		szAnsi[ansiLen] = '\0';

		fileWirte.Write(szAnsi,ansiLen * sizeof(char));
	}

	fileWirte.Flush();
	fileWirte.Close();

	return TRUE;
}

UINT CWorkClass::ReadFile_Ansi( LPCTSTR lpszFilePath,CString& strText )
{
	CFile fileFile(lpszFilePath,CFile::modeRead);
// 	unsigned short n2Temp;
// 	
// 	fileFile.Read(&n2Temp,2);
// 
// 	if (n2Temp != 0xFEFF ) return FALSE;


	char* szAnsi = new char[fileFile.GetLength()];

	fileFile.Read(szAnsi,fileFile.GetLength());
	fileFile.Close();


	// ansi to unicode
	// 	char* szAnsi = "abcd1234你我他";
	//预转换，得到所需空间的大小
	int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);
	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	//转换
	::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), wszString, wcsLen);
	//最后加上'\0'
	wszString[wcsLen] = '\0';
	//unicode版的MessageBox API
	::MessageBoxW(NULL, wszString, wszString, MB_OK);

	return NULL;

}