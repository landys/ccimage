// DlgSetAngle.cpp : implementation file
//

#include "stdafx.h"
#include "CCImage.h"
#include "DlgSetAngle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetAngle dialog


CDlgSetAngle::CDlgSetAngle(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetAngle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetAngle)
	m_fAngle = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgSetAngle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetAngle)
	DDX_Text(pDX, IDC_PARAM, m_fAngle);
	DDV_MinMaxFloat(pDX, m_fAngle, -360.f, 360.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetAngle, CDialog)
	//{{AFX_MSG_MAP(CDlgSetAngle)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetAngle message handlers
