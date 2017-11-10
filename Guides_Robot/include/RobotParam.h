#pragma once


//��������
const double JOINT_MOVE_ACC = 0.3;         
const double JOINT_MOVE_SPEED = 0.2;       
const double JOINT_MOVE_TIME = 0;        
const double SPEED_AROUND_X = 0.1;
const double SPEED_ALONG_X =  0.02;  
const double PI = 3.141592654;
const double MAX_FORCE = 0.5;
const double MIN_FORCE = 30;
const double MAX_WAITING_TIME = 1000;
const double MASS_PAYLOAD = 0.0;


//�������汾��Ŀǰ֧��CB2,CB3��CB3.2
enum ControllerVision
{
	VISION_CB2 = 1,
	VISION_CB3,
	VISION_CB3X2
};

//ÿ���汾�Ľ�������
enum PackParaLength
{
	CB2_PACKPARALENGTH   = 812,
	CB3_PACKPARALENGTH   = 1044,
	CB3X2_PACKPARALENGTH = 1060,
	MAX_PACK_LENGTH      = 2000
};

//��е��״̬
enum RobotMode
{
	NO_CONTROLLER_MODE = -1,
	ROBOT_RUNNING_MODE,
	ROBOT_FREEDRIVE_MODE,
	ROBOT_READY_MODE,
	ROBOT_INITIALIZING_MODE,  
	ROBOT_SECURITY_STOPPED_MODE,
	ROBOT_EMERGENCY_STOPPED_MODE,
	ROBOT_FAULT_MODE,
	ROBOT_NO_POWER_MODE,
	ROBOT_NOT_CONNECTED_MODE,
	ROBOT_SHUTDOWN_MODE
};


//��е��ʵ��״̬
enum ARMMode
{
	///////////// /////////////��е��ʵ��״̬ ///////////////////////////////
	ARM_POWER_OFF = 1,						   //��е�۶ϵ�
	ARM_POWER_ON_OR_EMERGENCY_UP,              //��е�ۿ�����߼�ͣ��ť����
	ARM_BRAKE,								   //��е��δ����
	ARM_BRAKE_RELEASE,					       //��е�۽���            
	ARM_UNINITIALIZED,						   //��ʼ��δ���             
	ARM_INITIALIZED_OR_RUNNUNG,			       //��ʼ����ɻ����˶�����               
	ARM_EMERGENCY_DOWN,				           //��ͣ��ť����                 	                    
	ARM_PROTECTIVE_STOPPED,					   //������ײ���˶�ѧ����⣬���Ｋ��λ����������°�ȫֹͣ             
	ARM_FREEDRIVE,							   //�����˶�
	ARM_TEACH,								   //ʾ��ģʽ	
	ARM_UNKOWN_STATE						   //δ֪״̬
};


//��е��ʵ�ʴ���״̬
enum ARMErrorMode
{
	ROBOT_SOCKET_DISCONNECT = -1,
	ROBOT_WORKING_CORRECT,
	ROBOT_MOVE_TARGET_POS_ERROR,
	ROBOT_POWER_ON_ERROR,
	ROBOT_POWER_DOWN_ERROR,
	ROBOT_POWER_OFF_ERROR,
	ROBOT_BRAKE_RELEASE_ERROR,
	ROBOT_FREEDRIVE_OR_TEACH_ERROR,
	ROBOT_INITIALIZED_OR_RUNNUNG_ERROR,
	ROBOT_MOVE_STATE_ERROR
};


//�ؽ�״̬
enum JointMode
{
	JOINT_MODE_BAR = 235,
	JOINT_SHUTTING_DOWN_MODE,
	JOINT_PART_D_CALIBRATION_MODE,
	JOINT_BACKDRIVE_MODE,
	JOINT_POWER_OFF_MODE,
	JOINT_EMERGENCY_STOPPED_MODE,
	JOINT_CALVAL_INITIALIZATION_MODE,
	JOINT_ERROR_MODE,
	JOINT_BACKDRIVEABLE_MODE,
	JOINT_SIMULATED_MODE,
	JOINT_NOT_RESPONDING_MODE,
	JOINT_MOTOR_INITIALISATION_MODE,
	JOINT_ADC_CALIBRATION_MODE,				  //CB2
	JOINT_BOOTING_MODE = 247,				  //CB3
	JOINT_DEAD_COMMUTATION_MODE,			  //CB2
	JOINT_PART_D_CALIBRATION_ERROR_MODE = 248,//CB3
	JOINT_BOOTLOADER_MODE,
	JOINT_CALIBRATION_MODE,
	JOINT_SECURITY_STOPPED_MODE,
	JOINT_FAULT_MODE,
	JOINT_RUNNING_MODE,
	JOINT_INITIALISATION_MODE,
	JOINT_IDLE_MODE

};

//������״̬
enum RobotModeCB3
{
	ROBOT_MODE_NO_CONTROLLER  = -1,
	ROBOT_MODE_DISCONNECTED,
	ROBOT_MODE_CONFIRM_SAFETY,
	ROBOT_MODE_BOOTING,
	ROBOT_MODE_POWER_OFF,
	ROBOT_MODE_POWER_ON,
	ROBOT_MODE_IDLE,
	ROBOT_MODE_BACKDRIVE,
	ROBOT_MODE_RUNNING,
	ROBOT_MODE_UPDATING_FIRMWARE

};
//��ȫ״̬
enum SafetyModeCB3
{
	SAFETY_MODE_NORMAL = 1,
	SAFETY_MODE_REDUCED,
	SAFETY_MODE_PROTECTIVE_STOP,
	SAFETY_MODE_RECOVERY,
	SAFETY_MODE_SAFEGUARD_STOP,
	SAFETY_MODE_SYSTEM_EMERGENCY_STOP,
	SAFETY_MODE_ROBOT_EMERGENCY_STOP,
	SAFETY_MODE_VIOLATION,
	SAFETY_MODE_FAULT
};





//���ݰ����������ṹ�������п��ư汾�����ݷ���һ�𣬳�ʼ�������ɸ������ư汾�汾���Լ�ά��
typedef struct tagRobotPackData
{
	int msg_size;				//Total message length in bytes
	double t_time;			//Time elapsed since the controller 	was started

	double q_target[6];		//Target joint positions
	double qd_target[6];		//Target joint velocities
	double qdd_target[6];		//Target joint accelerations
	double i_target[6];		//Target joint currents
	double m_target[6];		//Target joint moments (torques)

	double q_actual[6];		//Actual joint positions
	double qd_actual[6];		//Actual joint velocities
	double i_actual[6];		//Actual joint currents
	double i_control[6];		//Joint control currents
	

	//////////define just in CB2 begin////////////////////////

	double Tool_accelerometer_values[3];	//Tool x,y and z accelerometer values

	double msg_unused[15];			//Unused

	double toolVector[6];				//Cartesian coordinates of the tool:
	//(x,y,z,rx,ry,rz), where rx, ry and rz
	//is a rotation vector representation
	//of the tool orientation

	double tcpSpeed[6];				//Speed of the tool given in cartesian	coordinates

	//////////define just in CB2 end//////////////////

	double toolVector_actual[6];	//Actual Cartesian coordinates of the tool: (x,y,z,rx,ry,rz), 
	//where rx, ry and rz is a rotation vector representation of the tool orientation

	double tcpSpeed_actual[6];		//Actual speed of the tool given in Cartesian coordinates

	double tcpForce[6];			//Generalised forces in the TCP

	double toolVector_target[6];	//Target Cartesian coordinates of the tool: (x,y,z,rx,ry,rz), 
								//where rx, ry and rz is a rotation vector representation of the tool orientation

	double tcpSpeed_target[6];		//Target speed of the tool given in Cartesian coordinates

	double stateDigitalInput;		//Current state of the digital inputs.
								//NOTE: these are bits encoded as
								//int64_t, e.g. a value of 5
								//corresponds to bit 0 and bit 2 set  high

	double jointMotorTemperature[6];	//Temperature of each joint in	degrees celcius

	double controller_timer;			//Controller realtime thread execution time

	double test_value;				//A value used by Universal Robots 	software only

	double robotMode;					//Robot control mode
	double jointMode[6];				//Joint control modes

	double safetyMode;				//Safety mode
	double URUsedA[6];				//Used by Universal Robots software only
	double toolAccelerometer_values[3];	//Tool x,y and z accelerometer values (software version 1.7)
	double URUsedB[6];				//Used by Universal Robots software only

	double speedScaling;				//Speed scaling of the trajectory limiter

	double linearMomentum_norm;		//Norm of Cartesian linear momentum;

	double URUsedC;					//Used by Universal Robots software only
	double URUsedD;					//Used by Universal Robots software only

	double mainVoltage;				//Masterboard: Main voltage
	double robotVoltage;				//Masterboard: Robot voltage (48V)
	double robotCurrent;				//Masterboard: Robot current

	double jointVoltages_actual[6];		//Actual joint voltages
}RobotPackData;


//�����˲���
typedef struct tagRobotParam
{
	RobotPackData m_PackData;

	float m_pinLen;                                        // �����볤��
	float m_pinRaidus;                                     // ������뾶
	float m_DHValue[27];								   // ������DH����	

}RobotParam;