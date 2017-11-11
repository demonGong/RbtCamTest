#ifndef __CMT_C_H__
#define __CMT_C_H__


#include "Markers.h"
#include <string.h>
#include "Marker.h"
#include "XPoints.h"
#include "MTXPoint.h"
#include "Persistence.h"
#include "Cameras.h"
#include "Facet.h"
#include "Xform3D.h"
#include "MTC.h"
#include <math.h>
#include <stdio.h>
#include <vector>




using namespace std;
using namespace MTMarker;
using namespace MTCollection;

class EXT_CLASS CMT_C
{
public:
	CMT_C(void);
	virtual ~CMT_C(void);
	
	//初始化或关闭相机相关函数
	void initialINIAccess();
	bool setupCameras();
	void detach();

	//makrer的操作函数
	void refreshMarkerTemplates();
	int getMarkerTemplatesCount();
	void getMarkerTemplateItemName(int index,string &name);
	CString getCurrMarkerName();
	void changeTemplateName(int index, string oldName, string newName);
	void saveMarkerTemplate(string name);
	void deleteTemplate(string Name);

	//帧采集处理函数
	void startGrabingFrames();
	void AdjustLigtCoolnessByCoolCard();
	void processCurrFrame();
	
	//绘制图像到DC
	void setMTDC(CDC *pDC);
	void showIdentifiedMarkers(CDC *pDC, bool isDrawingText = true);
	void showIdentifiedXPoints(CDC *pDC);
	void FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp );
	void DrawToPic( CDC *pDC, RECT* pDstRect, bool isDrawingText, int pic = 0);
	void DisplayLRImages();
	void DisplayLRImages(int Side);
	void drawImage(int x, int y, int width, int height, unsigned char *pixels, bool firstRowAtBottom = false );
	void drawImages(bool left, bool right, bool middle);
	void drawLine(CDC *pDC,int side, float x1, float y1, float x2, float y2, float r, float g, float b, int stipple);
	void drawCircle(CDC *pDC,int side, float x, float y, float radius, float r, float g, float b, int stipple);
	void drawText( CDC *pDC,char* text, int X, int Y);
	void saveLRImages(CString lImageName,CString rImageName);
	void ClearWindow( );

	//判断指定marker是否在marker列表或者实时采集的marker集合中
	int findMarkerInSet(const char* name);
	int findMarkerInCollection(const char* name,Collection *pMarkersCollection);

	//未注册marker采集保存函数
	void setNewName(CString inputNewMarkName);
	void collectNewSamples();
	bool stopSampling(char* name);
	void saveSample();

	//探针针尖的采集保存计算误差等函数
	bool collectMarksPose(CString TTBlockName,CString ToolTipName);
	bool stopToolTipSampling(CString ToolTipNam);
	void GetToolTipPos(CString toolTipName, double *pos2M, double *pos2C);
	void getToolTipMeasureErrors(double *errors);//0 平均误差，1，标准差，2，最大误差
	void getToolAvePos(double *pos, int nums);
	void getCurrMarkerANYPos(double *pos,double *posC);
	bool getFacetVector(int markernum, double p_vecPos[][3]);
	bool getMarkerVectorAndPose(CString markName, double p_vecPos[][3],double center[3],double RotateMat[9]);
	bool getMarkerPosByName(CString markName,double *pos, double *RotateVector);
	bool getAveMarkerPosByName(CString markName, double *pos, double *RotateVector, int samplingTimes = 200);
	//向量的计算
	double findDistance(double* v1, double* v2);//SI
	double dotProductVectors(double* v1, double* v2);//SI

	//保存任意点到marker文件
	void savePointsInMarkerFile(CString pos[]);
	void savePointsInMarkerFile(char *filePath,const vector<double> pts);

	//仅仅得到Marker文件中额外点相对于Marker的坐标
	void getAllAdditionPoints(char *filePath, vector<double> &pointsInMarker);

	//函数功能：实时获取某个Marker坐标，读取Marker文件，求取额外点的相对于Marker的坐标，以及相对于Camera的坐标
	//输入参数：filePath 是某个Marker文件的路径;
	//输入参数：markerName 是某个Marker的名称;
	//输出参数：pointsInMarker 是Marker文件中保存的额外点对于Marker坐标系的坐标;
	//输出参数：pointsInCamera 是Marker文件中保存的额外点对于Camera坐标系的坐标;
	//返回：void
	void getAllAdditionPointsPos(const char *filePath, const char *markerName, vector<double> &pointsInMarker, vector<double> &pointsInCamera);

	//得到图像上所有Xpoint的坐标
	void getAllKeyPointsPos(vector<vector<double>> &keyPoints, int XpointsNum);

	//vector释放内存,可以使用模板函数
	void  deleteVectors();
//私有成员变量
private:
	//MicronTrack的C++类变量
	Persistence* pPers;
	Markers* pMarkers;
	Cameras* pCameras;
	MCamera* pCurrCam;
	MTMarker::Marker* pCurrMarker;
	MTXPoint* pCurrXPoint;
	XPoints* pXPoints;

	//当前设备环境描述，用于绘图
	CDC *m_pDC;
	double coodOnLRImage[2][3][2][2];

//公有成员变量
public:
	int imgIndex;
	int TextLine;
	int equalizeIndex;
	int vectorIndex;
	int xpointsIndex;
	int imageIndex;
	int MarkersprocessingIndex;
	int XPointsprocessingIndex;
	int HdrModeIndex;
	int BackGroundProcessingIndex;
	int Cam2CamRegIndex;
	vector<int> CamIndexVector;


	CString m_InputMakerName;	//输入的新marker名字
	int m_collectedSampleFrames;//已采集样本的数量

	vector<MTCollection::Collection*> sampleVectors; //未知marker采集的向量集
	vector<Xform3D*> facet1ToCameraXfs;			
	vector<Xform3D*> ToolTipToMarkerXfs;	//采集的针尖相对于探针坐标系的姿态集
	vector<Xform3D*> TestMarkerToCamXfs;
	double measureError[3];					//统计采集针尖的误差

	double realNum[3];					//针尖实际平均坐标位置


//功能开关变量
public:
	bool isCameraOpened;		//相机是否打开
	bool isShowingVectors;		//是否显示marker的向量
	bool isShowingXpoints;		//是否显示检测到的Xpoints
	bool isShowingHalfSize;		//是否显示最高分辨率的一半
	bool isDisplayEnabled;		//显示使能
	bool isMarkersProcessingEnabled;	//处理marker使能
	bool isXPointsProcessingEnabled;	//处理XPoints使能
	bool isHistogramEqualizeImages;		//图像直方图均衡化使能
	bool isHdrModeEnabled;
	bool isBackGroundProcessingEnabled; //背景处理使能
	bool isCam2CamRegistered;
	bool isSingleWindow;
	bool useOpenGL;
	bool isShowingPositions;			//marker位置显示使能
	bool isCaptureEnabled;				//捕获图像使能
	bool isShowingAngleAndDistance; 
	bool isMarkerTemplatesInitialized;	//初始化marker模板集合
	bool isOptionsInitialized;
	bool autoLightCoolness;				
	bool isAdditionalFacetAdding;		//采集一个marker的多个facet使能
	bool isCollectingSamples;			//采集marker样本使能

};

#endif