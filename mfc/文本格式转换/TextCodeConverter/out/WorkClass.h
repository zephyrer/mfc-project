#pragma once

class CWorkClass
{
public:
	enum {FILE_ANSI=WM_APP+0x0001,FILE_UNICODE,FILE_UNICODE_BIG,FILE_UTF_8,FILE_UNKNOW};
	enum ERR_TYPE{
		ERR_NONE=WM_APP+0x0100,		
		ERR_GENERICEXCEPTION,
		ERR_FILENOTFOUND,
		ERR_BADPATH,
		ERR_TOOMANYOPENFILES,
		ERR_ACCESSDENIED,
		ERR_INVALIDFILE,
		ERR_REMOVECURRENTDIR,
		ERR_DIRECTORYFULL,
		ERR_BADSEEK,
		ERR_HARDIO,
		ERR_SHARINGVIOLATION,
		ERR_LOCKVIOLATION,
		ERR_DISKFULL,
		ERR_ENDOFFILE,
		ERR_READ
};


public:
	CWorkClass(void);
	~CWorkClass(void);

private:
	UINT m_nFileCodeType;

public:
	UINT ReadFile_(LPCTSTR lpszFilePath,CString& strText);
	BOOL SaveFile_(LPCTSTR lpszFilePath,CString& strText,UINT nCode=FILE_ANSI);

	UINT ReadFile_Ansi(LPCTSTR lpszFilePath,CString& strText);

};
