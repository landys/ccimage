#if !defined(AFX_DLGSETANGLE_H__53F6E64D_3EC6_4106_A6F5_0FC5AFE09EC6__INCLUDED_)
#define AFX_DLGSETANGLE_H__53F6E64D_3EC6_4106_A6F5_0FC5AFE09EC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetAngle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetAngle dialog

class CDlgSetAngle : public CDialog
{
// Construction
public:
	CDlgSetAngle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetAngle)
	enum { IDD = IDD_DLG_SETANGLE };
	float	m_fAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetAngle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetAngle)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETANGLE_H__53F6E64D_3EC6_4106_A6F5_0FC5AFE09EC6__INCLUDED_)
