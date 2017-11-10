
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "RbtCamTestView.h"
#include "ComTestView.h"

#define		MAX_RP_WIDTH	300   //300   //4K 450
#define		MAX_LP_WIDTH	53    // 53   //4k 75
#define     MIN_VIEW_WIDTH  0

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar        m_wndStatusBar;
	CSplitterWnd	m_wndSplitter;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


