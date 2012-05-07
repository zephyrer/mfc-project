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
��������:				WorkClass::ProcessSlnFile	����sln�ļ�
================================================================================
����˵��:				CString & strFileContent	sln�ļ�����
--------------------------------------------------------------------------------
����ֵ:					BOOL
--------------------------------------------------------------------------------
�ļ�����:				King.Sollyu					QQ:191067617
*******************************************************************************/
BOOL WorkClass::ProcessSlnFile( CString& strFileContent )
{
	std::wstring search_string(strFileContent);
	std::tr1::wregex reg(TEXT("Version \\d+\\.\\d{2}"));
	std::tr1::wsmatch results;

	try
	{
		// Ѱ�ҵ�ǰ�汾��Ϣ һ��Ϊ��Version X.00
		// û���ҵ���˵����ʽ����
		if (std::tr1::regex_search(search_string,results,reg))
		{
			// ���ҵ��ĸ�ʽ�滻Ϊ 10.00
			std::wstring replace = TEXT("Version 10.00");
			search_string = std::tr1::regex_replace(search_string,reg,replace); 

			// ���Ұ汾��ʽ���Ƿ���� # Visual Studio 2008 
			// ����������
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
		// ���ļ�����ʶ��ʲô����
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
	
	// ���屣���ȡ���ݵ����飬��СΪ�ı��ļ�����
	char* pContent=(char*)calloc(nFileSize+1,sizeof(char));

	// UTF-8�ļ����ļ���ǰ�����ֽ���:EFBBBF��
	unsigned long head(3);
	if (fileText.Read(&head,3) != 3)
	{
		fileText.Close();
		return SetLastError(ERR_READFILE),NULL;
	}
	
	// ����ļ���ǰ�����ֽڲ���0xEFBBBF����˵��Ϊ�����ı��ĵ�
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
		// ��UTF-8ת��ΪANSI����
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
��������:				WorkClass::WriteString2File	д�ַ������ݵ��ļ�
================================================================================
����˵��:				const CString strText �ļ�����
����˵��:				const CString strSavePath �ļ�·��
����˵��:				BOOL bUtf_8 �Ƿ�ΪUTF-8��ʽ
--------------------------------------------------------------------------------
����ֵ:					BOOL
--------------------------------------------------------------------------------
�ļ�����:				King.Sollyu					QQ:191067617
*******************************************************************************/
BOOL WorkClass::WriteString2File( const CString strText,const CString strSavePath,BOOL bUtf_8 )
{
	CFile fileText; CFileException fileException;
	if (fileText.Open(strSavePath,CFile::modeReadWrite|CFile::modeCreate,&fileException) == FALSE)
	{
		ASSERT (FALSE);
		// ���ļ�����ʶ��ʲô����
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
		// ��UNICODE ת����UTF8
		// ��Ҫ����AtlUnicodeToUTF8 ͷ�ļ�: <atlenc.h>
		int nLen = AtlUnicodeToUTF8(strText, nSrcLen, buffer, nBufLen);   
		buffer[nLen] = 0;
		utf8String.ReleaseBuffer();
		//д�ļ�
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

