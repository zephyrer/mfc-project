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

		fileTxt.Close();

	}else if (n2Temp == 0xBBEF)
	{
		unsigned char n1Temp;

		if (fileTxt.Read(&n1Temp,1) != 1)
			return ERR_READ;
		if (n1Temp != 0xBF)
			return ERR_INVALIDFILE;

		wchar_t* szText = new wchar_t[(UINT)lFileSize/2];
		memset(szText,'\0', sizeof(szText));

		fileTxt.Close();


		FILE * f1;
		_wfopen_s(&f1,lpszFilePath,L"rt+,ccs=UTF-8");  //or UTF-8

// 		locale loc("");
// 		wcout.imbue(loc);
		while (!feof(f1))
		{
			fgetws(szText,100,f1);
		}
		fclose(f1);

// 		int nReadSize = fileTxt.Read(szText,(UINT)lFileSize-2);
// 		szText[nReadSize/2]='\0';

		strText.Format(TEXT("%s"),szText);

		if (AfxMessageBox(strText,MB_YESNO) != IDYES)
			return ERR_INVALIDFILE;
	}
	else
	{
		char* szText = new char[(UINT)lFileSize+1];
		memset(szText,'\0', sizeof(szText));

		fileTxt.SeekToBegin();
		int nReadSize = fileTxt.Read(szText,(UINT)lFileSize);
		szText[nReadSize]='\0';

		strText = szText;
//		free(szText);
		fileTxt.Close();
	}


	return ERR_NONE;
}



/*******************************************************************************
函数名称:　　　　　　　　 ReadFile_　使用file函数，手动指定编码格式
================================================================================
参数说明:　　　　　　　　 LPCTSTR lpszFilePath
参数说明:　　　　　　　　 CString & strText
参数说明:　　　　　　　　 UINT nCode
--------------------------------------------------------------------------------
返回值:　　　　　　　　 　 UINT
--------------------------------------------------------------------------------
文件作者:				King.Sollyu					QQ:191067617
*******************************************************************************/
UINT CWorkClass::ReadFile_( LPCTSTR lpszFilePath,CString& strText,UINT nCode )
{
	errno_t err;
	if (nCode == FILE_UNICODE || nCode == FILE_UTF_8)
	{
		FILE * f1;
		if (nCode == FILE_UNICODE)
			err=_wfopen_s(&f1, lpszFilePath,TEXT("r, ccs=UNICODE"));
		else
			err=_wfopen_s(&f1, lpszFilePath,TEXT("r, ccs=UTF-8"));
		ASSERT(f1);
	
		CStdioFile file (f1);

		unsigned int nFileSize = (int)file.GetLength();

		wchar_t* szChar = new wchar_t[nFileSize / sizeof(wchar_t)];
		memset(szChar,'\0',sizeof(szChar));
	
		int nReadSize = file.Read(szChar,nFileSize);
	
		szChar[nReadSize / sizeof(wchar_t)] = '\0';

		strText = szChar;
		
	}else if(nCode == FILE_ANSI)
	{
		FILE * f1;
		if ((err=_wfopen_s(&f1,lpszFilePath,L"r")) != 0)
			return ERR_INVALIDFILE;

		ASSERT(f1);

		CStdioFile file (f1);
		unsigned int nFileSize = (int)file.GetLength();

		char* linex = new char[nFileSize];

		int nReadSize = file.Read(linex,nFileSize);
		linex[nReadSize / sizeof(char)] = '\0';

		CString strW (linex);
		strW.Replace(_T("\n"),_T("\r\n"));
		strText = strW;

	}

	

	_fcloseall( );

	return AfxMessageBox(strText,MB_YESNO) == IDYES?ERR_NONE:ERR_INVALIDFILE;

}
BOOL CWorkClass::SaveFile_( LPCTSTR lpszFilePath,CString strText,UINT nCode/*=FILE_ANSI*/ )
{

	errno_t err;
	if (nCode == FILE_UNICODE)
	{
		FILE * f1;
		err=_wfopen_s(&f1,lpszFilePath,L"wb+,ccs=UNICODE");  //or UTF-8
		unsigned char data[2] = {0xFF, 0xFE};
		fwrite(data,1,2,f1);
	
		strText.Replace(TEXT("\n"),TEXT("\r\n"));
		size_t strSize = strText.GetLength();
		if ((fwrite(strText, sizeof(wchar_t), strSize, f1) != strSize) && fclose(f1))
			return FALSE;
	}else if (nCode == FILE_UTF_8)
	{
		FILE * f1;
		err=_wfopen_s(&f1,lpszFilePath,L"wb+,ccs=UTF-8");  //or UTF-8
		unsigned char data[3] = {0xEF, 0xBB, 0xBF};
		fwrite(data,1,3,f1);

		strText.Replace(TEXT("\n"),TEXT("\r\n"));
		size_t strSize = strText.GetLength();
		if ((fwrite(strText, sizeof(wchar_t), strSize, f1) != strSize) && fclose(f1))
			return FALSE;
	}
	


//////////////////////////////////////////////////////////////////////////
//  老方法
//
// 	CFile fileWirte(lpszFilePath,CFile::modeCreate|CFile::modeReadWrite);
// 
// 	if (FILE_UNICODE == nCode)
// 	{
// 		WCHAR* sWchar=new WCHAR[strText.GetLength()];
// 		wcscpy(sWchar,(LPCWSTR)(LPCTSTR)strText);
// 		fileWirte.Write("\xff\xfe",2);
// 		fileWirte.Write(sWchar,wcslen(sWchar)*sizeof(WCHAR));
// 	}else if (FILE_ANSI == nCode)
// 	{
// 
// 		wchar_t* wszString =strText.GetBuffer(strText.GetLength());
// 		//预转换，得到所需空间的大小，这次用的函数和上面名字相反
// 		int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
// 		//同上，分配空间要给'\0'留个空间
// 		char* szAnsi = new char[ansiLen + 1];
// 		//转换
// 		//unicode版对应的strlen是wcslen
// 		::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), szAnsi, ansiLen, NULL, NULL);
// 		//最后加上'\0'
// 		szAnsi[ansiLen] = '\0';
// 
// 		fileWirte.Write(szAnsi,ansiLen * sizeof(char));
// 	}
// 
// 	fileWirte.Flush();
// 	fileWirte.Close();

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


// 	char* szAnsi = new char[fileFile.GetLength()];
// 
// 	fileFile.Read(szAnsi,fileFile.GetLength());
// 	fileFile.Close();


	// ansi to unicode
	// 	char* szAnsi = "abcd1234你我他";
	//预转换，得到所需空间的大小
// 	int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);
// 	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
// 	wchar_t* wszString = new wchar_t[wcsLen + 1];
// 	//转换
// 	::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), wszString, wcsLen);
// 	//最后加上'\0'
// 	wszString[wcsLen] = '\0';
// 	//unicode版的MessageBox API
// 	::MessageBoxW(NULL, wszString, wszString, MB_OK);
// 
	return NULL;

}