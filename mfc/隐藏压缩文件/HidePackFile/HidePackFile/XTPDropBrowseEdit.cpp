#include "StdAfx.h"
#include "XTPDropBrowseEdit.h"


CXTPDropBrowseEdit::CXTPDropBrowseEdit( BOOL isImage )
{
	m_isImage = isImage;
}

CXTPDropBrowseEdit::~CXTPDropBrowseEdit(void)
{
}
BEGIN_MESSAGE_MAP(CXTPDropBrowseEdit, CXTPBrowseEdit)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


void CXTPDropBrowseEdit::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	if (count != 0 && count == 1)
	{
		wchar_t filePath[200];
		int pathLen = DragQueryFile(hDropInfo, 0, filePath, sizeof(filePath));
		
		if (m_isImage == TRUE)
		{ 
			if (CHECK_IS_PIC_FILE(PathFindExtension(filePath)) == TRUE) 
				SetWindowText(filePath); 
		}
		else 
		{ 
			if (CHECK_IS_PACK_FILE(PathFindExtension(filePath)) == TRUE) 
				SetWindowText(filePath); 
		}
	}

	DragFinish(hDropInfo);

	CXTPBrowseEdit::OnDropFiles(hDropInfo);
}
