
// VS2008ConverterDlg.h : 头文件
//

#pragma once

#define CDialogSampleDlgBase CXTPDialogBase<CXTResizeDialog>
#include "workclass.h"
#include "XSupperTip/XSuperTooltip.h"

// CVS2008ConverterDlg 对话框
class CVS2008ConverterDlg : public CDialogSampleDlgBase
{
// 构造
public:
	CVS2008ConverterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VS2008CONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;
	WorkClass m_work;

	void OnInitUI();
	CToolTipCtrl m_toolTip;
	CXSuperTooltip m_xToolTip;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnHyperlinkClicked(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	afx_msg BOOL ProcessFunc( CString &strFilePath );
	CString loadStringFromID(UINT nID);
	CString getDlgItemText_(UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


