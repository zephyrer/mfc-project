
// VS2008Converter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVS2008ConverterApp:
// �йش����ʵ�֣������ VS2008Converter.cpp
//

class CVS2008ConverterApp : public CWinAppEx
{
public:
	CVS2008ConverterApp();

// ��д
	public:
	virtual BOOL InitInstance();

	void CommandLineProc( CString str_com );
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVS2008ConverterApp theApp;