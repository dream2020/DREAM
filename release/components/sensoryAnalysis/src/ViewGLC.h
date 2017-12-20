/*
This Glut3DView class is created for 3D show in Dream Project
*/

#ifndef _VIEWGL_C
#define _VIEWGL_C

#include <iostream>
#include <string>
#include <time.h>
#include <opencv2/opencv.hpp> 
#include <fstream>
#include <freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace cv;

//use namespace to protect the static this pointer
namespace Glut3DView{

	class ViewGLC
	{
	public:
		ViewGLC();
		~ViewGLC();
	
		static ViewGLC *Glut_instance;// for the encapsulation of opengl
		void setInstance(); //to initialize the static instance that is used for the object oriented wrapper calls

		//flag to control the showing
		bool KeyExitFlag;
		bool SafeStop3DFlag; //for the safe of multi stop function
		bool DrawSensorPositionFlag;
		bool Flag3Dwindow; //make sure the 3d window can only be created for once

		//////////////////////////////3D data////////////////////////
		//Pointer is used to speed up the performance
		Mat K0_depth, K1_depth; //it should be unsigned short
		Mat K0_color, K1_color;
		vector<double> GazeDirection; //copy
		vector<double> GazeStart;
		vector<double> HeadGazeDirection; //copy
		vector<double> HeadGazeStart;
		vector<double> RobotHead;
		double RobotHeadRadius;

		//intrinsic parameter
		Mat_<double> kinect0color_K,kinect1color_K,middle_camera0_K,left_camera1_K,right_camera2_K;	
		vector<vector<double> > CameraPosition_R;//Kinect0, Kinect1, Camera0, Camera1, Camera2
		vector<vector<double> > CameraPosition_T;
		vector<string> CameraName;

		void SetCalibrationParameter(vector<Mat_<double>> &K_Mat, vector<Mat_<double>> &R_Mat, vector<Mat_<double>> &T_Mat, vector<string> &Name_C);

		void AddRobotHead(vector<double>  &robot_head_C, double radius_C); //add robot Head
		void AddHeadGaze(vector<double>  &gazevector_C, vector<double>  &GazeStartPoint_C); //add gaze in the window
		void AddGaze(vector<double>  &gazevector_C, vector<double>  &GazeStartPoint_C); //add gaze in the window
		void show(Mat &K0_depth_C, Mat &K0_color_C, Mat &K1_depth_C, Mat &K1_color_C); // show the glut window
		void stop(); // destroy the glut window

		//display related, it got to be static because of FreeGLUT
		int Width;
		int Height;
		void DrawGazeVector(vector<double>  &Vector_C, vector<double>  &Start_C, vector<double>  &Color_C); //draw gaze in the window
		void DrawCameraPosition(vector<double> &t, vector<double> &R, int size, string CameraName);
		void DrawGLScene();
		void ReSizeGLScene(int Width, int Height);

		//mouse related
		int mousex,mousey;
		vector<int> mouserot;
		vector<int> mousetrans;
		void mouseMoved(int x, int y);
		void mousePress(int button, int state, int x, int y);

		//key related	
		void keyPressed(unsigned char key, int x, int y);

		//////////create wrapper since free opengl don't allow member function/////////
		static void DrawGLSceneWrapper();
		static void ReSizeGLSceneWrapper(int Width, int Height);
		static void keyPressedWrapper(unsigned char key, int x, int y);
		static void mouseMovedWrapper(int x, int y);
		static void mousePressWrapper(int button, int state, int x, int y);

	};

} // namespace
#endif