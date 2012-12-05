/********************************************************************
	文件作者:		King.Sollyu
	
	使用方法:		CUpdate up("要检查升级的XML网络地址")
					CUpdate up(_T("http://sollyu.svfree.net/update.xml"));

					使用 up.CheckUpdate(fNowVersion);来自动检测是否需要
					升级。
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
	// 创建检查更新线程
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

	// 获得最后一个版本的版本号
	CString GetLastVersion();
	CString GetLastTitle();
	CString GetLastTime();
	CString GetLastDownURL();
	CString GetLastText();


	BOOL CheckUpdate(
		__in double fNowVersion ,
		__in BOOL isTip = TRUE);

private:
	// 升级的Xml文件路径
	CString m_strConfPath;
	CString m_strURL;

	MSXML2::IXMLDOMDocumentPtr	pDoc; 
	MSXML2::IXMLDOMNodeListPtr  xmlNodes;


	// 获得软件节点上面的信息。
	CString OnGetSoftInfo(const LPCTSTR lpszInfo ,const LPCTSTR lpszAttr );
	
	// 获得升级节点上面的信息。
	CString GetVersionInfo( __in CString strAttr);


};
#endif // Update_h__
