/*
20170107
	create for dream motion recognition 
*/
#ifndef _MOTION_DREAM
#define _MOTION_DREAM

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include "MotionFeature.h"

//svm
#include "svm.h"

//live kinect 
#include <windows.h>
#include "NuiApi.h"

using namespace cv;
using namespace std;

class DreamMotion
{
public:
	DreamMotion();
	~DreamMotion();

	//for motion recognition
	int windowsize;
	int DesireFrameN; // the desire frame number of spine function
	int JointsN; // the number of skeleton joints
	int BinN; //number of bins for H feature, this number is can not be changed

	vector<Mat> MatSkeletonAction; //save all window size skeleton
	vector<Mat> Mat_skeletonnew; //update every frame
	vector<bool> newskeletonflag; //flag to show the validation of a new frame
	vector<int> motionframecount; //count to reach windowsize
	vector<bool> motionflag;
	MotionFeature *feature;

	//for svm
	struct svm_node *x;
	int max_nr_attr;
	struct svm_model* model;
	int predict_probability;

	bool loadmodel(string ModelName);
	void motionfun(NUI_SKELETON_FRAME &kinectskeleton, vector<int> & output, bool motion_restart_flag = false);
	int predict_svm(Mat &NormalizedMatFeature);
};

#endif