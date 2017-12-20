 /** @file assessChildPerformance.h 
  *
  * Version 1.0
  * 
  * 08/11/2016
  *
  * \defgroup DREAM_assessChildPerformance assessChildPerformance
  * 
  * Interface file for the assessChildPerformance DREAM component
  *
  * \section component_description Component Description
  *
  * Functionally, the component listens for requests to detect specific  behaviours by the 
  * child from WP6. It also listens to any relevant source of behavioural data. 
  * Once a request for a specific behaviour is obtained, its detection on the corresponding 
  * source, within the timeframe specified in the request will trigger the return of the 
  * corresponding performance value (typically 1 or 0, but for trajectories detected by WP4
  * graded values between 0 and 1 are possible and indicate confidence). By default, if no
  * evidence for or against the detection of an appropriate behaviour is received, the 
  * component will return a performance of -1 at the end of the specified time period
  * 
  * \section lib_sec Libraries
  *
  * - YARP.
  *
  * \section parameters_sec Parameters
  * 
  * <b>Command-line Parameters </b> 
  * 
  * The following key-value pairs can be specified as command-line parameters 
  * by prefixing \c -- to the key e.g. \c --from file.ini. 
  * The value part can be changed to suit your needs; the default values are 
  * shown below. 
  *
  * - \c from \c assessChildPerformance.ini 
  *    - specifies the configuration file
  *
  * - \c context \c component/assessChildPerformance/configuration
  *   - specifies the sub-path from \c $DREAM_ROOT/release
  *   to the configuration file
  *
  * - \c name \c assessChildPerformance   
  *   - specifies the name of the component (used to form the stem of component port names)  
  *
  *
  * <b>Configuration File Parameters </b>
  *
  * The following key-value pairs can be specified as parameters in the configuration file 
  * (they can also be specified as command-line parameters if you so wish). 
  * The value part can be changed to suit your needs; the default values are shown below. 
  * 
  * Key | Value
  * :-- | :----  
  * _identifyVoiceIn | /assessChildPerformance/identifyVoice:i
  * _getEyeGazeIn | /assessChildPerformance/getEyeGaze:i
  * _getBodyPoseIn | /assessChildPerformance/getBodyPose:i
  * _identifyTrajectoryIn | /assessChildPerformance/identifyTrajectory:i
  * _getInterventionStatusIn | /assessChildPerformance/getInterventionStatus:i
  * _interactionEventIn | /assessChildPerformance/interactionEvent:i
  *
  * - description
  *   
  * Key | Value
  * :-- | :---- 
  * _getChildBehaviourOut | /assessChildPerformance/getChildBehaviour:o    
  * _identifyTrajectoryOut | /assessChildPerformance/identifyTrajectory:o    
  * _getChildPerformanceOut | /assessChildPerformance/getChildPerformance:o    
  *
  * - description
  *
  * \section portsa_sec Ports Accessed
  * 
  * - None
  *                      
  * \section portsc_sec Ports Created
  *
  * <b>Input ports</b>
  *
  *  - \c /assessChildPerformance
  *
  *  - \c /assessChildPerformance/identifyVoice:i
  *  - \c /assessChildPerformance/getEyeGaze:i
  *  - \c /assessChildPerformance/getBodyPose:i
  *  - \c /assessChildPerformance/identifyTrajectory:i
  *  - \c /assessChildPerformance/getInterventionStatus:i
  *  - \c /assessChildPerformance/interactionEvent:i
  *
  * <b>Output ports</b>
  *
  *  - \c /assessChildPerformance
  *
  *  - \c /assessChildPerformance/getChildBehaviour:o
  *  - \c /assessChildPerformance/identifyTrajectory:o
  *  - \c /assessChildPerformance/getChildPerformance:o
  *
  * <b>Port types </b>
  *
  * The functional specification only names the ports to be used to communicate with the component 
  * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
  *
  * \c BufferedPort<VectorOf<int> >		identifyVoiceIn; 
  * \c BufferedPort<VectorOf<double> >		getEyeGazeIn; 
  * \c BufferedPort<VectorOf<double> >		getBodyPoseIn; 
  * \c BufferedPort<VectorOf<double> >		identifyTrajectoryIn; 
  * \c BufferedPort<VectorOf<int> >		getInterventionStatusIn; 
  * \c BufferedPort<VectorOf<int> >		interactionEventIn; 
  * \c BufferedPort<VectorOf<double> >		getChildBehaviourOut; 
  * \c BufferedPort<VectorOf<int> >		identifyTrajectoryOut; 
  * \c BufferedPort<VectorOf<double> >		getChildPerformanceOut; 
  *
  * \section in_files_sec Input Data Files
  *
  * None
  *
  * \section out_data_sec Output Data Files
  *
  * None
  *
  * \section conf_file_sec Configuration Files
  *
  * \c assessChildPerformance.ini   
  *
  * \section example_sec Example Instantiation of the Component
  * 
  * <tt>assessChildPerformance --name assessChildPerformance --context assessChildPerformance/config --from assessChildPerformance.ini </tt>
  *
  * \author 
  * 
  * <Serge Thill>, <University of Skövde>  
  * 
  * Copyright (C) 2014 DREAM Consortium
  * 
  */
  
/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Serge Thill, University of Skövde 
 * Email:   serge.thill@his.se 
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
  
 /* 
  * Audit Trail
  * -----------
  * 08/11/2016 Version 1.0: ... (Serge Thill)
  */ 
 
#ifndef ASSESSCHILDPERFORMANCE
#define ASSESSCHILDPERFORMANCE

#include <iostream>
#include <string>
#include <vector>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>

using namespace yarp::os; 
using namespace yarp::sig;

class AssessChildPerformanceYarpInterface;
class AssessChildPerformanceController;

class IdentifyVoiceInAnalyser : public BufferedPort<VectorOf<int> >{
   virtual void onRead(VectorOf<int>& identifyVoiceInData);
   AssessChildPerformanceYarpInterface *_assessChildPerformanceYarpInterface;
public:
   void setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface);
};

class GetEyeGazeInAnalyser : public BufferedPort<VectorOf<double> >{
   virtual void onRead(VectorOf<double>& getEyeGazeInData);
   AssessChildPerformanceYarpInterface *_assessChildPerformanceYarpInterface;
public:
   void setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface);
};

class GetBodyPoseInAnalyser : public BufferedPort<VectorOf<double> >{
   virtual void onRead(VectorOf<double>& getBodyPoseInData);
   AssessChildPerformanceYarpInterface *_assessChildPerformanceYarpInterface;
public:
   void setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface);
};

class IdentifyTrajectoryInAnalyser : public BufferedPort<VectorOf<double> >{
   virtual void onRead(VectorOf<double>& identifyTrajectoryInData);
   AssessChildPerformanceYarpInterface *_assessChildPerformanceYarpInterface;
public:
   void setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface);
};

class GetInterventionStatusInAnalyser : public BufferedPort<VectorOf<int> >{
   virtual void onRead(VectorOf<int>& getInterventionStatusInData);
   AssessChildPerformanceYarpInterface *_assessChildPerformanceYarpInterface;
public:
   void setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface);
};

class InteractionEventInAnalyser : public BufferedPort<VectorOf<int> >{
   virtual void onRead(VectorOf<int>& interactionEventInData);
   AssessChildPerformanceYarpInterface *_assessChildPerformanceYarpInterface;
public:
   void setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface);
};



class AssessChildPerformanceThread : public yarp::os::Thread{
private:
	/* class variables */
	yarp::os::BufferedPort<VectorOf<int> >* _identifyVoiceIn;
	yarp::os::BufferedPort<VectorOf<double> >* _getEyeGazeIn;
	yarp::os::BufferedPort<VectorOf<double> >* _getBodyPoseIn;
	yarp::os::BufferedPort<VectorOf<double> >* _identifyTrajectoryIn;
	yarp::os::BufferedPort<VectorOf<int> >* _getInterventionStatusIn;
	yarp::os::BufferedPort<VectorOf<int> >* _interactionEventIn;
	yarp::os::BufferedPort<VectorOf<double> >* _getChildBehaviourOut;
	yarp::os::BufferedPort<VectorOf<int> >* _identifyTrajectoryOut;
	yarp::os::BufferedPort<VectorOf<double> >* _getChildPerformanceOut;
	AssessChildPerformanceYarpInterface* _yarpInterface;
	IdentifyVoiceInAnalyser *_identifyVoiceInAnalyser;
	GetEyeGazeInAnalyser *_getEyeGazeInAnalyser;
	GetBodyPoseInAnalyser *_getBodyPoseInAnalyser;
	IdentifyTrajectoryInAnalyser *_identifyTrajectoryInAnalyser;
	GetInterventionStatusInAnalyser *_getInterventionStatusInAnalyser;
	InteractionEventInAnalyser *_interactionEventInAnalyser;
	
public:
	/* class methods */
	AssessChildPerformanceThread(BufferedPort<VectorOf<int> >* identifyVoiceIn, BufferedPort<VectorOf<double> >* getEyeGazeIn, BufferedPort<VectorOf<double> >* getBodyPoseIn, BufferedPort<VectorOf<double> >* identifyTrajectoryIn, BufferedPort<VectorOf<int> >* getInterventionStatusIn, BufferedPort<VectorOf<int> >* interactionEventIn, BufferedPort<VectorOf<double> >* getChildBehaviourOut, BufferedPort<VectorOf<int> >* identifyTrajectoryOut, BufferedPort<VectorOf<double> >* getChildPerformanceOut);
   void setYarpInterface(AssessChildPerformanceYarpInterface* yarpInterface);
	bool threadInit();
	void threadRelease();
	void sendToGetChildBehaviour(VectorOf<double> data);
	void sendToIdentifyTrajectory(VectorOf<int> data);
	void sendToGetChildPerformance(VectorOf<double> data);
	void run();
};

class AssessChildPerformance : public yarp::os::RFModule{
private:
	/* module parameters */
	std::string _moduleName;
	std::string _identifyVoiceInName;
	std::string _getEyeGazeInName;
	std::string _getBodyPoseInName;
	std::string _identifyTrajectoryInName;
	std::string _getInterventionStatusInName;
	std::string _interactionEventInName;
	std::string _getChildBehaviourOutName;
	std::string _identifyTrajectoryOutName;
	std::string _getChildPerformanceOutName;
	std::string _handlerPortName;

	/* class variables */
	yarp::os::BufferedPort<VectorOf<int> > _identifyVoiceIn;
	yarp::os::BufferedPort<VectorOf<double> > _getEyeGazeIn;
	yarp::os::BufferedPort<VectorOf<double> > _getBodyPoseIn;
	yarp::os::BufferedPort<VectorOf<double> > _identifyTrajectoryIn;
	yarp::os::BufferedPort<VectorOf<int> > _getInterventionStatusIn;
	yarp::os::BufferedPort<VectorOf<int> > _interactionEventIn;
	yarp::os::BufferedPort<VectorOf<double> > _getChildBehaviourOut;
	yarp::os::BufferedPort<VectorOf<int> > _identifyTrajectoryOut;
	yarp::os::BufferedPort<VectorOf<double> > _getChildPerformanceOut;
	yarp::os::Port _handlerPort;

	/* pointer to a new thread */
	AssessChildPerformanceThread* _assessChildPerformanceThread;
   AssessChildPerformanceController* _assessChildPerformanceController;
   AssessChildPerformanceYarpInterface* _assessChildPerformanceYarpInterface;
public:
	/* class methods */
	bool configure(yarp::os::ResourceFinder &rf);
	bool interruptModule();
	bool close();
	bool respond(const yarp::os::Bottle& command, Bottle& reply);
	double getPeriod();
	bool updateModule();
};

#endif
