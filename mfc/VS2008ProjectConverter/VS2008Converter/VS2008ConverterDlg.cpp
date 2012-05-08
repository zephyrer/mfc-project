/ VS2008ConverterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VS2008Converter.h"
#include "VS2008ConverterDlg.h"


#define UNM_HYPERLINK_CLICKED	(WM_USER + 0x100)


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CVS2008ConverterDlg 对话框




CVS2008ConverterDlg::CVS2008ConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogSampleDlgBase(CVS2008ConverterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);//这句必须有，
	XTPSkinManager()->LoadSkin(_T("Metro.cjstyles"));//这句是设置一种风格，Office2007.cjstyles是风格文件名，要拷贝过来。

}

void CVS2008ConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVS2008ConverterDlg, CDialog)
	//{{AFX_MSG_MAP(CVS2008ConverterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CVS2008ConverterDlg::OnBnClickedButton1)

	ON_MESSAGE(UNM_HYPERLINK_CLICKED, OnHyperlinkClicked)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

// 自定义结构体，用来处理透明
typedef struct tag_Transpren{
	int nStart;
	int nEnd;
	int nStep;
}TRANSPARENT_TAG,*LPTRANSPARENT_TAG;

int		nNowTran_g = 255;	//现在的透明值
HANDLE  hThread	   = NULL;	//线程句柄

/*******************************************************************************
函数名称:				Transparen	设置窗体透明主函数
================================================================================
参数说明:				int nStart	开始透明度位置
参数说明:				int nEnd	结束透明度	
参数说明:				int nStep	步长	
--------------------------------------------------------------------------------
返回值:					void
--------------------------------------------------------------------------------
文件作者:				King.Sollyu					QQ:191067617
*******************************************************************************/

void Transparen(int nStart,int nEnd,int nStep)
{
	// TRACE("Thread is start DownTransparen(%d,%d,%d)",nStart,nEnd,nStep);

	#ifdef _DEBUG
	return ;
	#endif

	// 判断输入参数是否合理
	if (nStart>255 || nStart<0 || nEnd>255 || nEnd<0)	return;

	// 几次用来调试输出时显示换行
	int nSum = 0;

	try{
		DWORD dwExStyle=GetWindowLong(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),GWL_EXSTYLE); 
		if((dwExStyle&0x80000)!=0x80000) 
			SetWindowLong(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),GWL_EXSTYLE,dwExStyle|WS_EX_LAYERED); 

// 		AfxGetApp()->GetMainWnd()->ModifyStyle(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),0, WS_EX_LAYERED,NULL); //修改窗口属性为WS_EX_LAYERED

		while (nStart>nEnd ? (nNowTran_g>=nEnd):(nNowTran_g<nEnd))
		{
			SetLayeredWindowAttributes(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),0,nNowTran_g,2);
			Sleep(10);
			nNowTran_g += nStart>nEnd?(-1 * nStep):nStep;

			//if (nSum++% 15 == 0)	// TRACE(TEXT("\n"));
			// TRACE(TEXT("%d\t"),nNowTran_g);
		}
	}catch(...){
		// TRACE(TEXT("在处理透明度上出现了问题，输入参数：DownTransparen(%d,%d,%d)"),nStart,nEnd,nStep);

	}
	return;
}


/*******************************************************************************
函数名称:				threadCallTran	线程调用透明度入口
================================================================================
参数说明:				LPVOID lParam		
--------------------------------------------------------------------------------
返回值:					void
--------------------------------------------------------------------------------
文件作者:				King.Sollyu					QQ:191067617
*******************************************************************************/
void threadCallTran(LPVOID lParam)
{
	TRANSPARENT_TAG* tran = (TRANSPARENT_TAG*)lParam;
	Transparen(tran->nStart,tran->nEnd,tran->nStep);
	ExitThread(0);
}
// CVS2008ConverterDlg 消息处理程序

BOOL CVS2008ConverterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	OnInitUI();

	LPWSTR  *argv = NULL;
	int args = 0;
	argv = CommandLineToArgvW(GetCommandLine(),&args);
	if (args != 1)
	{
		CFile fileText; CFileException fileException;
		if (fileText.Open(argv[1],CFile::modeRead,&fileException) == FALSE)
		{
			ASSERT (FALSE);
			// 打开文件错误，识别什么错误
			switch (fileException.m_cause)
			{
			case CFileException::fileNotFound: AfxMessageBox(loadStringFromID(WorkClass::ERR_FILENOTFOUND));break;
			case CFileException::accessDenied: AfxMessageBox(loadStringFromID(WorkClass::ERR_ACCESSDENIED));break;
			case CFileException::sharingViolation: AfxMessageBox(loadStringFromID(WorkClass::ERR_SHARINGVIOLATION));break;
			default:AfxMessageBox(loadStringFromID(WorkClass::ERR_UNKNOW));
			}
			EndDialog(NULL);
			return FALSE;
		}
		CString strFileExt = fileText.GetFileTitle().Right(fileText.GetFileTitle().GetLength() - fileText.GetFileTitle().ReverseFind('.'));
		if (strFileExt == TEXT(".sln"))
		{
			CString str_file_path(argv[1]);
			ProcessFunc(str_file_path);
			MessageBox(loadStringFromID(m_work.GetLastError()),loadStringFromID(ids_ui_main_dlg_title),MB_ICONINFORMATION);
		}else
		{
			MessageBox(loadStringFromID(ids_ui_msg_drop_file),loadStringFromID(ids_ui_main_dlg_title),MB_ICONINFORMATION);
		}

		EndDialog(NULL);
	}


	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVS2008ConverterDlg::OnInitUI()
{ 
	AfxGetApp()->GetMainWnd()->SetWindowText(loadStringFromID(ids_ui_main_dlg_title));
	SetDlgItemText(IDC_BUTTON1,loadStringFromID(ids_ui_dlg_btn_brow_proj));

	m_xToolTip.Create(this);
	SUPER_TOOLTIP_INFO sti;
 
	sti.bSuperTooltip     = TRUE;
	sti.nIDTool           = IDC_BUTTON1;                // can be anything you want
	sti.nSizeX            = XSUPERTOOLTIP_DEFAULT_WIDTH;
	sti.pWnd              = GetDlgItem(IDC_BUTTON1);
	sti.strHeader         = loadStringFromID(ids_ui_tip_main_help);
	sti.bLineAfterHeader  = FALSE;
	sti.strBody           = loadStringFromID(ids_ui_tip_main_button1);

 	sti.strFooter         = loadStringFromID(ids_ui_tip_main_button_click);
	sti.nBodyImage		  = IDB_BITMAP1;
	sti.nFooterImage      = IDB_BITMAP2;
	sti.bLineBeforeFooter = TRUE;
	sti.rgbBegin          = RGB(255, 255, 255);
	sti.rgbMid            = RGB(242, 242, 246);
	sti.rgbEnd            = RGB(212, 212, 224);
	sti.rgbText           = RGB(0, 0, 0);

	m_xToolTip.SetCallbackHyperlink(this->GetSafeHwnd(), UNM_HYPERLINK_CLICKED);
	m_xToolTip.EnableHyperlink(TRUE);

	CString strHtml = m_xToolTip.AddTool(&sti);

}

void CVS2008ConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
		//   手动添加WM_NCLBUTTONUP事件
		if((nID&0xFFF0)==SC_MOVE) PostMessage(WM_NCLBUTTONUP,nID,lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVS2008ConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVS2008ConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*******************************************************************************
函数名称:				CTranTestDlg::OnNcLButtonDown	按下标题栏
================================================================================
返回值:					void
--------------------------------------------------------------------------------
文件作者:				King.Sollyu					QQ:191067617
*******************************************************************************/
void CVS2008ConverterDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TRACE("------------OnNcLButtonDown------nHitTest:%d\n",nHitTest);

	#ifdef _DEBUG
		CXTPDialogBase<CXTResizeDialog>::OnNcLButtonDown(nHitTest, point);
		return;
	#endif

	if (hThread != NULL)	TerminateThread(hThread,0);

	static TRANSPARENT_TAG tran={nNowTran_g,100,7};
	hThread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)threadCallTran,&tran,NULL,NULL);

	if (nHitTest != NULL)
		CXTPDialogBase<CXTResizeDialog>::OnNcLButtonDown(nHitTest, point);
}


/*******************************************************************************
函数名称:				CTranTestDlg::OnNcLButtonUp	松开标题栏
================================================================================
返回值:					void
--------------------------------------------------------------------------------
文件作者:				King.Sollyu					QQ:191067617
*******************************************************************************/
void CVS2008ConverterDlg::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
#ifdef _DEBUG
	CXTPDialogBase<CXTResizeDialog>::OnNcLButtonUp(nHitTest, point);
	return ;
#endif

	WaitForSingleObject(hThread,INFINITE);
	// TRACE("------------OnNcLButtonUp--------nHitTest:%d\n",nHitTest);


	static TRANSPARENT_TAG tran={nNowTran_g,255,7};
	hThread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)threadCallTran,&tran,NULL,NULL);

	if (nHitTest != NULL)
		CXTPDialogBase<CXTResizeDialog>::OnNcLButtonUp(nHitTest, point);
}


/*******************************************************************************
函数名称:				CTranTestDlg::OnActivate	激活窗体事件
================================================================================
参数说明:				UINT nState		
参数说明:				CWnd * pWndOther		
参数说明:				BOOL bMinimized		
--------------------------------------------------------------------------------
返回值:					void
--------------------------------------------------------------------------------
文件作者:				King.Sollyu					QQ:191067617
*******************************************************************************/
void CVS2008ConverterDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CXTPDialogBase<CXTResizeDialog>::OnActivate(nState, pWndOther, bMinimized);
#ifdef _DEBUG
	return ;
#endif
	switch (nState)
	{
	case WA_INACTIVE : 
		// TRACE("------------WA_INACTIVE------\t0x%04x\n",nState);
		OnNcLButtonDown(NULL,NULL);

		break;
	case WA_CLICKACTIVE:// 鼠标激活方式
		// TRACE("------------WA_CLICKACTIVE------\t0x%04x\n",nState);
		if(nNowTran_g > 250) break;
		if (hThread != NULL)	TerminateThread(hThread,0);
		OnNcLButtonUp(NULL,NULL);

		break;
	case WA_ACTIVE://其他激活方式，比如 Alt+Tab
		// TRACE("------------WA_ACTIVE------\n");
		if (hThread != NULL)	TerminateThread(hThread,0);
		OnNcLButtonUp(NULL,NULL);

		break;
	}

}


CString CVS2008ConverterDlg::loadStringFromID( UINT nID )
{
	CString strTmp;
	strTmp.LoadString(nID);
	return strTmp;
}


CString CVS2008ConverterDlg::getDlgItemText_( UINT nID )
{
	CString set_tmp;
	GetDlgItemText(nID,set_tmp);
	return set_tmp;
}
void CVS2008ConverterDlg::OnBnClickedButton1()
{
	CFileDialog fDlg(TRUE,TEXT("sln"),NULL,4|2,loadStringFromID(ids_ui_dlg_main_open_filter));
	if (fDlg.DoModal() != IDOK) return;
	
	CString strFilePath = fDlg.GetPathName();
	
	ProcessFunc(strFilePath);
	MessageBox(loadStringFromID(m_work.GetLastError()),loadStringFromID(ids_ui_main_dlg_title),MB_ICONINFORMATION);



}

BOOL CVS2008ConverterDlg::ProcessFunc( CString &strFilePath )
{

	CString strFile = m_work.ReadFile2Txt(strFilePath);
	if (FALSE == m_work.ProcessSlnFile(strFile))
	{
		TRACE(TEXT("LastError = %d"),m_work.GetLastError());
		return FALSE;
	}

	CString strBackFileName = strFilePath + TEXT(".bak");
	m_work.RenameFile(strFilePath.GetBuffer(),strBackFileName);

	m_work.WriteString2File(strFile,strFilePath,TRUE);

	CString strFindFolder;
	strFindFolder = strFilePath.Left( strFilePath.ReverseFind('\\')) + TEXT("\\");
	// 	MessageBox(strFindFolder);

	CFileFind findFile;
	BOOL bFinded = findFile.FindFile(strFindFolder + TEXT("\\*"));
	while (bFinded)
	{
		bFinded = findFile.FindNextFile();
		if (!findFile.IsDots())
		{
			if (findFile.IsDirectory() == TRUE)
			{
				CFileFind findFile2;CString strFindFolder2 = findFile.GetFilePath();
				BOOL bFinded2 = findFile2.FindFile(strFindFolder2 + TEXT("\\*.vcproj"));
				while (bFinded2)
				{
					bFinded2 = findFile2.FindNextFile();
					if (!findFile.IsDots())
					{
						CString strVcproj = m_work.ReadFile2Txt(findFile2.GetFilePath());
						if (m_work.ProcessVcprojFile(strVcproj) == FALSE)
						{
							TRACE(TEXT("LastError = %d ,FilePath:%s"),m_work.GetLastError(),findFile2.GetFilePath());
							return FALSE;
						}
						CString strBackVcproj = findFile2.GetFilePath() + TEXT(".bak");
						m_work.RenameFile(findFile2.GetFilePath(),strBackVcproj);
						m_work.WriteString2File(strVcproj,findFile2.GetFilePath(),FALSE);
					}

				}
				findFile2.Close();

			}
		}
	}
	findFile.Close();
	return TRUE;
}
BOOL CVS2008ConverterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (m_xToolTip.RelayEvent(pMsg))
		return TRUE;

// 	m_toolTip.RelayEvent(pMsg);
	return CXTPDialogBase<CXTResizeDialog>::PreTranslateMessage(pMsg);
}

LRESULT CVS2008ConverterDlg::OnHyperlinkClicked( WPARAM wParam, LPARAM lParam )
{
	TRACE(_T("in CXSuperTooltipTestDlg::OnHyperlinkClicked\n"));

	if (wParam)
	{
		CString msg = _T("");
		msg.Format(TEXT("%s"),(LPCTSTR)wParam);
		if (msg == "QQ")
		{
			ShellExecute(NULL,TEXT("open"),TEXT("tencent://message/?Menu=yes&uin=191067617&Site=&Service=201&sigT=1bc763b91e8bb2f4f5213fa4632432762527b8939f547a5a53dc02f33a57300d2f94597503f4b0cf546b1c6c11111f60&sigU=c456bfa86b1f1ab9f7f2130d47d520f0c565a8e53e0ebd303f939e022139e7235e5c16e713241965"),NULL,NULL,SW_HIDE);

		}
	}
	else
	{
		AfxMessageBox(_T("ERROR!  UNM_HYPERLINK_CLICKED received from CPPToolTip with NULL wParam"),
			MB_OK | MB_ICONSTOP);
	}

	return TRUE;
}