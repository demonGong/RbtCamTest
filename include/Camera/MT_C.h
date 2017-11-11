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
	
	//��ʼ����ر������غ���
	void initialINIAccess();
	bool setupCameras();
	void detach();

	//makrer�Ĳ�������
	void refreshMarkerTemplates();
	int getMarkerTemplatesCount();
	void getMarkerTemplateItemName(int index,string &name);
	CString getCurrMarkerName();
	void changeTemplateName(int index, string oldName, string newName);
	void saveMarkerTemplate(string name);
	void deleteTemplate(string Name);

	//֡�ɼ�������
	void startGrabingFrames();
	void AdjustLigtCoolnessByCoolCard();
	void processCurrFrame();
	
	//����ͼ��DC
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

	//�ж�ָ��marker�Ƿ���marker�б����ʵʱ�ɼ���marker������
	int findMarkerInSet(const char* name);
	int findMarkerInCollection(const char* name,Collection *pMarkersCollection);

	//δע��marker�ɼ����溯��
	void setNewName(CString inputNewMarkName);
	void collectNewSamples();
	bool stopSampling(char* name);
	void saveSample();

	//̽�����Ĳɼ�����������Ⱥ���
	bool collectMarksPose(CString TTBlockName,CString ToolTipName);
	bool stopToolTipSampling(CString ToolTipNam);
	void GetToolTipPos(CString toolTipName, double *pos2M, double *pos2C);
	void getToolTipMeasureErrors(double *errors);//0 ƽ����1����׼�2��������
	void getToolAvePos(double *pos, int nums);
	void getCurrMarkerANYPos(double *pos,double *posC);
	bool getFacetVector(int markernum, double p_vecPos[][3]);
	bool getMarkerVectorAndPose(CString markName, double p_vecPos[][3],double center[3],double RotateMat[9]);
	bool getMarkerPosByName(CString markName,double *pos, double *RotateVector);
	bool getAveMarkerPosByName(CString markName, double *pos, double *RotateVector, int samplingTimes = 200);
	//�����ļ���
	double findDistance(double* v1, double* v2);//SI
	double dotProductVectors(double* v1, double* v2);//SI

	//��������㵽marker�ļ�
	void savePointsInMarkerFile(CString pos[]);
	void savePointsInMarkerFile(char *filePath,const vector<double> pts);

	//�����õ�Marker�ļ��ж���������Marker������
	void getAllAdditionPoints(char *filePath, vector<double> &pointsInMarker);

	//�������ܣ�ʵʱ��ȡĳ��Marker���꣬��ȡMarker�ļ�����ȡ�����������Marker�����꣬�Լ������Camera������
	//���������filePath ��ĳ��Marker�ļ���·��;
	//���������markerName ��ĳ��Marker������;
	//���������pointsInMarker ��Marker�ļ��б���Ķ�������Marker����ϵ������;
	//���������pointsInCamera ��Marker�ļ��б���Ķ�������Camera����ϵ������;
	//���أ�void
	void getAllAdditionPointsPos(const char *filePath, const char *markerName, vector<double> &pointsInMarker, vector<double> &pointsInCamera);

	//�õ�ͼ��������Xpoint������
	void getAllKeyPointsPos(vector<vector<double>> &keyPoints, int XpointsNum);

	//vector�ͷ��ڴ�,����ʹ��ģ�庯��
	void  deleteVectors();
//˽�г�Ա����
private:
	//MicronTrack��C++�����
	Persistence* pPers;
	Markers* pMarkers;
	Cameras* pCameras;
	MCamera* pCurrCam;
	MTMarker::Marker* pCurrMarker;
	MTXPoint* pCurrXPoint;
	XPoints* pXPoints;

	//��ǰ�豸�������������ڻ�ͼ
	CDC *m_pDC;
	double coodOnLRImage[2][3][2][2];

//���г�Ա����
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


	CString m_InputMakerName;	//�������marker����
	int m_collectedSampleFrames;//�Ѳɼ�����������

	vector<MTCollection::Collection*> sampleVectors; //δ֪marker�ɼ���������
	vector<Xform3D*> facet1ToCameraXfs;			
	vector<Xform3D*> ToolTipToMarkerXfs;	//�ɼ�����������̽������ϵ����̬��
	vector<Xform3D*> TestMarkerToCamXfs;
	double measureError[3];					//ͳ�Ʋɼ��������

	double realNum[3];					//���ʵ��ƽ������λ��


//���ܿ��ر���
public:
	bool isCameraOpened;		//����Ƿ��
	bool isShowingVectors;		//�Ƿ���ʾmarker������
	bool isShowingXpoints;		//�Ƿ���ʾ��⵽��Xpoints
	bool isShowingHalfSize;		//�Ƿ���ʾ��߷ֱ��ʵ�һ��
	bool isDisplayEnabled;		//��ʾʹ��
	bool isMarkersProcessingEnabled;	//����markerʹ��
	bool isXPointsProcessingEnabled;	//����XPointsʹ��
	bool isHistogramEqualizeImages;		//ͼ��ֱ��ͼ���⻯ʹ��
	bool isHdrModeEnabled;
	bool isBackGroundProcessingEnabled; //��������ʹ��
	bool isCam2CamRegistered;
	bool isSingleWindow;
	bool useOpenGL;
	bool isShowingPositions;			//markerλ����ʾʹ��
	bool isCaptureEnabled;				//����ͼ��ʹ��
	bool isShowingAngleAndDistance; 
	bool isMarkerTemplatesInitialized;	//��ʼ��markerģ�弯��
	bool isOptionsInitialized;
	bool autoLightCoolness;				
	bool isAdditionalFacetAdding;		//�ɼ�һ��marker�Ķ��facetʹ��
	bool isCollectingSamples;			//�ɼ�marker����ʹ��

};

#endif