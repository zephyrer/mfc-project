
// HidePackFile.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "HidePackFile.h"
#include "HidePackFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHidePackFileApp

BEGIN_MESSAGE_MAP(CHidePackFileApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHidePackFileApp ����

CHidePackFileApp::CHidePackFileApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHidePackFileApp ����

CHidePackFileApp theApp;


// CHidePackFileApp ��ʼ��

// UNDONE: ���������ȡ��
// void Thread_CheckUpdate(void)
// {
// 	CUpdate up(MY_SOFT_CHECK_UPDATA);
// 	up.CheckUpdate(MySoftVersoin);
// }

BOOL CHidePackFileApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

#ifndef _DEBUG
	//------------------------------------------------------------------------------
	// �����������߳�
	//------------------------------------------------------------------------------
// 	::CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)Thread_CheckUpdate,NULL,NULL,NULL);
#endif
	AfxOleInit();
	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("King.Sollyu"));

	CHidePackFileDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

