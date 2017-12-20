#ifndef _EyeLOCALIZATION_
#define _EyeLOCALIZATION_

#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;
using namespace std;

// make sure that the eye length is bigger than 15
class EyeLOCALIZATION
{
public: //Members
	EyeLOCALIZATION();
    ~EyeLOCALIZATION();
    /********************kernel****************/
	int nkernel; //number of kernel used for eye center detection
	std::vector<cv::Mat> kernel_filter; //kernel used for eye center detection
	std::vector<cv::Mat> totalkernel_filter; //kernel used for eye center detection

	Rect sdmlefteye, sdmrighteye;

public: //Methods
    //read background eye center data
	void buildkernel(); //find the largest face

	/************using sdm eye region to locate eye center*****************/
	//using convolution and binary image to locate one eye center
	int con_binaryimg(Mat &gray, Mat& blackgray, Point& center, double pyramid_size);
	//return the two eye center in the sdm eye region
	bool Locate(Mat &Gray_C, Mat &Blackgray_C, Rect &Rec_C, Point &Eye_C, int &Radius_C, double pyramid_size);
	void Region_EyeLocate(const Mat_<float> X_C, Mat &gray, Point &EyeR_C, int &RadiusR_C, Point &EyeL_C, int &RadiusL_C, double pyramid_size);
};
#endif
