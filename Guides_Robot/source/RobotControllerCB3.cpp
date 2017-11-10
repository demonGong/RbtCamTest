#include "StdAfx.h"
#include "RobotControllerCB3.h"

//��е�۽����߳�
UINT CRobotControllerCB3::UnlockRobotThreadProc(LPVOID pParam)
{
	CRobotControllerCB3 * pURCtrl=(CRobotControllerCB3 *)pParam;

	Sleep(500);
	if(pURCtrl->CheckRbtState() == ARM_BRAKE_RELEASE)
	{
		pURCtrl->SetRobotMoveSpeed(0.2);        /// ���û����˳�ʼ�ٶ�
		AfxMessageBox(_T("�����ɹ�"));
		return 0;
	}

	while(pURCtrl->RobotPowerOn() != ROBOT_WORKING_CORRECT 
		|| (pURCtrl->CheckRbtState() == ARM_POWER_ON_OR_EMERGENCY_UP))
	{
		Sleep(500);
	}

	while(pURCtrl->RobotBrakeRelease() != ROBOT_WORKING_CORRECT)
	{
		Sleep(500);
	}

	pURCtrl->SetRobotMoveSpeed(0.2);        /// ���û����˳�ʼ�ٶ�
	AfxMessageBox(_T("�����ɹ�"));

	return 0;
}

CRobotControllerCB3::CRobotControllerCB3(void)
{

}


CRobotControllerCB3::~CRobotControllerCB3(void)
{

}

//��ʼ�������ˣ�������λ��������������������������������߳�
UINT CRobotControllerCB3::InitialRobot(RobotParam *m_Parm)
{
	if ( NULL != m_Parm )
		m_RobotParam = m_Parm;

	CString m_IniFliePath = GetIniFileName();
	getParaFromINIFlie(m_IniFliePath);

	m_Socket->initialClient();
	if (TRUE == m_Socket->m_bClientSocketConnecting)
	{
		AfxBeginThread(TimeCheckThread,this);
		AfxBeginThread(PackAnalysisThread,this);
		return ROBOT_WORKING_CORRECT;
	}
	else
	{
		AfxMessageBox("����������ʧ�ܣ������ԣ�");
		return ROBOT_SOCKET_DISCONNECT;
	}
}

// ���û�����ʾ��ģʽ teach_mode()
UINT CRobotControllerCB3::SetRobotFreeDriveMode()
{
	CString str;

	// 	if(FALSE == m_Socket->m_bClientSocketConnecting)
	// 	{
	// 		AfxMessageBox("δ����������ӣ�");
	// 		return ROBOT_SOCKET_DISCONNECT;
	// 	}


	if (CheckRbtState() == ARM_PROTECTIVE_STOPPED)
		SetProtectedStopUnLock();

	if((CheckRbtState() == ARM_TEACH) && ( TRUE == m_robotDriveMode))
	{
		return ROBOT_WORKING_CORRECT;
	}

	else
	{
		str="def freedrivemode():\n"
			"  teach_mode()\n"
			"  sleep(3600)\n"
			"end\n";
		m_Socket->ClientSend(str.GetBuffer(),str.GetLength());
		m_robotDriveMode = TRUE;

		Sleep(500);
		if ((CheckRbtState() == ARM_TEACH) && ( TRUE == m_robotDriveMode))
			return ROBOT_WORKING_CORRECT;
		else
		{
			//AfxMessageBox("ʾ������ģʽʧ�ܣ�");
			return  ROBOT_MOVE_STATE_ERROR;	
		}
			
	}
	

}
// UINT CRobotControllerCB3::StartFreeDriveMode()
// {
// 	CString str,s;
// 	str.Empty();
// 	s = "def inkognito():\n";
// 	str += s;
// 	s = "  teach_mode()\n";
// 	str += s;
// 	s = "   sleep(3600)\n";
// 	str += s;
// 	s = "end\n";
// 	str += s;
// 
// 	m_Socket->ClientSend(str.GetBuffer(0),str.GetLength());
// 
// 	return ROBOT_WORKING_CORRECT;
// 
// }
// UINT CRobotControllerCB3::StopFreeDriveMode()
// {
// 	
// 	CString str,s;
// 	str.Empty();
// 	s = "def inkognito():\n";
// 	str += s;
// 	s = "  end_teach_mode()\n";
// 	str += s;
// 	s = "end\n";
// 	str += s;
// 
// 	m_Socket->ClientSend(str.GetBuffer(0),str.GetLength());
// 
// 	return ROBOT_WORKING_CORRECT;
// 
// }

// ���û���������ģʽ end_teach_mode()
UINT CRobotControllerCB3::SetRobotRunningMode()
{
	CString str;
// 	if(FALSE == m_Socket->m_bClientSocketConnecting)
// 	{
// 		AfxMessageBox("δ����������ӣ�");
// 		return ROBOT_SOCKET_DISCONNECT;
// 	}

	if((CheckRbtState() == ARM_BRAKE_RELEASE) && (FALSE == m_robotDriveMode))
		return ROBOT_WORKING_CORRECT;

	else if((CheckRbtState() == ARM_TEACH) && (TRUE == m_robotDriveMode))
	{
		str="end_teach_mode()\n";			
		m_Socket->ClientSend(str.GetBuffer(),str.GetLength());
		m_robotDriveMode = FALSE;

		Sleep(500);
		if ((CheckRbtState() == ARM_BRAKE_RELEASE) && (FALSE == m_robotDriveMode))
			return ROBOT_WORKING_CORRECT;
		else
		{
			//AfxMessageBox("���������˶�ģʽʧ�ܣ�");
			return ROBOT_MOVE_STATE_ERROR;	
		}
	}
	else 
		return ROBOT_MOVE_STATE_ERROR;	

}

// ��ȡ������ʵ��״̬����������ڲ�����
UINT CRobotControllerCB3::CheckRbtState()
{
	if (m_Socket->m_bClientSocketConnecting)
	{
		switch ((int)m_RobotParam->m_PackData.robotMode)
		{
		case ROBOT_MODE_POWER_OFF:
			if(CheckSafetyMode(SAFETY_MODE_NORMAL)&&CheckJointMode(JOINT_POWER_OFF_MODE))
				return ARM_POWER_OFF;
			else if(CheckSafetyMode(SAFETY_MODE_ROBOT_EMERGENCY_STOP)&&CheckJointMode(JOINT_POWER_OFF_MODE))
				return ARM_EMERGENCY_DOWN;
			break;
		case ROBOT_MODE_IDLE:
			if(CheckJointMode(JOINT_IDLE_MODE)&&CheckSafetyMode(SAFETY_MODE_NORMAL))
				return ARM_POWER_ON_OR_EMERGENCY_UP;
			break;
		case ROBOT_MODE_RUNNING:
			if(CheckJointMode(JOINT_RUNNING_MODE)&&CheckSafetyMode(SAFETY_MODE_NORMAL))
			{
				if(m_robotDriveMode == FALSE)
					return ARM_BRAKE_RELEASE;
				else
					return ARM_TEACH;
			}
			else if(CheckJointMode(JOINT_RUNNING_MODE)&&CheckSafetyMode(SAFETY_MODE_PROTECTIVE_STOP))
				return ARM_PROTECTIVE_STOPPED;
			break;
		}
		return ARM_UNKOWN_STATE;
	}
	else
		return ROBOT_SOCKET_DISCONNECT;
}


UINT CRobotControllerCB3::MoveRobotStop()
{
// 	if(FALSE == m_Socket->m_bClientSocketConnecting)
// 	{
// 		AfxMessageBox("δ����������ӣ�");
// 		return ROBOT_SOCKET_DISCONNECT;
// 	}

// 	if(CheckRbtState()==ARM_BRAKE_RELEASE)
// 	{		
	CString program("speedj([0,0,0,0,0,0],7,0.2)\n");
// 	CString program;
// 	program += str;
// 
// 	str = "teach_mode()\n";
// 	program += str;
// 
// 	str = "end_teach_mode()\n";
// 	program += str;

	m_Socket->ClientSend(program.GetBuffer(),program.GetLength());
	return ROBOT_WORKING_CORRECT;
// 	}
// 	return ROBOT_MOVE_STATE_ERROR;
}


//�������ݰ�
void CRobotControllerCB3::PackAnalysis()
{
	CString str;
	str.Format("%d",1);
	TRACE(str);
	TRACE("\n");

	char pBuff[MAX_PACK_LENGTH];
	m_Socket->ClientRecv(pBuff,MAX_PACK_LENGTH);
	BYTE cMessageLength[4];			//����
	int *nMessageLength;
	for(int i=0;i<4;i++)
		cMessageLength[i]=pBuff[3-i];
	nMessageLength = (int *)cMessageLength;
	if (*nMessageLength != CB3_PACKPARALENGTH)
		return;
	else
		m_nPackRecvNum++;

	BYTE cTime[8];					//������ʱ��
	double* fTime;

	BYTE cActualJointPos[48];		//ʵ�ʹؽ�ֵ
	double* fActualJointPos;

	BYTE cActualTcpPos[48];			//ʵ�ʹ�������
	double* fActualTcpPos;

	BYTE cRobotMode[8];				//������״̬
	double* fRobotMode;
	//UINT nRobotMode;

	BYTE cJointModes[48];			//�ؽ�״̬
	double* fJointModes;

	BYTE cTcpForce[48];				//ʵʱ����ֵ��������ϵ��
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


//��ȡ��ǰ��е�۰�ȫ״̬
BOOL CRobotControllerCB3::CheckSafetyMode(UINT state)
{
	if (state == m_RobotParam->m_PackData.safetyMode)
		return TRUE;
	else
		return FALSE;
}

 //����е�۰�ȫ����״̬����Ϊ��ͨ��ȫ״̬�����������ֹͣ
UINT CRobotControllerCB3::SetProtectedStopUnLock() 
{
	//RobotBrakeRelease();
	CString str;	
	if (CheckSafetyMode(SAFETY_MODE_PROTECTIVE_STOP))
	{
		str="set unlock protective stop\n";
		m_Socket->ClientSend(str.GetBuffer(),str.GetLength());

		Sleep(500);
		if (CheckSafetyMode(SAFETY_MODE_NORMAL))
			return ROBOT_WORKING_CORRECT;
		else
		{
			//AfxMessageBox("�����ȫ����ʧ�ܣ�");
			return ROBOT_MOVE_STATE_ERROR;
		}
		
	}
	else
		return ROBOT_WORKING_CORRECT;
		
}