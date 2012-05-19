
// TextCodeConverterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TextCodeConverter.h"
#include "TextCodeConverterDlg.h"
#include "adddlg.h"

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


// CTextCodeConverterDlg 对话框




CTextCodeConverterDlg::CTextCodeConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogSampleDlgBase(CTextCodeConverterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);//这句必须有，
	XTPSkinManager()->LoadSkin(_T("Metro.cjstyles"));//这句是设置一种风格，Office2007.cjstyles是风格文件名，要拷贝过来。
	XTPSkinManager()->SetAutoApplyNewThreads(FALSE); // 去自动Hook线程
}

void CTextCodeConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTextCodeConverterDlg, CDialog)
//	ON_WM_SYSCOMMAND()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_ADD, &CTextCodeConverterDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_GO, &CTextCodeConverterDlg::OnBnClickedBtnGo)
	ON_LBN_DBLCLK(IDC_LIST1, &CTextCodeConverterDlg::OnLbnDblclkList1)
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

	// 判断输入参数是否合理
	if (nStart>255 || nStart<0 || nEnd>255 || nEnd<0)	return;

	// 几次用来调试输出时显示换行
	int nSum = 0;

	try{
		SetWindowLong(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),GWL_EXSTYLE, 
			GetWindowLong(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),GWL_EXSTYLE) | 0x80000); 

		while (nStart>nEnd ? (nNowTran_g>=nEnd):(nNowTran_g<nEnd))
		{
			SetLayeredWindowAttributes(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),0,nNowTran_g,2);
			Sleep(10);
			nNowTran_g += nStart>nEnd?(-1 * nStep):nStep;

			// if (nSum++% 15 == 0)	// TRACE(TEXT("\n"));
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
// CTextCodeConverterDlg 消息处理程序
void CTextCodeConverterDlg::OnInitUI()
{
	SetWindowText(theApp.LoadStringFormId(ids_ui_main_dlg_title));
	SetDlgItemText(IDC_BTN_ADD,theApp.LoadStringFormId(ids_ui_main_btn_add));
	SetDlgItemText(IDC_BTN_GO,theApp.LoadStringFormId(ids_ui_main_btn_go));

}

BOOL CTextCodeConverterDlg::OnInitDialog()
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

	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
	OnInitUI();

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTextCodeConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTextCodeConverterDlg::OnPaint()
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
HCURSOR CTextCodeConverterDlg::OnQueryDragIcon()
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
void CTextCodeConverterDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TRACE("------------OnNcLButtonDown------nHitTest:%d\n",nHitTest);

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
void CTextCodeConverterDlg::OnNcLButtonUp(UINT nHitTest, CPoint point)
{

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
void CTextCodeConverterDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CXTPDialogBase<CXTResizeDialog>::OnActivate(nState, pWndOther, bMinimized);
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



void CTextCodeConverterDlg::OnBnClickedBtnAdd()
{
	CListBox* listBox = (CListBox*)GetDlgItem(IDC_LIST1);

	CAddDlg addDlg;
	if ( addDlg.DoModal() == IDOK)
	{
		if (isJustProce == TRUE)
		{
			listBox->ResetContent();
			isJustProce = FALSE;
		}
		listBox->AddString(addDlg.m_strEdit);
	}
}

void CTextCodeConverterDlg::OnBnClickedBtnGo()
{
	CListBox*  listBox = (CListBox*)GetDlgItem(IDC_LIST1);
	CComboBox* comboBox = (CComboBox*)GetDlgItem(IDC_COMBO1);

	// 定义保存列表框中输入路径
	CStringArray straList;

	// 定义列表框中获得的路径和临时转换的文本变量
	CString strListBoxItemPath,strText;

	// 循环列表框中的每一个元素
	for (int i=0;i<listBox->GetCount();i++)
	{
		// 获得列表框中指定文本内容并添加到数组中
		listBox->GetText(i,strListBoxItemPath);
		straList.Add(strListBoxItemPath);		
	}

	int nListNowPos = 0;
	listBox->ResetContent();

	for (int i=0;i<straList.GetCount();i++)
	{
		listBox->InsertString(nListNowPos,straList.GetAt(i));
		// 采用搜索模式，这样可以支持通配符
		CFileFind findFile;
		BOOL isFinded = findFile.FindFile(straList.GetAt(i));
		while (isFinded == TRUE)
		{
			isFinded = findFile.FindNextFile();

			// 读取找到的文件，存放在strText中
			m_work.ReadFile_(findFile.GetFilePath(),strText);

			//备份原始文件：将来存放在设置中（此处如果备份文件已存在，则会出现错误）
			CFile::Rename(findFile.GetFilePath(),findFile.GetFilePath()+TEXT(".bak"));

			// 保存选择的保存编码（全部重新文件）
			if (m_work.SaveFile_(findFile.GetFilePath(),strText,comboBox->GetCurSel()==0?CWorkClass::FILE_ANSI:CWorkClass::FILE_UNICODE) == TRUE)
			{
				CString strTemp;
				strTemp.Format(TEXT("    %s\t%10s"),findFile.GetFilePath(),TEXT("√"));
				listBox->InsertString(++nListNowPos,strTemp);
			}
		}
		listBox->InsertString(++nListNowPos,TEXT(""));
	}

	isJustProce = TRUE;

}

void CTextCodeConverterDlg::OnLbnDblclkList1()
{
	CListBox*  listBox = (CListBox*)GetDlgItem(IDC_LIST1);
	int nSel = listBox->GetCurSel();

	if (nSel >= 0)
	{
			CString strTmp;
			
			listBox->GetText(nSel,strTmp);
			if (strTmp.Left(1) != TEXT(" "))
			{
				CAddDlg addDlg(strTmp);
				addDlg.DoModal();
			}
	}
}
