// CCImage.h : main header file for the CCIMAGE application
//

#if !defined(AFX_CCIMAGE_H__0C202F4D_00F3_49EC_8EF2_EF8F4964E0C0__INCLUDED_)
#define AFX_CCIMAGE_H__0C202F4D_00F3_49EC_8EF2_EF8F4964E0C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCCImageApp:
// See CCImage.cpp for the implementation of this class
//

class CCCImageApp : public CWinApp
{
public:
	CCCImageApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCImageApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCCImageApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCIMAGE_H__0C202F4D_00F3_49EC_8EF2_EF8F4964E0C0__INCLUDED_)
