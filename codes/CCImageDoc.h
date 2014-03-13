// CCImageDoc.h : interface of the CCCImageDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCIMAGEDOC_H__835FD124_EAFD_4E05_9BB7_EBC3399A723E__INCLUDED_)
#define AFX_CCIMAGEDOC_H__835FD124_EAFD_4E05_9BB7_EBC3399A723E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define pi 3.141592654
#include "dib.h"

class CCCImageDoc : public CDocument
{
protected: // create from serialization only
	CCCImageDoc();
	DECLARE_DYNCREATE(CCCImageDoc)

// Attributes
public:
	CDIB m_clsDIB;
	CString m_strCurrentFile;
	HDIB m_hDIB;
// Operations
public:
	void InitDIBData();
	void ReplaceHDIB(HDIB hDIB);
	void ReopenFile();
	HDIB GetHDIB() const
	{ 
		return m_hDIB; 
	}
	CPalette* GetDocPalette() const
	{ 
		return m_palDIB; 
	}
	CSize GetDocSize() const
	{ 
		return m_sizeDoc; 
	}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCImageDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCCImageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPalette* m_palDIB;
	CSize m_sizeDoc;
// Generated message map functions
protected:
	//{{AFX_MSG(CCCImageDoc)
	afx_msg void OnFileReopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCIMAGEDOC_H__835FD124_EAFD_4E05_9BB7_EBC3399A723E__INCLUDED_)
