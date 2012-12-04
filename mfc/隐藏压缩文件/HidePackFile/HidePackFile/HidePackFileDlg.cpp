
// HidePackFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HidePackFile.h"
#include "HidePackFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogSampleDlgBase
{
public:
	CAboutDlg() : CDialogSampleDlgBase(CAboutDlg::IDD)
	{
		fontTitle.CreatePointFont(180,L"΢���ź�"); 
	}
	CFont fontTitle;

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:

	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog()
	{
		CDialogSampleDlgBase::OnInitDialog();

		CString strTxt = 
			L"============================================\r\n"
			L"			����ѹ���ļ�\r\n"
			L"============================================\r\n"
			L"\r\n"
			L"	���ŵ��Լ������ռ��������ʢ�У����Ե�ʹ�������ִ������ܲ��ɷ��ˡ�������Խ��Խ�������ԣ������ڵ��Լ�������������Ч���������ʱ�����ȷ�����ϰ�ȫ�����ϵ��Դ��漰���紫��İ�ȫʹ�þ��Ե�Խ����Ҫ������ͳ�ƣ���ҵ�����ٵ���Ѷ��ȫ��в�У���65%������������֯�ڲ����ɴ˿�֪�����������������ǽ���Է�ֹ���ֵĺڿ����⣬��Ӧ�ü�ǿ�ڲ����Դ������ϵİ�ȫ�ԡ�������һЩ��������ܹ���������ܡ��ء���ͼƬ�У�������������ͨ��ͼ���ļ�һ����\r\n"
			L"\r\n"
			L"2012��12��4�� 01:27:33\r\n"
			L"  * ��ȫ���º�ʵ��\r\n"
			L"  \r\n";
		// �˴����ü�Ҫ˵����
		SetDlgItemText(IDC_EDIT_TEXT,strTxt);

		return TRUE;  // return TRUE unless you set the focus to a control
		// �쳣: OCX ����ҳӦ���� FALSE
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

// CHidePackFileDlg �Ի���




CHidePackFileDlg::CHidePackFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogSampleDlgBase(CHidePackFileDlg::IDD, pParent,xtpResizeNoVertical)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);//�������У�
// 	XTPSkinManager()->LoadSkin(_T("Metro.cjstyles"));//���������һ�ַ��Office2007.cjstyles�Ƿ���ļ�����Ҫ����������
	
	XTPSkinManager()->GetResourceFile()->SetModuleHandle(AfxGetInstanceHandle());
	XTPSkinManager()->LoadSkin(NULL, _T("NormalADTL.ini")); 
	XTPSkinManager()->SetApplyOptions(xtpSkinApplyFrame | xtpSkinApplyMetrics);
	
	XTPSkinManager()->SetAutoApplyNewThreads(FALSE); // ȥ�Զ�Hook�߳�
	
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

// �Զ���ṹ�壬��������͸��
typedef struct tag_Transpren{
	int nStart;
	int nEnd;
	int nStep;
}TRANSPARENT_TAG,*LPTRANSPARENT_TAG;

int		nNowTran_g = 255;	//���ڵ�͸��ֵ
HANDLE  hThread	   = NULL;	//�߳̾��

// CHidePackFileDlg ��Ϣ�������

/*******************************************************************************
��������:				Transparen	���ô���͸��������
================================================================================
����˵��:				int nStart	��ʼ͸����λ��
����˵��:				int nEnd	����͸����	
����˵��:				int nStep	����	
--------------------------------------------------------------------------------
����ֵ:					void
--------------------------------------------------------------------------------
�ļ�����:				King.Sollyu					QQ:191067617
*******************************************************************************/

void Transparen(int nStart,int nEnd,int nStep)
{
	// TRACE("Thread is start DownTransparen(%d,%d,%d)",nStart,nEnd,nStep);

	// �ж���������Ƿ����
	if (nStart>255 || nStart<0 || nEnd>255 || nEnd<0)	return;

	// ���������������ʱ��ʾ����
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
		// TRACE(TEXT("�ڴ���͸�����ϳ��������⣬���������DownTransparen(%d,%d,%d)"),nStart,nEnd,nStep);

	}
	return;
}


/*******************************************************************************
��������:				threadCallTran	�̵߳���͸�������
================================================================================
����˵��:				LPVOID lParam		
--------------------------------------------------------------------------------
����ֵ:					void
--------------------------------------------------------------------------------
�ļ�����:				King.Sollyu					QQ:191067617
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

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	m_wndPack.Register();

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	CFileDialog* fImageDlg = new CFileDialog(TRUE,NULL,NULL,6,_T("ȫ��֧���ļ�(*.jpg,*.bmp,*.gif,*.png)|*.jpg;*.bmp;*.gif;*.png|JPG�ļ�(*.jpg)|*.jpg|BMP�ļ�(*.bmp)|*.bmp|GIF�ļ�(*.gif)|*.gif|ȫ���ļ�(*.*)|*.*||"),NULL,0,FALSE);
	CFileDialog* fPackDlg = new CFileDialog(TRUE,NULL,NULL,6,_T("ȫ��֧���ļ�(*.zip,*.7z,*.rar)|*.zip;*.7z;*.rar|ZIP�ļ�(*.zip)|*.zip|7zѹ���ļ�(*.7z)|*.7z|Rarѹ���ļ�(*.rar)|*.rar|ȫ���ļ�(*.*)|*.*||"),NULL,0,FALSE);

	m_wndImage.Initialize(this,BES_XTP_CHOOSEFILE);
	m_wndPack.Initialize(this,BES_XTP_CHOOSEFILE);

	m_wndImage.SetFileDialog(fImageDlg);
	m_wndPack.SetFileDialog(fPackDlg);


	SetResize(IDC_EDIT1,XTP_ANCHOR_TOPLEFT,XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_EDIT2,XTP_ANCHOR_TOPLEFT,XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_BUTTON1,XTP_ANCHOR_TOPRIGHT,XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_LOG_OUT,XTP_ANCHOR_TOPLEFT,XTP_ANCHOR_TOPRIGHT);


	ShowWindow(SW_RESTORE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
		//   �ֶ����WM_NCLBUTTONUP�¼�
		if((nID&0xFFF0)==SC_MOVE||(nID&0xFFF0)==SC_SIZE) PostMessage(WM_NCLBUTTONUP,nID,lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHidePackFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogSampleDlgBase::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHidePackFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*******************************************************************************
��������:				CTranTestDlg::OnNcLButtonDown	���±�����
================================================================================
����ֵ:					void
--------------------------------------------------------------------------------
�ļ�����:				King.Sollyu					QQ:191067617
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
��������:				CTranTestDlg::OnNcLButtonUp	�ɿ�������
================================================================================
����ֵ:					void
--------------------------------------------------------------------------------
�ļ�����:				King.Sollyu					QQ:191067617
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
��������:				CTranTestDlg::OnActivate	������¼�
================================================================================
����˵��:				UINT nState		
����˵��:				CWnd * pWndOther		
����˵��:				BOOL bMinimized		
--------------------------------------------------------------------------------
����ֵ:					void
--------------------------------------------------------------------------------
�ļ�����:				King.Sollyu					QQ:191067617
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
	case WA_CLICKACTIVE:// ��꼤�ʽ
		// TRACE("------------WA_CLICKACTIVE------\t0x%04x\n",nState);
		if(nNowTran_g > 250) break;
		if (hThread != NULL)	TerminateThread(hThread,0);
		OnNcLButtonUp(NULL,NULL);

		break;
	case WA_ACTIVE://�������ʽ������ Alt+Tab
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
// �������Ĳ����Ƿ�ϸ�
	if (strPicImage == _TEXT(""))
	{
		m_wndImage.SetFocus();
		SetDlgItemText(IDC_LOG_OUT,_TEXT("������ͼƬ·����"));
		return ;
	}else if (strPackPath == _TEXT(""))
	{
		m_wndPack.SetFocus();
		SetDlgItemText(IDC_LOG_OUT,_TEXT("������ѹ����·����"));
		return;
	}
	

	if (FILEEXISTS_S(strPicImage) == FALSE)
	{
		m_wndImage.SetFocus();
		m_wndImage.SetSel(0,strPicImage.GetLength());
		SetDlgItemText(IDC_LOG_OUT,_TEXT("�޷����ʵ��������ͼƬ��������ѡ��"));
		return ;
	}else if (FILEEXISTS_S(strPackPath) == FALSE)
	{
		m_wndPack.SetFocus();
		m_wndPack.SetSel(0,strPackPath.GetLength());
		SetDlgItemText(IDC_LOG_OUT,_TEXT("�޷����ʵ��������ѹ������������ѡ��"));
		return ;
	}
	CString strImageExt = PathFindExtension(strPicImage);
	CString strPackExt = PathFindExtension(strPackPath);

	if (CHECK_IS_PIC_FILE(strImageExt) == FALSE)
	{
		m_wndImage.SetFocus();
		m_wndImage.SetSel(0,strPicImage.GetLength());
		SetDlgItemText(IDC_LOG_OUT,_TEXT("������һ�����Ϸ���ͼƬ��������ѡ��"));
		return ;
	}else if (CHECK_IS_PACK_FILE(strPackExt) == FALSE)
	{
		m_wndPack.SetFocus();
		m_wndPack.SetSel(0,strPackPath.GetLength());
		SetDlgItemText(IDC_LOG_OUT,_TEXT("������һ�����Ϸ���ѹ������������ѡ��"));
		return ;
	}
//=============================================================================================



	strFinishNamePath.Format(_T("%s_�ϲ�%s"),strPicImage.Left(strPicImage.GetLength() - strImageExt.GetLength()),strImageExt);

	if (FILEEXISTS_S(strFinishNamePath) == TRUE)
	{
		CString strErr;
		strErr.Format(_TEXT("�ļ���%s �Ѵ��ڣ��Ƿ񸲸ǣ�"),strFinishNamePath);

		// HACK:���Զ��崰����ʾȥ��
		// 		if (CMyExt::ShowError(strErr,80,TDCBF_YES_BUTTON | TDCBF_NO_BUTTON, _TEXT("��д����"),TD_QUESTION_ICON) == IDNO)
		if (MessageBox(strErr,_TEXT("��д����"),MB_ICONQUESTION | MB_YESNO) == IDNO )
			return;
	}

	CString strCMD;
	strCMD.Format(_T("/c copy /b \"%s\" + \"%s\" \"%s\""),strPicImage,strPackPath,strFinishNamePath);

	ShellExecute(this->m_hWnd,_T("open"),_T("cmd.exe"),strCMD,_T(""), SW_HIDE );

	CString strLog;
	strLog.Format(_TEXT("���ɳɹ����ļ�·����<a href=\"%s\">%s</a>"),strFinishNamePath,strFinishNamePath);
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
