/*
20160118
	add one more kinect
*/
#ifndef _KINECT_SDK_CLASS
#define _KINECT_SDK_CLASS

#include <iostream>
#include <string>
#include <time.h>
#include <opencv2/opencv.hpp> 
#include <windows.h>
#include "NuiApi.h"

//speech recognition
#include "KinectSpeech.h"

using namespace std;
using namespace cv;

class Kinectsdk
{
public:
	HRESULT hr;

	int KinectColorWidth;
	int KinectColorHeight;
	int KinectDepthWidth;
	int KinectDepthHeight;
	NUI_IMAGE_RESOLUTION  KinectDepthResolution;
	NUI_IMAGE_RESOLUTION  KinectColorResolution;

	double K0_cx, K0_cy,K0_focuscx, K0_focuscy;//kinect color intrinsic parameters
	double K0_dx, K0_dy,K0_focusdx, K0_focusdy;//kinect depth intrinsic parameters

	cv::Mat kinectcolor_k0,kinectcolor_k1;
	cv::Mat depthregister_k0,depthregister_k1;
	cv::Mat kinectdepth_k0,kinectdepth_k1;

	INuiSensor *m_pNuiSensork0; //the first kinect, assume to be the middle kinect	
	HANDLE  m_pColorStreamHandle_k0;
	HANDLE  m_pDepthStreamHandle_k0;
	HANDLE  m_hNextColorFrameEvent_k0;
	HANDLE  m_hNextDepthFrameEvent_k0;
	HANDLE m_hNextSkeletonFrameEvent_k0;
	DWORD m_skeletonFlags_k0; //skeleton tracking
	NUI_SKELETON_FRAME m_skeletonFrame_k0; // Internal skeleton frame

	/////////////////second kinect///////////////////////
	INuiSensor *m_pNuiSensork1; //second kinect, assume to be the top kinect
	HANDLE  m_pColorStreamHandle_k1;
	HANDLE  m_pDepthStreamHandle_k1;
	HANDLE  m_hNextColorFrameEvent_k1;
	HANDLE  m_hNextDepthFrameEvent_k1;

	//kinect near mode
	bool m_bNearMode;

	//audio direction,saving audio,speech recognition
	KinectSpeech *kinectspeech;
	bool speech_initial_flag;
public:
	Kinectsdk();
	~Kinectsdk();

	//Create a connected Kinect 
	HRESULT CreatemyConnected(string SpeechGrammarName);
	void convetdepth2rgb(Mat &shortdepth, Mat &depthdraw);
};

#endif