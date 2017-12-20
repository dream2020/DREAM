/*
20161216
	create motion feature class, without spline (desire frame number)
*/
#ifndef _MOTION_FEATURE
#define _MOTION_FEATURE

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

class MotionFeature
{
public:
	MotionFeature(int JointsN, int DesireFrameN);
	~MotionFeature();

	const int JointsN;
	const int DesireFrameN; //equal to window size-1

	void getindex(int &firstind, double &firstcos, int &secondindex, double &secondcos, double dx, double dy, double dz, double V[26][3]);
	double Extract_Hfeature(Mat &SkeletonFrame, vector<vector<double> > &histog, int nrows);
	void Extract_Gfeature(Mat &SkeletonFrame,int nrows, vector< vector<double> > &Gfeature,int recols);
	void ExtractFeature(Mat &Mat_skeletoni, Mat &FeatureMat);

	int BinN;
};

#endif

