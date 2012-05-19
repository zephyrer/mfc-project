
// TextCodeConverterDlg.h : 头文件
//

#pragma once

#define CDialogSampleDlgBase CXTPDialogBase<CXTResizeDialog>
#include "out/WorkClass.h"

// CTextCodeConverterDlg 对话框
class CTextCodeConverterDlg : public CDialogSampleDlgBase
{
// 构造
public:
	CTextCodeConverterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEXTCODECONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CWorkClass m_work;
	BOOL isJustProce;

	void OnInitUI();
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnGo();
	afx_msg void OnLbnDblclkList1();
};
extern CTextCodeConverterApp theApp;