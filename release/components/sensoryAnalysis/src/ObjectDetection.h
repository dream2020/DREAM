#ifndef _OBJECT_DETECTION
#define _OBJECT_DETECTION

#include <iostream> 
#include "blob.h"
#include "BlobResult.h"
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class DreamObject
{
public:
	DreamObject();
	~DreamObject();

	//Parameters during calibration
	Rect DeskRoi; //Region of interest and
	Mat SubTemplate; //background template
	bool LoadObjectConfig(std::string filepath_C,string TemplateImgName, string ObjectClassifierName);
	bool ReadObjectFile(std::string filepath);


	bool ObjectDetect(Mat &inputframe, vector<Rect> &object_rect, vector<double> &object_ID);
	double ObjectThreshold;

	/////////////////////////////////classification///////////////////////////////
	int HogWidth;
	int HogHeight;
	HOGDescriptor *hog;
	vector<float>descriptors;
	SVM ObjectClassierSVM;

};


#endif

