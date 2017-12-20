/** @file sensoryAnalysis.h  Interface file for sensoryAnalysis Component
 *
 * Version 3.0
 * 
 * 13/2/2017
 *
 * \defgroup DREAM_sensoryAnalysis sensoryAnalysis
 * 
 * Interface file for sensoryAnalysis DREAM compoent
 * 
 * \section component_description Component Description
 * 
 * The DREAM system architecture, described in Deliverable D3.1, 
 * that correspond to the following work-packages. 
 *
 * - WP4: Sensing and Interpretation, 
 * - WP5: Child Behaviour Analysis, and 
 * - WP6: Robot Behaviour. 
 * 
 * Initially, these three sub-systems are implemented by three placeholder components, as follows.
 * 
 * - sensoryInterpretation 
 * - childBehaviourClassification
 * - cognitiveControl
 * 
 * The current component is the implementation as a part of the sensoryInterpretation placeholder component.
 * It provides stub functionality for all the 25 sensoryAnalysis perception primitives (see D1.3 and D3.1)
 * 
 * - checkMutualGaze()  
 * - getArmAngle(left_azimuth, left_elevation, right_azimuth, right_elevation)  
 * - getBody(x, y, z) 
 * - getBodyPose(<joint_i>)  
 * - getEyeGaze(eye, x, y, z) 
 * - getEyes(eyeL_x, eyeL_y, eyeL_z, eyeR_x, eyeR_y, eyeR_z)  
 * - getFaces(<x, y, z>) 
 * - getGripLocation(object_x, object_y, object_z, grip_x, grip_y, grip_z) 
 * - getHands(<x, y, z>) 
 * - getHead(x, y, z) 
 * - getHeadGaze(x, y, z) 
 * - getHeadGaze(p_x1, p_y1, p_z1, p_x2, p_y2, p_z2, p_x3, p_y3, p_z3, x, y, z)  
 * - getObjects(<x, y, z>) 
 * - getObjects(centre_x, centre_y, centre_z, radius, <x, y, z>)  
 * - getObjectTableDistance(object_x, object_y, object_z, vertical_distance) 
 * - getSoundDirection(threshold, azimuth, elevation)  
 * - identifyFace(x, y, z, face_id)  
 * - identifyFaceExpression(x, y, z, expression_id) 
 * - identifyObject(x, y, z, object_id)  
 * - identifyTrajectory(<x, y, z, t>, trajectory_descriptor)  
 * - identifyVoice(voice_descriptor)  
 * - recognizeSpeech(text) 
 * - trackFace(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z)  
 * - trackHand(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z) 
 * - trackObject(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z) 
 *
 * \section lib_sec Libraries
 *
 * - YARP.
 *
 * \section parameters_sec Parameters
 * 
 * <b>Command-line Parameters </b> 
 * 
 * All configuration file parameters can also be given as command line parameters. Please refer to the 
 * following section for details. 
 *
 *
 * <b>Configuration File Parameters </b>
 *
 * The following key-value pairs can be specified as parameters in the configuration file 
 * (they can also be specified as command-line parameters if you so wish). 
 * The value part can be changed to suit your needs; the default values are shown below. 
 *
 * Parameters for controlling the simulation \n
 * 
 * Key | Value
 * :-- | :----
  *
 * Parameters associated with the functions being simulated (i.e. the input and output port names) \n
 *
 * Note that all these default values for port names will be prefixed by \c /sensoryAnalysis 
 * or whatever else is specifed by the \c name parameter
 * 
 * Key | Value
 * :-- | :----
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
 *     /sensoryAnalysis/getGripLocation:i
 *     /sensoryAnalysis/getHeadGaze:i
 *     /sensoryAnalysis/getObjects:i
 *     /sensoryAnalysis/getObjectTableDistance:i
 *     /sensoryAnalysis/getSoundDirection:i
 *     /sensoryAnalysis/identifyFace:i
 *     /sensoryAnalysis/identifyFaceExpression:i
 *     /sensoryAnalysis/identifyObject:i
 *     /sensoryAnalysis/identifyTrajectory:i
 *     /sensoryAnalysis/trackFace:i
 *     /sensoryAnalysis/trackHand:i
 *     /sensoryAnalysis/trackObject:i
 * 
 * <b>Output ports</b>
 *
 *     /sensoryAnalysis   (see above)
 *     /sensoryAnalysis/checkMutualGaze:o
 *     /sensoryAnalysis/getArmAngle:o
 *     /sensoryAnalysis/getBody:o
 *     /sensoryAnalysis/getBodyPose:o
 *     /sensoryAnalysis/getEyeGaze:o
 *     /sensoryAnalysis/getEyes:o
 *     /sensoryAnalysis/getFaces:o
 *     /sensoryAnalysis/getGripLocation:o
 *     /sensoryAnalysis/getHands:o
 *     /sensoryAnalysis/getHead:o
 *     /sensoryAnalysis/getHeadGaze:o
 *     /sensoryAnalysis/getObjects:o
 *     /sensoryAnalysis/getObjectTableDistance:o
 *     /sensoryAnalysis/getSoundDirection:o
 *     /sensoryAnalysis/identifyFace:o
 *     /sensoryAnalysis/identifyFaceExpression:o
 *     /sensoryAnalysis/identifyObject:o
 *     /sensoryAnalysis/identifyTrajectory:o
 *     /sensoryAnalysis/identifyVoice:o
 *     /sensoryAnalysis/recognizeSpeech:o
 *     /sensoryAnalysis/trackFace:o
 *     /sensoryAnalysis/trackHand:o
 *     /sensoryAnalysis/trackObject:o
 *
 * <b>Port types </b>
 *
 * The functional specification only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *
 *     BufferedPort<VectorOf<int>>		checkMutualGazeOut;			
 *     BufferedPort<VectorOf<double>>    getArmAngleOut;               
 *     BufferedPort<VectorOf<double>>    getBodyOut;  
 *     BufferedPort<VectorOf<double>>    getBodyPoseOut;
 *     BufferedPort<VectorOf<double>>    getEyeGazeIn;
 *     BufferedPort<VectorOf<double>>    getEyeGazeOut;	
 *     BufferedPort<VectorOf<double>>    getEyesOut;
 *     BufferedPort<VectorOf<double>>    getFacesOut; 
 *     BufferedPort<VectorOf<double>>    getGripLocationIn;
 *     BufferedPort<VectorOf<double>>    getGripLocationOut;
 *     BufferedPort<VectorOf<double>>    getHandsOut;
 *     BufferedPort<VectorOf<double>>    getHeadOut;
 *     BufferedPort<VectorOf<double>>    getHeadGazeIn;	
 *     BufferedPort<VectorOf<double>>    getHeadGazeOut;
 *     BufferedPort<VectorOf<double>>    getObjectsIn;	
 *     BufferedPort<VectorOf<double>>    getObjectsOut;	
 *     BufferedPort<VectorOf<double>>    getObjectTableDistanceIn;
 *     BufferedPort<VectorOf<double>>    getObjectTableDistanceOut; 
 *     BufferedPort<VectorOf<double>>    getSoundDirectionIn;
 *     BufferedPort<VectorOf<double>>    getSoundDirectionOut;
 *     BufferedPort<VectorOf<double>>    identifyFaceIn;
 *     BufferedPort<VectorOf<double>>    identifyFaceOut;
 *     BufferedPort<VectorOf<double>>    identifyFaceExpressionIn;	
 *     BufferedPort<VectorOf<<int>>      identifyFaceExpressionOut; 
 *     BufferedPort<VectorOf<double>>    identifyObjectIn;	
 *     BufferedPort<VectorOf<double>>    identifyObjectOut;	
 *     BufferedPort<VectorOf<int>>	     identifyTrajectoryIn;
 *     BufferedPort<VectorOf<double>>    identifyTrajectoryOut;
 *     BufferedPort<VectorOf<int>>       identifyVoiceOut;				
 *     BufferedPort<Bottle>				 recognizeSpeechOut;		
 *     BufferedPort<VectorOf<double>>    trackFaceIn;
 *     BufferedPort<VectorOf<double>>    trackFaceOut;
 *     BufferedPort<VectorOf<double>>    trackHandIn;
 *     BufferedPort<VectorOf<double>>    trackHandOut;	
 *     BufferedPort<VectorOf<double>>    trackObjectIn;	
 *     BufferedPort<VectorOf<double>>    trackObjectOut;
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
 * \c sensoryAnalysis.ini in \c $DREAM_ROOT/release/components/sensoryAnalyisisGUI/config
 *
 *
 * \section example_sec Example Instantiation of the Component
 * 
 * <tt>sensoryAnalysis --name sensoryAnalysis --context components/sensoryAnalysis/config --from sensoryAnalysis.ini </tt>
 *
 * \author 
 * 
 * David Vernon, University of Skövde, Sweden.
 * Yifneng Fang, University of Portsmouth, UK.
 * Haibin Cai, University of Portsmouth, UK.
 * Tong Cui, University of Portsmouth, UK.
 * Disi Cheng, University of Portsmouth, UK.
 * Yiming Wang, University of Portsmouth, UK.
 * 
 */

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


#ifndef LOAD_IMAGE //to debug when the DREAM Save is off/on
#define LOAD_IMAGE
#endif

#ifndef GUI_Ports //to debug when the DREAM Save is off/on
#define GUI_Ports
#endif

#include <iostream>
#include <string>



#include <vector>
#include <windows.h>
#include <time.h>
#include <iomanip>
#include <yarp/os/Time.h>
#include<opencv2/opencv.hpp>

#ifdef LOAD_IMAGE 
#include "DreamSave.h"
#endif


#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>


#ifdef GUI_Ports
#include "SensoryAnalysisShow.h"
#endif

using namespace cv;
using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;

struct MyVector3D
{
	double X;
	double Y;
	double Z;
};


class SensoryAnalysisThread : public Thread {

private:
	
	

	BufferedPort<VectorOf<int>>		*checkMutualGazePortOut; 
	BufferedPort<VectorOf<double>>  *getArmAnglePortOut;
	BufferedPort<VectorOf<double>>  *getBodyPortOut;
	BufferedPort<VectorOf<double>>  *getBodyPosePortOut;
	BufferedPort<VectorOf<double>>  *getEyeGazePortIn; 
	BufferedPort<VectorOf<double>>  *getEyeGazePortOut;
	BufferedPort<VectorOf<double>>  *getEyesPortOut; 
	BufferedPort<VectorOf<double>>  *getFacesPortOut;
	BufferedPort<VectorOf<double>>  *getGripLocationPortIn; 
	BufferedPort<VectorOf<double>>  *getGripLocationPortOut;
	BufferedPort<VectorOf<double>>  *getHandsPortOut;
	BufferedPort<VectorOf<double>>  *getHeadPortOut;
	BufferedPort<VectorOf<double>>  *getHeadGazePortIn; 
	BufferedPort<VectorOf<double>>  *getHeadGazePortOut;
	BufferedPort<VectorOf<double>>  *getObjectsPortIn; 
	BufferedPort<VectorOf<double>>  *getObjectsPortOut;
	BufferedPort<VectorOf<double>>  *getObjectTableDistancePortIn; 
	BufferedPort<VectorOf<double>>  *getObjectTableDistancePortOut;
	BufferedPort<VectorOf<double>>  *getSoundDirectionPortIn; 
	BufferedPort<VectorOf<double>>  *getSoundDirectionPortOut;
	BufferedPort<VectorOf<double>>  *identifyFacePortIn;
	BufferedPort<VectorOf<double>>  *identifyFacePortOut;
	BufferedPort<VectorOf<double>>  *identifyFaceExpressionPortIn;
	BufferedPort<VectorOf<int>>     *identifyFaceExpressionPortOut;
	BufferedPort<VectorOf<double>>  *identifyObjectPortIn;
	BufferedPort<VectorOf<double>>  *identifyObjectPortOut;
	BufferedPort<VectorOf<int>>		*identifyTrajectoryPortIn;  
	BufferedPort<VectorOf<double>>  *identifyTrajectoryPortOut;
	BufferedPort<VectorOf<int>>		*identifyVoicePortOut;
	BufferedPort<Bottle>			*recognizeSpeechPortOut; 
	BufferedPort<VectorOf<double>>  *trackFacePortIn;
	BufferedPort<VectorOf<double>>  *trackFacePortOut;
	BufferedPort<VectorOf<double>>  *trackHandPortIn;
	BufferedPort<VectorOf<double>>  *trackHandPortOut;
	BufferedPort<VectorOf<double>>  *trackObjectPortIn;  
	BufferedPort<VectorOf<double>>  *trackObjectPortOut;

public:

   SensoryAnalysisThread(
								BufferedPort<VectorOf<int>>		*checkMutualGazeOut,
								BufferedPort<VectorOf<double>>  *getArmAngleOut,
								BufferedPort<VectorOf<double>>  *getBodyOut,
								BufferedPort<VectorOf<double>>  *getBodyPoseOut, 
								BufferedPort<VectorOf<double>>  *getEyeGazeIn,
								BufferedPort<VectorOf<double>>  *getEyeGazeOut, 
								BufferedPort<VectorOf<double>>  *getEyesOut,
								BufferedPort<VectorOf<double>>  *getFacesOut, 
								BufferedPort<VectorOf<double>>  *getGripLocationIn, 
								BufferedPort<VectorOf<double>>  *getGripLocationOut,
								BufferedPort<VectorOf<double>>  *getHandsOut,
								BufferedPort<VectorOf<double>>  *getHeadOut,
								BufferedPort<VectorOf<double>>  *getHeadGazeIn, 
								BufferedPort<VectorOf<double>>  *getHeadGazeOut, 
								BufferedPort<VectorOf<double>>  *getObjectsIn,
								BufferedPort<VectorOf<double>>  *getObjectsOut, 
								BufferedPort<VectorOf<double>>  *getObjectTableDistanceIn, 
								BufferedPort<VectorOf<double>>  *getObjectTableDistanceOut, 
								BufferedPort<VectorOf<double>>  *getSoundDirectionIn,
								BufferedPort<VectorOf<double>>  *getSoundDirectionOut,
								BufferedPort<VectorOf<double>>  *identifyFaceIn,
								BufferedPort<VectorOf<double>>  *identifyFaceOut,  
								BufferedPort<VectorOf<double>>  *identifyFaceExpressionIn,  
								BufferedPort<VectorOf<int>>     *identifyFaceExpressionOut,  
								BufferedPort<VectorOf<double>>  *identifyObjectIn,
								BufferedPort<VectorOf<double>>  *identifyObjectOut, 
								BufferedPort<VectorOf<int>>		*identifyTrajectoryIn,  
								BufferedPort<VectorOf<double>>  *identifyTrajectoryOut,  
								BufferedPort<VectorOf<int>>		*identifyVoiceOut,
								BufferedPort<Bottle>			*recognizeSpeechOut,
								BufferedPort<VectorOf<double>>  *trackFaceIn,
								BufferedPort<VectorOf<double>>  *trackFaceOut, 
								BufferedPort<VectorOf<double>>  *trackHandIn,
								BufferedPort<VectorOf<double>>  *trackHandOut,  
								BufferedPort<VectorOf<double>>  *trackObjectIn, 
								BufferedPort<VectorOf<double>>  *trackObjectOut
								);
	bool threadInit();     
	void threadRelease();
	void run(); 
	double getangles(MyVector3D left_S,MyVector3D left_E,MyVector3D left_W, MyVector3D right_S, MyVector3D right_E, MyVector3D right_W, vector<double> &angles);
   
};



class SensoryAnalysis:public RFModule {


	/* module functional parameters */

	string moduleName; 
	string handlerPortName;
	string checkMutualGazeOutputPortName;
	string getArmAngleOutputPortName;
	string getBodyOutputPortName;
	string getBodyPoseOutputPortName;
	string getEyeGazeInputPortName;
	string getEyeGazeOutputPortName;
	string getEyesOutputPortName;
	string getFacesOutputPortName;
	string getGripLocationInputPortName;
	string getGripLocationOutputPortName;
	string getHandsOutputPortName;
	string getHeadOutputPortName;
	string getHeadGazeInputPortName;
	string getHeadGazeOutputPortName;
	string getObjectsOutputPortName;
	string getObjectsInputPortName;
	string getObjectTableDistanceInputPortName;
	string getObjectTableDistanceOutputPortName;
	string getSoundDirectionInputPortName;
	string getSoundDirectionOutputPortName;
	string identifyFaceInputPortName;
	string identifyFaceOutputPortName;
	string identifyFaceExpressionInputPortName;
	string identifyFaceExpressionOutputPortName;
	string identifyObjectInputPortName;
	string identifyObjectOutputPortName;
	string identifyTrajectoryInputPortName;
	string identifyTrajectoryOutputPortName;
	string identifyVoiceOutputPortName;
	string recognizeSpeechOutputPortName;
	string trackFaceInputPortName;
	string trackFaceOutputPortName;
	string trackHandInputPortName;
	string trackHandOutputPortName;
	string trackObjectInputPortName;
	string trackObjectOutputPortName;

	Port handlerPort;                               // a port to handle interactive messages (also uses bottles)

	BufferedPort<VectorOf<int>>		checkMutualGazeOut;//1
	BufferedPort<VectorOf<double>>  getArmAngleOut;
	BufferedPort<VectorOf<double>>  getBodyOut;
	BufferedPort<VectorOf<double>>  getBodyPoseOut; 
	BufferedPort<VectorOf<double>>  getEyeGazeIn;//5
	BufferedPort<VectorOf<double>>  getEyeGazeOut; 
	BufferedPort<VectorOf<double>>  getEyesOut;
	BufferedPort<VectorOf<double>>  getFacesOut; 
	BufferedPort<VectorOf<double>>  getGripLocationIn; 
	BufferedPort<VectorOf<double>>  getGripLocationOut; //10
	BufferedPort<VectorOf<double>>  getHandsOut;
	BufferedPort<VectorOf<double>>  getHeadOut;
	BufferedPort<VectorOf<double>>  getHeadGazeIn; 
	BufferedPort<VectorOf<double>>  getHeadGazeOut; 
	BufferedPort<VectorOf<double>>  getObjectsIn;
	BufferedPort<VectorOf<double>>  getObjectsOut; 
	BufferedPort<VectorOf<double>>  getObjectTableDistanceIn; 
	BufferedPort<VectorOf<double>>  getObjectTableDistanceOut; 
	BufferedPort<VectorOf<double>>  getSoundDirectionIn;
	BufferedPort<VectorOf<double>>  getSoundDirectionOut;  //20
	BufferedPort<VectorOf<double>>  identifyFaceIn;
	BufferedPort<VectorOf<double>>  identifyFaceOut;  
	BufferedPort<VectorOf<double>>	identifyFaceExpressionIn;  
	BufferedPort<VectorOf<int>>		identifyFaceExpressionOut;  
	BufferedPort<VectorOf<double>>  identifyObjectIn;
	BufferedPort<VectorOf<double>>  identifyObjectOut; 
	BufferedPort<VectorOf<int>>		identifyTrajectoryIn;  
	BufferedPort<VectorOf<double>>  identifyTrajectoryOut;  
	BufferedPort<VectorOf<int>>		identifyVoiceOut;
	BufferedPort<Bottle>			recognizeSpeechOut;//30 
	BufferedPort<VectorOf<double>>  trackFaceIn;
	BufferedPort<VectorOf<double>>  trackFaceOut;  
	BufferedPort<VectorOf<double>>  trackHandIn;
	BufferedPort<VectorOf<double>>  trackHandOut;  
	BufferedPort<VectorOf<double>>  trackObjectIn;  //35
	BufferedPort<VectorOf<double>>  trackObjectOut; 

   /* pointer to a new thread to be created and started in configure() and stopped in close() */

	SensoryAnalysisThread *sensoryAnalysisThread;					

	bool configureDreamSave(yarp::os::ResourceFinder &rf);
public:
   
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
};
