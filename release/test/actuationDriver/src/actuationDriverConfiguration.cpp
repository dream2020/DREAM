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
  * 25/10/2016 Version 1.0: ... (Emmanuel Senft)
  */ 

#include "actuationDriver.h"
#include "actuationDriverYarpInterface.h"
#include "actuationDriverController.h"
#include "actionMessages.h"
#include "moveSequenceId.h"


bool ActuationDriver::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("actuationDriver"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _disableRobotInName = "/" +  getName(rf.check("_disableRobotIn", Value("/disableRobot:i"), "Input disableRobot port (string)").asString());
   _enableRobotInName = "/" +  getName(rf.check("_enableRobotIn", Value("/enableRobot:i"), "Input enableRobot port (string)").asString());
   _gripInName = "/" +  getName(rf.check("_gripIn", Value("/grip:i"), "Input grip port (string)").asString());
   _moveHandInName = "/" +  getName(rf.check("_moveHandIn", Value("/moveHand:i"), "Input moveHand port (string)").asString());
   _moveHeadInName = "/" +  getName(rf.check("_moveHeadIn", Value("/moveHead:i"), "Input moveHead port (string)").asString());
   _moveSequenceInName = "/" +  getName(rf.check("_moveSequenceIn", Value("/moveSequence:i"), "Input moveSequence port (string)").asString());
   _moveTorsoInName = "/" +  getName(rf.check("_moveTorsoIn", Value("/moveTorso:i"), "Input moveTorso port (string)").asString());
   _pointAtInName = "/" +  getName(rf.check("_pointAtIn", Value("/pointAt:i"), "Input pointAt port (string)").asString());
   _releaseInName = "/" +  getName(rf.check("_releaseIn", Value("/release:i"), "Input release port (string)").asString());
   _sayInName = "/" +  getName(rf.check("_sayIn", Value("/say:i"), "Input say port (string)").asString());
   _sandtrayCommandInName = "/" +  getName(rf.check("_sandtrayCommandIn", Value("/sandtrayCommand:i"), "Input sandtrayCommand port (string)").asString());
   _actionFeedbackInName = "/" +  getName(rf.check("_actionFeedbackIn", Value("/actionFeedback:i"), "Input actionFeedback port (string)").asString());
   _robotSensorsInName = "/" +  getName(rf.check("_robotSensorsIn", Value("/robotSensors:i"), "Input robotSensors port (string)").asString());

   _eyeBlinkingOutName = "/" +  getName(rf.check("_eyeBlinkingOut", Value("/eyeBlinking:o"), "output eyeBlinking port (string)").asString());
   _fallingReactionOutName = "/" +  getName(rf.check("_fallingReactionOut", Value("/fallingReaction:o"), "output fallingReaction port (string)").asString());
   _fallingReactionSpeechOutName = "/" +  getName(rf.check("_fallingReactionSpeechOut", Value("/fallingReactionSpeech:o"), "output fallingReactionSpeech port (string)").asString());
   _socialFacialExpressionOutName = "/" +  getName(rf.check("_socialFacialExpressionOut", Value("/socialFacialExpression:o"), "output socialFacialExpression port (string)").asString());
   _socialReactionOutName = "/" +  getName(rf.check("_socialReactionOut", Value("/socialReaction:o"), "output socialReaction port (string)").asString());
   _socialReactionSpeechOutName = "/" +  getName(rf.check("_socialReactionSpeechOut", Value("/socialReactionSpeech:o"), "output socialReactionSpeech port (string)").asString());
   _robotMotorFeedbackOutName = "/" +  getName(rf.check("_robotMotorFeedbackOut", Value("/robotMotorFeedback:o"), "output robotMotorFeedback port (string)").asString());
   _selectedActionOutName = "/" +  getName(rf.check("_selectedActionOut", Value("/selectedAction:o"), "output selectedAction port (string)").asString());
   _sandtrayReturnOutName = "/" +  getName(rf.check("_sandtrayReturnOut", Value("/sandtrayReturn:o"), "output sandtrayReturn port (string)").asString());
   _sensorFeedbackOutName = "/" +  getName(rf.check("_sensorFeedbackOut", Value("/sensorFeedback:o"), "output sensorFeedback port (string)").asString());
   _moveHeadOutName = "/" +  getName(rf.check("_moveHeadOut", Value("/moveHead:o"), "output moveHead port (string)").asString());
   _fallingInterruptionOutName = "/" +  getName(rf.check("_fallingInterruptionOut", Value("/fallingInterruption:o"), "output fallingInterruption port (string)").asString());
   _elicitedAttentionOutName = "/" +  getName(rf.check("_elicitedAttentionOut", Value("/elicitedAttention:o"), "output elicitedAttention port (string)").asString());

   /* open the ports */
   if (!_disableRobotIn.open(_disableRobotInName.c_str())){
      std::cout << getName() << ": unable to open port " << _disableRobotInName << std::endl;
      return false;
   }
   if (!_enableRobotIn.open(_enableRobotInName.c_str())){
      std::cout << getName() << ": unable to open port " << _enableRobotInName << std::endl;
      return false;
   }
   if (!_gripIn.open(_gripInName.c_str())){
      std::cout << getName() << ": unable to open port " << _gripInName << std::endl;
      return false;
   }
   if (!_moveHandIn.open(_moveHandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _moveHandInName << std::endl;
      return false;
   }
   if (!_moveHeadIn.open(_moveHeadInName.c_str())){
      std::cout << getName() << ": unable to open port " << _moveHeadInName << std::endl;
      return false;
   }
   if (!_moveSequenceIn.open(_moveSequenceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _moveSequenceInName << std::endl;
      return false;
   }
   if (!_moveTorsoIn.open(_moveTorsoInName.c_str())){
      std::cout << getName() << ": unable to open port " << _moveTorsoInName << std::endl;
      return false;
   }
   if (!_pointAtIn.open(_pointAtInName.c_str())){
      std::cout << getName() << ": unable to open port " << _pointAtInName << std::endl;
      return false;
   }
   if (!_releaseIn.open(_releaseInName.c_str())){
      std::cout << getName() << ": unable to open port " << _releaseInName << std::endl;
      return false;
   }
   if (!_sayIn.open(_sayInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sayInName << std::endl;
      return false;
   }
   if (!_sandtrayCommandIn.open(_sandtrayCommandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sandtrayCommandInName << std::endl;
      return false;
   }
   if (!_actionFeedbackIn.open(_actionFeedbackInName.c_str())){
      std::cout << getName() << ": unable to open port " << _actionFeedbackInName << std::endl;
      return false;
   }
   if (!_robotSensorsIn.open(_robotSensorsInName.c_str())){
      std::cout << getName() << ": unable to open port " << _robotSensorsInName << std::endl;
      return false;
   }

   if (!_eyeBlinkingOut.open(_eyeBlinkingOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _eyeBlinkingOutName << std::endl;
      return false;
   }
   if (!_fallingReactionOut.open(_fallingReactionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _fallingReactionOutName << std::endl;
      return false;
   }
   if (!_fallingReactionSpeechOut.open(_fallingReactionSpeechOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _fallingReactionSpeechOutName << std::endl;
      return false;
   }
   if (!_socialFacialExpressionOut.open(_socialFacialExpressionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _socialFacialExpressionOutName << std::endl;
      return false;
   }
   if (!_socialReactionOut.open(_socialReactionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _socialReactionOutName << std::endl;
      return false;
   }
   if (!_socialReactionSpeechOut.open(_socialReactionSpeechOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _socialReactionSpeechOutName << std::endl;
      return false;
   }
   if (!_robotMotorFeedbackOut.open(_robotMotorFeedbackOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _robotMotorFeedbackOutName << std::endl;
      return false;
   }
   if (!_selectedActionOut.open(_selectedActionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _selectedActionOutName << std::endl;
      return false;
   }
   if (!_sandtrayReturnOut.open(_sandtrayReturnOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sandtrayReturnOutName << std::endl;
      return false;
   }
   if (!_sensorFeedbackOut.open(_sensorFeedbackOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sensorFeedbackOutName << std::endl;
      return false;
   }
   if (!_moveHeadOut.open(_moveHeadOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _moveHeadOutName << std::endl;
      return false;
   }
   if (!_fallingInterruptionOut.open(_fallingInterruptionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _fallingInterruptionOutName << std::endl;
      return false;
   }
   if (!_elicitedAttentionOut.open(_elicitedAttentionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _elicitedAttentionOutName << std::endl;
      return false;
   }

   /* create the handler port */
   _handlerPortName = "/";
   _handlerPortName += getName();

   if (!_handlerPort.open(_handlerPortName.c_str())){
      std::cout << getName() << ":unable to open port " << _handlerPortName << std::endl;
      return false;
   }
   
   attach(_handlerPort);

   /* Create the YarpInterface used to communicate between the main code in Controller and the other components through the thread class */
   _actuationDriverYarpInterface = new ActuationDriverYarpInterface();

   /* Create the main class: Controller */ 
   _actuationDriverController = new ActuationDriverController(_actuationDriverYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _actuationDriverThread = new ActuationDriverThread(&_disableRobotIn, &_enableRobotIn, &_gripIn, &_moveHandIn, &_moveHeadIn, &_moveSequenceIn, &_moveTorsoIn, &_pointAtIn, &_releaseIn, &_sayIn, &_sandtrayCommandIn, &_actionFeedbackIn, &_robotSensorsIn, &_eyeBlinkingOut, &_fallingReactionOut, &_fallingReactionSpeechOut, &_socialFacialExpressionOut, &_socialReactionOut, &_socialReactionSpeechOut, &_robotMotorFeedbackOut, &_selectedActionOut, &_sandtrayReturnOut, &_sensorFeedbackOut, &_moveHeadOut, &_fallingInterruptionOut, &_elicitedAttentionOut);
   _actuationDriverThread->setYarpInterface(_actuationDriverYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _actuationDriverYarpInterface->init(_actuationDriverThread, _actuationDriverController);
   
   _actuationDriverThread->start();

   return true;
}

bool ActuationDriver::interruptModule(){
   _disableRobotIn.interrupt();
   _enableRobotIn.interrupt();
   _gripIn.interrupt();
   _moveHandIn.interrupt();
   _moveHeadIn.interrupt();
   _moveSequenceIn.interrupt();
   _moveTorsoIn.interrupt();
   _pointAtIn.interrupt();
   _releaseIn.interrupt();
   _sayIn.interrupt();
   _sandtrayCommandIn.interrupt();
   _actionFeedbackIn.interrupt();
   _robotSensorsIn.interrupt();
   _eyeBlinkingOut.interrupt();
   _fallingReactionOut.interrupt();
   _fallingReactionSpeechOut.interrupt();
   _socialFacialExpressionOut.interrupt();
   _socialReactionOut.interrupt();
   _socialReactionSpeechOut.interrupt();
   _robotMotorFeedbackOut.interrupt();
   _selectedActionOut.interrupt();
   _sandtrayReturnOut.interrupt();
   _sensorFeedbackOut.interrupt();
   _moveHeadOut.interrupt();
   _fallingInterruptionOut.interrupt();
   _elicitedAttentionOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool ActuationDriver::close(){
   /* close the ports */
   _disableRobotIn.close();
   _enableRobotIn.close();
   _gripIn.close();
   _moveHandIn.close();
   _moveHeadIn.close();
   _moveSequenceIn.close();
   _moveTorsoIn.close();
   _pointAtIn.close();
   _releaseIn.close();
   _sayIn.close();
   _sandtrayCommandIn.close();
   _actionFeedbackIn.close();
   _robotSensorsIn.close();
   _eyeBlinkingOut.close();
   _fallingReactionOut.close();
   _fallingReactionSpeechOut.close();
   _socialFacialExpressionOut.close();
   _socialReactionOut.close();
   _socialReactionSpeechOut.close();
   _robotMotorFeedbackOut.close();
   _selectedActionOut.close();
   _sandtrayReturnOut.close();
   _sensorFeedbackOut.close();
   _moveHeadOut.close();
   _fallingInterruptionOut.close();
   _elicitedAttentionOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _actuationDriverThread->stop();

   return true;
}

bool ActuationDriver::respond(const Bottle &command, Bottle &reply){
   
	reply.clear();

	if (command.get(0).asString() == "help") {
		reply.addString("help: test | open | close | enable | disable | say | move | stand | grab | drop | fbImObjG |");
		reply.addString("fbImObjB | fbImMovG | fbImMovB | fbTTWaitG | fbTTWaitB | fbTTSortG | fbTTSortB | fbTTChosG |");
		reply.addString("fbTTChosB | fbTTPatG | fbTTPatB | fbJAG | fbJAB | robGood | point | lookPointSay | lookPoint0 |");
		reply.addString("lookPoint1 | lookPoint2 | look | blink | fallingSpeech | socialFacial | social | positiveSpeech |");
		reply.addString("negativeSpeech | elicitedAttention | autonomousLife | noAutonomousLife ");
	}
	else {
		reply.addString(command.get(0).asString() + " being executed...");

		if (command.get(0).asString() == "open") 					_actuationDriverController->testSelection(1, _ACTION_OPEN_HAND_,_RIGHT_ARM_);
		else if (command.get(0).asString() == "close") 				_actuationDriverController->testSelection(1, _ACTION_CLOSE_HAND_, _RIGHT_ARM_);
		else if (command.get(0).asString() == "enable") 			_actuationDriverController->testSelection(1, _ACTION_ENABLE_);
		else if (command.get(0).asString() == "disable") 			_actuationDriverController->testSelection(1, _ACTION_DISABLE_);
		else if (command.get(0).asString() == "say") 				_actuationDriverController->testSelection(1, _ACTION_SAY_, "test");
		else if (command.get(0).asString() == "move") 				_actuationDriverController->testSelection(1, _ACTION_MOVE_, "move");
		else if (command.get(0).asString() == "stand") 				_actuationDriverController->testSelection(1, _ACTION_MOVE_, _MOVE_STAND_);
		else if (command.get(0).asString() == "grab") 				_actuationDriverController->testSelection(1, _ACTION_GRAB_OBJECT_);
		else if (command.get(0).asString() == "drop") 				_actuationDriverController->testSelection(1, _ACTION_DROP_OBJECT_);
		else if (command.get(0).asString() == "fbImObjG") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_);
		else if (command.get(0).asString() == "fbImObjB") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_);
		else if (command.get(0).asString() == "fbImMovG") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_);
		else if (command.get(0).asString() == "fbImMovB") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_IMITATION_MOVE_BAD_);
		else if (command.get(0).asString() == "fbTTWaitG") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_TT_WAITING_GOOD_);
		else if (command.get(0).asString() == "fbTTWaitB") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_TT_WAITING_BAD_);
		else if (command.get(0).asString() == "fbTTSortG") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_TT_SORTING_GOOD_);
		else if (command.get(0).asString() == "fbTTSortB") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_TT_SORTING_BAD_);
		else if (command.get(0).asString() == "fbTTChosG") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_TT_CHOOSING_GOOD_);
		else if (command.get(0).asString() == "fbTTChosB") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_TT_CHOOSING_BAD_);
		else if (command.get(0).asString() == "fbTTPatG") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_TT_PATTERN_GOOD_);
		else if (command.get(0).asString() == "fbTTPatB") 			_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_TT_PATTERN_BAD_);
		else if (command.get(0).asString() == "fbJAG") 				_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_JA_GOOD_);
		else if (command.get(0).asString() == "fbJAB") 				_actuationDriverController->testSelection(1, _ACTION_FEEDBACK_JA_BAD_);
		else if (command.get(0).asString() == "robGood") 			_actuationDriverController->testSelection(1, _ACTION_ROBOT_GOOD_MOVE_);
		else if (command.get(0).asString() == "point") 				_actuationDriverController->testSelection(1, _ACTION_POINT_IMAGE_, 0);
		else if (command.get(0).asString() == "lookPointSay") 		_actuationDriverController->testSelection(1, _ACTION_LOOK_POINT_SAY_, 1);
		else if (command.get(0).asString() == "lookPoint0") 		_actuationDriverController->testSelection(1, _ACTION_LOOK_POINT_, 0);
		else if (command.get(0).asString() == "lookPoint1") 		_actuationDriverController->testSelection(1, _ACTION_LOOK_POINT_, 1);
		else if (command.get(0).asString() == "lookPoint2")			_actuationDriverController->testSelection(1, _ACTION_LOOK_POINT_, 2);
		else if (command.get(0).asString() == "look") 				_actuationDriverController->testSelection(1, _ACTION_LOOK_, 1);
		else if (command.get(0).asString() == "blink") 				_actuationDriverController->_yarpInterface->sendToEyeBlinking("Single", "Full", "2");
		else if (command.get(0).asString() == "fallingSpeech")		_actuationDriverController->_yarpInterface->sendToFallingReactionSpeech(0);
		else if (command.get(0).asString() == "socialFacial") 		_actuationDriverController->_yarpInterface->sendToSocialFacialExpression(4);
		else if (command.get(0).asString() == "social") 			_actuationDriverController->_yarpInterface->sendToSocialReaction(3);
		else if (command.get(0).asString() == "positiveSpeech")		_actuationDriverController->_yarpInterface->sendToSocialReactionSpeech(1);
		else if (command.get(0).asString() == "negativeSpeech")		_actuationDriverController->_yarpInterface->sendToSocialReactionSpeech(-1);
		else if (command.get(0).asString() == "elicitedAttention")	_actuationDriverController->_yarpInterface->sendToElicitedAttention(0.2, 0.5, 0.8);
		else if (command.get(0).asString() == "autonomousLife")		_actuationDriverController->_yarpInterface->sendToSocialReaction(0);
		else if (command.get(0).asString() == "noAutonomousLife")	_actuationDriverController->_yarpInterface->sendToSocialReaction(-1);
		else if (command.get(0).asString() == "test") 				_actuationDriverController->test();
		//else if (command.get(0).asString() == "actionFb") 		_actuationDriverController->sendToActionFeedback(1, 2, 1);
		//else if (command.get(0).asString() == "test") 			_actuationDriverController->testApp();
		else reply.addString("no command recognised");
	}
   return true;

}

bool ActuationDriver::updateModule(){
   return true;
}

double ActuationDriver::getPeriod(){
   return 0.1;
}
