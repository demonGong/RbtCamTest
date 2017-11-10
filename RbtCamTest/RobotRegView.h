#pragma once
#include "afxwin.h"


// CRobotRegView dialog
#define REG_CAP_POINT_NUM  50				  //每个marker 的采集次数
#define WK_PI 3.14159265358979323846		  //Pi

class CRobotRegView : public CDialogEx
{
	DECLARE_DYNAMIC(CRobotRegView)

public:
	CRobotRegView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRobotRegView();

// Dialog Data
	enum { IDD = IDD_DLG_ROBOT_MANUAL_REG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtMovetoPos1();
	afx_msg void OnBnClickedBtMoveToPos2();
	afx_msg void OnBnClickedBtMoveToPos3();
	afx_msg void OnBnClickedBtMoveToPos4();

	afx_msg void OnBnClickedBtPos1Reregister();
	afx_msg void OnBnClickedBtPos2Reregister();
	afx_msg void OnBnClickedBtPos3Reregister();
	afx_msg void OnBnClickedBtPos4Reregister();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedBtRmgRegisterAll();
	afx_msg void OnBnClickedBtRmgAutoToZero();
	afx_msg void OnBnClickedBtRmgStopRobot();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

private:
	void CaptureRobotMarker();
	void UpdateRegStatus();
	void BtMoveToPos(int m_nRegRobotIndex);
	void ReRegister(int m_nRegRobotIndex);
	void SetBkColor(CStatic &m_Pic, COLORREF m_Color);
	void RegisterRobot();
	void CheckRegAccuracy();
	void ComputeRobotRegistrationAccurary();

private:
	enum TIMER_RBT_REG_VIEW {
		REG_RBT_MARKER_TIMER,
		UPDATE_BUTTON_TIMER,
		CHECK_ACCURACY_TIME
	};

public:
	CStatic m_stRbtPos1RegStatus;
	CStatic m_stRbtPos2RegStatus;
	CStatic m_stRbtPos3RegStatus;
	CStatic m_stRbtPos4RegStatus;

public:
	BOOL                  m_bRbtPosRegFinished[4];                      // 机械臂某个位置是否注册完成
	//BOOL                  m_bRbtManualRegFinised;                       // 机械臂手动注册是否成功
	double                m_regRobotMarkerPtArr[REG_CAP_POINT_NUM][3];  // 机械臂Marker在相机坐标系下坐标集合
	float                 m_regRobotMarkerPos[4][3];                    // 机械臂Marker相机下坐标集合均值
	int                   m_nRegRobotIndex;                             // 机械臂注册位置索引号
	int                   m_nRegPointCounter;                           // 相机采集Marker点计数
	float                 m_regRobotTipPos[4][3];                       // 机械臂坐标系下，marker坐标
	float                 m_checkAccRobotPos[3];                        // 验证精度机械臂坐标系Mark坐标
	float                 m_checkAccCameraPos[3];                       // 验证精度相机坐标系Mark坐标
	float                 m_accuracyTestPoint[3];                       // 测试精度点坐标
	int                   m_lastRegPointIndex;                          // 最后一个测试位置索引

	COLORREF              m_clrRegUnfinished;                           // 没有注册颜色
	COLORREF              m_clrRegFinished;                             // 注册完成颜色

	float                 m_rbtInitialPos_1[6];							//机械臂零位
	CString				  markerName;									//机械臂凹槽marker名字
	
	afx_msg void OnBnClickedBtPrRbtUnlock();
	afx_msg void OnBnClickedBtPrShutDown();
};
