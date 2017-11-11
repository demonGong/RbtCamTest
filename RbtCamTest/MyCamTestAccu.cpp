// MyCamTestAccu.cpp : 实现文件
//

#include "stdafx.h"
#include "RbtCamTest.h"
#include "MyCamTestAccu.h"
#include "afxdialogex.h"
#include "RbtCamTestDoc.h"
#include "MainFrm.h"


// CMyCamTestAccu 对话框

IMPLEMENT_DYNAMIC(CMyCamTestAccu, CPropertyPage)

CMyCamTestAccu::CMyCamTestAccu()
	: CPropertyPage(CMyCamTestAccu::IDD)
	, m_CurretKeyPotNums(0)
	, m_SamplingTimes(0)
{
	m_keyPointNums = 0;
}

CMyCamTestAccu::~CMyCamTestAccu()
{
	KillTimer(TIMER_REFRESH_WINDOW_DATA);
	KillTimer(TIMER_SAMPLING_CAMERA_KEYPOINTS);
}

void CMyCamTestAccu::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_POINT_COOD, m_PointCoord);
	DDX_Text(pDX, IDC_EDIT2, m_CurretKeyPotNums);
	DDX_Text(pDX, IDC_STATIC_SAMPLINGTIMES, m_SamplingTimes);
}


BEGIN_MESSAGE_MAP(CMyCamTestAccu, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_TEMP_START, &CMyCamTestAccu::OnBnClickedButtonTempStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP_AND_SAVE, &CMyCamTestAccu::OnBnClickedButtonStopAndSave)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMyCamTestAccu 消息处理程序


void CMyCamTestAccu::OnBnClickedButtonTempStart()
{
	// TODO:  在此添加控件通知处理程序代码
	m_SamplingTimes = 0;
	SetTimer(TIMER_SAMPLING_CAMERA_KEYPOINTS, 50, NULL);
}


void CMyCamTestAccu::OnBnClickedButtonStopAndSave()
{
	// TODO:  在此添加控件通知处理程序代码
	FILE* nPoints;
	nPoints = fopen("..//BIN//Data//nPoints.txt", "w");
	if (nPoints == NULL)
		AfxMessageBox("写入nPoints.txt文件失败！");

	FILE* AvePointsPos;
	AvePointsPos = fopen("..//BIN//Data//AvePointsPos", "w");
	if (AvePointsPos == NULL)
		AfxMessageBox("写入AvePointsPos.txt文件失败！");

	
	for (vector<vector<double>>::iterator it1 = m_keyPoints.begin(); it1 != m_keyPoints.end(); it1++)
	{
		double avrPosX = 0, avrPosY = 0, avrPosZ = 0;
		int i = 0;
		for (vector<double>::iterator it2 = (*it1).begin(); it2 != (*it1).end(); it2++, i++)
		{
			fprintf(nPoints, "%f  ", (*it2));
			if (0 == i % 3)
				avrPosX += (*it2);
			if (1 == i % 3)
				avrPosY += (*it2);
			if (2 == i % 3)
				avrPosZ += (*it2);

		}
		
		fprintf(AvePointsPos, "%f  ", avrPosX / i);
		fprintf(AvePointsPos, "%f  ", avrPosY / i);
		fprintf(AvePointsPos, "%f  ", avrPosZ / i);

		fprintf(AvePointsPos, "\n");
		fprintf(nPoints, "\n");
	}

	fclose(nPoints);
	fclose(AvePointsPos);


}


void CMyCamTestAccu::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TIMER_REFRESH_WINDOW_DATA:
		UpdateWindowData();
		break;
	case TIMER_SAMPLING_CAMERA_KEYPOINTS:
		SamplingXpoints();
		break;

	default:
		break;


	}

	CPropertyPage::OnTimer(nIDEvent);
}


BOOL CMyCamTestAccu::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(TIMER_REFRESH_WINDOW_DATA, 50, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CMyCamTestAccu::UpdateWindowData()
{

	UpdateData(false);

}


void CMyCamTestAccu::SamplingXpoints()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRbtCamTestDoc*   pDoc = (CRbtCamTestDoc*)pFrm->GetActiveDocument();
	ASSERT(pFrm != NULL && pDoc != NULL);

	static int samplingTimes = 1;
	int keyPointNums = 0;
	vector<vector<double>> keyPoints;

	if (1 == samplingTimes)
	{
		pDoc->m_Cam->getAllKeyPointsPos(m_keyPoints, m_keyPointNums);
		m_CurretKeyPotNums = m_keyPointNums;
	}
	else
	{
		pDoc->m_Cam->getAllKeyPointsPos(keyPoints, keyPointNums);
		m_CurretKeyPotNums = keyPointNums;
	}

	if (m_keyPointNums != keyPointNums || 0 == m_CurretKeyPotNums)
	{
		KillTimer(TIMER_SAMPLING_CAMERA_KEYPOINTS);
		return;
	}
		
	for (vector<vector<double>>::iterator it1 = m_keyPoints.begin(); it1 != m_keyPoints.end(); it1++)
	{

		vector<double>::iterator it2 = (*it1).begin();
		for (vector<vector<double>>::iterator it3 = keyPoints.begin(); it3 != keyPoints.end(); it3++)
		{
			vector<double>::iterator it4 = (*it3).begin();
			if (abs((*it4) - (*it2)) < 1 && abs((*(it4 + 1)) - (*(it2 + 1))) < 1 && abs((*(it4 + 2)) - (*(it2 + 2))) < 1)
			{
				(*it1).push_back(*it4);
				(*it1).push_back(*(it4 + 1));
				(*it1).push_back(*(it4 + 2));

			}

		}

	}

	m_SamplingTimes = samplingTimes;
	samplingTimes++;
}
