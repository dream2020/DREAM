/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Yinfeng fang, University of Portsmouth, David Vernon, University of Skövde 
 * Email:   yinfeng.fang@port.ac.uk, david.vernon@his.se, 
 * Website: www.dream2020.eu 
 * 
 * This file is part of DREAM.
 * 
 * DREAM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * DREAM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with DREAM.  If not, see <http://www.gnu.org/licenses/>.
 */
 

#include "sensoryAnalysis.h"
using namespace std;

////////////////////////////////Dream Project Ports////////////////////////////////////
//Global variables to communication with DreamSave
double FpsCheck = 0;
string TimeCheck;
vector<bool> validflag(30,false);
vector<bool> inputvalidflag(30,false);

Mat Kinect0RGB;
Mat Kinect1RGB;
Mat Kinect0Depth;
Mat Kinect1Depth;

double checkMutualGazeValue;			//		[size = 1][-1/0/1]->[no face/no-mutualgaze/mutualgaze]
vector<double> getArmAngle;				//      [size = 4][left_azimuth/left_elevation/right_azimuth/right_elevation)]
vector<double> getBody;					//		[size = 3][body.x/body.y/body.z]; The body center
vector<double> getBodyPose;				//		[size = 30][joint1.x, joint1.y,joint1.z, ? joint10.x, joint10.y, joint10.z]
vector<double> getEyeGaze;				//		[size = 6][eye.x_in, eye.y_in, eye.z_in, x, y, z]; the left or right eye as input, output gaze direction
vector<double> getEyes;					//		[size = 6][l.x, l.y, l.z, r.x, r.y, r.z,]; the left and right eye
vector<double> getFaces;				//		[size = 16], first bit is the nubmer of faces, following by the face locations
vector<double> getGripLocation;			//	    [size =6][x_in, y_in, z_in,x, y, z], input object loctaion, output the location of a grip point
vector<double> getHands;				//		[size =6][l.x, l.y, l.z, r.x, r.y, r.z]
vector<double> getHead;					//		[size =3][x, y, z]
vector<double> getHeadGaze;				//	    [size =6->12][plane_x_in, plane_y_in, plane_z_in, x, y, z]; when received input output the point on a surface, otherwise the headgaze direction
vector<double> getObjects;				//		[size =12][centre_x_in, centre_y_in, centre_z_in, radius_in, object1.x, object1.y, object1.z, object1.id, object2.x, object2.y, object2.z, object2.id]
vector<double> getObjectTableDistance;	// 		[size =4][object_x_in, object_y_in, object_z_in, vertical_distance]; get the table distance with object location input
vector<double> getSoundDirection;		//		[size =3][threshold_in, horizontal_sound_direction, vertical_sound_direction]
vector<double> identifyFace;			//		[size =4][x_in, y_in, z_in, face_ID]
vector<double> identifyFaceExpression;	//	    [size =4][x_in, y_in, z_in, faceExpresion_ID] faceExpresion_ID = [0, 1, ..., 3,4] -> [happy, sad, angry, fear,nature]
vector<double> identifyObject;			//		[size =4][x_in, y_in, z_in, object_ID], object.ID = [0/1/2/.../6] -> [no object, airplane, car,flower,cup, woods toys,others]; After receiving input
vector<double> identifyTrajectory;		//      [size =12] the first is no motion [The probability of different motions]----Notice: do not match deliverable 1.3
double identifyVoice;					//		[size =1][the ID of the speaker]; [0, 1] -> [threopist, child]
string recognizeSpeech;					//      [the ID of sound]; [0,1,..., 10]->[broom, veshi, gala, smelling sound of flower, wiwi, coocoo, bye bye, youhuu, cry, ho, hmm]----Notice: do not match deliverable 1.3
vector<double> trackFace;				//		[size =7][x_in, y_in, z_in, t_in, x, y, z]; input face location, output the face location after t_in
vector<double> trackHand;				//		[size =7][x_in, y_in, z_in, t_in, x, y, z]; input handlocation, output the hand location after t_in
vector<double> trackObject;				//		[size =8][object_ID_in, x_in, y_in, z_in, t_in, x, y, z]

bool key1record = false;				//		to save the data when it is set to true
bool key2preview = false;				//		to save the data when it is set to false
bool key3DShow = false;
bool main_loop_flag = true;				//		the flag would be set to false after click stop in gyarpmanager to help exit run function
bool logging_flag = false;				//      to keep the state of logging_flag
bool flag_timepoint = false;			//      tthe flag showing if the start time is reseted
clock_t timebegin, timeend;				//		to calculate the time passed

vector<Mat> CameraFrames; //camera frames
vector<bool> LandmarkFlag; //three
vector<Mat_<double> > HeadRotations;
vector<Mat_<double> > GazeRotations;
vector<Point> Eyes_2D;
Rect facebox;
int SelectID;//selected camera ID, -1 means there is no face in the 3 three cameras
vector<string> FaceName;
vector<string> EmotionName;
vector<String> ObjectName;

vector<Rect> ObjectRect;
vector<double> ObjectID;

#ifndef _MUTRUALGAZE_SOUND
#define _MUTRUALGAZE_SOUND
#endif

#ifdef _MUTRUALGAZE_SOUND
bool SpeechMutualGazeFlag = false;
HANDLE funThread;
bool stopthreadflag;
bool SafeExitFlag;
#endif

#ifdef LOAD_IMAGE //to debug when the DREAM Save is off/on
DreamSave DreamPort;					//		the flag would be set to false after clicking the stop button in gyarpmanager to help exit run function
#endif



#ifdef _MUTRUALGAZE_SOUND
DWORD WINAPI PlaySoundThread(LPVOID lpParameter)
{
	SafeExitFlag = false;
	while (!stopthreadflag)
	{
		if (SpeechMutualGazeFlag)
			PlaySound(TEXT("C://DREAM//release//components//sensoryAnalysis//config//MutualGaze.wav"), NULL, SND_ALIAS);
		Sleep(4);
	}
	SafeExitFlag = true;
	return 0;
}
#endif

SensoryAnalysisThread::SensoryAnalysisThread(			
														BufferedPort<VectorOf<int>>		*checkMutualGazeOut,					//1 int checkMutualGaze
														BufferedPort<VectorOf<double>>  *getArmAngleOut,
														BufferedPort<VectorOf<double>>  *getBodyOut,
														BufferedPort<VectorOf<double>>  *getBodyPoseOut, 
														BufferedPort<VectorOf<double>>	*getEyeGazeIn,
														BufferedPort<VectorOf<double>>  *getEyeGazeOut, 
														BufferedPort<VectorOf<double>>  *getEyesOut,
														BufferedPort<VectorOf<double>>  *getFacesOut, 
														BufferedPort<VectorOf<double>>  *getGripLocationIn, 
														BufferedPort<VectorOf<double>>  *getGripLocationOut, //10
														BufferedPort<VectorOf<double>>  *getHandsOut,
														BufferedPort<VectorOf<double>>  *getHeadOut,
														BufferedPort<VectorOf<double>>  *getHeadGazeIn, 
														BufferedPort<VectorOf<double>>  *getHeadGazeOut, 
														BufferedPort<VectorOf<double>>  *getObjectsIn,
														BufferedPort<VectorOf<double>>  *getObjectsOut, 
														BufferedPort<VectorOf<double>>  *getObjectTableDistanceIn, 
														BufferedPort<VectorOf<double>>  *getObjectTableDistanceOut, 
														BufferedPort<VectorOf<double>>  *getSoundDirectionIn,
														BufferedPort<VectorOf<double>>	*getSoundDirectionOut,  //20 
														BufferedPort<VectorOf<double>>  *identifyFaceIn,
														BufferedPort<VectorOf<double>>	*identifyFaceOut,  
														BufferedPort<VectorOf<double>>	*identifyFaceExpressionIn,  
														BufferedPort<VectorOf<int>>		*identifyFaceExpressionOut,  
														BufferedPort<VectorOf<double>>  *identifyObjectIn,
														BufferedPort<VectorOf<double>>  *identifyObjectOut, 
														BufferedPort<VectorOf<int>>		*identifyTrajectoryIn,  
														BufferedPort<VectorOf<double>>  *identifyTrajectoryOut,  
														BufferedPort<VectorOf<int>>		*identifyVoiceOut,
														BufferedPort<Bottle>			*recognizeSpeechOut,//30 
														BufferedPort<VectorOf<double>>  *trackFaceIn,
														BufferedPort<VectorOf<double>>  *trackFaceOut, 
														BufferedPort<VectorOf<double>>  *trackHandIn,
														BufferedPort<VectorOf<double>>  *trackHandOut,  
														BufferedPort<VectorOf<double>>  *trackObjectIn,  //35
														BufferedPort<VectorOf<double>>  *trackObjectOut
                                                    )
{

	checkMutualGazePortOut        = checkMutualGazeOut;
	getArmAnglePortOut            = getArmAngleOut;
	getBodyPortOut                = getBodyOut;
	getBodyPosePortOut            = getBodyPoseOut;
	getEyeGazePortIn              = getEyeGazeIn;			//
	getEyeGazePortOut             = getEyeGazeOut;
	getEyesPortOut                = getEyesOut;  
	getFacesPortOut               = getFacesOut;
	getGripLocationPortIn         = getGripLocationIn; 
	getGripLocationPortOut        = getGripLocationOut;
	getHandsPortOut               = getHandsOut;
	getHeadPortOut                = getHeadOut;
	getHeadGazePortIn             = getHeadGazeIn;			//
	getHeadGazePortOut            = getHeadGazeOut;  
	getObjectsPortIn              = getObjectsIn; 
	getObjectsPortOut             = getObjectsOut;
	getObjectTableDistancePortIn  = getObjectTableDistanceIn;  
	getObjectTableDistancePortOut = getObjectTableDistanceOut;
	getSoundDirectionPortIn       = getSoundDirectionIn; 
	getSoundDirectionPortOut      = getSoundDirectionOut; 
	identifyFacePortIn            = identifyFaceIn;
	identifyFacePortOut           = identifyFaceOut;  
	identifyFaceExpressionPortIn  = identifyFaceExpressionIn;    
	identifyFaceExpressionPortOut = identifyFaceExpressionOut;  
	identifyObjectPortIn          = identifyObjectIn;
	identifyObjectPortOut         = identifyObjectOut; 
	identifyTrajectoryPortIn      = identifyTrajectoryIn;  
	identifyTrajectoryPortOut     = identifyTrajectoryOut;  
	identifyVoicePortOut          = identifyVoiceOut;
	recognizeSpeechPortOut        = recognizeSpeechOut; 
	trackFacePortIn               = trackFaceIn;
	trackFacePortOut              = trackFaceOut;  
	trackHandPortIn               = trackHandIn;
	trackHandPortOut              = trackHandOut;  
	trackObjectPortIn             = trackObjectIn;  
	trackObjectPortOut            = trackObjectOut; 
	
}

bool ReadFRName(const string &Filename_C, vector<string> &FRName_C)
{
	ifstream infile(Filename_C);
	if(!infile.is_open())
	{
		cout << "can not open " << Filename_C<<endl;
		return false;
	}
	string s;
	while (getline(infile, s))
		FRName_C.push_back(s);
	infile.close();
	return true;
}

bool SensoryAnalysisThread::threadInit() 
{

	#ifdef _MUTRUALGAZE_SOUND
	funThread = CreateThread(NULL, 0, PlaySoundThread, NULL, 0, NULL);
#endif

//intitialize global variables as the output from DreamSave

	Eyes_2D.assign(6, Point(0, 0));

	for (int i = 0; i != 3; i++) //create space for mat object
	{
		CameraFrames.push_back(Mat::zeros(960, 1280, CV_8UC3));
		HeadRotations.push_back(Mat::zeros(3, 3, CV_64F));
		GazeRotations.push_back(Mat::zeros(3, 3, CV_64F));
	}
	
	LandmarkFlag.assign(3, false);

	string FRNameList = "C:/DREAM/release/components/sensoryAnalysis/config/FRNameList.txt";
	ReadFRName(FRNameList, FaceName);

	EmotionName.push_back("happy");
	EmotionName.push_back("sad");
	EmotionName.push_back("angry");
	EmotionName.push_back("fear");
	EmotionName.push_back("neutral");

	ObjectName.push_back("car");
	ObjectName.push_back("cup");
	ObjectName.push_back("flower");
	ObjectName.push_back("plane");

	Kinect0RGB.create(480, 640, CV_8UC3);
	Kinect1RGB.create(480, 640, CV_8UC3);
	Kinect0Depth.create(480, 640, CV_8UC3);
	Kinect1Depth.create(480, 640, CV_8UC3);

	checkMutualGazeValue = 0;
	getArmAngle.assign(4,0);
	getBody.assign(3,0);
	getBodyPose.assign(30,0);
	getEyeGaze.assign(7,0); // add camera selection dimension
	getEyes.assign(6,0);
	getFaces.assign(16,0);
	getGripLocation.assign(6,0);
	getHands.assign(6,0);
	getHead.assign(3,0);
	getHeadGaze.assign(12,0);						// Haibing changed from 6 to 12, each plane needs 3 points
	getObjects.assign(12,0);
	getObjectTableDistance.assign(4,0);
	getSoundDirection.assign(3,0);
	identifyFace.assign(4,0);
	identifyFaceExpression.assign(4,0);
	identifyObject.assign(4,0);
	identifyTrajectory.assign(12,0);
	identifyVoice = 0;
	recognizeSpeech = "";
	trackFace.assign(7,0);
	trackHand.assign(7,0);
	trackObject.assign(8,0);

   return true;
}

void drawGaze(Mat& img, const Mat_<double> &rot, Point& startpoint, double lineL)
{
	int thickness = 2;
	int lineType = 8;

	Mat_<double> P = (Mat_<double>(3, 4) <<
		0, lineL, 0, 0,
		0, 0, -lineL, 0,
		0, 0, 0, -lineL);
	P = rot.rowRange(0, 2)*P;
	P.row(0) += startpoint.x;
	P.row(1) += startpoint.y;
	Point p0(P(0, 0), P(1, 0));

	line(img, p0, Point(P(0, 3), P(1, 3)), Scalar(255, 255, 255), thickness, lineType);
}

void drawPose(Mat& img, const Mat_<double> &rot, Point &startpoint, double lineL)
{
	int thickness = 2;
	int lineType = 8;

	Mat_<double> P = (cv::Mat_<double>(3, 4) <<
		0, lineL, 0, 0,
		0, 0, -lineL, 0,
		0, 0, 0, -lineL);
	P = rot.rowRange(0, 2)*P;
	P.row(0) += startpoint.x;
	P.row(1) += startpoint.y;
	cv::Point p0(P(0, 0), P(1, 0));

	line(img, p0, Point(P(0, 1), P(1, 1)), Scalar(255, 0, 0), thickness, lineType);
	line(img, p0, Point(P(0, 2), P(1, 2)), Scalar(0, 255, 0), thickness, lineType);
	line(img, p0, Point(P(0, 3), P(1, 3)), Scalar(0, 0, 255), thickness, lineType);
}
void drawingfunciton(vector<vector<Point> > &landmarks_C)
{
	//rectangle(Kinect1RGB, DreamPort.object_detect->DeskRoi, Scalar(0, 0, 255), 1, 1, 0);
	//draw camera ID
	putText(CameraFrames[0], "Middle Camera0", cvPoint(8, 60), FONT_HERSHEY_SIMPLEX, 1.7, cv::Scalar(125, 255, 255), 6);
	putText(CameraFrames[1], "Left Camera1", cvPoint(8, 60), FONT_HERSHEY_SIMPLEX, 1.7, cv::Scalar(125, 255, 255), 6);
	putText(CameraFrames[2], "Right Camera2", cvPoint(8, 60), FONT_HERSHEY_SIMPLEX, 1.7, cv::Scalar(125, 255, 255), 6);

	if (-1 != SelectID)
	{
		if (SelectID == 0) circle(CameraFrames[0], cv::Point(50, 50), 30, cv::Scalar(0, 0, 255), -1);
		if (SelectID == 1) circle(CameraFrames[1], cv::Point(50, 50), 30, cv::Scalar(0, 0, 255), -1);
		if (SelectID == 2) circle(CameraFrames[2], cv::Point(50, 50), 30, cv::Scalar(0, 0, 255), -1);
		//drawing function
		for (int i = 0; i != 3; i++) // 3 frames to draw
		{
			if (LandmarkFlag[i])
			{
				int PointSize = 4;
				//draw landmark points
				for (int point_i = 0; point_i < 49; point_i++)
				{
					//circle(CameraFrames[i], landmarks_C[i][point_i], PointSize, Scalar(0, 255, 0), -1);
				}
				drawPose(CameraFrames[i], HeadRotations[i], landmarks_C[i][13], 60);// line start from the 10th point
				drawGaze(CameraFrames[i], GazeRotations[i], landmarks_C[i][10], 60);

				///////////////////////////////////draw picture///////////////////////////
				//line(CameraFrames[i], Point(Eyes_2D[2 * i].x - 5, Eyes_2D[2 * i].y), Point(Eyes_2D[2 * i].x + 5, Eyes_2D[2 * i].y), Scalar(0, 0, 255), PointSize, 8);
				//line(CameraFrames[i], Point(Eyes_2D[2 * i].x, Eyes_2D[2 * i].y - 5), Point(Eyes_2D[2 * i].x, Eyes_2D[2 * i].y + 5), Scalar(0, 0, 255), PointSize, 8);
				//line(CameraFrames[i], Point(Eyes_2D[2 * i + 1].x - 5, Eyes_2D[2 * i + 1].y), Point(Eyes_2D[2 * i + 1].x + 5, Eyes_2D[2 * i + 1].y), Scalar(0, 0, 255), PointSize, 8);
				//line(CameraFrames[i], Point(Eyes_2D[2 * i + 1].x, Eyes_2D[2 * i + 1].y - 5), Point(Eyes_2D[2 * i + 1].x, Eyes_2D[2 * i + 1].y + 5), Scalar(0, 0, 255), PointSize, 8);
			}

		}
		//rectangle(CameraFrames[SelectID], facebox, Scalar(0, 0, 255), 1, 1, 0);
		//putText(CameraFrames[SelectID], FaceName[identifyFace[3]], cvPoint(facebox.x + 2, facebox.y + 12), FONT_HERSHEY_SIMPLEX, 1.7, cvScalar(0, 255, 255), 6);
		//putText(CameraFrames[SelectID], EmotionName[identifyFaceExpression[3]], cvPoint(facebox.x + 2, facebox.y + 72), CV_FONT_HERSHEY_COMPLEX, 1.7, cvScalar(0, 255, 255), 6);
	}
	//drawing object
	for (int i = 0; i != DreamPort.ObjectRect.size(); i++)
	{
		rectangle(Kinect1RGB, DreamPort.ObjectRect[i], Scalar(0, 255, 0), 2);
		putText(Kinect1RGB, ObjectName[DreamPort.ObjectID[i]], Point(DreamPort.ObjectRect[i].x, DreamPort.ObjectRect[i].y + DreamPort.ObjectRect[i].height + 15), FONT_ITALIC, 1, Scalar(0, 0, 255), 2, 8);
	}
}

void SensoryAnalysisThread::run(){

	//the flag would be set to false after clicking the stop button in gyarpmanager to help exit run function
	//

#ifdef GUI_Ports //to debug when the DREAM Save is off/on
	PortGUI port_gui;
	string ConfigFilePath = "C://DREAM/release//components//sensoryAnalysis//config//";
	port_gui.init(ConfigFilePath);

#endif

	//int key = -1;

	int key = 0;

	while (main_loop_flag)  
	{

		////////////////////////////////////////////Input Port///////////////////////////////////////////////
		validflag.assign(30,0);
		inputvalidflag.assign(30,0);
		
		VectorOf<double>  *tempDoubleVector;	//to point the receive the latest recived data from yarp ports	
		tempDoubleVector = NULL;
		tempDoubleVector = getEyeGazePortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) {
			inputvalidflag[11] = true;
			getEyeGaze[0] = (*tempDoubleVector)[0];
			getEyeGaze[1] = (*tempDoubleVector)[1];
			getEyeGaze[2] = (*tempDoubleVector)[2];
		}
		tempDoubleVector = NULL;
		tempDoubleVector = getGripLocationPortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) 
		{
			inputvalidflag[14] = true;
			getGripLocation[0] = (*tempDoubleVector)[0];
			getGripLocation[1] = (*tempDoubleVector)[1];
			getGripLocation[2] = (*tempDoubleVector)[2];
		}
		tempDoubleVector = NULL;
		tempDoubleVector = getHeadGazePortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) 
		{
			inputvalidflag[17] = true;
			getHeadGaze[0] = (*tempDoubleVector)[0];
			getHeadGaze[1] = (*tempDoubleVector)[1];
			getHeadGaze[2] = (*tempDoubleVector)[2];						

			for(int h = 0; h < 12; h++)
			{
				getHeadGaze[h] = (*tempDoubleVector)[h];
			}
		}
		tempDoubleVector = NULL;
		tempDoubleVector = getObjectsPortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) {
			inputvalidflag[18] = true;
			getObjects.clear();
			getObjects.push_back((*tempDoubleVector)[0]);
			getObjects.push_back((*tempDoubleVector)[1]);
			getObjects.push_back((*tempDoubleVector)[2]);
			getObjects.push_back((*tempDoubleVector)[3]);
		}
		tempDoubleVector = NULL;
		tempDoubleVector = getObjectTableDistancePortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) {
			inputvalidflag[19] = true;
			getObjectTableDistance[0] = (*tempDoubleVector)[0];
			getObjectTableDistance[1] = (*tempDoubleVector)[1];
			getObjectTableDistance[2] = (*tempDoubleVector)[2];
		}
		tempDoubleVector = NULL;
		tempDoubleVector = getSoundDirectionPortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) {
			inputvalidflag[20] = true;
			getSoundDirection[0] = (*tempDoubleVector)[0];
		}
		tempDoubleVector = NULL;
		tempDoubleVector = identifyFacePortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) {
			inputvalidflag[21] = true;
			identifyFace[0] = (*tempDoubleVector)[0];
			identifyFace[1] = (*tempDoubleVector)[1];
			identifyFace[2] = (*tempDoubleVector)[2];
		}
		tempDoubleVector = NULL;
		tempDoubleVector = identifyFaceExpressionPortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) {
			inputvalidflag[22] = true;
			identifyFaceExpression[0] = (*tempDoubleVector)[0];
			identifyFaceExpression[1] = (*tempDoubleVector)[1];
			identifyFaceExpression[2] = (*tempDoubleVector)[2];
		}
		tempDoubleVector = NULL;
		tempDoubleVector = identifyObjectPortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) {
			inputvalidflag[23] = true;
			identifyObject[0] = (*tempDoubleVector)[0];
			identifyObject[1] = (*tempDoubleVector)[1];
			identifyObject[2] = (*tempDoubleVector)[2];
		}
		tempDoubleVector = NULL;
		tempDoubleVector = identifyObjectPortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) {
			inputvalidflag[24] = true;
			identifyObject[0] = (*tempDoubleVector)[0];
			identifyObject[1] = (*tempDoubleVector)[1];
			identifyObject[2] = (*tempDoubleVector)[2];
			identifyObject[3] = (*tempDoubleVector)[2];
		}
		tempDoubleVector = NULL;
		tempDoubleVector = trackFacePortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) {
			inputvalidflag[27] = true;
			trackFace[0] = (*tempDoubleVector)[0];
			trackFace[1] = (*tempDoubleVector)[1];
			trackFace[2] = (*tempDoubleVector)[2];
			trackFace[3] = (*tempDoubleVector)[3];
		}
		tempDoubleVector = NULL;
		tempDoubleVector = trackHandPortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) {
			inputvalidflag[28] = true;
			trackHand[0] = (*tempDoubleVector)[0];
			trackHand[1] = (*tempDoubleVector)[1];
			trackHand[2] = (*tempDoubleVector)[2];
			trackHand[3] = (*tempDoubleVector)[3];
		}
		tempDoubleVector = NULL;
		tempDoubleVector = trackObjectPortIn->read(false);                   // non-blocking version
		if (tempDoubleVector != NULL) {
			inputvalidflag[29] = true;
			trackObject[0] = (*tempDoubleVector)[0];
			trackObject[1] = (*tempDoubleVector)[1];
			trackObject[2] = (*tempDoubleVector)[2];
			trackObject[3] = (*tempDoubleVector)[3];
		}

#ifdef LOAD_IMAGE //to debug when the DREAM Save is off/on


		/////////////////////function in the GUI main loop/////////////
		//stop button is pressed
		if(!(key1record||key2preview))
		{
			DreamPort.Stop();
			Sleep(20);
		}

		//either save or preview button is pressed
		if(key1record) 
		{
			vector<vector<Point> > landmarks;
			if(!DreamPort.Save(TimeCheck,FpsCheck,inputvalidflag,validflag, CameraFrames,Kinect0RGB,Kinect1RGB, Kinect0Depth,
				Kinect1Depth,checkMutualGazeValue,getArmAngle,getBody,getBodyPose,getEyeGaze,getEyes,
				getFaces,getGripLocation,getHands,getHead,getHeadGaze,getObjects,getObjectTableDistance,
				getSoundDirection,identifyFace,identifyFaceExpression,identifyObject,identifyTrajectory,
				identifyVoice,recognizeSpeech,trackFace,trackHand,trackObject, LandmarkFlag, landmarks, HeadRotations,
				GazeRotations, Eyes_2D, facebox, SelectID,ObjectRect,ObjectID))
			{
				cout << "Error occurs during Dream video saving!" << endl;
				getchar();
			}
			drawingfunciton(landmarks);
		}else if(key2preview){
			vector<vector<Point> > landmarks;
			if(!DreamPort.Preview(TimeCheck,FpsCheck,inputvalidflag,validflag, CameraFrames, Kinect0RGB,Kinect1RGB, Kinect0Depth,
				Kinect1Depth,checkMutualGazeValue,getArmAngle,getBody,getBodyPose,getEyeGaze,getEyes,
				getFaces,getGripLocation,getHands,getHead,getHeadGaze,getObjects,getObjectTableDistance,
				getSoundDirection,identifyFace,identifyFaceExpression,identifyObject,identifyTrajectory,
				identifyVoice,recognizeSpeech,trackFace,trackHand,trackObject, LandmarkFlag, landmarks, HeadRotations,
				GazeRotations, Eyes_2D, facebox, SelectID, ObjectRect,ObjectID))
			{
				cout << "Error occurs during Dream video previewing!" << endl;
				getchar();
			}	
			drawingfunciton(landmarks);	
		}
		//////////////////////////////////////////////////

#endif

		/* Primitive 1 */
      
		/* checkMutualGaze()                                                                               */
		/* ----------------------------------------------------------------------------------------------- */
		/* The checkMutualGaze() primitive returns a value of true or false depending on whether the       */
		/* child and the robot are looking at each other. Note, when detecting mutual gaze, the robot only */
		/* has to determine whether or not the child looks at the robot's head, but not necessarily at the */
		/* robot's eyes. The gaze has to be held for a minimum period, e.g. 3 seconds.                     */
		/*                                                                                                 */
		/* Output TRUE (1) or FALSE (0) to port /checkMutualGaze:o                                         */
		

		if(true == validflag[7])
		{
			#ifdef _MUTRUALGAZE_SOUND
			SpeechMutualGazeFlag = (checkMutualGazeValue == 1);
#endif	

			VectorOf<int> &dataVector1 = checkMutualGazePortOut->prepare();
			dataVector1.clear();
			dataVector1.push_back((int)checkMutualGazeValue);	
			checkMutualGazePortOut->write(); 
		}

		//* Primitive 2 */

		//* getArmAngle(left_azimuth, left_elevation, right_azimuth, right_elevation)                             */
		//* ----------------------------------------------------------------------------------------------------- */
		//* The getArmAngle() primitive returns the azimuth and elevation angles of the child’s upper left        */
		//* and right arms, defined with respect to the robot’s torso, i.e. in a child-centred frame of reference */
		//*                                                                                                       */
		//* Output arm angles to port /getArmAngle:o                                                              */
		//* find out how many events have occurred and perform that many write operations on the port */
	   
		double motion4flag = 0;
		if(true == validflag[10]) // If the skeleton is identified, the arm Angle is identified as well.
		{
			validflag[8] = true;
			MyVector3D left_S = {getBodyPose[6],getBodyPose[7],getBodyPose[8]};
			MyVector3D left_E = {getBodyPose[9],getBodyPose[10],getBodyPose[11]};
			MyVector3D left_W = {getBodyPose[12],getBodyPose[13],getBodyPose[14]};
			MyVector3D right_S = {getBodyPose[18],getBodyPose[19],getBodyPose[20]};
			MyVector3D right_E = {getBodyPose[21],getBodyPose[22],getBodyPose[23]};
			MyVector3D right_W = {getBodyPose[24],getBodyPose[25],getBodyPose[26]};
		
			motion4flag = getangles(left_S, left_E, left_W, right_S, right_E, right_W, getArmAngle); 
		}
		
		if(true == validflag[8] )
		{   
			VectorOf<double> &dataVector2 = getArmAnglePortOut->prepare();
			dataVector2.clear();
			for(int i=0; i< 4; i++)
			{
				dataVector2.push_back(getArmAngle[i]);
			}
			getArmAnglePortOut->write(); 		
		}


		/* Primitive 3 */
   
		/* getBody(x, y, z)                                                               */
		/* ------------------------------------------------------------------------------ */
		/* The getBody() primitive returns the location of the centre of the child’s body */
		/*                                                                                */
		/* Output the location of the centre of the child’s body to port /getBody:o       */
		
		if(true == validflag[9] )
		{
			VectorOf<double> &dataVector3 = getBodyPortOut->prepare();
			dataVector3.clear();
			for(int i=0; i< 3; i++)
			{
				dataVector3.push_back(getBody[i]);
			}
			getBodyPortOut->write(); 

		}

		//* Primitive 4 */
  //      
		//* getBodyPose(<joint_i>)                                                                                                 */
		//* ---------------------------------------------------------------------------------------------------------------------- */
		//* The getBodyPose() primitive returns a list (denoted by < >) of joint angles describing the child’s current body pose   */
		//* The joint angles correspond to a canonical representation of a body skeleton, yet to be defined                        */
		//* The first item in the list is the number of joint angles that follow.                                                  */
		//*                                                                                                                        */
		//* Output a vector of joint angles specifying the child’s current body post to port /getBodyPose:o                        */
	
		if( true == validflag[10] )
		{
			VectorOf<double> &dataVector4 = getBodyPosePortOut->prepare();
			dataVector4.clear();

			for(int i=0; i< 30; i++)
			{
				dataVector4.push_back(getBodyPose[i]);
			}
			getBodyPosePortOut->write(); 
		}


		//* Primitive 5 */  

		//* getEyeGaze(eye, x, y, z)              */
		//* ------------------------------------- */
		//* The getEyeGaze() primitive returns the location in the world at which the child’s eye gaze is     */
		//* directed. The gaze direction is given by the line connecting the child’s eye and that location.   */
		//* Any point on that line could be the object of the child’s attention. The eye to be used is passed */ 
		//* in the eye parameter (0 for left a 1 for right).                                                  */ 
		//*                                                                                                   */
		//* Input eye from port /getEyeGaze:i                                                                 */
		//* Output x, y, z, to port /getEyeGaze:o                                                             */

		
		if( true == validflag[11] )
		{
			inputvalidflag[11] = false;
			VectorOf<double> &dataVector5 = getEyeGazePortOut->prepare();
			dataVector5.clear();

			for(int i=0; i< 4; i++) //the first 3 bits are the input, add the last digit as the camera being selected
			{
				dataVector5.push_back(getEyeGaze[3+i]);
			}
			getEyeGazePortOut->write(); 
		
		}



		//* Primitive 6 */ 

		//* getEyes(eyeL_x, eyeL_y, eyeL_z, eyeR_x, eyeR_y, eyeR_z)                    */
		//* -------------------------------------------------------------------------- */
		//* The getEyes() primitive the locations of the child’s left and right eyes,  */
		//* specified in Cartesian coordinates in a world frame of reference.          */
		//*                                                                            */
		//* Output the locations of the child’s left and right eyes to port /getEyes:o */
		
		if( true == validflag[12] )
		{
			VectorOf<double> &dataVector6 = getEyesPortOut->prepare();
			dataVector6.clear();
			for(int i=0; i< 6; i++)
			{
				dataVector6.push_back(getEyes[i]);
			}
			getEyesPortOut->write();
		}
       

      /* Primitive 7 */ 

      /* getFaces(<x, y, z>)                                                                             */
      /* ----------------------------------------------------------------------------------------------- */
      /* The getFaces() primitive returns a list (denoted by < >) of locations where faces are visible   */
      /* in the therapy environment (viewed from any of the therapy cameras). Each location is specified */
      /* by a triple of Cartesian coordinates x, y, and z.                                               */
      /* The first item in the list is the number of triples that follow.                                */
      /* The maximum number of faces is given by NUMBER_OF_FACES                                         */
      /*                                                                                                 */
      /* Output locations of faces that are visible in the therapy environment to port /getFaces:o       */
		
		if( true == validflag[13] )
		{
			VectorOf<double> &dataVector7 = getFacesPortOut->prepare();	  
			dataVector7.clear();		//the first one is face nubmer, maxium number of faces =  5
			for(int i=0;i<16;i++)
			{
				dataVector7.push_back(getFaces[i]);
			}
			getFacesPortOut->write();
		}


		//* Primitive 8 */ 

		//* getGripLocation(object_x, object_y, object_z, grip_x, grip_y, grip_z)                        */
		//* -------------------------------------------------------------------------------------------- */
		//* The getGripLocation() primitive returns the location of a grip point of a play object in the */
		//* therapy environment. The location of the object is passed to the primitive.                  */
		//*                                                                                              */
		//* Input location of an object from port /getGripLocation:i                                     */
		//* Output grip point to port /getGripLocation:o                                                 */
	
		if( true == validflag[14] )
		{
			inputvalidflag[14] = false;
			VectorOf<double> &dataVector8 = getGripLocationPortOut->prepare();
			dataVector8.clear();//maxium number of faces =  5

			for(int i = 0; i < 3; i++ )
			{
				dataVector8.push_back(getGripLocation[3+i]);
			}
			getGripLocationPortOut->write();
		}

      /* Primitive 9 */  

      /* getHands(<x, y, z>)                                                                             */
      /* ----------------------------------------------------------------------------------------------- */
      /* The getHands() primitive returns a list (denoted by < >) of locations where hands are visible   */
      /* in the therapy environment (viewed from any of the therapy cameras). Each location is specified */
      /* by a triple of Cartesian coordinates x, y, and z.                                               */
      /* The first item in the list is the number of triples that follow.                                */
      /* The maximum number of hands is given by NUMBER_OF_HANDS                                         */
      /*                                                                                                 */
      /* Output locations of hands that are visible in the therapy environment to port /getHands:o       */
		
		if( true == validflag[15] )
		{
			VectorOf<double> &dataVector9 = getHandsPortOut->prepare();
			dataVector9.clear();
 			for(int i =0; i<6;i++)
			{
				dataVector9.push_back(getHands[i]);
			}
			getHandsPortOut->write(); 
		}
 
   
		//* Primitive 10 */ 
  // 
		//* getHead(x, y, z)                                                                          */
		//* ----------------------------------------------------------------------------------------- */
		//* The getHead() primitive returns the locations of the child’s head, specified in Cartesian */
		//* coordinates in a world frame of reference.                                                */
		//*                                                                                           */
		//* Output location of the child’s head to port /getHead:o                                    */
		
		if( true == validflag[16] )
		{
			VectorOf<double> &dataVector10 = getHeadPortOut->prepare();
			dataVector10.clear();
			for(int i =0; i<3; i++)
			{
				dataVector10.push_back(getHead[i]);
			}
			getHeadPortOut->write(); 
		}

 
      /* Primitive 11 */ 
   
      /* getHeadGaze(x, y, z)                                                                                         */
      /* ------------------------------------------------------------------------------------------------------------ */
      /* The getHeadGaze() primitive returns the location in the world at which the child’s head is directed.         */
      /* The head direction is given by the line connecting the mid-point between the child’s eyes and that location. */
      /* Any point on that line could be the object of the child’s attention.                                         */
      /*                                                                                                              */
      /* Output location of the point in the world at which the child’s head is directed to port /getHeadGaze:o       */

	
		if( true == validflag[17] )
		{
			inputvalidflag[17] = false;
			VectorOf<double> &dataVector11 = getHeadGazePortOut->prepare();
			dataVector11.clear();
			for(int i =0; i<3; i++)
			{
				dataVector11.push_back(getHeadGaze[9+i]);
			}
			getHeadGazePortOut->write();
		}

      /* Primitive 12 */ 

      /* getHeadGaze(p_x1, p_y1, p_z1, p_x2, p_y2, p_z2, p_x3, p_y3, p_z3, x, y, z)                                           */
      /* -------------------------------------------------------------------------------------------------------------------- */
      /* This alternative version of the getHeadGaze() primitive returns the location on a flat surface                       */
      /* (i.e. a plane defined by a vector of Cartesian coordinates ) at which the child’s head is directed.                  */
      /* Typically, this plane corresponds to the table on which play objects are placed.                                     */
      /*                                                                                                                      */
      /* Input a triple of 3-D coordinates specifying the plane of a flat surface from port /getHeadGaze:i                    */
      /* Output location of the location a point on that surface at which the child’s head is directed to port /getHeadGaze:o */



      /* Primitive 13 */  

      /* getObjects(<x, y, z>)                                                                                        */
      /* ------------------------------------------------------------------------------------------------------------ */
      /* The getObjects() primitive returns a list (denoted by < >) of locations where play objects are visible       */
      /* in the therapy environment (viewed from any of the therapy cameras). Each location is specified by a triple  */
      /* of Cartesian coordinates x, y, and z. The first item in the list is the number of triples that follow.       */
      /* The maximum number of objects is given by NUMBER_OF_OBJECTS                                                  */
      /*                                                                                                              */
      /* Output locations of objects that are visible in the therapy environment to port /getObjects:o                */


		if( true == validflag[18] )
		{
			inputvalidflag[18] = false;
			VectorOf<double> &dataVector13 = getObjectsPortOut->prepare();
			dataVector13.clear();
			for(int i =0; i<getObjects.size(); i++)
			{
				dataVector13.push_back(getObjects[i]);
			}
			getObjectsPortOut->write();
		}

      /* Primitive 14 */ 

  //    /* getObjects(centre_x, centre_y, centre_z, radius, <x, y, z>)                                                  */
  //    /* ------------------------------------------------------------------------------------------------------------ */
  //    /* This alternative form of getObjects() primitive returns a list (denoted by < >) of locations                 */
  //    /* where play objects are visible in the therapy environment (viewed from any of the therapy                    */
  //    /* cameras). Each location is specified by a triple of Cartesian coordinates x, y, and z. In this case,         */
  //    /* the search for the objects is restricted to a circular region given by the centre coordinates                */
  //    /* and radius parameters. The first item in the list is the number of triples that follow.                                                                                      */
  //    /*                                                                                                              */
  //    /* Input the coordinates specifying the centre of a circle and its radius from port /getObjects:i               */
  //    /* Output location of objects that are visible in this circle in the therapy environment to port /getObjects:o  */

  //    /* Primitive 15 */

  //    /* getObjectTableDistance(object_x, object_y, object_z, vertical_distance)                        */
  //    /* ---------------------------------------------------------------------------------------------- */
  //    /* The getObjectTableDistance() primitive returns the vertical distance of an object given by the */
  //    /* Cartesian coordinates to the table on which play objects are places.                           */
  //    /*                                                                                                */
  //    /* Input location of an object from port /getObjectTableDistance:i                                */
  //    /* Output vertical distance between object and table to port /getObjectTableDistance:o            */


		if( true == validflag[19] )
		{
			VectorOf<double> &dataVector15 = getObjectTableDistancePortOut->prepare();
			dataVector15.clear();
			dataVector15.push_back(getObjectTableDistance[3+0]);
			getObjectTableDistancePortOut->write(); 
		}

 
  //    /* Primitive 16 */
  // 
  //    /* getSoundDirection(threshold, azimuth, elevation)                                                           */
  //    /* ---------------------------------------------------------------------------------------------------------- */
  //    /* The getSoundDirection() primitive returns the horizontal (azimuth) and vertical (elevation) angles         */
  //    /* defining the direction to the loudest sound in the environment (e.g. when someone is speaking).            */
  //    /* The angles are defined with respect to the robot’s local frame of reference centred in its torso.          */
  //    /*                                                                                                            */
  //    /* Input sound threshold from from port /getSoundDirection:i                                                  */
  //    /* the azimuth) and elevation angles defining the direction to the loudest sound to port /getSoundDirection:o */

		
		if( true == validflag[20] )
		{
			VectorOf<double> &dataVector16 = getSoundDirectionPortOut->prepare();
			dataVector16.clear();
			for (int i=0; i<2; i++) 
			{
				dataVector16.push_back(getSoundDirection[1+i]);
			}
			getSoundDirectionPortOut->write();
		}

  //    /* Primitive 17 */ 

  //    /* identifyFace(x, y, z, face_id)                                                                       */
  //    /* ---------------------------------------------------------------------------------------------------- */
  //    /* The identifyFace() primitive classifies the face at the specified location. The possible classes     */
  //    /* are determined during the set up phase. Typically, they will include the therapist and the           */
  //    /* children involved in the therapy sessions. The class identifier is returned in the face_id parameter */
  //    /* The maximum number of faces is given by NUMBER_OF_FACES                                              */
  //    /*                                                                                                      */
  //    /* Input location of a face from port /identifyFace:i                                                   */
  //    /* Output face identification number to port /identifyFace:o                                            */


		if( true == validflag[21] )
		{
			VectorOf<double> &dataVector17 = identifyFacePortOut->prepare();
			dataVector17.clear();  
			dataVector17.push_back(identifyFace[3]);
			identifyFacePortOut->write();
		}
		

  //    /* Primitive 18 */ 

  //    /* identifyFaceExpression(x, y, z, expression_id)                                                       */
  //    /* ---------------------------------------------------------------------------------------------------- */
  //    /* The identifyFaceExpression() primitive classifies the expression of the face at the specified        */
  //    /* location. The possible classes are determined during the set up phase. Typically, they will be       */
  //    /* happy, sad, angry, or fearful (i.e. the four classes of emotion used in the interventions defined in */
  //    /* Deliverable D1.1). The emotion identifier is returned in the expression_id parameter.                */
  //    /* The maximum number of expressions is given by NUMBER_OF_EXPRESSIONS                                  */
  //    /*                                                                                                      */
  //    /* Input location of a face from port /identifyFaceExpression:i                                         */
  //    /* Output expression identification number to port /identifyFaceExpression:o                            */


		
		if( true == validflag[22] )
		{
			VectorOf<int> &dataVector18 = identifyFaceExpressionPortOut->prepare();
			dataVector18.clear();
			dataVector18.push_back((int)identifyFaceExpression[3]);
			identifyFaceExpressionPortOut->write();
		}
 
  //    /* Primitive 19 */

  //    /* identifyObject(x, y, z, object_id)                                                              */
  //    /* ----------------------------------------------------------------------------------------------- */
  //    /* The identifyObject() primitive classifies the object at the specified location. The possible    */
  //    /* classes are determined during the set up phase. Typically, they will correspond to a designated */
  //    /*  set of play objects, pictures to be placed on the table, or pictures on the sand-tray.         */
  //    /* The object identifier is returned in the object_id parameter.                                   */
  //    /*                                                                                                 */
  //    /* Input location of an from port /identifyObject:i                                                */
  //    /* Output object identification number to port /identifyObject:o                                   */



		if( true == validflag[23] )
		{
			VectorOf<double> &dataVector19 = identifyObjectPortOut->prepare();
			dataVector19.clear();     
			dataVector19.push_back(identifyObject[3+0]);
			identifyObjectPortOut->write();    
		}

  //    /* Primitive 20 */ 

  //    /* identifyTrajectory(<x, y, z, t>, trajectory_descriptor)                                                      */
  //    /* -------------------------------------------------------------------------------------------------------------------- */
  //    /* The identifyTrajectory() primitive classifies the trajectory defined by a list of 4-tuples, each 4-tuple defining    */
  //    /* a 3-D location and a time-stamp. The possible classes are determined during the set up phase. Typically, they will   */
  //    /* correspond to a designated set of hand gestures to be made by the child (e.g. a wave). The first item in the list    */
  //    /* is the number of 4-tuples that follow.                                                                               */
  //    /* The trajectory identifier is returned in the trajectory_descriptor parameter.                                        */
  //    /*                                                                                                                      */
  //    /* Input a trajectory defined by a sequence of 4-tuples (3-D location and a time-stamp) from port /identifyTrajectory:i */
  //    /* Output location of the location a point on that surface at which the child’s head is directed to port /getHeadGaze:o */
	
		
		
		if( true == validflag[24] )
		{
			VectorOf<double> &dataVector20 = identifyTrajectoryPortOut->prepare();
			dataVector20.clear();

			if(motion4flag) //add by cai 20171105
			{
				for(int i = 0; i <12; i++)
				{
					identifyTrajectory[i]=0;
				}
				identifyTrajectory[4]=1;
			}

			for (int i=0; i<12; i++) 
			{
				dataVector20.push_back(identifyTrajectory[i]);
			}

			identifyTrajectoryPortOut->write(); 
		}  

  //    /* Primitive 21 */ 

  //    /* identifyVoice(voice_descriptor)                                                                       */
  //    /* ----------------------------------------------------------------------------------------------------- */
  //    /* The identifyVoice() primitive classifies the voice that is currently speaking. The possible classes   */
  //    /* are determined during the set up phase. Typically, they will include the therapist and the children   */
  //    /*  involved in the therapy sessions. The voice identifier is returned in the voice_descriptor parameter.*/
  //    /*                                                                                                       */
  //    /* Output voice identification number to port /identifyVoice:o                                           */


		if( (true == validflag[25]) || (true == validflag[20]))					//If valid information is received from Haibin's code, or sound directio nis valid
		{
			VectorOf<int> &dataVector21 = identifyVoicePortOut->prepare();
			dataVector21.clear();
			if (true == validflag[25])
			{				
				//if(debug_flag) cout<<"Flag for identifyVoice: " << validflag[21+skip_N]<<endl;				
				dataVector21.push_back((int)identifyVoice);							//the theropist's ID. The total number of children is 6, starting from 1;
				identifyVoicePortOut->write(); 
			} else {
				//cout<<getSoundDirection[1]<<" "<<getSoundDirection[2]<<endl;
				//if(debug_flag) cout<<"Flag for identifyVoice: " << validflag[21+skip_N]<<endl;				
				if ((getSoundDirection[1]>20 || getSoundDirection[1] <-20) && getSoundDirection[2] >0.5) // if the sound direciton is from the side of the table wiht more than 0.5 confidence 
				{
					dataVector21.push_back(0); //the theropist's ID. The total number of children is 6, starting from 1;
					identifyVoice = 0;
					identifyVoicePortOut->write(); 
				} else if(((getSoundDirection[1]<20) && (getSoundDirection[1] >-20)) && (getSoundDirection[2] >0.5) && (getSoundDirection[1] != 0))  {// if the sound direciton is front ofthe table wiht more than 0.5 confidence 
																																			 // when there is no sound, the output is always 0, so 0 is excluded. 
					dataVector21.push_back(1);
					identifyVoice = 1;
					identifyVoicePortOut->write(); 
				}
				else
				{
					identifyVoice = -1;
				}
			}
		}


  //    /* Primitive 22 */ 

  //    /* recognizeSpeech(text)                                                                                                              */
  //    /* ---------------------------------------------------------------------------------------------------------------------------------- */
  //    /* The recognizeSpeech() primitive returns a textual representation of anything currently being spoken by the therapist or the child. */
  //    /*                                                                                                                                    */
  //    /* Output a textual representation of anything currently being spoken by the therapist or the child to port /recognizeSpeech:o        */



		if( true == validflag[26] )
		{		
			cout << "speech valid!" <<endl;
			//if(debug_flag) cout<<"Flag for recognizeSpeech: " << validflag[22+skip_N]<<endl;  
			//cout<<recognizeSpeech<<endl;
			Bottle &dataVector22 = recognizeSpeechPortOut->prepare();
			dataVector22.clear(); // receive string, and output int for speech recognition   
			dataVector22.fromString(recognizeSpeech);
			//if (recognizeSpeech=="Broom,broom") dataVector22.fromString("Broom,broom");
			//else if (recognizeSpeech=="Veshi") dataVector22.fromString("Veshi");
			//else if (recognizeSpeech=="gela") dataVector22.fromString("gela");
			//else if (recognizeSpeech=="smelling sound of flower") dataVector22.fromString("smelling sound of flower");
			//else if (recognizeSpeech=="wiwi") dataVector22.fromString("wiwi");
			//else if (recognizeSpeech=="coocoo -hide ourselves") dataVector22.fromString("coocoo");
			//else if (recognizeSpeech=="Bye bye") dataVector22.fromString("Bye bye");
			//else if (recognizeSpeech=="youhuu") dataVector22.fromString("youhuu");
			//else if (recognizeSpeech=="ho") dataVector22.fromString("ho");
			//else if (recognizeSpeech=="hmm, hmm") dataVector22.fromString("hmm, hmm");
			//else {
			//	cout << "wrong speech label " <<endl;
			//	dataVector22.fromString("hmm, hmm");
			//}
			recognizeSpeechPortOut->write(); 
		}

  //       
  //
  //    /* Primitive 23 */ 

  //    /* trackFace(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z)                 */
  //    /* ------------------------------------------------------------------------------------------------------- */
  //    /* The trackFace() primitive tracks the face that is currently located at the position given by the        */
  //    /* Cartesian seed coordinates and returns the projected location in the next time interval. The            */
  //    /* duration of the time interval is specified in milliseconds. By reassigning the projected location       */
  //    /* to the seed and repeatedly invoking trackFace() the locations of the face can be tracked and  recorded. */
  //    /*                                                                                                         */
  //    /* Input seed coordinates of a face to be tracked and time interval  milliseconds from port /trackFace:i   */
  //    /* Output the projected location in the next time interval to port /trackFace:o                            */
	

 
		if( true == validflag[27] )
		{
			//if(debug_flag) cout<<"Flag for trackFace: " << validflag[23+skip_N]<<endl; 
			inputvalidflag[27] = false;
			VectorOf<double> &dataVector23 = trackFacePortOut->prepare();
			dataVector23.clear();     
			for (int i=0; i<3; i++) 
			{
				dataVector23.push_back(trackFace[4 + i]);
			}
			trackFacePortOut->write();
		}
 


  //    /* Primitive 24 */ 

  //    /* trackHand(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z)               */
  //    /* ----------------------------------------------------------------------------------------------------- */
  //    /* The trackHand() primitive tracks the hand that is currently located at the position given by the      */
  //    /* Cartesian seed coordinates and returns the projected location in the next time interval. The          */
  //    /* duration of the time interval is specified in milliseconds. By reassigning the projected location     */
  //    /* to the seed and repeatedly invoking trackHand() the locations of the hand can be tracked and recorded */
  //    /*                                                                                                       */
  //    /* Input seed coordinates of a hand to be tracked and time interval  milliseconds from port /trackHand:i */
  //    /* Output the projected location in the next time interval to port /trackHand:o                          */



		if( true == validflag[28] )
		{
			inputvalidflag[28] = false;
			VectorOf<double> &dataVector24 = trackHandPortOut->prepare();
			dataVector24.clear(); 
			for (int i=0; i<3; i++) 
			{    
				dataVector24.push_back(trackHand[4+i]);
			}		
			trackHandPortOut->write();     
		}

  //    
  //    /* Primitive 25 */

  //    /* trackObject(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z)               */
  //    /* ------------------------------------------------------------------------------------------------------- */
  //    /* The trackObject() primitive tracks the object given by the object descriptor and currently              */
  //    /* located at the position given by the Cartesian seed coordinates and returns the projected location      */
  //    /* in the next time interval. The duration of the time interval is specified in milliseconds. By           */
  //    /* reassigning the projected location to the seed and repeatedly invoking trackObject() the                */
  //    /* locations of the object can be tracked and recorded.                                                    */
  //    /*                                                                                                         */
  //    /* Input seed coordinates of object to be tracked and time interval  milliseconds from port /trackObject:i */
  //    /* Output the projected location in the next time interval to port /trackObject:o                          */



		if( true == validflag[29] )
		{
			inputvalidflag[29] = false;
			VectorOf<double> &dataVector25 = trackObjectPortOut->prepare();
			dataVector25.clear(); 
			for (int i=0; i<3; i++) 
			{    
				dataVector25.push_back(trackObject[5+i]);
			}		
			trackHandPortOut->write();    
		}


#ifdef GUI_Ports
		//get the trajectory ID
		static int TrajectoryID=0;
		if(true == validflag[24])
		for(int i=0; i<12; i++)
		{	
			if(1 == identifyTrajectory[i])
			{
				TrajectoryID = i;
				break;
			}
		}

		std::string tempVoice = (identifyVoice == 1) ? "Child" : "Theropist";
		if (identifyVoice == -1) tempVoice = "No";
		int guifaceid = identifyFace[3];
		int guiface_exid = identifyFaceExpression[3];

		if(!port_gui.show(
				Kinect0RGB, Kinect0Depth,Kinect1RGB,Kinect1Depth, //update video data to GUI for dispaly
				CameraFrames[0], CameraFrames[1], CameraFrames[2],	
				key2preview, key1record, DreamPort.Dream3DViewFlag,TimeCheck, FpsCheck, checkMutualGazeValue,
			getSoundDirection[1], tempVoice, recognizeSpeech, guifaceid, guiface_exid, TrajectoryID))						//get the key state from the GUI
		{
			//exit button clicked
			DreamPort.Stop();
			main_loop_flag = false;
			break;
		}
#endif	
	}

}


void SensoryAnalysisThread::threadRelease() 
{	
	main_loop_flag = 0;
#ifdef LOAD_IMAGE //to debug when the DREAM Save is off/on
	DreamPort.Stop();
#endif

}

//
double SensoryAnalysisThread::getangles(MyVector3D left_S,MyVector3D left_E,MyVector3D left_W, MyVector3D right_S, MyVector3D right_E, MyVector3D right_W, vector<double> &angles)
{
	//define the variables to calculate angles
	//to calculate left angles
	double left_elevation, left_azimuth, right_elevation, right_azimuth;

	MyVector3D vector1;  
	MyVector3D vector2;
	MyVector3D vector3;
	MyVector3D vector4;

	MyVector3D normal1;  
	MyVector3D normal2;

	double deltaNormal1;  
	double deltaNormal2;  
	double deltaNormal3;  
	double deltaNormal4; 

	double deltaVector1;
	double deltaVector2;

	double cosAngle1; 
	double cosAngle2;

	MyVector3D vector5;  
	MyVector3D vector6;
	MyVector3D vector7;
	MyVector3D vector8;
	MyVector3D vector9;
	MyVector3D vector10;
	MyVector3D vector11;

	MyVector3D normal3;
	MyVector3D normal4;

	double deltaNormal5;  
	double deltaNormal6;  
	double deltaNormal7;  
	double deltaNormal8; 
	double deltaVector3;
	double deltaVector4;

	double cosAngle3; 
	double cosAngle4;
	double cosAngle5;	
	double cosAngle6;	
	double cosAngle7;	// CHECK RIGHT SHOULDER ANGLE
	double cosAngle8;	// CHECK LEFT SHOULDER ANGLE


	//////////////////////////////////////////////////////////////////////DISI 11.5
	// USE INPUT PARAMETER TO CALCULATE SHOULDER & SPINE  

	// GENERATE POINTS
	MyVector3D Sp;                 
	MyVector3D Center_S;

	Center_S.X = (left_S.X + right_S.X)/2;
	Center_S.Y = (left_S.Y + right_S.Y)/2;
	Center_S.Z = (left_S.Z + right_S.Z)/2;

	Sp.X = 0;
	Sp.Y = (((right_S.Y - left_S.Y) * (right_S.Y + left_S.Y)) - ((right_S.X - left_S.X) * (right_S.X + left_S.X))) / (2 * (right_S.Y - left_S.Y));
	Sp.Z = (left_S.Z + right_S.Z)/2;

	vector9.X = Sp.X - Center_S.X; // vector9  shoulder center - spine
	vector9.Y = Sp.Y - Center_S.Y;  
	vector9.Z = Sp.Z - Center_S.Z; 

	vector10.X = left_E.X - left_S.X;  // vector10  shoulder left - elbow  <---- vector1
	vector10.Y = left_E.Y - left_S.Y;  
	vector10.Z = left_E.Z - left_S.Z;

	vector11.X = right_E.X - right_S.X;  // vector11  shoulder right - elbow
	vector11.Y = right_E.Y - right_S.Y;  
	vector11.Z = right_E.Z - right_S.Z; 

	double innerProduct1 = vector9.X*vector10.X+vector9.Y*vector10.Y+vector9.Z*vector10.Z;
	double module1_1 = sqrt(vector9.X*vector9.X+vector9.Y*vector9.Y+vector9.Z*vector9.Z);
	double module1_2 = sqrt(vector10.X*vector10.X+vector10.Y*vector10.Y+vector10.Z*vector10.Z);
	cosAngle7 = innerProduct1/(module1_1*module1_2);

	double innerProduct2 = vector11.X*vector9.X+vector11.Y*vector9.Y+vector11.Z*vector9.Z;
	double module2_1 = sqrt(vector11.X*vector3.X+vector11.Y*vector3.Y+vector3.Z*vector3.Z);
	double module2_2 = sqrt(vector9.X*vector9.X+vector9.Y*vector9.Y+vector9.Z*vector9.Z);
	cosAngle8 = innerProduct2/(module2_1 * module2_2);

	//////////////////////////////////////////////////////////////////////DISI 11.5

	vector1.X = left_S.X - left_E.X; // vector1 shoulder - elbow
	vector1.Y = left_S.Y - left_E.Y;  
	vector1.Z = left_E.Z - left_S.Z; 

	vector2.X = left_W.X - left_E.X; // vector2  hand - elbow  
	vector2.Y = left_W.Y - left_E.Y;  
	vector2.Z = left_E.Z - left_W.Z;

	double innerProduct3 = vector1.X*vector2.X+vector1.Y*vector2.Y+vector1.Z*vector2.Z;
	double module3_1 = sqrt(vector1.X*vector1.X+vector1.Y*vector1.Y+vector1.Z*vector1.Z);
	double module3_2 = sqrt(vector2.X*vector2.X+vector2.Y*vector2.Y+vector2.Z*vector2.Z);
	cosAngle1 = innerProduct3 / (module3_1 * module3_2);

	//

	vector5.X = right_S.X - right_E.X; // vector5 shoulder - elbow
	vector5.Y = right_S.Y - right_E.Y;  
	vector5.Z = right_S.Z - right_E.Z; 

	vector6.X = right_W.X - right_E.X; // vector6  hand - elbow  
	vector6.Y = right_W.Y - right_E.Y;  
	vector6.Z = right_W.Z - right_E.Z;
	
	double innerProduct4 = vector5.X*vector6.X+vector5.Y*vector6.Y+vector5.Z*vector6.Z;
	double module4_1 = sqrt(vector5.X*vector5.X+vector5.Y*vector5.Y+vector5.Z*vector5.Z);
	double module4_2 = sqrt(vector6.X*vector6.X+vector6.Y*vector6.Y+vector6.Z*vector6.Z);
	cosAngle2 = innerProduct4 / (module4_1 *module4_2);


	//normal1.X = vector1.Y * vector2.Z - vector1.Z * vector2.Y;  // normal1 = vector1 x vector2
	//normal1.Y = vector1.Z * vector2.X - vector1.X * vector2.Z;  
	//normal1.Z = vector1.X * vector2.Y - vector1.Y * vector2.X; 

	//normal2.X = 0.0;  
	//normal2.Y = -100.0;  
	//normal2.Z = 0.0;

	//vector3.X = left_E.X - left_S.X;  
	//vector3.Y = left_E.Y - left_S.Y;  
	//vector3.Z = 0.0;  

	//vector4.X = 0.0;  
	//vector4.Y = 100;  
	//vector4.Z = 0.0;  

	//deltaNormal1 = sqrt(normal1.X * normal1.X + normal1.Y * normal1.Y + normal1.Z * normal1.Z);  
	//deltaNormal2 = sqrt(normal2.X * normal2.X + normal2.Y * normal2.Y + normal2.Z * normal2.Z); 
	//deltaNormal3 = sqrt(vector3.X * vector3.X + vector3.Y * vector3.Y + vector3.Z * vector3.Z);  
	//deltaNormal4 = sqrt(vector4.X * vector4.X + vector4.Y * vector4.Y + vector4.Z * vector4.Z);  

	//if (deltaNormal1*deltaNormal2 >0.0)
	//{
	//	cosAngle1 = (normal1.X * normal2.X + normal1.Y * normal2.Y + normal1.Z * normal2.Z) / (deltaNormal1 * deltaNormal2);

	//	left_elevation = acos(cosAngle1) *180 /M_PI;
	//}
	//if (deltaNormal3 * deltaNormal4 >0.0)
	//{
	//	cosAngle2 = (vector3.X * vector4.X + vector3.Y * vector4.Y + vector3.Z * vector4.Z) / (deltaNormal3 * deltaNormal4);
	//	left_azimuth = acos(cosAngle2)*180/M_PI;

	//}

	////to calculate joint angle
	//deltaVector1 = sqrt(vector1.X * vector1.X + vector1.Y * vector1.Y + vector1.Z * vector1.Z);  
	//deltaVector2 = sqrt(vector2.X * vector2.X + vector2.Y * vector2.Y + vector2.Z * vector2.Z);

	//if (deltaVector1 * deltaVector2 > 0.0)  
	//{  
	//	double cosAngle5 = (vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z) / (deltaVector1 * deltaVector2);  
	//	int jointangle1 = acos(cosAngle5)*180/M_PI;
	//}

	////to calculate right angles
	//vector5.X = right_S.X - right_E.X; // vector5 shoulder - elbow
	//vector5.Y = right_S.Y - right_E.Y;  
	//vector5.Z = right_E.Z - right_S.Z; 

	//vector6.X = right_W.X - right_E.X; // vector6  hand - elbow  
	//vector6.Y = right_W.Y - right_E.Y;  
	//vector6.Z = right_E.Z - right_W.Z;

	//normal3.X = vector5.Y * vector6.Z - vector5.Z * vector6.Y;  // normal3 = vector5 x vector6
	//normal3.Y = vector5.Z * vector6.X - vector5.X * vector6.Z;  
	//normal3.Z = vector5.X * vector6.Y - vector5.Y * vector6.X; 

	//normal4.X = 0.0;  
	//normal4.Y = -100.0; 
	//normal4.Z = 0.0;

	//vector7.X = right_E.X - right_S.X;  
	//vector7.Y = right_E.Y - right_S.Y;  
	//vector7.Z = 0.0;  

	//vector8.X = 0.0;
	//vector8.Y = 100;
	//vector8.Z = 0.0;

	//deltaNormal5 = sqrt(normal3.X * normal3.X + normal3.Y * normal3.Y + normal3.Z * normal3.Z);  
	//deltaNormal6 = sqrt(normal4.X * normal4.X + normal4.Y * normal4.Y + normal4.Z * normal4.Z); 
	//deltaNormal7 = sqrt(vector7.X * vector7.X + vector7.Y * vector7.Y + vector7.Z * vector7.Z);  
	//deltaNormal8 = sqrt(vector8.X * vector8.X + vector8.Y * vector8.Y + vector8.Z * vector8.Z);  

	//if (deltaNormal5*deltaNormal6 >0.0)
	//{
	//	cosAngle3 = (normal3.X * normal4.X + normal3.Y * normal4.Y + normal3.Z * normal4.Z) / (deltaNormal5 * deltaNormal6);

	//	right_elevation = acos(cosAngle3) *180 /M_PI;

	//}
	//if (deltaNormal7 * deltaNormal8 >0.0)
	//{
	//	cosAngle4 = (vector7.X * vector8.X + vector7.Y * vector8.Y + vector7.Z * vector8.Z) / (deltaNormal7 * deltaNormal8);
	//	right_azimuth = acos(cosAngle4)*180/M_PI;

	//}
	////to calculate joint angle
	//deltaVector3 = sqrt(vector5.X * vector5.X + vector5.Y * vector5.Y + vector5.Z * vector5.Z);  
	//deltaVector4 = sqrt(vector6.X * vector6.X + vector6.Y * vector6.Y + vector6.Z * vector6.Z);

	//if (deltaVector3 * deltaVector4 > 0.0)  
	//{  
	//	cosAngle6 = (vector5.X * vector6.X + vector5.Y * vector6.Y + vector5.Z * vector6.Z) / (deltaVector3 * deltaVector4);  
	//	int jointangle2 = acos(cosAngle6)*180/M_PI;

	//}

//////////////////////////////////////////////////////////////////////DISI 11.5

	double motion4 = 0;
	if ((cosAngle7 >= (-0.55)) && (cosAngle7 <= (0.55)) && (cosAngle8 >= (-0.55)) && (cosAngle8 <= (0.55)) 
		&& (cosAngle1>=(-1)) && (cosAngle1<=(-0.8)) && (cosAngle2>=(-1)) && (cosAngle2<=(-0.8)))
	{
		motion4 = 1;
		//cout << "**********************extended arm ********************" << endl;

	}

//////////////////////////////////////////////////////////////////////DISI 11.5

	//float *angles;
	angles[0] = left_elevation;
	angles[1] = left_azimuth;
	angles[2] = right_elevation;
	angles[3] = right_azimuth;

	return motion4;

}
