
// TextCodeConverter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTextCodeConverterApp:
// �йش����ʵ�֣������ TextCodeConverter.cpp
//

class CTextCodeConverterApp : public CWinAppEx
{
public:
	CTextCodeConverterApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	CString LoadStringFormId( UINT nID );
};

extern CTextCodeConverterApp theApp;