// FreeDriveButton.cpp : 实现文件
//

#include "stdafx.h"
#include "FreeDriveButton.h"


// CFreeDriveButton

IMPLEMENT_DYNAMIC(CFreeDriveButton, CButton)

CFreeDriveButton::CFreeDriveButton()
{
	m_nBreakTime = DEF_TIMER_BREAK;
}

CFreeDriveButton::~CFreeDriveButton()
{
}


BEGIN_MESSAGE_MAP(CFreeDriveButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CFreeDriveButton 消息处理程序


void CFreeDriveButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	 
	m_bTracking = FALSE;

	SetTimer(CU_MYBUTTON_TIMER,m_nBreakTime,NULL);    // 启动示教指令持续发送

	CButton::OnLButtonDown(nFlags, point);
}


void CFreeDriveButton::OnMouseHover(UINT nFlags, CPoint point)
{
 
	CButton::OnMouseHover(nFlags, point);

	m_bTracking = FALSE;
}


void CFreeDriveButton::OnMouseLeave()
{
	 
	CButton::OnMouseLeave();

	m_bTracking = FALSE;
}


void CFreeDriveButton::OnMouseMove(UINT nFlags, CPoint point)
{
	 
	if(!m_bTracking && nFlags == MK_LBUTTON)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}


void CFreeDriveButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bTracking = TRUE;

	KillTimer(CU_MYBUTTON_TIMER);
	CButton::OnLButtonUp(nFlags, point);

	WPARAM wParam = ( ((DWORD)WM_LBUTTONUP)<<16 ) + ((DWORD)GetDlgCtrlID()) ;
	::SendMessage(this->GetParent()->GetSafeHwnd(),WM_COMMAND,wParam,0);
}


void CFreeDriveButton::OnTimer(UINT_PTR nIDEvent)
{
	
	if(CU_MYBUTTON_TIMER == nIDEvent)
	{
		WPARAM wParam = (((DWORD)WM_LBUTTONDOWN) << 16) + ((DWORD)GetDlgCtrlID());
		::PostMessage(this->GetParent()->GetSafeHwnd(),WM_COMMAND, wParam, 0);
	}

	CButton::OnTimer(nIDEvent);
}

void CFreeDriveButton::SetBreakTime(int nMilliSecond)
{
	if(nMilliSecond > 0)
		m_nBreakTime = nMilliSecond;
}