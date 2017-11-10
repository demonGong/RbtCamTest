
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RbtCamTest.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx=1250; 
	cs.cy=800; 
	cs.x   =   (GetSystemMetrics(SM_CXSCREEN)   -   cs.cx)/2; 
	cs.y   =   (GetSystemMetrics(SM_CYSCREEN)   -   cs.cy)/2; 

	cs.style   =     WS_CAPTION   |   WS_SYSMENU; 
	//WS_SYSMENU|WS_CAPTION   ; 
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	/*return CFrameWnd::OnCreateClient(lpcs, pContext);*/
	// TODO: 在此添加专用代码和/或调用基类
	/*  Main Window is divided as follows:0
	*--------------*---------------*-------*-------*-------*
	|    |          ADSoC		   |       |       |       |
	|    0	ZSlice	 |	 3D  	   0 	   |       |       |
	|Tool|			 1			   | Slice | Fusion|Info   |
	|Form|			 |			   | Form  | View  | View  |
	|	 0----1----1-|---1---1-----0	   |       |       | 
	*	 |			 |			   |	   |       |       | 
	|    |			 |			   |	   |       |       |
	*	 0	XSlice	 1   YSlice	   0       |       |       |
	|    |			 |             |	   |       |       |
	*---------*---------*----------*-------*-------*-------*
	*/

	//分割窗体
	if (m_wndSplitter.CreateStatic(this,1,2)==NULL)  			//分割主窗体
	{
		this->DestroyWindow();
		return FALSE;
	}
	


	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CRbtCamTestView), CSize(800, 0), pContext)||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(ComTestView), CSize(0, 0), pContext))
	{
		m_wndSplitter.DestroyWindow();
		
		return FALSE;
	}

	CRect ClientRc;
	GetClientRect(&ClientRc);

// 	int nInfoPaneWidth= MAX_RP_WIDTH, nToolPaneWidth = MAX_LP_WIDTH;
// 	int iWidthSpliter1_0 = ClientRc.Width() - nInfoPaneWidth - nToolPaneWidth;
// 	int iHeightSpliter1_0 = ClientRc.Height();
// 
// 	m_wndSplitter.SetColumnInfo(0,nToolPaneWidth,0);
// 	m_wndSplitter.SetColumnInfo(1,iWidthSpliter1_0,0);
// 	m_wndSplitter.SetColumnInfo(2,0,0);
// 	m_wndSplitter.SetColumnInfo(3,0,0);
// 	m_wndSplitter1.SetColumnInfo(0,iWidthSpliter1_0/2,0);
// 	m_wndSplitter1.SetRowInfo(0,iHeightSpliter1_0/2,0);	



	return TRUE;
}
