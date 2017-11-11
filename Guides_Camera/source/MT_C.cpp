#include "stdafx.h"
#include "MT_C.h"


CMT_C::CMT_C(void)
{
	this->pPers = new Persistence();
	this->pMarkers = new Markers();

	this->isCameraOpened = false;
	this->isShowingVectors = false;
	this->isShowingXpoints = false;
	this->isShowingHalfSize = false;
	this->isDisplayEnabled = true;
	this->isMarkersProcessingEnabled = true;
	this->isXPointsProcessingEnabled = true;
	this->isHistogramEqualizeImages = false;
	this->isHdrModeEnabled = false;
	this->isBackGroundProcessingEnabled = false;
	this->isCam2CamRegistered = true;
	this->isSingleWindow = false;
	this->useOpenGL =false;
	this->isCollectingSamples = false;
	this->isAdditionalFacetAdding = false;
	this->isShowingPositions = true; 
	this->isCaptureEnabled = true;
	this->isShowingAngleAndDistance = true;
	this->isMarkerTemplatesInitialized = false;
	this->isOptionsInitialized = false;
	this->autoLightCoolness = false;
	this->isAdditionalFacetAdding = false;
	this->isCollectingSamples = false;


	this->pCurrCam = NULL;
	this->pCurrMarker = NULL;
	this->m_pDC = NULL;

	m_InputMakerName = _T("");
	this->m_collectedSampleFrames = 0;

	for (int i = 0; i < 3; i++)
	{
		this->realNum[i] = 0.0;
	}

}


CMT_C::~CMT_C(void)
{
	this->detach();
	this->deleteVectors();
	delete this->pPers;
	delete this->pMarkers;
}

//读取INI相机配置文件，以及创建Marker文件夹
void CMT_C::initialINIAccess()
{

	char currDir[255];
	mtUtils::getCurrPath(currDir);
#ifdef WIN32
	strcat(currDir,"\\MicronTracker.ini");
#else
	strcat(currDir,"/MTDemoCPP.ini");
#endif
	this->pPers->setPath(currDir);

	this->pPers->setSection ("General");
#if 0
	//Setting the FrameInterleave property in the Markers object
	int defaultFrameInterleave = 0;
	this->pMarkers->setPredictiveFramesInterleave( this->pPers->retrieveInt("PredictiveFramesInterleave", defaultFrameInterleave) );
#endif

	//Setting the TemplateMatchToleranceMM property in the Markers object
	double defaultTempMatchToleranceMM = 1.0;
	double defaultLightCoolness = 0.1;

	this->pMarkers->setTemplateMatchToleranceMM( this->pPers->retrieveDouble("TemplateMatchToleranceMM", defaultTempMatchToleranceMM) );
	//this->lightCoolness = this->pPers->retrieveDouble("LightCoolness", defaultLightCoolness);

// 	int s = (this->pPers->retrieveInt("SingleWindow", 1) );
// 	isSingleWindow = (s == 0? true:true);
// 	s = (this->pPers->retrieveInt("UseOpenGL", 1) );
// 	useOpenGL = (s == 0? false:true);
// 	cout << "Open Gl: " << useOpenGL<< endl;

	bool defaultSmallerXPFootprint = true;
	int defaultExtrapolatedFrames = 5;

	bool SmallerXPFootprint = (bool)(this->pPers->retrieveInt("DetectSmallMarkers", defaultSmallerXPFootprint));
	int ExtrapolatedFrames = this->pPers->retrieveInt("ExtrapolatedFrames", defaultExtrapolatedFrames);

	this->pMarkers->setSmallerXPFootprint(SmallerXPFootprint);
	this->pMarkers->setExtrapolatedFrames(ExtrapolatedFrames);

	//could have been clipped
	this->pPers->saveInt("ExtrapolatedFrames", this->pMarkers->getExtrapolatedFrames());

	//Test if Markers dir exists, otherwise create
	char currPath[255];
	mtUtils::getCurrPath(currPath);
#ifdef WIN32
	strcat(currPath, "\\Markers\\");
#else
	strcat(currPath, "/Markers/");
#endif


#ifdef WIN32
	int result = _mkdir(currPath); 
	if (result != 0 && errno != EEXIST) {
		AfxMessageBox("无法创建Marker文件路径，请手动设置");
		return;
	}

#else
	int result = mkdir(currPath, 777); 
	int e = errno;
	if (result != 0 && e != EEXIST) {
		fl_message("Could not create Markers folder! Please create it manually.");
		return;
	}
#endif

}


//查找所有相机设备，并得把当前相机设置为0号相机
bool CMT_C::setupCameras()
{
	this->pCameras = new Cameras();
	int result = this->pCameras->AttachAvailableCameras();

	if (result == 0 &&  this->pCameras->getCount() >= 1 ) {
		//this->pCurrCam = this->pCameras->m_vCameras[0];
		this->pCurrCam = this->pCameras->getCamera(0);
		if (this->pCurrCam->getXRes() > 1200 ) this->isShowingHalfSize = true;
		
	} else {
		AfxMessageBox("无相机接入或者丢失相机标定文件！ ");
		return false;
		//exit(1);
	}
}

//关闭相机
void CMT_C::detach()
{
	this->pCameras->Detach();
}


//更新Marker模板列表
void CMT_C::refreshMarkerTemplates()
{
	vector<string> templateNames;
	mtUtils::getFileNamesFromDirectory( templateNames,"Markers", true);

	int result = 0;
	result = this->pMarkers->clearTemplates();

	char currentFolderPath[MT_MAX_STRING_LENGTH];
	mtCompletionCode st;

	mtUtils::getCurrPath(currentFolderPath);
#ifdef WIN32
	strcat(currentFolderPath, "\\Markers\\");
#else
	strcat(currentFolderPath, "/Markers/");
#endif

	st = Markers_LoadTemplates(currentFolderPath);

	if( st != mtOK)
		AfxMessageBox("链接Marker模板文件失败");

}


//得到所有marker模板的数量
int CMT_C::getMarkerTemplatesCount()
{
	return this->pMarkers->getTemplateCount();

}

//得到index所对应的marker名字
void CMT_C::getMarkerTemplateItemName(int index,string &name)
{
	if (index > this->pMarkers->getTemplateCount() || index <= 0)
		return;

	this->pMarkers->getTemplateItemName(index,name);
}

//获得当前marker的名称
CString CMT_C::getCurrMarkerName()
{
	CString CurrentMarkerName = "";
	if (this->pCurrMarker != NULL)
	{
		CurrentMarkerName = this->pCurrMarker->getName();
	}
	return CurrentMarkerName;
}

//修改模板内一个marker的名称
void CMT_C::changeTemplateName(int index, string oldName, string newName)
{
	//	if (!this->isValid(newName)) // Check to see if this name already exists
	//	{
	//		fl_message("The name you entered already exists. Please select another name.");
	//		return;
	//	}
	int i = -1;
	char currDir[255];
	mtUtils::getCurrPath(currDir);
	char currDir2[255];
	mtUtils::getCurrPath(currDir2);

#ifdef WIN32
	strcat(currDir, "\\Markers\\");
#else
	strcat(currDir, "/Markers/");
#endif
	strcat(currDir, oldName.c_str());

#ifdef WIN32
	strcat(currDir2, "\\Markers\\");
#else
	strcat(currDir2, "/Markers/");
#endif
	strcat(currDir2, newName.c_str());

	// Change the file name in the markers 
	Persistence persistForLoadedMarker;

	// We have to save it back in the template file
	// This approach is trouble! We should hide the handles and keep arrays/list 
	// of C++ object. The higher level should not be aware of handles etc.

	persistForLoadedMarker.setPath( (char*)(currDir2) );
	i = this->pMarkers->setTemplateItemName(index, (char*)(newName.c_str()) );
	this->pMarkers->storeTemplate(index, persistForLoadedMarker.getHandle(),NULL);

	vector<double> points;
	this->getAllAdditionPoints(currDir,points);
	this->savePointsInMarkerFile(currDir2,points);

	SetFileAttributes(currDir,GetFileAttributes(currDir) & ~FILE_ATTRIBUTE_READONLY); //去掉文件只读属性
	int result = remove(currDir); 
	if (result != 0) {// if name change is not succeeded
		AfxMessageBox("重命名失败，请重试.");
		return;
	}

	this->refreshMarkerTemplates();
	
}


//保存marker文件
void CMT_C::saveMarkerTemplate(string name)
{
	char currDir[255];
	mtUtils::getCurrPath(currDir);
#ifdef WIN32
	strcat(currDir, "\\Markers\\");
#else
	strcat(currDir, "/Markers/");
#endif
	strcat(currDir, name.c_str());

	Persistence* newPersistence = new Persistence();
	newPersistence->setPath(currDir);

	int storeResult = this->pCurrMarker->storeTemplate(newPersistence, "");//(char*)(name.c_str()) );

	// Refresh the templates and the GUI list
	this->refreshMarkerTemplates();

	int indexToolTip = this->findMarkerInSet(name.c_str());
	this->pCurrMarker = new Marker(this->pMarkers->getTemplateItem(indexToolTip));
	
	delete newPersistence;

}


//删除marker列表的一个marker文件
void CMT_C::deleteTemplate(string Name)
{
	//	if (!this->isValid(Name)) // Check to see if the file exists
	//	{
	//		return;
	//	}

	// delete the specified template file  
	char currPath[255];
	mtUtils::getCurrPath(currPath);
#ifdef WIN32
	strcat(currPath, "\\Markers\\");
#else
	strcat(currPath, "/Markers/");
#endif
	strcat(currPath, Name.c_str());

	SetFileAttributes(currPath,GetFileAttributes(currPath) & ~FILE_ATTRIBUTE_READONLY); //去掉文件只读属性

	int result = remove(currPath); 
	if (result != 0) {
		AfxMessageBox("删除失败，请重试.");
		return;
	}

	this->refreshMarkerTemplates();
}



// Starts the main window and sets the timer，使用FLTK时才会调用
void CMT_C::startGrabingFrames()
{

	int width = pCurrCam->getXRes() * (this->isShowingHalfSize ? 0.5 : 1);
	int height = pCurrCam->getYRes() * (this->isShowingHalfSize ? 0.5 : 1);
	int mult = 2.0;
	if ( isSingleWindow ) {
		mult = 1.0;
	}

	// 	window = new Fl_Window(width*mult, height+MENU_HEIGHT);
	// 
	// 	Fl::add_timeout(0.03, ctimer_cb, this);
	// 
	// 
	// 	Fl_Menu_Bar* mainMenu = new Fl_Menu_Bar(0, 0, width*mult, MENU_HEIGHT, "");
	// 	MarkersprocessingIndex = mainMenu->add("Activate/ Markers Processing", 0, 0, 0, FL_MENU_TOGGLE|FL_MENU_VALUE);
	// 	XPointsprocessingIndex = mainMenu->add("Activate/ XPoints Processing", 0, 0, 0, FL_MENU_TOGGLE|0);
	// 	HdrModeIndex = mainMenu->add("Activate/ HDR Mode", 0, 0, 0, FL_MENU_TOGGLE|0);
	// 	BackGroundProcessingIndex = mainMenu->add("Activate/ BackGround Processing", 0, 0, 0, FL_MENU_TOGGLE|0);
	// 	Cam2CamRegIndex = mainMenu->add("Activate/ Cam-Cam Registration", 0, 0, 0, FL_MENU_TOGGLE|FL_MENU_VALUE);
	// 	mainMenu->add("Activate/ Marker Templates...", 0, (Fl_Callback*)activateMarkerTemplates, (void *)this);
	// 	mainMenu->add("Activate/ Options...", 0, (Fl_Callback*)activateOptions, (void *)this);
	// 	imageIndex = mainMenu->add("Display/ Image", 0, 0, 0, FL_MENU_TOGGLE|FL_MENU_VALUE);
	// 	mainMenu->add("Display/ Equalize", 0, (Fl_Callback*)SetEqualize, (void *)this, FL_MENU_TOGGLE|FL_MENU_VALUE);
	// 	vectorIndex = mainMenu->add("Display/ Vectors", 0, 0, 0, FL_MENU_TOGGLE|FL_MENU_VALUE);
	// 	xpointsIndex = mainMenu->add("Display/ Xpoints", 0, 0, 0, FL_MENU_TOGGLE|FL_MENU_VALUE);

	char str[MT_MAX_STRING_LENGTH];

	int mIndex, i;
	void **cbuffer = 0;
	for ( i=0 ;i< this->pCameras->getCount();i++) {
		int width = this->pCameras->m_vCameras[i]->getXRes() * (this->isShowingHalfSize ? 0.5 : 1);
		int height = this->pCameras->m_vCameras[i]->getYRes() * (this->isShowingHalfSize ? 0.5 : 1);		
		sprintf(str,"Cameras/ %d  (%d*%d)",this->pCameras->m_vCameras[i]->getSerialNum(), width , height );
		cbuffer =  (void **)malloc(2*sizeof(void*));
		cbuffer[0] = (void*)this;
		cbuffer[1] = (void*) this->pCameras->m_vCameras[i];	
		//	mIndex =  mainMenu->add(str, 0, (Fl_Callback*)SetCurrCamera, (void *)cbuffer, FL_MENU_RADIO|FL_MENU_VALUE);
		CamIndexVector.push_back( mIndex);
	}

	// 	menuArray = (Fl_Menu_Item *)mainMenu->menu();
	// 	if (this->pCameras->getCount() > 1) menuArray[CamIndexVector[0]].setonly();
	// 	window->end();
	// 	window->show();
	// 	Fl::run();
}

//根据颜色板自动调整图像冷光度
void CMT_C::AdjustLigtCoolnessByCoolCard()
{
	MTCollection::Collection* markersCollection = new MTCollection::Collection(this->pMarkers->identifiedMarkers(this->pCurrCam));
	if (markersCollection->count() == 0) {
		delete markersCollection; 
		return;
	}
	int markerNum = 1;
	MTMarker::Marker* m;

	for (markerNum = 1; markerNum <= markersCollection->count(); markerNum++)
	{
		m = new MTMarker::Marker(markersCollection->itemI(markerNum));
		if (m->wasIdentified(this->pCurrCam) )
		{
			if ( strncmp( m->getName(),"COOL",4 ) == 0 ||  
				( strncmp( m->getName(),"cool",4 ) == 0) ||
				( strncmp( m->getName(),"Cool",4 ) == 0)) {
					MTCollection::Collection* facetsCollection = new MTCollection::Collection(m->identifiedFacets(this->pCurrCam));
					for (int facetNum = 1; facetNum <= facetsCollection->count(); facetNum++)
					{
						Facet* f = new Facet(facetsCollection->itemI(facetNum));
						if (facetNum == 1) {
							vector<Vector *> ColorVector;
							ColorVector = (f->IdentifiedVectors());
							if ( ColorVector[0] != NULL) {
								this->pPers->setSection ("General");
								this->pCurrCam->AdjustCoolnessFromColorVector(ColorVector[0]->Handle());
								this->pPers->saveDouble("LightCoolness",this->pCurrCam->getLightCoolness());
							}
							delete ColorVector[0];
							delete ColorVector[1];
						}
						delete f;
					}
					delete facetsCollection;
			}
		}
		free(m);
	}
	delete markersCollection;


}


//处理当前图像
void CMT_C::processCurrFrame()
{
	if (isBackGroundProcessingEnabled)
		this->pMarkers->setBackGroundProcess(true);

	if (this->pMarkers->getBackGroundProcess() == false)
	{
		if (isMarkersProcessingEnabled)
			this->pMarkers->processFrame(NULL);
		if (isXPointsProcessingEnabled)
			this->pXPoints->processFrame(NULL);
	}

	if (isHistogramEqualizeImages)
		this->pCameras->setHistogramEqualizeImages(true);
	
	if (this->isCollectingSamples)
		this->collectNewSamples();

}


//设置设备环境描述表
void CMT_C::setMTDC(CDC *pDC)
{
	this->m_pDC = pDC;

}

//在图像中显示marker名字以及各点位置
void CMT_C::showIdentifiedMarkers(CDC *pDC, bool isDrawingText)
{
	//int reuslt = this->pMarkers->identifiedMarkers(NULL);
	MTCollection::Collection* markersCollection = new MTCollection::Collection(this->pMarkers->identifiedMarkers(NULL)); //this->pCurrCam));

	if (markersCollection->count() == 0) {
		delete markersCollection; 
		return;
	}

	int markerNum = 1;
	int facetNum = 1;
	this->TextLine = 0;
	MTMarker::Marker* m;
	// here we need the left side presentation: coordinates in sensor space, distances etc	
	MTMarker::Marker* marker;
	// here we need the left side presentation: coordinates in sensor space, distances etc	
	Xform3D* Marker2CurrCameraXf = NULL;
	if(this->isShowingPositions)
	{
		for (markerNum = 1; markerNum <= markersCollection->count(); markerNum++)
		{
			marker = new MTMarker::Marker(markersCollection->itemI(markerNum));
			Marker2CurrCameraXf = marker->marker2CameraXf(this->pCurrCam->Handle());
			// There may be a situation where marker was identified by another camera (not CurrCamera)
			// and the identifying camera is not registered with CurrCamera. In this case, the pose is
			// not known in CurrCamera coordinates and Marker2CurrCameraXf is Nothing.
			if(Marker2CurrCameraXf != NULL)
			{
// 				if(this->isShowingXpoints)
// 				{
// 					Facet* ft = new Facet(marker->identifiedFacets(this->pCurrCam));
// 					MTCollection::Collection* c = new MTCollection::Collection(marker->getTemplateFacets());
// 					Vector* LV = (ft->IdentifiedVectors())[0];
// 					Vector* SV = (ft->IdentifiedVectors())[1];
// 					static double XPHistory[3][20][31];
// 					long NextXPI[31] = {0};
// 					double endPos[2][3];
// 					int i;
// 					LV->getEndPos2x3( (double*)endPos);
// 					for ( i=0; i<3; i++)
// 						XPHistory[i][ NextXPI[markerNum] ][markerNum] = endPos[0][i];
// 					double AvgXPI[3] = {0,0,0};
// 					NextXPI[markerNum] =  (NextXPI[markerNum]+1)  % 20;
// 					for (i=0; i<20; i++)
// 					{
// 						AvgXPI[0] = AvgXPI[0] + XPHistory[0][i][markerNum] / 20.0;
// 						AvgXPI[1] = AvgXPI[1] + XPHistory[1][i][markerNum] / 20.0;
// 						AvgXPI[2] = AvgXPI[2] + XPHistory[2][i][markerNum] / 20.0;
// 					}
// #if 0
// 					if (this->isCaptureEnabled)
// 					{ 
// 						cout << "Average: (" << AvgXPI[0] << ", " << AvgXPI[1] << ", " <<  AvgXPI[2] << ")" << endl;
// 					}
// 					else //snapshot
// 					{
// 					}
// #endif
// 					// Show also the averaged location of the LV base in the images
// 					static double XPImgHistory[2][2][20][31];
// 					long NextXPImgI[31] = {0};
// 					double EXP[2][2][2];
// 					LV->getEndXPoints( (double*)EXP );
// 					for(i=0; i<2; i++)
// 					{
// 						XPImgHistory[i][0][ NextXPImgI[markerNum] ][markerNum] = EXP[i][0][0]; // (x/y, base/head, left/right)
// 						XPImgHistory[i][1][ NextXPImgI[markerNum] ][markerNum] = EXP[i][0][1]; // (x/y, base/head, left/right)
// 					}
// 					double AvgXPImg[2][2] = {0};
// 					NextXPImgI[markerNum] = (NextXPImgI[markerNum]+1) % 20;
// 					for (i=0; i<20; i++)
// 					{
// 						AvgXPImg[0][0] = AvgXPImg[0][0] + XPImgHistory[0][0][i][markerNum] / 20.0;
// 						AvgXPImg[0][1] = AvgXPImg[0][1] + XPImgHistory[0][1][i][markerNum] / 20.0;
// 						AvgXPImg[1][0] = AvgXPImg[1][0] + XPImgHistory[1][0][i][markerNum] / 20.0;
// 						AvgXPImg[1][1] = AvgXPImg[1][1] + XPImgHistory[1][1][i][markerNum] / 20.0;
// 					}
// #if 0
// 					if (this->isCaptureEnabled)
// 					{
// 						cout << " L-R imx: " << AvgXPImg[0][0] << "-" << AvgXPImg[0][1] << endl;
// 						cout << " imy: " << AvgXPImg[1][0] << "-" << AvgXPImg[1][1] << endl;
// 					}
// 					else
// 					{
// 						cout << "L-R imx: " << EXP[0][0][0] << "-" << EXP[0][0][1] << endl;
// 						cout << "imy: " << EXP[1][0][0] << "-" << EXP[1][0][1] << endl;
// 					}
// #endif
// 					delete ft; delete c; 
// 				}
// 				else
// 				{
					//Show the XYZ position of the Marker's origin.
					//string s = ".(";
				char buffer[6][100];
				char s[600];
				Xform3D* m2c;
				m2c = marker->marker2CameraXf(this->pCurrCam->Handle());
				for (int i=0; i<3; i++)
				{	
					sprintf(buffer[i], "%.2f",m2c->getShift(i));
				}
				double markerRotateMat[9],markerPose[16];
				m2c->getRotateMat(markerRotateMat);
				markerPose[0] = markerRotateMat[0];
				markerPose[1] = markerRotateMat[3];
				markerPose[2] = markerRotateMat[6];
				markerPose[3] = m2c->getShift(0);

				markerPose[4] = markerRotateMat[1];
				markerPose[5] = markerRotateMat[4];
				markerPose[6] = markerRotateMat[7];
				markerPose[7] = m2c->getShift(1);

				markerPose[8] = markerRotateMat[2];
				markerPose[9] = markerRotateMat[5];
				markerPose[10] = markerRotateMat[8];
				markerPose[11] = m2c->getShift(2);

				markerPose[12] = 0;
				markerPose[13] = 0;
				markerPose[14] = 0;
				markerPose[15] = 1;	

				double R[3][3];
				for (int i=0;i<3;i++)
					for (int j=0;j<3;j++)
						R[i][j] = markerPose[i*4 + j];

				double Rad_X,Rad_Y,Rad_Z;
				if(1-R[2][0]*R[2][0] != 0)
				{
					Rad_Y = atan2(-R[2][0],sqrt(R[0][0]*R[0][0]+R[1][0]*R[1][0]));////////单位是弧度
					Rad_X = atan2(R[2][1],R[2][2]);
					Rad_Z = atan2(R[1][0],R[0][0]);
				}
				else
				{
					if(R[2][0] == 1)
					{
						Rad_Y = -PI/2;
						Rad_Z = 0;
						Rad_X = -atan2(R[0][1],R[1][1]);
					}
					else
					{
						Rad_Y = PI/2;
						Rad_Z = 0;
						Rad_X = atan2(R[0][1],R[1][1]);
					}
				}

				Rad_X = Rad_X / PI * 180;
				Rad_Y = Rad_Y / PI * 180;
				Rad_Z = Rad_Z / PI * 180;
				sprintf(buffer[3], "%.3f",Rad_X);
				sprintf(buffer[4], "%.3f",Rad_Y);
				sprintf(buffer[5], "%.3f",Rad_Z);


				//sprintf(s, "%d", markerNum);
				//strcat(s, ". ");
				sprintf(s, "%s",  marker->getName());
				//strcat(s, marker->getName());
				strcat(s, ":( ");
				strcat(s, buffer[0]);
				strcat(s, ",  ");
				strcat(s, buffer[1]);
				strcat(s, ",  ");
				strcat(s, buffer[2]);
				strcat(s, ",  ");
				strcat(s, buffer[3]);
				strcat(s, ",  ");
				strcat(s, buffer[4]);
				strcat(s, ",  ");
				strcat(s, buffer[5]);
				strcat(s, ")");
				strcat(s, ";");

// 					double pos[3][3];
// 					getFacetVector(1,pos);
// 					for (int i=1; i<4; i++)
// 					{	
// 						for (int j=0; j<3; j++)
// 						{
// 							sprintf(buffer[i*3 + j], "%.1f",pos[i][j]);
// 						}
// 						
// 					}
// 					strcat(s, "3points:(");
// 				
// 					strcat(s, buffer[3]);
// 					strcat(s, ",");
// 					strcat(s, buffer[4]);
// 					strcat(s, ",");
// 					strcat(s, buffer[5]);
// 					strcat(s, ",");
// 					strcat(s, buffer[6]);
// 					strcat(s, ",");
// 					strcat(s, buffer[7]);
// 					strcat(s, ",");
// 					strcat(s, buffer[8]);
// 					strcat(s, ",");
// 					strcat(s, buffer[9]);
// 					strcat(s, ",");
// 					strcat(s, buffer[10]);
// 					strcat(s, ",");
// 					strcat(s, buffer[11]);
// 					strcat(s, ")");

					// If there's a tooltip, add it
					//Xform3D* t2m; // tooltip to marker xform
					//Xform3D* t2c; // tooltip to camera xform
					////double toolTip2MarkerPos[3];
					//double toolTip2MarkerPos[3];

					//t2m = marker->tooltip2MarkerXf();
					//t2m->getShiftVector(toolTip2MarkerPos);
					//if (toolTip2MarkerPos[0] != 0 || toolTip2MarkerPos[1] != 0 || toolTip2MarkerPos[2] != 0) { // non-null transform
					//	strcat(s, " tip(");
					//	t2c = t2m->concatenate(m2c);
					//	for (int i=0; i<3; i++) {
					//		sprintf(buffer[i], "%.1f", t2c->getShift(i));
					//	}
					//	strcat(s, buffer[0]);
					//	strcat(s, ",");
					//	strcat(s, buffer[1]);
					//	strcat(s, ",");
					//	strcat(s, buffer[2]);
					//	strcat(s, ")");
					//	delete t2c;
					//}

					////显示额外的加入的点
					////
					//char currDir[255];
					//mtUtils::getCurrPath(currDir);
					//strcat(currDir, "\\Markers\\");
					//strcat(currDir, marker->getName());
					//vector<double> points;
					//getAllAdditionPoints(currDir,points);

					//for (int i=0; i<points.size(); i=i+3)
					//{	
					//	char num[2];
					//	double pointPos[3];
					//	sprintf(num, "%d", i/3+1);

					//	for (int j=0; j<3; j++)
					//	{
					//		pointPos[j] = points[i + j];
					//	}

					//	Xform3D* t2m = new Xform3D(); // tooltip to marker xform
					//	Xform3D* t2c; // tooltip to marker xform

					//	t2m->setShiftVector(pointPos);
					//	t2c = t2m->concatenate(m2c);
					//
					//	for (int k=0; k<3; k++) {
					//		sprintf(buffer[k], "%.1f", t2c->getShift(k));
					//	}

					//	/*strcat(s, " Point");
					//	strcat(s, num);
					//	strcat(s, " (");
					//	strcat(s, buffer[0]);
					//	strcat(s, ",");
					//	strcat(s, buffer[1]);
					//	strcat(s, ",");
					//	strcat(s, buffer[2]);
					//	strcat(s, ")");*/

					//	delete t2m;
					//	delete t2c;
					//}

					if(isDrawingText)
					{
						this->drawText(pDC, s, 10/(this->isShowingHalfSize ? 0.5 : 1), this->TextLine/(this->isShowingHalfSize ? 0.5 : 1));
						this->TextLine += 20;
					}
					

			/*		delete t2m;*/
					delete m2c;
					//free(t2c);

				}
				delete Marker2CurrCameraXf;
// 			}
// 			else // Not known
// 			{
// 				// cout << markerNum << ". another (unregistered) camera";
// 			}
			free(marker);

		}
	}


	for (markerNum = 1; markerNum <= markersCollection->count(); markerNum++)
	{
		m = new MTMarker::Marker(markersCollection->itemI(markerNum));
		if (m->wasIdentified(this->pCurrCam) )
		{

			MTCollection::Collection* facetsCollection = new MTCollection::Collection(m->identifiedFacets(this->pCurrCam));
			for (facetNum = 1; facetNum <= facetsCollection->count(); facetNum++)
			{
				Facet* f = new Facet(facetsCollection->itemI(facetNum));
				// get Xpoints and then draw on each image if enabled	
				double LS_LR_BH_XY[2][3][2][2];
				f->getXpoints(this->pCurrCam, (double *)LS_LR_BH_XY);
				int max;
				if (isSingleWindow)
					max = 0;
				else
					max = 1;
				/*if (menuArray[vectorIndex].value() || menuArray[xpointsIndex].value()){
					int nx = (LS_LR_BH_XY[0][0][0][0] + LS_LR_BH_XY[0][0][1][0])/2 ;
					int ny = (LS_LR_BH_XY[0][0][0][1] + LS_LR_BH_XY[0][0][1][1])/2 ;
					char caption[255];
					if (facetsCollection->count() > 1) {
						sprintf(caption,"%d.%s/%d",markerNum, m->getName(), facetNum);
					} else {
						sprintf(caption,"%d.%s",markerNum, m->getName());
					}
					drawText(caption, nx, ny);
				}

				for (int side=0; side<=max;side ++) {
					if ( menuArray[vectorIndex].value()) {
						// long vector
						drawLine(side, LS_LR_BH_XY[0][side][0][0], LS_LR_BH_XY[0][side][0][1],
							LS_LR_BH_XY[0][side][1][0], LS_LR_BH_XY[0][side][1][1],
							0.0, 0.5, 1.0, 0xFFFF);
						// short vector
						drawLine(side, LS_LR_BH_XY[1][side][0][0], LS_LR_BH_XY[1][side][0][1],
							LS_LR_BH_XY[1][side][1][0], LS_LR_BH_XY[1][side][1][1],
							1.0, 1.0, 0., 0x8888);
					}
					if ( menuArray[xpointsIndex].value()) {
						int radius = 4;
						// Draw a circle arounf each point
						drawCircle(side, LS_LR_BH_XY[0][side][0][0], LS_LR_BH_XY[0][side][0][1], radius, 0, 0, 1.0, 0xFFFF);
						drawCircle(side, LS_LR_BH_XY[0][side][1][0], LS_LR_BH_XY[0][side][1][1], radius, 0, 0, 1.0, 0xFFFF);
						drawCircle(side, LS_LR_BH_XY[1][side][0][0], LS_LR_BH_XY[1][side][0][1], radius, 0, 0, 1.0, 0xFFFF);
						drawCircle(side, LS_LR_BH_XY[1][side][1][0], LS_LR_BH_XY[1][side][1][1], radius, 0, 0, 1.0, 0xFFFF);
					}
				}*/
				delete f;
			}
			delete facetsCollection;
		}
		free(m);
	}
	// here we need the left side presentation: coordinates in sensor space, distances etc		


// 	if (this->isShowingAngleAndDistance && markersCollection->count() > 1)
// 	{
// 		Xform3D* Mi2Cam = NULL;
// 		Xform3D* Mj2Cam = NULL;
// 		for(int i=1; i<markersCollection->count(); i++)
// 		{
// 			MTMarker::Marker* m1 = new MTMarker::Marker(markersCollection->itemI(i));
// 			Mi2Cam = m1->marker2CameraXf(this->pCurrCam->Handle());
// 			for (int j=i+1; j<markersCollection->count()+1; j++)
// 			{
// 				MTMarker::Marker* m2 = new MTMarker::Marker(markersCollection->itemI(j));
// 				Mj2Cam = m2->marker2CameraXf(this->pCurrCam->Handle());
// 				if(Mi2Cam != NULL || Mj2Cam != NULL)
// 				{
// 					double XUnitV[3] = {0};
// 					double angleCos;
// 					double angleRads;
// 
// 					if (this->isShowingXpoints)
// 					{
// 						Facet* Fti;
// 						Facet* Ftj;
// 						Vector* LVij[2];
// 						static long NextDistI;
// 						static double distanceHistory[20];
// 						MTCollection::Collection* iIdFacets = new MTCollection::Collection(m1->identifiedFacets(this->pCurrCam));
// 						Fti = new Facet(iIdFacets->itemI(1));
// 						LVij[0] = (Fti->IdentifiedVectors())[0];
// 						MTCollection::Collection* jIdFacets = new MTCollection::Collection(m2->identifiedFacets(this->pCurrCam));
// 						Ftj = new Facet(jIdFacets->itemI(1));
// 						LVij[1] = (Fti->IdentifiedVectors())[0];
// 					} else	{
// 						double vec1[3];
// 						Mi2Cam->getShiftVector(vec1);
// 						//m1->marker2CameraXf(this->pCurrCam->Handle())->getShiftVector(vec1);
// 						double vec2[3];
// 						//m2->marker2CameraXf(this->pCurrCam->Handle())->getShiftVector(vec2);
// 						Mj2Cam->getShiftVector(vec2);
// 						double distance = this->findDistance( vec1, vec2 );
// 
// 						char s[300];
// 						char buffer[100];
// 						sprintf(s, "%d", i);
// 						strcat(s, "-");
// 						sprintf(buffer, "%d", i+1);
// 						strcat(s, buffer);
// 						strcat(s, ": ");
// 						sprintf(buffer, "%.1f", distance);
// 						strcat(s, buffer);
// 						strcat(s, " mm / ");
// 
// 						XUnitV[0] = 1;
// 						double XVect1[3];
// 						//m1->marker2CameraXf(this->pCurrCam->Handle())->getRotateVector(XVect1, XUnitV);
// 						Mi2Cam->getRotateVector(XVect1, XUnitV);
// 						double XVect2[3];
// 						//m2->marker2CameraXf(this->pCurrCam->Handle())->getRotateVector(XVect2, XUnitV);
// 						Mj2Cam->getRotateVector(XVect2, XUnitV);
// 						angleCos = this->dotProductVectors(XVect1, XVect2);
// 						angleRads = mtUtils::acos(angleCos);
// 
// 						sprintf(buffer, "%.1f", angleRads * 180 / PI);
// 						strcat(s, buffer);
// 
// 						//this->drawText(pDC, s, 10/(this->isShowingHalfSize ? 0.5 : 1), this->TextLine/(this->isShowingHalfSize ? 0.5 : 1) );
// 						this->TextLine += 20;
// 
// 					}
// 				}
// 				delete Mj2Cam;
// 				free(m2);
// 			}
// 			delete Mi2Cam;
// 			free(m1);
// 		}
// 	}
	delete markersCollection; 

}


//在图像中显示dXPoints的位置
void CMT_C::showIdentifiedXPoints(CDC *pDC)
{
	MTCollection::Collection* xpointsCollection = new MTCollection::Collection(this->pXPoints->detectedXPoints(this->pCurrCam)); 

	if (xpointsCollection->count() == 0) {
		delete xpointsCollection; 
		return;
	}

	
	double x3,y3,z3;
	double x[2],y[2];
	int XPNum=1;
	MTXPoint* XP;
	double radius = 5;
	// here we need the left side presentation: coordinates in sensor space, distances etc	
	Xform3D* Marker2CurrCameraXf = NULL;
// 	if(this->isShowingPositions)
// 	{
	for (XPNum = 1; XPNum <= xpointsCollection->count(); XPNum++)
	{
		XP = new MTXPoint(xpointsCollection->itemI(XPNum));
		XP->Position2D(&x[0], &y[0], &x[1], &y[1], &x[2], &y[2]);
		XP->Position3D(&x3, &y3, &z3);
		XP->setIndex(XPNum);

		//drawCircle(pDC, 0, x[0], y[0], radius, 1.0, 0, 0, 0xFFFF);
	}
//	}

// 	char s[100];
// 	sprintf(s, "%d", xpointsCollection->count());
// 	strcat(s, " XPs are detected by Current Camera");
// 	this->drawText(pDC, s, 10/(this->isShowingHalfSize ? 0.5 : 1), 40/(this->isShowingHalfSize ? 0.5 : 1));

	delete xpointsCollection; 
}

//填写bitmap的信息头
void  CMT_C::FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp )  
{  
	ASSERT( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));  

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);  

	memset( bmih, 0, sizeof(*bmih));  
	bmih->biSize = sizeof(BITMAPINFOHEADER);  
	bmih->biWidth = width;  
	bmih->biHeight = - height;  
	bmih->biPlanes = 1;  
	bmih->biBitCount = (unsigned short)bpp;  
	bmih->biCompression = BI_RGB;  

	if( bpp == 24 )  
	{  
		RGBQUAD* palette = bmi->bmiColors;  
		int i;  
		for( i = 0; i < 256; i++ )  
		{  
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;  
			palette[i].rgbReserved = 0;  
		}  
	}  
}  

//绘制图像，默认左视图
void CMT_C::DrawToPic( CDC *pDC, RECT* pDstRect, bool isDrawingText, int pic)
{
	unsigned char **laddr, **raddr, **maddr;
	this->pCurrCam->grabFrame();

	if (this->pCurrCam->getSensorsNum() == 3) 
		if (this->isShowingHalfSize)
			pCurrCam->getHalfSizeImages3( &laddr, &raddr, &maddr, pCurrCam->getXRes(), pCurrCam->getYRes() );
		else
			pCurrCam->getImages3( &laddr, &raddr, &maddr);
	else
		if (this->isShowingHalfSize)
			pCurrCam->getHalfSizeImages( &laddr, &raddr, pCurrCam->getXRes(), pCurrCam->getYRes() );
		else
			pCurrCam->getImages( &laddr, &raddr);

	int width = pCurrCam->getXRes() * (this->isShowingHalfSize ? 0.5 : 1);
	int height = pCurrCam->getYRes() * (this->isShowingHalfSize ? 0.5 : 1);

	char buffer[sizeof(BITMAPINFOHEADER) + 1024];  
	BITMAPINFO* bmi = (BITMAPINFO*)buffer;  
	int bmp_w = width, bmp_h = height;  
	int dst_w = pDstRect->right - pDstRect->left;
	int dst_h = pDstRect->bottom - pDstRect->top;

	int reslt = width * height;
	unsigned char *limage, *rimage;
	
	limage = (unsigned char *)malloc(sizeof(unsigned char ) * reslt*3);
	rimage = (unsigned char *)malloc(sizeof(unsigned char ) * reslt*3);

	if(laddr == NULL || raddr == NULL || limage == NULL || rimage == NULL)
		return;

	for (int i=0; i<height; i++)
	{
		for (int j=0; j<width; j++)
		{
			int index = i * width + j;
			if (this->pCurrCam->getSensorsNum() == 3) 
			{
				limage[index*3] =  ((unsigned char *)maddr)[index];
				limage[index*3 + 1] = ((unsigned char *)maddr)[index];
				limage[index*3 + 2] =  ((unsigned char *)maddr)[index];
			}
			else
			{
				limage[index*3] =  ((unsigned char *)laddr)[index];
				limage[index*3 + 1] = ((unsigned char *)laddr)[index];
				limage[index*3 + 2] =  ((unsigned char *)laddr)[index];
			}


			rimage[index*3] =  ((unsigned char *)raddr)[index];
			rimage[index*3 + 1] = ((unsigned char *)raddr)[index];
			rimage[index*3 + 2] =  ((unsigned char *)raddr)[index];
		}
	}

	Collection* markersCollection = new Collection(this->pMarkers->identifiedMarkers(this->pCurrCam));
	if (markersCollection->count() == 0) {
		delete markersCollection; 
	}
	else
	{
		MTMarker::Marker* marker;

		for (int markerNum = 1; markerNum <= markersCollection->count(); markerNum++)
		{
			marker = new MTMarker::Marker(markersCollection->itemI(markerNum));
			Collection* facetCollection = new Collection(marker->identifiedFacets(this->pCurrCam));

			for (int facetNum = 1; facetNum <= facetCollection->count(); facetNum++)
			{
				Facet* ft = new Facet(facetCollection->itemI(facetNum));
				ft->getXpoints(this->pCurrCam,(double *)coodOnLRImage);
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						int x = 0, y = 0;
						int radius = 0;
						if (this->isShowingHalfSize)
						{
							x = coodOnLRImage[i][2][j][0]/2;
							y = coodOnLRImage[i][2][j][1]/2;
							radius = 10;
						}
						else
						{
							x = coodOnLRImage[i][2][j][0];
							y = coodOnLRImage[i][2][j][1];
							radius = 20;
						}

						if (x > 0 && y > 0)
						{
							for (int iy = (y - radius > 0 ? y - radius : 0); iy < y + radius; iy++)
							{	
								for (int ix = (x - radius > 0 ? x - radius : 0); ix < x + radius; ix++)
								{
									if((iy - y) * (iy - y) + (ix - x) * (ix - x) <= (radius * radius))
									{
										int index = iy * width + ix;

										limage[index*3] =  0;
										limage[index*3 + 1] = 0;
										limage[index*3 + 2] =  255;
									}
								}
							}
						}
						
						

						if (this->isShowingHalfSize)
						{
							x = coodOnLRImage[i][1][j][0]/2;
							y = coodOnLRImage[i][1][j][1]/2;
						}
						else
						{
							x = coodOnLRImage[i][1][j][0];
							y = coodOnLRImage[i][1][j][1];
						}

						if (x > 0 && y > 0)
						{
							for (int iy = (y - radius > 0 ? y - radius : 0); iy < y + radius; iy++)
							{	
								for (int ix = (x - radius > 0 ? x - radius : 0); ix < x + radius; ix++)
								{
									if((iy - y) * (iy - y) + (ix - x) * (ix - x) <= (radius * radius))
									{
										int index = iy * width + ix;

										rimage[index*3] =  0;
										rimage[index*3 + 1] = 0;
										rimage[index*3 + 2] =  255;
									}
								}
							}
						}

						
					
					}
					
				}

				delete ft;
			}	
			delete facetCollection;
			free(marker);
		}
		delete markersCollection; 
	}
		
// 	for (int i=0; i<reslt; i++)
// 	{
// 		
// 		
// 		limage[i*3] =  ((unsigned char *)laddr)[i];
// 		limage[i*3 + 1] = ((unsigned char *)laddr)[i];
// 		limage[i*3 + 2] =  ((unsigned char *)laddr)[i];
// 
// 		rimage[i*3] =  ((unsigned char *)raddr)[i];
// 		rimage[i*3 + 1] = ((unsigned char *)raddr)[i];
// 		rimage[i*3 + 2] =  ((unsigned char *)raddr)[i];
// 		
// 	}

// 	for (int i=50; i<reslt/2; i++)
// 	{
// 		limage[i*3] = 0;
// 		limage[i*3 +1] = 0;
// 		limage[i*3 +2] = 255;
// 	}
	
	if(bmp_w > dst_w )   
	{  
		SetStretchBltMode(  
			pDC->GetSafeHdc(),           // handle to device context  
			HALFTONE );  
	}  
	else  
	{  
		SetStretchBltMode(  
			pDC->GetSafeHdc(),         // handle to device context  
			COLORONCOLOR );  
	}  

	FillBitmapInfo( bmi, bmp_w, bmp_h, 24 );  


	if (0 == pic)
	{
		::StretchDIBits(  
			pDC->GetSafeHdc(),   
			pDstRect->left, pDstRect->top, dst_w, dst_h,  
			0, 0, bmp_w, bmp_h,  
			limage, bmi, DIB_RGB_COLORS, SRCCOPY ); 
	}
	
	else if (1 == pic)
	{
		::StretchDIBits(  
			pDC->GetSafeHdc(),   
			pDstRect->left, pDstRect->top, dst_w, dst_h,  
			0, 0, bmp_w, bmp_h,  
			rimage, bmi, DIB_RGB_COLORS, SRCCOPY ); 
	}

	else if (2 == pic)
	{
		::StretchDIBits(  
			pDC->GetSafeHdc(),   
			pDstRect->left, pDstRect->top, dst_w, dst_h,  
			0, 0, bmp_w, bmp_h,  
			(unsigned char *)maddr, bmi, DIB_RGB_COLORS, SRCCOPY ); 
	}



	if (isMarkersProcessingEnabled)
		this->showIdentifiedMarkers(pDC,isDrawingText);
	//if (isXPointsProcessingEnabled)
		

	free(limage);
	free(rimage);
	limage = NULL;
	rimage = NULL;
}  

//显示左右视图FLTK
void CMT_C::DisplayLRImages()
{
	if ( ! isSingleWindow)
		drawImages(true, true, false);
	else 
		drawImages(true, false, false);
}

//根据窗口数量显示左右视图FLTK
void CMT_C::DisplayLRImages(int Side)
{
	if (Side == 0 ) {
		drawImages(true, false, false);
	} else if (Side == 1) {
		drawImages(false, true, false);
	} else if (Side == 2) {
		drawImages(false, false, true);
	} else {
		if ( ! isSingleWindow) {
			drawImages(true, true, false);
		} else {
			drawImages(true, false, false);
		}
	}
}

//绘制一副图像FLTK
void CMT_C::drawImage(int x, int y, int width, int height, unsigned char *pixels, bool firstRowAtBottom )
{
	//drawToDC
}


//绘制左中右视图FLTK
void CMT_C::drawImages(bool left, bool right, bool middle)
{


	unsigned char **laddr, **raddr, **maddr;
	if (this->pCurrCam->getSensorsNum() == 3) 
		if (this->isShowingHalfSize)
			pCurrCam->getHalfSizeImages3( &laddr, &raddr, &maddr, pCurrCam->getXRes(), pCurrCam->getYRes() );
		else
			pCurrCam->getImages3( &laddr, &raddr, &maddr);
	else
		if (this->isShowingHalfSize)
			pCurrCam->getHalfSizeImages( &laddr, &raddr, pCurrCam->getXRes(), pCurrCam->getYRes() );
		else
			pCurrCam->getImages( &laddr, &raddr);

	int width = pCurrCam->getXRes() * (this->isShowingHalfSize ? 0.5 : 1);
	int height = pCurrCam->getYRes() * (this->isShowingHalfSize ? 0.5 : 1);

	// switches left & right images subsequently	
	/*if (left_turn == true) 
	drawImage(window, 0, MENU_HEIGHT, width, height, (unsigned char *)laddr);
	else
	drawImage(window, 0, MENU_HEIGHT, width, height, (unsigned char *)raddr);
	*/


	if (left) {
		drawImage( 0, 0, width, height, (unsigned char *)laddr);
	}
	if (right) {
		drawImage( width, 0, width, height, (unsigned char *)raddr);
	}
	if (middle) {
		drawImage( 0, 0, width, height, (unsigned char *)maddr);
	}
}

//绘制直线
void CMT_C::drawLine(CDC *pDC,int side, float x1, float y1, float x2, float y2, float r, float g, float b, int stipple)
{
	
}

//绘制圆
void CMT_C::drawCircle(CDC *pDC,int side, float x, float y, float radius, float r, float g, float b, int stipple)
{
	
}

//绘制文字
void CMT_C::drawText(CDC *pDC, char* text, int x, int y)
{
	//View左上角写字
	CFont m_font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 150;
	lf.lfWidth = 8;
	m_font.CreatePointFontIndirect(&lf);
	CFont *pOldFont = pDC->SelectObject(&m_font);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(200,0,0));
	pDC->TextOut(x,y,text);
	pDC->SelectObject(pOldFont);

}

//保存左右视图
void CMT_C::saveLRImages(CString lImageName,CString rImageName)
{
	unsigned char **laddr, **raddr, **maddr;
	if (this->pCurrCam->getSensorsNum() == 3) 
		if (this->isShowingHalfSize)
			pCurrCam->getHalfSizeImages3( &laddr, &raddr, &maddr, pCurrCam->getXRes(), pCurrCam->getYRes() );
		else
			pCurrCam->getImages3( &laddr, &raddr, &maddr);
	else
		if (this->isShowingHalfSize)
			pCurrCam->getHalfSizeImages( &laddr, &raddr, pCurrCam->getXRes(), pCurrCam->getYRes() );
		else
			pCurrCam->getImages( &laddr, &raddr);
	

	int m_WidthOfModel = pCurrCam->getXRes();
	int m_HeightOfModel = pCurrCam->getYRes(); //图像的高与宽

	
	BITMAPINFOHEADER bmiHdr; //定义信息头   
	RGBQUAD rgbQuad[256];    //定义调色板         
	//对信息头进行赋值    
	bmiHdr.biSize = sizeof(BITMAPINFOHEADER);   
	bmiHdr.biWidth = m_WidthOfModel;   
	bmiHdr.biHeight = -m_HeightOfModel;   
	bmiHdr.biPlanes = 1;   
	bmiHdr.biBitCount = 8;    
	bmiHdr.biCompression = BI_RGB;    
	bmiHdr.biSizeImage = m_WidthOfModel*m_HeightOfModel;   
	bmiHdr.biXPelsPerMeter = 0;   
	bmiHdr.biYPelsPerMeter = 0;   
	bmiHdr.biClrUsed = 0;    
	bmiHdr.biClrImportant = 0;         
	//对调色板进行赋值   
	for(int i=0; i<256; i++)   
	{     
		rgbQuad[i].rgbBlue = (BYTE)i;    
		rgbQuad[i].rgbGreen = (BYTE)i;    
		rgbQuad[i].rgbRed = (BYTE)i;    
		rgbQuad[i].rgbReserved = 0;   
	}
	BITMAPFILEHEADER bmfHdr; //定义文件头
	//对文件头进行赋值
	bmfHdr.bfType = (WORD)0x4D42;//;((WORD)('M'<<8) | 'B')
	bmfHdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256 + m_WidthOfModel*m_HeightOfModel);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256);

	//保存文件   
	CFile fpLeft;
	fpLeft.Open(lImageName,CFile::modeCreate|CFile::modeWrite);
	fpLeft.Write((LPSTR)&bmfHdr,sizeof(BITMAPFILEHEADER));//写头文件   
	fpLeft.Write((LPSTR)&bmiHdr,sizeof(BITMAPINFOHEADER));//写信息头
	fpLeft.Write((LPSTR)rgbQuad,sizeof(RGBQUAD)*256); //写调色板
	fpLeft.Write(laddr,m_WidthOfModel*m_HeightOfModel);//写数据  
	fpLeft.Close();

	CFile fpRight;
	fpRight.Open(rImageName,CFile::modeCreate|CFile::modeWrite);
	fpRight.Write((LPSTR)&bmfHdr,sizeof(BITMAPFILEHEADER));//写头文件   
	fpRight.Write((LPSTR)&bmiHdr,sizeof(BITMAPINFOHEADER));//写信息头
	fpRight.Write((LPSTR)rgbQuad,sizeof(RGBQUAD)*256); //写调色板
	fpRight.Write(raddr,m_WidthOfModel*m_HeightOfModel);//写数据  
	fpRight.Close();

	Collection *markersCollection = new Collection(this->pMarkers->identifiedMarkers(this->pCurrCam));

	if (markersCollection->count() <= 0)
	{
		delete markersCollection;
		return ;
	}

	CFile XformFile;
	XformFile.Open("Tool在左右视图下的Marker2Camera的姿态.txt",CFile::modeCreate|CFile::modeWrite);

	for (int i = 1; i <= markersCollection->count(); i++)
	{
		double rotateMat[9];
		double shift[3];
		CString curMarkName;
		Marker *m = new Marker(markersCollection->itemI(i));
		Xform3D *curXform = m->marker2CameraXf(this->pCurrCam->getHandle());
		curMarkName = m->getName();
		curXform->getRotateMat(rotateMat);
		curXform->getShiftVector(shift);

		XformFile.Write(curMarkName,curMarkName.GetLength());
		XformFile.Write("\r\n",2);
		//XformFile.Write("\n",1);
		for (int j=0; j<3; j++)
		{
			for (int k=0; k<3; k++)
			{
				CString curTemp;
				curTemp.Format("%.10f",rotateMat[j*3 + k]);
				XformFile.Write(curTemp,curTemp.GetLength());
				XformFile.Write("\t",1);

			}
			XformFile.Write("\r\n",2);
		}

		for (int p=0; p<3; p++)
		{	
			CString tempS;
			tempS.Format("%.6f",shift[p]);
			XformFile.Write(tempS,tempS.GetLength());
			XformFile.Write("\t",1);
		}
		
		delete curXform;
	}
	delete markersCollection;
	
	XformFile.Close();

}
//擦除窗口FLTK
void CMT_C::ClearWindow( )
{
	int width = pCurrCam->getXRes() * (this->isShowingHalfSize ? 0.5 : 1);
	int height = pCurrCam->getYRes() * (this->isShowingHalfSize ? 0.5 : 1);
	//window->make_current();
	float mult = 2.0;
	if ( isSingleWindow ) {
		mult = 1.0;
	}
	// 	fl_color(100, 100, 100);
	// 	fl_rectf( 0,  MENU_HEIGHT, mult*width, height);

}


//判断一个marker是否属于所有的marker集合中
int CMT_C::findMarkerInSet(const char* name)
{
	for(int i = 1; i <= this->pMarkers->getTemplateCount(); i++)
	{
		std::string currName;
		this->pMarkers->getTemplateItemName(i, currName);
		if (currName.compare(name)==0)
		{
			return i;
		}
		// TODO:verify this
	}
	return -1;
}

//判断一个marker是否属于当前采集到的marker集合中
int CMT_C::findMarkerInCollection(const char* name, Collection *pMarkersCollection)
{

	if (NULL == pMarkersCollection)
	{
		pMarkersCollection = new Collection(this->pMarkers->identifiedMarkers(this->pCurrCam));

		if (pMarkersCollection->count() < 1)
		{
			delete pMarkersCollection;
			pMarkersCollection = NULL;
			return  -1;
		}
	}
	for(int i = 1; i <= pMarkersCollection->count(); i++)
	{
		std::string currName;
		Marker *cMaker = new Marker(pMarkersCollection->itemI(i));
		currName = cMaker->getName();
		if (currName.compare(name)==0)
		{
			return i;
		}
		// TODO:verify this
	}
	return -1;
}

//设置新marker名称
void CMT_C::setNewName(CString inputNewMarkName)
{
	this->m_InputMakerName = inputNewMarkName;

}

//采集未知marker的向量集
void CMT_C::collectNewSamples()
{
	static char buffer[255];
	this->pMarkers->processFrame(NULL);
	MTCollection::Collection* col = new MTCollection::Collection( this->pMarkers->unidentifiedVectors(this->pCurrCam) );
	if (col->count() == 2 )
	{
		//this->collectingStatus->label("Collecting samples");
		this->sampleVectors.push_back(col);
		this->m_collectedSampleFrames++;
		// Additional facets
		if (this->isAdditionalFacetAdding)
		{
			MTCollection::Collection* identifiedFacetsCol = new MTCollection::Collection(this->pCurrMarker->identifiedFacets(NULL));
			if (identifiedFacetsCol->count() > 0)
			{
				// Also compute and save the xform of facet1 to sensor
				if (this->pCurrMarker->marker2CameraXf(this->pCurrCam->Handle()) != (Xform3D*)NULL)
					this->facet1ToCameraXfs.push_back(this->pCurrMarker->marker2CameraXf(this->pCurrCam->Handle()));
				else // Seen by a camera not registered with this one
				{
					//this->collectingStatus->label("Cannot sample: unregistered camera");
					this->sampleVectors.pop_back();
					this->m_collectedSampleFrames--;
				}
			}
			else // Not seeing a known facet
			{
				//this->collectingStatus->label("Cannot sample: no known facet!");
				this->sampleVectors.pop_back();
				this->m_collectedSampleFrames--;
			}
			delete identifiedFacetsCol;
		}
		sprintf(buffer, "Collected: %d\n", this->m_collectedSampleFrames);
		//this->collectingStatus->label(buffer);
	}
	else if(col->count() < 2) {
		//this->collectingStatus->label("No new facet detected.");
	} else if(col->count() > 2) {
		sprintf(buffer, "More than 2 vectors detected [%d].\n", col->count());
		//this->collectingStatus->label(buffer);
	}
	//delete col;
}


//停止采集未知marker向量，根据已采集向量定义marker
bool CMT_C::stopSampling(char* name)
{  
	if (!this->isCollectingSamples)
		return false;
	this->isCollectingSamples = false;
	string errorsString;
	Facet* f = new Facet();
	vector<Vector*> vectorPair;

	if (f->setVectorsFromSample(this->sampleVectors, errorsString))
	{
		if (this->isAdditionalFacetAdding)
		{
			vector<Xform3D*> facet1ToNewFacetXfs;
			int i;
			if ( this->sampleVectors.size() <1) return false;
			Xform3D* facet1ToNewFacetXf;
			for (i=0; i<this->sampleVectors.size(); i++)
			{
				vectorPair.clear();
				vectorPair.push_back(new Vector( this->sampleVectors[i]->itemI(0) ));
				vectorPair.push_back(new Vector( this->sampleVectors[i]->itemI(1) ));
				if (f->identify(this->pCurrCam, vectorPair, 1)) // Then the sample matches the template
				{
					// Compute the xform between the first marker facet and the new one
					facet1ToNewFacetXf = this->facet1ToCameraXfs[i]->concatenate(f->getFacet2CameraXf(this->pCurrCam)->inverse());
					facet1ToNewFacetXfs.push_back(facet1ToNewFacetXf);
				}
			}
			// Combine the transforms accumulated to a new one and save it with the facet in the marker
			facet1ToNewFacetXf = facet1ToNewFacetXfs[0];
			for (i=1; i<facet1ToNewFacetXfs.size(); i++)
				facet1ToNewFacetXf->inBetween(facet1ToNewFacetXfs[i], 1);// will result in equal contribution by all faces
			this->pCurrMarker->addTemplateFacet(f, facet1ToNewFacetXf);
		}
		else
		{
			this->pCurrMarker = new MTMarker::Marker();
			Xform3D* Xf = new Xform3D();
			this->pCurrMarker->setName(name);
			int addingResult = this->pCurrMarker->addTemplateFacet(f, Xf);
			if(addingResult != mtOK)
			{
				AfxMessageBox("创建新marker发生错误!");
				return false;
			}
		}
		AfxMessageBox("采样完成!");
		return true;
	}
	else
	{
		AfxMessageBox("无有效的采集数据!");
		return false;
	}

}


//保存未知marker的文件
void CMT_C::saveSample()
{
	//	if (!data->pCurrMarker->validateTemplate(data->pMarkers->getTemplateMatchToleranceMM(), complString))
	//		data->collectingStatus->label("Save failed!");
	this->saveMarkerTemplate( this->pCurrMarker->getName() );
	this->m_collectedSampleFrames = 0;
}

//采集探针针尖的姿态
bool CMT_C::collectMarksPose(CString TTBlockName,CString ToolTipName)
{
	this->pMarkers->processFrame(this->pCurrCam);

	Collection *markersCollection = new Collection(this->pMarkers->identifiedMarkers(this->pCurrCam));

	if (markersCollection->count() < 2)
	{
		delete markersCollection;
		return  false;
	}

	int indexTTBlock = findMarkerInCollection(TTBlockName.GetBuffer(TTBlockName.GetLength()),markersCollection);
	int indexToolTip = findMarkerInCollection(ToolTipName.GetBuffer(TTBlockName.GetLength()),markersCollection);
	if (indexTTBlock < 0 || indexToolTip < 0) 
		return false;
	Marker *mTTBlock = new Marker(markersCollection->itemI(indexTTBlock));
	Marker *mToolTip = new Marker(markersCollection->itemI(indexToolTip));

	Xform3D *ToolTipToMarkerXf = mTTBlock->marker2CameraXf(this->pCurrCam->getHandle())->concatenate(mToolTip->marker2CameraXf(this->pCurrCam->getHandle())->inverse());

	this->ToolTipToMarkerXfs.push_back(ToolTipToMarkerXf);
	this->m_collectedSampleFrames = this->ToolTipToMarkerXfs.size();

	return true;
}


//停止采集针尖姿态，并用四元组球形插值算法求得针尖最终的姿态
bool CMT_C::stopToolTipSampling(CString ToolTipName)
{
	if (!this->isCollectingSamples)
		return false;
	if (ToolTipToMarkerXfs.empty() == true)
	{
		AfxMessageBox("无采集数据请重试！");
		return false;
	}

	this->isCollectingSamples = false;
	// Combine the transforms accumulated to a new one and save it with the facet in the marker
	Xform3D *ToolTipToMarkerXf = this->ToolTipToMarkerXfs[0];
	for (int i=1; i<ToolTipToMarkerXfs.size(); i++)
		ToolTipToMarkerXf->inBetween(ToolTipToMarkerXfs[i], 1);// will result in equal contribution by all faces
	
	ToolTipToMarkerXf->getShiftVector(realNum);

	//Collection* markersCollection = new Collection(this->pMarkers->identifiedMarkers(this->pCurrCam));
	int indexToolTip = this->findMarkerInSet(ToolTipName.GetBuffer(ToolTipName.GetLength()));
	this->pCurrMarker = new Marker(this->pMarkers->getTemplateItem(indexToolTip));
	this->pCurrMarker->tooltip2MarkerXfSet(ToolTipToMarkerXf);


	AfxMessageBox("采集完成！");
	return true;

}


//根据tool名字得到tip在当前帧相机坐标系以及自身坐标系下的坐标值
void CMT_C::GetToolTipPos(CString toolTipName, double *pos2M, double *pos2C)
{
	int indexToolTip = -1;
	MTCollection::Collection* markersCollection = new MTCollection::Collection(this->pMarkers->identifiedMarkers(NULL)); //this->pCurrCam));

	if (markersCollection->count() == 0) {
		delete markersCollection; 
		return;
	}

	if (toolTipName.IsEmpty())
	{
		for (int i = 1; i <= markersCollection->count(); i++)
		{
			Xform3D *tool2Marker;
			Xform3D *tool2Camera;
			Xform3D *marker2Camera;
			Marker *m = new Marker(markersCollection->itemI(i));
			tool2Marker =m->tooltip2MarkerXf();
			tool2Marker->getShiftVector(pos2M);
			if (pos2M[0] != 0 || pos2M[1] != 0 || pos2M[2] != 0)
			{
				marker2Camera = m->marker2CameraXf(this->pCurrCam->getHandle());
				tool2Camera = tool2Marker->concatenate(marker2Camera);
				tool2Camera->getShiftVector(pos2C);

				delete tool2Camera;
				delete marker2Camera;
			}

			delete tool2Marker;
			free(m);
		}
	}

	if (!toolTipName.IsEmpty() && (indexToolTip = this->findMarkerInCollection(toolTipName.GetBuffer(toolTipName.GetLength()),markersCollection)) > 0)
	{
		this->pCurrMarker = new Marker(markersCollection->itemI(indexToolTip));
		Xform3D *Marker2Camera;
		Xform3D *ToolTip2Marker;
		Xform3D *ToolTip2Camera;

		ToolTip2Marker = this->pCurrMarker->tooltip2MarkerXf();
		Marker2Camera = this->pCurrMarker->marker2CameraXf(this->pCurrCam->getHandle());
		ToolTip2Camera = ToolTip2Marker->concatenate(Marker2Camera);

		ToolTip2Marker->getShiftVector(pos2M);
		ToolTip2Camera->getShiftVector(pos2C);

		delete Marker2Camera;
		delete ToolTip2Marker;
		delete ToolTip2Camera;

	}

	delete markersCollection;

}


//求取针尖姿态的测量误差，平均，标准，最大误差
void CMT_C::getToolTipMeasureErrors(double *errors)
{
	if (isCollectingSamples || ToolTipToMarkerXfs.empty())
	{
		return;
	}

	int samplingNums = ToolTipToMarkerXfs.size();
	double (*data)[3];
	double sum = 0;
	data = new double[samplingNums][3];

	for (int i=0; i<samplingNums; i++)
	{
		Xform3D *ToolTipToMarkerXf = this->ToolTipToMarkerXfs[i];
		ToolTipToMarkerXf->getShiftVector(data[i]);
	}

	//四元组球面插值算法求平均位置坐标
// 	Xform3D *ToolTipToMarkerXf = this->ToolTipToMarkerXfs[0];
// 	for (int i=1; i<ToolTipToMarkerXfs.size(); i++)
// 		ToolTipToMarkerXf->inBetween(ToolTipToMarkerXfs[i], 1);

	//简单方法求平均坐标
	/*for (int i = 0; i < samplingNums; i++)
	{
		realNum[0] += data[i][0];
		realNum[1] += data[i][1];
		realNum[2] += data[i][2];
	}

	realNum[0] = realNum[0]/samplingNums;
	realNum[1] = realNum[1]/samplingNums;
	realNum[2] = realNum[2]/samplingNums;*/

	sum = 0.0;
	double* dev = new double[samplingNums];
	for(int i = 0 ; i < samplingNums; i ++)
	{
		dev[i] = sqrt(
			(data[i][0]-realNum[0])*(data[i][0]-realNum[0]) +
			(data[i][1]-realNum[1])*(data[i][1]-realNum[1]) +
			(data[i][2]-realNum[2])*(data[i][2]-realNum[2])
			);
		sum += dev[i];
		//最大误差
		if (dev[i] > errors[2])
		{
			errors[2] = dev[i];
		}
	}

	//平均误差
	errors[0] = sum / samplingNums;


	//标准误差  
	sum = 0;
	for(int i = 0 ; i < samplingNums; i++)
	{
		sum += (dev[i]-errors[0])*(dev[i]-errors[0]);
	}

	errors[1] = sqrt(sum/(samplingNums-1));

	//保存文件
	FILE* errorsFile;
	errorsFile = fopen("data.txt","w");
	if (errorsFile==NULL)
	{
		return;
	}
	for (int i=0; i < samplingNums; i++)
	{
		fprintf(errorsFile,"%d :", i);
		for (int j=0;j<=2;j++)
		{
			fprintf(errorsFile,"data:%f\t", data[i][j]);
		}
		fprintf(errorsFile,"\n");
	}
	fprintf(errorsFile,"\n\n");
	fprintf(errorsFile,"平均坐标: X: %f\tY :%f\tZ :%f\t\n\n", realNum[0], realNum[1], realNum[2]);
	fprintf(errorsFile,"平均误差:%f\t标准误差:%f\t最大误差:%f\t\n", errors[0], errors[1], errors[2]);
	fclose(errorsFile);


	delete[] data;
	delete[] dev;

}

void CMT_C::getToolAvePos(double *pos, int nums)
{
	if (pos == NULL || nums < 3)
	{
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		pos[i] = realNum[i];
	}
}

//计算两个向量的距离
double CMT_C::findDistance(double* v1, double* v2)
{
	double acc = 0.0;
	for (int i=0; i< 3; i++)
		acc = acc + ( (v1[i] - v2[i]) * (v1[i] - v2[i]) );
	return sqrt(acc);
}

void CMT_C::getCurrMarkerANYPos(double *pos,double *posC)
{
	MTCollection::Collection* markersCollection = new MTCollection::Collection(this->pMarkers->identifiedMarkers(NULL)); //this->pCurrCam));

	if (markersCollection->count() != 1) {
		delete markersCollection; 
		return;
	}

	Xform3D *Point2Marker = new Xform3D();
	Point2Marker->setShiftVector(pos);
	
	Xform3D *marker2Camera;
	Xform3D *Point2Camera;

	Marker *m = new Marker(markersCollection->itemI(1));
	marker2Camera = m->marker2CameraXf(this->pCurrCam->getHandle());

	Point2Camera = Point2Marker->concatenate(marker2Camera);
	Point2Camera->getShiftVector(posC);

	delete Point2Marker;
	delete marker2Camera;
	delete Point2Camera;
	delete markersCollection;

}


bool CMT_C::getFacetVector(int markernum, double p_vecPos[][3])
{

	bool bOK =false;

	Collection* markersCollection = new Collection(this->pMarkers->identifiedMarkers(this->pCurrCam));
	if (markersCollection->count() == 0) {
		delete markersCollection; 
		return  bOK;
	}

	if ( markernum <=0 || markernum>getMarkerTemplatesCount() ) 
		return bOK;

	// here we need the left side presentation: coordinates in sensor space, distances etc	
	Xform3D* Marker2CurrCameraXf = NULL;

	Marker* marker = new Marker(markersCollection->itemI(markernum));
	Marker2CurrCameraXf = marker->marker2CameraXf(this->pCurrCam->Handle());

	if(Marker2CurrCameraXf != NULL)
	{
		//错误，identitiedFacets函数返回是collection类型，
		Collection* facetCollection = new Collection(marker->identifiedFacets(this->pCurrCam));
		Facet* ft = new Facet(facetCollection->itemI(1));

		Vector* LV = (ft->IdentifiedVectors())[0];
		Vector* SV = (ft->IdentifiedVectors())[1];
	
		double endPos[2][3];
		LV->getEndPos2x3( (double*)endPos);
		for (int i=0; i<2; i++)
			for (int j=0; j<3; j++)
				p_vecPos[i][j]= endPos[i][j];
			

		SV->getEndPos2x3( (double*)endPos);
		for (int i=0; i<2; i++)
			for (int j=0; j<3; j++)
				p_vecPos[i+2][j]= endPos[i][j];

		bOK =true;
		delete ft;

	}


	delete Marker2CurrCameraXf;
	delete markersCollection;

	return bOK;

}

bool CMT_C::getMarkerVectorAndPose(CString markName, double p_vecPos[][3], double center[3], double RotateMat[9])
{
	if (markName.IsEmpty() || p_vecPos == NULL)
		return false;

	this->pCurrCam->grabFrame();
	this->pMarkers->processFrame(this->pCurrCam);
	Collection* markersCollection = new Collection(this->pMarkers->identifiedMarkers(this->pCurrCam));		

	if (markersCollection->count() <= 0) {
		delete markersCollection; 
		return false;
	}
	// here we need the left side presentation: coordinates in sensor space, distances etc	

	MTMarker::Marker* marker = NULL;
	// here we need the left side presentation: coordinates in sensor space, distances etc	
	Xform3D* Marker2CurrCameraXf = NULL;	

	bool isFind = false;

	for (int markerNum = 1; markerNum <= markersCollection->count(); markerNum++)
	{
		marker = new MTMarker::Marker(markersCollection->itemI(markerNum));
		Marker2CurrCameraXf = marker->marker2CameraXf(this->pCurrCam->Handle());

		if (markName == marker->getName() && Marker2CurrCameraXf != NULL)
		{
			Collection* facetCollection = new Collection(marker->identifiedFacets(this->pCurrCam));
			Facet* ft = new Facet(facetCollection->itemI(1));

			Vector* LV = (ft->IdentifiedVectors())[0];
			Vector* SV = (ft->IdentifiedVectors())[1];

			double endPos[2][3];
			LV->getEndPos2x3( (double*)endPos);
			for (int i=0; i<2; i++)
				for (int j=0; j<3; j++)
					p_vecPos[i][j] = endPos[i][j];	

			SV->getEndPos2x3( (double*)endPos);
			for (int i=0; i<2; i++)
				for (int j=0; j<3; j++)
					p_vecPos[i+2][j] = endPos[i][j];


			for (int i=0; i<3; i++)
				center[i] = Marker2CurrCameraXf->getShift(i);

			Marker2CurrCameraXf->getRotateMat(RotateMat);

			delete ft;
			delete facetCollection;
			delete Marker2CurrCameraXf;
			free(marker);

			isFind = true;
			break;
		}
		else
		{
			delete Marker2CurrCameraXf;
			free(marker);
		}	
	}
	delete markersCollection;

	return isFind;
}

bool CMT_C::getMarkerPosByName(CString markName, double *pos, double *RotateVector)
{
	MTCollection::Collection* markersCollection = new MTCollection::Collection(this->pMarkers->identifiedMarkers(NULL)); //this->pCurrCam));

	if (markersCollection->count() == 0) { 
		delete markersCollection; 
		return false;
	}

	int markerNum = 1;
	
	MTMarker::Marker* marker;
	// here we need the left side presentation: coordinates in sensor space, distances etc	
	Xform3D* Marker2CurrCameraXf = NULL;

	bool isFindMarker = false;

	for (markerNum = 1; markerNum <= markersCollection->count(); markerNum++)
	{
		marker = new MTMarker::Marker(markersCollection->itemI(markerNum));
		Marker2CurrCameraXf = marker->marker2CameraXf(this->pCurrCam->Handle());

		if (markName == marker->getName() && Marker2CurrCameraXf != NULL)
		{
			for (int i=0; i<3; i++)
				pos[i] = Marker2CurrCameraXf->getShift(i);
			
			Marker2CurrCameraXf->getRotateMat(RotateVector);
			isFindMarker = true;
			break;

		}
		free(marker);
	}
	delete markersCollection;

	return isFindMarker;


}

bool CMT_C::getAveMarkerPosByName(CString markName, double *pos, double *RotateVector, int samplingTimes)
{
	if (this->TestMarkerToCamXfs.capacity() > 0)
	{
		for (vector<Xform3D *>::iterator it = TestMarkerToCamXfs.begin(); it != TestMarkerToCamXfs.end(); it ++) 
			if (NULL != *it) 
			{
				delete *it; 
				*it = NULL;
			}
			TestMarkerToCamXfs.clear();
	}

	for (int i=0, j=0; i<samplingTimes;)
	{
		this->pCurrCam->grabFrame();

		this->pMarkers->processFrame(this->pCurrCam);

		Collection *markersCollection = new Collection(this->pMarkers->identifiedMarkers(this->pCurrCam));

		if (j >= samplingTimes)
			return  false;

		if (markersCollection->count() <= 0)
		{
			j++;
			delete markersCollection;
			continue;
		}
		else
		{
			MTMarker::Marker* marker;
			// here we need the left side presentation: coordinates in sensor space, distances etc	
			Xform3D* Marker2CurrCameraXf = NULL;

			for (int markerNum = 1; markerNum <= markersCollection->count(); markerNum++)
			{
				marker = new MTMarker::Marker(markersCollection->itemI(markerNum));
				Marker2CurrCameraXf = marker->marker2CameraXf(this->pCurrCam->Handle());

				if (markName == marker->getName() && Marker2CurrCameraXf != NULL)
				{
					TestMarkerToCamXfs.push_back(Marker2CurrCameraXf);
					i++;
					j=0;
				}
				free(marker);
			}
			
		}
		delete markersCollection;
	}

	Xform3D *AveMarker2CurrCameraXf = this->TestMarkerToCamXfs[0];
	for (int i=1; i<TestMarkerToCamXfs.size(); i++)
		AveMarker2CurrCameraXf->inBetween(TestMarkerToCamXfs[i], 1);// will result in equal contribution by all faces

	AveMarker2CurrCameraXf->getRotateMat(RotateVector);
	AveMarker2CurrCameraXf->getShiftVector(pos);

	if (this->TestMarkerToCamXfs.capacity() > 0)
	{
		for (vector<Xform3D *>::iterator it = TestMarkerToCamXfs.begin(); it != TestMarkerToCamXfs.end(); it ++) 
			if (NULL != *it) 
			{
				delete *it; 
				*it = NULL;
			}
			TestMarkerToCamXfs.clear();
	}

	return true;

}

//计算两个向量点乘值
double CMT_C::dotProductVectors(double* v1, double* v2)
{
	double result = 0;
	for (int i=0; i<3; i++)
		result += v1[i]*v2[i];
	return result;
}

//保存任意一点到Marker文件
void CMT_C::savePointsInMarkerFile(CString pos[])
{
	MTCollection::Collection* markersCollection = new MTCollection::Collection(this->pMarkers->identifiedMarkers(NULL)); //this->pCurrCam));

	if (markersCollection->count() != 1) {
		delete markersCollection; 
		AfxMessageBox("检测到的Marker数量不是一个，无法保存！");
		return;
	}

	Marker *m = new Marker(markersCollection->itemI(1));

	char currDir[255];
	mtUtils::getCurrPath(currDir);
	strcat(currDir, "\\Markers\\");
	strcat(currDir, m->getName());
	
	vector<double> points;
	getAllAdditionPoints(currDir,points);
	
	//检测输入点是否与现有的点重合
	double p[3];
	for (int j=0; j<3; j++)
	{
		p[j] = atof(pos[j]);
	}

	for (int i=0; i<points.size(); i=i+3)
	{
		double dis = 0.0;
		
		for (int k=0; k<3; k++)
		{
			dis += (points[i + k] - p[k]) * (points[i + k] - p[k]);
		}

		if (dis < 1.0)
		{
			AfxMessageBox("输入点与以保存点几乎重合，未保存！");
			return;
		}
	}
	
	FILE *crtMarkerFile = fopen(currDir,"a+");
	fprintf(crtMarkerFile,"\n");
	fprintf(crtMarkerFile,"[POINT]");
	fprintf(crtMarkerFile,"\n");

	CString temp;
	temp = "S0=" + pos[0];
	fprintf(crtMarkerFile,temp);
	fprintf(crtMarkerFile,"\n");

	temp = "S1=" + pos[1];
	fprintf(crtMarkerFile,temp);
	fprintf(crtMarkerFile,"\n");

	temp = "S2=" + pos[2];
	fprintf(crtMarkerFile,temp);
	fprintf(crtMarkerFile,"\n");
	
	fclose(crtMarkerFile);
	delete markersCollection;
}

//保存pts到Marker文件
void CMT_C::savePointsInMarkerFile(char *filePath,const vector<double> pts)
{
	if (pts.empty())
		return;

	FILE *crtMarkerFile = fopen(filePath,"a+");
	if (crtMarkerFile == NULL)
	{
		AfxMessageBox("无法打开文件！请查询路径是否正确！");
	}

	CString pos[3];

	for (int i=0; i<pts.size(); i=i+3)
	{
		for (int j=0; j<3; j++)
		{
			pos[j].Format("%.8f",pts[i + j]);
		}

		fprintf(crtMarkerFile,"\n");
		fprintf(crtMarkerFile,"[POINT]");
		fprintf(crtMarkerFile,"\n");

		CString tempStr;
		tempStr = "S0=" + pos[0];
		fprintf(crtMarkerFile,tempStr);
		fprintf(crtMarkerFile,"\n");

		tempStr = "S1=" + pos[1];
		fprintf(crtMarkerFile,tempStr);
		fprintf(crtMarkerFile,"\n");

		tempStr = "S2=" + pos[2];
		fprintf(crtMarkerFile,tempStr);
		fprintf(crtMarkerFile,"\n");
	}
	
	fclose(crtMarkerFile);

}

//读取路径文件中在Marker坐标系所有点
void CMT_C::getAllAdditionPoints(char *filePath, vector<double> &pointsInMarker)
{
	//打开文件
	FILE* crtMarkerFile;
	crtMarkerFile = fopen(filePath,"r");
	if (crtMarkerFile == NULL)
	{
		AfxMessageBox("文件打开失败，请确认路径是否有误！");
		return;
	}

	//读取一行的最大字符数
	char buf[MAX_PATH];
	double position[3] = {0};

	//原点
	for (int i=0; i<3; i++)
		pointsInMarker.push_back(position[i]);

	//约定的额外点的关键字
	CString pointTable = "[POINT]";
	CString S0 = "S0=";
	CString S1 = "S1=";
	CString S2 = "S2=";

	//开关变量
	bool IsStartRecord = false;
	bool IsPointReady = false;

	//打开Marker文件，查找并存储Marker坐标系下的额外点
	while (fgets(buf,MAX_PATH,crtMarkerFile) != NULL)
	{
		CString resultStr = buf;
		if (resultStr.Find(pointTable) >= 0)
		{
			IsStartRecord = true;
			IsPointReady = false;
		}

		else if (resultStr.Find(S0) >= 0 && IsStartRecord)
		{
			CString pos0 = resultStr.Mid(S0.GetLength());
			position[0] = atof(pos0);
		}

		else if (resultStr.Find(S1) >= 0 && IsStartRecord)
		{
			CString pos1 = resultStr.Mid(S1.GetLength());
			position[1] = atof(pos1);
		}

		else if (resultStr.Find(S2) >= 0 && IsStartRecord)
		{
			CString pos2 = resultStr.Mid(S2.GetLength());
			position[2] = atof(pos2);

			IsPointReady = true;
			IsStartRecord = false;
		}

		if (IsPointReady)
		{
			for (int i=0; i<3; i++)
				pointsInMarker.push_back(position[i]);

			IsPointReady = false;
		}	
	}
	fclose(crtMarkerFile);

}

//计算保存MarkerName下所有的在Marker坐标系及相机坐标系下的点
void CMT_C::getAllAdditionPointsPos(const char *filePath, const char *markerName, vector<double> &pointsInMarker, vector<double> &pointsInCamera)
{
	Collection *markersCollection = new Collection(this->pMarkers->identifiedMarkers(this->pCurrCam));
	int i = markersCollection->count();
	if (markersCollection->count() < 1)
	{
		delete markersCollection;
		AfxMessageBox("未发现任何Marker！");
		return;
	}

	//查找检测到的Marker是否包含markerName
	Marker *dstMarker = NULL;
	int index = -1;
	for(int i = 1; i <= markersCollection->count(); i++)
	{
		std::string currName;
		Marker *cMaker = new Marker(markersCollection->itemI(i));
		currName = cMaker->getName();
		if (currName.compare(markerName)==0)
		{
			index = i;
		}

	}
	if (index < 0)
	{
		AfxMessageBox("未找到该Marker！");
		return;
	}
	else
		dstMarker = new Marker(markersCollection->itemI(index));

	//打开文件
	FILE* crtMarkerFile;
	crtMarkerFile = fopen(filePath,"r");
	if (crtMarkerFile == NULL)
	{
		AfxMessageBox("文件打开失败，请确认路径是否有误！");
		return;
	}

	//读取一行的最大字符数
	char buf[MAX_PATH];
	double position[3] = {0};

	//原点
	for (int i=0; i<3; i++)
		pointsInMarker.push_back(position[i]);

	//约定的额外点的关键字
	CString pointTable = "[POINT]";
	CString S0 = "S0=";
	CString S1 = "S1=";
	CString S2 = "S2=";

	//开关变量
	bool IsStartRecord = false;
	bool IsPointReady = false;

	//打开Marker文件，查找并存储Marker坐标系下的额外点
	while (fgets(buf,MAX_PATH,crtMarkerFile) != NULL)
	{
		CString resultStr = buf;
		if (resultStr.Find(pointTable) >= 0)
		{
			IsStartRecord = true;
			IsPointReady = false;
		}

		else if (resultStr.Find(S0) >= 0 && IsStartRecord)
		{
			CString pos0 = resultStr.Mid(S0.GetLength());
			position[0] = atof(pos0);
		}

		else if (resultStr.Find(S1) >= 0 && IsStartRecord)
		{
			CString pos1 = resultStr.Mid(S1.GetLength());
			position[1] = atof(pos1);
		}

		else if (resultStr.Find(S2) >= 0 && IsStartRecord)
		{
			CString pos2 = resultStr.Mid(S2.GetLength());
			position[2] = atof(pos2);

			IsPointReady = true;
			IsStartRecord = false;
		}

		if (IsPointReady)
		{
			for (int i=0; i<3; i++)
				pointsInMarker.push_back(position[i]);

			IsPointReady = false;
		}	
	}
	fclose(crtMarkerFile);

	//计算额外点相对于Camera的坐标
	Xform3D* m2c = dstMarker->marker2CameraXf(this->pCurrCam->Handle());
	Xform3D* t2m = new Xform3D(); 
	Xform3D* t2c; 

	for (int i=0; i<pointsInMarker.size(); i=i+3)
	{
		double pointPosInM[3] = {0};
		double pointPosInC[3] = {0};

		for (int j=0; j<3; j++)
			pointPosInM[j] = pointsInMarker[i + j];

		t2m->setShiftVector(pointPosInM);
		t2c = t2m->concatenate(m2c);
		t2c->getShiftVector(pointPosInC);

		for (int k=0; k<3; k++)
			pointsInCamera.push_back(pointPosInC[k]);

		delete t2c;
	}
	
	delete t2m;
	delete m2c;
	delete markersCollection;
}
//得到图像上所有Xpoint的坐标
void CMT_C::getAllKeyPointsPos(vector<vector<double >> &keyPoints, int XpointsNum)
{
	this->pXPoints->processFrame(this->pCurrCam);
	MTCollection::Collection* xpointsCollection = new MTCollection::Collection(this->pXPoints->detectedXPoints(this->pCurrCam));

	XpointsNum = xpointsCollection->count();

	if (xpointsCollection->count() == 0) {
		delete xpointsCollection;
		return;
	}


	double x3, y3, z3;
	//double x[2], y[2];
	int XPNum = 1;
	MTXPoint* XP;
	double radius = 5;
	// here we need the left side presentation: coordinates in sensor space, distances etc	
	Xform3D* Marker2CurrCameraXf = NULL;
	
	for (XPNum = 1; XPNum <= xpointsCollection->count(); XPNum++)
	{
		XP = new MTXPoint(xpointsCollection->itemI(XPNum));
		//XP->Position2D(&x[0], &y[0], &x[1], &y[1], &x[2], &y[2]);
		XP->Position3D(&x3, &y3, &z3);
		//XP->setIndex(XPNum);
		vector<double> m_point;
		m_point.push_back(x3);
		m_point.push_back(y3);
		m_point.push_back(z3);

		keyPoints.push_back(m_point);
		
	}

	delete xpointsCollection;

}

//释放vector内存
void  CMT_C::deleteVectors()
{
	if (this->sampleVectors.capacity() > 0)
	{
		for (vector<Collection *>::iterator it = sampleVectors.begin(); it != sampleVectors.end(); it ++) 
			if (NULL != *it) 
			{
				delete *it; 
				*it = NULL;
			}
			sampleVectors.clear();
	}

	if (this->facet1ToCameraXfs.capacity() > 0)
	{
		for (vector<Xform3D *>::iterator it = facet1ToCameraXfs.begin(); it != facet1ToCameraXfs.end(); it ++) 
			if (NULL != *it) 
			{
				delete *it; 
				*it = NULL;
			}
			facet1ToCameraXfs.clear();
	}

	if (this->ToolTipToMarkerXfs.capacity() > 0)
	{
		for (vector<Xform3D *>::iterator it = ToolTipToMarkerXfs.begin(); it != ToolTipToMarkerXfs.end(); it ++) 
			if (NULL != *it) 
			{
				delete *it; 
				*it = NULL;
			}
			ToolTipToMarkerXfs.clear();
	}
}

























