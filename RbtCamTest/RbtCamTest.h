
// RbtCamTest.h : main header file for the RbtCamTest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRbtCamTestApp:
// See RbtCamTest.cpp for the implementation of this class
//

class CRbtCamTestApp : public CWinApp
{
public:
	CRbtCamTestApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRbtCamTestApp theApp;
