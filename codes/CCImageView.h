// CCImageView.h : interface of the CCCImageView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCIMAGEVIEW_H__D76B0162_780B_4FB5_8A3C_35299CFB317A__INCLUDED_)
#define AFX_CCIMAGEVIEW_H__D76B0162_780B_4FB5_8A3C_35299CFB317A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCCImageView : public CScrollView
{
protected: // create from serialization only
	CCCImageView();
	DECLARE_DYNCREATE(CCCImageView)

// Attributes
public:
	CCCImageDoc* GetDocument();
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCImageView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCCImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCCImageView)
	afx_msg void OnDoRealize(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnFiltersMedian();
	afx_msg void OnFiltersSmooth();
	afx_msg void OnFiltersSharp();
	afx_msg void OnGeometryHorimirror();
	afx_msg void OnGeometryRotate();
	afx_msg void OnGeometryTranslate();
	afx_msg void OnGeometryVertmirror();
	afx_msg void OnOrthoFft();
	afx_msg void OnGeometryEnlarge();
	afx_msg void OnGeometryLinearenlarge();
	afx_msg void OnGeometryLinearrotate();
	afx_msg void OnHistoequ();
	afx_msg void OnHistography();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CCImageView.cpp
inline CCCImageDoc* CCCImageView::GetDocument()
   { return (CCCImageDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCIMAGEVIEW_H__D76B0162_780B_4FB5_8A3C_35299CFB317A__INCLUDED_)
