#ifndef _DREAM_SAVE
#define _DREAM_SAVE

#include<opencv2/opencv.hpp>
#include "ObjectDetection.h" 
#include "DreamMotion.h"
#include<iostream>
#include<windows.h>
#include <time.h>
#include "NuiApi.h"
#include "Kinectsdk.h"
#include <fstream>
#include "intraface/FaceAlignment.h"
#include "intraface/XXDescriptor.h"
#include "EyeLocalization.h"
#include <iomanip>      // std::setw
#include "ViewGLC.h"
#include "FaceAnalysis.h"

using namespace std;
using namespace cv;

class DreamSave
{
public:
	DreamSave();
	~DreamSave();
	
	/////////////////////////////////////interface functions///////////////////////////////////////////
	bool LoadConfigFiles(string savingPathName_C,double CameraWidthName_C,double CameraHeightName_C,
		string faceDetectionModelName_C,string facialLandmarkDetectModelName_C,
		string facialLandmarkTrackingModelName_C,string facialExpressionClassifierName_C,
		string faceIDClassifierName_C,string CameraLKMatDMatName_C,string CameraLRMatTMatName_C,
		string CameraMKMatDMatName_C,string CameraMRMatTMatName_C,string CameraRKMatDMatName_C,
		string CameraRRMatTMatName_C,string Kinect0KMatDMatName_C,string Kinect1KMatDMatName_C,
		string Kinect1RMatTMatName_C,string ObjectROIFile_C,string ObjectTemplate_C,
		string ObjectClassifier_C,string SpeechBasciPathName_C,string SkmotionClassifierName_C, string FaceModelName_C);

	bool Save(string &TimeCheck_C, double &FpsCheck,vector<bool> &input_valid_flag_C, vector<bool> &valid_flag_C, vector<Mat> &CamFrames_C,
		Mat &Kinect0RGB_C,Mat &Kinect1RGB_C, Mat &Kinect0Depth_C, Mat &Kinect1Depth_C,
		double &checkMutualGazeValue_C,vector<double> &getArmAngle_C,vector<double> &getBody_C,
		vector<double> &getBodyPose_C,vector<double> &getEyeGaze_C,vector<double> &getEyes_C,
		vector<double> &getFaces_C,vector<double> &getGripLocation_C,vector<double> &getHands_C,
		vector<double> &getHead_C,vector<double> &getHeadGaze_C,vector<double> &getObjects_C,
		vector<double> &getObjectTableDistance_C,vector<double> &getSoundDirection_C,vector<double> &identifyFace_C,
		vector<double> &identifyFaceExpression_C,vector<double> &identifyObject_C,vector<double> &identifyTrajectory_C,
		double &identifyVoice_C,string &recognizeSpeech_C,vector<double> &trackFace_C,
		vector<double> &trackHand_C,vector<double> &trackObject_C, vector<bool> &LandmarkValidFlag_C, vector<vector<Point> > &landmarks_C, vector<Mat_<double> > &HeadRotations_C,
		vector<Mat_<double> > &GazeRotations_C, vector<Point> &Eyes_2D, Rect &facebox_C, int &SelectID_C, vector<Rect> &ObjectRect_C, vector<double> &ObjectID_C);

	bool Preview(string &TimeCheck_C,double &FpsCheck,vector<bool> &input_valid_flag_C,vector<bool> &valid_flag_C, vector<Mat> &CamFrames_C,
		Mat &Kinect0RGB_C,Mat &Kinect1RGB_C, Mat &Kinect0Depth_C, Mat &Kinect1Depth_C,
		double &checkMutualGazeValue_C,vector<double> &getArmAngle_C,vector<double> &getBody_C,
		vector<double> &getBodyPose_C,vector<double> &getEyeGaze_C,vector<double> &getEyes_C,
		vector<double> &getFaces_C,vector<double> &getGripLocation_C,vector<double> &getHands_C,
		vector<double> &getHead_C,vector<double> &getHeadGaze_C,vector<double> &getObjects_C,
		vector<double> &getObjectTableDistance_C,vector<double> &getSoundDirection_C,vector<double> &identifyFace_C,
		vector<double> &identifyFaceExpression_C,vector<double> &identifyObject_C,vector<double> &identifyTrajectory_C,
		double &identifyVoice_C,string &recognizeSpeech_C,vector<double> &trackFace_C,
		vector<double> &trackHand_C,vector<double> &trackObject_C, vector<bool> &LandmarkValidFlag_C, vector<vector<Point> > &landmarks_C, vector<Mat_<double> > &HeadRotations_C,
		vector<Mat_<double> > &GazeRotations_C, vector<Point> &Eyes_2D, Rect &facebox_C, int &SelectID_C, vector<Rect> &ObjectRect_C, vector<double> &ObjectID_C);

	void YarpInterfaceFun(string &TimeCheck_C, double &FpsCheck, vector<bool> &input_valid_flag_C,
		vector<bool> &valid_flag_C, vector<Mat> &CamFrames_C,
		Mat &Kinect0RGB_C, Mat &Kinect1RGB_C, Mat &Kinect0Depth_C, Mat &Kinect1Depth_C,
		double &checkMutualGazeValue_C, vector<double> &getArmAngle_C, vector<double> &getBody_C,
		vector<double> &getBodyPose_C, vector<double> &getEyeGaze_C, vector<double> &getEyes_C,
		vector<double> &getFaces_C, vector<double> &getGripLocation_C, vector<double> &getHands_C,
		vector<double> &getHead_C, vector<double> &getHeadGaze_C, vector<double> &getObjects_C,
		vector<double> &getObjectTableDistance_C, vector<double> &getSoundDirection_C, vector<double> &identifyFace_C,
		vector<double> &identifyFaceExpression_C, vector<double> &identifyObject_C, vector<double> &identifyTrajectory_C,
		double &identifyVoice_C, string &recognizeSpeech_C, vector<double> &trackFace_C,
		vector<double> &trackHand_C, vector<double> &trackObject_C, vector<bool> &LandmarkValidFlag_C, vector<vector<Point> > &landmarks_C, vector<Mat_<double> > &HeadRotations_C,
		vector<Mat_<double> > &GazeRotations_C, vector<Point> &Eyes_2D, Rect &facebox_C, int &SelectID_C, vector<Rect> &ObjectRect_C, vector<double> &ObjectID_C);

	void Stop();

	///////////////////////////////////////loaded parameters/////////////////////////////////////////////////////////////////
	int CameraWidth; 
	int CameraHeight; 
	int KinectWidth; 
	int KinectHeight; 
	Mat_<double> DeskWorld_R_mat, DeskWorld_T_mat;
	vector<double> SelectedPoint_WC;
	bool ShowRobotHeadFlag;
	vector<double> RobotHeadCenter;

	vector<Mat_<double> > CamRMats, CamTMats, CamKMats, CamDMats;//0 middle, 1 left, 2 right
	Mat_<double> kinect1_R_mat, kinect0depth_R_mat;
	Mat_<double> kinect1_t_mat, kinect0depth_t_mat;
	Mat_<double> kinect0color_K_mat, kinect0depth_K_mat, kinect1color_K_mat;
	Mat_<double> kinect0color_distortion_Mat, kinect0depth_distortion_Mat, kinect1_distortion_Mat;

	bool Read_PlaneMat(std::string filepath);
	bool Read_RT(std::string filepath, Mat_<double> &R_mat, Mat_<double> &t_mat);
	bool Read_KMat(string filepath, Mat_<double> &K_mat, Mat_<double> &distortion_Mat);

	//online version
	vector<int> InCameraIDs; // input camera id from config file
	string FilePath; //path to save data
	Size fixedsize; //leave for help of judge captured image size
	bool ConnectDevices(string SpeechGrammar);

	/////////////////////////////////////////////Image data/////////////////////////////////////////////////////////
	vector<VideoCapture> Cap;  // Open input
	vector<Mat> CamFrames; //kinect data is in its own class
	vector<Mat> CamFramesGray;
	int framei; // index of the frame

	//online vesrion	
	vector<Mat> CamFramesCopy; //copied data for the safe of multi thread
	Mat K0ColorCopy;//for the display of skeleton
	Mat K1ColorCopy;//for the display of object tracking
	Mat K0DepthCopy, K1DepthCopy; //for the show of 3d data, it is flipped
	Kinectsdk *mkinect; //kinect devices

	//////////////////////////////////////////////facial alignment////////////////////////////////////////////////
	CascadeClassifier FaceCascade;
	INTRAFACE::FaceAlignment *fa;
	INTRAFACE::XXDescriptor *xxd;
	vector<Mat> X_c, X_pre;
	vector<bool> IsDetect;//flag to perform face detection
	vector<float> SrcScore;
	float notFace;
	int DetectFaceSize;

	//////////////////////////////////////////////gaze estimation///////////////////////////////////////////////////////////
	double pyramid_size; //eye localizaiton
	EyeLOCALIZATION *Eye_locate;

	int SelectCameraID;
	vector<bool> gazeflag; //gaze flag for middle camera 0, left camera 1, right camera 2.
	vector<double> gazevector;
	vector<double> headvector;
	double GazeRadius;
	vector<double> GazeStartPoint; //we use the point one the top of nose
	vector<double> EyeLeft_3D;
	vector<double> EyeRight_3D;

	vector<Mat_<double> > HeadRots, GazeRots, HeadT; //rotation matrix
	vector<double> GazeRoll, GazeYaw, GazePitch; //Gaze angel
	vector<double> HeadRoll, HeadYaw, HeadPitch;//Head pose angel

	bool FaceAlignmentFun(int ID_C, int facesize, Point &center_l, Point &center_r);
	void computeRotate(Mat_<double> &rot, double roll, double yaw, double pitch);
	void camera_color2world(const int &x, const int &y, Mat &Redepth_C, const Mat_<double> &Camera_K_mat, const Mat_<double> &Camera_R_mat, const Mat_<double> &Camera_t_mat, vector<double> &Gaze_start);
	void Camera_transform_gaze(double roll, double yaw, double pitch, Mat &CamR, vector<double> &gazevec);
	bool DetectMutualGaze(double &distancegaze, vector<double> &gazestartv, vector<double> &gazev);
	void ProjectK0Depth(const Mat &depthimg, Mat &Redepth_C, const Mat &camera_color, const Mat_<double> &K0_K_mat, const Mat_<double> &Camera_K_mat, const Mat_<double> &Camera_R_mat, const Mat_<double> &Camera_t_mat);
	void getheadgaze(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, vector<double> &gazevec, vector<double> &headvec, double &x, double &y, double &z);
	void getEyeGaze(const Mat_<float> &X0, const Point& center_r, const Point& center_l, double& gazeyawtest, double& gazepitchtest);

	//CamMutualGaze
	bool UseCamMutualGazeFlag; //use camera gaze or not
	bool CamMutualGazeFlag; // result from camera gaze
	double MutualGazeMaxX, MutualGazeMinX, MutualGazeMaxY, MutualGazeMinY;
	Mat_<double> Landmark3D;
	void HeadPoseFun(const Mat_<float> &X_C, const Mat_<double> CamKMat_C, vector<double> &HeadAngel_C, Mat_<double> &HeadR_C, Mat_<double> &HeadT_C);
	void CamMutualGazeFun(const Mat_<float> &X_C, const INTRAFACE::HeadPose &HP_C, const Point &EyeR_C, const Point &EyeL_C, int RLen, int LLen, Mat_<double> &GazeR_C);
	bool CamMutualGaze(Mat_<double> &R_C, Mat_<double> &T_C);
	bool ReadFaceModel(string ModelPath_C);

	//online version
	void ClearKinectDepthMat(cv::Mat& img); // fill the mat data with zeros

	////////////////////////////////////////object related/////////////////////////////////////////////////////////////////////
	DreamObject *object_detect;
	vector<Rect> ObjectRect;
	vector<double> ObjectID;
	vector<double> Object_K0_Positionx;
	vector<double> Object_K0_Positiony;
	vector<double> Object_K0_Positionz;
	void kinect1_color2world(const int &x, const int &y, const Mat &depthimg, double &X, double &Y, double &Z);

	/////////////////////////////////////////////3D viewer////////////////////////////////////////////////////////////////
	bool Dream3DViewFlag;
	Glut3DView::ViewGLC *viewer3d;
	void GL_Read_Calibration_Mat();

	///////////////////////////////////////////////face & expression recognition////////////////////////////////////////
	FaceAnalysis *DreamFace;
	void FaceRecognition(Mat frame_gray_C, Mat X0_C, Rect &facebox_C, int &identity, int &emotion);

	//////////////////////////////////Online control parameters/////////////////////////////////
	//For the protection of thread, we prevent the save flag and preview flag from being both ture
	bool SaveFlag; //a flag for the press of save button
	bool PreviewFlag; //a flag for the press of preview button
	bool SaveThreadExitFlag; //make sure the thread can exit before the object
	bool SpeechThreadControlFlag; //control the speech thread
	bool SpeechThreadExitFlag; // detect if speech is safely exited
	bool WriteFileFlag;
	bool errorstopflag; //to prevent hardware damage during recording

	bool speechflag; // speech recognition result flag
	string speechtext;
	double SoundAngle;
	double SoundAngleConfidence;

	////////////////////////////////Online Data Recording/////////////////////////////////////////////////////////////////////////
	ofstream write_depth_coordinate; //skeleton txt file in depth coordinate
	ofstream write_color_coordinate; //skeleton txt file in color coordinate
	ofstream write_frametime_audio_direction; //saving audio file
	ofstream write_speech; //saving audio file
	
	double frame_time_start; //the start time of recording
	double frame_time; // for the recording of time

	VideoWriter outputVideo0,outputVideo1,outputVideo2; // Open the output
	VideoWriter kinect0colorvideo,kinect1colorvideo;
	FILE *fdepth_k0, *fdepth_k1; //for write kinect 0 depth data
	FILE *fdepthregister_k0, *fdepthregister_k1; //for write depth data

	std::string kinect0colorname,kinect0depthname,kinect0depthrename,kinect0skeletonname,kinect0skeleton_colorco_name,kinect0_audio_name,speechname;
	std::string kinect1colorname,kinect1depthname,kinect1depthrename;
	LONG* colorCoordinates_k0; // for the registration of kinect 0 
	LONG* colorCoordinates_k1;  
	 
	string currentDateTime_C();
	string currentDateTimeMs_C();
	void WriteDepthFrame_C(FILE * fp, const Mat &kinectdepth);
	bool BuildFileWrite();
	static DWORD WINAPI StaticSpeech_Save_Thread(void *lpParamter);
	DWORD Speech_Save_Thread(); //The real save data thread

	static DWORD WINAPI StaticSaveDataControlThread(void *lpParamter);
	DWORD SaveDataControlThread(); //The real save data thread

	static DWORD WINAPI StaticCamera0_Save_Thread(void *lpParamter);
	DWORD Camera0_Save_Thread(); //The real save data thread

	static DWORD WINAPI StaticCamera1_Save_Thread(void *lpParamter);
	DWORD Camera1_Save_Thread(); //The real save data thread

	static DWORD WINAPI StaticCamera2_Save_Thread(void *lpParamter);
	DWORD Camera2_Save_Thread(); //The real save data thread

	static DWORD WINAPI StaticKinect0_Save_Thread(void *lpParamter);
	DWORD Kinect0_Save_Thread(); //The real save data thread

	static DWORD WINAPI StaticKinect1_Save_Thread(void *lpParamter);
	DWORD Kinect1_Save_Thread(); //The real save data thread
	HANDLE kinect0color_mutex, kinect0depth_mutex, kinect1color_mutex,kinect1depth_mutex, camera0_frame_mutex,camera1_frame_mutex, camera2_frame_mutex;

	///////////////////////////////////////////Online Motion recognition////////////////////////////////////////////////////////////////
	bool SkeletonReadFlag;
	vector<double> Skeleton_vector;
	vector<Scalar> SKELETON_COLORS;

	DreamMotion *Motion;
	vector<int> motion_six_person;
	int selectedmotion; // the chosen child's motion	

	/////////////////////////////////////////online drawing function///////////////////////////////////////////////////////////////
	void DrawBone_C(Mat* pImg, NUI_SKELETON_DATA* pSkel, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1, Point jointPositions[NUI_SKELETON_POSITION_COUNT], Scalar color);
	void DrawSkeleton_C(Mat* pImg, NUI_SKELETON_DATA* pSkel, Scalar color, Point jointPositions[]);

};

#endif