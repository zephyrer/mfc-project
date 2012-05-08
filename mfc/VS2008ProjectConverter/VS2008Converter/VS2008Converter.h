
// VS2008Converter.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CVS2008ConverterApp:
// 有关此类的实现，请参阅 VS2008Converter.cpp
//

class CVS2008ConverterApp : public CWinApp
{
public:
	CVS2008ConverterApp();

// 重写
	public:
	virtual BOOL InitInstance();

	void CommandLineProc( CString str_com );
// 实现

	DECLARE_MESSAGE_MAP()
};

extern CVS2008ConverterApp theApp;