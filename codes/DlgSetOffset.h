#if !defined(AFX_DLGSETOFFSET_H__0220AC93_E2DD_4CBA_80FD_E83E86E201C1__INCLUDED_)
#define AFX_DLGSETOFFSET_H__0220AC93_E2DD_4CBA_80FD_E83E86E201C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetOffset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetOffset dialog

class CDlgSetOffset : public CDialog
{
// Construction
public:
	CDlgSetOffset(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetOffset)
	enum { IDD = IDD_DLG_TRANSLATION };
	LONG	m_xOffset;
	LONG	m_yOffset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetOffset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetOffset)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETOFFSET_H__0220AC93_E2DD_4CBA_80FD_E83E86E201C1__INCLUDED_)
