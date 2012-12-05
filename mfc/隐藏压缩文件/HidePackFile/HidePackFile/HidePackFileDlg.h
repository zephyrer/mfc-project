
// HidePackFileDlg.h : ͷ�ļ�
//

#pragma once
#include "XTPDropBrowseEdit.h"

#define CDialogSampleDlgBase CXTPResizeDialog
// #define CDialogSampleDlgBase CDialogEx


// CHidePackFileDlg �Ի���
class CHidePackFileDlg : public /*CDialogEx*/CDialogSampleDlgBase
{
// ����
public:
	CHidePackFileDlg(CWnd* pParent = NULL);	// ��׼���캯��

	CXTPDropBrowseEdit m_wndImage,m_wndPack;
	
// �Ի�������
	enum { IDD = IDD_HIDEPACKFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	inline BOOL CHECK_IS_PIC_FILE(CString strImageExt)
	{
		if (strImageExt.MakeLower() == _T(".jpg") || strImageExt.MakeLower() == _T(".gif") || strImageExt.MakeLower() == _T(".bmp") || strImageExt.MakeLower() == _T(".png") )
			return TRUE;
		return FALSE;
	};

	inline BOOL CHECK_IS_PACK_FILE(CString strPackExt)
	{
		if (strPackExt.MakeLower() == _T(".7z") || strPackExt.MakeLower() == _T(".zip") || strPackExt.MakeLower() == _T(".rar"))
			return TRUE;
		return FALSE;
	};


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	
	
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMClickLogOut(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDropFiles(HDROP hDropInfo);


};
