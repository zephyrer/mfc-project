#include "StdAfx.h"
#include "Update.h"


CUpdate::CUpdate( CString strPath )
:m_strURL(strPath)
{

	::CoInitialize(NULL);

	HRESULT HR = pDoc.CreateInstance(_uuidof(MSXML2::DOMDocument30));
	if(!SUCCEEDED(HR))
	{ 
		CMyExt::ShowError(_TEXT("更新程序在初始化时失败！"),GetLastError());
	}

	SetConfPath(CMyExt::GetTempFilePath());
	CMyExt::GetWebSrcCode(m_strURL,GetConfPath());

}
CUpdate::~CUpdate(void)
{
	pDoc.Release();
	DeleteFile(GetConfPath());

	::CoUninitialize();
}

CString CUpdate::GetSoftwareName()
{
	return OnGetSoftInfo(_TEXT("SoftInfo"),_TEXT("Description"));
}

CString CUpdate::OnGetSoftInfo(__in const LPCTSTR lpszInfo ,__in const LPCTSTR lpszAttr )
{

	// 定义要定位的子节点名称
	LPCTSTR lpszSubKeyName = lpszInfo;

	// 定义返回出来的变量
	LPCTSTR lpszResult     = _TEXT("");

	TRY 
	{
		// 加载文件
		pDoc->load(GetConfPath().GetString());

		// 直接跳到指定节点
		xmlNodes = pDoc->getElementsByTagName(lpszSubKeyName);

		// 如果定位失败。
		if (xmlNodes->Getlength() == 0)
		{ 
// 			CMyExt::ShowError(_TEXT("xmlNodes->Getlength() == 0"));
			return _TEXT("");
		}

		// 查看每个节点上面的名称。
		for (int i = 0; i < xmlNodes->Getitem(0)->Getattributes()->Getlength() ; i++)
		{
			// 获得节点名称。
			CString strNodeName = xmlNodes->Getitem(0)->Getattributes()->Getitem(i)->nodeName;
			// 如果名称匹配就获得对应的值。
			if ( strNodeName == lpszAttr )
			{
				lpszResult = (LPCTSTR)xmlNodes->Getitem(0)->Getattributes()->Getitem(i)->text;
				break;
			}
		}

		xmlNodes.Release();
	}
	CATCH (CMemoryException, e)
	{

	}
	END_CATCH
	


	return lpszResult;
}

CString CUpdate::GetVersionInfo( __in CString strAttr)
{
	MSXML2::IXMLDOMElementPtr	xmlNode;
	MSXML2::IXMLDOMNodeListPtr  xmlNodes;
	MSXML2::IXMLDOMNamedNodeMapPtr	xmlAttr;

	LPCTSTR lpszResult     = _TEXT("");

	try
	{	
		pDoc->load(GetConfPath().GetString());

		xmlNode = (MSXML2::IXMLDOMElementPtr)pDoc->selectSingleNode(_TEXT("//update"));
		xmlNodes = xmlNode->GetchildNodes();//子节点列表


		// 如果定位失败。
		if (xmlNodes->Getlength() == 0)
		{ 
// 			CMyExt::ShowError(_TEXT("xmlNodes->Getlength() == 0")); return _TEXT(""); 
		}


		MSXML2::IXMLDOMNodePtr xmlThisNode = xmlNodes->Getitem(xmlNodes->Getlength()-1);

		for (int i = 0; i < xmlThisNode->Getattributes()->Getlength() ; i++)
		{
			CString strNodeName = xmlThisNode->Getattributes()->Getitem(i)->nodeName;
			if ( strNodeName == strAttr )
			{
				// 如果名称匹配就获得对应的值。
				lpszResult = (LPCTSTR)xmlThisNode->Getattributes()->Getitem(i)->text;
				break;
			}
		}


		xmlNodes.Release();
	}
	catch (CException* e)
	{
		LPTSTR lpszErr = {0};
		e->GetErrorMessage(lpszErr,255);
		CMyExt::ShowError(CString( lpszErr ),GetLastError());
	}

	return lpszResult;
}

CString CUpdate::GetLastVersion()
{
	return OnGetSoftInfo(_TEXT("SoftVersion"),_TEXT("Versoin"));

// 	CString strAttr = _TEXT("SoftVersoin");
// 	return GetVersionInfo(strAttr);
}

CString CUpdate::GetLastTitle()
{
	return OnGetSoftInfo(_TEXT("SoftVersion"),_TEXT("UpdateTitle"));

// 	CString strAttr = _TEXT("UpdateTitle");
// 	return GetVersionInfo(strAttr);
}

CString CUpdate::GetLastTime()
{
	return OnGetSoftInfo(_TEXT("SoftVersion"),_TEXT("UpdateTime"));

// 	CString strAttr = _TEXT("UpdateTime");
// 	return GetVersionInfo(strAttr);
}

CString CUpdate::GetLastDownURL()
{
	return OnGetSoftInfo(_TEXT("SoftVersion"),_TEXT("DownURL"));

// 	CString strAttr = _TEXT("DownURL");
// 	return GetVersionInfo(strAttr);
}

CString CUpdate::GetLastText()
{
	return OnGetSoftInfo(_TEXT("SoftVersion"),_TEXT("UpdateText"));
	
// 	CString strAttr = _TEXT("UpdateText");
// 	return GetVersionInfo(strAttr);
}

BOOL CUpdate::CheckUpdate( __in double fNowVersion ,__in BOOL isTip /*= TRUE*/ )
{
	double fNewVersion = _wtof(GetLastVersion());

	if ( isTip == TRUE && fNewVersion > fNowVersion )
	{
		CString strDownURL;
		CTaskDialogEx taskDlg(AfxGetApp()->GetMainWnd());
		taskDlg.EnableHyperlinks(TRUE);
		taskDlg.EnableRelativePosition(TRUE);

		taskDlg.SetCommonButtons(TDCBF_OK_BUTTON);
		taskDlg.SetMainIcon(TD_INFORMATION_ICON);
		taskDlg.SetMainInstruction(GetLastTitle());
		taskDlg.SetContent(GetLastText());

		strDownURL.Format(_TEXT("更新时间：%s \r\n下载地址：<a href=\"%s\">点击下载</a>"),GetLastTime(),GetLastDownURL());

		taskDlg.SetExpandedInformation(strDownURL);
		taskDlg.SetExpandedControlText(_TEXT("关闭详细信息"));
		taskDlg.SetCollapsedControlText(_TEXT("打开详细信息"));

		taskDlg.DoModal();
		return TRUE;
	}else
		return FALSE;


}

