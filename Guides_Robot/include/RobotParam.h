#pragma once


//常量参数
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


//控制器版本，目前支持CB2,CB3，CB3.2
enum ControllerVision
{
	VISION_CB2 = 1,
	VISION_CB3,
	VISION_CB3X2
};

//每个版本的解析包长
enum PackParaLength
{
	CB2_PACKPARALENGTH   = 812,
	CB3_PACKPARALENGTH   = 1044,
	CB3X2_PACKPARALENGTH = 1060,
	MAX_PACK_LENGTH      = 2000
};

//机械臂状态
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


//机械臂实际状态
enum ARMMode
{
	///////////// /////////////机械臂实际状态 ///////////////////////////////
	ARM_POWER_OFF = 1,						   //机械臂断电
	ARM_POWER_ON_OR_EMERGENCY_UP,              //机械臂开电或者急停按钮弹起
	ARM_BRAKE,								   //机械臂未解锁
	ARM_BRAKE_RELEASE,					       //机械臂解锁            
	ARM_UNINITIALIZED,						   //初始化未完成             
	ARM_INITIALIZED_OR_RUNNUNG,			       //初始化完成或者运动过程               
	ARM_EMERGENCY_DOWN,				           //急停按钮按下                 	                    
	ARM_PROTECTIVE_STOPPED,					   //发生碰撞，运动学无逆解，到达极限位置三种情况下安全停止             
	ARM_FREEDRIVE,							   //自由运动
	ARM_TEACH,								   //示教模式	
	ARM_UNKOWN_STATE						   //未知状态
};


//机械臂实际错误状态
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


//关节状态
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

//机器人状态
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
//安全状态
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





//数据包解析参数结构，把所有控制版本的数据放在一起，初始化工作由各个控制版本版本类自己维护
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


//机器人参数
typedef struct tagRobotParam
{
	RobotPackData m_PackData;

	float m_pinLen;                                        // 穿刺针长度
	float m_pinRaidus;                                     // 穿刺针半径
	float m_DHValue[27];								   // 机器人DH参数	

}RobotParam;