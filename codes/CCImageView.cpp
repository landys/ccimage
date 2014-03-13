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
	// ��ʾ�ȴ����
	BeginWaitCursor();
	
	// ��ȡ�ĵ�
	CCCImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// ��ȡDIB
	HDIB hDIB = pDoc->GetHDIB();
	
	// �ж�DIB�Ƿ�Ϊ��
	if (hDIB != NULL)
	{
		LPBYTE lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);
		
		// ��ȡDIB���
		int cxDIB = (int) pDoc->m_clsDIB.DIBWidth(lpDIB);
		
		// ��ȡDIB�߶�
		int cyDIB = (int) pDoc->m_clsDIB.DIBHeight(lpDIB);
		
		::GlobalUnlock((HGLOBAL) hDIB);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		
		CRect rcDest;
		
		// �ж��Ƿ��Ǵ�ӡ
		if (pDC->IsPrinting())
		{
			// �Ǵ�ӡ���������ͼ���λ�úʹ�С���Ա����ҳ��
			
			// ��ȡ��ӡҳ���ˮƽ���(����)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			
			// ��ȡ��ӡҳ��Ĵ�ֱ�߶�(����)
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			
			// ��ȡ��ӡ��ÿӢ��������
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			
			// �����ӡͼ���С�����ţ�����ҳ���ȵ���ͼ���С��
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch) / ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
			
			// �����ӡͼ��λ�ã���ֱ���У�
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp/2;
			rcDest.top += temp/2;
		}
		else // �Ǵ�ӡ
		{
			// ��������ͼ��
			rcDest = rcDIB;
		}
		// ���DIB
		pDoc->m_clsDIB.PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());

		// �������ù�����ͼ��С
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	}
	// �ָ��������
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
	// ������ҳ��Ϊһ��
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
//	���㴰����ͼ��С���Լ������������   //	
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
//	�����Ӵ���Ĭ�ϱ���ɫΪm_refColorBKG  //
///////////////////////////////////////////

BOOL CCCImageView::OnEraseBkgnd(CDC* pDC) 
{
	// ��ȡ�ĵ�
	CCCImageDoc* pDoc = GetDocument();
	
	// ����һ��Brush
	CBrush brush(0x808080);                                              
	
	// ������ǰ��Brush
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	
	// ��ȡ�ػ�����
	CRect rectClip;
	pDC->GetClipBox(&rectClip);
	
	// �ػ�
	pDC->PatBlt(rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(), PATCOPY);
	
	// �ָ���ǰ��Brush
	pDC->SelectObject(pOldBrush);                                                  
	
	// ����
	return TRUE;
}


///////////////////////////////////////////
//	ʵ���µĵ�ɫ��						 //
///////////////////////////////////////////

void CCCImageView::OnDoRealize(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);
	
	// ��ȡ�ĵ�
	CCCImageDoc* pDoc = GetDocument();
	
	// �ж�DIB�Ƿ�Ϊ��
	if (pDoc->GetHDIB() == NULL)
	{
		// ֱ�ӷ���
		return;
	}
	
	// ��ȡPalette
	CPalette* pPal = pDoc->GetDocPalette();
	if (pPal != NULL)
	{
		// ��ȡMainFrame
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
	// ��ȡ�ĵ�
	CCCImageDoc* pDoc = GetDocument();
	
	// �򿪼�����
	if (OpenClipboard())
	{
		// ���Ĺ����״
		BeginWaitCursor();
		
		// ��ռ�����
		EmptyClipboard();
		
		// ���Ƶ�ǰͼ�񵽼�����
		SetClipboardData (CF_DIB, pDoc->m_clsDIB.CopyHandle((HANDLE) pDoc->GetHDIB()) );
		
		// �رռ�����
		CloseClipboard();
		
		// �ָ����
		EndWaitCursor();
	}
	
}

void CCCImageView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// �����ǰDIB���󲻿գ����Ʋ˵�����Ч
	pCmdUI->Enable(GetDocument()->GetHDIB() != NULL);
}

void CCCImageView::OnEditPaste() 
{
	// ������DIB
	HDIB hNewDIB = NULL;
	
	// ��ȡ�ĵ�
	CCCImageDoc* pDoc = GetDocument();
	
	// �򿪼�����
	if (OpenClipboard())
	{
		// ���Ĺ����״
		BeginWaitCursor();
		
		// ��ȡ�������е�ͼ��
		hNewDIB = (HDIB) pDoc->m_clsDIB.CopyHandle(::GetClipboardData(CF_DIB));
		
		// �رռ�����
		CloseClipboard();
		
		// �ж��Ƿ��ȡ�ɹ�
		if (hNewDIB != NULL)
		{
			// �滻DIB��ͬʱ�ͷž�DIB����
			pDoc->ReplaceHDIB(hNewDIB);
			
			// ����DIB��С�͵�ɫ��
			pDoc->InitDIBData();
			
			// ��������
			pDoc->SetModifiedFlag(TRUE);
			
			// �������ù�����ͼ��С
			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
			
			// ʵ���µĵ�ɫ��
			OnDoRealize((WPARAM)m_hWnd,0);
			
			// ������ͼ
			pDoc->UpdateAllViews(NULL);
		}
		
		// �ָ����
		EndWaitCursor();
	}
}

void CCCImageView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// �����ǰ����������DIB����ճ���˵�����Ч
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
}


// ͼ����ײ�ֵ�Ŵ�
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
	//���ͼ����Ϣ
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//����ָ��ָ��λ����
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	//����ͼ���С��
	int mwidth = 2*nwidth;
	int mheigth = 2*nheigth;
	int mwidthbytes = ((mwidth+3)/4)*4;
	
	int mBitmapSize = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)+mheigth*mwidthbytes;
	//������ͼ����ڴ棬����ͷ��Ϣ����
	hNewDIB =(HDIB ) GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,mBitmapSize);
	if(!hNewDIB)
	{
		MessageBox("�ڴ����벻�ɹ�",NULL,MB_OK);
		return;
	}
	
	LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
	LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
	
	lpNewbmi->biWidth = 2*lpbmi->biWidth;
	lpNewbmi->biHeight=2*lpbmi->biHeight;
	
	//�����ֵ
	
	for(int x = 0; x<lpNewbmi->biWidth; x++)
	{
		for(int y =0; y< lpNewbmi->biHeight; y++)
		{
			*(lpNewbits+y*mwidthbytes+x) = *(lpbits+(y/2)*nwidthbytes+(x/2));
		}
	}	
	// �ͷ�ԭͼ�񣬽���ǰλͼ���ָ��Ŵ���ͼ��
		
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
	GlobalUnlock(hNewDIB);
	
	// �滻DIB��ͬʱ�ͷž�DIB����
	pDoc->ReplaceHDIB(hNewDIB);
	
	// ����DIB��С�͵�ɫ��
	pDoc->InitDIBData();
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// �������ù�����ͼ��С
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

// ͼ�����Բ�ֵ�Ŵ�
void CCCImageView::OnGeometryLinearenlarge() 
{
	// �����Ի���
	CDlgGeomZoom dlg;
	
	// ��ʾ�Ի������û����г߶ȱ��������趨
	if (dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		HDIB hNewDIB;
		CCCImageDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		HDIB hDIB = pDoc->m_hDIB;	
		if(hDIB == NULL)
			return;
		float fscale = dlg.m_fValue;
		
		LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
		//���ͼ����Ϣ
		LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
		int nwidth = lpbmi->biWidth;
		int nheigth = lpbmi->biHeight;
		int nwidthbytes = ((nwidth+3)/4)*4;
		//����ָ��ָ��λ����
		LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
		
		//����ͼ���С��
		int mwidth = (int)(fscale*nwidth);
		int mheigth =(int)(fscale*nheigth);
		int mwidthbytes = ((mwidth+3)/4)*4;
		
		int mBitmapSize = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)+mheigth*mwidthbytes;
		//   //������ͼ����ڴ棬����ͷ��Ϣ����
		hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,mBitmapSize);
		if(!hNewDIB)
		{
			MessageBox("�ڴ����벻�ɹ�",NULL,MB_OK);
			return;
		}
		
		LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
		LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
		
		LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
		memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
		
		
		
		lpNewbmi->biWidth = mwidth;
		lpNewbmi->biHeight = mheigth;
		
		//����˫���Բ�ֵ
		
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
		// �ͷ�ԭͼ�񣬽���ǰλͼ���ָ��Ŵ���ͼ��
		
		GlobalUnlock(hDIB);
		GlobalFree(hDIB);
		GlobalUnlock(hNewDIB);
		
		// �滻DIB��ͬʱ�ͷž�DIB����
		pDoc->ReplaceHDIB(hNewDIB);
		
		// ����DIB��С�͵�ɫ��
		pDoc->InitDIBData();
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// �������ù�����ͼ��С
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
}

// ͼ��ƽ��
void CCCImageView::OnGeometryTranslate() 
{
	// ��ȡ�ĵ�
	CCCImageDoc* pDoc = GetDocument();
	// �ɶԻ���������ת�Ƕ�
	CDlgSetOffset dlg;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		int lxOffset = dlg.m_xOffset;
		int lyOffset = dlg.m_yOffset;
		HDIB hDIB = pDoc->m_hDIB;
		HDIB hNewDIB;
		
		LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
		//���ͼ����Ϣ
		LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
		int nwidth = lpbmi->biWidth;
		int nheigth = lpbmi->biHeight;
		int nwidthbytes = ((nwidth+3)/4)*4;
		//����ָ��ָ��λ����
		LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
		
		//����ͼ���С��
		int mwidth = lxOffset+nwidth;
		int mheigth = lyOffset+nheigth;
		int mwidthbytes = ((mwidth+3)/4)*4;
		
		int mBitmapSize = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)+mheigth*mwidthbytes;
		//������ͼ����ڴ棬����ͷ��Ϣ����
		hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,mBitmapSize);
		if(!hNewDIB)
		{
			MessageBox("�ڴ����벻�ɹ�",NULL,MB_OK);
			return;
		}
		
		LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
		LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
		
		LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
		memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));
		
		lpNewbmi->biWidth = mwidth;
		lpNewbmi->biHeight = mheigth;
		
		// ƽ�ƺ�, ������Ĳ�����Ϊ��ɫ
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
		
		// �ͷ�ԭͼ�񣬽���ǰλͼ���ָ��Ŵ���ͼ��
		
		GlobalUnlock(hDIB);
		GlobalFree(hDIB);
		GlobalUnlock(hNewDIB);

		// �滻DIB��ͬʱ�ͷž�DIB����
		pDoc->ReplaceHDIB(hNewDIB);
		
		// ����DIB��С�͵�ɫ��
		pDoc->InitDIBData();
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// �������ù�����ͼ��С
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
}

// ͼ����ײ�ֵ��ת
void CCCImageView::OnGeometryRotate() 
{
	// ��ȡ�ĵ�
	CCCImageDoc* pDoc = GetDocument();
	
	// �ɶԻ���������ת�Ƕ�
	CDlgSetAngle dlg;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		int nAngle = int(dlg.m_fAngle);
		HDIB hNewDIB;
		HDIB hDIB = pDoc->m_hDIB;

		if(hDIB == NULL)
			return;
		LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
		//���ͼ����Ϣ
		LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
		int nwidth = lpbmi->biWidth;
		int nheigth = lpbmi->biHeight;
		int nwidthbytes = ((nwidth+3)/4)*4;
			//����ָ��ָ��λ����
		LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);

		double fcos = cos(nAngle*pi/180.0);
		double fsin = sin(nAngle*pi/180.0);

		//��ͼ������Ϊ����ԭ�㣬�����ĸ���������꣬�Լ���ת�������
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
      
	   //����ͼ���С��
		int mwidth = (int)(max(fabs(fDstX4-fDstX1), fabs(fDstX3-fDstX2))+0.5);
		int mheigth = (int)(max(fabs(fDstY4-fDstY1), fabs(fDstY3-fDstY2))+0.5);

		int mwidthbytes = ((mwidth+3)/4)*4;

		int mBitmapSize = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)+mheigth*mwidthbytes;
		//   //������ͼ����ڴ棬����ͷ��Ϣ����
		hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,mBitmapSize);
		if(!hNewDIB)
		{
			MessageBox("�ڴ����벻�ɹ�",NULL,MB_OK);
			return;
		}

		LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
		LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);

		LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
		memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));

		lpNewbmi->biWidth = mwidth;
		lpNewbmi->biHeight= mheigth;

		//�����ֵ

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

		// �ͷ�ԭͼ�񣬽���ǰλͼ���ָ����ת���ͼ��
		GlobalUnlock(hDIB);
	    GlobalFree(hDIB);
		GlobalUnlock(hNewDIB);

		// �滻DIB��ͬʱ�ͷž�DIB����
		pDoc->ReplaceHDIB(hNewDIB);
		
		// ����DIB��С�͵�ɫ��
		pDoc->InitDIBData();
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// �������ù�����ͼ��С
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
}

// ͼ�����Բ�ֵ��ת
void CCCImageView::OnGeometryLinearrotate() 
{
		// ��ȡ�ĵ�
	CCCImageDoc* pDoc = GetDocument();
	
	// �ɶԻ���������ת�Ƕ�
	CDlgSetAngle dlg;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		int nAngle = int(dlg.m_fAngle);
		HDIB hNewDIB;
		HDIB hDIB = pDoc->m_hDIB;
		if(hDIB == NULL)
			return;
		LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
		//���ͼ����Ϣ
		LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
		int nwidth = lpbmi->biWidth;
		int nheigth = lpbmi->biHeight;
		int nwidthbytes = ((nwidth+3)/4)*4;
		//����ָ��ָ��λ����
		LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);

		//��������������cos(angle) sin(angle)
		double fcos = cos(nAngle*pi/180.0);
		double fsin = sin(nAngle*pi/180.0);

		//��ͼ������Ϊ����ԭ�㣬�����ĸ���������꣬�Լ���ת�������
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
      
	   //����ͼ���С��
		int mwidth = (int)(max(fabs(fDstX4-fDstX1), fabs(fDstX3-fDstX2))+0.5);
		int mheigth = (int)(max(fabs(fDstY4-fDstY1), fabs(fDstY3-fDstY2))+0.5);

		int mwidthbytes = ((mwidth+3)/4)*4;

		int mBitmapSize = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)+mheigth*mwidthbytes;
		//������ͼ����ڴ棬����ͷ��Ϣ����
		hNewDIB =(HDIB) GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,mBitmapSize);
		if(!hNewDIB)
		{
			MessageBox("�ڴ����벻�ɹ�",NULL,MB_OK);
			return;
		}

		LPSTR lpNewDIB  = (LPSTR)GlobalLock(hNewDIB);
		LPSTR lpNewbits =  lpNewDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);

		LPBITMAPINFOHEADER lpNewbmi = (LPBITMAPINFOHEADER)lpNewDIB;
		memcpy(lpNewbmi,lpbmi,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD));

		lpNewbmi->biWidth = mwidth;
		lpNewbmi->biHeight= mheigth;

		//�����ֵ
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
		// �ͷ�ԭͼ�񣬽���ǰλͼ���ָ����ת���ͼ��
		GlobalUnlock(hDIB);
	    GlobalFree(hDIB);
		GlobalUnlock(hNewDIB);

		// �滻DIB��ͬʱ�ͷž�DIB����
		pDoc->ReplaceHDIB(hNewDIB);
		
		// ����DIB��С�͵�ɫ��
		pDoc->InitDIBData();
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// �������ù�����ͼ��С
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
}

// ͼ��ˮƽ����
void CCCImageView::OnGeometryHorimirror() 
{
	// ��ȡ�ĵ�
	CCCImageDoc* pDoc = GetDocument();
	
	// ѭ������
	int y;
	int x;
	
	HDIB hDIB = pDoc->m_hDIB;
	if(hDIB == NULL)
		return;
	
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//���ͼ����Ϣ
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheight = lpbmi->biHeight;
	int mwidthbytes = ((nwidth+3)/4)*4;
	//����ָ��ָ��λ����
	LPBYTE lpDIBBits = (unsigned char* )pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	// ������char��Ҳ������::strcpy()
	unsigned char* ptemp = new unsigned char[mwidthbytes * nheight];
	
	//��ֱ��������
	int lMiddleX = nwidth / 2;
	//���ͼ��ÿ�н��в���
	for(y=0; y<nheight; y++)
	{
		//ָ��ͼ��ĵ�����y��
		unsigned char* ptempOld = lpDIBBits;
		ptempOld +=  y * mwidthbytes;
		unsigned char* ptempNew = ptemp; 
		ptempNew +=  y * mwidthbytes;
		//���ÿ��ͼ����벿�ֽ��в���
		for(x=0; x<=lMiddleX; x++)
		{
			//��Դͼ���x�����ظ��Ƶ���ͼ������x������
			*(ptempNew + (nwidth -1 - x)) = *(ptempOld+x);
			//��Դͼ������x�����ظ��Ƶ���ͼ���x������
			*(ptempNew + x) = *(ptempOld + (nwidth -1 - x));
		}
	}
	
	// �ش洦������DIB
	for(x = 0; x < mwidthbytes * nheight; x++)
		*(lpDIBBits + x) = ptemp[x];
	
	// �������
	::GlobalUnlock(hDIB);
	
	// �ͷ��ڴ�
	delete[] ptemp;
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

// ͼ��ֱ����
void CCCImageView::OnGeometryVertmirror() 
{
	// ��ȡ�ĵ�
	CCCImageDoc* pDoc = GetDocument();
	
	// ѭ������
	int y;
	int x;
	
	HDIB hDIB = pDoc->m_hDIB;
	if(hDIB == NULL)
		return;
	
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//���ͼ����Ϣ
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheight = lpbmi->biHeight;
	int mwidthbytes = ((nwidth+3)/4)*4;
	//����ָ��ָ��λ����
	LPBYTE lpDIBBits = (unsigned char* )pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	// ������char��Ҳ������::strcpy()
	unsigned char* ptemp = new unsigned char[mwidthbytes * nheight];
	
	//ˮƽ�������ꪥ
	int lMiddleY = nheight / 2;
	//����ϰ�ͼ����в�����
	for(y=0; y <= lMiddleY; y++)
	{
		//ָ��Դͼ������y����������ָ�몥
		unsigned char* ptemp1 = lpDIBBits;
		ptemp1 +=  y * mwidthbytes;
		//ָ����ͼ���y����������ָ��
		unsigned char* ptemp2 = ptemp;
		ptemp2 +=  (nheight - 1 - y) * mwidthbytes;
		//��Դͼ������y�����ظ��Ƶ���ͼ���y��
		memcpy(ptemp2,  ptemp1,  mwidthbytes);
		//ָ��Դͼ���y����������ָ��
		ptemp1 = lpDIBBits;
		ptemp1 +=  (nheight -1 -y) * mwidthbytes;
		//ָ����ͼ������y����������ָ��
		ptemp2 = ptemp;
		ptemp2 +=  y * mwidthbytes;
		//��Դͼ���y�����ظ��Ƶ���ͼ������y��
		memcpy(ptemp2,  ptemp1,  mwidthbytes);
	}
	
	// �ش洦������DIB
	for(x = 0; x < mwidthbytes * nheight; x++)
		*(lpDIBBits + x) = ptemp[x];
	
	// �������
	::GlobalUnlock(hDIB);
	
	// �ͷ��ڴ�
	delete[] ptemp;
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

// ��ʾֱ��ͼ
void CCCImageView::OnHistography() 
{
	CCCImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HDIB hDIB = pDoc->m_hDIB;
	
	if(hDIB == NULL)
		return;
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//���ͼ����Ϣ
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//����ָ��ָ��λ����
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	CDlgHistography dlg;
	//ͳ��ֱ��ͼ
	for(int y = 0 ;  y < nheigth; y++)
	{
		for( int x = 0; x < nwidth; x++)
		{
			unsigned char gray = (unsigned char)*(lpbits+y*nwidthbytes+x);
			dlg.m_pHisto[gray]++;
		}
	}
	// ��ʾ���
	dlg.DoModal();
}

// ֱ��ͼ���⻯
void CCCImageView::OnHistoequ() 
{
	CCCImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HDIB hDIB = pDoc->m_hDIB;
	
	if(hDIB == NULL)
		return;
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//���ͼ����Ϣ
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//����ָ��ָ��λ����
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	CDlgHistography dlg;
	// ͳ��ֱ��ͼ
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
	
	// ���⻯
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
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// ��ʾֱ��ͼ�Ի���
	dlg.DoModal();
}

// FFT
void CCCImageView::OnOrthoFft() 
{
	CCCImageDoc* pDoc1 = GetDocument();
	ASSERT_VALID(pDoc1);
	HDIB hDIB = pDoc1->m_hDIB;

	HDIB hFFTAbs,hFFTPhs;

	// ��ʾ�ȴ����
	BeginWaitCursor();

	BOOL iRet = ::ImageFFT(hDIB, &hFFTAbs, &hFFTPhs);

	if(iRet == FALSE)
		MessageBox("FFT����!");

	CDocTemplate *pDocTem=GetDocument()->GetDocTemplate();

	CCCImageDoc *pDoc=(CCCImageDoc*) pDocTem->OpenDocumentFile(NULL);;
	pDoc->m_hDIB = hFFTPhs;
	// ����DIB��С�͵�ɫ��
	pDoc->InitDIBData();
	// �������ù�����ͼ��С
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	pDoc->SetModifiedFlag(TRUE);
	POSITION pos = pDoc->GetFirstViewPosition(); 
    CCCImageView* pView = (CCCImageView*)pDoc->GetNextView(pos);
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

	pDocTem=GetDocument()->GetDocTemplate();

	pDoc=(CCCImageDoc*) pDocTem->OpenDocumentFile(NULL);;
	pDoc->m_hDIB = hFFTAbs;
	// ����DIB��С�͵�ɫ��
	pDoc->InitDIBData();
	// �������ù�����ͼ��С
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	pDoc->SetModifiedFlag(TRUE);
	pos = pDoc->GetFirstViewPosition(); 
    pView = (CCCImageView*)pDoc->GetNextView(pos);
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	EndWaitCursor();
}

// ��ͨƽ���˲�
//average ƽ���任
//����Ϊ:       
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
	//���ͼ����Ϣ
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//����ָ��ָ��λ����
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	HDIB hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)+nwidthbytes*nheigth);
	if(!hNewDIB)
	{
		MessageBox("�ڴ����벻�ɹ�",NULL,MB_OK);
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
	// ����DIB��С�͵�ɫ��
	pDoc->InitDIBData();
	// �������ù�����ͼ��С
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	pDoc->SetModifiedFlag(TRUE);
	POSITION pos = pDoc->GetFirstViewPosition(); 
	CCCImageView* pView = (CCCImageView*)pDoc->GetNextView(pos);
		
    // ������ͼ
	pDoc->UpdateAllViews(NULL);
}

// ��������ֵ�˲�
void CCCImageView::OnFiltersMedian() 
{
	CCCImageDoc* pDoc1 = GetDocument();
	ASSERT_VALID(pDoc1);
	HDIB hDIB = pDoc1->m_hDIB;
	
	if(hDIB == NULL)
		return;
   	
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//���ͼ����Ϣ
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//����ָ��ָ��λ����
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	unsigned int pailie[9];

	HDIB hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)+nwidthbytes*nheigth);
	if(!hNewDIB)
	{
		MessageBox("�ڴ����벻�ɹ�",NULL,MB_OK);
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

	// ����DIB��С�͵�ɫ��
	pDoc->InitDIBData();
	// �������ù�����ͼ��С
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	pDoc->SetModifiedFlag(TRUE);
	POSITION pos = pDoc->GetFirstViewPosition(); 
	CCCImageView* pView = (CCCImageView*)pDoc->GetNextView(pos);
		
    // ������ͼ
	pDoc->UpdateAllViews(NULL);
}

// ͼ���Ե��ǿ
// Laplas����
void CCCImageView::OnFiltersSharp() 
{
	CCCImageDoc* pDoc1 = GetDocument();
	ASSERT_VALID(pDoc1);
	HDIB hDIB = pDoc1->m_hDIB;
	
	if(hDIB == NULL)
		return;
   	
	LPSTR pDIB = (LPSTR)GlobalLock(hDIB);
	//���ͼ����Ϣ
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)pDIB;
	int nwidth = lpbmi->biWidth;
	int nheigth = lpbmi->biHeight;
	int nwidthbytes = ((nwidth+3)/4)*4;
	//����ָ��ָ��λ����
	LPSTR lpbits = pDIB + sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	
	HDIB hNewDIB = (HDIB)GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)+nwidthbytes*nheigth);
	if(!hNewDIB)
	{
		MessageBox("�ڴ����벻�ɹ�",NULL,MB_OK);
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

	// ����DIB��С�͵�ɫ��
	pDoc->InitDIBData();
	// �������ù�����ͼ��С
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	pDoc->SetModifiedFlag(TRUE);
	POSITION pos = pDoc->GetFirstViewPosition(); 
	CCCImageView* pView = (CCCImageView*)pDoc->GetNextView(pos);
		
    // ������ͼ
	pDoc->UpdateAllViews(NULL);
}
