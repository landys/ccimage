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
	// 设备上下文
	CPaintDC dc(this);
	
	// 获取绘制坐标的静态框
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	// 创建画笔对象
	CPen* pPenRed = new CPen;
	// 红色画笔
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	// 创建画笔对象
	CPen* pPenBlue = new CPen;
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));
	// 创建画笔对象
	CPen* pPenGreen = new CPen;
	// 绿色画笔
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));
	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	// 绘制坐标轴
	pDC->MoveTo(10,10);
	pDC->LineTo(10,280);
	pDC->LineTo(320,280);
	
	CString str;
	// 写X轴刻度值
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

	// 绘制X轴刻度
	for (int i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10的倍数
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 284);
		}
		else
		{
			// 10的倍数
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 282);
		}
	}
	// 绘制X轴箭头
	pDC->MoveTo(315,275);
	pDC->LineTo(320,280);
	pDC->LineTo(315,285);
	
	// 绘制Y轴箭头
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);

    int m_iLowGray=0;
	int m_iUpGray = 255;
	int lMaxCount = 0;
	// 计算最大计数值
	for (i = m_iLowGray; i <= m_iUpGray; i ++)
	{
		// 判断是否大于当前最大值
		if (m_pHisto[i] > lMaxCount)
		{
			// 更新最大值
			lMaxCount = m_pHisto[i];
		}
	}
	// 输出最大计数值
	pDC->MoveTo(10, 25);
	pDC->LineTo(14, 25);
	str.Format("%d", lMaxCount);
	pDC->TextOut(11, 26, str);
	
	// 更改成绿色画笔
	pDC->SelectObject(pPenGreen);	
	
	// 绘制窗口上下限
	pDC->MoveTo(m_iLowGray + 10, 25);
	pDC->LineTo(m_iLowGray + 10, 280);
	
	pDC->MoveTo(m_iUpGray + 10, 25);
	pDC->LineTo(m_iUpGray + 10, 280);
	
	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);	
	
	// 判断是否有计数
	if (lMaxCount > 0)
	{
		// 绘制直方图
		for (i = m_iLowGray; i <= m_iUpGray; i ++)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 281 - (int) (m_pHisto[i] * 256 / lMaxCount));
		}
	}
	
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
}
