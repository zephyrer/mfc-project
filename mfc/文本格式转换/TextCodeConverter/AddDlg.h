#pragma once
#include "afxwin.h"


// CAddDlg 对话框
#define IDI_DLG_WITH		480


class CAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	CAddDlg(LPCTSTR szPath);
	virtual ~CAddDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	BOOL m_bUpdate;
	LPCTSTR m_szPath;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_ComboBox;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeCombo1();
	CButton m_ok;
	CButton m_cancel;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
