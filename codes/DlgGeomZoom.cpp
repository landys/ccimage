// DlgGeomZoom.cpp : implementation file
//

#include "stdafx.h"
#include "CCImage.h"
#include "DlgGeomZoom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGeomZoom dialog


CDlgGeomZoom::CDlgGeomZoom(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGeomZoom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGeomZoom)
	m_fValue = 1.0;
	//}}AFX_DATA_INIT
}


void CDlgGeomZoom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGeomZoom)
	DDX_Text(pDX, IDC_CVALUE, m_fValue);
	DDV_MinMaxFloat(pDX, m_fValue, 0.1f, 10.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGeomZoom, CDialog)
	//{{AFX_MSG_MAP(CDlgGeomZoom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGeomZoom message handlers
BOOL CDlgGeomZoom::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

