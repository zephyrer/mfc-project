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
	//处理OnDropEx消息是必须的，否则OnDrop函数不会被执行
	//当然也可以在OnDropEx函数中处理数据
	virtual LRESULT OnDrop(WPARAM pDropInfoClass, LPARAM lParm);
	virtual LRESULT OnDropEx(WPARAM pDropInfoClass, LPARAM lParm);
	virtual LRESULT OnDragOver(WPARAM pDropInfoClass,LPARAM lParm);
	//{{AFX_MSG(CDropEdit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

