#include "StdAfx.h"
#include "XTPDropBrowseEdit.h"


CXTPDropBrowseEdit::CXTPDropBrowseEdit(void)
{
	m_bDraging = FALSE;
}


CXTPDropBrowseEdit::~CXTPDropBrowseEdit(void)
{
}

BEGIN_MESSAGE_MAP(CXTPDropBrowseEdit, CXTPBrowseEdit)
	//{{AFX_MSG_MAP(CDropEdit)
	// ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(DROPM_DRAGOVER,OnDragOver)
	ON_MESSAGE(DROPM_DROPEX,OnDropEx)
	ON_MESSAGE(DROPM_DROP,OnDrop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropEdit message handlers

BOOL CXTPDropBrowseEdit::Register()
{
	return m_dropEx.Register( this );
}

LRESULT CXTPDropBrowseEdit::OnDragOver(WPARAM pDropInfoClass, LPARAM lParm)
{
	COleDropInfo* pInfo = (COleDropInfo* )pDropInfoClass;
	ASSERT(pInfo->IsKindOf(RUNTIME_CLASS(COleDropInfo)));

	pInfo->m_pDataObject;
	if( pInfo->m_pDataObject->IsDataAvailable( CF_TEXT ) )
		return DROPEFFECT_COPY;
	else
		return DROPEFFECT_NONE;
}

LRESULT CXTPDropBrowseEdit::OnDropEx(WPARAM pDropInfoClass, LPARAM lParm)
{
	return (DROPEFFECT)-1;
}

LRESULT CXTPDropBrowseEdit::OnDrop(WPARAM pDropInfoClass, LPARAM lParm)
{
	COleDropInfo* pInfo = (COleDropInfo* )pDropInfoClass;
	ASSERT(pInfo->IsKindOf(RUNTIME_CLASS(COleDropInfo)));

	if( pInfo->m_pDataObject->IsDataAvailable( CF_TEXT ) )
	{
		HGLOBAL hMem = pInfo->m_pDataObject->GetGlobalData( CF_TEXT );
		LPCTSTR lp = (LPCTSTR)GlobalLock((HGLOBAL) hMem);//lock source
		if (lp != NULL)
		{
			//Set Windows title with Drop text 
			SetWindowText( lp );
		}
		GlobalUnlock( hMem );//unlock source
		return TRUE;
	}
	else if (pInfo->m_pDataObject->IsDataAvailable( CF_ENHMETAFILE ))
	{
		AfxMessageBox(_T("ddd"));
		return FALSE;
	}
}

void CXTPDropBrowseEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDown(nFlags, point);

	//确信要拖动,而不是随便点击
	//按下左键250ms则认为要拖动
	Sleep(250);
	MSG msg;
	::PeekMessage(
		&msg,
		GetSafeHwnd(),
		WM_LBUTTONUP,
		WM_LBUTTONUP,
		PM_NOREMOVE
		);
	//随意点击而已,返回
	if( msg.message==WM_LBUTTONUP )
		return;

	CString sContens;
	GetWindowText( sContens );
	HGLOBAL hData = GlobalAlloc(GHND|GMEM_SHARE, sContens.GetLength()+1 );
	char *pS = (char *)GlobalLock( hData );
	memcpy( pS, (LPCTSTR)sContens, sContens.GetLength()+1 );
	GlobalUnlock( hData );

	m_bDraging = TRUE;	//开始拖动
	COleDataSource source;
	source.CacheGlobalData( CF_TEXT, hData );
	source.DoDragDrop( DROPEFFECT_COPY );
	GlobalFree( hData );
	m_bDraging = FALSE;	//完成拖动
}
