
// RbtCamTestDoc.h : interface of the CRbtCamTestDoc class
//


#pragma once
#include "Vector3f.h"
#include "LibMatrix.h"
#include "NavArithmetic.h"
#include "MT_C.h"
#include "AbstractRbtCtrllerFactory.h"
#include "AbstractRobotController.h"
#include "SimpleRbtCtrllerFactory.h"
#include "myClientSocket.h"
#include "RobotControllerCB2.h"
#include "RobotControllerCB3.h"
#include "RobotControllerCB3X2.h"
#include "RobotParam.h"
#include "wkIniConfigurator.h"

#include "RobotRegView.h"
#include "ComAccTest.h"
#include "Matrix.h"

const CString SECTION_PARAMETER_CAMETER   =  _T("Section_Parameter_Camera");
const CString SECTION_PARAMETER_ROBOT     =  _T("Section_Parameter_Robot");
const CString SECTION_PARAMETER_SYSTEM    =  _T("Section_Parameter_URGs");
const CString SECTION_PARAMETER_SN        =  _T("Section_Parameter_SN");

#define REG_CAP_POINT_NUM  50				  //每个marker 的采集次数
#define WK_PI 3.14159265358979323846		  //Pi

class CRbtCamTestDoc : public CDocument
{
protected: // create from serialization only
	CRbtCamTestDoc();
	DECLARE_DYNCREATE(CRbtCamTestDoc)

public:

	////////////////***配置文件路径***///////////////////
	CString  settingFileName;

	///////////////***摄像头***//////////////////
	CString calibDirName;
	CString markerDirName;

	CString robotMarkerName;
	CString testMarkerName;


	CString headMarkerL;
	CString headMarkerR;
	CString headMarkerM;
	CString checkMarker;

	float markerLoc1[3];
	float markerLoc2[3];
	float markerLoc3[3];
	float markerLoc4[3];

	float markRegVarThreshold;

	///////////////***机器人***//////////////////
	int robotCtrlModel;
	CString	ipAddress;
	UINT nPort;
	float rbtInitialJointZero[6];
	double robotMarkerPos[3];
	float tcpPin[3];
	
	///////////////***数据***//////////////////
	CMT_C *m_Cam;
	CAbstractRbtCtrllerFactory * RbtCtrllerFactory;
	CAbstractRobotController * RbtCtrller;

	BOOL   m_bRbtManualRegFinised;															// 机械臂手动注册是否成功
	double testMarkerPos[3];																//待检测Marker相对于相机的当前位置
	float m_regMtxCam2Rbt[4][4];                                                           // 相机到机械臂之间的空间映射矩阵仿射变换算法
	float m_regMtxCam2RbtSVD[4][4];                                                        // 相机到机械臂之间的空间映射矩阵奇异值分解算法
	float m_regMtxRbt2Cam[4][4];                                                           // 机械臂到相机之间的空间映射矩阵
// Attributes
public:

// Operations
public:
	////////////////////////////// 文件路径及读取 /////////////////////
	CString GetAppExeName();
	CString GetAppExePath();
	CString GetINIFilePath();
	void ReadAllParaFromINIFile(const char *filePath);

	////////////////////////////// 空间坐标映射(仿射) 及误差统计/////////////////////
	float SolveTransMatrix_Rbt2Cam(float cam[4][3],float rbt[4][3],float r2cMtx[4][4],float errArr[4]);
	float SolveTransMatrix_Cam2Rbt(float cam[4][3],float rbt[4][3],float c2rMtx[4][4],float errArr[4]);

	float ComputeCumulativeError(float source[4][3],float target[4][3],float mtx[4][4],float errArr[4]);

	////////////////////////////// 空间坐标映射(SVD) 及误差统计/////////////////////
	void SolveMapMatrixSUV(float source[4][3],float target [4][3],float T[4][4]);

	void ComputeMatchPointMean(float source[4][3],float target [4][3],float outX[3],float outY[3]);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CRbtCamTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnRegisterRbt();
	afx_msg void OnComAccTest();
};
