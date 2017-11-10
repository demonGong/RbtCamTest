#pragma once
#include "AbstractRobotController.h"

class EXT_CLASS CRobotControllerCB2: public CAbstractRobotController
{
public:
	CRobotControllerCB2(void);
	~CRobotControllerCB2(void);

public:

	UINT RobotInitMove();												// 机械臂初始化，开始初始化线程
	UINT CheckRbtState();											    // 获取机器人实际状态，详见函数内部定义
	virtual UINT SetRobotFreeDriveMode();								// 设置机器人示教模式
	virtual UINT SetRobotRunningMode();									// 机器人运行模式 runmode
	virtual UINT SetProtectedStopUnLock();								// 设置机器人安全状态
	virtual void PackAnalysis();					                    // 解析数据包
																		
	static UINT RobotInitMoveThread(LPVOID pParam);	                    // 机械臂初始化线程，每个关节运动一段小位移

public:

	BOOL m_bRobotInitMoveThreadRunning;									//机械臂初始化线程状态
};

