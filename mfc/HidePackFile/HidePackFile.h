
// HidePackFile.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHidePackFileApp:
// �йش����ʵ�֣������ HidePackFile.cpp
//

class CHidePackFileApp : public CWinApp
{
public:
	CHidePackFileApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHidePackFileApp theApp;