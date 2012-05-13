// AddDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TextCodeConverter.h"
#include "AddDlg.h"
#include "TextCodeConverterDlg.h"


// CAddDlg 对话框

IMPLEMENT_DYNAMIC(CAddDlg, CDialog)

CAddDlg::CAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddDlg::IDD, pParent)
{
	m_bUpdate = FALSE;
}

CAddDlg::CAddDlg( LPCTSTR szPath )
	: CDialog(CAddDlg::IDD, NULL)
{
	m_bUpdate = TRUE;
	m_szPath = szPath;

}
CAddDlg::~CAddDlg()
{
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
}


BEGIN_MESSAGE_MAP(CAddDlg, CDialog)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAddDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CAddDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAddDlg 消息处理程序

BOOL CAddDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if (m_bUpdate == TRUE)
	{
		m_ComboBox.SetCurSel(0);
		SetDlgItemText(IDC_EDIT1,m_szPath);
		SetWindowPos(NULL,0,0,IDI_DLG_WITH,120,SWP_NOMOVE);//设置窗口的初始大小为750*555

	}else
	{
		SetWindowPos(NULL,0,0,IDI_DLG_WITH,50,SWP_NOMOVE);//设置窗口的初始大小为750*555

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAddDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	TRACE(TEXT("nType:%d cx:%d cy:%d\n"),nType,cx,cy);

	if (m_ComboBox.GetSafeHwnd() != NULL)
	{
		CRect rc,rcOK,rcCancel;
		m_ComboBox.GetClientRect(&rc);
// 		rc.left = rc.left + 10;
// 		rc.right = cx -10 ;
		rc.right = cx;
		m_ComboBox.MoveWindow(&rc);
	}


}

void CAddDlg::OnCbnSelchangeCombo1()
{
	CRect rcDlg;
	GetClientRect(&rcDlg);

	GetDlgItem(IDC_EDIT1)	->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LB_1)	->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON1)	->ShowWindow(SW_HIDE);
	GetDlgItem(IDOK)		->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)	->ShowWindow(SW_HIDE);


	for(int i=rcDlg.bottom;rcDlg.bottom>=50;i--)
	{
		rcDlg.bottom -= 1;
		SetWindowPos(NULL,0,0,IDI_DLG_WITH,rcDlg.bottom,SWP_NOMOVE);
	}


	if (m_ComboBox.GetCurSel() == 0)
	{
		SetDlgItemText(IDC_EDIT1,m_szPath);
		for(int i=rcDlg.bottom;rcDlg.bottom<=120;i++)
		{
			rcDlg.bottom += 1;
			SetWindowPos(NULL,0,0,IDI_DLG_WITH,rcDlg.bottom,SWP_NOMOVE);
		}
		
	}else
	{
		SetDlgItemText(IDC_EDIT1,TEXT(""));
		for(int i=rcDlg.bottom;rcDlg.bottom<=120;i++)
		{
			rcDlg.bottom += 1;
			SetWindowPos(NULL,0,0,IDI_DLG_WITH,rcDlg.bottom,SWP_NOMOVE);
		}
	}

	GetDlgItem(IDC_EDIT1)	->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(IDC_LB_1)	->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(IDC_BUTTON1)	->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(IDOK)		->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(IDCANCEL)	->ShowWindow(SW_SHOWNORMAL);
}

void CAddDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strEdit;
	CTextCodeConverterDlg* MainDlg = (CTextCodeConverterDlg*)this->GetParent();	
	ASSERT(MainDlg->GetSafeHwnd());

	CListBox* listBox = (CListBox*)MainDlg->GetDlgItem(IDC_LIST1);
	ASSERT(listBox->GetSafeHwnd());

	GetDlgItemText(IDC_EDIT1,strEdit);

	if (m_bUpdate == TRUE)
	{
		listBox->DeleteString(listBox->GetCurSel());
		listBox->InsertString(listBox->GetCurSel(),strEdit);
	}
	else
	{
		listBox->AddString(strEdit);
	}

	OnOK();
}

void CAddDlg::OnBnClickedButton1()
{
	CFileDialog fDlg(TRUE);
	if (fDlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT1,fDlg.GetPathName());
	}
}
