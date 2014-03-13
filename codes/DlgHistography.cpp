// DlgHistography.cpp : implementation file
//

#include "stdafx.h"
#include "CCImage.h"
#include "DlgHistography.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHistography dialog


CDlgHistography::CDlgHistography(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHistography::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHistography)
	for (int i=0; i<256; i++)
	{
		m_pHisto[i] = 0;
	}
	//}}AFX_DATA_INIT
}


void CDlgHistography::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHistography)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHistography, CDialog)
	//{{AFX_MSG_MAP(CDlgHistography)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHistography message handlers

void CDlgHistography::OnPaint() 
{
	// �豸������
	CPaintDC dc(this);
	
	// ��ȡ��������ľ�̬��
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	// ָ��
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	// �������ʶ���
	CPen* pPenRed = new CPen;
	// ��ɫ����
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	// �������ʶ���
	CPen* pPenBlue = new CPen;
	// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));
	// �������ʶ���
	CPen* pPenGreen = new CPen;
	// ��ɫ����
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	// ����������
	pDC->MoveTo(10,10);
	pDC->LineTo(10,280);
	pDC->LineTo(320,280);
	
	CString str;
	// дX��̶�ֵ
	str.Format("0");
	pDC->TextOut(10, 283, str);
	str.Format("50");
	pDC->TextOut(60, 283, str);
	str.Format("100");
	pDC->TextOut(110, 283, str);
	str.Format("150");
	pDC->TextOut(160, 283, str);
	str.Format("200");
	pDC->TextOut(210, 283, str);
	str.Format("255");
	pDC->TextOut(265, 283, str);

	// ����X��̶�
	for (int i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10�ı���
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 284);
		}
		else
		{
			// 10�ı���
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 282);
		}
	}
	// ����X���ͷ
	pDC->MoveTo(315,275);
	pDC->LineTo(320,280);
	pDC->LineTo(315,285);
	
	// ����Y���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);

    int m_iLowGray=0;
	int m_iUpGray = 255;
	int lMaxCount = 0;
	// ����������ֵ
	for (i = m_iLowGray; i <= m_iUpGray; i ++)
	{
		// �ж��Ƿ���ڵ�ǰ���ֵ
		if (m_pHisto[i] > lMaxCount)
		{
			// �������ֵ
			lMaxCount = m_pHisto[i];
		}
	}
	// ���������ֵ
	pDC->MoveTo(10, 25);
	pDC->LineTo(14, 25);
	str.Format("%d", lMaxCount);
	pDC->TextOut(11, 26, str);
	
	// ���ĳ���ɫ����
	pDC->SelectObject(pPenGreen);	
	
	// ���ƴ���������
	pDC->MoveTo(m_iLowGray + 10, 25);
	pDC->LineTo(m_iLowGray + 10, 280);
	
	pDC->MoveTo(m_iUpGray + 10, 25);
	pDC->LineTo(m_iUpGray + 10, 280);
	
	// ���ĳ���ɫ����
	pDC->SelectObject(pPenBlue);	
	
	// �ж��Ƿ��м���
	if (lMaxCount > 0)
	{
		// ����ֱ��ͼ
		for (i = m_iLowGray; i <= m_iUpGray; i ++)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 281 - (int) (m_pHisto[i] * 256 / lMaxCount));
		}
	}
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
}
