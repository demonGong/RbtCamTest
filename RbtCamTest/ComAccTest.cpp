// ComAccTest.cpp : implementation file
//

#include "stdafx.h"
#include "RbtCamTest.h"
#include "ComAccTest.h"
#include "afxdialogex.h"
#include "RbtCamTestDoc.h"
#include "MainFrm.h"


// ComAccTest dialog

IMPLEMENT_DYNAMIC(ComAccTest, CDialogEx)

ComAccTest::ComAccTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(ComAccTest::IDD, pParent)
{
	markerName = _T("TTBlock");
	tcpPin[0] = 190.20000;
	tcpPin[1] = 41.60000;
	tcpPin[2] = 115.16000;
}

ComAccTest::~ComAccTest()
{
}

void ComAccTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ComAccTest, CDialogEx)
	ON_BN_CLICKED(IDC_REG_TARGET, &ComAccTest::OnBnClickedRegTarget)
	ON_BN_CLICKED(IDC_MOVETO_TARGET, &ComAccTest::OnBnClickedMovetoTarget)
END_MESSAGE_MAP()


// ComAccTest message handlers


void ComAccTest::OnBnClickedRegTarget()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	bool ret = pDoc->m_Cam->getMarkerPosByName(markerName,markerPos,markerRotateMat);
	if (!ret)
	{
		AfxMessageBox(_T("未发现该marker！"));
		return;
	}
	
	int markerIndex = pDoc->m_Cam->findMarkerInCollection(markerName,NULL);
	if (markerIndex > 0)
		pDoc->m_Cam->getFacetVector(markerIndex,vecPos);

	for (int i=0; i<3; i++)
		targetPt[i] = vecPos[0][i];

	markerPose[0] = markerRotateMat[0];
	markerPose[1] = markerRotateMat[1];
	markerPose[2] = markerRotateMat[2];
	markerPose[3] = targetPt[0];

	markerPose[4] = markerRotateMat[3];
	markerPose[5] = markerRotateMat[4];
	markerPose[6] = markerRotateMat[5];
	markerPose[7] = targetPt[1];

	markerPose[8] = markerRotateMat[6];
	markerPose[9] = markerRotateMat[7];
	markerPose[10] = markerRotateMat[8];
	markerPose[11] = targetPt[2];

	markerPose[12] = 0;
	markerPose[13] = 0;
	markerPose[14] = 0;
	markerPose[15] = 1;

	

}


void ComAccTest::OnBnClickedMovetoTarget()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	CLibMatrix martix;
	double rotateMat[16] = {0,0,-1,0,0,1,0,0,1,0,0,0,0,0,0,1};
	double marker2CamMat[16] = {0};
	martix.couArray(markerPose,rotateMat,marker2CamMat,4,4,4);

	double cam2RbtMat[16];
	for (int i=0; i<4; i++)
		for (int j=0; j<4;j ++)
			cam2RbtMat[i*4 + j] = pDoc->m_regMtxCam2Rbt[i][j];

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

	pDoc->RbtCtrller->SetTcpPos(tcpPin);
	double m_curTargetJoint[6] = {0};
	/////////////////////// 求解机械臂反解 //////////////////////////////////
	if(pDoc->RbtCtrller->GetPosInverse(m_curTargetJoint,m_TargetRbtPos) != 0)
		return;
	
	pDoc->RbtCtrller->MoveRobotJoint(m_curTargetJoint);
	pDoc->RbtCtrller->SetRobotMoveSpeed(0.2);

	
}
