
// RbtCamTestView.cpp : implementation of the CRbtCamTestView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RbtCamTest.h"
#endif

#include "RbtCamTestDoc.h"
#include "RbtCamTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRbtCamTestView

IMPLEMENT_DYNCREATE(CRbtCamTestView, CView)

BEGIN_MESSAGE_MAP(CRbtCamTestView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CRbtCamTestView construction/destruction

CRbtCamTestView::CRbtCamTestView()
{
	// TODO: add construction code here

}

CRbtCamTestView::~CRbtCamTestView()
{
}

BOOL CRbtCamTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRbtCamTestView drawing

void CRbtCamTestView::OnDraw(CDC* /*pDC*/)
{
	CRbtCamTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CRbtCamTestView printing

BOOL CRbtCamTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRbtCamTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRbtCamTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRbtCamTestView diagnostics

#ifdef _DEBUG
void CRbtCamTestView::AssertValid() const
{
	CView::AssertValid();
}

void CRbtCamTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRbtCamTestDoc* CRbtCamTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRbtCamTestDoc)));
	return (CRbtCamTestDoc*)m_pDocument;
}

void CRbtCamTestView::drawToHDc()
{
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)GetDocument();
	ASSERT(pDoc != NULL);

	CRect rect;
	this->GetClientRect(rect);

	CDC *pDC;
	pDC = this->GetDC();

	if (pDC == NULL)
		return;

	pDoc->m_Cam->setMTDC(pDC);
	pDoc->m_Cam->processCurrFrame();
	pDoc->m_Cam->DrawToPic(pDC,&rect,true,0);
	this->ReleaseDC(pDC);

	

}

void CRbtCamTestView::openCamera()
{
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)GetDocument();
	ASSERT(pDoc != NULL);


	pDoc->m_Cam->initialINIAccess();
	if (pDoc->m_Cam->setupCameras())
	{
		pDoc->m_Cam->refreshMarkerTemplates();
		pDoc->m_Cam->isCameraOpened = true;

		SetTimer(DISPLAY,70,NULL);
	}

}

#endif //_DEBUG


// CRbtCamTestView message handlers


void CRbtCamTestView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case DISPLAY:
		drawToHDc();
		//UpdateWindow();
		break;
	default:
		break;
	}
	CView::OnTimer(nIDEvent);
}


void CRbtCamTestView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	openCamera();
	// TODO: Add your specialized code here and/or call the base class
}


BOOL CRbtCamTestView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//设置brush为希望的背景颜色
	COLORREF crColor(RGB(0,0,0));
	CBrush backBrush(crColor);

	//保存旧的brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);

	//画需要的区域
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);

	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}
