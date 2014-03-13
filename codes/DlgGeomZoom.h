#if !defined(AFX_DLGGEOMZOOM_H__490BAD68_4162_490A_938A_4787FB5BA5C0__INCLUDED_)
#define AFX_DLGGEOMZOOM_H__490BAD68_4162_490A_938A_4787FB5BA5C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGeomZoom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGeomZoom dialog

class CDlgGeomZoom : public CDialog
{
// Construction
public:
	CDlgGeomZoom(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGeomZoom)
	enum { IDD = IDD_DLG_GEOMETRYZOOM };
	float m_fValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGeomZoom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation	
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGeomZoom)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGEOMZOOM_H__490BAD68_4162_490A_938A_4787FB5BA5C0__INCLUDED_)
