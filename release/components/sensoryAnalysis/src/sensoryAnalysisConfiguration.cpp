/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  David Vernon, University of Skövde,  Yinfeng fang, University of Portsmouth, Haibin Cai, University of Portsmouth
 * Email:   david.vernon@his.se, yinfeng.fang@port.ac.uk, haibin.cai@port.ac.uk
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

#include "sensoryAnalysis.h"


#ifdef LOAD_IMAGE 
extern DreamSave DreamPort;
#endif
extern bool main_loop_flag;

bool SensoryAnalysis::configure(yarp::os::ResourceFinder &rf)
{    
	/* Process all parameters from both command-line and .ini file */

	/* get the module name which will form the stem of all module port names */

	moduleName            = rf.check("name", 
							Value("sensoryAnalysis"), 
							"module name (string)").asString();

	/*
	* before continuing, set the module name before getting any other parameters, 
	* specifically the port names which are dependent on the module name
	*/
   
	setName(moduleName.c_str());

	/* get the name of the input and output ports, automatically prefixing the module name by using getName() */
   
	checkMutualGazeOutputPortName         = "/";
	checkMutualGazeOutputPortName        += getName(rf.check("checkMutualGazeOutputPort", Value("/checkMutualGaze:o"), "checkMutualGaze Output Port (string)").asString());
  
	getArmAngleOutputPortName             = "/";
	getArmAngleOutputPortName            += getName(rf.check("getArmAngleOutputPort", Value("/getArmAngle:o"), "getArmAngle Output Port (string)").asString());

	getBodyOutputPortName                 = "/";
	getBodyOutputPortName                += getName(rf.check("getBodyOutputPort", Value("/getBody:o"), "getBody Output Port (string)").asString());

	getBodyPoseOutputPortName             = "/";
	getBodyPoseOutputPortName            += getName(rf.check("getBodyPoseOutputPort", Value("/getBodyPose:o"), "getBodyPose Output Port port (string)").asString());

	getEyeGazeInputPortName               = "/";
	getEyeGazeInputPortName              += getName(rf.check("getEyeGazeInputPort", Value("/getEyeGaze:i"), "getEyeGaze Input Port (string)").asString());
    
	getEyeGazeOutputPortName              = "/";
	getEyeGazeOutputPortName             += getName(rf.check("getEyeGazeOutputPort", Value("/getEyeGaze:o"), "getEyeGaze Output Port (string)").asString());

	getEyesOutputPortName                 = "/";
	getEyesOutputPortName                += getName(rf.check("getEyesOutputPort", Value("/getEyes:o"), "getEyes Output Port (string)").asString());
    
	getFacesOutputPortName                = "/";
	getFacesOutputPortName               += getName(rf.check("getFacesOutputPort", Value("/getFaces:o"), "getFaces Output Port (string)").asString());

	getGripLocationInputPortName          = "/";
	getGripLocationInputPortName         += getName(rf.check("getGripLocationInputPort", Value("/getGripLocation:i"), "getGripLocation Input Port (string)").asString());

	getGripLocationOutputPortName         = "/";
	getGripLocationOutputPortName        += getName(rf.check("getGripLocationOutputPort", Value("/getGripLocation:o"), "getGripLocation Output Port (string)").asString());

	getHandsOutputPortName                = "/";
	getHandsOutputPortName               += getName(rf.check("getHandsOutputPort", Value("/getHands:o"), "getHands Output Port (string)").asString());

	getHeadOutputPortName                 = "/";
	getHeadOutputPortName                += getName(rf.check("getHeadOutputPort", Value("/getHead:o"), "getHead Output Port (string)").asString());

	getHeadGazeInputPortName              = "/";
	getHeadGazeInputPortName             += getName(rf.check("getHeadGazeInputPort", Value("/getHeadGaze:i"), "getHeadGaze Input Port (string)").asString());

	getHeadGazeOutputPortName             = "/";
	getHeadGazeOutputPortName            += getName(rf.check("getHeadGazeOutputPort", Value("/getHeadGaze:o"), "getHeadGaze Output Port (string)").asString());

	getObjectsOutputPortName              = "/";
	getObjectsOutputPortName             += getName(rf.check("getObjectsOutputPortName", Value("/getObjects:o"), "getObjects Output Port (string)").asString());

	getObjectsInputPortName               = "/";
	getObjectsInputPortName              += getName(rf.check("getObjectsInputPort", Value("/getObjects:i"), "getObjects Input Port (string)").asString());

	getObjectTableDistanceInputPortName   = "/";
	getObjectTableDistanceInputPortName  += getName(rf.check("getObjectTableDistanceInputPort", Value("/getObjectTableDistance:i"), "getObjectTableDistance Input Port (string)").asString());

	getObjectTableDistanceOutputPortName  = "/";
	getObjectTableDistanceOutputPortName += getName(rf.check("getObjectTableDistanceOutputPort", Value("/getObjectTableDistance:o"), "getObjectTableDistance Output Port (string)").asString());

	getSoundDirectionInputPortName        = "/";
	getSoundDirectionInputPortName       += getName(rf.check("getSoundDirectionInputPort", Value("/getSoundDirection:i"), "getSoundDirection Input Port (string)").asString());

	getSoundDirectionOutputPortName       = "/";
	getSoundDirectionOutputPortName      += getName(rf.check("getSoundDirectionOutputPort", Value("/getSoundDirection:o"), "getSoundDirection Output Port (string)").asString());

	identifyFaceInputPortName             = "/";
	identifyFaceInputPortName            += getName(rf.check("identifyFaceInputPort", Value("/identifyFace:i"), "identifyFace Input Port (string)").asString());
 
	identifyFaceOutputPortName            = "/";
	identifyFaceOutputPortName           += getName(rf.check("identifyFaceOutputPort", Value("/identifyFace:o"), "identifyFace Output Port (string)").asString());

	identifyFaceExpressionInputPortName   = "/";
	identifyFaceExpressionInputPortName  += getName(rf.check("identifyFaceExpressionInputPort", Value("/identifyFaceExpression:i"), "identifyFaceExpression Input Port (string)").asString());

	identifyFaceExpressionOutputPortName  = "/";
	identifyFaceExpressionOutputPortName += getName(rf.check("identifyFaceExpressionOutputPort", Value("/identifyFaceExpression:o"), "identifyFaceExpression Output Port (string)").asString());

	identifyObjectInputPortName           = "/";
	identifyObjectInputPortName          += getName(rf.check("identifyObjectInputPort", Value("/identifyObject:i"), "identifyObject Input Port (string)").asString());
 
	identifyObjectOutputPortName          = "/";
	identifyObjectOutputPortName         += getName(rf.check("identifyObjectOutputPort", Value("/identifyObject:o"), "identifyObject Output Port (string)").asString());
 
	identifyTrajectoryInputPortName       = "/";
	identifyTrajectoryInputPortName      += getName(rf.check("identifyTrajectoryInputPort", Value("/identifyTrajectory:i"), "identifyTrajectory Input Port (string)").asString());
   
	identifyTrajectoryOutputPortName      = "/";
	identifyTrajectoryOutputPortName     += getName(rf.check("identifyTrajectoryOutputPortName", Value("/identifyTrajectory:o"), "identifyTrajectory Output Port (string)").asString());
  
	identifyVoiceOutputPortName           = "/";
	identifyVoiceOutputPortName          += getName(rf.check("identifyVoiceOutputPort", Value("/identifyVoice:o"), "identifyVoice Output Port (string)").asString());
   
	recognizeSpeechOutputPortName         = "/";
	recognizeSpeechOutputPortName        += getName(rf.check("recognizeSpeechOutputPortName", Value("/recognizeSpeech:o"), "recognizeSpeech Output Port port (string)").asString());
   
	trackFaceInputPortName                = "/";
	trackFaceInputPortName               += getName(rf.check("trackFaceInputPort", Value("/trackFace:i"), "trackFace Input Port (string)").asString());
   
	trackFaceOutputPortName               = "/";
	trackFaceOutputPortName              += getName(rf.check("trackFaceOutputPort", Value("/trackFace:o"), "trackFace Output Port (string)").asString());
      
	trackHandInputPortName                = "/";
	trackHandInputPortName               += getName(rf.check("trackHandInputPort", Value("/trackHand:i"), "trackHand Input Port (string)").asString());
   
	trackHandOutputPortName               = "/";
	trackHandOutputPortName              += getName(rf.check("trackHandOutputPort", Value("/trackHand:o"), "trackHand Output Port (string)").asString());
   
	trackObjectInputPortName              = "/";
	trackObjectInputPortName             += getName(rf.check("trackObjectInputPort", Value("/trackObject:i"), "trackObject Input Port (string)").asString());
   
	trackObjectOutputPortName             = "/";
	trackObjectOutputPortName            += getName(rf.check("trackObjectOutputPort", Value("/trackObject:o"), "trackObject Output Port (string)").asString());

     
   /* open ports  */ 
   if (!checkMutualGazeOut.open(checkMutualGazeOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << checkMutualGazeOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getArmAngleOut.open(getArmAngleOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getArmAngleOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getBodyOut.open(getBodyOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getBodyOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!getBodyPoseOut.open(getBodyPoseOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getBodyPoseOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getEyeGazeIn.open(getEyeGazeInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getEyeGazeInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getEyeGazeOut.open(getEyeGazeOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getEyeGazeOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getEyesOut.open(getEyesOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getEyesOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getFacesOut.open(getFacesOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getFacesOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getGripLocationIn.open(getGripLocationInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getGripLocationInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getGripLocationOut.open(getGripLocationOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getGripLocationOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHandsOut.open(getHandsOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHandsOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHeadOut.open(getHeadOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHeadOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHeadGazeIn.open(getHeadGazeInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHeadGazeInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHeadGazeOut.open(getHeadGazeOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHeadGazeOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getObjectsIn.open(getObjectsInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectsOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getObjectsOut.open(getObjectsOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectsOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getObjectTableDistanceIn.open(getObjectTableDistanceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectTableDistanceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!getObjectTableDistanceOut.open(getObjectTableDistanceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectTableDistanceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getSoundDirectionIn.open(getSoundDirectionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSoundDirectionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getSoundDirectionOut.open(getSoundDirectionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSoundDirectionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyFaceIn.open(identifyFaceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyFaceOut.open(identifyFaceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyFaceExpressionIn.open(identifyFaceExpressionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceExpressionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!identifyFaceExpressionOut.open(identifyFaceExpressionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceExpressionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyObjectIn.open(identifyObjectInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyObjectInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyObjectOut.open(identifyObjectOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyObjectOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyTrajectoryIn.open(identifyTrajectoryInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyTrajectoryInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyTrajectoryOut.open(identifyTrajectoryOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyTrajectoryOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyVoiceOut.open(identifyVoiceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyVoiceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!recognizeSpeechOut.open(recognizeSpeechOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << recognizeSpeechOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!trackFaceIn.open(trackFaceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackFaceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!trackFaceOut.open(trackFaceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackFaceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!trackHandIn.open(trackHandInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackHandInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!trackHandOut.open(trackHandOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackHandOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!trackObjectIn.open(trackObjectInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackObjectInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!trackObjectOut.open(trackObjectOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackObjectOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }



   handlerPortName =  "/";
   handlerPortName += getName();         // use getName() rather than a literal 
 
   if (!handlerPort.open(handlerPortName.c_str())) {           
      cout << getName() << ": Unable to open port " << handlerPortName << endl;  
      return false;
   }

   attach(handlerPort);                  // attach to port



   /* create the thread and pass pointers to the module parameters */

   sensoryAnalysisThread = new SensoryAnalysisThread(
																&checkMutualGazeOut,  
																&getArmAngleOut,
																&getBodyOut,
																&getBodyPoseOut,
																&getEyeGazeIn,
																&getEyeGazeOut, 
																&getEyesOut,
																&getFacesOut,
																&getGripLocationIn, 
																&getGripLocationOut,
																&getHandsOut,
																&getHeadOut,
																&getHeadGazeIn, 
																&getHeadGazeOut, 
																&getObjectsIn,
																&getObjectsOut,
																&getObjectTableDistanceIn,
																&getObjectTableDistanceOut,
																&getSoundDirectionIn,
																&getSoundDirectionOut,  
																&identifyFaceIn,
																&identifyFaceOut,
																&identifyFaceExpressionIn,  
																&identifyFaceExpressionOut,  
																&identifyObjectIn,
																&identifyObjectOut,
																&identifyTrajectoryIn, 
																&identifyTrajectoryOut,  
																&identifyVoiceOut,
																&recognizeSpeechOut, 
																&trackFaceIn,
																&trackFaceOut, 
																&trackHandIn,
																&trackHandOut,  
																&trackObjectIn,
																&trackObjectOut
																);


   /* now start the thread to do the work */

   if (!configureDreamSave(rf)) {
	   return false;
   }

   sensoryAnalysisThread->start(); // this calls threadInit() and it if returns true, it then calls run()

   return true ;      // let the RFModule know everything went well
                      // so that it will then run the module
}

/** Configure DreamSave */
bool SensoryAnalysis::configureDreamSave(yarp::os::ResourceFinder &rf) {
	
	#ifdef LOAD_IMAGE //to debug when the DREAM Save is off/on
	string savingPathName = rf.check("recordingPath", Value("C:\\DreamRecordings\\"), "Target folder for recorded sensor data (string)").asString();
  
	string ConfigFilePath = "C://DREAM/release//components//sensoryAnalysis//config//";
	string faceDetectionModelName = ConfigFilePath+"haarcascade_frontalface_alt2.xml";
	string facialLandmarkDetectModelName = ConfigFilePath+"DetectionModel-v1.5.bin";
	string facialLandmarkTrackingModelName = ConfigFilePath+"TrackingModel-v1.10.bin";
	string facialExpressionClassifierName = ConfigFilePath+"ExpressionClassifier.xml";
	string faceIDClassifierName = ConfigFilePath+"FaceIDClassifier.xml";
	string CameraLKMatDMatName = ConfigFilePath+"CameraLKMatDMat.xml";
	string CameraLRMatTMatName = ConfigFilePath+"CameraLRMatTMat.xml";
	string CameraMKMatDMatName = ConfigFilePath+"CameraMKMatDMat.xml";
	string CameraMRMatTMatName = ConfigFilePath+"CameraMRMatTMat.xml";
	string CameraRKMatDMatName = ConfigFilePath+"CameraRKMatDMat.xml";
	string CameraRRMatTMatName = ConfigFilePath+"CameraRRMatTMat.xml";
	string Kinect0KMatDMatName = ConfigFilePath+"Kinect0color_KMatDMat.xml";
	string Kinect1KMatDMatName = ConfigFilePath+"Kinect1KMatDMat.xml";
	string Kinect1RMatTMatName = ConfigFilePath+"Kinect1RMatTMat.xml";
	string ObjectROIFileName = ConfigFilePath+"ObjectFile.xml";
	string ObjectTemplateName = ConfigFilePath+"ObjectTemplate.png";
	string ObjectClassifierName = ConfigFilePath+"ObjectClassifier.xml";
	string SpeechBasciPathName = ConfigFilePath+"Speechconfig.grxml";
	string SkmotionClassifierName = ConfigFilePath+"DreamAdult_train_finallable.model";
	string FaceModelName = ConfigFilePath+"Landmark13_3D.xml";

	if (savingPathName.back() != '\\' && savingPathName.back() != '/') savingPathName.push_back('\\');

	cout << "Recording sensor data to " << savingPathName << endl;

	DWORD ftyp = GetFileAttributes(savingPathName.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES || !(ftyp & FILE_ATTRIBUTE_DIRECTORY)) {
		cout << "Warning! Recording path invalid!" << endl;
		return false;
	}
	
	if(!DreamPort.LoadConfigFiles(savingPathName, 1280, 960,
		faceDetectionModelName,facialLandmarkDetectModelName,
		facialLandmarkTrackingModelName,facialExpressionClassifierName,
		faceIDClassifierName,CameraLKMatDMatName,CameraLRMatTMatName,
		CameraMKMatDMatName,CameraMRMatTMatName,CameraRKMatDMatName,
		CameraRRMatTMatName,Kinect0KMatDMatName,Kinect1KMatDMatName,
		Kinect1RMatTMatName,ObjectROIFileName,ObjectTemplateName,
		ObjectClassifierName,SpeechBasciPathName,SkmotionClassifierName, FaceModelName))
	{
		cout << "Sensory Analysis GUI Initial Error !" <<endl;
		return false;
	}

	#endif
	return true;
}


bool SensoryAnalysis::interruptModule()
{
   /* NB you must stop the thread before interrupting the ports and then closing them
      If you don't then gyarpmanger will not be able to stop the module and you will have
      to resort to killing it                                                              */
  
   // to ensure a saft exit
   main_loop_flag = false;
   cout<<"SensoryAnalysis::interruptModule () is called!"<<endl;

#ifdef LOAD_IMAGE //to debug when the DREAM Save is off/on
   DreamPort.Stop();
#endif

   sensoryAnalysisThread->stop();
    cout<<"SensoryAnalysis::interruptModule () is test 1!"<<endl; 
   checkMutualGazeOut.interrupt();
   getArmAngleOut.interrupt();
   getBodyOut.interrupt();
   getBodyPoseOut.interrupt(); 
   getEyeGazeIn.interrupt();
   cout<<"SensoryAnalysis::interruptModule () is test 1.5!"<<endl; 
   getEyeGazeOut.interrupt(); 
   getEyesOut.interrupt();
   getFacesOut.interrupt(); 
   getGripLocationIn.interrupt(); 
   getGripLocationOut.interrupt(); 
   getHandsOut.interrupt();
   getHeadOut.interrupt();
    cout<<"SensoryAnalysis::interruptModule () is test 2!"<<endl; 
   getHeadGazeIn.interrupt(); 
   getHeadGazeOut.interrupt(); 
   getObjectsIn.interrupt();
   getObjectsOut.interrupt(); 
    cout<<"SensoryAnalysis::interruptModule () is test 3!"<<endl; 
   getObjectTableDistanceIn.interrupt(); 
   getObjectTableDistanceOut.interrupt(); 
   getSoundDirectionIn.interrupt();
   getSoundDirectionOut.interrupt();  
   identifyFaceIn.interrupt();
   identifyFaceOut.interrupt();  
 cout<<"SensoryAnalysis::interruptModule () is test 4!"<<endl; 
   identifyFaceExpressionIn.interrupt();  
   identifyFaceExpressionOut.interrupt();  
   identifyObjectIn.interrupt();
   identifyObjectOut.interrupt(); 
   identifyTrajectoryIn.interrupt();  
   identifyTrajectoryOut.interrupt();  
   identifyVoiceOut.interrupt();
    cout<<"SensoryAnalysis::interruptModule () is test 5!"<<endl; 
   recognizeSpeechOut.interrupt(); 
   trackFaceIn.interrupt();
   trackFaceOut.interrupt();  
   trackHandIn.interrupt();
   trackHandOut.interrupt();  
   trackObjectIn.interrupt();  
   trackObjectOut.interrupt(); 


   handlerPort.interrupt();
  cout<<"SensoryAnalysis::interruptModule () is called!"<<endl; 

   return true;
}


bool SensoryAnalysis::close()
{
	cout<<"SensoryAnalysis::close () is called!"<<endl;
	// to ensure a saft exit
   main_loop_flag = false;

#ifdef LOAD_IMAGE //to debug when the DREAM Save is off/on
   DreamPort.Stop();
#endif

   checkMutualGazeOut.close();
   getArmAngleOut.close();
   getBodyOut.close();
   getBodyPoseOut.close(); 
   getEyeGazeIn.close();
   getEyeGazeOut.close(); 
   getEyesOut.close();
   getFacesOut.close(); 
   getGripLocationIn.close(); 
   getGripLocationOut.close(); 
   getHandsOut.close();
   getHeadOut.close();
   getHeadGazeIn.close(); 
   getHeadGazeOut.close(); 
   getObjectsIn.close();
   getObjectsOut.close(); 
   getObjectTableDistanceIn.close(); 
   getObjectTableDistanceOut.close(); 
   getSoundDirectionIn.close();
   getSoundDirectionOut.close();  
   identifyFaceIn.close();
   identifyFaceOut.close();  
   identifyFaceExpressionIn.close();  
   identifyFaceExpressionOut.close();  
   identifyObjectIn.close();
   identifyObjectOut.close(); 
   identifyTrajectoryIn.close();  
   identifyTrajectoryOut.close();  
   identifyVoiceOut.close();
   recognizeSpeechOut.close(); 
   trackFaceIn.close();
   trackFaceOut.close();  
   trackHandIn.close();
   trackHandOut.close();  
   trackObjectIn.close();  
   trackObjectOut.close(); 

   handlerPort.close();
cout<<"SensoryAnalysis::close () is end!"<<endl;
   return true;
}


bool SensoryAnalysis::respond(const Bottle& command, Bottle& reply) 
{
    return true;
}


/* Called periodically every getPeriod() seconds */
bool SensoryAnalysis::updateModule()
{
   return true;
}


double SensoryAnalysis::getPeriod()
{
   return 0.1;
}
