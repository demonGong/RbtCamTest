#include "StdAfx.h"
#include "RobotControllerCB3X2.h"


CRobotControllerCB3X2::CRobotControllerCB3X2(void)
{
}


CRobotControllerCB3X2::~CRobotControllerCB3X2(void)
{
}

//解析数据包
void CRobotControllerCB3X2::PackAnalysis()
{
	char pBuff[MAX_PACK_LENGTH];

	CString str;
	str.Format("%d",1);
	TRACE(str);
	TRACE("\n");
	
	m_Socket->ClientRecv(pBuff,CB3X2_PACKPARALENGTH);
	BYTE cMessageLength[4];			//包长
	int *nMessageLength;
	for(int i=0;i<4;i++)
		cMessageLength[i]=pBuff[3-i];
	nMessageLength = (int *)cMessageLength;
	if (*nMessageLength != CB3X2_PACKPARALENGTH)
		return;
	else
		m_nPackRecvNum++;

	BYTE cTime[8];					//机器人时间
	double* fTime;

	BYTE cActualJointPos[48];		//实际关节值
	double* fActualJointPos;

	BYTE cActualTcpPos[48];			//实际工具坐标
	double* fActualTcpPos;

	BYTE cRobotMode[8];				//机器人状态
	double* fRobotMode;
	//UINT nRobotMode;

	BYTE cJointModes[48];			//关节状态
	double* fJointModes;

	BYTE cTcpForce[48];				//实时受力值（基座标系）
	double* fTcpForce;

	BYTE cSafetyMode[8];
	double *fSafetyMode;

	for(int i =0;i<8;i++)
		cTime[i]=pBuff[11-i];
	for(int i=0;i<48;i++)
		cActualJointPos[i]=pBuff[299-i];
	// 	for(int i=0;i<48;i++)
	// 		cActualTcpPos[i]=pBuff[491-i];
	for(int i=0;i<8;i++)
		cRobotMode[i]=pBuff[763-i];
	for(int i=0;i<48;i++)
		cJointModes[i]=pBuff[811-i];	
	for(int i=0;i<48;i++)
		cTcpForce[i]=pBuff[587-i];	
	for(int i=0;i<8;i++)
		cSafetyMode[i]=pBuff[819-i];
	for(int i=0;i<48;i++)
		cActualTcpPos[i]=pBuff[635-i];


	fTime=(double *)cTime;
	m_curretPackTime = *fTime;
	fActualJointPos=(double *)cActualJointPos;
	fActualTcpPos=(double *)cActualTcpPos;
	fRobotMode=(double *)cRobotMode;
	fJointModes=(double *)cJointModes;
	fTcpForce=(double *)cTcpForce;
	fSafetyMode=(double *)cSafetyMode;

	m_csURCtrl.Lock();

	m_RobotParam->m_PackData.msg_size = *nMessageLength;
	m_RobotParam->m_PackData.t_time=*fTime;
	m_RobotParam->m_PackData.robotMode = *fRobotMode;
	m_RobotParam->m_PackData.safetyMode = *fSafetyMode;

	for(int i=0;i<6;i++)
	{
		m_RobotParam->m_PackData.q_target[i] = fActualJointPos[5-i];
		m_RobotParam->m_PackData.toolVector_actual[i] = fActualTcpPos[5-i];
		m_RobotParam->m_PackData.jointMode[i] = fJointModes[5-i];
		m_RobotParam->m_PackData.tcpForce[i] = fTcpForce[5-i];
	}	
	for (int i=0; i<3; i++)
	{
		m_RobotParam->m_PackData.toolVector_actual[i] *= 1000;
	}

	m_csURCtrl.Unlock();
	str.Format(_T("%d\n"),2);
	TRACE(str);
}