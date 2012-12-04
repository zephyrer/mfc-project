
// HidePackFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HidePackFile.h"
#include "HidePackFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogSampleDlgBase
{
public:
	CAboutDlg() : CDialogSampleDlgBase(CAboutDlg::IDD)
	{
		fontTitle.CreatePointFont(180,L"微软雅黑"); 
	}
	CFont fontTitle;

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:

	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog()
	{
		CDialogSampleDlgBase::OnInitDialog();

		CString strTxt = 
			L"============================================\r\n"
			L"			隐藏压缩文件\r\n"
			L"============================================\r\n"
			L"\r\n"
			L"	随着电脑技术的普及和网络的盛行，电脑的使用已与现代生活密不可分了。当人们越来越依赖电脑，沉溺于电脑及网络所带来的效率与便利性时，如何确保资料安全、保障电脑储存及网络传输的安全使用就显得越发重要。根据统计，企业所面临的资讯安全威胁中，有65%以上来自于组织内部。由此可知，除了增设网络防火墙，以防止入侵的黑客以外，更应该加强内部电脑储存资料的安全性。有这样一些软件，它能够将你的秘密“藏”在图片中，看起来就像普通的图形文件一样。\r\n"
			L"\r\n"
			L"2012年12月4日 01:27:33\r\n"
			L"  * 完全更新和实现\r\n"
			L"  \r\n";
		// 此处设置简要说明。
		SetDlgItemText(IDC_EDIT_TEXT,strTxt);

		return TRUE;  // return TRUE unless you set the focus to a control
		// 异常: OCX 属性页应返回 FALSE
	}


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
	{
		HBRUSH hbr = CDialogSampleDlgBase::OnCtlColor(pDC, pWnd, nCtlColor);

		switch (pWnd->GetDlgCtrlID())  
		{  
		case IDC_STATIC_TITLE:  
			pDC->SetTextColor(RGB(255,0,0));  
			pDC->SelectObject(&fontTitle);  
			break;  
		} 
		return hbr;

	}

	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
	{
		PNMLINK pNMLink = (PNMLINK)pNMHDR;
		ShellExecute(m_hWnd, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
		*pResult = 0;
	}

};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogSampleDlgBase)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
END_MESSAGE_MAP()

// CHidePackFileDlg 对话框




CHidePackFileDlg::CHidePackFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogSampleDlgBase(CHidePackFileDlg::IDD, pParent,xtpResizeNoVertical)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);//这句必须有，
// 	XTPSkinManager()->LoadSkin(_T("Metro.cjstyles"));//这句是设置一种风格，Office2007.cjstyles是风格文件名，要拷贝过来。
	
	XTPSkinManager()->GetResourceFile()->SetModuleHandle(AfxGetInstanceHandle());
	XTPSkinManager()->LoadSkin(NULL, _T("NormalADTL.ini")); 
	XTPSkinManager()->SetApplyOptions(xtpSkinApplyFrame | xtpSkinApplyMetrics);
	
	XTPSkinManager()->SetAutoApplyNewThreads(FALSE); // 去自动Hook线程
	
}

void CHidePackFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSampleDlgBase::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_EDIT1,m_wndImage);
	DDX_Control(pDX,IDC_EDIT2,m_wndPack);

}

BEGIN_MESSAGE_MAP(CHidePackFileDlg, CDialogSampleDlgBase)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_ACTIVATE()

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CHidePackFileDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LOG_OUT, &CHidePackFileDlg::OnNMClickLogOut)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

// 自定义结构体，用来处理透明
typedef struct tag_Transpren{
	int nStart;
	int nEnd;
	int nStep;
}TRANSPARENT_TAG,*LPTRANSPARENT_TAG;

int		nNowTran_g = 255;	//现在的透明值
HANDLE  hThread	   = NULL;	//线程句柄

// CHidePackFileDlg 消息处理程序

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

	// 判断输入参数是否合理
	if (nStart>255 || nStart<0 || nEnd>255 || nEnd<0)	return;

	// 几次用来调试输出时显示换行
	int nSum = 0;
	
	try{
		while (nStart>nEnd ? (nNowTran_g>=nEnd):(nNowTran_g<nEnd))
		{
			DWORD dwExStyle=GetWindowLong(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),GWL_EXSTYLE); 
			if((dwExStyle&0x80000)!=0x80000) 
				SetWindowLong(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),GWL_EXSTYLE,dwExStyle|WS_EX_LAYERED); 

			::SetLayeredWindowAttributes(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),0,nNowTran_g,2);
			Sleep(10);
			nNowTran_g += nStart>nEnd?(-1 * nStep):nStep;

			// if (nSum++% 15 == 0)	// TRACE(TEXT("\n"));
			// TRACE(TEXT("%d\t"),nNowTran_g);
		}
		::RedrawWindow(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), NULL, NULL,
			RDW_ERASE | RDW_INVALIDATE | 
			RDW_FRAME | RDW_ALLCHILDREN);
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


BOOL CHidePackFileDlg::OnInitDialog()
{
	CDialogSampleDlgBase::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	m_wndPack.Register();

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

	ShowWindow(SW_MINIMIZE);

	CFileDialog* fImageDlg = new CFileDialog(TRUE,NULL,NULL,6,_T("全部支持文件(*.jpg,*.bmp,*.gif,*.png)|*.jpg;*.bmp;*.gif;*.png|JPG文件(*.jpg)|*.jpg|BMP文件(*.bmp)|*.bmp|GIF文件(*.gif)|*.gif|全部文件(*.*)|*.*||"),NULL,0,FALSE);
	CFileDialog* fPackDlg = new CFileDialog(TRUE,NULL,NULL,6,_T("全部支持文件(*.zip,*.7z,*.rar)|*.zip;*.7z;*.rar|ZIP文件(*.zip)|*.zip|7z压缩文件(*.7z)|*.7z|Rar压缩文件(*.rar)|*.rar|全部文件(*.*)|*.*||"),NULL,0,FALSE);

	m_wndImage.Initialize(this,BES_XTP_CHOOSEFILE);
	m_wndPack.Initialize(this,BES_XTP_CHOOSEFILE);

	m_wndImage.SetFileDialog(fImageDlg);
	m_wndPack.SetFileDialog(fPackDlg);


	SetResize(IDC_EDIT1,XTP_ANCHOR_TOPLEFT,XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_EDIT2,XTP_ANCHOR_TOPLEFT,XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_BUTTON1,XTP_ANCHOR_TOPRIGHT,XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_LOG_OUT,XTP_ANCHOR_TOPLEFT,XTP_ANCHOR_TOPRIGHT);


	ShowWindow(SW_RESTORE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHidePackFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogSampleDlgBase::OnSysCommand(nID, lParam);
		//   手动添加WM_NCLBUTTONUP事件
		if((nID&0xFFF0)==SC_MOVE||(nID&0xFFF0)==SC_SIZE) PostMessage(WM_NCLBUTTONUP,nID,lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHidePackFileDlg::OnPaint()
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
		CDialogSampleDlgBase::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHidePackFileDlg::OnQueryDragIcon()
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
void CHidePackFileDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TRACE("------------OnNcLButtonDown------nHitTest:%d\n",nHitTest);

	if (hThread != NULL)	TerminateThread(hThread,0);

	static TRANSPARENT_TAG tran={nNowTran_g,100,7};
	hThread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)threadCallTran,&tran,NULL,NULL);

	if (nHitTest != NULL)
		CDialogSampleDlgBase::OnNcLButtonDown(nHitTest, point);
}


/*******************************************************************************
函数名称:				CTranTestDlg::OnNcLButtonUp	松开标题栏
================================================================================
返回值:					void
--------------------------------------------------------------------------------
文件作者:				King.Sollyu					QQ:191067617
*******************************************************************************/
void CHidePackFileDlg::OnNcLButtonUp(UINT nHitTest, CPoint point)
{

	WaitForSingleObject(hThread,INFINITE);
	// TRACE("------------OnNcLButtonUp--------nHitTest:%d\n",nHitTest);


	static TRANSPARENT_TAG tran={nNowTran_g,255,7};
	hThread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)threadCallTran,&tran,NULL,NULL);

	if (nHitTest != NULL)
		CDialogSampleDlgBase::OnNcLButtonUp(nHitTest, point);
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
void CHidePackFileDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogSampleDlgBase::OnActivate(nState, pWndOther, bMinimized);
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


void CHidePackFileDlg::OnBnClickedButton1()
{
	CString strPicImage,strPackPath,strFinishNamePath;
	GetDlgItemText(IDC_EDIT1,strPicImage);
	GetDlgItemText(IDC_EDIT2,strPackPath);

//=============================================================================================
// 检查输入的参数是否合格
	if (strPicImage == _TEXT(""))
	{
		m_wndImage.SetFocus();
		SetDlgItemText(IDC_LOG_OUT,_TEXT("请输入图片路径！"));
		return ;
	}else if (strPackPath == _TEXT(""))
	{
		m_wndPack.SetFocus();
		SetDlgItemText(IDC_LOG_OUT,_TEXT("请输入压缩包路径！"));
		return;
	}
	

	if (FILEEXISTS_S(strPicImage) == FALSE)
	{
		m_wndImage.SetFocus();
		m_wndImage.SetSel(0,strPicImage.GetLength());
		SetDlgItemText(IDC_LOG_OUT,_TEXT("无法访问到你输入的图片，请重新选择！"));
		return ;
	}else if (FILEEXISTS_S(strPackPath) == FALSE)
	{
		m_wndPack.SetFocus();
		m_wndPack.SetSel(0,strPackPath.GetLength());
		SetDlgItemText(IDC_LOG_OUT,_TEXT("无法访问到你输入的压缩包，请重新选择！"));
		return ;
	}
	CString strImageExt = PathFindExtension(strPicImage);
	CString strPackExt = PathFindExtension(strPackPath);

	if (CHECK_IS_PIC_FILE(strImageExt) == FALSE)
	{
		m_wndImage.SetFocus();
		m_wndImage.SetSel(0,strPicImage.GetLength());
		SetDlgItemText(IDC_LOG_OUT,_TEXT("你输入一个不合法的图片，请重新选择！"));
		return ;
	}else if (CHECK_IS_PACK_FILE(strPackExt) == FALSE)
	{
		m_wndPack.SetFocus();
		m_wndPack.SetSel(0,strPackPath.GetLength());
		SetDlgItemText(IDC_LOG_OUT,_TEXT("你输入一个不合法的压缩包，请重新选择！"));
		return ;
	}
//=============================================================================================



	strFinishNamePath.Format(_T("%s_合并%s"),strPicImage.Left(strPicImage.GetLength() - strImageExt.GetLength()),strImageExt);

	if (FILEEXISTS_S(strFinishNamePath) == TRUE)
	{
		CString strErr;
		strErr.Format(_TEXT("文件：%s 已存在！是否覆盖？"),strFinishNamePath);

		// HACK:将自定义窗口显示去掉
		// 		if (CMyExt::ShowError(strErr,80,TDCBF_YES_BUTTON | TDCBF_NO_BUTTON, _TEXT("重写提醒"),TD_QUESTION_ICON) == IDNO)
		if (MessageBox(strErr,_TEXT("重写提醒"),MB_ICONQUESTION | MB_YESNO) == IDNO )
			return;
	}

	CString strCMD;
	strCMD.Format(_T("/c copy /b \"%s\" + \"%s\" \"%s\""),strPicImage,strPackPath,strFinishNamePath);

	ShellExecute(this->m_hWnd,_T("open"),_T("cmd.exe"),strCMD,_T(""), SW_HIDE );

	CString strLog;
	strLog.Format(_TEXT("生成成功！文件路径：<a href=\"%s\">%s</a>"),strFinishNamePath,strFinishNamePath);
	SetDlgItemText(IDC_LOG_OUT,strLog);
}


void CHidePackFileDlg::OnNMClickLogOut(NMHDR *pNMHDR, LRESULT *pResult)
{
	PNMLINK pNMLink = (PNMLINK)pNMHDR;

	CString strOpenCommand;
	strOpenCommand.Format(_TEXT("/select,%s"),pNMLink->item.szUrl);

	ShellExecute(m_hWnd, _T("open"), _TEXT("explorer.exe"), strOpenCommand, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}

void CHidePackFileDlg::OnDropFiles(HDROP hDropInfo)
{
	MessageBox(_T("dsd"));
	CDialogSampleDlgBase::OnDropFiles(hDropInfo);
}
