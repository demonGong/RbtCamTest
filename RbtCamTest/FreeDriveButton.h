#pragma once


// CFreeDriveButton

class CFreeDriveButton : public CButton
{
	DECLARE_DYNAMIC(CFreeDriveButton)

public:
	CFreeDriveButton();
	virtual ~CFreeDriveButton();

public:
	void SetBreakTime(int nMilliSecond);
	
protected:
	BOOL        m_bTracking;
	int         m_nBreakTime;

	enum {CU_MYBUTTON_TIMER};

	enum { 
		DEF_TIMER_BREAK = 100};  // й╪жу

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


