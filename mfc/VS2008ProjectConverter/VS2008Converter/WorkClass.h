#pragma once
#include <regex>

class WorkClass
{
public:
	enum ERR_TYPE{ERR_NONE=0x0F00000,ERR_INVALIDFORMAT,ERR_UNKNOW,
					ERR_FILENOTFOUND,ERR_ACCESSDENIED,ERR_SHARINGVIOLATION,
					ERR_READFILE};
private :
	UINT m_nLastError;
	void SetLastError(UINT val) { m_nLastError = val; }

public:
	WorkClass(void);
	virtual ~WorkClass(void);


public:
	UINT GetLastError() const { return m_nLastError; }
	BOOL ProcessSlnFile(CString& strFileContent);
	BOOL ProcessVcprojFile(CString& strFileContent);
	CString ReadFile2Txt(const CString strFilePath);
	BOOL WriteString2File(const CString strText,const CString strSavePath,BOOL bUtf_8);
	BOOL RenameFile(const CString strOldFileName,const CString strNewName);
	
};
