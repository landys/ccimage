// CCImageView.cpp : implementation of the CCCImageView class
//

#include "stdafx.h"
#include "CCImage.h"

#include "CCImageDoc.h"
#include "CCImageView.h"
#include "MainFrm.h"
#include "DlgHistography.h"
#include "DlgGeomZoom.h"
#include "DlgSetAngle.h"
#include "DlgSetOffset.h"
#include "FFT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCCImageView

IMPLEMENT_DYNCREATE(CCCImageView, CScrollView)

BEGIN_MESSAGE_MAP(CCCImageView, CScrollView)
	//{{AFX_MSG_MAP(CCCImageView)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_FILTERS_MEDIAN, OnFiltersMedian)
	ON_COMMAND(ID_FILTERS_SMOOTH, OnFiltersSmooth)
	ON_COMMAND(ID_FILTERS_SHARP, OnFiltersSharp)
	ON_COMMAND(ID_GEOMETRY_HORIMIRROR, OnGeometryHorimirror)
	ON_COMMAND(ID_GEOMETRY_ROTATE, OnGeometryRotate)
	ON_COMMAND(ID_GEOMETRY_TRANSLATE, OnGeometryTranslate)
	ON_COMMAND(ID_GEOMETRY_VERTMIRROR, OnGeometryVertmirror)
	ON_COMMAND(ID_ORTHO_FFT, OnOrthoFft)
	ON_COMMAND(ID_GEOMETRY_ENLARGE, OnGeometryEnlarge)
	ON_COMMAND(ID_GEOMETRY_LINEARENLARGE, OnGeometryLinearenlarge)
	ON_COMMAND(ID_GEOMETRY_LINEARROTATE, OnGeometryLinearrotate)
	ON_COMMAND(ID_HISTOEQU, OnHistoequ)
	ON_COMMAND(ID_HISTOGRAPHY, OnHistography)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCCImageView construction/destruction

CCCImageView::CCCImageView()
{
}

CCCImageView::~CCCImageView()
{
}

BOOL CCCImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	// make the background of view gray
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW |CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_GRAYTEXT));

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCCImageView drawing

void CCCImageView::OnDraw(CDC* pDC)
{
	// 显示等待光标
	BeginWaitCursor();
	
	// 获取文档
	CCCImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// 获取DIB
	HDIB hDIB = pDoc->GetHDIB();
	
	// 判断DIB是否为空
	if (hDIB != NULL)
	{
		LPBYTE lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);
		
		// 获取DIB宽度
		int cxDIB = (int) pDoc->m_clsDIB.DIBWidth(lpDIB);
		
		// 获取DIB高度
		int cyDIB = (int) pDoc->m_clsDIB.DIBHeight(lpDIB);
		
		::GlobalUnlock((HGLOBAL) hDIB);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		
		CRect rcDest;
		
		// 判断是否是打印
		if (pDC->IsPrinting())
		{
			// 是打印，计算输出图像的位置和大小，以便符合页面
			
			// 获取打印页面的水平宽度(象素)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			
			// 获取打印页面的垂直高度(象素)
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			
			// 获取打印机每英寸象素数
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			
			// 计算打印图像大小（缩放，根据页面宽度调整图像大小）
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch) / ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
			
			// 计算打印图像位置（垂直居中）
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp/2;
			rcDest.top += temp/2;
		}
		else // 非打印
		{
			// 不必缩放图像
			rcDest = rcDIB;
		}
		// 输出DIB
		pDoc->m_clsDIB.PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());

		// 重新设置滚动视图大小
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	}
	// 恢复正常光标
	EndWaitCursor();
}

void CCCImageView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CCCImageView printing

BOOL CCCImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 设置总页数为一。
	pInfo->SetMaxPage(1);
	return DoPreparePrinting(pInfo);
}

void CCCImageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCCImageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

///////////////////////////////////////////
//	计算窗口视图大小，以计算滚动条设置   //	
///////////////////////////////////////////

void CCCImageView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	CScrollView::CalcWindowRect(lpClientRect, nAdjustType);
	ASSERT(GetDocument() != NULL);
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
}

void CCCImageView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	if (bActivate)
	{
		ASSERT(pActivateView == this);
		OnDoRealize((WPARAM)m_hWnd, 0);   // same as SendMessage(WM_DOREALIZE);
	}
}

///////////////////////////////////////////
//	设置子窗体默认背景色为m_refColorBKG  //
///////////////////////////////////////////

BOOL CCCImageView::OnEraseBkgnd(CDC* pDC) 
{
	// 获取文档
	CCCImageDoc* pDoc = GetDocument();
	
	// 创建一个Brush
	CBrush brush(0x808080);                                              
	
	// 保存以前的Brush
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	
	// 获取重绘区域
	CRect rectClip;
	pDC->GetClipBox(&rectClip);
	
	// 重绘
	pDC->PatBlt(rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(), PATCOPY);
	
	// 恢复以前的Brush
	pDC->SelectObject(pOldBrush);                                                  
	
	// 返回
	return TRUE;
}


///////////////////////////////////////////
//	实现新的调色板						 //
///////////////////////////////////////////

void CCCImageView::OnDoRealize(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);
	
	// 获取文档
	CCCImageDoc* pDoc = GetDocument();
	
	// 判断DIB是否为空
	if (pDoc->GetHDIB() == NULL)
	{
		// 直接返回
		return;
	}
	
	// 获取Palette
	CPalette* pPal = pDoc->GetDocPalette();
	if (pPal != NULL)
	{
		// 获取MainFrame
		CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		ASSERT_KINDOF(CMainFrame, pAppFrame);
		
		CClientDC appDC(pAppFrame);
		
		// All views but one should be a background palette.
		// wParam contains a handle to the active view, so the SelectPalette
		// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
		CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);
		
		if (oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
				pDoc->UpdateAllViews(NULL);
			appDC.SelectPalette(oldPalette, TRUE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCCImageView diagnostics

#ifdef _DEBUG
void CCCImageView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCCImageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCCImageDoc* CCCImageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCCImageDoc)));
	return (CCCImageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCCImageView message handlers

void CCCImageView::OnEditCopy() 
{
	// 获取文档
	CCCImageDoc* pDoc = GetDocument();
	
	// 打开剪贴板
	if (OpenClipboard())
	{
		// 更改光标形状
		BeginWaitCursor();
		
		// 清空剪贴板
		EmptyClipboard();
		
		// 复制当前图像到剪贴板
		SetClipboardData (CF_DIB, pDoc->m_clsDIB.CopyHandle((HANDLE) pDoc->GetHDIB()) );
		
		// 关闭剪贴板
		CloseClipboard();
		
		// 恢复光标
		EndWaitCursor();
	}
	
}

void CCCImageView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// 如果当前DIB对象不空，复制菜单项有效
	pCmdUI->Enable(GetDocument()->GetHDIB() != NULL);
}

void CCCImageView::OnEditPaste() 
{
	// 创建新DIB
	HDIB hNewDIB = NULL;
	
	// 获取文档
	CCCImageDoc* pDoc = GetDocument();
	
	// 打开剪贴板
	if (OpenClipboard())
	{
		// 更改光标形状
		BeginWaitCursor();
		
		// 读取剪贴板中的图像
		hNewDIB = (HDIB) pDoc->m_clsDIB.CopyHandle(::GetClipboardData(CF_DIB));
		
		// 关闭剪贴板
		CloseClipboard();
		
		// 判断是否读取成功
		if (hNewDIB != NULL)
		{
			// 替换DIB，同时释放旧DIB对象
			pDoc->ReplaceHDIB(hNewDIB);
			
			// 更新DIB大小和调色板
			pDoc->InitDIBData();
			
			// 设置脏标记
			pDoc->SetModifiedFlag(TRUE);
			
			// 重新设置滚动视图大小
			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
			
			// 实现新的调色板
			OnDoRealize((WPARAM)m_hWnd,0);
			
			// 更新视图
			pDoc->UpdateAllViews(NULL);
		}
		
		// 恢复光标
		EndWaitCursor();
	}
}

void CCCImageView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// 如果当前剪贴板中有DIB对象，粘贴菜单项有效
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
}


// 图像零阶插值放大
void CCCImageView::OnGeometryEnlarge() 
{
	// TODO: Add your command handler code here
	CCCImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	HDIB hDIB = pDoc->m_hDIB;
	HDIB hNewDIB;
	
	if(hDIB == NULL)
		return;
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//获得图像信息
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//数据指针指向位数据
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	//计算图像大小　
	int mwidth = 2*nwidth;
	int mheigth = 2*nheigth;
	int mwidthbytes = ((mwidth+3)/4)*4;
	
	int mBitmapSize = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)+mheigth*mwidthbytes;
	//申请新图像的内存，并置头信息参数
	hNewDIB =(HDIB ) GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,mBitmapSize);
	if(!hNewDIB)
	{
		MessageBox("内存申请不成功",NULL,MB_OK);
		return;
	}
	
	LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
	LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
	
	lpNewbmi->biWidth = 2*lpbmi->biWidth;
	lpNewbmi->biHeight=2*lpbmi->biHeight;
	
	//后向插值
	
	for(int x = 0; x<lpNewbmi->biWidth; x++)
	{
		for(int y =0; y< lpNewbmi->biHeight; y++)
		{
			*(lpNewbits+y*mwidthbytes+x) = *(lpbits+(y/2)*nwidthbytes+(x/2));
		}
	}	
	// 释放原图像，将当前位图句柄指向放大后的图像
		
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
	GlobalUnlock(hNewDIB);
	
	// 替换DIB，同时释放旧DIB对象
	pDoc->ReplaceHDIB(hNewDIB);
	
	// 更新DIB大小和调色板
	pDoc->InitDIBData();
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 重新设置滚动视图大小
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
}

// 图像线性插值放大
void CCCImageView::OnGeometryLinearenlarge() 
{
	// 创建对话框
	CDlgGeomZoom dlg;
	
	// 显示对话框，由用户进行尺度比例常数设定
	if (dlg.DoModal() == IDOK)
	{
		// 更新数据
		UpdateData(TRUE);

		HDIB hNewDIB;
		CCCImageDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		HDIB hDIB = pDoc->m_hDIB;	
		if(hDIB == NULL)
			return;
		float fscale = dlg.m_fValue;
		
		LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
		//获得图像信息
		LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
		int nwidth = lpbmi->biWidth;
		int nheigth = lpbmi->biHeight;
		int nwidthbytes = ((nwidth+3)/4)*4;
		//数据指针指向位数据
		LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
		
		//计算图像大小　
		int mwidth = (int)(fscale*nwidth);
		int mheigth =(int)(fscale*nheigth);
		int mwidthbytes = ((mwidth+3)/4)*4;
		
		int mBitmapSize = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)+mheigth*mwidthbytes;
		//   //申请新图像的内存，并置头信息参数
		hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,mBitmapSize);
		if(!hNewDIB)
		{
			MessageBox("内存申请不成功",NULL,MB_OK);
			return;
		}
		
		LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
		LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
		
		LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
		memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
		
		
		
		lpNewbmi->biWidth = mwidth;
		lpNewbmi->biHeight = mheigth;
		
		//后向双线性插值
		
		for(int x = 0; x<lpNewbmi->biWidth; x++)
		{
			for(int y =0; y< lpNewbmi->biHeight; y++)
			{
				float x1 = x/fscale;
				float y1= y/fscale;
				unsigned char a1 = *(lpbits+(int)y1*nwidthbytes+(int)x1);
				unsigned char a2 = *(lpbits+(int)y1*nwidthbytes+(int)(x1+0.5));
				unsigned char a3 = *(lpbits+(int)(y1+0.5)*nwidthbytes+(int)x1);
				unsigned char a4 = *(lpbits+(int)(y1+0.5)*nwidthbytes+(int)(x1+0.5));
				float fx0 = a1 + (x1-(int)x1)*(a2-a1);
				float fx1 = a3 + (x1-(int)x1)*(a4-a3);
				float fy = fx0+(y1-(int)y1)*(fx1-fx0);
					
				*(lpNewbits+y*mwidthbytes+x) = (unsigned char)fy;
			}
		}	
		// 释放原图像，将当前位图句柄指向放大后的图像
		
		GlobalUnlock(hDIB);
		GlobalFree(hDIB);
		GlobalUnlock(hNewDIB);
		
		// 替换DIB，同时释放旧DIB对象
		pDoc->ReplaceHDIB(hNewDIB);
		
		// 更新DIB大小和调色板
		pDoc->InitDIBData();
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 重新设置滚动视图大小
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
}

// 图像平移
void CCCImageView::OnGeometryTranslate() 
{
	// 获取文档
	CCCImageDoc* pDoc = GetDocument();
	// 由对话框输入旋转角度
	CDlgSetOffset dlg;
	if(dlg.DoModal() == IDOK)
	{
		// 更新数据
		UpdateData(TRUE);

		int lxOffset = dlg.m_xOffset;
		int lyOffset = dlg.m_yOffset;
		HDIB hDIB = pDoc->m_hDIB;
		HDIB hNewDIB;
		
		LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
		//获得图像信息
		LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
		int nwidth = lpbmi->biWidth;
		int nheigth = lpbmi->biHeight;
		int nwidthbytes = ((nwidth+3)/4)*4;
		//数据指针指向位数据
		LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
		
		//计算图像大小　
		int mwidth = lxOffset+nwidth;
		int mheigth = lyOffset+nheigth;
		int mwidthbytes = ((mwidth+3)/4)*4;
		
		int mBitmapSize = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)+mheigth*mwidthbytes;
		//申请新图像的内存，并置头信息参数
		hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,mBitmapSize);
		if(!hNewDIB)
		{
			MessageBox("内存申请不成功",NULL,MB_OK);
			return;
		}
		
		LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
		LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
		
		LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
		memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
		
		lpNewbmi->biWidth = mwidth;
		lpNewbmi->biHeight = mheigth;
		
		// 平移后, 在扩大的部分设为白色
		for(int x = 0; x<mwidth; x++)
		{
			for(int y =0; y<mheigth; y++)
			{
				int x1 = x-lxOffset;
				int y1= y;
				if((x1>=0)&&(x1<nwidth)&&(y1>=0)&&(y1<nheigth)) 
				{
					*(lpNewbits+(y)*mwidthbytes+x) =*(lpbits+(y1)*nwidthbytes+x1);}
				else 
				{
					*(lpNewbits+y*mwidthbytes+x)=char(255);
				}
			}
		}
		
		// 释放原图像，将当前位图句柄指向放大后的图像
		
		GlobalUnlock(hDIB);
		GlobalFree(hDIB);
		GlobalUnlock(hNewDIB);

		// 替换DIB，同时释放旧DIB对象
		pDoc->ReplaceHDIB(hNewDIB);
		
		// 更新DIB大小和调色板
		pDoc->InitDIBData();
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 重新设置滚动视图大小
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
}

// 图像零阶插值旋转
void CCCImageView::OnGeometryRotate() 
{
	// 获取文档
	CCCImageDoc* pDoc = GetDocument();
	
	// 由对话框输入旋转角度
	CDlgSetAngle dlg;
	if(dlg.DoModal() == IDOK)
	{
		// 更新数据
		UpdateData(TRUE);

		int nAngle = int(dlg.m_fAngle);
		HDIB hNewDIB;
		HDIB hDIB = pDoc->m_hDIB;

		if(hDIB == NULL)
			return;
		LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
		//获得图像信息
		LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
		int nwidth = lpbmi->biWidth;
		int nheigth = lpbmi->biHeight;
		int nwidthbytes = ((nwidth+3)/4)*4;
			//数据指针指向位数据
		LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);

		double fcos = cos(nAngle*pi/180.0);
		double fsin = sin(nAngle*pi/180.0);

		//以图像中心为坐标原点，计算四个顶点的坐标，以及旋转后的坐标
		double fx1 = -(nwidth-1)/2.0;
		double fx2 = (nwidth-1)/2.0;
		double fy1 = -(nheigth-1)/2.0;
		double fy2 = (nheigth-1)/2.0;

		double fDstX1 =	fcos * fx1 + fsin * fy1;
		double fDstY1 = -fsin * fx1 + fcos * fy1;

		double fDstX2 =  fcos * fx2 + fsin * fy1;
		double fDstY2 = -fsin * fx2 + fcos * fy1;

		double fDstX3 =  fcos * fx1 + fsin * fy2;
		double fDstY3 = -fsin * fx1 + fcos * fy2;

		double fDstX4 =  fcos * fx2 + fsin * fy2;
		double fDstY4 = -fsin * fx2 + fcos * fy2;
      
	   //计算图像大小　
		int mwidth = (int)(max(fabs(fDstX4-fDstX1), fabs(fDstX3-fDstX2))+0.5);
		int mheigth = (int)(max(fabs(fDstY4-fDstY1), fabs(fDstY3-fDstY2))+0.5);

		int mwidthbytes = ((mwidth+3)/4)*4;

		int mBitmapSize = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)+mheigth*mwidthbytes;
		//   //申请新图像的内存，并置头信息参数
		hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,mBitmapSize);
		if(!hNewDIB)
		{
			MessageBox("内存申请不成功",NULL,MB_OK);
			return;
		}

		LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
		LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);

		LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
		memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));

		lpNewbmi->biWidth = mwidth;
		lpNewbmi->biHeight= mheigth;

		//后向插值

		for(int x = 0; x<lpNewbmi->biWidth; x++)
		{
			for(int y =0; y< lpNewbmi->biHeight; y++)
			{
				double x1 = (x-(mwidth-1)/2)*fcos-(y-(mheigth-1)/2)*fsin+(nwidth-1)/2;
				double y1= (x-(mwidth-1)/2)*fsin+(y-(mheigth-1)/2)*fcos+(nheigth-1)/2;
				if(x1<0 || y1<0 || x1> nwidth-1 || y1> nheigth-1)
					*(lpNewbits+y*mwidthbytes+x) = char(255);
				else{
				
					*(lpNewbits+y*mwidthbytes+x) = *(lpbits+(int)y1*nwidthbytes+(int)x1);
				}
			}
		}

		// 释放原图像，将当前位图句柄指向旋转后的图像
		GlobalUnlock(hDIB);
	    GlobalFree(hDIB);
		GlobalUnlock(hNewDIB);

		// 替换DIB，同时释放旧DIB对象
		pDoc->ReplaceHDIB(hNewDIB);
		
		// 更新DIB大小和调色板
		pDoc->InitDIBData();
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 重新设置滚动视图大小
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
}

// 图像线性插值旋转
void CCCImageView::OnGeometryLinearrotate() 
{
		// 获取文档
	CCCImageDoc* pDoc = GetDocument();
	
	// 由对话框输入旋转角度
	CDlgSetAngle dlg;
	if(dlg.DoModal() == IDOK)
	{
		// 更新数据
		UpdateData(TRUE);

		int nAngle = int(dlg.m_fAngle);
		HDIB hNewDIB;
		HDIB hDIB = pDoc->m_hDIB;
		if(hDIB == NULL)
			return;
		LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
		//获得图像信息
		LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
		int nwidth = lpbmi->biWidth;
		int nheigth = lpbmi->biHeight;
		int nwidthbytes = ((nwidth+3)/4)*4;
		//数据指针指向位数据
		LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);

		//计算两个常数　cos(angle) sin(angle)
		double fcos = cos(nAngle*pi/180.0);
		double fsin = sin(nAngle*pi/180.0);

		//以图像中心为坐标原点，计算四个顶点的坐标，以及旋转后的坐标
		double fx1 = -(nwidth-1)/2.0;
		double fx2 = (nwidth-1)/2.0;
		double fy1 = -(nheigth-1)/2.0;
		double fy2 = (nheigth-1)/2.0;

		double fDstX1 =	fcos * fx1 + fsin * fy1;
		double fDstY1 = -fsin * fx1 + fcos * fy1;

		double fDstX2 =  fcos * fx2 + fsin * fy1;
		double fDstY2 = -fsin * fx2 + fcos * fy1;

		double fDstX3 =  fcos * fx1 + fsin * fy2;
		double fDstY3 = -fsin * fx1 + fcos * fy2;

		double fDstX4 =  fcos * fx2 + fsin * fy2;
		double fDstY4 = -fsin * fx2 + fcos * fy2;
      
	   //计算图像大小　
		int mwidth = (int)(max(fabs(fDstX4-fDstX1), fabs(fDstX3-fDstX2))+0.5);
		int mheigth = (int)(max(fabs(fDstY4-fDstY1), fabs(fDstY3-fDstY2))+0.5);

		int mwidthbytes = ((mwidth+3)/4)*4;

		int mBitmapSize = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)+mheigth*mwidthbytes;
		//申请新图像的内存，并置头信息参数
		hNewDIB =(HDIB) GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,mBitmapSize);
		if(!hNewDIB)
		{
			MessageBox("内存申请不成功",NULL,MB_OK);
			return;
		}

		LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
		LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);

		LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
		memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));

		lpNewbmi->biWidth = mwidth;
		lpNewbmi->biHeight= mheigth;

		//后向插值
		for(int x = 0; x<lpNewbmi->biWidth; x++)
		{
			for(int y =0; y< lpNewbmi->biHeight; y++)
			{
				double x1 = (x-(mwidth-1)/2)*fcos-(y-(mheigth-1)/2)*fsin+(nwidth-1)/2;
				double y1= (x-(mwidth-1)/2)*fsin+(y-(mheigth-1)/2)*fcos+(nheigth-1)/2;
				if(x1<0 || y1<0 || x1> nwidth-1 || y1> nheigth-1)
					*(lpNewbits+y*mwidthbytes+x) = char(255);
				else
				{
					unsigned char a1 = *(lpbits+(int)y1*nwidthbytes+(int)x1);
					unsigned char a2 = *(lpbits+(int)y1*nwidthbytes+(int)(x1+0.5));
					unsigned char a3 = *(lpbits+(int)(y1+0.5)*nwidthbytes+(int)x1);
					unsigned char a4 = *(lpbits+(int)(y1+0.5)*nwidthbytes+(int)(x1+0.5));
					double fx0 = a1 + (x1-(int)x1)*(a2-a1);
					double fx1 = a3 + (x1-(int)x1)*(a4-a3);
					double fy = fx0+(y1-(int)y1)*(fx1-fx0);
					*(lpNewbits+y*mwidthbytes+x) = (unsigned char)fy;
				}
			}
		}
		// 释放原图像，将当前位图句柄指向旋转后的图像
		GlobalUnlock(hDIB);
	    GlobalFree(hDIB);
		GlobalUnlock(hNewDIB);

		// 替换DIB，同时释放旧DIB对象
		pDoc->ReplaceHDIB(hNewDIB);
		
		// 更新DIB大小和调色板
		pDoc->InitDIBData();
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 重新设置滚动视图大小
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
}

// 图像水平镜像
void CCCImageView::OnGeometryHorimirror() 
{
	// 获取文档
	CCCImageDoc* pDoc = GetDocument();
	
	// 循环变量
	int y;
	int x;
	
	HDIB hDIB = pDoc->m_hDIB;
	if(hDIB == NULL)
		return;
	
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//获得图像信息
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheight = lpbmi->biHeight;
	int mwidthbytes = ((nwidth+3)/4)*4;
	//数据指针指向位数据
	LPBYTE lpDIBBits = (unsigned char* )pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	// 不能用char，也不能用::strcpy()
	unsigned char* ptemp = new unsigned char[mwidthbytes * nheight];
	
	//垂直中轴坐标
	int lMiddleX = nwidth / 2;
	//针对图像每行进行操作
	for(y=0; y<nheight; y++)
	{
		//指向图像的倒数第y行
		unsigned char* ptempOld = lpDIBBits;
		ptempOld +=  y * mwidthbytes;
		unsigned char* ptempNew = ptemp; 
		ptempNew +=  y * mwidthbytes;
		//针对每行图像左半部分进行操作
		for(x=0; x<=lMiddleX; x++)
		{
			//将源图像第x个像素复制到新图像倒数第x个像素
			*(ptempNew + (nwidth -1 - x)) = *(ptempOld+x);
			//将源图像倒数第x个像素复制到新图像第x个像素
			*(ptempNew + x) = *(ptempOld + (nwidth -1 - x));
		}
	}
	
	// 回存处理结果到DIB
	for(x = 0; x < mwidthbytes * nheight; x++)
		*(lpDIBBits + x) = ptemp[x];
	
	// 解除锁定
	::GlobalUnlock(hDIB);
	
	// 释放内存
	delete[] ptemp;
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
}

// 图像垂直镜像
void CCCImageView::OnGeometryVertmirror() 
{
	// 获取文档
	CCCImageDoc* pDoc = GetDocument();
	
	// 循环变量
	int y;
	int x;
	
	HDIB hDIB = pDoc->m_hDIB;
	if(hDIB == NULL)
		return;
	
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//获得图像信息
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheight = lpbmi->biHeight;
	int mwidthbytes = ((nwidth+3)/4)*4;
	//数据指针指向位数据
	LPBYTE lpDIBBits = (unsigned char* )pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	// 不能用char，也不能用::strcpy()
	unsigned char* ptemp = new unsigned char[mwidthbytes * nheight];
	
	//水平中轴坐标
	int lMiddleY = nheight / 2;
	//针对上半图像进行操作
	for(y=0; y <= lMiddleY; y++)
	{
		//指向源图像倒数第y行像素起点的指针
		unsigned char* ptemp1 = lpDIBBits;
		ptemp1 +=  y * mwidthbytes;
		//指向新图像第y行像素起点的指针
		unsigned char* ptemp2 = ptemp;
		ptemp2 +=  (nheight - 1 - y) * mwidthbytes;
		//将源图像倒数第y行像素复制到新图像第y行
		memcpy(ptemp2,  ptemp1,  mwidthbytes);
		//指向源图像第y行像素起点的指针
		ptemp1 = lpDIBBits;
		ptemp1 +=  (nheight -1 -y) * mwidthbytes;
		//指向新图像倒数第y行像素起点的指针
		ptemp2 = ptemp;
		ptemp2 +=  y * mwidthbytes;
		//将源图像第y行像素复制到新图像倒数第y行
		memcpy(ptemp2,  ptemp1,  mwidthbytes);
	}
	
	// 回存处理结果到DIB
	for(x = 0; x < mwidthbytes * nheight; x++)
		*(lpDIBBits + x) = ptemp[x];
	
	// 解除锁定
	::GlobalUnlock(hDIB);
	
	// 释放内存
	delete[] ptemp;
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
}

// 显示直方图
void CCCImageView::OnHistography() 
{
	CCCImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HDIB hDIB = pDoc->m_hDIB;
	
	if(hDIB == NULL)
		return;
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//获得图像信息
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//数据指针指向位数据
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	CDlgHistography dlg;
	//统计直方图
	for(int y = 0 ;  y < nheigth; y++)
	{
		for( int x = 0; x < nwidth; x++)
		{
			unsigned char gray = (unsigned char)*(lpbits+y*nwidthbytes+x);
			dlg.m_pHisto[gray]++;
		}
	}
	// 显示面板
	dlg.DoModal();
}

// 直方图均衡化
void CCCImageView::OnHistoequ() 
{
	CCCImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HDIB hDIB = pDoc->m_hDIB;
	
	if(hDIB == NULL)
		return;
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//获得图像信息
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//数据指针指向位数据
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	CDlgHistography dlg;
	// 统计直方图
	int x, y;
	for(y=0 ; y<nheigth; y++)
	{
		for(x=0; x<nwidth; x++)
		{
			unsigned char gray = (unsigned char)*(lpbits+y*nwidthbytes+x);
			dlg.m_pHisto[gray]++;	
		}
	}

	int i = 0;
	for(i=0; i<256; i++)
		dlg.m_pHisto[i]+=dlg.m_pHisto[i-1];
	
	for(i=0;i<256;i++)
		dlg.m_pHisto[i]=(DWORD)(dlg.m_pHisto[i]*255.0/((double) nwidth*(double)nheigth)+0.5);
	
	// 均衡化
	for(y = 0 ;  y < nheigth; y++)
	{
		for(x = 0; x < nwidth; x++)
		{
			unsigned int gray = (unsigned char)*(lpbits+y*nwidthbytes+x);
			if(gray>3)
			{	
				*(lpbits+y*nwidthbytes+x)=(BYTE)dlg.m_pHisto[gray];
			}
		}
	}
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 显示直方图对话框
	dlg.DoModal();
}

// FFT
void CCCImageView::OnOrthoFft() 
{
	CCCImageDoc* pDoc1 = GetDocument();
	ASSERT_VALID(pDoc1);
	HDIB hDIB = pDoc1->m_hDIB;

	HDIB hFFTAbs,hFFTPhs;

	// 显示等待光标
	BeginWaitCursor();

	BOOL iRet = ::ImageFFT(hDIB, &hFFTAbs, &hFFTPhs);

	if(iRet == FALSE)
		MessageBox("FFT出错!");

	CDocTemplate *pDocTem=GetDocument()->GetDocTemplate();

	CCCImageDoc *pDoc=(CCCImageDoc*) pDocTem->OpenDocumentFile(NULL);;
	pDoc->m_hDIB = hFFTPhs;
	// 更新DIB大小和调色板
	pDoc->InitDIBData();
	// 重新设置滚动视图大小
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	pDoc->SetModifiedFlag(TRUE);
	POSITION pos = pDoc->GetFirstViewPosition(); 
    CCCImageView* pView = (CCCImageView*)pDoc->GetNextView(pos);
	// 更新视图
	pDoc->UpdateAllViews(NULL);

	pDocTem=GetDocument()->GetDocTemplate();

	pDoc=(CCCImageDoc*) pDocTem->OpenDocumentFile(NULL);;
	pDoc->m_hDIB = hFFTAbs;
	// 更新DIB大小和调色板
	pDoc->InitDIBData();
	// 重新设置滚动视图大小
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	pDoc->SetModifiedFlag(TRUE);
	pos = pDoc->GetFirstViewPosition(); 
    pView = (CCCImageView*)pDoc->GetNextView(pos);
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	EndWaitCursor();
}

// 低通平滑滤波
//average 平滑变换
//算子为:       
//			1  |  1   1   1  |
//         --- |  1   1   1  |
//          9  |  1   1   1  |
//
void CCCImageView::OnFiltersSmooth() 
{
	CCCImageDoc* pDoc1 = GetDocument();
	ASSERT_VALID(pDoc1);
	HDIB hDIB = pDoc1->m_hDIB;
	
	if(hDIB == NULL)
		return;
   	
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//获得图像信息
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//数据指针指向位数据
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	HDIB hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)+nwidthbytes*nheigth);
	if(!hNewDIB)
	{
		MessageBox("内存申请不成功",NULL,MB_OK);
		return;
	}
	
	LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
	LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
	
	for(int y = 1; y < nheigth-1; y ++)
	{
		for(int x =1; x < nwidth-1; x++)
		{
			unsigned int temp = (unsigned char)*(lpbits+(y-1)*nwidthbytes+(x-1)) //f(x-1,y-1)
				+(unsigned char)*(lpbits+(y-1)*nwidthbytes+(x))   //f(x,y-1)
				+(unsigned char)*(lpbits+(y-1)*nwidthbytes+(x+1))   //f(x+1,y-1)
				+(unsigned char)*(lpbits+(y  )*nwidthbytes+(x-1))   //f(x-1,y)
				+(unsigned char)*(lpbits+(y)*nwidthbytes+(x))   //f(x,y)
				+(unsigned char)*(lpbits+(y)*nwidthbytes+(x+1))   //f(x+1,y)
				+(unsigned char)*(lpbits+(y+1)*nwidthbytes+(x-1))   //f(x-1,y+1)
				+(unsigned char)*(lpbits+(y+1)*nwidthbytes+(x))   //f(x,y-1)
				+(unsigned char)*(lpbits+(y+1)*nwidthbytes+(x+1));   //f(x,y-1)
			temp = temp/9;
			*(lpNewbits+y*nwidthbytes+x) = (unsigned char)temp;
		}
	}

	CDocTemplate *pDocTem=GetDocument()->GetDocTemplate();
	
	CCCImageDoc  *pDoc=(CCCImageDoc*) pDocTem->OpenDocumentFile(NULL);;
	pDoc->m_hDIB = hNewDIB;
	// 更新DIB大小和调色板
	pDoc->InitDIBData();
	// 重新设置滚动视图大小
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	pDoc->SetModifiedFlag(TRUE);
	POSITION pos = pDoc->GetFirstViewPosition(); 
	CCCImageView* pView = (CCCImageView*)pDoc->GetNextView(pos);
		
    // 更新视图
	pDoc->UpdateAllViews(NULL);
}

// 非线性中值滤波
void CCCImageView::OnFiltersMedian() 
{
	CCCImageDoc* pDoc1 = GetDocument();
	ASSERT_VALID(pDoc1);
	HDIB hDIB = pDoc1->m_hDIB;
	
	if(hDIB == NULL)
		return;
   	
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//获得图像信息
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//数据指针指向位数据
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	unsigned int pailie[9];

	HDIB hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)+nwidthbytes*nheigth);
	if(!hNewDIB)
	{
		MessageBox("内存申请不成功",NULL,MB_OK);
		return;
	}
	LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
	LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
	
	for(int y = 1; y < nheigth-1; y++)
	{
		for(int x =1; x < nwidth-1; x++)
		{
			pailie[0] = (unsigned char)*(lpbits+(y-1)*nwidthbytes+(x-1));
			pailie[1] = (unsigned char)*(lpbits+(y-1)*nwidthbytes+(x));
			pailie[2] = (unsigned char)*(lpbits+(y-1)*nwidthbytes+(x+1));
            pailie[3] = (unsigned char)*(lpbits+(y)*nwidthbytes+(x-1));
			pailie[4] = (unsigned char)*(lpbits+(y)*nwidthbytes+(x));
			pailie[5] = (unsigned char)*(lpbits+(y)*nwidthbytes+(x+1));
			pailie[6] = (unsigned char)*(lpbits+(y+1)*nwidthbytes+(x-1));
			pailie[7] = (unsigned char)*(lpbits+(y+1)*nwidthbytes+(x));
			pailie[8] = (unsigned char)*(lpbits+(y+1)*nwidthbytes+(x+1));
            for(int k=1; k<=8;k++ )
			{
				if(pailie[k]>=pailie[k-1]) continue;
				int flag=pailie[k];
				int m=k;
				for (m=k-1;(m>=0)&&(pailie[m]>=flag);m--)
					pailie[m+1]=pailie[m];
				pailie[m+1]=flag;
			}
			*(lpNewbits+y*nwidthbytes+x) = (unsigned char)pailie[4];
		}
	}
	CDocTemplate *pDocTem=GetDocument()->GetDocTemplate();
	
	CCCImageDoc  *pDoc=(CCCImageDoc*) pDocTem->OpenDocumentFile(NULL);;
	pDoc->m_hDIB = hNewDIB;

	// 更新DIB大小和调色板
	pDoc->InitDIBData();
	// 重新设置滚动视图大小
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	pDoc->SetModifiedFlag(TRUE);
	POSITION pos = pDoc->GetFirstViewPosition(); 
	CCCImageView* pView = (CCCImageView*)pDoc->GetNextView(pos);
		
    // 更新视图
	pDoc->UpdateAllViews(NULL);
}

// 图像边缘增强
// Laplas算子
void CCCImageView::OnFiltersSharp() 
{
	CCCImageDoc* pDoc1 = GetDocument();
	ASSERT_VALID(pDoc1);
	HDIB hDIB = pDoc1->m_hDIB;
	
	if(hDIB == NULL)
		return;
   	
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//获得图像信息
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//数据指针指向位数据
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	HDIB hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)+nwidthbytes*nheigth);
	if(!hNewDIB)
	{
		MessageBox("内存申请不成功",NULL,MB_OK);
		return;
	}
	
	LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
	LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
	
	for(int y=1; y<nheigth-1; y++)
	{
		for(int x=1; x<nwidth-1; x++)
		{
			unsigned int temp = 0*(unsigned char)*(lpbits+(y-1)*nwidthbytes+(x-1)) //f(x-1,y-1)
				+(unsigned char)*(lpbits+(y-1)*nwidthbytes+(x))   //f(x,y-1)
				+0*(unsigned char)*(lpbits+(y-1)*nwidthbytes+(x+1))   //f(x+1,y-1)
				+(unsigned char)*(lpbits+(y  )*nwidthbytes+(x-1))   //f(x-1,y)
				-4*(unsigned char)*(lpbits+(y)*nwidthbytes+(x))   //f(x,y)
				+(unsigned char)*(lpbits+(y)*nwidthbytes+(x+1))   //f(x+1,y)
				+0*(unsigned char)*(lpbits+(y+1)*nwidthbytes+(x-1))   //f(x-1,y+1)
				+(unsigned char)*(lpbits+(y+1)*nwidthbytes+(x))   //f(x,y-1)
				+0*(unsigned char)*(lpbits+(y+1)*nwidthbytes+(x+1));   //f(x+1,y-1)
			//temp = temp/9;
			*(lpNewbits+y*nwidthbytes+x) = (unsigned char)temp;	
		}
	}
	CDocTemplate *pDocTem=GetDocument()->GetDocTemplate();
	
	CCCImageDoc  *pDoc=(CCCImageDoc*) pDocTem->OpenDocumentFile(NULL);;
	pDoc->m_hDIB = hNewDIB;

	// 更新DIB大小和调色板
	pDoc->InitDIBData();
	// 重新设置滚动视图大小
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	pDoc->SetModifiedFlag(TRUE);
	POSITION pos = pDoc->GetFirstViewPosition(); 
	CCCImageView* pView = (CCCImageView*)pDoc->GetNextView(pos);
		
    // 更新视图
	pDoc->UpdateAllViews(NULL);
}
