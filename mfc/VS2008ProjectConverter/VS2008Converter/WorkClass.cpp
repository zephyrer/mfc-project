#include "StdAfx.h"
#include "WorkClass.h"
#include <atlenc.h>

WorkClass::WorkClass(void)
{
}

WorkClass::~WorkClass(void)
{
}

/*******************************************************************************
函数名称:				WorkClass::ProcessSlnFile	处理sln文件
================================================================================
参数说明:				CString & strFileContent	sln文件内容
--------------------------------------------------------------------------------
返回值:					BOOL
--------------------------------------------------------------------------------
文件作者:				King.Sollyu					QQ:191067617
*******************************************************************************/
BOOL WorkClass::ProcessSlnFile( CString& strFileContent )
{
	std::wstring search_string(strFileContent);
	std::tr1::wregex reg(TEXT("Version \\d+\\.\\d{2}"));
	std::tr1::wsmatch results;

	try
	{
		// 寻找当前版本信息 一般为：Version X.00
		// 没有找到则说明格式错误
		if (std::tr1::regex_search(search_string,results,reg))
		{
			// 将找到的格式替换为 10.00
			std::wstring replace = TEXT("Version 10.00");
			search_string = std::tr1::regex_replace(search_string,reg,replace); 

			// 查找版本格式后是否跟随 # Visual Studio 2008 
			// 不跟随就添加
			std::tr1::wregex reg2 (TEXT("Version 10.00\r\n# Visual Studio \\d{4}"));
			if (!std::tr1::regex_search(search_string,results,reg2))
			{
				replace = TEXT("Version 10.00\r\n# Visual Studio 2008");
				search_string = std::tr1::regex_replace(search_string,reg,replace);
			}else
			{
				std::tr1::wregex reg3 (TEXT("# Visual Studio \\d{4}"));
				if (std::tr1::regex_search(search_string,results,reg3) != NULL)
				{
					replace = TEXT("# Visual Studio 2008");
					search_string = std::tr1::regex_replace(search_string,reg3,replace);
				}
			}

		}else
		{	
			return SetLastError(ERR_INVALIDFORMAT),FALSE;
		}
	}
	catch (...){ return SetLastError(ERR_UNKNOW),FALSE; }

	strFileContent.Format(TEXT("%s"),search_string.c_str());
	return  SetLastError(ERR_NONE),TRUE;

}


BOOL WorkClass::ProcessVcprojFile( CString& strFileContent )
{
	std::wstring search_string(strFileContent);
	std::tr1::wregex reg(TEXT("Version=(\"\\d\\.\\d{2}\")"));
	std::tr1::wsmatch results;

	if (std::tr1::regex_search(search_string,results,reg))
	{
		std::wstring replace = TEXT("Version=\"9.00\"");
		search_string = std::tr1::regex_replace(search_string,reg,replace);

	}else
	{	
		return SetLastError(ERR_INVALIDFORMAT),FALSE;
	}
	strFileContent.Format(TEXT("%s"),search_string.c_str());
	return  SetLastError(ERR_NONE),TRUE;
}

CString WorkClass::ReadFile2Txt(const CString strFilePath)
{
	CFile fileText; CFileException fileException;
	if (fileText.Open(strFilePath,CFile::modeRead,&fileException) == FALSE)
	{
		ASSERT (FALSE);
		// 打开文件错误，识别什么错误
		switch (fileException.m_cause)
		{
		case CFileException::fileNotFound: return SetLastError(ERR_FILENOTFOUND),NULL;break;
		case CFileException::accessDenied: return SetLastError(ERR_ACCESSDENIED),NULL;break;
		case CFileException::sharingViolation: return SetLastError(ERR_SHARINGVIOLATION),NULL;break;
		default:return SetLastError(ERR_UNKNOW),NULL;break;
		}
	}

	CString strFile = TEXT("");
	ULONG nFileSize = (ULONG)fileText.GetLength();
	
	// 定义保存读取内容的数组，大小为文本文件长度
	char* pContent=(char*)calloc(nFileSize+1,sizeof(char));

	// UTF-8文件，文件的前三个字节是:EFBBBF。
	unsigned long head(3);
	if (fileText.Read(&head,3) != 3)
	{
		fileText.Close();
		return SetLastError(ERR_READFILE),NULL;
	}
	
	// 如果文件的前三个字节不是0xEFBBBF，则说明为正常文本文档
	fileText.Seek(head == 0xEFBBBF ? 3:0,CFile::current);

	ULONG nReadSize = nFileSize - (head == 0xEFBBBF ? 0:3);
	if (fileText.Read(pContent,nReadSize) != nReadSize)
	{
		ASSERT(FALSE),fileText.Close();
		SetLastError(ERR_READFILE);
		return strFile;
	}
	fileText.Close();

	if (head == 0xEFBBBF)
	{
		// 将UTF-8转化为ANSI编码
		int n=MultiByteToWideChar(CP_UTF8,0,pContent,nFileSize+1,NULL,0);
		wchar_t* pWideChar=(wchar_t*)calloc(n+1,sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8,0,pContent,nFileSize+1,pWideChar,n);
		strFile=CString(pWideChar);
		free(pWideChar);
		
	}else
	{
		strFile = pContent;
	}
	free(pContent);

	SetLastError(ERR_NONE);
	return strFile;

}


/*******************************************************************************
函数名称:				WorkClass::WriteString2File	写字符串内容到文件
================================================================================
参数说明:				const CString strText 文件内容
参数说明:				const CString strSavePath 文件路径
参数说明:				BOOL bUtf_8 是否为UTF-8格式
--------------------------------------------------------------------------------
返回值:					BOOL
--------------------------------------------------------------------------------
文件作者:				King.Sollyu					QQ:191067617
*******************************************************************************/
BOOL WorkClass::WriteString2File( const CString strText,const CString strSavePath,BOOL bUtf_8 )
{
	CFile fileText; CFileException fileException;
	if (fileText.Open(strSavePath,CFile::modeReadWrite|CFile::modeCreate,&fileException) == FALSE)
	{
		ASSERT (FALSE);
		// 打开文件错误，识别什么错误
		switch (fileException.m_cause)
		{
		case CFileException::fileNotFound:	return SetLastError(ERR_FILENOTFOUND),FALSE;break;
		case CFileException::accessDenied:	return SetLastError(ERR_ACCESSDENIED),FALSE;break;
		case CFileException::sharingViolation: return SetLastError(ERR_SHARINGVIOLATION),FALSE;break;
		default:return SetLastError(ERR_UNKNOW),FALSE;break;
		}
	}

	if (bUtf_8 == TRUE)
	{
		unsigned char data[3] = {0xEF, 0xBB, 0xBF};
		fileText.Write(data,3);
		int nSrcLen = (int)wcslen(strText);
		CStringA utf8String(strText);
		int nBufLen = (nSrcLen+1) * 6;
		LPSTR buffer = utf8String.GetBufferSetLength(nBufLen);
		// 将UNICODE 转换成UTF8
		// 需要函数AtlUnicodeToUTF8 头文件: <atlenc.h>
		int nLen = AtlUnicodeToUTF8(strText, nSrcLen, buffer, nBufLen);   
		buffer[nLen] = 0;
		utf8String.ReleaseBuffer();
		//写文件
		fileText.SeekToEnd();
		fileText.Write((LPCSTR)utf8String, nLen);
	}else
	{
		CStringA strANSI(strText);
		fileText.Write((LPCSTR)strANSI,strANSI.GetLength());
	}

	fileText.Close();
	return TRUE;

}

BOOL WorkClass::RenameFile( const CString strOldFileName,const CString strNewName )
{
	CFile::Rename(strOldFileName,strNewName);
	return TRUE;
}

