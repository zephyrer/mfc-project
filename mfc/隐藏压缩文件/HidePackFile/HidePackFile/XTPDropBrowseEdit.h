#pragma once
class CXTPDropBrowseEdit : public CXTPBrowseEdit
{
private:
	BOOL m_isImage;
public:
	CXTPDropBrowseEdit(BOOL isImage);
	~CXTPDropBrowseEdit(void);

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


	DECLARE_MESSAGE_MAP()
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

