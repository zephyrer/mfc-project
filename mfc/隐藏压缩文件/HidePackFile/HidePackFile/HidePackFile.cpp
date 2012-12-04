
// HidePackFile.cpp : 定义应用程序的类行为。
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


// CHidePackFileApp 构造

CHidePackFileApp::CHidePackFileApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CHidePackFileApp 对象

CHidePackFileApp theApp;


// CHidePackFileApp 初始化

// UNDONE: 检测升级被取消
// void Thread_CheckUpdate(void)
// {
// 	CUpdate up(MY_SOFT_CHECK_UPDATA);
// 	up.CheckUpdate(MySoftVersoin);
// }

BOOL CHidePackFileApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

#ifndef _DEBUG
	//------------------------------------------------------------------------------
	// 创建检查更新线程
	//------------------------------------------------------------------------------
// 	::CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)Thread_CheckUpdate,NULL,NULL,NULL);
#endif
	AfxOleInit();
	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// 例如修改为公司或组织名
	SetRegistryKey(_T("King.Sollyu"));

	CHidePackFileDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

