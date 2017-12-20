 /** @file naoInterface.h  Interface file for naoInterface Component
  *
  * Version 1.0
  * 
  * 22/06/2016
  * 
  * Generated by YarpGenerator, Emmanuel Senft, PLYM
  *
  * \section component_description Component Description
  *
  * Functionally, the component convert primitives into robot commands using the NaoQi API.
  * Feedbacks are provided to describe the results of actions executed and sensory inputs on the robot are relayed though the sensorFeedback port.
  * 
  * \section lib_sec Libraries
  *
  * - YARP.
  * - NaoQi
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
  * - \c from \c naoInterface.ini 
  *    - specifies the configuration file
  *
  * - \c context \c component/naoInterface/configuration
  *   - specifies the sub-path from \c $DREAM_ROOT/release
  *   to the configuration file
  *
  * - \c name \c naoInterface   
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
  * _disableRobotIn | /naoInterface/disableRobot:i
  * _enableRobotIn | /naoInterface/enableRobot:i
  * _gripIn | /naoInterface/grip:i
  * _moveHandIn | /naoInterface/moveHand:i
  * _moveTorsoIn | /naoInterface/moveTorso:i
  * _releaseIn | /naoInterface/release:i
  * _sayIn | /naoInterface/say:i
  * _moveSequenceIn | /naoInterface/moveSequence:i
  * _moveHeadIn | /naoInterface/moveHead:i
  * _pointAtIn | /naoInterface/pointAt:i
  *
  * - description
  *   
  * Key | Value
  * :-- | :---- 
  * _sensorFeedbackOut | /naoInterface/sensorFeedback:o    
  * _robotMotorFeedbackOut | /naoInterface/robotMotorFeedback:o    
  *
  * - description
  * 
  * Key | Value
  * :-- | :---- 
  * ip | 192.168.1.18
  * port | 9559
  * behaviour0 | movecar-3e4d5f/behavior_1
  * 
  * - The ip key should be used in combination with the port to set the address of the robot.
  * - behaviours can be defined in the config to reflect behaviours installed on the robot
  * and being able to used them from the script.
  * 
  * \section portsa_sec Ports Accessed
  * 
  * - None
  *                      
  * \section portsc_sec Ports Created
  *
  * <b>Input ports</b>
  *
  *  - \c /naoInterface
  *
  *  - \c /naoInterface/disableRobot:i
  *  - \c /naoInterface/enableRobot:i
  *  - \c /naoInterface/grip:i
  *  - \c /naoInterface/moveHand:i
  *  - \c /naoInterface/moveTorso:i
  *  - \c /naoInterface/release:i
  *  - \c /naoInterface/say:i
  *  - \c /naoInterface/moveSequence:i
  *  - \c /naoInterface/moveHead:i
  *  - \c /naoInterface/pointAt:i
  *
  * <b>Output ports</b>
  *
  *  - \c /naoInterface
  *
  *  - \c /naoInterface/sensorFeedback:o
  *  - \c /naoInterface/robotMotorFeedback:o
  *
  * <b>Port types </b>
  *
  * The functional specification only names the ports to be used to communicate with the component 
  * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
  *
  * \c BufferedPort<VectorOf<int>>		disableRobotIn; 
  * \c BufferedPort<VectorOf<int>>		enableRobotIn; 
  * \c BufferedPort<VectorOf<int>>		gripIn; 
  * \c BufferedPort<VectorOf<double>>		moveHandIn; 
  * \c BufferedPort<VectorOf<double>>		moveTorsoIn; 
  * \c BufferedPort<VectorOf<int>>		releaseIn; 
  * \c BufferedPort<Bottle>		sayIn; 
  * \c BufferedPort<VectorOf<int>>		moveSequenceIn; 
  * \c BufferedPort<VectorOf<double>>		moveHeadIn; 
  * \c BufferedPort<VectorOf<float>>		pointAtIn; 
  * \c BufferedPort<Bottle>		sensorFeedbackOut; 
  * \c BufferedPort<Bottle>		robotMotorFeedbackOut; 
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
  * \c naoInterface.ini   
  *
  * \section example_sec Example Instantiation of the Component
  * 
  * <tt>naoInterface --name naoInterface --context naoInterface/config --from naoInterface.ini </tt>
  *
  * \author 
  * 
  * <James Kennedy>, <Plymouth University>  
  * 
  * Copyright (C) 2014 DREAM Consortium
  * 
  */
  
/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Emmanuel Senft, Plymouth University 
 * Email:   emmanuel.senft@plymouth.ac.uk 
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
  * 15/04/2016 Version 1.0: ... (Emmanuel Senft)
  * 22/06/2016 Version 2.0: Total reworking of naoInterface to simplify the flow (Emmanuel Senft)
  */ 
 
#ifndef NAOINTERFACE
#define NAOINTERFACE

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

class NaoInterfaceYarpInterface;
class NaoInterfaceController;

class DisableRobotInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& disableRobotInData);
   NaoInterfaceYarpInterface *_naoInterfaceYarpInterface;
public:
   void setNaoInterface(NaoInterfaceYarpInterface* yarpInterface);
};

class EnableRobotInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& enableRobotInData);
   NaoInterfaceYarpInterface *_naoInterfaceYarpInterface;
public:
   void setNaoInterface(NaoInterfaceYarpInterface* yarpInterface);
};

class GripInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& gripInData);
   NaoInterfaceYarpInterface *_naoInterfaceYarpInterface;
public:
   void setNaoInterface(NaoInterfaceYarpInterface* yarpInterface);
};

class MoveHandInAnalyser : public BufferedPort<VectorOf<double>>{
   virtual void onRead(VectorOf<double>& moveHandInData);
   NaoInterfaceYarpInterface *_naoInterfaceYarpInterface;
public:
   void setNaoInterface(NaoInterfaceYarpInterface* yarpInterface);
};

class MoveTorsoInAnalyser : public BufferedPort<VectorOf<double>>{
   virtual void onRead(VectorOf<double>& moveTorsoInData);
   NaoInterfaceYarpInterface *_naoInterfaceYarpInterface;
public:
   void setNaoInterface(NaoInterfaceYarpInterface* yarpInterface);
};

class ReleaseInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& releaseInData);
   NaoInterfaceYarpInterface *_naoInterfaceYarpInterface;
public:
   void setNaoInterface(NaoInterfaceYarpInterface* yarpInterface);
};

class SayInAnalyser : public BufferedPort<Bottle>{
   virtual void onRead(Bottle& sayInData);
   NaoInterfaceYarpInterface *_naoInterfaceYarpInterface;
public:
   void setNaoInterface(NaoInterfaceYarpInterface* yarpInterface);
};

class MoveSequenceInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& moveSequenceInData);
   NaoInterfaceYarpInterface *_naoInterfaceYarpInterface;
public:
   void setNaoInterface(NaoInterfaceYarpInterface* yarpInterface);
};

class MoveHeadInAnalyser : public BufferedPort<VectorOf<double>>{
   virtual void onRead(VectorOf<double>& moveHeadInData);
   NaoInterfaceYarpInterface *_naoInterfaceYarpInterface;
public:
   void setNaoInterface(NaoInterfaceYarpInterface* yarpInterface);
};

class PointAtInAnalyser : public BufferedPort<VectorOf<float>>{
   virtual void onRead(VectorOf<float>& pointAtInData);
   NaoInterfaceYarpInterface *_naoInterfaceYarpInterface;
public:
   void setNaoInterface(NaoInterfaceYarpInterface* yarpInterface);
};



class NaoInterfaceThread : public yarp::os::Thread{
private:
	/* class variables */
	yarp::os::BufferedPort<VectorOf<int>>* _disableRobotIn;
	yarp::os::BufferedPort<VectorOf<int>>* _enableRobotIn;
	yarp::os::BufferedPort<VectorOf<int>>* _gripIn;
	yarp::os::BufferedPort<VectorOf<double>>* _moveHandIn;
	yarp::os::BufferedPort<VectorOf<double>>* _moveTorsoIn;
	yarp::os::BufferedPort<VectorOf<int>>* _releaseIn;
	yarp::os::BufferedPort<Bottle>* _sayIn;
	yarp::os::BufferedPort<VectorOf<int>>* _moveSequenceIn;
	yarp::os::BufferedPort<VectorOf<double>>* _moveHeadIn;
	yarp::os::BufferedPort<VectorOf<float>>* _pointAtIn;
	yarp::os::BufferedPort<Bottle>* _sensorFeedbackOut;
	yarp::os::BufferedPort<Bottle>* _robotMotorFeedbackOut;
	NaoInterfaceYarpInterface* _yarpInterface;
	DisableRobotInAnalyser *_disableRobotInAnalyser;
	EnableRobotInAnalyser *_enableRobotInAnalyser;
	GripInAnalyser *_gripInAnalyser;
	MoveHandInAnalyser *_moveHandInAnalyser;
	MoveTorsoInAnalyser *_moveTorsoInAnalyser;
	ReleaseInAnalyser *_releaseInAnalyser;
	SayInAnalyser *_sayInAnalyser;
	MoveSequenceInAnalyser *_moveSequenceInAnalyser;
	MoveHeadInAnalyser *_moveHeadInAnalyser;
	PointAtInAnalyser *_pointAtInAnalyser;
	
public:
	/* class methods */
	NaoInterfaceThread(BufferedPort<VectorOf<int>>* disableRobotIn, BufferedPort<VectorOf<int>>* enableRobotIn, BufferedPort<VectorOf<int>>* gripIn, BufferedPort<VectorOf<double>>* moveHandIn, BufferedPort<VectorOf<double>>* moveTorsoIn, BufferedPort<VectorOf<int>>* releaseIn, BufferedPort<Bottle>* sayIn, BufferedPort<VectorOf<int>>* moveSequenceIn, BufferedPort<VectorOf<double>>* moveHeadIn, BufferedPort<VectorOf<float>>* pointAtIn, BufferedPort<Bottle>* sensorFeedbackOut, BufferedPort<Bottle>* robotMotorFeedbackOut);
   void setYarpInterface(NaoInterfaceYarpInterface* yarpInterface);
	bool threadInit();
	void threadRelease();
	void sendToSensorFeedback(Bottle data);
	void sendToRobotMotorFeedback(Bottle data);
	void run();
};

class NaoInterface : public yarp::os::RFModule{
private:
	/* module parameters */
	std::string _moduleName;
	std::string _disableRobotInName;
	std::string _enableRobotInName;
	std::string _gripInName;
	std::string _moveHandInName;
	std::string _moveTorsoInName;
	std::string _releaseInName;
	std::string _sayInName;
	std::string _moveSequenceInName;
	std::string _moveHeadInName;
	std::string _pointAtInName;
	std::string _sensorFeedbackOutName;
	std::string _robotMotorFeedbackOutName;
	std::string _handlerPortName;

	/* class variables */
	yarp::os::BufferedPort<VectorOf<int>> _disableRobotIn;
	yarp::os::BufferedPort<VectorOf<int>> _enableRobotIn;
	yarp::os::BufferedPort<VectorOf<int>> _gripIn;
	yarp::os::BufferedPort<VectorOf<double>> _moveHandIn;
	yarp::os::BufferedPort<VectorOf<double>> _moveTorsoIn;
	yarp::os::BufferedPort<VectorOf<int>> _releaseIn;
	yarp::os::BufferedPort<Bottle> _sayIn;
	yarp::os::BufferedPort<VectorOf<int>> _moveSequenceIn;
	yarp::os::BufferedPort<VectorOf<double>> _moveHeadIn;
	yarp::os::BufferedPort<VectorOf<float>> _pointAtIn;
	yarp::os::BufferedPort<Bottle> _sensorFeedbackOut;
	yarp::os::BufferedPort<Bottle> _robotMotorFeedbackOut;
	yarp::os::Port _handlerPort;

	/* pointer to a new thread */
	NaoInterfaceThread* _naoInterfaceThread;
   NaoInterfaceController* _naoInterfaceController;
   NaoInterfaceYarpInterface* _naoInterfaceYarpInterface;
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