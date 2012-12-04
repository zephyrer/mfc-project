#pragma once
#include "OleDropTargetEx.h"
class CXTPDropBrowseEdit : public CXTPBrowseEdit
{
public:
	CXTPDropBrowseEdit(void);
	virtual ~CXTPDropBrowseEdit(void);

	// Implementation
public:
	BOOL m_bDraging;
	virtual BOOL Register();

	// Generated message map functions
protected:
	COleDropTargetEx m_dropEx;
	//����OnDropEx��Ϣ�Ǳ���ģ�����OnDrop�������ᱻִ��
	//��ȻҲ������OnDropEx�����д�������
	virtual LRESULT OnDrop(WPARAM pDropInfoClass, LPARAM lParm);
	virtual LRESULT OnDropEx(WPARAM pDropInfoClass, LPARAM lParm);
	virtual LRESULT OnDragOver(WPARAM pDropInfoClass,LPARAM lParm);
	//{{AFX_MSG(CDropEdit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

