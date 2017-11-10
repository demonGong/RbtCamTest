#pragma once

#include "RegisterRbt.h"
#include "ComTest.h"
#include "FreeDriveButton.h"
// ComTestView form view

class ComTestView : public CFormView
{
	DECLARE_DYNCREATE(ComTestView)

protected:
	ComTestView();           // protected constructor used by dynamic creation
	virtual ~ComTestView();

public:
	enum { IDD = IDD_DIALOG_TEST_VIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtPrShutDown();
	afx_msg void OnBnClickedBtPrRbtUnlock();
	afx_msg void OnBnClickedBtRmgAutoToZero();
	afx_msg void OnBnClickedBtRmgStopRobot();
	afx_msg void OnBnClickedButtonSamplingMarkerData();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	void samplingMarkersPos();

public:
	CPropertySheet m_sheet;
	CRegisterRbt m_page1;
	CComTest m_page2;
	CFreeDriveButton m_btFreeDrive;

private:
	enum{
		DISPLAY_TCP_POS,
		REG_MARKER_TIMER
	};				//timers
	
	
};


