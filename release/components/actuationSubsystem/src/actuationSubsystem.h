 /** @file actuationSubsystem.h Interface file for actuationSubsystem Component
  *
  * Version 1.0
  * 
  * 21/02/2017
  *
  * \defgroup DREAM_actuationSubsystem actuationSubsystem
  * 
  * Interface file for the actuationSubsystem DREAM component
  *
  * \section component_description Component Description
  *
  * Functionally, the component receives high level actions from the GUI or the Attention-Reaction Subsystem
  * and transform them into low level actions using the primitives. It also relay sensory inputs from the robot
  * and interact with the sandtrayServer to know the coordinates of images on the sandtray.
  * 
  * \section lib_sec Libraries
  *
  * - YARP.
  * - Boost.
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
  * - \c from \c actuationSubsystem.ini 
  *    - specifies the configuration file
  *
  * - \c context \c component/actuationSubsystem/configuration
  *   - specifies the sub-path from \c $DREAM_ROOT/release
  *   to the configuration file
  *
  * - \c name \c actuationSubsystem   
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
  * _eyeBlinkingIn | /actuationSubsystem/eyeBlinking:i
  * _fallingReactionIn | /actuationSubsystem/fallingReaction:i
  * _fallingReactionSpeechIn | /actuationSubsystem/fallingReactionSpeech:i
  * _socialFacialExpressionIn | /actuationSubsystem/socialFacialExpression:i
  * _socialReactionIn | /actuationSubsystem/socialReaction:i
  * _socialReactionSpeechIn | /actuationSubsystem/socialReactionSpeech:i
  * _fallingInterruptionIn | /actuationSubsystem/fallingInterruption:i
  * _elicitedAttentionIn | /actuationSubsystem/elicitedAttention:i
  * _robotMotorFeedbackIn | /actuationSubsystem/robotMotorFeedback:i
  * _selectedActionIn | /actuationSubsystem/selectedAction:i
  * _sandtrayReturnIn | /actuationSubsystem/sandtrayReturn:i
  * _sensorFeedbackIn | /actuationSubsystem/sensorFeedback:i
  * _emergencyIn | /actuationSubsystem/emergency:i
  *
  * - description
  *   
  * Key | Value
  * :-- | :---- 
  * _robotSensorsOut | /actuationSubsystem/robotSensors:o    
  * _disableRobotOut | /actuationSubsystem/disableRobot:o    
  * _enableRobotOut | /actuationSubsystem/enableRobot:o    
  * _gripOut | /actuationSubsystem/grip:o    
  * _moveHandOut | /actuationSubsystem/moveHand:o    
  * _moveHeadOut | /actuationSubsystem/moveHead:o    
  * _moveSequenceOut | /actuationSubsystem/moveSequence:o    
  * _moveTorsoOut | /actuationSubsystem/moveTorso:o    
  * _pointAtOut | /actuationSubsystem/pointAt:o    
  * _releaseOut | /actuationSubsystem/release:o    
  * _sayOut | /actuationSubsystem/say:o    
  * _sandtrayCommandOut | /actuationSubsystem/sandtrayCommand:o    
  * _actionFeedbackOut | /actuationSubsystem/actionFeedback:o    
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
  *  - \c /actuationSubsystem
  *
  *  - \c /actuationSubsystem/eyeBlinking:i
  *  - \c /actuationSubsystem/fallingReaction:i
  *  - \c /actuationSubsystem/fallingReactionSpeech:i
  *  - \c /actuationSubsystem/socialFacialExpression:i
  *  - \c /actuationSubsystem/socialReaction:i
  *  - \c /actuationSubsystem/socialReactionSpeech:i
  *  - \c /actuationSubsystem/fallingInterruption:i
  *  - \c /actuationSubsystem/elicitedAttention:i
  *  - \c /actuationSubsystem/robotMotorFeedback:i
  *  - \c /actuationSubsystem/selectedAction:i
  *  - \c /actuationSubsystem/sandtrayReturn:i
  *  - \c /actuationSubsystem/sensorFeedback:i
  *  - \c /actuationSubsystem/emergency:i
  *
  * <b>Output ports</b>
  *
  *  - \c /actuationSubsystem
  *
  *  - \c /actuationSubsystem/robotSensors:o
  *  - \c /actuationSubsystem/disableRobot:o
  *  - \c /actuationSubsystem/enableRobot:o
  *  - \c /actuationSubsystem/grip:o
  *  - \c /actuationSubsystem/moveHand:o
  *  - \c /actuationSubsystem/moveHead:o
  *  - \c /actuationSubsystem/moveSequence:o
  *  - \c /actuationSubsystem/moveTorso:o
  *  - \c /actuationSubsystem/pointAt:o
  *  - \c /actuationSubsystem/release:o
  *  - \c /actuationSubsystem/say:o
  *  - \c /actuationSubsystem/sandtrayCommand:o
  *  - \c /actuationSubsystem/actionFeedback:o
  *
  * <b>Port types </b>
  *
  * The functional specification only names the ports to be used to communicate with the component 
  * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
  *
  * \c BufferedPort<Bottle>		eyeBlinkingIn; 
  * \c BufferedPort<VectorOf<int>>		fallingReactionIn; 
  * \c BufferedPort<VectorOf<int>>		fallingReactionSpeechIn; 
  * \c BufferedPort<VectorOf<int>>		socialFacialExpressionIn; 
  * \c BufferedPort<VectorOf<int>>		socialReactionIn; 
  * \c BufferedPort<VectorOf<int>>		socialReactionSpeechIn; 
  * \c BufferedPort<VectorOf<int>>		fallingInterruptionIn; 
  * \c BufferedPort<VectorOf<double>>		elicitedAttentionIn; 
  * \c BufferedPort<Bottle>		robotMotorFeedbackIn; 
  * \c BufferedPort<Bottle>		selectedActionIn; 
  * \c BufferedPort<Bottle>		sandtrayReturnIn; 
  * \c BufferedPort<Bottle>		sensorFeedbackIn; 
  * \c BufferedPort<Bottle>		emergencyIn; 
  * \c BufferedPort<Bottle>		robotSensorsOut; 
  * \c BufferedPort<VectorOf<int>>		disableRobotOut; 
  * \c BufferedPort<VectorOf<int>>		enableRobotOut; 
  * \c BufferedPort<VectorOf<int>>		gripOut; 
  * \c BufferedPort<VectorOf<double>>		moveHandOut; 
  * \c BufferedPort<VectorOf<double>>		moveHeadOut; 
  * \c BufferedPort<VectorOf<int>>		moveSequenceOut; 
  * \c BufferedPort<VectorOf<double>>		moveTorsoOut; 
  * \c BufferedPort<VectorOf<float>>		pointAtOut; 
  * \c BufferedPort<VectorOf<int>>		releaseOut; 
  * \c BufferedPort<Bottle>		sayOut; 
  * \c BufferedPort<Bottle>		sandtrayCommandOut; 
  * \c BufferedPort<Bottle>		actionFeedbackOut; 
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
  * \c actuationSubsystem.ini   
  *
  * \section example_sec Example Instantiation of the Component
  * 
  * <tt>actuationSubsystem --name actuationSubsystem --context actuationSubsystem/config --from actuationSubsystem.ini </tt>
  *
  * \author 
  * 
  * <Emmanuel Senft>, <Plymouth University>  
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
  * 21/02/2017 Version 1.0: ... (Emmanuel Senft)
  */ 
 
#ifndef ACTUATIONSUBSYSTEM
#define ACTUATIONSUBSYSTEM

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

class ActuationSubsystemYarpInterface;
class ActuationSubsystemController;

class EyeBlinkingInAnalyser : public BufferedPort<Bottle>{
   virtual void onRead(Bottle& eyeBlinkingInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class FallingReactionInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& fallingReactionInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class FallingReactionSpeechInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& fallingReactionSpeechInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class SocialFacialExpressionInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& socialFacialExpressionInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class SocialReactionInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& socialReactionInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class SocialReactionSpeechInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& socialReactionSpeechInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class FallingInterruptionInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& fallingInterruptionInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class ElicitedAttentionInAnalyser : public BufferedPort<VectorOf<double>>{
   virtual void onRead(VectorOf<double>& elicitedAttentionInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class RobotMotorFeedbackInAnalyser : public BufferedPort<Bottle>{
   virtual void onRead(Bottle& robotMotorFeedbackInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class SelectedActionInAnalyser : public BufferedPort<Bottle>{
   virtual void onRead(Bottle& selectedActionInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class SandtrayReturnInAnalyser : public BufferedPort<Bottle>{
   virtual void onRead(Bottle& sandtrayReturnInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class SensorFeedbackInAnalyser : public BufferedPort<Bottle>{
   virtual void onRead(Bottle& sensorFeedbackInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};

class EmergencyInAnalyser : public BufferedPort<Bottle>{
   virtual void onRead(Bottle& emergencyInData);
   ActuationSubsystemYarpInterface *_actuationSubsystemYarpInterface;
public:
   void setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface);
};



class ActuationSubsystemThread : public yarp::os::Thread{
private:
	/* class variables */
	yarp::os::BufferedPort<Bottle>* _eyeBlinkingIn;
	yarp::os::BufferedPort<VectorOf<int>>* _fallingReactionIn;
	yarp::os::BufferedPort<VectorOf<int>>* _fallingReactionSpeechIn;
	yarp::os::BufferedPort<VectorOf<int>>* _socialFacialExpressionIn;
	yarp::os::BufferedPort<VectorOf<int>>* _socialReactionIn;
	yarp::os::BufferedPort<VectorOf<int>>* _socialReactionSpeechIn;
	yarp::os::BufferedPort<VectorOf<int>>* _fallingInterruptionIn;
	yarp::os::BufferedPort<VectorOf<double>>* _elicitedAttentionIn;
	yarp::os::BufferedPort<Bottle>* _robotMotorFeedbackIn;
	yarp::os::BufferedPort<Bottle>* _selectedActionIn;
	yarp::os::BufferedPort<Bottle>* _sandtrayReturnIn;
	yarp::os::BufferedPort<Bottle>* _sensorFeedbackIn;
	yarp::os::BufferedPort<Bottle>* _emergencyIn;
	yarp::os::BufferedPort<Bottle>* _robotSensorsOut;
	yarp::os::BufferedPort<VectorOf<int>>* _disableRobotOut;
	yarp::os::BufferedPort<VectorOf<int>>* _enableRobotOut;
	yarp::os::BufferedPort<VectorOf<int>>* _gripOut;
	yarp::os::BufferedPort<VectorOf<double>>* _moveHandOut;
	yarp::os::BufferedPort<VectorOf<double>>* _moveHeadOut;
	yarp::os::BufferedPort<VectorOf<int>>* _moveSequenceOut;
	yarp::os::BufferedPort<VectorOf<double>>* _moveTorsoOut;
	yarp::os::BufferedPort<VectorOf<float>>* _pointAtOut;
	yarp::os::BufferedPort<VectorOf<int>>* _releaseOut;
	yarp::os::BufferedPort<Bottle>* _sayOut;
	yarp::os::BufferedPort<Bottle>* _sandtrayCommandOut;
	yarp::os::BufferedPort<Bottle>* _actionFeedbackOut;
	ActuationSubsystemYarpInterface* _yarpInterface;
	EyeBlinkingInAnalyser *_eyeBlinkingInAnalyser;
	FallingReactionInAnalyser *_fallingReactionInAnalyser;
	FallingReactionSpeechInAnalyser *_fallingReactionSpeechInAnalyser;
	SocialFacialExpressionInAnalyser *_socialFacialExpressionInAnalyser;
	SocialReactionInAnalyser *_socialReactionInAnalyser;
	SocialReactionSpeechInAnalyser *_socialReactionSpeechInAnalyser;
	FallingInterruptionInAnalyser *_fallingInterruptionInAnalyser;
	ElicitedAttentionInAnalyser *_elicitedAttentionInAnalyser;
	RobotMotorFeedbackInAnalyser *_robotMotorFeedbackInAnalyser;
	SelectedActionInAnalyser *_selectedActionInAnalyser;
	SandtrayReturnInAnalyser *_sandtrayReturnInAnalyser;
	SensorFeedbackInAnalyser *_sensorFeedbackInAnalyser;
	EmergencyInAnalyser *_emergencyInAnalyser;
	
public:
	/* class methods */
	ActuationSubsystemThread(BufferedPort<Bottle>* eyeBlinkingIn, BufferedPort<VectorOf<int>>* fallingReactionIn, BufferedPort<VectorOf<int>>* fallingReactionSpeechIn, BufferedPort<VectorOf<int>>* socialFacialExpressionIn, BufferedPort<VectorOf<int>>* socialReactionIn, BufferedPort<VectorOf<int>>* socialReactionSpeechIn, BufferedPort<VectorOf<int>>* fallingInterruptionIn, BufferedPort<VectorOf<double>>* elicitedAttentionIn, BufferedPort<Bottle>* robotMotorFeedbackIn, BufferedPort<Bottle>* selectedActionIn, BufferedPort<Bottle>* sandtrayReturnIn, BufferedPort<Bottle>* sensorFeedbackIn, BufferedPort<Bottle>* emergencyIn, BufferedPort<Bottle>* robotSensorsOut, BufferedPort<VectorOf<int>>* disableRobotOut, BufferedPort<VectorOf<int>>* enableRobotOut, BufferedPort<VectorOf<int>>* gripOut, BufferedPort<VectorOf<double>>* moveHandOut, BufferedPort<VectorOf<double>>* moveHeadOut, BufferedPort<VectorOf<int>>* moveSequenceOut, BufferedPort<VectorOf<double>>* moveTorsoOut, BufferedPort<VectorOf<float>>* pointAtOut, BufferedPort<VectorOf<int>>* releaseOut, BufferedPort<Bottle>* sayOut, BufferedPort<Bottle>* sandtrayCommandOut, BufferedPort<Bottle>* actionFeedbackOut);
   void setYarpInterface(ActuationSubsystemYarpInterface* yarpInterface);
	bool threadInit();
	void threadRelease();
	void sendToRobotSensors(Bottle data);
	void sendToDisableRobot(VectorOf<int> data);
	void sendToEnableRobot(VectorOf<int> data);
	void sendToGrip(VectorOf<int> data);
	void sendToMoveHand(VectorOf<double> data);
	void sendToMoveHead(VectorOf<double> data);
	void sendToMoveSequence(VectorOf<int> data);
	void sendToMoveTorso(VectorOf<double> data);
	void sendToPointAt(VectorOf<float> data);
	void sendToRelease(VectorOf<int> data);
	void sendToSay(Bottle data);
	void sendToSandtrayCommand(Bottle data);
	void sendToActionFeedback(Bottle data);
	void run();
};

class ActuationSubsystem : public yarp::os::RFModule{
private:
	/* module parameters */
	std::string _moduleName;
	std::string _eyeBlinkingInName;
	std::string _fallingReactionInName;
	std::string _fallingReactionSpeechInName;
	std::string _socialFacialExpressionInName;
	std::string _socialReactionInName;
	std::string _socialReactionSpeechInName;
	std::string _fallingInterruptionInName;
	std::string _elicitedAttentionInName;
	std::string _robotMotorFeedbackInName;
	std::string _selectedActionInName;
	std::string _sandtrayReturnInName;
	std::string _sensorFeedbackInName;
	std::string _emergencyInName;
	std::string _robotSensorsOutName;
	std::string _disableRobotOutName;
	std::string _enableRobotOutName;
	std::string _gripOutName;
	std::string _moveHandOutName;
	std::string _moveHeadOutName;
	std::string _moveSequenceOutName;
	std::string _moveTorsoOutName;
	std::string _pointAtOutName;
	std::string _releaseOutName;
	std::string _sayOutName;
	std::string _sandtrayCommandOutName;
	std::string _actionFeedbackOutName;
	std::string _handlerPortName;

	/* class variables */
	yarp::os::BufferedPort<Bottle> _eyeBlinkingIn;
	yarp::os::BufferedPort<VectorOf<int>> _fallingReactionIn;
	yarp::os::BufferedPort<VectorOf<int>> _fallingReactionSpeechIn;
	yarp::os::BufferedPort<VectorOf<int>> _socialFacialExpressionIn;
	yarp::os::BufferedPort<VectorOf<int>> _socialReactionIn;
	yarp::os::BufferedPort<VectorOf<int>> _socialReactionSpeechIn;
	yarp::os::BufferedPort<VectorOf<int>> _fallingInterruptionIn;
	yarp::os::BufferedPort<VectorOf<double>> _elicitedAttentionIn;
	yarp::os::BufferedPort<Bottle> _robotMotorFeedbackIn;
	yarp::os::BufferedPort<Bottle> _selectedActionIn;
	yarp::os::BufferedPort<Bottle> _sandtrayReturnIn;
	yarp::os::BufferedPort<Bottle> _sensorFeedbackIn;
	yarp::os::BufferedPort<Bottle> _emergencyIn;
	yarp::os::BufferedPort<Bottle> _robotSensorsOut;
	yarp::os::BufferedPort<VectorOf<int>> _disableRobotOut;
	yarp::os::BufferedPort<VectorOf<int>> _enableRobotOut;
	yarp::os::BufferedPort<VectorOf<int>> _gripOut;
	yarp::os::BufferedPort<VectorOf<double>> _moveHandOut;
	yarp::os::BufferedPort<VectorOf<double>> _moveHeadOut;
	yarp::os::BufferedPort<VectorOf<int>> _moveSequenceOut;
	yarp::os::BufferedPort<VectorOf<double>> _moveTorsoOut;
	yarp::os::BufferedPort<VectorOf<float>> _pointAtOut;
	yarp::os::BufferedPort<VectorOf<int>> _releaseOut;
	yarp::os::BufferedPort<Bottle> _sayOut;
	yarp::os::BufferedPort<Bottle> _sandtrayCommandOut;
	yarp::os::BufferedPort<Bottle> _actionFeedbackOut;
	yarp::os::Port _handlerPort;

	/* pointer to a new thread */
	ActuationSubsystemThread* _actuationSubsystemThread;
   ActuationSubsystemController* _actuationSubsystemController;
   ActuationSubsystemYarpInterface* _actuationSubsystemYarpInterface;
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
