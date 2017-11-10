#pragma once

#include "AbstractRobotController.h"

class EXT_CLASS CRobotControllerCB3: public CAbstractRobotController
{
public:
	CRobotControllerCB3(void);
	virtual ~CRobotControllerCB3(void);

public:

	virtual UINT InitialRobot(RobotParam *m_Parm = NULL);							 //初始化机器人，连接上位机，开启检测断连，解析包，检测力的线程
	virtual UINT SetRobotFreeDriveMode();								             // 设置机器人示教模式 teach_mode()
	virtual UINT SetRobotRunningMode();												 // 设置机器人运行模式 end_teach_mode()
	virtual UINT CheckRbtState();												     // 获取机器人实际状态，详见函数内部定义
	virtual UINT MoveRobotStop();                                                    // 机械臂停止运动并消除震动

	virtual void PackAnalysis();													 //分析数据包
	BOOL CheckSafetyMode(UINT state);												 //获取当前机械臂安全状态
	UINT SetProtectedStopUnLock();                                                   //将机械臂安全保护状态设置为普通安全状态

	static UINT UnlockRobotThreadProc(LPVOID pParam);								  //机械臂解锁线程，目前set robotmode run可自行完成，功能类似

private:
	BOOL m_robotDriveMode;
};

