#include "StdAfx.h"
#include "RobotControllerCB2.h"

// 机械臂初始化线程，每个关节运动一段小位移
UINT CRobotControllerCB2::RobotInitMoveThread(LPVOID pParam)
{
	CRobotControllerCB2 * pURCtrl=(CRobotControllerCB2 *)pParam;
	if(TRUE == pURCtrl->m_bRobotInitMoveThreadRunning)
		return ROBOT_WORKING_CORRECT;
	CString str;
	CString speedp("0.1");
	CString speedz("0.0");
	CString speedn("-0.1");
	CString speed[6];
	BOOL direction[6];// 0为负，1为正
	// 位姿初判断
	// 各关节逆时针方向旋转为正，第3和第5关节为-90度，其他为0度
	pURCtrl->m_bRobotInitMoveThreadRunning = TRUE;
	for (int i=0;i<6;i++)
	{
		if(2==i || 4==i)
		{
			if((pURCtrl->m_RobotParam->m_PackData.q_actual[i]+1.57) <= 0)
				direction[i] = TRUE;
			else
				direction[i] = FALSE;
		}
		else
		{
			if(pURCtrl->m_RobotParam->m_PackData.q_actual[i] <= 0)
				direction[i] = TRUE;
			else
				direction[i] = FALSE;
		}
	}
	// 初始化运动
	while (pURCtrl->m_bRobotInitMoveThreadRunning)
	{
		pURCtrl->m_csURCtrl.Lock();
		for (int i=0;i<6;i++)
		{
			if(JOINT_INITIALISATION_MODE == (int)pURCtrl->m_RobotParam->m_PackData.jointMode[i])
			{
				if(direction[i])
					speed[i] = speedp;
				else
					speed[i] = speedn;
			}
			else if (JOINT_RUNNING_MODE == (int)pURCtrl->m_RobotParam->m_PackData.jointMode[i])
			{
				speed[i] = speedz;
			}
		}
		pURCtrl->m_csURCtrl.Unlock();
		str="speedj_init(["+speed[0]+","+speed[1]+","+speed[2]+","+speed[3]+","+speed[4]+","+speed[5]+"],1,0.5)\n";
		pURCtrl->m_Socket->ClientSend(str.GetBuffer(),str.GetLength());
		Sleep(500);

		if(pURCtrl->CheckJointMode(JOINT_RUNNING_MODE))
		{
			pURCtrl->m_bRobotInitMoveThreadRunning = FALSE;
			break;		
		}
	}

	return ROBOT_WORKING_CORRECT;

}


CRobotControllerCB2::CRobotControllerCB2(void)
{
	
	
}


CRobotControllerCB2::~CRobotControllerCB2(void)
{

	
}


// 机械臂初始化，开始初始化线程
UINT CRobotControllerCB2::RobotInitMove()
{
	if (CheckRbtState() == ARM_BRAKE_RELEASE)
	{
		AfxBeginThread(RobotInitMoveThread,this);
		return ROBOT_WORKING_CORRECT;
	}
	else if (ROBOT_INITIALIZING_MODE == (int)m_RobotParam->m_PackData.robotMode)
	{
		for (int i=0;i<6;i++)
		{
			if (JOINT_INITIALISATION_MODE == m_RobotParam->m_PackData.jointMode[i])
			{
				AfxBeginThread(RobotInitMoveThread,this);
				return ROBOT_WORKING_CORRECT;
			}
		}
	}
	return ROBOT_MOVE_STATE_ERROR;	
}

// 获取机器人实际状态，详见函数内部定义
UINT CRobotControllerCB2::CheckRbtState()
{
	if (m_Socket->m_bClientSocketConnecting)
	{
		switch ((int)m_RobotParam->m_PackData.robotMode)
		{

		case ROBOT_RUNNING_MODE:
			if(CheckJointMode(JOINT_INITIALISATION_MODE))
				return ARM_UNINITIALIZED;
			else if(CheckJointMode(JOINT_RUNNING_MODE))
				return ARM_INITIALIZED_OR_RUNNUNG;
			break;
		case ROBOT_FREEDRIVE_MODE:
			if(CheckJointMode(JOINT_BACKDRIVEABLE_MODE))
				return ARM_FREEDRIVE;
			break;
		case ROBOT_INITIALIZING_MODE:
			if(CheckJointMode(JOINT_IDLE_MODE))
				return ARM_POWER_ON_OR_EMERGENCY_UP;
			else if(CheckJointMode(JOINT_INITIALISATION_MODE))
				return ARM_BRAKE_RELEASE;
			else if(CheckJointMode(JOINT_MOTOR_INITIALISATION_MODE))
				return ARM_BRAKE;
			break;
		case ROBOT_SECURITY_STOPPED_MODE:
			if(CheckJointMode(JOINT_SECURITY_STOPPED_MODE))
				return ARM_PROTECTIVE_STOPPED;
			break;
		case ROBOT_EMERGENCY_STOPPED_MODE:
			if(CheckJointMode(JOINT_EMERGENCY_STOPPED_MODE))
				return ARM_EMERGENCY_DOWN;   
		case ROBOT_NO_POWER_MODE:
			if(CheckJointMode(JOINT_POWER_OFF_MODE))
				return ARM_POWER_OFF;
			break;
		}
		return ARM_UNKOWN_STATE;
	}
	else
		return ROBOT_SOCKET_DISCONNECT;

}

// 设置机器人示教模式
UINT CRobotControllerCB2::SetRobotFreeDriveMode()
{
	if (CheckRbtState() == ARM_FREEDRIVE)
		return ROBOT_WORKING_CORRECT;

	CString str;
	str="set robotmode freedrive\n";
	m_Socket->ClientSend(str.GetBuffer(),str.GetLength());
	Sleep(500);
	if (CheckRbtState() != ARM_FREEDRIVE)
		return ROBOT_MOVE_STATE_ERROR;	
	else
		return ROBOT_WORKING_CORRECT;
}

// 机器人运行模式 runmode
UINT CRobotControllerCB2::SetRobotRunningMode()
{
	if (CheckRbtState() == ARM_INITIALIZED_OR_RUNNUNG)
		return ROBOT_WORKING_CORRECT;

	CString str;
	str="set robotmode run\n";
	m_Socket->ClientSend(str.GetBuffer(),str.GetLength());
	Sleep(500);
	if (CheckRbtState() != ARM_INITIALIZED_OR_RUNNUNG)
		return ROBOT_MOVE_STATE_ERROR;
	else
		return ROBOT_WORKING_CORRECT;
	
}

// 设置机器人安全状态
UINT CRobotControllerCB2::SetProtectedStopUnLock()
{
	return ROBOT_WORKING_CORRECT;
}

// 解析数据包
void CRobotControllerCB2::PackAnalysis()
{

	CString str;
	str.Format("%d",1);
	TRACE(str);
	TRACE("\n");

	char pBuff[MAX_PACK_LENGTH];
	m_Socket->ClientRecv(pBuff,MAX_PACK_LENGTH);
	BYTE cMessageLength[4];			//包长
	int *nMessageLength;
	for(int i=0;i<4;i++)
		cMessageLength[i]=pBuff[3-i];
	nMessageLength = (int *)cMessageLength;
	if (*nMessageLength != CB2_PACKPARALENGTH)
		return;
	else
		m_nPackRecvNum++;

	BYTE cTime[8];					//机器人时间
	double *fTime;

	BYTE cActualJointPos[48];		//实际关节值
	double *fActualJointPos;

	BYTE cActualTcpPos[48];			//实际工具坐标
	double *fActualTcpPos;

	BYTE cRobotMode[8];				//机器人状态
	double *fRobotMode;
	//UINT nRobotMode;

	BYTE cJointModes[48];			//关节状态
	double *fJointModes;

	BYTE cTcpForce[48];				//实时受力值（基座标系）
	double *fTcpForce;

	for(int i =0;i<8;i++)
		cTime[i]=pBuff[11-i];
	for(int i=0;i<48;i++)
		cActualJointPos[i]=pBuff[299-i];
	for(int i=0;i<48;i++)
		cActualTcpPos[i]=pBuff[635-i];
	for(int i=0;i<8;i++)
		cRobotMode[i]=pBuff[763-i];
	for(int i=0;i<48;i++)
		cJointModes[i]=pBuff[811-i];	
	for(int i=0;i<48;i++)
		cTcpForce[i]=pBuff[587-i];	

	fTime=(double *)cTime;
	m_curretPackTime = *fTime;
	fActualJointPos=(double *)cActualJointPos;
	fActualTcpPos=(double *)cActualTcpPos;	
	fRobotMode=(double *)cRobotMode;	
	fJointModes=(double *)cJointModes;	
	fTcpForce=(double *)cTcpForce;


	m_csURCtrl.Lock();

	m_RobotParam->m_PackData.msg_size = *nMessageLength;
	m_RobotParam->m_PackData.t_time = *fTime;
	m_RobotParam->m_PackData.robotMode = *fRobotMode;

	for(int i=0;i<6;i++)
	{
		m_RobotParam->m_PackData.q_target[i] = fActualJointPos[5-i];
		m_RobotParam->m_PackData.toolVector[i] = cActualTcpPos[5-i];
		m_RobotParam->m_PackData.jointMode[i] =  fJointModes[5-i];
		m_RobotParam->m_PackData.tcpForce[i] = fTcpForce[5-i];

	}

	for (int i=0; i<3; i++)
	{
		m_RobotParam->m_PackData.toolVector[i] *= 1000;
	}
	
	m_csURCtrl.Unlock();
	str.Format(_T("%d\n"),2);
	TRACE(str);
}

