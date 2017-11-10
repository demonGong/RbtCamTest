#include "StdAfx.h"
#include "AbstractRobotController.h"

// Socketʱ�������̺߳���
UINT CAbstractRobotController::TimeCheckThread(LPVOID pParam)
{
	CAbstractRobotController * pURCtrl=(CAbstractRobotController *)pParam;
	if(TRUE == pURCtrl->m_bRobotTimeCheckThreadRunning)
		return ROBOT_WORKING_CORRECT;
	pURCtrl->m_bRobotTimeCheckThreadRunning = TRUE;
	double intervalTime = 0.0;
	pURCtrl->m_curretPackTime=0.2;

	while(pURCtrl->m_bRobotTimeCheckThreadRunning)
	{
		Sleep(500);
		pURCtrl->m_csURCtrl.Lock();
		intervalTime = pURCtrl->m_curretPackTime - pURCtrl->m_prePackTime;
		pURCtrl->m_csURCtrl.Unlock();
		// ʱ������300mm���ڣ�˵��Socket�Ѿ�������m_fPackTime�Ѿ��д���400mmû�и���ֵ
		if (intervalTime < 0.1)
		{
			pURCtrl->m_csURCtrl.Lock();
			pURCtrl->m_bRobotTimeCheckThreadRunning = FALSE;
			pURCtrl->m_Socket->ClientClose();
			pURCtrl->m_bPackParaAnalysisThreadRunning = FALSE;
			pURCtrl->m_csURCtrl.Unlock();
			AfxMessageBox("������˶Ͽ����ӣ�");		
		}

		 pURCtrl->m_prePackTime = pURCtrl->m_curretPackTime;
	}
	return ROBOT_WORKING_CORRECT;
}

// ��е�����ݰ������߳�
UINT CAbstractRobotController::PackAnalysisThread(LPVOID pParam)
{
	CAbstractRobotController * pURCtrl=(CAbstractRobotController *)pParam;
	if(TRUE == pURCtrl->m_bPackParaAnalysisThreadRunning)
		return ROBOT_WORKING_CORRECT;
	else
		pURCtrl->m_bPackParaAnalysisThreadRunning = TRUE;

	while(pURCtrl->m_bPackParaAnalysisThreadRunning)
	{
		pURCtrl->PackAnalysis();
	}

	//pURCtrl->MoveRobotStop();
	pURCtrl->m_bRobotTimeCheckThreadRunning = FALSE;
	pURCtrl->m_bPackParaAnalysisThreadRunning = FALSE;
	pURCtrl->m_Socket->ClientClose();

	return ROBOT_SOCKET_DISCONNECT;

}

// ���ĩ�������Ƿ������ֵ�߳�
UINT CAbstractRobotController::ForceThread(LPVOID pParam)
{
	CAbstractRobotController * pURCtrl=(CAbstractRobotController *)pParam;
	
	while(pURCtrl->m_Socket->m_bClientSocketConnecting)
	{
		//pURCtrl->m_csURCtrl.Lock();
		if (pURCtrl->m_RobotParam->m_PackData.tcpForce[5] > MIN_FORCE 
		  &&pURCtrl->m_RobotParam->m_PackData.tcpForce[5] < MAX_FORCE)  
		{
			pURCtrl->RobotBrakeRelease();
			pURCtrl->SetRobotFreeDriveMode();
			if (!pURCtrl->m_bBrakeReleaseTimeThreadRunning)
			{
				pURCtrl->m_bBrakeReleaseTimeThreadRunning = TRUE;
				pURCtrl->SetPayLoad(MASS_PAYLOAD);
				AfxBeginThread(BrakeReleaseTimeThread,pURCtrl);
				
			}

		}
		//pURCtrl->m_csURCtrl.Unlock();
		
	}

	return ROBOT_WORKING_CORRECT;

}

// ����е���Ƿ񳬹�ʾ��ʱ���̣߳��������Զ���runmode
UINT CAbstractRobotController::BrakeReleaseTimeThread(LPVOID pParam)
{
	DWORD preTimer = 0;
	DWORD curTimer =0;
	int ThreadWorkingTimes = 1;
	double preJointPos[6] = {0};
	double curJointPos[6] = {0};
	BOOL IsRobotMoving = TRUE;

	CAbstractRobotController * pURCtrl=(CAbstractRobotController *)pParam;

	while(1)
	{
		if (1 == ThreadWorkingTimes)
		{
			pURCtrl->m_csURCtrl.Lock();
			for(int i=0; i<6; i++)
			{	
				curJointPos[i] = pURCtrl->m_RobotParam->m_PackData.i_actual[i];
				preJointPos[i] = curJointPos[i];
			}
			pURCtrl->m_csURCtrl.Unlock();
		
		}
		else
		{
			pURCtrl->m_csURCtrl.Lock();
			for(int i=0; i<6; i++)
			{	
				curJointPos[i] = pURCtrl->m_RobotParam->m_PackData.i_actual[i];
			}
			pURCtrl->m_csURCtrl.Unlock();

			for (int i=0; i<6; i++)
			{
				if (abs(curJointPos[i] - preJointPos[i]) > 0.1)
				{
					IsRobotMoving = TRUE;
					break;
				}
			}

			for (int i=0; i<6; i++)
				preJointPos[i] = curJointPos[i];
		}

		if (IsRobotMoving)
		{
			curTimer = GetTickCount();
			preTimer = curTimer;

			IsRobotMoving = FALSE;
		}
		else
		{
			curTimer = GetTickCount();
			DWORD staticTime = curTimer - preTimer;
			if (staticTime > MAX_WAITING_TIME)
			{
				pURCtrl->SetRobotRunningMode();
				break;
			}
		}

		ThreadWorkingTimes++;
	}

	pURCtrl->SetPayLoad(0.0);
	pURCtrl->m_bBrakeReleaseTimeThreadRunning = FALSE;
	return ROBOT_WORKING_CORRECT;
}




CAbstractRobotController::CAbstractRobotController(void)
{
	
	m_RobotParam = new RobotParam;
	m_Socket = new myClientSocket;

	m_curretPackTime = 0.0;
	m_prePackTime    = 0.0;

	m_bRobotTimeCheckThreadRunning   = FALSE;
	m_bPackParaAnalysisThreadRunning = FALSE;
	m_bBrakeReleaseTimeThreadRunning = FALSE;

	pinlength = 190;

}


CAbstractRobotController::~CAbstractRobotController(void)
{
	m_bRobotTimeCheckThreadRunning   = FALSE;
	m_bPackParaAnalysisThreadRunning = FALSE;

	if (m_RobotParam != NULL)
	{
		delete m_RobotParam;
		m_RobotParam = NULL;
	}

	char pBuffer[MAX_BUFF_LENGTH];
	if (m_Socket != NULL && 0 == m_Socket->ClientRecv( pBuffer, MAX_BUFF_LENGTH))
	{
		m_Socket->ClientClose();
		delete m_Socket;
		m_Socket = NULL;
	}
	
}

//�õ���ǰ��������·����׼����ȡ�����ļ�
CString CAbstractRobotController::GetAppPath()
{
	CString strAppPath;

	char m_AppPath[MAX_PATH];
	GetModuleFileName(NULL,m_AppPath,MAX_PATH);


	strAppPath = m_AppPath;
	if (!strAppPath.IsEmpty())
	{
		strAppPath.MakeReverse();
		strAppPath = strAppPath.Right(strAppPath.GetLength() - strAppPath.Find('\\'));
		strAppPath.MakeReverse();
	}
	return strAppPath;
}

//�õ������������ļ�·��
CString CAbstractRobotController::GetIniFileName()
{
	CString szInifileName;
	szInifileName = GetAppPath();
	szInifileName += _T("RbtCamTest.ini");

	return szInifileName;

}

//��ȡ�����������ļ�����ʼ������
void CAbstractRobotController::getParaFromINIFlie(const char *iniFilePath)
{
	char buffer[255];

	GetPrivateProfileString("Section_Parameter_Robot","ipAddress",NULL,buffer,255,iniFilePath);
	if ( 0 == strlen(buffer))
		m_Socket->setServerIpAddress("192.168.1.106");
	else
		m_Socket->setServerIpAddress(buffer);

	GetPrivateProfileString("Section_Parameter_Robot","nPort",NULL,buffer,255,iniFilePath);

	if (0 == strlen(buffer))
		m_Socket->setServerHostPort(30003);
	else
	{
		int hostPort = atoi(buffer);
		m_Socket->setServerHostPort(hostPort);
	}

	GetPrivateProfileString("Section_Parameter_Robot","pinlen",NULL,buffer,255,iniFilePath);
	if (0 == strlen(buffer))
		m_RobotParam->m_pinLen = 0;
	else
		m_RobotParam->m_pinLen = atof(buffer);
	
}

//��ʼ�������ˣ�������λ�����������������������߳�
UINT CAbstractRobotController::InitialRobot(RobotParam *m_Parm)
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
		return ROBOT_SOCKET_DISCONNECT;

}

//����������λ���Ͽ�����
UINT CAbstractRobotController::DisConnectRobot()
{
	int iResult = 0;
	iResult = m_Socket->ClientClose();
	if (0 == iResult)
		return ROBOT_WORKING_CORRECT;
	else
		return ROBOT_MOVE_STATE_ERROR;
}

//�������ϵ�
UINT CAbstractRobotController::RobotPowerOn()
{
	CString str;
	if(FALSE == m_Socket->m_bClientSocketConnecting)
	{
		AfxMessageBox("δ����������ӣ�");
		return ROBOT_SOCKET_DISCONNECT;
	}

	str="power on\n";
	m_Socket->ClientSend(str.GetBuffer(),str.GetLength());
	Sleep(500);
	return ROBOT_WORKING_CORRECT;
	
}


//�����˽�����set robotmode run��
UINT CAbstractRobotController::RobotBrakeRelease()
{
	CString str;
	if(FALSE == m_Socket->m_bClientSocketConnecting)
	{
		AfxMessageBox("δ����������ӣ�");
		return ROBOT_SOCKET_DISCONNECT;
	}

	str="set robotmode run\n";
	m_Socket->ClientSend(str.GetBuffer(),str.GetLength());
	SetRobotMoveSpeed(0.2);
	Sleep(500);

	return ROBOT_BRAKE_RELEASE_ERROR;
	

}

//�µ�
UINT CAbstractRobotController::RobotPowerOff()
{
	CString str;
	if(FALSE == m_Socket->m_bClientSocketConnecting)
	{
		AfxMessageBox("δ����������ӣ�");
		return ROBOT_SOCKET_DISCONNECT;
	}

	str="power off\n";
	m_Socket->ClientSend(str.GetBuffer(),str.GetLength());

	Sleep(500);
	if (CheckRbtState() != ARM_POWER_OFF)
	{
		AfxMessageBox("�µ�ʧ�ܣ�");
		return ROBOT_POWER_OFF_ERROR;
	}
	else
		return ROBOT_WORKING_CORRECT;
}

//�ػ�
UINT CAbstractRobotController::RobotPowerDown()
{
	CString str;
	if(FALSE == m_Socket->m_bClientSocketConnecting)
	{
		AfxMessageBox("δ����������ӣ�");
		return ROBOT_SOCKET_DISCONNECT;
	}

	str="powerdown()\n";
	m_Socket->ClientSend(str.GetBuffer(),str.GetLength());

	Sleep(500);
	if (CheckRbtState() == ARM_POWER_OFF 
	|| CheckRbtState() == ROBOT_SOCKET_DISCONNECT
	|| CheckRbtState() == ARM_UNKOWN_STATE)
		return ROBOT_WORKING_CORRECT;
	else
	{
		AfxMessageBox("�ػ�ʧ�ܣ�");
		return ROBOT_POWER_DOWN_ERROR;
	}
	
}

//��е��TCP�˶�����е�ۻ�����ϵtargetλ�ã����ؽ�����ɻ�е�ۿ�������ɣ�����+�˶���target��
UINT CAbstractRobotController::MoveRobotInverse(double * target,double acc,double speed,double time)
{

	if(CheckRbtState()!=ARM_BRAKE_RELEASE)
		return ROBOT_MOVE_STATE_ERROR;
	/**
	�ַ���
	"def InverseMove():\n"					
	"pos=p[0.546492,0.211033,-0.0513432,3.098,0.376514,1.66502]\n"
	**/
	double mTarget[6];
	if(target!=NULL)
	{
		mTarget[0]=target[0]/1000.0;
		mTarget[1]=target[1]/1000.0;
		mTarget[2]=target[2]/1000.0;
		mTarget[3]=target[3];
		mTarget[4]=target[4];
		mTarget[5]=target[5];
	}
	else
		return ROBOT_MOVE_TARGET_POS_ERROR;

	CString str;
	CString pos,program;
	for(int i=0;i<5;i++)
	{
		str.Format(_T("%f"),mTarget[i]);
		pos=pos+str+_T(",");
	}
	str.Format(_T("%f"),target[5]);
	pos=pos+str;
	//���뺯�����壬�ں�������end��30003�˿ڳ����Զ�ִ�иú���
	program="def InverseMove():\npos=p["+pos+"]\n";
	str="Rx =pos[3]\n" //d2r(90)\n"
		"varmsg(\"Rx\",Rx)\n"
		"Ry =pos[4]\n"// d2r(0)\n"
		"varmsg(\"Ry\",Ry)\n"
		"Rz = pos[5]\n"//d2r(0)\n"
		"varmsg(\"Rz\",Rz)\n"
		"rotat2 = pose_trans(p[0,0,0,0,0,Rz],p[0,0,0,0,Ry,0])\n"
		"varmsg(\"rotat2\",rotat2)\n"
		"rotat3 = pose_trans(rotat2,p[0,0,0,Rx,0,0])\n"
		"varmsg(\"rotat3\",rotat3)\n"
		"global temp_pos = pos\n"
		"temp_pos[3] = rotat3[3]\n"
		"temp_pos[4] = rotat3[4]\n"
		"temp_pos[5] = rotat3[5]\n"
		"textmsg(temp_pos)\n";
	program=program+str;
	str.Format("%f",acc);
	program=program+"movej(temp_pos,"+str+",";
	str.Format("%f",speed);
	program=program+str+",";
	str.Format("%f",time);
	program=program+str+",0)\n"+"end\n";
// 		"movej(temp_pos)\n"
// 		"end\n";
	//program=program+str;
	m_Socket->ClientSend(program.GetBuffer(),program.GetLength());
	return ROBOT_WORKING_CORRECT;
}

//������ؽڵ�targetλ�ã�ʹ�û������˶�����λ�ã��޷��⣬ʵ���������⣩
UINT CAbstractRobotController::MoveRobotJoint(double * target,double acc,double speed,double time)
{
// 	if(CheckRbtState()!=ARM_BRAKE_RELEASE)
// 		return ROBOT_MOVE_STATE_ERROR;
// 	if(target==NULL)
// 		return ROBOT_MOVE_TARGET_POS_ERROR;
	CString str;
	CString program;
	CString pos;
	for(int i=0;i<5;i++)
	{
		str.Format(_T("%f"),target[i]);
		pos=pos+str+_T(",");
	}
	str.Format(_T("%f"),target[5]);
	pos=pos+str;

	//movej(q, a=3, v=0.75, t=0, r=0)���

	str.Format("%f",acc);
	program="movej(["+pos+"],"+str+",";
	str.Format("%f",speed);
	program=program+str+",";
	str.Format("%f",time);
	program=program+str+",0)\n";
	m_Socket->ClientSend(program.GetBuffer(),program.GetLength());
	return ROBOT_WORKING_CORRECT;
}

//����jointNum�ؽڵ��˶����򣬼��ٶȣ��ٶȵȣ����Ը��ٶ������˶�
UINT CAbstractRobotController::RotateJointSpeed(int jointNum, 
												int direction,
												double speed, 
												double acc, 
												double time)
{
	if(CheckRbtState() == ARM_INITIALIZED_OR_RUNNUNG || CheckRbtState()==ARM_BRAKE_RELEASE)
	{

		double jointSpeed[6] = {0};
		jointSpeed[jointNum - 1] = speed * direction;

		CString	str,program;
		program.Empty();
		str = "def inkognito():\n";
		program += str;
		str.Format("  speedj([%f, %f, %f, %f, %f, %f], %f, %f)\n",jointSpeed[0],jointSpeed[1],jointSpeed[2],jointSpeed[3],jointSpeed[4],jointSpeed[5],acc, time);
		program += str;
		str ="end\n";
		program += str;

		m_Socket->ClientSend(program.GetBuffer(),program.GetLength());
		return ROBOT_WORKING_CORRECT;
	}
	else
		return ROBOT_MOVE_STATE_ERROR;

}


//������TCPֱ�߲�ֵ�˶���target����
UINT CAbstractRobotController::MoveRobotLineTarget(double * target)
{
	if(target==NULL)
		return ROBOT_MOVE_TARGET_POS_ERROR;
	if(CheckRbtState() == ARM_INITIALIZED_OR_RUNNUNG || CheckRbtState() == ARM_BRAKE_RELEASE)
	{
		CString str;
		CString program;
		CString pos;
		for(int i=0;i<5;i++)
		{
			str.Format(_T("%f"),target[i]);
			pos=pos+str+_T(",");
		}
		str.Format(_T("%f"),target[5]);
		pos=pos+str;

		//movej(q, a=3, v=0.75, t=0, r=0)���

		str.Format("%f",JOINT_MOVE_ACC);
		program="movel(["+pos+"],"+str+",";
		double speed = 0.05;
		str.Format("%f",JOINT_MOVE_SPEED);
		program=program+str+",";
		str.Format("%f",JOINT_MOVE_TIME);
		program=program+str+",0)\n";
		m_Socket->ClientSend(program.GetBuffer(),program.GetLength());
		return ROBOT_WORKING_CORRECT;
	}
	else
		return ROBOT_MOVE_STATE_ERROR;
}

//��е��ֹͣ�˶�����ʾ��ģʽ����Ч
UINT CAbstractRobotController::MoveRobotStop()
{
// 	if(FALSE == m_Socket->m_bClientSocketConnecting)
// 	{
// 		AfxMessageBox("δ����������ӣ�");
// 		return ROBOT_SOCKET_DISCONNECT;
// 	}

// 	if(CheckRbtState()==ARM_BRAKE_RELEASE)
// 	{		
	CString program("speedj([0,0,0,0,0,0],7,0.2)\n");
	m_Socket->ClientSend(program.GetBuffer(),program.GetLength());
	return ROBOT_WORKING_CORRECT;
// 	}
// 	return ROBOT_MOVE_STATE_ERROR;

}

//��е�ۻص���ʼ��λ����ֱλ��
UINT CAbstractRobotController::MovoRobotZeroPos()
{
	if(FALSE == m_Socket->m_bClientSocketConnecting)
	{
		AfxMessageBox("δ����������ӣ�");
		return ROBOT_SOCKET_DISCONNECT;
	}

	double mTarget[6]={0,-PI/2,0,-PI/2,0,0};
	return MoveRobotJoint(mTarget);
}


//���û�е�۸����ؽڵ��˶��ٶ�
UINT CAbstractRobotController::SetRobotMoveSpeed(double speed)
{
	CString str;
	if((speed>1)||(speed<0.02))
		return FALSE;
	str.Format("set speed %f\n",speed);
	//str="set speed 0.2\n";
	m_Socket->ClientSend(str.GetBuffer(),str.GetLength());
	return ROBOT_WORKING_CORRECT;

}

//��е��ĩ��tool��Speed_Vector���������˶���������
// UINT CAbstractRobotController::RotateAroundX(double Speed_Vector[3])
// {
// 	if(CheckRbtState()!=ARM_INITIALIZED_OR_RUNNUNG || CheckRbtState()!=ARM_BRAKE_RELEASE)
// 		return ROBOT_MOVE_STATE_ERROR;
// 
// 	if (0 == Speed_Vector[0] && 0 == Speed_Vector[1] && 0 == Speed_Vector[2])
// 		return ROBOT_MOVE_TARGET_POS_ERROR;
// 
// 	CString str;
// 	CString vector;
// 	CString program;
// 
// 	double Vector[3] = {Speed_Vector[0],Speed_Vector[1],Speed_Vector[2]};
// 
// 	vector = "[0,0,0";
// 
// 	for (int i=0;i<3;i++)
// 	{
// 		Vector[i] = Vector[i] * SPEED_AROUND_X;
// 		str.Format("%lf",Vector[i]);
// 		vector += ',';
// 		vector += str;
// 	}
// 
// 	vector += ']';
// 
// 	program = "def RotateX():\n";
// 	program += "speedl(";
// 	program += vector;
// 
// 	program += ',';
// 
// 	str.Format("%lf",SPEED_AROUND_X);
// 	program += str;
// 	program += ',';
// 
// 	program += "100)\n";
// 
// 	program += "end\n";
// 
// 	m_Socket->ClientSend(program.GetBuffer(),program.GetLength());
// 
// 	return ROBOT_WORKING_CORRECT;
// }
// 
// //��е��toolֹͣ����ĳ��������ֱ���˶�
// UINT CAbstractRobotController::StopRotateAroundX()
// {
// 	if(CheckRbtState()!=ARM_INITIALIZED_OR_RUNNUNG || CheckRbtState()!=ARM_BRAKE_RELEASE)
// 		return ROBOT_MOVE_STATE_ERROR;
// 
// 	CString program;
// 	CString str;
// 
// 	program += "stopl(";
// 	str.Format("%lf",SPEED_AROUND_X*10);
// 	program += str;
// 	program += ")\n";
// 
// 	m_Socket->ClientSend(program.GetBuffer(),program.GetLength());
// 
// 	return ROBOT_WORKING_CORRECT;
// }

//


//��е��ĩ��tool��Path_Vector���������˶���������
UINT CAbstractRobotController::TransAlongPath(double Path_Vector[3])
{
	if(CheckRbtState()!=ARM_INITIALIZED_OR_RUNNUNG || CheckRbtState()!=ARM_BRAKE_RELEASE)
		return ROBOT_MOVE_STATE_ERROR;

	if (0 == Path_Vector[0] && 0 == Path_Vector[1] && 0 == Path_Vector[2])
		return ROBOT_MOVE_TARGET_POS_ERROR;

	double Vector[3] = {Path_Vector[0],Path_Vector[1],Path_Vector[2]};

	CString str;
	CString vector;
	CString program;

	vector = "[";

	for (int i=0;i<3;i++)
	{
		Vector[i] = Vector[i] * SPEED_ALONG_X;
		str.Format("%lf",Vector[i]);
		vector += str;
		vector += ',';
	}

	vector += "0,0,0]";

	program = "def TransX():\n";
	program += "speedl(";
	program += vector;

	program += ',';

	str.Format("%lf",SPEED_ALONG_X);
	program += str;
	program += ',';

	program += "100)\n";

	program += "end\n";

	m_Socket->ClientSend(program.GetBuffer(),program.GetLength());

	return ROBOT_WORKING_CORRECT;
}

//���ؽڷֱ��˶���Ŀ��ؽ�λ��
UINT CAbstractRobotController::MoveRobotMutilRoute(double* target, int routeNum)
{
	if(CheckRbtState()!=ARM_INITIALIZED_OR_RUNNUNG || CheckRbtState()!=ARM_BRAKE_RELEASE)
		return ROBOT_MOVE_STATE_ERROR;
	if(target==NULL)
		return ROBOT_MOVE_TARGET_POS_ERROR;
	CString str;
	CString program;
	CString pos;

	program = "";
	for (int n=0;n<routeNum;n++)
	{
		pos = "";
		for(int i=0;i<5;i++)
		{
			str.Format(_T("%f"),target[i+n*6]);
			pos=pos+str+_T(",");
		}
		str.Format(_T("%f"),target[5+n*6]);
		pos=pos+str;

		str.Format("%f",JOINT_MOVE_ACC);
		program = program + "movej([" + pos + "]," + str + ",";
		str.Format("%f",JOINT_MOVE_SPEED);
		program=program + str + ",";
		str.Format("%f",JOINT_MOVE_TIME);
		program = program + str + ",0)\n";
	}


	CString sendStr;
	sendStr = "def MultiRouteMove():\n" + program + "end\n";

	m_Socket->ClientSend(sendStr.GetBuffer(),sendStr.GetLength());

	return ROBOT_WORKING_CORRECT;
}

//��������״̬
UINT CAbstractRobotController::CheckRbtState()
{
	return ROBOT_WORKING_CORRECT;
}

//���ؽ�״̬
BOOL CAbstractRobotController::CheckJointMode(UINT state)
{
	int num=0;
	for (int i=0;i<6;i++)
	{
		if(state == m_RobotParam->m_PackData.jointMode[i])
			num++;
	}

	if(6 == num)
		return TRUE;
	else
		return FALSE;
}

//���û�е������
UINT CAbstractRobotController::SetPayLoad(double massKilograms, double *centerOfGravity)
{
	//double center[3] = {centerOfGravity[0],centerOfGravity[1],centerOfGravity[2]};
	CString program,str;

	program = "def SetPayload():\n";
	str = "set_payload(";
	program += str;

	str.Format("%f",massKilograms);
	program += str;
	
	if(NULL == centerOfGravity)
	{
		program += ")\n";
	}
	else
	{
		CString s;
		program += ",";
		str = "[";
		for (int i=0; i<2; i++)
		{
			s.Format("%f",centerOfGravity[i]);
			str += s;
			str += ",";
		}
		s.Format("%f",centerOfGravity[2]);
		str += s;
		str += "])\n";
		program += str;
	}

	program += "end\n";
	m_Socket->ClientSend(program.GetBuffer(),program.GetLength());
	Sleep(500);
	return ROBOT_WORKING_CORRECT;
}


//��õ�ǰ�ؽ�ֵ
UINT CAbstractRobotController::GetActualJointPos(double * jointpos)
{
	if(TRUE==m_Socket->m_bClientSocketConnecting)
	{
		//m_csURCtrl.Lock();
		for(int i=0;i<6;i++)
			jointpos[i] = m_RobotParam->m_PackData.q_actual[i];
		//m_csURCtrl.Unlock();
		return ROBOT_WORKING_CORRECT;
	}
	else
		return ROBOT_SOCKET_DISCONNECT;
}

// ��ȡ��ǰ��������
UINT CAbstractRobotController::GetActualTcpPos(double * tcppos)
{

	if(TRUE==m_Socket->m_bClientSocketConnecting)
	{
		//m_csURCtrl.Lock();
		for(int i=0;i<6;i++)
			tcppos[i]=m_RobotParam->m_PackData.toolVector_actual[i];
		//m_csURCtrl.Unlock();
		return ROBOT_WORKING_CORRECT;
	}
	else
		return ROBOT_SOCKET_DISCONNECT;

}

// ��ȡĩ�˹ؽڵ���̬����
UINT CAbstractRobotController::GetActualJointMartrix(double* martrix)
{
	double Tcp_Pose[6];

	if (FALSE == m_Socket->m_bClientSocketConnecting)
	{
		return ROBOT_SOCKET_DISCONNECT;
	}
	GetActualTcpPos(Tcp_Pose);
	

	double radius = sqrt(Tcp_Pose[3]*Tcp_Pose[3] + Tcp_Pose[4]*Tcp_Pose[4] + Tcp_Pose[5]*Tcp_Pose[5]);

	double R_X = Tcp_Pose[3] / radius;
	double R_Y = Tcp_Pose[4] / radius;
	double R_Z = Tcp_Pose[5] / radius;

	double S = sin(radius);
	double C = cos(radius);
	double V = 1-C;

	martrix[0] = R_X * R_X * V + C;
	martrix[1] = R_Y * R_X * V - R_Z * S;
	martrix[2] = R_Z * R_X * V + R_Y * S;
	martrix[3] = Tcp_Pose[0];

	martrix[4] = R_X * R_Y * V + R_Z * S;
	martrix[5] = R_Y * R_Y * V + C;
	martrix[6] = R_Z * R_Y * V - R_X * S;
	martrix[7] = Tcp_Pose[1];

	martrix[8] = R_X * R_Z * V - R_Y * S;
	martrix[9] = R_Y * R_Z * V + R_X * S;
	martrix[10] = R_Z * R_Z * V + C;
	martrix[11] = Tcp_Pose[2];

	martrix[12] = 0;
	martrix[13] = 0;
	martrix[14] = 0;
	martrix[15] = 1;

	return ROBOT_WORKING_CORRECT;
}

// ����tcpλ������
UINT CAbstractRobotController::SetTcpPos(double tcpPos[3])
{
// 	if(CheckRbtState()!=ARM_INITIALIZED_OR_RUNNUNG || CheckRbtState()!=ARM_BRAKE_RELEASE)
// 		return ROBOT_MOVE_STATE_ERROR;
// 	if(tcpPos==NULL)
// 		return ROBOT_MOVE_TARGET_POS_ERROR;
	if(FALSE == m_Socket->m_bClientSocketConnecting)
		return ROBOT_SOCKET_DISCONNECT;

	CString str;
	CString tcp("p[");
	CString program;

	for(int i=0;i<3;i++)
	{
		str.Format(_T("%lf"),tcpPos[i]/1000);
		tcp=tcp+str+_T(",");
	}
	tcp += "0,0,0]";

	program = "set_tcp(";
	program += tcp;
	program += ")\n";
	m_Socket->ClientSend(program.GetBuffer(),program.GetLength());
	Sleep(100);

	return ROBOT_WORKING_CORRECT;
}

// ����tcpλ���������̬
UINT CAbstractRobotController::SetTcp(double tcp[6])
{
	if(CheckRbtState()!=ARM_INITIALIZED_OR_RUNNUNG || CheckRbtState()!=ARM_BRAKE_RELEASE)
		return ROBOT_MOVE_STATE_ERROR;
	if(tcp==NULL)
		return ROBOT_MOVE_TARGET_POS_ERROR;

	CString str;
	CString tcpStr("p[");
	CString program;

	for(int i=0;i<3;i++)
	{
		str.Format(_T("%lf"),tcp[i]/1000);
		tcpStr=tcpStr+str+_T(",");
	}

	for(int i=3;i<5;i++)
	{
		str.Format(_T("%lf"),tcp[i]);
		tcpStr=tcpStr+str+_T(",");
	}

	str.Format(_T("%lf"),tcp[5]);
	tcpStr=tcpStr+str+_T("]");

	program = "set_tcp(";
	program += tcpStr;
	program += ")\n";
	m_Socket->ClientSend(program.GetBuffer(),program.GetLength());
	Sleep(100);

	return ROBOT_WORKING_CORRECT;
}

// ��ȡ��ǰ��������ϵ�¸�������������ϵ��
UINT CAbstractRobotController::GetActualTcpForce(double * tcpforce)
{
	if(TRUE == m_Socket->m_bClientSocketConnecting)
	{
		m_csURCtrl.Lock();
		for(int i=0; i<6; i++)
			tcpforce[i] = m_RobotParam->m_PackData.tcpForce[i];
		m_csURCtrl.Unlock();
		return ROBOT_WORKING_CORRECT;
	}
	else
		return ROBOT_SOCKET_DISCONNECT;
}

// ��ȡ��ǰ��������
UINT CAbstractRobotController::GetTipData(float * tcppos)
{
	if(TRUE == m_Socket->m_bClientSocketConnecting)
	{
		//m_csURCtrl.Lock();
		for(int i=0; i<3; i++)
			tcppos[i]=(float)m_RobotParam->m_PackData.toolVector_actual[i];
		//m_csURCtrl.Unlock();
		return ROBOT_WORKING_CORRECT;
	}
	else
		return ROBOT_SOCKET_DISCONNECT;
}

// ��ȡ�����볯��
UINT CAbstractRobotController::GetActualPinVector(double* vect)
{

	double Tcp_Pose[6];

	if (FALSE == m_Socket->m_bClientSocketConnecting)
	{
		return ROBOT_SOCKET_DISCONNECT;
	}
	m_csURCtrl.Lock();
	GetActualTcpPos(Tcp_Pose);
	m_csURCtrl.Unlock();

	double radius = sqrt(Tcp_Pose[3]*Tcp_Pose[3] + Tcp_Pose[4]*Tcp_Pose[4] + Tcp_Pose[5]*Tcp_Pose[5]);

	double R_X = Tcp_Pose[3] / radius;
	double R_Y = Tcp_Pose[4] / radius;
	double R_Z = Tcp_Pose[5] / radius;

	double S = sin(radius);
	double C = cos(radius);
	double V = 1-C;

	vect[0] = R_X * R_X * V + C;
	vect[1] = R_X * R_Y * V + R_Z * S;
	vect[2] = R_X * R_Z * V - R_Y * S;

	return ROBOT_WORKING_CORRECT;
}

// ��ȡ��β����
UINT CAbstractRobotController::GetRootData(float * rootdata)
{
	float tip[3];
	GetTipData(tip);
	double vect[3];
	KinematicsMatrix(vect);
	GetActualPinVector(vect);
	rootdata[0] = (float)(tip[0] - vect[0] * pinlength);
	rootdata[1] = (float)(tip[1] - vect[1] * pinlength);
	rootdata[2] = (float)(tip[2] - vect[2] * pinlength);

	return ROBOT_WORKING_CORRECT;
}

// ��ȡ�����β����
UINT CAbstractRobotController::GetTipRootData(float* tip,float* root)
{
	UINT bOK;

	bOK = GetTipData(tip);

	if(bOK != ROBOT_WORKING_CORRECT)
		return ROBOT_SOCKET_DISCONNECT;

	bOK = GetRootData(root);

	if(bOK != ROBOT_WORKING_CORRECT)
		return ROBOT_SOCKET_DISCONNECT;

	return ROBOT_WORKING_CORRECT;

}

// ��ȡ���⹤������
UINT CAbstractRobotController::GetKinematicsTipData(double *tipdata, double *m_fTcpCenter_RegRBT2MTC)			//������� markԭ��
{
	double lfRootdata[3];

	if (FALSE == m_Socket->m_bClientSocketConnecting)
		return ROBOT_SOCKET_DISCONNECT;

	Kinematics(lfRootdata,m_fTcpCenter_RegRBT2MTC);

	for (int i=0;i<3;i++)
		tipdata[i] = lfRootdata[i];

	return ROBOT_WORKING_CORRECT;
}

// ��ȡ������β����
UINT CAbstractRobotController::GetKinematicsRootData(double *rootdata, double *m_fTcpCenter_Root)							   // ��ȡ������β����
{
	double lfRootdata[3];

	if (FALSE==m_Socket->m_bClientSocketConnecting)
		return ROBOT_SOCKET_DISCONNECT;

	Kinematics(lfRootdata,m_fTcpCenter_Root);

	for (int i=0;i<3;i++)
		rootdata[i] = lfRootdata[i];

	return ROBOT_WORKING_CORRECT;
}

// ��ȡ�ؽ��˶�ѧ���
UINT CAbstractRobotController::GetPosInverse(double Joint[6],double* Pos)
{
	double mTarget[6];
	if(Pos!=NULL)
	{
		mTarget[0]=Pos[0]/1000.0;
		mTarget[1]=Pos[1]/1000.0;
		mTarget[2]=Pos[2]/1000.0;
		mTarget[3]=Pos[3];
		mTarget[4]=Pos[4];
		mTarget[5]=Pos[5];
	}
	else
		return ROBOT_MOVE_TARGET_POS_ERROR;

	WORD sockVersion = MAKEWORD(2,2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion,&wsaData) != 0)
	{
		AfxMessageBox("�����ʼ��ʧ�ܣ�");
		return WSAGetLastError();
	}

	SOCKET server;
	server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(7777);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	if(bind(server,(LPSOCKADDR)&sin,sizeof(sin)) == SOCKET_ERROR)
	{
		AfxMessageBox("�����������ַ��ʧ��!");
		return WSAGetLastError();
	}

	if(listen(server,1) == SOCKET_ERROR)
	{
		AfxMessageBox("�������������ʧ��!");
		return WSAGetLastError();
	}

	u_long nonblocking = 1;
	ioctlsocket(server,FIONBIO,&nonblocking);

	SOCKET sClient = INVALID_SOCKET;
	sockaddr_in ClientAddr;
	nonblocking = 1;
	struct fd_set list;
	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	int Len_Addr = sizeof(ClientAddr);

	FD_ZERO(&list);
	FD_SET(server,&list);

	////////////////////////////////////////////////////////

	CString str;
	CString pose;
	for(int i=0;i<5;i++)
	{
		str.Format(_T("%lf"),mTarget[i]);
		pose=pose+str+_T(",");
	}
	str.Format(_T("%lf"),mTarget[5]);
	pose=pose+str;

	CString program;
	//���뺯�����壬�ں�������end��30003�˿ڳ����Զ�ִ�иú���
	program="def InverseGet():\nsocket_close(\"Command\")\npos=p["+pose+"]\n";
	str="Rx =pos[3]\n" 
		"Ry =pos[4]\n"
		"Rz = pos[5]\n"
		"rotat2 = pose_trans(p[0,0,0,0,0,Rz],p[0,0,0,0,Ry,0])\n"
		"rotat3 = pose_trans(rotat2,p[0,0,0,Rx,0,0])\n"
		"global temp_pos = pos\n"
		"temp_pos[3] = rotat3[3]\n"
		"temp_pos[4] = rotat3[4]\n"
		"temp_pos[5] = rotat3[5]\n"
		"textmsg(temp_pos)\n";
	program = program + str;
	program = program + "joint = get_inverse_kin(temp_pos)\n";
	program = program + "socket_open(\"192.168.1.105\",7777,\"Command\")\n";
	program = program + "socket_send_string(joint,\"Command\")\n";
	program = program + "socket_close(\"Command\")\n";
	program = program + "end\n";

	m_Socket->ClientSend(program.GetBuffer(),program.GetLength());

	///////////////////////////////////////////////////////

	if (select(server+1,&list,0,0,&timeout) == 0)
	{
	//	AfxMessageBox("�ȴ��ͻ������ӳ�ʱ��");
		closesocket(server);
		return ROBOT_MOVE_TARGET_POS_ERROR;
	}

	sClient = accept(server,(SOCKADDR*)&ClientAddr,&Len_Addr);

	if (INVALID_SOCKET == sClient)
	{
		AfxMessageBox("��Ч�Ŀͻ�������");
		closesocket(server);
		return ROBOT_MOVE_TARGET_POS_ERROR;
	}

	ioctlsocket(sClient,FIONBIO,&nonblocking);

	FD_ZERO(&list);
	FD_SET(sClient,&list);

	if (select(sClient+1,&list,0,0,&timeout) == 0)
	{
		AfxMessageBox("�ȴ����ݳ�ʱ!");
		closesocket(sClient);
		closesocket(server);
		return ROBOT_MOVE_TARGET_POS_ERROR;
	}

	char revData[255];
	memset(revData,0,255);
	int ret = recv(sClient,revData,254,0);

	if (ret <= 0)
	{
		AfxMessageBox("��������ʧ�ܣ�");
		closesocket(sClient);
		closesocket(server);
		return ROBOT_MOVE_TARGET_POS_ERROR;
	}

	for (int i =0;i<6;i++)
	{
		if (0 == i)
		{
			Joint[i] = atof(strtok(revData,"[,]"));
		}
		else
		{
			Joint[i] = atof(strtok(NULL,"[,]"));
		}
	}

	closesocket(sClient);
	closesocket(server);

	WSACleanup();

	return ROBOT_WORKING_CORRECT;
}

//�˶�ѧ���⣬���ݳ�ʼDH�������Լ������ؽڵ��˶������tcp����̬����
UINT CAbstractRobotController::Kinematics(double * targetpos,double * tcpcenter)
{
	double jointPos[6];
	double T[16];
	//double m_DHValue[27] = {0,WK_PI/2,89.159,0,-425,0,0,0,-392.25,0,0,0,0,WK_PI/2,109.15,0,0,-WK_PI/2,94.65,0,0,0,82.3,0,-15.5885,0,115};
	double dhValue[27];
	for (int i=0;i<27;i++)
	{
		dhValue[i] = m_RobotParam->m_DHValue[i];
	}
	dhValue[1] = PI/dhValue[1];
	dhValue[13] = PI/dhValue[13];
	dhValue[17] = PI/dhValue[17];

	double lfCoordinate[16] = {1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1};	
	double lfResult[16];
	double lfTcp[4];
	double lfRobotPos[4];
	if (m_Socket->m_bClientSocketConnecting == FALSE)
	{
		return ROBOT_SOCKET_DISCONNECT;
	}
	m_csURCtrl.Lock();
	for (int i=0;i<6;i++)
	{
		jointPos[i] = m_RobotParam->m_PackData.q_actual[i];
	}
	m_csURCtrl.Unlock();

	for (int j=0;j<6;j++)
	{
		T[0] = cos(dhValue[j*4+3]+jointPos[j]);
		T[1] = -sin(dhValue[j*4+3]+jointPos[j])*cos(dhValue[j*4+3-2]);
		T[2] = sin(dhValue[j*4+3]+jointPos[j])*sin(dhValue[j*4+3-2]);
		T[3] = dhValue[j*4+3-3]*cos(dhValue[j*4+3]+jointPos[j]);

		T[4] = sin(dhValue[j*4+3]+jointPos[j]);
		T[5] = cos(dhValue[j*4+3]+jointPos[j])*cos(dhValue[j*4+3-2]);
		T[6] = -cos(dhValue[j*4+3]+jointPos[j])*sin(dhValue[j*4+3-2]);
		T[7] = dhValue[j*4+3-3]*sin(dhValue[j*4+3]+jointPos[j]);

		T[8] = 0;
		T[9] = sin(dhValue[j*4+3-2]);
		T[10] = cos(dhValue[j*4+3-2]);
		T[11] = dhValue[j*4+3-1];

		T[12] = 0;
		T[13] = 0;
		T[14] = 0;
		T[15] = 1;

		cuMatrixMultiply(lfCoordinate,T,lfResult,4,4,4);
		for (int m=0;m<16;m++)
		{
			lfCoordinate[m] = lfResult[m];
		}		
	}

	lfTcp[0] = tcpcenter[0];
	lfTcp[1] = tcpcenter[1];
	lfTcp[2] = tcpcenter[2];
	lfTcp[3]=1;
	cuMatrixMultiply(lfCoordinate,lfTcp,lfRobotPos,4,4,1);
	targetpos[0] = lfRobotPos[0];
	targetpos[1] = lfRobotPos[1];
	targetpos[2] = lfRobotPos[2];
	return ROBOT_WORKING_CORRECT;
}

//��ȡ��ķ�������
UINT CAbstractRobotController::KinematicsMatrix(double* vect)
{
	double jointPos[6];
	double T[16];
	//double m_DHValue[27] = {0,WK_PI/2,89.159,0,-425,0,0,0,-392.25,0,0,0,0,WK_PI/2,109.15,0,0,-WK_PI/2,94.65,0,0,0,82.3,0,-15.5885,0,115};
	double dhValue[27];
	for (int i=0;i<27;i++)
	{
		dhValue[i] = m_RobotParam->m_DHValue[i];
	}
	double lfCoordinate[16]={1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1};	
	double lfResult[16];
	if (m_Socket->m_bClientSocketConnecting==FALSE)
	{
		return ROBOT_SOCKET_DISCONNECT;
	}
	m_csURCtrl.Lock();
	for (int i=0;i<6;i++)
	{
		//m_fJointPos[i]=jointpos[i];
		jointPos[i]= m_RobotParam->m_PackData.q_actual[i];
	}
	m_csURCtrl.Unlock();

	for (int j=0;j<6;j++)
	{
		T[0]=cos(dhValue[j*4+3]+jointPos[j]);
		T[1]=-sin(dhValue[j*4+3]+jointPos[j])*cos(dhValue[j*4+3-2]);
		T[2]=sin(dhValue[j*4+3]+jointPos[j])*sin(dhValue[j*4+3-2]);
		T[3]=dhValue[j*4+3-3]*cos(dhValue[j*4+3]+jointPos[j]);

		T[4]=sin(dhValue[j*4+3]+jointPos[j]);
		T[5]=cos(dhValue[j*4+3]+jointPos[j])*cos(dhValue[j*4+3-2]);
		T[6]=-cos(dhValue[j*4+3]+jointPos[j])*sin(dhValue[j*4+3-2]);
		T[7]=dhValue[j*4+3-3]*sin(dhValue[j*4+3]+jointPos[j]);

		T[8]=0;
		T[9]=sin(dhValue[j*4+3-2]);
		T[10]=cos(dhValue[j*4+3-2]);
		T[11]=dhValue[j*4+3-1];

		T[12]=0;
		T[13]=0;
		T[14]=0;
		T[15]=1;

		cuMatrixMultiply(lfCoordinate,T,lfResult,4,4,4);
		for (int m=0;m<16;m++)
		{
			lfCoordinate[m]=lfResult[m];
		}		
	}

	vect[0] = lfCoordinate[0];
	vect[1] = lfCoordinate[4];
	vect[2] = lfCoordinate[8];

	return ROBOT_WORKING_CORRECT;
}

void CAbstractRobotController::cuMatrixMultiply(double *A, double *B, double *C, int N, int L, int M)
{
	int i,j,k,ka,kb,kt;
	for(i=0; i<N; i++)
	{
		for(j=0; j<M; j++)
		{
			kt=i*M+j;
			*(C+kt)=0.0;
			for(k=0; k<L; k++)
			{
				ka=i*L+k;
				kb=k*M+j;
				//**********&&&&&&&&&&&&&&&&&&&&&&&&&******************
				*(C+kt)=(*(A+ka))*(*(B+kb))+(*(C+kt));  
				//**********&&&&&&&&&&&&&&&&&&&&&&&&&******************
			}
		}
	}
}
