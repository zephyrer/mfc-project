/********************************************************************
	�ļ�����:		King.Sollyu
	
	ʹ�÷���:		CUpdate up("Ҫ���������XML�����ַ")
					CUpdate up(_T("http://sollyu.svfree.net/update.xml"));

					ʹ�� up.CheckUpdate(fNowVersion);���Զ�����Ƿ���Ҫ
					������
#define MySoftVersoin 1.0
#define MY_EXT_ERR_URL			_TEXT("http://hi.baidu.com/kingsollyu/item/34d35be9a653ae2f6cabb860#reply")
#define MY_SOFT_CHECK_UPDATA	_TEXT("http://sollyu.fenghuohack.com/MySoft/LolConver/update.xml")

void Thread_CheckUpdate(void)
{
	CUpdate up(MY_SOFT_CHECK_UPDATA);
	up.CheckUpdate(MySoftVersoin);
}
	
#ifndef _DEBUG
	//------------------------------------------------------------------------------
	// �����������߳�
	//------------------------------------------------------------------------------
	::CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)Thread_CheckUpdate,NULL,NULL,NULL);
#endif
*********************************************************************/

#pragma once

#ifndef Update_h__
#define Update_h__

#include "stdafx.h"

#import "msxml3.dll"
using namespace MSXML2;


class CUpdate
{
public:
	CUpdate( CString strPath );
	virtual ~CUpdate(void);


	CString GetSoftwareName();


	CString GetConfPath() const 		{ return m_strConfPath; }
	void 	SetConfPath(CString val)	{ m_strConfPath = val;  }

	// ������һ���汾�İ汾��
	CString GetLastVersion();
	CString GetLastTitle();
	CString GetLastTime();
	CString GetLastDownURL();
	CString GetLastText();


	BOOL CheckUpdate(
		__in double fNowVersion ,
		__in BOOL isTip = TRUE);

private:
	// ������Xml�ļ�·��
	CString m_strConfPath;
	CString m_strURL;

	MSXML2::IXMLDOMDocumentPtr	pDoc; 
	MSXML2::IXMLDOMNodeListPtr  xmlNodes;


	// �������ڵ��������Ϣ��
	CString OnGetSoftInfo(const LPCTSTR lpszInfo ,const LPCTSTR lpszAttr );
	
	// ��������ڵ��������Ϣ��
	CString GetVersionInfo( __in CString strAttr);


};
#endif // Update_h__
