
// RbtCamTestView.h : interface of the CRbtCamTestView class
//

#pragma once
#include "RbtCamTestDoc.h"


class CRbtCamTestView : public CView
{
protected: // create from serialization only
	CRbtCamTestView();
	DECLARE_DYNCREATE(CRbtCamTestView)

// Attributes
public:
	CRbtCamTestDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRbtCamTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	void drawToHDc();
	void openCamera();
private:
	enum{
		DISPLAY,
	};				//timers
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in RbtCamTestView.cpp
inline CRbtCamTestDoc* CRbtCamTestView::GetDocument() const
   { return reinterpret_cast<CRbtCamTestDoc*>(m_pDocument); }
#endif

