// ComTest.cpp : implementation file
//

#include "stdafx.h"
#include "RbtCamTest.h"
#include "ComTest.h"
#include "afxdialogex.h"
#include "RbtCamTestDoc.h"
#include "MainFrm.h"
#include<math.h>

// CComTest dialog

IMPLEMENT_DYNAMIC(CComTest, CPropertyPage)

CComTest::CComTest()
	: CPropertyPage(CComTest::IDD)
{

	isRegSucc = false;

}

CComTest::~CComTest()
{
}

void CComTest::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LEFT_IMG, m_leftPicture);
	DDX_Control(pDX, IDC_RIGHT_IMG, m_rightPicture);
	DDX_Control(pDX, IDC_COMBO_ANGLE, m_angle);
	DDX_Control(pDX, IDC_COMBO_TARGET, m_target);
	DDX_Control(pDX, IDC_COMBO_MARKER_TEMP, m_MarkerTemp);
	DDX_Control(pDX, IDC_COMBO_REGESTER_METHOD, m_RegesterMehthod);
}


BEGIN_MESSAGE_MAP(CComTest, CPropertyPage)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REG_TARGET, &CComTest::OnBnClickedButtonRegTarget)
	ON_BN_CLICKED(IDC_BUTTON_MOVETO_TARGET, &CComTest::OnBnClickedButtonMovetoTarget)
//	ON_BN_CLICKED(IDC_RECORD_THEORY_COORD, &CComTest::OnBnClickedRecordTheoryCoord)
	ON_BN_CLICKED(IDC_RECORD_ACTUAL_COORD, &CComTest::OnBnClickedRecordActualCoord)
//	ON_BN_CLICKED(IDC_BUTTON_SAVE_DATA, &CComTest::OnBnClickedButtonSaveData)
//	ON_BN_CLICKED(IDC_BUTTON_PROC_DATA, &CComTest::OnBnClickedButtonProcData)
ON_BN_CLICKED(IDC_BUTTON_SAVE_DATA, &CComTest::OnBnClickedButtonSaveData)
ON_BN_CLICKED(IDC_BUTTON_LAST_MOVE, &CComTest::OnBnClickedButtonLastMove)
END_MESSAGE_MAP()


// CComTest message handlers


void CComTest::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CPropertyPage::OnPaint() for painting messages

}


void CComTest::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case DISPLAY:
		drawToPics();
		//UpdateWindow();
		break;
	case REG_RBT_MARKER_TIMER:
		getInvertPos();
		break;
	default:
		break;
	}

	CPropertyPage::OnTimer(nIDEvent);
}
void CComTest::drawToPics()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	
	ASSERT(pFrm != NULL && pDoc != NULL);

	if (!pDoc->m_Cam->isCameraOpened)
	{
		CRect prect;
		m_leftPicture.GetClientRect(&prect);   //获取区域
		FillRect(m_leftPicture.GetDC()->GetSafeHdc(),&prect,CBrush(RGB(0,0,0)));     //填充颜色

		m_rightPicture.GetClientRect(&prect);   //获取区域
		FillRect(m_rightPicture.GetDC()->GetSafeHdc(),&prect,CBrush(RGB(0,0,0)));     //填充颜色

		return;

	}

	CDC *pDC;
	CRect rect;
	
	pDC = m_leftPicture.GetDC();
	if (pDC == NULL)
		return;
	
	pDoc->m_Cam->setMTDC(pDC);
	pDoc->m_Cam->processCurrFrame();

	m_leftPicture.GetClientRect(rect);
	pDoc->m_Cam->DrawToPic(pDC,&rect,false,0);
	this->ReleaseDC(pDC);


	pDC = m_rightPicture.GetDC();
	if (pDC == NULL)
		return;
	pDoc->m_Cam->setMTDC(pDC);
	pDoc->m_Cam->processCurrFrame();

	m_rightPicture.GetClientRect(rect);
	pDoc->m_Cam->DrawToPic(pDC,&rect,false,1);
	this->ReleaseDC(pDC);

}

BOOL CComTest::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	CString temp;

	m_RegesterMehthod.AddString(_T("仿射变换(Affine)"));
	m_RegesterMehthod.AddString(_T("奇异值分解(SVD)"));
	
	m_MarkerTemp.AddString(_T("a"));
	m_MarkerTemp.AddString(_T("TTBlock"));

	for (int i=0; i<5; i++)
	{
		temp.Format(_T("%d"),i);
		m_target.AddString(temp);
	}

	for (int i=10; i<100; i=i+10)
	{
		temp.Format(_T("%d"),i);
		temp += _T("°");
		m_angle.AddString(temp);
	}

	m_RegesterMehthod.SetCurSel(0);
	m_MarkerTemp.SetCurSel(1);
	m_target.SetCurSel(0);
	m_angle.SetCurSel(8);

// 	((CButton *)GetDlgItem(IDC_BUTTON_SAVE_DATA))->EnableWindow(FALSE);
// 	((CButton *)GetDlgItem(IDC_BUTTON_PROC_DATA))->EnableWindow(FALSE);

	((CComboBox *)GetDlgItem(IDC_COMBO_REGESTER_METHOD))->EnableWindow(FALSE);
	((CComboBox *)GetDlgItem(IDC_COMBO_MARKER_TEMP))->EnableWindow(FALSE);
	((CComboBox *)GetDlgItem(IDC_COMBO_TARGET))->EnableWindow(FALSE);
	((CComboBox *)GetDlgItem(IDC_COMBO_ANGLE))->EnableWindow(FALSE);

	SetTimer(DISPLAY,30,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CComTest::OnBnClickedButtonRegTarget()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);
	
	if (!pDoc->m_Cam->isCameraOpened)
	{
		AfxMessageBox("未发现摄像头或者摄像头打开失败！");
		return;
	}

	/*if (!pDoc->RbtCtrller->m_Socket->m_bClientSocketConnecting)
	{
	AfxMessageBox("未连接机器人！");
	return;
	}
	*/
	((CButton *)GetDlgItem(IDC_BUTTON_REG_TARGET))->EnableWindow(FALSE);

	SetTimer(REG_RBT_MARKER_TIMER,30,NULL);

}

void CComTest::getInvertPos()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	//bool ret = pDoc->m_Cam->getAveMarkerPosByName(pDoc->testMarkerName,targetPt,markerRotateMat);
	static int samplingFailTimes = 0;
	static int samplingSuccTimes = 0;
	static double ptTargetSum[4][3] = {0};
	static double centerI[REG_CAP_POINT_NUM][3] = {0};
	static double centerSum[3] = {0};
	static double rotateMatSum[9] = {0};

	if (samplingFailTimes > REG_CAP_POINT_NUM)
	{
		KillTimer(REG_RBT_MARKER_TIMER);
		
		samplingFailTimes = 0;
		samplingSuccTimes = 0;

		for (int i=0; i<4; i++)
			for (int j=0; j<3; j++)
				ptTargetSum[i][j] = 0;

		for (int i=0; i<REG_CAP_POINT_NUM; i++)
			for (int j=0; j<3; j++)
				centerI[i][j] = 0.0;

		for(int i=0; i<3; i++)
			centerSum[i] = 0;

		for (int i=0; i<9; i++)
			rotateMatSum[i] = 0;

		((CButton *)GetDlgItem(IDC_BUTTON_REG_TARGET))->EnableWindow(TRUE);
		AfxMessageBox("未找到靶点marker，注册失败！");
		return;
	}
	double ptTarget[4][3] = {0};
	double center[3] = {0};
	double rotateMat[9] = {0};
	int nIndex = m_MarkerTemp.GetCurSel();
	m_MarkerTemp.GetLBText( nIndex, pDoc->testMarkerName);

	bool ret = pDoc->m_Cam->getMarkerVectorAndPose(pDoc->testMarkerName, ptTarget, center, rotateMat);

	if (!ret)
	{
		samplingSuccTimes = 0;
		samplingFailTimes++;
		return;
	}

	if(samplingSuccTimes < REG_CAP_POINT_NUM)
	{

		for (int i=0; i<4; i++)
			for (int j=0; j<3; j++)
				ptTargetSum[i][j] += ptTarget[i][j];

		for(int i=0; i<3; i++)
		{
			centerI[samplingSuccTimes][i] = center[i];
			centerSum[i] += center[i];
		}

		for (int i=0; i<9; i++)
			rotateMatSum[i] += rotateMat[i];

		samplingSuccTimes++;
		samplingFailTimes = 0;

	}
	else
	{
		KillTimer(REG_RBT_MARKER_TIMER);
		((CButton *)GetDlgItem(IDC_BUTTON_REG_TARGET))->EnableWindow(TRUE);
		

		for (int i=0; i<4; i++)
			for (int j=0; j<3; j++)
				ptTargetSum[i][j] /= samplingSuccTimes;

		for(int i=0; i<3; i++)
			centerSum[i] /= samplingSuccTimes;

		for (int i=0; i<9; i++)
			rotateMatSum[i] /= samplingSuccTimes;

		for (int i=0; i<9; i++)
			markerRotateMat[i] = rotateMatSum[i];

		
		/// 计算标准差

		double sumXX,sumYY,sumZZ;
		sumXX = 0.0;
		sumYY = 0.0;
		sumZZ = 0.0;

		for (int i=0;i<REG_CAP_POINT_NUM;i++)
		{
			sumXX += (centerI[i][0] - centerSum[0])*(centerI[i][0] - centerSum[0]);
			sumYY += (centerI[i][1] - centerSum[1])*(centerI[i][1] - centerSum[1]);
			sumZZ += (centerI[i][2] - centerSum[2])*(centerI[i][2] - centerSum[2]);
		}

		double varX,varY,varZ;
		varX = sqrt(sumXX/REG_CAP_POINT_NUM);
		varY = sqrt(sumYY/REG_CAP_POINT_NUM);
		varZ = sqrt(sumZZ/REG_CAP_POINT_NUM);

		if(varX > pDoc->markRegVarThreshold || varY > pDoc->markRegVarThreshold || varZ > pDoc->markRegVarThreshold)
		{
			samplingSuccTimes = 0;                // 置0，重新开始采集
			samplingFailTimes = 0;

			for (int i=0; i<4; i++)
				for (int j=0; j<3; j++)
					ptTargetSum[i][j] = 0;

			for (int i=0; i<REG_CAP_POINT_NUM; i++)
				for (int j=0; j<3; j++)
					centerI[i][j] = 0.0;

			for(int i=0; i<3; i++)
				centerSum[i] = 0;

			for (int i=0; i<9; i++)
				rotateMatSum[i] = 0;

			return;
		}

		if (CheckIsExist(m_Target_Pos, centerSum[0],centerSum[1],centerSum[2]))
		{
			KillTimer(REG_RBT_MARKER_TIMER);

			samplingFailTimes = 0;
			samplingSuccTimes = 0;

			for (int i=0; i<4; i++)
				for (int j=0; j<3; j++)
					ptTargetSum[i][j] = 0;

			for (int i=0; i<REG_CAP_POINT_NUM; i++)
				for (int j=0; j<3; j++)
					centerI[i][j] = 0.0;

			for(int i=0; i<3; i++)
				centerSum[i] = 0;

			for (int i=0; i<9; i++)
				rotateMatSum[i] = 0;

			((CButton *)GetDlgItem(IDC_BUTTON_REG_TARGET))->EnableWindow(TRUE);
			AfxMessageBox("靶点已存在，请换其他位置！");

			return;

		
		}

		//记录数据
		for(int i=0; i<3; i++)
		{
			curTTBlockPos[i] = centerSum[i];
		}

		//计算靶点位置
		nIndex = m_target.GetCurSel();
		
		if (0 == nIndex)
		{
			for (int i =0; i<3; i++)	
				targetPt[i] = centerSum[i];
		}

		else
		{
			for (int i =0; i<3; i++)	
				targetPt[i] = ptTargetSum[nIndex - 1][i];
		}
		
		//数据归零
		samplingFailTimes = 0;
		samplingSuccTimes = 0;

		for (int i=0; i<4; i++)
			for (int j=0; j<3; j++)
				ptTargetSum[i][j] = 0;

		for(int i=0; i<3; i++)
			centerSum[i] = 0;

		for (int i=0; i<9; i++)
			rotateMatSum[i] = 0;


		markerPose[0] = markerRotateMat[0];
		markerPose[1] = markerRotateMat[3];
		markerPose[2] = markerRotateMat[6];
		markerPose[3] = targetPt[0];

		markerPose[4] = markerRotateMat[1];
		markerPose[5] = markerRotateMat[4];
		markerPose[6] = markerRotateMat[7];
		markerPose[7] = targetPt[1];

		markerPose[8] = markerRotateMat[2];
		markerPose[9] = markerRotateMat[5];
		markerPose[10] = markerRotateMat[8];
		markerPose[11] = targetPt[2];

		markerPose[12] = 0;
		markerPose[13] = 0;
		markerPose[14] = 0;
		markerPose[15] = 1;	
		
		nIndex = m_angle.GetCurSel();
		CString strCBText;
		m_angle.GetLBText( nIndex, strCBText);

		int angleIndex = 10;
		for (; angleIndex<100; angleIndex = angleIndex + 10)
		{
			CString temp;
			temp.Format("%d",angleIndex);
			temp += _T("°");

			if (temp == strCBText)
				break;
		}

		CLibMatrix martix;

		double DeltaMat[16] = {0};
		
		DeltaMat[0] = 1;
		DeltaMat[5] = 1;
		DeltaMat[10] = 1;
		DeltaMat[15] = 1;

		DeltaMat[3] = 0;
		DeltaMat[7] = 0;
		DeltaMat[11] = -30;

		double markerPoseDelta[16] = {0};
		martix.couArray(markerPose,DeltaMat,markerPoseDelta,4,4,4);

		for (int i=0; i<16; i++)
		{
			markerPose[i] = markerPoseDelta[i];
		}

		double rotateMat[16] = {cos(WK_PI/180*angleIndex),0,sin(-WK_PI/180*angleIndex),0,
								0,1,0,0,
								sin(WK_PI/180*angleIndex),0,cos(WK_PI/180*angleIndex),0,
								0,0,0,1};
		double marker2CamMat[16] = {0};
		martix.couArray(markerPose,rotateMat,marker2CamMat,4,4,4);

		nIndex = m_RegesterMehthod.GetCurSel();
		double cam2RbtMat[16];
		for (int i=0; i<4; i++)
			for (int j=0; j<4;j ++)
			{
				if(0 == nIndex)
					cam2RbtMat[i*4 + j] = pDoc->m_regMtxCam2Rbt[i][j];
				else
					cam2RbtMat[i*4 + j] = pDoc->m_regMtxCam2RbtSVD[i][j];
			}

		double marker2RbtMat[16];
		martix.couArray(cam2RbtMat,marker2CamMat,marker2RbtMat,4,4,4);

		double R[3][3];
		for (int i=0;i<3;i++)
			for (int j=0;j<3;j++)
				R[i][j] = marker2RbtMat[i*4 + j];

		////////根据旋转矩阵求解欧拉角，机械臂输入的欧拉角是绕固定系转XYZ
		double Rad_X,Rad_Y,Rad_Z;
		if(1-R[2][0]*R[2][0] != 0)
		{
			Rad_Y = atan2(-R[2][0],sqrt(R[0][0]*R[0][0]+R[1][0]*R[1][0]));////////单位是弧度
			Rad_X = atan2(R[2][1],R[2][2]);
			Rad_Z = atan2(R[1][0],R[0][0]);
		}
		else
		{
			if(R[2][0] == 1)
			{
				Rad_Y = -WK_PI/2;
				Rad_Z = 0;
				Rad_X = -atan2(R[0][1],R[1][1]);
			}
			else
			{
				Rad_Y = WK_PI/2;
				Rad_Z = 0;
				Rad_X = atan2(R[0][1],R[1][1]);
			}
		}

		double m_TargetRbtPos[6] = {0};

		m_TargetRbtPos[0] = marker2RbtMat[3];
		m_TargetRbtPos[1] = marker2RbtMat[7];
		m_TargetRbtPos[2] = marker2RbtMat[11];

		m_TargetRbtPos[3] = Rad_X;
		m_TargetRbtPos[4] = Rad_Y;
		m_TargetRbtPos[5] = Rad_Z;

		for (int i=0; i<3; i++)
		{
			m_tcpPin[i] = pDoc->tcpPin[i];
		}
		pDoc->RbtCtrller->SetTcpPos(m_tcpPin);
		for (int i=0; i<6; i++)
			m_curTargetJoint[i] = 0;
		/////////////////////// 求解机械臂反解 //////////////////////////////////
		if(pDoc->RbtCtrller->GetPosInverse(m_curTargetJoint,m_TargetRbtPos) != 0)
		{
			AfxMessageBox(_T("当前位置无解,靶点注册失败！"));
			isRegSucc = false;
		}
		else
		{
			//AfxMessageBox(_T("靶点注册成功！"));
			//Sleep(1000);
			ComptTarget();
			isRegSucc = true;
		}

		

	}

}

void CComTest::ComptTarget()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	markerPose[0] = markerRotateMat[0];
	markerPose[1] = markerRotateMat[3];
	markerPose[2] = markerRotateMat[6];
	markerPose[3] = targetPt[0];

	markerPose[4] = markerRotateMat[1];
	markerPose[5] = markerRotateMat[4];
	markerPose[6] = markerRotateMat[7];
	markerPose[7] = targetPt[1];

	markerPose[8] = markerRotateMat[2];
	markerPose[9] = markerRotateMat[5];
	markerPose[10] = markerRotateMat[8];
	markerPose[11] = targetPt[2];

	markerPose[12] = 0;
	markerPose[13] = 0;
	markerPose[14] = 0;
	markerPose[15] = 1;	

	int nIndex = m_angle.GetCurSel();
	CString strCBText;
	m_angle.GetLBText( nIndex, strCBText);

	int angleIndex = 10;
	for (; angleIndex<100; angleIndex = angleIndex + 10)
	{
		CString temp;
		temp.Format("%d",angleIndex);
		temp += _T("°");

		if (temp == strCBText)
			break;
	}

	CLibMatrix martix;

	double rotateMat[16] = {cos(WK_PI/180*angleIndex),0,sin(-WK_PI/180*angleIndex),0,
		0,1,0,0,
		sin(WK_PI/180*angleIndex),0,cos(WK_PI/180*angleIndex),0,
		0,0,0,1};
	double marker2CamMat[16] = {0};
	martix.couArray(markerPose,rotateMat,marker2CamMat,4,4,4);

	nIndex = m_RegesterMehthod.GetCurSel();
	double cam2RbtMat[16];
	for (int i=0; i<4; i++)
		for (int j=0; j<4;j ++)
		{
			if(0 == nIndex)
				cam2RbtMat[i*4 + j] = pDoc->m_regMtxCam2Rbt[i][j];
			else
				cam2RbtMat[i*4 + j] = pDoc->m_regMtxCam2RbtSVD[i][j];
		}

		double marker2RbtMat[16];
		martix.couArray(cam2RbtMat,marker2CamMat,marker2RbtMat,4,4,4);

		double R[3][3];
		for (int i=0;i<3;i++)
			for (int j=0;j<3;j++)
				R[i][j] = marker2RbtMat[i*4 + j];

		////////根据旋转矩阵求解欧拉角，机械臂输入的欧拉角是绕固定系转XYZ
		double Rad_X,Rad_Y,Rad_Z;
		if(1-R[2][0]*R[2][0] != 0)
		{
			Rad_Y = atan2(-R[2][0],sqrt(R[0][0]*R[0][0]+R[1][0]*R[1][0]));////////单位是弧度
			Rad_X = atan2(R[2][1],R[2][2]);
			Rad_Z = atan2(R[1][0],R[0][0]);
		}
		else
		{
			if(R[2][0] == 1)
			{
				Rad_Y = -WK_PI/2;
				Rad_Z = 0;
				Rad_X = -atan2(R[0][1],R[1][1]);
			}
			else
			{
				Rad_Y = WK_PI/2;
				Rad_Z = 0;
				Rad_X = atan2(R[0][1],R[1][1]);
			}
		}

		double m_TargetRbtPos[6] = {0};

		m_TargetRbtPos[0] = marker2RbtMat[3];
		m_TargetRbtPos[1] = marker2RbtMat[7];
		m_TargetRbtPos[2] = marker2RbtMat[11];

		m_TargetRbtPos[3] = Rad_X;
		m_TargetRbtPos[4] = Rad_Y;
		m_TargetRbtPos[5] = Rad_Z;

		for (int i=0; i<3; i++)
		{
			m_tcpPin[i] = pDoc->tcpPin[i];
		}
		pDoc->RbtCtrller->SetTcpPos(m_tcpPin);
		for (int i=0; i<6; i++)
			m_LastTargetJoint[i] = 0;
		/////////////////////// 求解机械臂反解 //////////////////////////////////
		if(pDoc->RbtCtrller->GetPosInverse(m_LastTargetJoint,m_TargetRbtPos) != 0)
		{
			AfxMessageBox(_T("当前位置无解,靶点注册失败！"));
			isRegSucc = false;
		}
		else
		{
			AfxMessageBox(_T("靶点注册成功！"));
			isRegSucc = true;
		}

}

void CComTest::OnBnClickedButtonMovetoTarget()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if (isRegSucc)
	{
		pDoc->RbtCtrller->MoveRobotJoint(m_curTargetJoint);
		pDoc->RbtCtrller->SetRobotMoveSpeed(0.2);
	}
	else
	{
		AfxMessageBox(_T("靶点未注册或者当前位置无解！"));
	}

	
}


void CComTest::OnBnClickedRecordActualCoord()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	(CButton *)GetDlgItem(IDC_RECORD_ACTUAL_COORD)->EnableWindow(FALSE);

	float *tcp = new float[3];
	for (int i=0; i<3; i++)
		m_tcpPin[i] = pDoc->tcpPin[i];
	
	pDoc->RbtCtrller->SetTcpPos(m_tcpPin);
	pDoc->RbtCtrller->GetTipData(tcp);

	
	bool isFind = false;
	vector<float *>::iterator it = m_Actural_TCP_Pos.begin();
	for (;it != m_Actural_TCP_Pos.end(); it++)
	{

		if (abs((*it)[0] - tcp[0]) < 10e-2 && 
			abs((*it)[1] - tcp[1]) < 10e-2 &&
			abs((*it)[2] - tcp[2]) < 10e-2 )
		{
			isFind = true;
			break;
		}
			
	}
	if (!isFind)
	{
		m_Actural_TCP_Pos.push_back(tcp);

		ofstream actural_TCP;
		static bool isFirstTimeOpenFile = true;
		if (isFirstTimeOpenFile)
		{
			actural_TCP.open("..//BIN//Data//actural_TCP.txt",ios::out);
			isFirstTimeOpenFile = false;
		}
		else
			actural_TCP.open("..//BIN//Data//actural_TCP.txt",ios::app);

		if (!actural_TCP.is_open())
		{
			AfxMessageBox(_T("文件打开失败！"));
			(CButton *)GetDlgItem(IDC_RECORD_ACTUAL_COORD)->EnableWindow(TRUE);
			return;
		}
		
		actural_TCP <<tcp[0]<<" "
					<<tcp[1]<<" "
					<<tcp[2]<<"\n";

		actural_TCP.close();

		AfxMessageBox("保存记录成功！");
	}
	else
		AfxMessageBox("与其他点位很近，保存失败！");

	(CButton *)GetDlgItem(IDC_RECORD_ACTUAL_COORD)->EnableWindow(TRUE);
		
		
}


//void CComTest::OnBnClickedRecordTheoryCoord()
//{
//	// TODO: Add your control notification handler code here
//	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
//	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
//	ASSERT(pFrm != NULL && pDoc != NULL);
//
//	(CButton *)GetDlgItem(IDC_RECORD_THEORY_COORD)->EnableWindow(FALSE);
//
//	float *tcp = new float[3];
//	for (int i=0; i<3; i++)
//		m_tcpPin[i] = pDoc->tcpPin[i];
//
//	pDoc->RbtCtrller->SetTcpPos(m_tcpPin);
//	pDoc->RbtCtrller->GetTipData(tcp);
//
//
//	bool isFind = false;
//	vector<float *>::iterator it = m_Theory_TCP_Pos.begin();
//	for (;it != m_Theory_TCP_Pos.end(); it++)
//	{
//		if (abs((*it)[0] - tcp[0]) < 10e-2 && 
//			abs((*it)[1] - tcp[1]) < 10e-2 &&
//			abs((*it)[2] - tcp[2]) < 10e-2 )
//		{
//			isFind = true;
//			break;
//		}
//
//	}
//	if (!isFind)
//	{
//		m_Theory_TCP_Pos.push_back(tcp);
//		ofstream theory_TCP;
//
//		static bool isFirstTimeOpenFile = true;
//		if (isFirstTimeOpenFile)
//		{
//			theory_TCP.open("..//BIN//Data//theory_TCP.txt",ios::out);
//			isFirstTimeOpenFile = false;
//		}
//		else
//			theory_TCP.open("..//BIN//Data//theory_TCP.txt",ios::app);
//
//		if (!theory_TCP.is_open())
//		{
//			AfxMessageBox(_T("文件打开失败！"));
//			(CButton *)GetDlgItem(IDC_RECORD_THEORY_COORD)->EnableWindow(TRUE);
//			return;
//		}
//
//		theory_TCP  <<tcp[0]<<" "
//					<<tcp[1]<<" "
//					<<tcp[2]<<"\n";
//
//		theory_TCP.close();
//
//		AfxMessageBox("保存记录成功！");
//
//	}
//	else
//		AfxMessageBox("与其他点位很近，保存失败！");
//	
//	(CButton *)GetDlgItem(IDC_RECORD_THEORY_COORD)->EnableWindow(TRUE);
//
//
//}


//void CComTest::OnBnClickedButtonSaveData()
//{
//	// TODO: Add your control notification handler code here
//	(CButton *)GetDlgItem(IDC_BUTTON_SAVE_DATA)->EnableWindow(FALSE);
//
//	ofstream actural_TCP, theory_TCP;
//	actural_TCP.open("..//BIN//Data//actural_TCP.txt",ios::out);
//	theory_TCP.open("..//BIN//Data//theory_TCP.txt",ios::out);
//
//	if (!actural_TCP.is_open() || !theory_TCP.is_open())
//	{
//		AfxMessageBox(_T("文件打开失败！"));
//		(CButton *)GetDlgItem(IDC_BUTTON_SAVE_DATA)->EnableWindow(TRUE);
//		return;
//	}
//	
//	vector<float *>::iterator itActural = m_Actural_TCP_Pos.begin();
//	vector<float *>::iterator itTheory = m_Theory_TCP_Pos.begin();
//	for (; itActural != m_Actural_TCP_Pos.end(); itActural++)
//	{
//		actural_TCP <<(*itActural)[0]<<" "
//					<<(*itActural)[1]<<" "
//					<<(*itActural)[2]
//					<<"\n";
//
//	}
//
//	for (; itTheory != m_Theory_TCP_Pos.end(); itTheory++)
//	{
//
//		theory_TCP  <<(*itTheory)[0]<<" "
//					<<(*itTheory)[1]<<" "
//					<<(*itTheory)[2]
//					<<"\n";
//
//	}
//	
//	actural_TCP.close();  
//	theory_TCP.close();  
//
//	(CButton *)GetDlgItem(IDC_BUTTON_SAVE_DATA)->EnableWindow(TRUE);
//
//}
//
//
//void CComTest::OnBnClickedButtonProcData()
//{
//	// TODO: Add your control notification handler code here
//	if(m_Actural_TCP_Pos.empty() || m_Theory_TCP_Pos.empty())
//		return;
//
//	vector<float *>::iterator itActural = m_Actural_TCP_Pos.begin();
//	vector<float *>::iterator itTheory = m_Theory_TCP_Pos.begin();
//
//	double MinError = 10e6, MaxError = 0.0, StdError = 0.0;
//	double disSum = 0;
//	int sampleNums = 0;
//	vector<double> disError;
//	for(;itActural != m_Actural_TCP_Pos.end(), itTheory != m_Theory_TCP_Pos.end(); itActural++ ,itTheory++)
//	{
//		//最大误差，标准差，平均误差
//		double dis = 0;
//		dis = ((*itActural)[0] - (*itTheory)[0]) * ((*itActural)[0] - (*itTheory)[0])
//			+ ((*itActural)[1] - (*itTheory)[1]) * ((*itActural)[1] - (*itTheory)[1])
//			+ ((*itActural)[2] - (*itTheory)[2]) * ((*itActural)[2] - (*itTheory)[2]);
//		dis = sqrt(dis);
//
//		disError.push_back(dis);
//		disSum += dis;
//
//		sampleNums++;
//
//		if (dis > MaxError)
//			MaxError = dis;
//
//		if (dis < MinError)
//			MinError = dis;
//		
//	}
//
//	double averError = disSum/sampleNums;
//	double stdDisSum = 0.0;
//	vector<double>::iterator itDis = disError.begin();
//	for (; itDis != disError.end(); itDis++)
//		stdDisSum = ((*itDis) - averError) * ((*itDis) - averError);
//	
//	if (sampleNums > 1)
//		StdError = sqrt(stdDisSum/(sampleNums - 1));
//	else
//		StdError = 0;
//	
//
//	ofstream errorData;
//	errorData.open("..//BIN//Data//errorData.txt",ios::out);
//
//	if (!errorData.is_open())
//	{
//		AfxMessageBox(_T("文件打开失败！"));
//		return;
//	}
//
//	errorData <<"最大误差："<<MinError<<"\n"
//		      <<"最小误差："<<MaxError<<"\n"
//			  <<"平均误差："<<averError<<"\n"
//	          <<"标准差："<<StdError<<"\n";
//
//	errorData.close();
//}


void CComTest::OnBnClickedButtonSaveData()
{
	// TODO: Add your control notification handler code here

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	((CButton *)GetDlgItem(IDC_BUTTON_SAVE_DATA))->EnableWindow(FALSE);

	//for (int i=0;i<3;i++)
	//{	
	//	m_Target_Pos.push_back(curTTBlockPos[i]);
	//}

	//记录摄像头数据
	FILE* CamTTBlockPoint;
	CamTTBlockPoint = fopen("..//BIN//Data//CamTTBlockPoint.txt","a+");
	if (CamTTBlockPoint == NULL)
		AfxMessageBox("写入CamTTBlockPoint.txt文件失败！");

	for (int i=0; i<3; i++)
	{
		fprintf(CamTTBlockPoint,"%f  ", curTTBlockPos[i]);
	}

	fprintf(CamTTBlockPoint,"\n");
	fclose(CamTTBlockPoint);

	//记录机械臂数据
	double curPose[6] = {0};
	double tcp[3] = {0};
	pDoc->RbtCtrller->SetTcpPos(tcp);
	if(pDoc->RbtCtrller->GetActualTcpPos(curPose) != 0)
	{
		AfxMessageBox(_T("未获取机器人坐标，请查看连接状态！"));
		((CButton *)GetDlgItem(IDC_BUTTON_SAVE_DATA))->EnableWindow(TRUE);

		return;
	}
	


	FILE* RbtPoint;
	RbtPoint = fopen("..//BIN//Data//RbtOriginalPoint.txt","a+");
	if (RbtPoint == NULL)
		AfxMessageBox("写入RbtOriginalPoint.txt文件失败！");

	for (int i=0; i<6; i++)
	{
		fprintf(RbtPoint,"%f  ", curPose[i]);
	}

	fprintf(RbtPoint,"\n");
	fclose(RbtPoint);

	((CButton *)GetDlgItem(IDC_BUTTON_SAVE_DATA))->EnableWindow(TRUE);
}


bool CComTest::CheckIsExist(vector<double> &m_data, double x, double y, double z)
{
	ASSERT(m_data.size() % 3 == 0);
	if (0 == m_data.size())
		return false;

	for (auto iter1 = m_data.begin(); iter1 != m_data.end(); iter1 = iter1+3)
	{
		if (abs((*iter1) - x) < 0.2
			&&abs((*(iter1+1)) - y) < 0.2
			&&abs((*(iter1+2)) - z) < 0.2)
			return true;
	}

	return false;

}

void CComTest::OnBnClickedButtonLastMove()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if (isRegSucc)
	{
		pDoc->RbtCtrller->MoveRobotJoint(m_LastTargetJoint);
		pDoc->RbtCtrller->SetRobotMoveSpeed(0.2);
	}
	else
	{
		AfxMessageBox(_T("靶点未注册或者当前位置无解！"));
	}

}
