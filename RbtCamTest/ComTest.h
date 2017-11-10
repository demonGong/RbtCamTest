#pragma once
#include "afxwin.h"
#include <vector>
#include <iostream>

// CComTest dialog
using namespace std;
class CComTest : public CPropertyPage
{
	DECLARE_DYNAMIC(CComTest)

public:
	CComTest();
	virtual ~CComTest();

// Dialog Data
	enum { IDD = IDD_DIALOG_COM_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButtonRegTarget();
	afx_msg void OnBnClickedButtonMovetoTarget();
//	afx_msg void OnBnClickedRecordTheoryCoord();
	afx_msg void OnBnClickedRecordActualCoord();
//	afx_msg void OnBnClickedButtonProcData();
	afx_msg void OnBnClickedButtonSaveData();

	void drawToPics();
	void ComptTarget();
	void getInvertPos(); 
	//void 
	bool CheckIsExist(vector<double> &m_data, double x, double y, double z);

public:
	enum{
		DISPLAY,
		REG_RBT_MARKER_TIMER
	};				//timers

	CStatic m_leftPicture;
	CStatic m_rightPicture;
	CComboBox m_MarkerTemp;
	CComboBox m_angle;
	CComboBox m_target;
	CComboBox m_RegesterMehthod;

	double targetPt[3];
	double markerPos[3];
	double markerRotateMat[9];
	double markerPose[16];
	double vecPos[4][3];
	double m_tcpPin[3];
	double curTTBlockPos[3];

	bool isRegSucc;
	double m_LastTargetJoint[6];
	double m_curTargetJoint[6];
	vector<float *> m_Actural_TCP_Pos;
	vector<float *> m_Theory_TCP_Pos;
	vector<double> m_Target_Pos;
	
	
	afx_msg void OnBnClickedButtonLastMove();
};
