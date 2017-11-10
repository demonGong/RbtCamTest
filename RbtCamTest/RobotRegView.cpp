// RobotRegView.cpp : implementation file
//

#include "stdafx.h"
#include "RbtCamTest.h"
#include "RobotRegView.h"
#include "afxdialogex.h"
#include "RbtCamTestDoc.h"
#include "MainFrm.h"


// CRobotRegView dialog

IMPLEMENT_DYNAMIC(CRobotRegView, CDialogEx)

CRobotRegView::CRobotRegView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRobotRegView::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

	m_clrRegFinished = RGB(0,128,0);
	m_clrRegUnfinished = RGB(128,0,0);

	m_rbtInitialPos_1[0] = 85.31000;
	m_rbtInitialPos_1[1] = -142.05000;
	m_rbtInitialPos_1[2] = 144.55000;
	m_rbtInitialPos_1[3] = -88.00000;
	m_rbtInitialPos_1[4] = 2.87000;
	m_rbtInitialPos_1[5] = -2.99000;

	for (int i=0; i<4; i++)
		m_bRbtPosRegFinished[i] = FALSE;
	
	markerName = _T("1");

	m_nRegRobotIndex = 0;

	m_nRegPointCounter = 0;

	for (int i=0;i<4;i++)
		for(int j=0;j<3;j++)
		{
			m_regRobotMarkerPos[i][j] = 0.0;
		}

}

CRobotRegView::~CRobotRegView()
{
}

void CRobotRegView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RB_POS1_STATUS, m_stRbtPos1RegStatus);
	DDX_Control(pDX, IDC_RB_POS2_STATUS, m_stRbtPos2RegStatus);
	DDX_Control(pDX, IDC_RB_POS3_STATUS, m_stRbtPos3RegStatus);
	DDX_Control(pDX, IDC_RB_POS4_STATUS, m_stRbtPos4RegStatus);
}


BEGIN_MESSAGE_MAP(CRobotRegView, CDialogEx)
	ON_BN_CLICKED(IDC_BT_MOVE_TO_POS1, &CRobotRegView::OnBnClickedBtMovetoPos1)
	ON_BN_CLICKED(IDC_BT_MOVE_TO_POS2, &CRobotRegView::OnBnClickedBtMoveToPos2)
	ON_BN_CLICKED(IDC_BT_MOVE_TO_POS3, &CRobotRegView::OnBnClickedBtMoveToPos3)
	ON_BN_CLICKED(IDC_BT_MOVE_TO_POS4, &CRobotRegView::OnBnClickedBtMoveToPos4)
	ON_BN_CLICKED(IDC_BT_POS2_REREGISTER, &CRobotRegView::OnBnClickedBtPos2Reregister)
	ON_BN_CLICKED(IDC_BT_POS3_REREGISTER, &CRobotRegView::OnBnClickedBtPos3Reregister)
	ON_BN_CLICKED(IDC_BT_POS4_REREGISTER, &CRobotRegView::OnBnClickedBtPos4Reregister)
	ON_BN_CLICKED(IDC_BT_POS1_REREGISTER, &CRobotRegView::OnBnClickedBtPos1Reregister)
	ON_WM_TIMER()

	
	ON_BN_CLICKED(IDC_BT_RMG_REGISTER_ALL, &CRobotRegView::OnBnClickedBtRmgRegisterAll)
	ON_BN_CLICKED(IDC_BT_RMG_AUTO_TO_ZERO, &CRobotRegView::OnBnClickedBtRmgAutoToZero)
	ON_BN_CLICKED(IDC_BT_RMG_STOP_ROBOT, &CRobotRegView::OnBnClickedBtRmgStopRobot)
	ON_BN_CLICKED(IDC_BT_PR_RBT_UNLOCK, &CRobotRegView::OnBnClickedBtPrRbtUnlock)
	ON_BN_CLICKED(IDC_BT_PR_SHUT_DOWN, &CRobotRegView::OnBnClickedBtPrShutDown)
END_MESSAGE_MAP()


// CRobotRegView message handlers


void CRobotRegView::OnBnClickedBtMovetoPos1()
{
	// TODO: Add your control notification handler code here
	m_nRegRobotIndex = 0;
	BtMoveToPos(m_nRegRobotIndex);

	//////////////////////////////////////////////////////////////////////////////
// 	CString logStr;
// 	wkCommon dirHelper;
// 	logStr = dirHelper.GetAppExePath();
// 	logStr += _T("Robot\\RegisterPosition\\");
// 	dirHelper.CreateDirEx(logStr);

// 	double angle[6];
// 	pDoc->RbtCtrller->GetActualJointPos(angle);
// 	wkCommon log;
// 	CString filePath = logStr + _T("robotMovePos1.txt");
// 	CString strVal;
// 	strVal.Format(_T("%.3f %.3f %.3f %.3f %.3f %.3f\n"),angle[0],angle[1],angle[2],angle[3],angle[4],angle[5]);
// 	log.RecordInfoTrunc(filePath,strVal);

	////////////////////////////////////////////////////////////////////////
	GetDlgItem(IDC_BT_MOVE_TO_POS1)->EnableWindow(FALSE);
	SetTimer(REG_RBT_MARKER_TIMER,70,NULL);
		
}


void CRobotRegView::OnBnClickedBtMoveToPos2()
{
	// TODO: Add your control notification handler code here
	m_nRegRobotIndex = 1;
	BtMoveToPos(m_nRegRobotIndex);

	//////////////////////////////////////////////////////////////////////////////
	// 	CString logStr;
	// 	wkCommon dirHelper;
	// 	logStr = dirHelper.GetAppExePath();
	// 	logStr += _T("Robot\\RegisterPosition\\");
	// 	dirHelper.CreateDirEx(logStr);

	// 	double angle[6];
	// 	pDoc->RbtCtrller->GetActualJointPos(angle);
	// 	wkCommon log;
	// 	CString filePath = logStr + _T("robotMovePos1.txt");
	// 	CString strVal;
	// 	strVal.Format(_T("%.3f %.3f %.3f %.3f %.3f %.3f\n"),angle[0],angle[1],angle[2],angle[3],angle[4],angle[5]);
	// 	log.RecordInfoTrunc(filePath,strVal);

	////////////////////////////////////////////////////////////////////////
	GetDlgItem(IDC_BT_MOVE_TO_POS2)->EnableWindow(FALSE);
	SetTimer(REG_RBT_MARKER_TIMER,70,NULL);
		
}

void CRobotRegView::OnBnClickedBtMoveToPos3()
{
	// TODO: Add your control notification handler code here
	m_nRegRobotIndex = 2;
	BtMoveToPos(m_nRegRobotIndex);

	//////////////////////////////////////////////////////////////////////////////
	// 	CString logStr;
	// 	wkCommon dirHelper;
	// 	logStr = dirHelper.GetAppExePath();
	// 	logStr += _T("Robot\\RegisterPosition\\");
	// 	dirHelper.CreateDirEx(logStr);

	// 	double angle[6];
	// 	pDoc->RbtCtrller->GetActualJointPos(angle);
	// 	wkCommon log;
	// 	CString filePath = logStr + _T("robotMovePos1.txt");
	// 	CString strVal;
	// 	strVal.Format(_T("%.3f %.3f %.3f %.3f %.3f %.3f\n"),angle[0],angle[1],angle[2],angle[3],angle[4],angle[5]);
	// 	log.RecordInfoTrunc(filePath,strVal);

	////////////////////////////////////////////////////////////////////////
	GetDlgItem(IDC_BT_MOVE_TO_POS3)->EnableWindow(FALSE);
	SetTimer(REG_RBT_MARKER_TIMER,70,NULL);
		
}

void CRobotRegView::OnBnClickedBtMoveToPos4()
{
	// TODO: Add your control notification handler code here
	m_nRegRobotIndex = 3;
	BtMoveToPos(m_nRegRobotIndex);

	GetDlgItem(IDC_BT_MOVE_TO_POS4)->EnableWindow(FALSE);
	SetTimer(REG_RBT_MARKER_TIMER,70,NULL);

		
}


void CRobotRegView::OnBnClickedBtPos1Reregister()
{
	// TODO: Add your control notification handler code here
	ReRegister(0);

	GetDlgItem(IDC_BT_MOVE_TO_POS1)->EnableWindow(TRUE);
}

void CRobotRegView::OnBnClickedBtPos2Reregister()
{
	// TODO: Add your control notification handler code here
	ReRegister(1);

	GetDlgItem(IDC_BT_MOVE_TO_POS2)->EnableWindow(TRUE);
}


void CRobotRegView::OnBnClickedBtPos3Reregister()
{
	// TODO: Add your control notification handler code here
	ReRegister(2);

	GetDlgItem(IDC_BT_MOVE_TO_POS3)->EnableWindow(TRUE);
}


void CRobotRegView::OnBnClickedBtPos4Reregister()
{
	// TODO: Add your control notification handler code here
	ReRegister(3);

	GetDlgItem(IDC_BT_MOVE_TO_POS4)->EnableWindow(TRUE);
}

void CRobotRegView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case UPDATE_BUTTON_TIMER:
		UpdateRegStatus();
		break;
	case REG_RBT_MARKER_TIMER:
		CaptureRobotMarker();
		break;
	case CHECK_ACCURACY_TIME:
		CheckRegAccuracy();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CRobotRegView::CaptureRobotMarker()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	double pos[3];
	double rotateMat[9];
	int ret;
	ret = pDoc->m_Cam->getMarkerPosByName(markerName,pos,rotateMat);
	if(!ret)
		return;

	
	if(m_nRegPointCounter < REG_CAP_POINT_NUM)
	{
		for (int index=0; index<3; index++)
			m_regRobotMarkerPtArr[m_nRegPointCounter][index] = pos[index];

		m_nRegPointCounter++;
	}
	else
	{
		KillTimer(REG_RBT_MARKER_TIMER);

		/// 计算四个机械臂位置平均值
		double sumXX,sumYY,sumZZ;
		for (int i=0;i<REG_CAP_POINT_NUM;i++)
		{
			for (int j=0; j<3; j++)
				m_regRobotMarkerPos[m_nRegRobotIndex][j] += m_regRobotMarkerPtArr[i][j];
		}

		for (int i=0; i<3; i++)
			m_regRobotMarkerPos[m_nRegRobotIndex][i] /= REG_CAP_POINT_NUM;
		
		/// 计算标准差
		sumXX = 0.0;
		sumYY = 0.0;
		sumZZ = 0.0;

		for (int i=0;i<REG_CAP_POINT_NUM;i++)
		{
			sumXX += (m_regRobotMarkerPtArr[i][0] - m_regRobotMarkerPos[m_nRegRobotIndex][0])*(m_regRobotMarkerPtArr[i][0] - m_regRobotMarkerPos[m_nRegRobotIndex][0]);
			sumYY += (m_regRobotMarkerPtArr[i][1] - m_regRobotMarkerPos[m_nRegRobotIndex][1])*(m_regRobotMarkerPtArr[i][1] - m_regRobotMarkerPos[m_nRegRobotIndex][1]);
			sumZZ += (m_regRobotMarkerPtArr[i][2] - m_regRobotMarkerPos[m_nRegRobotIndex][2])*(m_regRobotMarkerPtArr[i][2] - m_regRobotMarkerPos[m_nRegRobotIndex][2]);
		}

		double varX,varY,varZ;
		varX = sqrt(sumXX/REG_CAP_POINT_NUM);
		varY = sqrt(sumYY/REG_CAP_POINT_NUM);
		varZ = sqrt(sumZZ/REG_CAP_POINT_NUM);

		if(varX > pDoc->markRegVarThreshold || varY > pDoc->markRegVarThreshold || varZ > pDoc->markRegVarThreshold)
		{
			m_nRegPointCounter = 0;                // 置0，重新开始采集
			for (int i=0;i<3;i++)
			{
				m_regRobotMarkerPos[m_nRegRobotIndex][i] = 0.0;
			}

			SetTimer(REG_RBT_MARKER_TIMER,70,NULL);

			return;
		}
		else
		{
			m_nRegPointCounter = 0;
			m_bRbtPosRegFinished[m_nRegRobotIndex] = TRUE;
			UpdateRegStatus();

			CString str;
			str.Format(_T("位置%d注册完成!"),m_nRegRobotIndex+1);
			AfxMessageBox(str);

		}

// 		m_lastRegPointIndex = m_nRegRobotIndex;
// 		m_accuracyTestPoint[0] =0.75*m_regRobotMarkerPtArr[0][0] + 0.25*m_regRobotMarkerPos[m_nRegRobotIndex][0];
// 		m_accuracyTestPoint[1] =0.75*m_regRobotMarkerPtArr[0][1] + 0.25*m_regRobotMarkerPos[m_nRegRobotIndex][1];
// 		m_accuracyTestPoint[2] =0.75*m_regRobotMarkerPtArr[0][2] + 0.25*m_regRobotMarkerPos[m_nRegRobotIndex][2];

		RegisterRobot();   /// 注册机械臂
	}
}

void CRobotRegView::UpdateRegStatus()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if (pDoc->m_bRbtManualRegFinised)
	{
		for (int i=0; i<4; i++)
			m_bRbtPosRegFinished[i] = TRUE;

		SetBkColor(m_stRbtPos1RegStatus,m_clrRegFinished);
		SetBkColor(m_stRbtPos2RegStatus,m_clrRegFinished);
		SetBkColor(m_stRbtPos3RegStatus,m_clrRegFinished);
		SetBkColor(m_stRbtPos4RegStatus,m_clrRegFinished);

		GetDlgItem(IDC_BT_MOVE_TO_POS1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_MOVE_TO_POS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_MOVE_TO_POS3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_MOVE_TO_POS4)->EnableWindow(FALSE);

		return;

	}

	if(m_bRbtPosRegFinished[0])
		SetBkColor(m_stRbtPos1RegStatus,m_clrRegFinished);
	else
		SetBkColor(m_stRbtPos1RegStatus,m_clrRegUnfinished);

	if(m_bRbtPosRegFinished[1])
		SetBkColor(m_stRbtPos2RegStatus,m_clrRegFinished);
	else
		SetBkColor(m_stRbtPos2RegStatus,m_clrRegUnfinished);

	if(m_bRbtPosRegFinished[2])
		SetBkColor(m_stRbtPos3RegStatus,m_clrRegFinished);
	else
		SetBkColor(m_stRbtPos3RegStatus,m_clrRegUnfinished);

	if(m_bRbtPosRegFinished[3])
		SetBkColor(m_stRbtPos4RegStatus,m_clrRegFinished);
	else
		SetBkColor(m_stRbtPos4RegStatus,m_clrRegUnfinished);


}

void CRobotRegView::BtMoveToPos(int m_nRegRobotIndex)
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if(!pDoc->RbtCtrller->CheckJointMode(ROBOT_RUNNING_MODE))
		pDoc->RbtCtrller->SetRobotRunningMode();

	pDoc->RbtCtrller->SetTcpPos(pDoc->robotMarkerPos);

	double tipPos[6];
	pDoc->RbtCtrller->GetActualTcpPos(tipPos);
	for (int i=0;i<3;i++)
	{
		m_regRobotTipPos[m_nRegRobotIndex][i] = tipPos[i];
	}
}

void CRobotRegView::ReRegister(int m_nRegRobotIndex)
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if(m_bRbtPosRegFinished[m_nRegRobotIndex])
	{
		CString index;
		index.Format(_T("%d"), m_nRegRobotIndex);

		CString warning = _T("确定要重新标定位置");
		warning += index;
		warning += _T("吗？[Y/N]");

		int ret = AfxMessageBox(warning,MB_YESNO);
		if(ret == IDNO)
			return;
	}

	m_bRbtPosRegFinished[m_nRegRobotIndex] = FALSE;
	pDoc->m_bRbtManualRegFinised = FALSE;
	KillTimer(REG_RBT_MARKER_TIMER);
	UpdateRegStatus();
	for (int i=0;i<3;i++)
	{
		m_regRobotMarkerPos[m_nRegRobotIndex][i] = 0.0;
	}
}

void CRobotRegView::SetBkColor(CStatic &m_Pic, COLORREF m_Color)
{
	CRect rect;
	m_Pic.GetWindowRect(&rect);
	CRect picrct;
	picrct.top = 0;
	picrct.left = 0;
	picrct.bottom = rect.Height();
	picrct.right = rect.Width();

	CBrush brs;
	brs.CreateSolidBrush(m_Color);

	CDC *pDC = m_Pic.GetDC();
	pDC->FillRect(&picrct, &brs);
	m_Pic.ReleaseDC(pDC);
}


void CRobotRegView::OnBnClickedBtRmgRegisterAll()
{
	// TODO: Add your control notification handler code here

	int ret = AfxMessageBox(_T("是否确定重新注册？[Y/N]"),MB_YESNO);
	if(ret == IDNO)
		return;

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

// 	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
// 	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();

// 	double tcpPos1[3];
// 	for (int i=0;i<3;i++)
// 	{
// 		tcpPos1[i] = theApp.m_pSetting->m_robotParam.tcpMarker[i];
// 	}
// 	if(OK != pDoc->m_pRobotCtrl->SetTcpPos(tcpPos1))
// 	{
// 		AfxMessageBox(_T("机械臂参数设置失败！"));
// 		return;
// 	}

	for (int i=0;i<4;i++)
	{
		m_bRbtPosRegFinished[i] = FALSE;
	}
	pDoc->m_bRbtManualRegFinised = FALSE;

	for (int i=0;i<4;i++)
		for (int j=0;j<3;j++)
			m_regRobotMarkerPos[i][j] = 0.0;

	GetDlgItem(IDC_BT_MOVE_TO_POS1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BT_MOVE_TO_POS2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BT_MOVE_TO_POS3)->EnableWindow(TRUE);
	GetDlgItem(IDC_BT_MOVE_TO_POS4)->EnableWindow(TRUE);
	UpdateRegStatus();

	m_nRegRobotIndex = 0;
	m_nRegPointCounter = 0;

		
}


void CRobotRegView::OnBnClickedBtRmgAutoToZero()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	int ret =AfxMessageBox(_T("确保安全，是否回原点位置?[Y/N]"),MB_YESNO);
	if(ret == IDNO)
		return;
	double target[6];
	for (int i=0;i<6;i++)
	{
		target[i] = m_rbtInitialPos_1[i]/180.0*WK_PI;
	}

	if(pDoc->RbtCtrller->MoveRobotJoint(target) != 0)
		AfxMessageBox(_T("原点位置定位失败，检查机械臂是否解锁！"));

	pDoc->RbtCtrller->SetRobotMoveSpeed(0.2); 
}


void CRobotRegView::OnBnClickedBtRmgStopRobot()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if(pDoc->RbtCtrller->MoveRobotStop() != 0)
		AfxMessageBox(_T("机械臂停止运动失败！"));
}


BOOL CRobotRegView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	static bool freeRbtMode = false;
	if(pMsg->message == WM_LBUTTONDOWN)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_BT_RMG_FREE_DRIVE)->m_hWnd)
		{
			pDoc->RbtCtrller->SetRobotFreeDriveMode();
			freeRbtMode = true;
			//if (ROBOT_WORKING_CORRECT != RbtCtrller->SetRobotFreeDriveMode())
			//AfxMessageBox("自由运动失败！请重试！");	
		}
	}

	if(pMsg->message == WM_LBUTTONUP && freeRbtMode)
	{
// 		if(pMsg->hwnd == GetDlgItem(IDC_BT_RMG_FREE_DRIVE)->m_hWnd)
// 		{
			// 按钮弹起
			pDoc->RbtCtrller->SetRobotRunningMode();
			//if (ROBOT_WORKING_CORRECT != RbtCtrller->SetRobotRunningMode())
			//AfxMessageBox("停止自由运动失败！请重试！");

		//}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CRobotRegView::CheckRegAccuracy()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	double pos[3];
	double rotateMat[9];
	int ret;
	ret = pDoc->m_Cam->getMarkerPosByName(markerName,pos,rotateMat);
	if(!ret) return;

	if(m_nRegPointCounter < REG_CAP_POINT_NUM)
	{
		m_regRobotMarkerPtArr[m_nRegPointCounter][0] = pos[0];
		m_regRobotMarkerPtArr[m_nRegPointCounter][1] = pos[1];
		m_regRobotMarkerPtArr[m_nRegPointCounter][2] = pos[2];

		m_nRegPointCounter++;
	}
	else
	{
		KillTimer(CHECK_ACCURACY_TIME);

		/// 计算四个机械臂位置平均值
		double sumXX,sumYY,sumZZ;
		for (int i=0;i<REG_CAP_POINT_NUM;i++)
		{
			m_checkAccCameraPos[0] += m_regRobotMarkerPtArr[i][0];
			m_checkAccCameraPos[1] += m_regRobotMarkerPtArr[i][1];
			m_checkAccCameraPos[2] += m_regRobotMarkerPtArr[i][2];
		}

		m_checkAccCameraPos[0] /= REG_CAP_POINT_NUM;
		m_checkAccCameraPos[1] /= REG_CAP_POINT_NUM;
		m_checkAccCameraPos[2] /= REG_CAP_POINT_NUM;


		/// 计算标准差
		sumXX = 0.0;
		sumYY = 0.0;
		sumZZ = 0.0;

		for (int i=0;i<REG_CAP_POINT_NUM;i++)
		{
			sumXX += (m_regRobotMarkerPtArr[i][0] - m_checkAccCameraPos[0])*(m_regRobotMarkerPtArr[i][0] - m_checkAccCameraPos[0]);
			sumYY += (m_regRobotMarkerPtArr[i][1] - m_checkAccCameraPos[1])*(m_regRobotMarkerPtArr[i][1] - m_checkAccCameraPos[1]);
			sumZZ += (m_regRobotMarkerPtArr[i][2] - m_checkAccCameraPos[2])*(m_regRobotMarkerPtArr[i][2] - m_checkAccCameraPos[2]);
		}


		double varX,varY,varZ;
		varX = sqrt(sumXX/REG_CAP_POINT_NUM);
		varY = sqrt(sumYY/REG_CAP_POINT_NUM);
		varZ = sqrt(sumZZ/REG_CAP_POINT_NUM);

		if(varX > pDoc->markRegVarThreshold || varY > pDoc->markRegVarThreshold || varZ > pDoc->markRegVarThreshold)
		{
			m_nRegPointCounter = 0;                // 置0，重新开始采集

			for(int i=0;i<3;i++)
			{
				m_checkAccCameraPos[i] = 0.0;
			}

			SetTimer(CHECK_ACCURACY_TIME,70,NULL);
			return;
		}
		else
		{
			m_nRegPointCounter = 0;

			/////////////开始验证精度
			double tranX,tranY,tranZ;       /// 空间映射的坐标
			tranX = pDoc->m_regMtxRbt2Cam[0][0]*m_checkAccRobotPos[0] + pDoc->m_regMtxRbt2Cam[0][1]*m_checkAccRobotPos[1] + pDoc->m_regMtxRbt2Cam[0][2]*m_checkAccRobotPos[2] + pDoc->m_regMtxRbt2Cam[0][3];
			tranY = pDoc->m_regMtxRbt2Cam[1][0]*m_checkAccRobotPos[0] + pDoc->m_regMtxRbt2Cam[1][1]*m_checkAccRobotPos[1] + pDoc->m_regMtxRbt2Cam[1][2]*m_checkAccRobotPos[2] + pDoc->m_regMtxRbt2Cam[1][3];
			tranZ = pDoc->m_regMtxRbt2Cam[2][0]*m_checkAccRobotPos[0] + pDoc->m_regMtxRbt2Cam[2][1]*m_checkAccRobotPos[1] + pDoc->m_regMtxRbt2Cam[2][2]*m_checkAccRobotPos[2] + pDoc->m_regMtxRbt2Cam[2][3];

			double err;
			err = sqrt( pow((tranX-m_checkAccCameraPos[0]),2) + pow((tranY-m_checkAccCameraPos[1]),2) + pow((tranZ-m_checkAccCameraPos[2]),2) );

			CString errStr;
			errStr.Format(_T("%.5f"),err);
// 			SetDlgItemText(IDC_ET_RBT_REG_ERR,errStr);
// 			GetDlgItem(IDC_BT_ROBOT_REG_ACURRY_CHECK)->EnableWindow(TRUE);
		}
	}
}


void CRobotRegView::RegisterRobot()
{

	if(m_bRbtPosRegFinished[0] && m_bRbtPosRegFinished[1] && m_bRbtPosRegFinished[2]
	&& m_bRbtPosRegFinished[3])
	{
		CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
		ASSERT(pFrm != NULL && pDoc != NULL);

		FILE* rbtMarker4Pos;
		rbtMarker4Pos = fopen("rbtMarker4Pos.txt","w");
		if (rbtMarker4Pos==NULL)
		{
			return;
		}
		for (int i=0; i<4; i++)
		{
			for (int j=0; j<3; j++)
			{
				fprintf(rbtMarker4Pos,"%f  ", m_regRobotTipPos[i][j]);
			}
			fprintf(rbtMarker4Pos,"\n");
		}
		
		fclose(rbtMarker4Pos);

		FILE* camMarker4Pos;
		camMarker4Pos = fopen("camMarker4Pos.txt","w");
		if (camMarker4Pos==NULL)
		{
			return;
		}
		for (int i=0; i<4; i++)
		{
			for (int j=0; j<3; j++)
			{
				fprintf(camMarker4Pos,"%f  ", m_regRobotMarkerPos[i][j]);
			}
			fprintf(camMarker4Pos,"\n");
		}

		fclose(camMarker4Pos);

		float err1,err2;
		float errArr1[4]={0.0,0.0,0.0,0.0},errArr2[4]={0.0,0.0,0.0,0.0};
		err1 = pDoc->SolveTransMatrix_Cam2Rbt(m_regRobotMarkerPos,m_regRobotTipPos,pDoc->m_regMtxCam2Rbt,errArr1);
		err2 = pDoc->SolveTransMatrix_Rbt2Cam(m_regRobotMarkerPos,m_regRobotTipPos,pDoc->m_regMtxRbt2Cam,errArr2);
		if(err1 > err2)
		{
			CLibMatrix martix;
			double matrixT[4][4];
			for(int i=0;i<4;i++)
				for (int j=0;j<4;j++)
					martix.SetElement(matrixT,i,j,pDoc->m_regMtxRbt2Cam[i][j]);

			martix.couInv4x4(matrixT);
			for(int i=0;i<4;i++)
				for (int j=0;j<4;j++)
					pDoc->m_regMtxCam2Rbt[i][j] = matrixT[i][j];
		}
		else
		{
			CLibMatrix martix;
			double matrixT[4][4];
			for(int i=0;i<4;i++)
				for (int j=0;j<4;j++)
					martix.SetElement(matrixT,i,j,pDoc->m_regMtxCam2Rbt[i][j]);

			martix.couInv4x4(matrixT);
			for(int i=0;i<4;i++)
				for (int j=0;j<4;j++)
					pDoc->m_regMtxRbt2Cam[i][j] = matrixT[i][j];
		}

		/////////// 记录相机空间与机械臂空间转换矩阵
// 		wkCommon log;
// 		CString filePath = pDoc->m_strDBFilePath + _T("\\") + TRANSFORM_MATRIX_DIR;
// 		CString strWritten = _T("");
// 		CString mtxFilePath;
// 		for (int i=0;i<4;i++)
// 		{
// 			for(int j=0;j<4;j++)
// 			{
// 				CString tmpStr;
// 				tmpStr.Format(_T("%.5f "),pDoc->m_regMtxRbt2Cam[i][j]);
// 				strWritten += tmpStr;
// 			}
// 			strWritten += '\n';
// 		}
// 		mtxFilePath = filePath + _T("\\") + _T("regMtxRbt2Cam.txt");
// 		log.RecordInfoTrunc(mtxFilePath,strWritten);
// 
// 		strWritten = _T("");
// 		for (int i=0;i<4;i++)
// 		{
// 			for(int j=0;j<4;j++)
// 			{
// 				CString tmpStr;
// 				tmpStr.Format(_T("%.5f "),pDoc->m_regMtxCam2Rbt[i][j]);
// 				strWritten += tmpStr;
// 			}
// 			strWritten += '\n';
// 		}
// 		mtxFilePath = filePath + _T("\\") + _T("regMtxCam2Rbt.txt");
// 		log.RecordInfoTrunc(mtxFilePath,strWritten);



		pDoc->m_bRbtManualRegFinised = TRUE;
		UpdateRegStatus();

		float averErr1 = 0, averErr2 = 0;
		averErr1 = (errArr1[0] + errArr1[1] + errArr1[2] + errArr1[3])/4;
		averErr2 = (errArr2[0] + errArr2[1] + errArr2[2] + errArr2[3])/4;
		CString errStr;
		if (err1 > err2)
			errStr.Format(_T("机械臂注册完成，4点注册精度为：%.2f %.2f %.2f %.2f （%.2f） "),errArr2[0],errArr2[1],errArr2[2],errArr2[3],averErr2);
		else
			errStr.Format(_T("机械臂注册完成，4点注册精度为：%.2f %.2f %.2f %.2f （%.2f） "),errArr1[0],errArr1[1],errArr1[2],errArr1[3],averErr1);
		
		AfxMessageBox(errStr);
		

	

// 		if(err1>20&&err2>20)
// 		{
// 			CString errStr;
// 			errStr.Format(_T("机械臂注册完成，注册精度为：%.5f."),err2>err1?err1:err2);
// 			AfxMessageBox(errStr);
// 		}
// 		else
// 			ComputeRobotRegistrationAccurary();

		// 		CString errStr;
		// 		if(err1>err2)
		// 		{
		// 			errStr.Format(_T("机械臂注册完成，注册精度为：%.4f."),err2);
		// 			AfxMessageBox(errStr);
		// 		}
		// 		else
		// 		{
		// 			errStr.Format(_T("机械臂注册完成，注册精度为：%.4f."),err1);
		// 			AfxMessageBox(errStr);
		// 		}
		//AfxMessageBox(_T("机械臂注册完成，请验证精度！"));
	}


}


void CRobotRegView::ComputeRobotRegistrationAccurary()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	/////////////开始验证精度
	double tranX,tranY,tranZ;       /// 空间映射的坐标
	tranX = pDoc->m_regMtxCam2Rbt[0][0]*m_accuracyTestPoint[0] + pDoc->m_regMtxCam2Rbt[0][1]*m_accuracyTestPoint[1] + pDoc->m_regMtxCam2Rbt[0][2]*m_accuracyTestPoint[2] + pDoc->m_regMtxCam2Rbt[0][3];
	tranY = pDoc->m_regMtxCam2Rbt[1][0]*m_accuracyTestPoint[0] + pDoc->m_regMtxCam2Rbt[1][1]*m_accuracyTestPoint[1] + pDoc->m_regMtxCam2Rbt[1][2]*m_accuracyTestPoint[2] + pDoc->m_regMtxCam2Rbt[1][3];
	tranZ = pDoc->m_regMtxCam2Rbt[2][0]*m_accuracyTestPoint[0] + pDoc->m_regMtxCam2Rbt[2][1]*m_accuracyTestPoint[1] + pDoc->m_regMtxCam2Rbt[2][2]*m_accuracyTestPoint[2] + pDoc->m_regMtxCam2Rbt[2][3];

	double err;
	err = sqrt( pow((tranX-m_regRobotTipPos[m_lastRegPointIndex][0]),2) + pow((tranY-m_regRobotTipPos[m_lastRegPointIndex][1]),2) + pow((tranZ-m_regRobotTipPos[m_lastRegPointIndex][2]),2) );

	CString errStr;
	errStr.Format(_T("机械臂注册完成，注册精度为%.5f"),err);
	AfxMessageBox(errStr);

}

BOOL CRobotRegView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//UpdateRegStatus();
	SetTimer(UPDATE_BUTTON_TIMER,70,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CRobotRegView::OnBnClickedBtPrRbtUnlock()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if( pDoc->RbtCtrller->InitialRobot(NULL) == 0)
	{
		pDoc->RbtCtrller->SetProtectedStopUnLock();
		pDoc->RbtCtrller->RobotPowerOn();
		pDoc->RbtCtrller->RobotBrakeRelease();
		Sleep(200);
		pDoc->RbtCtrller->SetRobotMoveSpeed(0.2);     /// 设置机械臂初始速度
		AfxMessageBox(_T("机械臂已解锁！"));
	}
}


void CRobotRegView::OnBnClickedBtPrShutDown()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if (!pDoc->RbtCtrller->m_bRobotTimeCheckThreadRunning)
	{
		AfxMessageBox(_T("机械臂未解锁！"));
		return;
	}

	int ret = AfxMessageBox(_T("确定要关闭机械臂吗？[Y/N]"),MB_YESNO);
	if(ret != IDYES)
		return;
	if(pDoc->RbtCtrller->RobotPowerDown()!=0)
		AfxMessageBox(_T("机械臂关机失败！"));
}
