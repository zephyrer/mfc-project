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
	m_szPath = TEXT("");
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

inline void CAddDlg::OnInitUI()
{
	SetWindowText(theApp.LoadStringFormId(ids_ui_add_dlg_title));
	m_ComboBox.ResetContent();
	m_ComboBox.AddString(theApp.LoadStringFormId(ids_ui_add_com_file));
	m_ComboBox.AddString(theApp.LoadStringFormId(ids_ui_add_com_folder));
	SetDlgItemText(IDC_LB_1,theApp.LoadStringFormId(ids_ui_add_lb_path));
	SetDlgItemText(IDOK,theApp.LoadStringFormId(ids_ui_add_btn_ok));
	SetDlgItemText(IDCANCEL,theApp.LoadStringFormId(ids_ui_add_btn_cancel));
}


BOOL CAddDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	OnInitUI();
	if (m_bUpdate == TRUE)
	{
		m_ComboBox.SetCurSel(0);
		SetDlgItemText(IDC_EDIT1,m_szPath);
		SetWindowPos(NULL,0,0,IDI_DLG_WITH,125,SWP_NOMOVE);//设置窗口的初始大小为750*555

	}else
	{
		SetWindowPos(NULL,0,0,IDI_DLG_WITH,55,SWP_NOMOVE);//设置窗口的初始大小为750*555
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
		for(int i=rcDlg.bottom;rcDlg.bottom<=125;i++)
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
	// 获得主窗口句柄，用来在列表框中增加数据（虽然这样不安全，但个人喜欢）
	CTextCodeConverterDlg* MainDlg = (CTextCodeConverterDlg*)this->GetParent();	
	ASSERT(MainDlg->GetSafeHwnd());

	// 取得主窗口中的列表框句柄
	CListBox* listBox = (CListBox*)MainDlg->GetDlgItem(IDC_LIST1);
	ASSERT(listBox->GetSafeHwnd());

	GetDlgItemText(IDC_EDIT1,m_strEdit);

	// 如果在启动窗口时为修改内容，则删除对应位置数据之后，然后在增加
	if (m_bUpdate == TRUE)
	{
		listBox->DeleteString(listBox->GetCurSel());
		listBox->InsertString(listBox->GetCurSel(),m_strEdit);
	}
	OnOK();
}

/*******************************************************************************
函数名称:　　　　　　　　 OnBnClickedButton1　点击浏览按钮
================================================================================
返回值:　　　　　　　　 　 void
--------------------------------------------------------------------------------
文件作者:				King.Sollyu					QQ:191067617
*******************************************************************************/
void CAddDlg::OnBnClickedButton1()
{
	CFileDialog fDlg(TRUE,TEXT("txt"),NULL,4|2,theApp.LoadStringFormId(ids_ui_add_filedlg_filer));
	if (fDlg.DoModal() == IDOK)
	{
		// 如果下拉菜单选择的为文件就直接输出文件路径
		// 负责就将选中的 文件名 改为 *
		if (m_ComboBox.GetCurSel() == 0)
		{
			SetDlgItemText(IDC_EDIT1,fDlg.GetPathName());
		}else 
		{
			CString strPathTemp;

			strPathTemp.Format(TEXT("%s\\*.%s"),fDlg.GetPathName().Left(fDlg.GetPathName().ReverseFind('\\')),fDlg.GetFileExt());
			SetDlgItemText(IDC_EDIT1,strPathTemp);
		}

	}
}
