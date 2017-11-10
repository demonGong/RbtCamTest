// RegisterRbt.cpp : implementation file
//

#include "stdafx.h"
#include "RbtCamTest.h"
#include "RegisterRbt.h"
#include "afxdialogex.h"
#include "RbtCamTestDoc.h"
#include "MainFrm.h"


// CRegisterRbt dialog

IMPLEMENT_DYNAMIC(CRegisterRbt, CPropertyPage)

CRegisterRbt::CRegisterRbt()
	: CPropertyPage(CRegisterRbt::IDD)
{
	m_clrRegFinished = RGB(0,128,0);
	m_clrRegUnfinished = RGB(128,0,0);

	for (int i=0; i<4; i++)
		m_bRbtPosRegFinished[i] = FALSE;

	m_nRegRobotIndex = 0;

	m_nRegPointCounter = 0;

	for (int i=0;i<4;i++)
		for(int j=0;j<3;j++)
		{
			m_regRobotMarkerPos[i][j] = 0.0;
		}
}

CRegisterRbt::~CRegisterRbt()
{
}

void CRegisterRbt::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RB_POS1_STATUS, m_stRbtPos1RegStatus);
	DDX_Control(pDX, IDC_RB_POS2_STATUS, m_stRbtPos2RegStatus);
	DDX_Control(pDX, IDC_RB_POS3_STATUS, m_stRbtPos3RegStatus);
	DDX_Control(pDX, IDC_RB_POS4_STATUS, m_stRbtPos4RegStatus);
}

BOOL CRegisterRbt::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	SetTimer(UPDATE_BUTTON_TIMER,70,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CRegisterRbt, CPropertyPage)
	ON_BN_CLICKED(IDC_BT_MOVE_TO_POS1, &CRegisterRbt::OnBnClickedBtMoveToPos1)
	ON_BN_CLICKED(IDC_BT_MOVE_TO_POS2, &CRegisterRbt::OnBnClickedBtMoveToPos2)
	ON_BN_CLICKED(IDC_BT_MOVE_TO_POS3, &CRegisterRbt::OnBnClickedBtMoveToPos3)
	ON_BN_CLICKED(IDC_BT_MOVE_TO_POS4, &CRegisterRbt::OnBnClickedBtMoveToPos4)
	ON_BN_CLICKED(IDC_BT_POS1_REREGISTER, &CRegisterRbt::OnBnClickedBtPos1Reregister)
	ON_BN_CLICKED(IDC_BT_POS2_REREGISTER, &CRegisterRbt::OnBnClickedBtPos2Reregister)
	ON_BN_CLICKED(IDC_BT_POS3_REREGISTER, &CRegisterRbt::OnBnClickedBtPos3Reregister)
	ON_BN_CLICKED(IDC_BT_POS4_REREGISTER, &CRegisterRbt::OnBnClickedBtPos4Reregister)
	ON_BN_CLICKED(IDC_BT_RMG_REGISTER_ALL, &CRegisterRbt::OnBnClickedBtRmgRegisterAll)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BT_LOAD_RESISTER, &CRegisterRbt::OnBnClickedBtLoadResister)
END_MESSAGE_MAP()


// CRegisterRbt message handlers


void CRegisterRbt::OnBnClickedBtMoveToPos1()
{
	// TODO: Add your control notification handler code here
	m_nRegRobotIndex = 0;

	if (!BtMoveToPos(m_nRegRobotIndex))
	{
		AfxMessageBox("机器人未连接！");
		return;
	}
	
	SetTimer(REG_RBT_MARKER_TIMER,70,NULL);
}


void CRegisterRbt::OnBnClickedBtMoveToPos2()
{
	// TODO: Add your control notification handler code here
	m_nRegRobotIndex = 1;

	if (!BtMoveToPos(m_nRegRobotIndex))
	{
		AfxMessageBox("机器人未连接！");
		return;
	}
	
	SetTimer(REG_RBT_MARKER_TIMER,70,NULL);
}


void CRegisterRbt::OnBnClickedBtMoveToPos3()
{
	// TODO: Add your control notification handler code here
	m_nRegRobotIndex = 2;

	if (!BtMoveToPos(m_nRegRobotIndex))
	{
		AfxMessageBox("机器人未连接！");
		return;
	}

	SetTimer(REG_RBT_MARKER_TIMER,70,NULL);
}


void CRegisterRbt::OnBnClickedBtMoveToPos4()
{
	// TODO: Add your control notification handler code here
	m_nRegRobotIndex = 3;

	if (!BtMoveToPos(m_nRegRobotIndex))
	{
		AfxMessageBox("机器人未连接！");
		return;
	}

	SetTimer(REG_RBT_MARKER_TIMER,70,NULL);
}


void CRegisterRbt::OnBnClickedBtPos1Reregister()
{
	// TODO: Add your control notification handler code here
	ReRegister(0);

	GetDlgItem(IDC_BT_MOVE_TO_POS1)->EnableWindow(TRUE);
}


void CRegisterRbt::OnBnClickedBtPos2Reregister()
{
	// TODO: Add your control notification handler code here
	ReRegister(1);

	GetDlgItem(IDC_BT_MOVE_TO_POS2)->EnableWindow(TRUE);
}


void CRegisterRbt::OnBnClickedBtPos3Reregister()
{
	// TODO: Add your control notification handler code here
	ReRegister(2);

	GetDlgItem(IDC_BT_MOVE_TO_POS3)->EnableWindow(TRUE);
}


void CRegisterRbt::OnBnClickedBtPos4Reregister()
{
	// TODO: Add your control notification handler code here
	ReRegister(3);

	GetDlgItem(IDC_BT_MOVE_TO_POS4)->EnableWindow(TRUE);
}

void CRegisterRbt::OnBnClickedBtRmgRegisterAll()
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

void CRegisterRbt::OnTimer(UINT_PTR nIDEvent)
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
	CPropertyPage::OnTimer(nIDEvent);
}

void CRegisterRbt::SetBkColor(CStatic &m_Pic, COLORREF m_Color)
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
	if (pDC == NULL)
		return;
	else
		pDC->FillRect(&picrct, &brs);
	
	m_Pic.ReleaseDC(pDC);
}

void CRegisterRbt::UpdateRegStatus()
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

bool CRegisterRbt::BtMoveToPos(int m_nRegRobotIndex)
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if (!pDoc->RbtCtrller->m_bPackParaAnalysisThreadRunning)
		return false;

	if(!pDoc->RbtCtrller->CheckJointMode(ROBOT_RUNNING_MODE))
		pDoc->RbtCtrller->SetRobotRunningMode();

	pDoc->RbtCtrller->SetTcpPos(pDoc->robotMarkerPos);

	double tipPos[6];
	if(pDoc->RbtCtrller->GetActualTcpPos(tipPos) != 0)
		return false;
	for (int i=0;i<3;i++)
	{
		m_regRobotTipPos[m_nRegRobotIndex][i] = tipPos[i];
	}
	return true;
}

void CRegisterRbt::CaptureRobotMarker()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	static int failCheckMarkerNums = 0;
	static double  m_regRobotMarkerPtArr[REG_CAP_POINT_NUM][3] = {0};  // 机械臂Marker在相机坐标系下坐标集合

	double pos[3];
	double rotateMat[9];
	int ret;
	if (!pDoc->m_Cam->isCameraOpened)
	{
		AfxMessageBox(_T("未发现摄像头！"));
		KillTimer(REG_RBT_MARKER_TIMER);
		return;
	}

	if (0 == m_nRegRobotIndex)
		GetDlgItem(IDC_BT_MOVE_TO_POS1)->EnableWindow(FALSE);
	else if (1 == m_nRegRobotIndex)
		GetDlgItem(IDC_BT_MOVE_TO_POS2)->EnableWindow(FALSE);
	else if (2 == m_nRegRobotIndex)
		GetDlgItem(IDC_BT_MOVE_TO_POS3)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_BT_MOVE_TO_POS4)->EnableWindow(FALSE);

	ret = pDoc->m_Cam->getMarkerPosByName(pDoc->robotMarkerName,pos,rotateMat);
	
	if (failCheckMarkerNums > REG_CAP_POINT_NUM)
	{
		KillTimer(REG_RBT_MARKER_TIMER);
		AfxMessageBox(_T("未找到机械臂marker！"));
		if (0 == m_nRegRobotIndex)
			GetDlgItem(IDC_BT_MOVE_TO_POS1)->EnableWindow(TRUE);
		else if (1 == m_nRegRobotIndex)
			GetDlgItem(IDC_BT_MOVE_TO_POS2)->EnableWindow(TRUE);
		else if (2 == m_nRegRobotIndex)
			GetDlgItem(IDC_BT_MOVE_TO_POS3)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BT_MOVE_TO_POS4)->EnableWindow(TRUE);
		failCheckMarkerNums = 0;
		
		return;
	}
	if(!ret) 
		failCheckMarkerNums++;
	else
	{

		failCheckMarkerNums = 0;

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
}

void CRegisterRbt::RegisterRobot()
{

	if(m_bRbtPosRegFinished[0] && m_bRbtPosRegFinished[1] && m_bRbtPosRegFinished[2]
	&& m_bRbtPosRegFinished[3])
	{
		CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
		ASSERT(pFrm != NULL && pDoc != NULL);

		FILE* rbtMarker4Pos;
		rbtMarker4Pos = fopen("..//BIN//Data//rbtMarker4Pos.txt","w");
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
		camMarker4Pos = fopen("..//BIN//Data//camMarker4Pos.txt","w");
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

		//SVD算法求解变换矩阵
		pDoc->SolveMapMatrixSUV(m_regRobotMarkerPos,m_regRobotTipPos,pDoc->m_regMtxCam2RbtSVD);

		//Affine算法求解变换矩阵
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

		pDoc->m_bRbtManualRegFinised = TRUE;
		UpdateRegStatus();

		CString errStr;
		if (err1 > err2)
			errStr.Format(_T("机械臂注册完成，4点注册精度为：%.2f %.2f %.2f %.2f （%.2f） "),errArr2[0],errArr2[1],errArr2[2],errArr2[3],err2);
		else
			errStr.Format(_T("机械臂注册完成，4点注册精度为：%.2f %.2f %.2f %.2f （%.2f） "),errArr1[0],errArr1[1],errArr1[2],errArr1[3],err1);

		AfxMessageBox(errStr);

		FILE* cam2RbtMat;
		cam2RbtMat = fopen("..//BIN//Data//cam2RbtMat.txt","w");
		if (cam2RbtMat == NULL)
			AfxMessageBox("写入cam2RbtMat.txt文件失败！");

		for (int i=0; i<4; i++)
		{
			for (int j=0; j<4; j++)
			{
				fprintf(cam2RbtMat,"%f  ", pDoc->m_regMtxCam2Rbt[i][j]);
			}
			fprintf(cam2RbtMat,"\n");
		}

		fprintf(cam2RbtMat,errStr);
		fclose(cam2RbtMat);

		FILE* cam2RbtMatSVD;
		cam2RbtMatSVD = fopen("..//BIN//Data//cam2RbtMatSVD.txt","w");
		if (cam2RbtMatSVD == NULL)
			AfxMessageBox("写入cam2RbtMatSVD.txt文件失败！");

		for (int i=0; i<4; i++)
		{
			for (int j=0; j<4; j++)
			{
				fprintf(cam2RbtMatSVD,"%f  ", pDoc->m_regMtxCam2RbtSVD[i][j]);
			}
			fprintf(cam2RbtMatSVD,"\n");
		}
		fclose(cam2RbtMatSVD);

	}


}

void CRegisterRbt::ReRegister(int m_nRegRobotIndex)
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if(m_bRbtPosRegFinished[m_nRegRobotIndex])
	{
		CString index;
		index.Format(_T("%d"), m_nRegRobotIndex+1);

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

void CRegisterRbt::CheckRegAccuracy()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	static double  m_regRobotMarkerPtArr[REG_CAP_POINT_NUM][3] = {0};  // 机械臂Marker在相机坐标系下坐标集合

	double pos[3];
	double rotateMat[9];
	int ret;
	ret = pDoc->m_Cam->getMarkerPosByName(pDoc->robotMarkerName,pos,rotateMat);
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

void CRegisterRbt::ComputeRobotRegistrationAccurary()
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

void CRegisterRbt::OnBnClickedBtLoadResister()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	char filePath[255] ="..//BIN//Data//cam2RbtMat.txt"; 
	// 	if (filePath == NULL)
	// 		return false;

	ifstream ifstr_data;
	//打开文件//
	ifstr_data.open(filePath, ios::in | ios::binary); 
	if (!ifstr_data.is_open())
		return;

	double temp;
	for(int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			ifstr_data >> temp;
			pDoc->m_regMtxCam2Rbt[i][j] = temp;
		}
	}
	
	//关闭文件//
	ifstr_data.close(); 
	
}
