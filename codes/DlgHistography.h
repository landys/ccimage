#if !defined(AFX_DLGHISTOGRAPHY_H__F28A3FF1_2CAD_4403_9B2E_BB5440F55679__INCLUDED_)
#define AFX_DLGHISTOGRAPHY_H__F28A3FF1_2CAD_4403_9B2E_BB5440F55679__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHistography.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHistography dialog

class CDlgHistography : public CDialog
{
// Construction
public:
	CDlgHistography(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	unsigned int m_pHisto[256];
	
	//{{AFX_DATA(CDlgHistography)
	enum { IDD = IDD_DLG_HISTOGRAPHY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHistography)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHistography)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISTOGRAPHY_H__F28A3FF1_2CAD_4403_9B2E_BB5440F55679__INCLUDED_)
