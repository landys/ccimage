// DlgSetOffset.cpp : implementation file
//

#include "stdafx.h"
#include "CCImage.h"
#include "DlgSetOffset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetOffset dialog


CDlgSetOffset::CDlgSetOffset(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetOffset::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetOffset)
	m_xOffset = 0l;
	m_yOffset = 0l;
	//}}AFX_DATA_INIT
}


void CDlgSetOffset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetOffset)
	DDX_Text(pDX, IDC_XOFFSET, m_xOffset);
	DDX_Text(pDX, IDC_YOFFSET, m_yOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetOffset, CDialog)
	//{{AFX_MSG_MAP(CDlgSetOffset)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetOffset message handlers
