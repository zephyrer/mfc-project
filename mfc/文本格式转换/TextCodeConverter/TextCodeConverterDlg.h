
// TextCodeConverterDlg.h : ͷ�ļ�
//

#pragma once

#define CDialogSampleDlgBase CXTPDialogBase<CXTResizeDialog>
#include "out/WorkClass.h"

// CTextCodeConverterDlg �Ի���
class CTextCodeConverterDlg : public CDialogSampleDlgBase
{
// ����
public:
	CTextCodeConverterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEXTCODECONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CWorkClass m_work;
	BOOL isJustProce;

	void OnInitUI();
	// ���ɵ���Ϣӳ�亯��
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