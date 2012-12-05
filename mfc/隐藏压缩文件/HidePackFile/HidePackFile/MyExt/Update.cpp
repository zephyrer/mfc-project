#include "StdAfx.h"
#include "Update.h"


CUpdate::CUpdate( CString strPath )
:m_strURL(strPath)
{

	::CoInitialize(NULL);

	HRESULT HR = pDoc.CreateInstance(_uuidof(MSXML2::DOMDocument30));
	if(!SUCCEEDED(HR))
	{ 
		CMyExt::ShowError(_TEXT("���³����ڳ�ʼ��ʱʧ�ܣ�"),GetLastError());
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

	// ����Ҫ��λ���ӽڵ�����
	LPCTSTR lpszSubKeyName = lpszInfo;

	// ���巵�س����ı���
	LPCTSTR lpszResult     = _TEXT("");

	TRY 
	{
		// �����ļ�
		pDoc->load(GetConfPath().GetString());

		// ֱ������ָ���ڵ�
		xmlNodes = pDoc->getElementsByTagName(lpszSubKeyName);

		// �����λʧ�ܡ�
		if (xmlNodes->Getlength() == 0)
		{ 
// 			CMyExt::ShowError(_TEXT("xmlNodes->Getlength() == 0"));
			return _TEXT("");
		}

		// �鿴ÿ���ڵ���������ơ�
		for (int i = 0; i < xmlNodes->Getitem(0)->Getattributes()->Getlength() ; i++)
		{
			// ��ýڵ����ơ�
			CString strNodeName = xmlNodes->Getitem(0)->Getattributes()->Getitem(i)->nodeName;
			// �������ƥ��ͻ�ö�Ӧ��ֵ��
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
		xmlNodes = xmlNode->GetchildNodes();//�ӽڵ��б�


		// �����λʧ�ܡ�
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
				// �������ƥ��ͻ�ö�Ӧ��ֵ��
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

		strDownURL.Format(_TEXT("����ʱ�䣺%s \r\n���ص�ַ��<a href=\"%s\">�������</a>"),GetLastTime(),GetLastDownURL());

		taskDlg.SetExpandedInformation(strDownURL);
		taskDlg.SetExpandedControlText(_TEXT("�ر���ϸ��Ϣ"));
		taskDlg.SetCollapsedControlText(_TEXT("����ϸ��Ϣ"));

		taskDlg.DoModal();
		return TRUE;
	}else
		return FALSE;


}

