
// TextCodeConverterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TextCodeConverter.h"
#include "TextCodeConverterDlg.h"
#include "adddlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTextCodeConverterDlg �Ի���




CTextCodeConverterDlg::CTextCodeConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogSampleDlgBase(CTextCodeConverterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);//�������У�
	XTPSkinManager()->LoadSkin(_T("Metro.cjstyles"));//���������һ�ַ��Office2007.cjstyles�Ƿ���ļ�����Ҫ����������
	XTPSkinManager()->SetAutoApplyNewThreads(FALSE); // ȥ�Զ�Hook�߳�
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

// �Զ���ṹ�壬��������͸��
typedef struct tag_Transpren{
	int nStart;
	int nEnd;
	int nStep;
}TRANSPARENT_TAG,*LPTRANSPARENT_TAG;

int		nNowTran_g = 255;	//���ڵ�͸��ֵ
HANDLE  hThread	   = NULL;	//�߳̾��

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
// CTextCodeConverterDlg ��Ϣ�������
void CTextCodeConverterDlg::OnInitUI()
{
	SetWindowText(theApp.LoadStringFormId(ids_ui_main_dlg_title));
	SetDlgItemText(IDC_BTN_ADD,theApp.LoadStringFormId(ids_ui_main_btn_add));
	SetDlgItemText(IDC_BTN_GO,theApp.LoadStringFormId(ids_ui_main_btn_go));

}

BOOL CTextCodeConverterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

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

	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
	OnInitUI();

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
		//   �ֶ����WM_NCLBUTTONUP�¼�
		if((nID&0xFFF0)==SC_MOVE) PostMessage(WM_NCLBUTTONUP,nID,lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTextCodeConverterDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTextCodeConverterDlg::OnQueryDragIcon()
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
��������:				CTranTestDlg::OnNcLButtonUp	�ɿ�������
================================================================================
����ֵ:					void
--------------------------------------------------------------------------------
�ļ�����:				King.Sollyu					QQ:191067617
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
void CTextCodeConverterDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CXTPDialogBase<CXTResizeDialog>::OnActivate(nState, pWndOther, bMinimized);
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

	// ���屣���б��������·��
	CStringArray straList;

	// �����б���л�õ�·������ʱת�����ı�����
	CString strListBoxItemPath,strText;

	// ѭ���б���е�ÿһ��Ԫ��
	for (int i=0;i<listBox->GetCount();i++)
	{
		// ����б����ָ���ı����ݲ���ӵ�������
		listBox->GetText(i,strListBoxItemPath);
		straList.Add(strListBoxItemPath);		
	}

	int nListNowPos = 0;
	listBox->ResetContent();

	for (int i=0;i<straList.GetCount();i++)
	{
		listBox->InsertString(nListNowPos,straList.GetAt(i));
		// ��������ģʽ����������֧��ͨ���
		CFileFind findFile;
		BOOL isFinded = findFile.FindFile(straList.GetAt(i));
		while (isFinded == TRUE)
		{
			isFinded = findFile.FindNextFile();

			// ��ȡ�ҵ����ļ��������strText��
			m_work.ReadFile_(findFile.GetFilePath(),strText);

			//����ԭʼ�ļ�����������������У��˴���������ļ��Ѵ��ڣ������ִ���
			CFile::Rename(findFile.GetFilePath(),findFile.GetFilePath()+TEXT(".bak"));

			// ����ѡ��ı�����루ȫ�������ļ���
			if (m_work.SaveFile_(findFile.GetFilePath(),strText,comboBox->GetCurSel()==0?CWorkClass::FILE_ANSI:CWorkClass::FILE_UNICODE) == TRUE)
			{
				CString strTemp;
				strTemp.Format(TEXT("    %s\t%10s"),findFile.GetFilePath(),TEXT("��"));
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
