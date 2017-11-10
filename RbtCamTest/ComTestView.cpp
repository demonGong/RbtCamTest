// ComTestView.cpp : implementation file
//

#include "stdafx.h"
#include "RbtCamTest.h"
#include "ComTestView.h"
#include "RbtCamTestDoc.h"
#include "MainFrm.h"


// ComTestView

IMPLEMENT_DYNCREATE(ComTestView, CFormView)

ComTestView::ComTestView()
	: CFormView(ComTestView::IDD)
{
	

}

ComTestView::~ComTestView()
{
}

void ComTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_RMG_FREE_DRIVE, m_btFreeDrive);
}

void ComTestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	static BOOL m_bFirstRun = TRUE;
	m_sheet.AddPage(&m_page1);   
	m_sheet.AddPage(&m_page2);
	m_sheet.Create(this,   WS_CHILD|WS_VISIBLE|WS_TABSTOP,   0);   
	m_sheet.SetWindowPos(NULL,0,0,400,410,NULL);

	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if (pDoc->m_Cam->isCameraOpened)
		((CButton *)GetDlgItem(IDC_BUTTON_SAMPLING_MARKER_DATA))->EnableWindow(TRUE);

	// 	ResizeParentToFit();   
	// 	if(!m_bFirstRun)  return;   //   定义一个变量，防止下面的程序多次运行   
	// 	m_sheet.AddPage(&m_page1);   
	// 	m_sheet.Create(this,   WS_CHILD|WS_VISIBLE|WS_TABSTOP,   0);   
	// 	m_sheet.ModifyStyleEx(0,WS_EX_CONTROLPARENT);
	// 	CRect   rect; 
	// 	GetClientRect(&rect);  //得到当前视图的大小
	// 	m_sheet.MoveWindow(rect);
	// 
	// 	m_sheet.GetTabControl()->MoveWindow(rect);
	// 	rect.left+=10;    //改变rect的大小，防止CPropertyPage盖住CPropertySheet
	// 	rect.top+=20;
	// 	rect.bottom-=10;
	// 	rect.right-=10;  
	// 	m_sheet.GetActivePage()->MoveWindow(rect); 
	// 	m_bFirstRun=FALSE;

}


BEGIN_MESSAGE_MAP(ComTestView, CFormView)
	ON_BN_CLICKED(IDC_BT_PR_SHUT_DOWN, &ComTestView::OnBnClickedBtPrShutDown)
	ON_BN_CLICKED(IDC_BT_PR_RBT_UNLOCK, &ComTestView::OnBnClickedBtPrRbtUnlock)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BT_RMG_AUTO_TO_ZERO, &ComTestView::OnBnClickedBtRmgAutoToZero)
	ON_BN_CLICKED(IDC_BT_RMG_STOP_ROBOT, &ComTestView::OnBnClickedBtRmgStopRobot)
	ON_BN_CLICKED(IDC_BUTTON_SAMPLING_MARKER_DATA, &ComTestView::OnBnClickedButtonSamplingMarkerData)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ComTestView diagnostics

#ifdef _DEBUG
void ComTestView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ComTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ComTestView message handlers

void ComTestView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// 	CRect   rect;     
	// 	GetClientRect(&rect);    
	// 	if(!(m_sheet)) return; //如果m_sheet还没有创建则不运行下面的程序  
	// 	m_sheet.MoveWindow(rect);  
	// 	m_sheet.GetTabControl()->MoveWindow(rect);  
	// 	rect.left+=10; //改变rect的大小，防止CPropertyPage盖住CPropertySheet  
	// 	rect.top+=20;  
	// 	rect.bottom-=10;  
	// 	rect.right-=10;  
	// 	m_sheet.GetActivePage()->MoveWindow(rect); 
}

void ComTestView::OnBnClickedBtPrShutDown()
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


void ComTestView::OnBnClickedBtPrRbtUnlock()
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

		AfxMessageBox(_T("机械臂已解锁！"));
	}
}

void ComTestView::OnBnClickedBtRmgAutoToZero()
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
		target[i] = pDoc->rbtInitialJointZero[i]/180.0*WK_PI;
	}

	if(pDoc->RbtCtrller->MoveRobotJoint(target) != 0)
		AfxMessageBox(_T("原点位置定位失败，检查机械臂是否解锁！"));

	pDoc->RbtCtrller->SetRobotMoveSpeed(0.2); 
}


void ComTestView::OnBnClickedBtRmgStopRobot()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	if(pDoc->RbtCtrller->MoveRobotStop() != 0)
		AfxMessageBox(_T("机械臂停止运动失败！"));
}




BOOL ComTestView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	int uid = LOWORD(wParam);

	if(LOWORD(wParam) == IDC_BT_RMG_FREE_DRIVE)
	{
		if( HIWORD(wParam) == WM_LBUTTONDOWN )
		{
			pDoc->RbtCtrller->SetRobotFreeDriveMode();
		}
		if(HIWORD(wParam) == WM_LBUTTONUP)
		{
			pDoc->RbtCtrller->SetRobotRunningMode();
		}
	}
	return CFormView::OnCommand(wParam, lParam);
}


void ComTestView::OnBnClickedButtonSamplingMarkerData()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	((CButton *)GetDlgItem(IDC_BUTTON_SAMPLING_MARKER_DATA))->EnableWindow(FALSE);

	SetTimer(REG_MARKER_TIMER,30,NULL);
	
}

void ComTestView::samplingMarkersPos()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	static int samplingFailTimes = 0;
	static int samplingSuccTimes = 0;
	static double ptTargetSum[2][4][3] = {0};
	static double centerSum[2][3] = {0};


	if(samplingFailTimes > REG_CAP_POINT_NUM)
	{
		KillTimer(REG_MARKER_TIMER);

		samplingFailTimes = 0;
		samplingSuccTimes = 0;

		for (int i=0; i<2; i++)
			for (int j=0; j<4; j++)
				for (int k=0; k<3; k++)
					ptTargetSum[i][j][k] = 0;

		for (int i=0; i<2; i++)
			for(int j=0; j<3; j++)
				centerSum[i][j] = 0;

		AfxMessageBox("未找到TTBlock面板，采集失败！");
		((CButton *)GetDlgItem(IDC_BUTTON_SAMPLING_MARKER_DATA))->EnableWindow(TRUE);
		return;
	}

	double ptTarget[2][4][3] = {0};
	double center[2][3] = {0};
	double rotateMat[2][9] = {0};

	bool retTTBlock = pDoc->m_Cam->getMarkerVectorAndPose(_T("TTBlock"), ptTarget[0], center[0], rotateMat[0]);
	bool retA = pDoc->m_Cam->getMarkerVectorAndPose(_T("a"), ptTarget[1], center[1], rotateMat[1]);

	if (!retTTBlock || !retA)
	{
		samplingFailTimes++;
		return;
	}

	if(samplingSuccTimes < REG_CAP_POINT_NUM)
	{
		samplingSuccTimes++;
		samplingFailTimes = 0;

		for (int i=0; i<4; i++)
			for (int j=0; j<3; j++)
			{
				ptTargetSum[0][i][j] += ptTarget[0][i][j];
				ptTargetSum[1][i][j] += ptTarget[1][i][j];
			}

			for(int i=0; i<3; i++)
			{
				centerSum[0][i] += center[0][i];
				centerSum[1][i] += center[1][i];
			}

		FILE* TTBlockPointsPos;
		TTBlockPointsPos = fopen("..//BIN//Data//CameraData//TTBlockPoints.txt","a+");
		if (TTBlockPointsPos == NULL)
			AfxMessageBox("写入TTBlockPoints.txt文件失败！");

		for (int i=0; i<4; i++)
		{
			for (int j=0; j<3; j++)
			{
				fprintf(TTBlockPointsPos,"%f  ", ptTarget[0][i][j]);
			}
			fprintf(TTBlockPointsPos,"\n");
		}

		for (int j=0; j<3; j++)
		{
			fprintf(TTBlockPointsPos,"%f  ", center[0][j]);
		}
		fprintf(TTBlockPointsPos,"\n");
		fprintf(TTBlockPointsPos,"\n");

		fclose(TTBlockPointsPos);

		FILE* APointsPos;
		APointsPos = fopen("..//BIN//Data//CameraData//APointsPos.txt","a+");
		if (APointsPos == NULL)
			AfxMessageBox("写入APointsPos.txt文件失败！");

		for (int i=0; i<4; i++)
		{
			for (int j=0; j<3; j++)
			{
				fprintf(APointsPos,"%f  ", ptTarget[1][i][j]);
			}
			fprintf(APointsPos,"\n");
		}

		for (int j=0; j<3; j++)
		{
			fprintf(APointsPos,"%f  ", center[1][j]);
		}
		fprintf(APointsPos,"\n");
		fprintf(APointsPos,"\n");

		fclose(APointsPos);
		

	}
	else
	{
		KillTimer(REG_MARKER_TIMER);

		for (int i=0; i<4; i++)
			for (int j=0; j<3; j++)
			{
				ptTargetSum[0][i][j] /= samplingSuccTimes;
				ptTargetSum[1][i][j] /= samplingSuccTimes;
			}

			for(int i=0; i<3; i++)
			{
				centerSum[0][i] /= samplingSuccTimes;
				centerSum[1][i] /= samplingSuccTimes;
			}

			double dis2Markers = 0;

			for (int i=0; i<3; i++)
				dis2Markers += (centerSum[0][i] - centerSum[1][i]) * (centerSum[0][i] - centerSum[1][i]);
			
			dis2Markers = sqrt(dis2Markers);

			FILE* dis2markerF;
			dis2markerF = fopen("..//BIN//Data//CameraData//dis2Markers.txt","a+");
			if (dis2markerF == NULL)
				AfxMessageBox("写入dis2Markers.txt文件失败！");

			fprintf(dis2markerF,"%f  ", dis2Markers);
			fprintf(dis2markerF,"\n");

			fclose(dis2markerF);


			double errorTT[6] = {0};

			for (int i=0; i<4; i++)
			{
				double disSum = 0;
				for (int j=0; j<3; j++)
				{
					disSum += (ptTargetSum[0][i][j] - centerSum[0][j]) * (ptTargetSum[0][i][j] - centerSum[0][j]);
				}
				if (i<2)
					errorTT[i] = sqrt(disSum) - 45;
				else
					errorTT[i] = sqrt(disSum) - 43;

			}

			for (int j=0; j<3; j++)
			{
				errorTT[4] += (ptTargetSum[0][0][j] - ptTargetSum[0][1][j]) * (ptTargetSum[0][0][j] - ptTargetSum[0][1][j]);
			}
			errorTT[4] = sqrt(errorTT[4]) - 90;

			for (int j=0; j<3; j++)
			{
				errorTT[5] += (ptTargetSum[0][2][j] - ptTargetSum[0][3][j]) * (ptTargetSum[0][2][j] - ptTargetSum[0][3][j]);
			}
			errorTT[5] = sqrt(errorTT[5]) - 86;

			double errorA[4] = {0};

			for (int i=0; i<2; i++)
			{
				double disSum = 0;
				for (int j=0; j<3; j++)
				{
					disSum += (ptTargetSum[1][i][j] - centerSum[1][j]) * (ptTargetSum[1][i][j] - centerSum[1][j]);
				}
				errorA[i] = sqrt(disSum) - 33;
			}

			for (int j=0; j<3; j++)
			{
				errorA[2] += (ptTargetSum[1][0][j] - ptTargetSum[1][1][j]) * (ptTargetSum[1][0][j] - ptTargetSum[1][1][j]);
			}
			errorA[2] = sqrt(errorA[2]) - 66;

			for (int j=0; j<3; j++)
			{
				errorA[3] += (ptTargetSum[1][2][j] - ptTargetSum[1][3][j]) * (ptTargetSum[1][2][j] - ptTargetSum[1][3][j]);
			}
			errorA[3] = sqrt(errorA[3]) - 47;


			FILE* averTTBlockPointsPos;
			averTTBlockPointsPos = fopen("..//BIN//Data//CameraData//averTTBlockPoints.txt","a+");
			if (averTTBlockPointsPos == NULL)
				AfxMessageBox("写入averTTBlockPoints.txt文件失败！");

			for (int i=0; i<4; i++)
			{
				for (int j=0; j<3; j++)
				{
					fprintf(averTTBlockPointsPos,"%f  ", ptTargetSum[0][i][j]);
				}
				fprintf(averTTBlockPointsPos,"\n");
			}

			for (int j=0; j<3; j++)
			{
				fprintf(averTTBlockPointsPos,"%f  ", centerSum[0][j]);
			}
			fprintf(averTTBlockPointsPos,"\n");
			fprintf(averTTBlockPointsPos,"\n");

			fclose(averTTBlockPointsPos);

			FILE* averAPointsPos;
			averAPointsPos = fopen("..//BIN//Data//CameraData//averAPoints.txt","a+");
			if (averTTBlockPointsPos == NULL)
				AfxMessageBox("写入averAPoints.txt文件失败！");

			for (int i=0; i<4; i++)
			{
				for (int j=0; j<3; j++)
				{
					fprintf(averAPointsPos,"%f  ", ptTargetSum[1][i][j]);
				}
				fprintf(averAPointsPos,"\n");
			}

			for (int j=0; j<3; j++)
			{
				fprintf(averAPointsPos,"%f  ", centerSum[1][j]);
			}
			fprintf(averAPointsPos,"\n");
			fprintf(averAPointsPos,"\n");

			fclose(averAPointsPos);


			FILE* ErrorTTPos;
			ErrorTTPos = fopen("..//BIN//Data//CameraData//ErrorTTPos.txt","a+");
			if (ErrorTTPos == NULL)
				AfxMessageBox("写入ErrorTTPos.txt文件失败！");

			for (int i=0; i<6; i++)	
				fprintf(ErrorTTPos,"%f  ",errorTT[i]);	

			fprintf(ErrorTTPos,"\n");
			fclose(ErrorTTPos);

			FILE* ErrorAPos;
			ErrorAPos = fopen("..//BIN//Data//CameraData//ErrorAPos.txt","a+");
			if (ErrorAPos == NULL)
				AfxMessageBox("写入ErrorAPos.txt文件失败！");

			for (int i=0; i<4; i++)	
				fprintf(ErrorAPos,"%f  ",errorA[i]);	

			fprintf(ErrorAPos,"\n");
			fclose(ErrorAPos);


			((CButton *)GetDlgItem(IDC_BUTTON_SAMPLING_MARKER_DATA))->EnableWindow(TRUE);

			samplingFailTimes = 0;
			samplingSuccTimes = 0;

			for (int i=0; i<2; i++)
				for (int j=0; j<4; j++)
					for (int k=0; k<3; k++)
						ptTargetSum[i][j][k] = 0;

			for (int i=0; i<2; i++)
				for(int j=0; j<3; j++)
					centerSum[i][j] = 0;

			AfxMessageBox("采集保存完毕");
	}
}

void ComTestView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDEvent)
	{
	case DISPLAY_TCP_POS:
		//drawToPics();
		//UpdateWindow();
		break;
	case REG_MARKER_TIMER:
		samplingMarkersPos();
		break;
	default:
		break;
	}
	
	CFormView::OnTimer(nIDEvent);
}
