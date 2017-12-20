 /*
  * Copyright (C) 2014 DREAM Consortium
  * FP7 Project 611391 co-funded by the European Commission
  *
  * Author:  Yinfeng Fang and Honghai Liu, University of Portsmouth
  * Email:   yinfeng.fang@port.ac.uk and honghai.liu@port.ac.uk
  * Website: www.dream2020.eu 
  *
  * This program comes with ABSOLUTELY NO WARRANTY.
  */

 
#include "usbCameraSource.h"

/*Computation source code for the middle camera*/
   UsbCameraSourceThread::UsbCameraSourceThread(
								 BufferedPort<ImageOf<PixelRgb>>*	imageOut, 
								 VideoCapture* cap,
								 int period, int width, int height, 
								 double  savingWithRatio, double  savingHeightRatio,
								 VideoWriter* outputVideo) : RateThread(period)
{
	debug                           = true;

	imagePortOut					= imageOut;
	theCap	                        = cap;
	widthValue						= width;
	heightValue						= height;

	savingWithRatioValue            = savingWithRatio;
	savingHeightRatioValue          = savingHeightRatio;
	theVideo                        = outputVideo;
}

UsbCameraSourceThread::~UsbCameraSourceThread(){
	//cout<<"usbCamera release"<<endl;
	//outputVideo.release();//to save the video
	//outputVideo.release();
	//outputVideo1.release();
	//outputVideo2.release();
}


bool UsbCameraSourceThread::threadInit() 
{

	/* generate a seed for the random variables */
	srand((int)(1000*yarp::os::Time::now()));

	return true;

}


void UsbCameraSourceThread::run(){

	//read a Farme from the camera, can convert it to outputImage with format conversion
	*theCap >> frame;
	Mat toSvaeframe;
	//cout<<savingWithRatioValue<<" "<<savingHeightRatioValue<<endl;
	resize(frame, toSvaeframe, Size(), savingWithRatioValue, savingHeightRatioValue);

	if (theVideo->isOpened()) {
		*theVideo << toSvaeframe;
	}

	IplImage * iplFrame;
	iplFrame = &frame.operator IplImage();
	IplImage* iplImage;

	ImageOf<PixelRgb> &outputImage = imagePortOut->prepare();
	outputImage.resize(iplFrame->width, iplFrame->height);
	iplImage = (IplImage*)outputImage.getIplImage();
	cvCopy(iplFrame, iplImage, 0);

	if (iplFrame->channelSeq[0] == 'B') {
		cvCvtColor(iplImage, iplImage, CV_BGR2RGB);
	}
	imagePortOut->write();

}


void UsbCameraSourceThread::threadRelease() 
{
	 
	//if(cap.isOpened()) 
	//{
	//	//cap.release();
	//	cout<<"cap released successfully!"<<endl;
	//} else
	//{
	//	cout<<"cap released unsuccessfully!"<<endl;
	//}
	//theCap->release();
	////outputVideo.release();
	//cout<<"released"<<endl;
}

