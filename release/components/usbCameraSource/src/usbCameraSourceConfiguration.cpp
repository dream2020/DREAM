 /*
  * Copyright (C) 2014 DREAM Consortium
  * FP7 Project 611391 co-funded by the European Commission
  *
  * Author:  Yinfeng Fang and Honghai Liu, University of Portsmouth
  * Email:   yinfeng.fang@port.ac.uk and honghai.liu@port.ac.uk
  * Website: www.dream2020.eu 
  * 
  * This file is part of DREAM.
  * 
  * DREAM is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  * 
  * DREAM is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  * 
  * You should have received a copy of the GNU General Public License
  * along with DREAM.  If not, see <http://www.gnu.org/licenses/>.
  */
 

 
#include "UsbCameraSource.h"


UsbCameraSource::UsbCameraSource(){


}
UsbCameraSource::~UsbCameraSource(){

}

bool UsbCameraSource::configure(yarp::os::ResourceFinder &rf)
{    

   debug = true;
   
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("usbCameraSource"), 
                           "module name (string)").asString();

   cout << "MODULE NAME: " << moduleName << endl;

   /*
    * before continuing, set the module name before getting any other parameters, 
    * specifically the port names which are dependent on the module name
    */
   
   setName(moduleName.c_str());



   /* now, get the rest of the parameters */


   outputPortNameCamera   = rf.check("outputPort", 
								Value("/cam:o"),
								"Output image port (string)").asString();
   outputPortNameCamera   = "/" + moduleName + outputPortNameCamera;

   /* get the frequency, width, height, and camera ID */

   frequency             = rf.check("frequency",
                           Value(10),		//the default value, if no indicated in *.ini file
                           "frequency key value (int)").asInt();


   width                 = rf.check("width",
                           Value(1280),//the default value, if no indicated in *.ini file
                           "output width key value (int)").asInt();


   height                = rf.check("height",
                           Value(960),//the default value, if no indicated in *.ini file
                           "output height key value (int)").asInt();


	cameraDeviceID        =  rf.check("cameraDeviceID",
                           Value(0),//the default value, if no indicated in *.ini file
                           "output height key value (int)").asInt();

	savingPathName       =  rf.check("savingPath",
                           Value("c:/videoSave/"),//the default value, if no indicated in *.ini file
                           "output savingPathName value (string)").asString();
	savingWithRatio      =  rf.check("savingWithRatio",
                           Value(1),//the default value, if no indicated in *.ini file
                           "output savingWithRatio value (int)").asDouble();
	savingHeightRatio    =  rf.check("savingHeightRatio",
                           Value(1),//the default value, if no indicated in *.ini file
                           "output savingHeightRatio value (int)").asDouble();

	if (savingWithRatio == 0 ) savingWithRatio =1;
	if (savingHeightRatio == 0 ) savingHeightRatio =1;

	//cout<<"usbCameraSource::"<<cameraDeviceID<<" "<<cameraDeviceID1<<" "<<cameraDeviceID2<<" "<<endl;

   if (debug) {
		cout << "usbCameraSource::configure: outputPortNameCamera  " <<endl<< outputPortNameCamera << endl;
		cout << "usbCameraSource::configure: cameraDeviceID              " << cameraDeviceID << endl;
		cout << "usbCameraSource::configure: width                       " << width << endl;
		cout << "usbCameraSource::configure: height                      " << height << endl;
		cout << "usbCameraSource::configure: savingPathName                      " << savingPathName << endl;
		cout << "usbCameraSource::configure: savingWithRatio                      " << savingWithRatio << endl;
		cout << "usbCameraSource::configure: savingHeightRatio                      " << savingHeightRatio << endl;
   }


   /* do all initialization here */
     
   /* open ports  */ 
       
   if (!imageOut.open(outputPortNameCamera.c_str())) {
      cout << "usbCameraSource::configure" << ": unable to open port " << outputPortNameCamera << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

	/*
	* attach a port of the same name as the module (prefixed with a /) to the module
	* so that messages received from the port are redirected to the respond method
	*/

	handlerPortName =  "/";
	handlerPortName += getName();         // use getName() rather than a literal 
 
	if (!handlerPort.open(handlerPortName.c_str())) {           
		cout << getName() << ": Unable to open port " << handlerPortName << endl;  
		return false;
	}

	attach(handlerPort);                  // attach to port
 
	cap.open(cameraDeviceID);
	usbCameraSourceThread = NULL;
	

	//check the state of camera , before create usbCameraSource thread
	if(cap.isOpened())
	{
		cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT,height);
		cout<<"Camera "<< cameraDeviceID << " opened successfully!" << endl;
 
		configureRecording();

		usbCameraSourceThread  = new UsbCameraSourceThread(&imageOut, &cap, (int)(1000 / frequency), width, height, savingWithRatio, savingHeightRatio, &outputVideo);
		/* now start the thread to do the work */
		usbCameraSourceThread->start(); // this calls threadInit() and it if returns true, it then calls run()
	}else {
		cout<<"Cannot find the camera with ID: "<<cameraDeviceID<<endl;
	}


    return true;       // let the RFModule know everything went well
                      // so that it will then run the module
}

bool UsbCameraSource::configureRecording() {
	cv::Size S0 = cv::Size(width*savingWithRatio, height*savingHeightRatio);
	//cout<<"UsbCameraSource: widthValue  "<<widthValue<<"  heightValue:"<<heightValue<<endl;
	//cv::Size S_color = cv::Size(mkinect.KinectColorWidth, mkinect.KinectColorHeight);
	int ex = CV_FOURCC('X','V','I','D') ; //Codec Type XVID---fourcc

	const std::string timebuf = currentDateTime();
	string recordingPath = savingPathName + "/" + moduleName + "_" + timebuf + ".avi";

	DWORD ftyp = GetFileAttributes(savingPathName.c_str());
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
		outputVideo.open(recordingPath, ex, 25, S0, true);
		cout << "Recording video to " << recordingPath << endl;
	} else {
		cout << "Warning! Saving path invalid. Videos will not be recorded." << endl;
		return false;
	}

	return true;
}

const std::string UsbCameraSource::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tstruct);
    return buf;
}

bool UsbCameraSource::interruptModule()
{
	return true;
}


bool UsbCameraSource::close()
{

	// Check whether the thread of usbCameraSoureceThread is created before stop it
	if(usbCameraSourceThread != NULL) 
	{	/* stop the thread */
		cout << "stopping camera thread" << endl;
		usbCameraSourceThread->stop();
		cout << "camera thread stoped" << endl;
	}

	if (outputVideo.isOpened()) {
		cout << "Closing video recording." << endl;
		outputVideo.release();
	}

	//Before stop module, release camera related space of cap.
	if(cap.isOpened())
	{	
		cap.release();		
	}

	imageOut.close();
	handlerPort.close();    

	return true;
}


bool UsbCameraSource::respond(const Bottle& command, Bottle& reply) 
{
   return true;
}


/* Called periodically every getPeriod() seconds */

bool UsbCameraSource::updateModule()
{
   return true;
}



double UsbCameraSource::getPeriod()
{
   /* module periodicity (seconds), called implicitly by imageSource */
    
   return 0.1;
}

 