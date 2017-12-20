#ifndef _FACE_ANALYSIS_H_
#define _FACE_ANALYSIS_H_
#include "opencv2/core/core.hpp"
#include <opencv/ml.h>
#include<vector>
#include <string>
using namespace cv;
using namespace std;

#define FR 0
#define FER1 1
#define FER2 2
#define block_x 8
#define block_y 8

class FaceAnalysis
{
public:
	//bool FaceAnalysis::compareRect(cv::Rect r1, cv::Rect r2);
	void read_csv(const string& filename, vector<int>& labels, char separator);
	template <typename _Tp> inline
		void elbp_(InputArray _src, OutputArray _dst, int radius, int neighbors);
	void elbp(InputArray src, OutputArray dst, int radius, int neighbors);
	Mat elbp(InputArray src, int radius, int neighbors);
	Mat histc(InputArray src);
	Mat spatial_histogram(Mat src, int numPatterns, int grid_x, int grid_y);
	Mat FaceDescriptor(InputArray src, bool normalization);
	Mat Landmark(InputArray src);
	Mat Hist_Lms(Mat hist, Mat lms);
	void GetFace(string path);
	void trainingdata(int rec_type);
	int FaceRecTrain(bool saveTrain);
	int FacialExTrain(int rec_type, bool saveTrain);
	int FaceTrainNB(int rec_type);
	bool LoadFaceConfig(string idconfig, string exconfig);
	void FaceDisplay();
	int FacePredict(Mat face, Mat landmark, int &identity, int &emotion);
	int FacePredict(Mat face, int &identity, int &emotion);
	Mat CrossValidation(int folders);
public:
	Mat trainSample;
	Mat label;
	CvSVM id;
	CvSVM ex;
};
#endif