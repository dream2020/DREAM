/** @file usbCameraSource.h  Interface file for the usbCameraSourc camera selection DREAM component
 *
 * Version 1.2
 * 
 * 23/09/16
 *
 * \defgroup DREAM_usbCameraSource usbCameraSource
 * 
 * Interface file for the usbCameraSource DREAM component
 *
 * \section component_description Component Description
 * 
 * Read images from three cameras, and stream them to three Yarp ports
 * 
 * \section lib_sec Libraries
 *
 * - YARP.
 * - OpenCV. 
 *
 * \section parameters_sec Parameters
 *
 * <b>Command-line Parameters </b>
 *
 * All configuration file parameters can also be given as command line parameters. Please refer to the 
 * following section for details. 
 *
 * <b>Configuration File Parameters</b>
 *
 * The following key-value pairs can be specified as parameters in the configuration file 
 * (they can also be specified as command-line parameters if you so wish). 
 * The value part can be changed to suit your needs; the default values are shown below. 
 * 
 * Key								 |  Value						  | Description
 * :---								 |  :---						  | :---
 * name								 |  usbCameraSource				  | The component name in YARP
 * outputPort                  	     |  /usbCameraSource/camLeft:o    | The output port name to which the RGB image from the left camera should be streamed
 * outputPort1                	     |  /usbCameraSource/camMid:o     | The output port name to which the RGB image from the middle camera should be streamed
 * outputPort2                 	     |  /usbCameraSource/camRight:o   | The output port name to which the RGB image from the right camera should be streamed
 * cameraDeviceID	    			 |  2 						      | specifies the camera Device ID for the left camera (need to be changed after camera installation)
 * cameraDeviceID1	    			 |  1						      | specifies the camera Device ID for the middle camera (need to be changed after camera installation)
 * cameraDeviceID2	    			 |  0						      | specifies the camera Device ID for the right camera (need to be changed after camera installation)
 * frequency						 |  25							  | specifies the maximum frequency of streaming image to the YARP server. 
 * width							 |  1920						  | specifies the width of the image to be streamed.  
 * height							 |  1080						  | specifies the height of the image to be streamed.  
 *
 *
 * \section portsa_sec Ports Accessed
 * 
 * - None
 *                      
 * \section portsc_sec Ports Created
 *
 * <b>Input ports</b>
 *
 * - None 
 *       
 *
 * <b>Output ports</b>
 *
 *     /usbCameraSource/camLeft:o
 *     /usbCameraSource/camMid:o
 *     /usbCameraSource/camRight:o
 *
 * <b>Port types </b>
 *
 * The functional specification only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *
 *  BufferedPort<ImageOf<PixelRgb> > imageOut;       // output port for an image from left camera
 *  BufferedPort<ImageOf<PixelRgb> > imageOut1;      // output port for an image from middle camera
 *  BufferedPort<ImageOf<PixelRgb> > imageOut2;      // output port for an image from right camera
 *
 * \section in_files_sec Input Data Files
 *
 * - None 
 *
 * \section out_data_sec Output Data Files
 *
 * - None
 *
 * \section conf_file_sec Configuration Files
 *
 * \c usbCameraSource.ini 
 * 
 * \section example_sec Example Instantiation of the Component
 *
 * <tt>usbCameraSource --name usbCameraSource --context components/usbCameraSource/config --from usbCameraSource.ini </tt>
 *
 * \author 
 * Yinfeng Fang and Honghai Liu
 *
 * Copyright (C) 2014 DREAM Consortium
 * 
 */


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



#include <iostream>
#include <string>
#include <cstdlib>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/Time.h>

#include<windows.h>
#include <time.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;
using namespace yarp::sig::file;
using namespace cv;


class UsbCameraSourceThread : public RateThread
{
private:

   /* class variables */

	bool debug;
	Mat frame;					//  save an iamge read from a camera
	VideoCapture* theCap;		//  indicate a camera device
	VideoWriter* theVideo;

	int		widthValue;     
	int		heightValue;      
	//int		cameraDeviceIDValue;// to identify the camera id
	int      threadlabelValue; 

	double  savingWithRatioValue;
	double  savingHeightRatioValue;
	


   /* thread parameters: they are pointers so that they refer to the original variables in SensoryInterpretationCS */

   BufferedPort<ImageOf<PixelRgb> > *imagePortOut;    

   	
	
public:

   /* class methods */

   UsbCameraSourceThread(BufferedPort<ImageOf<PixelRgb>>*	imageOut, 
								 VideoCapture* cap,
								 int period, int width, int height, 
								 double  savingWithRatio, double  savingHeightRatio,
								 VideoWriter* outputVideo);

   ~UsbCameraSourceThread();
   bool threadInit();     
   void threadRelease();
   void run(); 
};



class UsbCameraSource:public RFModule
{

   /* class variables */

   bool debug; 

   /* module parameters */
   string moduleName;
   string handlerPortName;
   string outputPortNameCamera;

   VideoCapture cap;			//  indicate a camera device
   cv::VideoWriter outputVideo; //  to save the video to file

   std::string savingPathName;
   double  savingWithRatio;
   double  savingHeightRatio;

   int    frequency;
   int    width;
   int    height;
   
   int	  cameraDeviceID;


  
   /* class variables */

   BufferedPort<ImageOf<PixelRgb> > imageOut;     // output port
   Port handlerPort;                              // a port to handle messages 


   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   UsbCameraSourceThread *usbCameraSourceThread;



public:
   UsbCameraSource();
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool configureRecording();
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
   ~UsbCameraSource();
  
   const std::string currentDateTime();
};


 