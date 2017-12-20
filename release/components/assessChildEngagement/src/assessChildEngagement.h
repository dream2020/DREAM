 /** @file assessChildEngagement.h 
  *
  * Version 1.0
  * 
  * 08/11/2016
  *
  * \defgroup DREAM_assessChildEngagement assessChildEngagement
  * 
  * Interface file for the assessChildEngagement DREAM component
  *
  * \section component_description Component Description
  *
  * Functionally, the component continuously queries the presence of three indicators of engagement
  * defined by UBB (position of the child in front of the robot, mutual gaze, smile). Once values 
  * for all three are obtained, the engagement value is calculated as the sum of the number of 
  * present indicators and written to the corresponding port
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
  * - \c from \c assessChildEngagement.ini 
  *    - specifies the configuration file
  *
  * - \c context \c component/assessChildEngagement/configuration
  *   - specifies the sub-path from \c $DREAM_ROOT/release
  *   to the configuration file
  *
  * - \c name \c assessChildEngagement   
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
  * _identifyFaceExpressionIn | /assessChildEngagement/identifyFaceExpression:i
  * _checkMutualGazeIn | /assessChildEngagement/checkMutualGaze:i
  * _getBodyIn | /assessChildEngagement/getBody:i
  *
  * - description
  *   
  * Key | Value
  * :-- | :---- 
  * _engagementFlagOut | /assessChildEngagement/engagementFlag:o    
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
  *  - \c /assessChildEngagement
  *
  *  - \c /assessChildEngagement/identifyFaceExpression:i
  *  - \c /assessChildEngagement/checkMutualGaze:i
  *  - \c /assessChildEngagement/getBody:i
  *
  * <b>Output ports</b>
  *
  *  - \c /assessChildEngagement
  *
  *  - \c /assessChildEngagement/engagementFlag:o
  *
  * <b>Port types </b>
  *
  * The functional specification only names the ports to be used to communicate with the component 
  * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
  *
  * \c BufferedPort<VectorOf<int>>		identifyFaceExpressionIn; 
  * \c BufferedPort<VectorOf<int>>		checkMutualGazeIn; 
  * \c BufferedPort<VectorOf<double>>		getBodyIn; 
  * \c BufferedPort<VectorOf<double>>		engagementFlagOut; 
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
  * \c assessChildEngagement.ini   
  *
  * \section example_sec Example Instantiation of the Component
  * 
  * <tt>assessChildEngagement --name assessChildEngagement --context assessChildEngagement/config --from assessChildEngagement.ini </tt>
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
 
#ifndef ASSESSCHILDENGAGEMENT
#define ASSESSCHILDENGAGEMENT

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

class AssessChildEngagementYarpInterface;
class AssessChildEngagementController;

class IdentifyFaceExpressionInAnalyser : public BufferedPort<VectorOf<int> >{
   virtual void onRead(VectorOf<int>& identifyFaceExpressionInData);
   AssessChildEngagementYarpInterface *_assessChildEngagementYarpInterface;
public:
   void setAssessChildEngagement(AssessChildEngagementYarpInterface* yarpInterface);
};

class CheckMutualGazeInAnalyser : public BufferedPort<VectorOf<int> >{
   virtual void onRead(VectorOf<int>& checkMutualGazeInData);
   AssessChildEngagementYarpInterface *_assessChildEngagementYarpInterface;
public:
   void setAssessChildEngagement(AssessChildEngagementYarpInterface* yarpInterface);
};

class GetBodyInAnalyser : public BufferedPort<VectorOf<double> >{
   virtual void onRead(VectorOf<double>& getBodyInData);
   AssessChildEngagementYarpInterface *_assessChildEngagementYarpInterface;
public:
   void setAssessChildEngagement(AssessChildEngagementYarpInterface* yarpInterface);
};



class AssessChildEngagementThread : public yarp::os::Thread{
private:
	/* class variables */
	yarp::os::BufferedPort<VectorOf<int> >* _identifyFaceExpressionIn;
	yarp::os::BufferedPort<VectorOf<int> >* _checkMutualGazeIn;
	yarp::os::BufferedPort<VectorOf<double> >* _getBodyIn;
	yarp::os::BufferedPort<VectorOf<double> >* _engagementFlagOut;
	AssessChildEngagementYarpInterface* _yarpInterface;
	IdentifyFaceExpressionInAnalyser *_identifyFaceExpressionInAnalyser;
	CheckMutualGazeInAnalyser *_checkMutualGazeInAnalyser;
	GetBodyInAnalyser *_getBodyInAnalyser;
	
public:
	/* class methods */
	AssessChildEngagementThread(BufferedPort<VectorOf<int> >* identifyFaceExpressionIn, BufferedPort<VectorOf<int> >* checkMutualGazeIn, BufferedPort<VectorOf<double> >* getBodyIn, BufferedPort<VectorOf<double> >* engagementFlagOut);
   void setYarpInterface(AssessChildEngagementYarpInterface* yarpInterface);
	bool threadInit();
	void threadRelease();
	void sendToEngagementFlag(VectorOf<double> data);
	void run();
};

class AssessChildEngagement : public yarp::os::RFModule{
private:
	/* module parameters */
	std::string _moduleName;
	std::string _identifyFaceExpressionInName;
	std::string _checkMutualGazeInName;
	std::string _getBodyInName;
	std::string _engagementFlagOutName;
	std::string _handlerPortName;

	/* class variables */
	yarp::os::BufferedPort<VectorOf<int> > _identifyFaceExpressionIn;
	yarp::os::BufferedPort<VectorOf<int> > _checkMutualGazeIn;
	yarp::os::BufferedPort<VectorOf<double> > _getBodyIn;
	yarp::os::BufferedPort<VectorOf<double> > _engagementFlagOut;
	yarp::os::Port _handlerPort;

	/* pointer to a new thread */
	AssessChildEngagementThread* _assessChildEngagementThread;
   AssessChildEngagementController* _assessChildEngagementController;
   AssessChildEngagementYarpInterface* _assessChildEngagementYarpInterface;
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
