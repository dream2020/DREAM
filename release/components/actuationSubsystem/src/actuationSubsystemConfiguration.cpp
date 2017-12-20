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

#include "actuationSubsystem.h"
#include "actuationSubsystemYarpInterface.h"
#include "actuationSubsystemController.h"

bool ActuationSubsystem::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("actuationSubsystem"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _eyeBlinkingInName = "/" +  getName(rf.check("_eyeBlinkingIn", Value("/eyeBlinking:i"), "Input eyeBlinking port (string)").asString());
   _fallingReactionInName = "/" +  getName(rf.check("_fallingReactionIn", Value("/fallingReaction:i"), "Input fallingReaction port (string)").asString());
   _fallingReactionSpeechInName = "/" +  getName(rf.check("_fallingReactionSpeechIn", Value("/fallingReactionSpeech:i"), "Input fallingReactionSpeech port (string)").asString());
   _socialFacialExpressionInName = "/" +  getName(rf.check("_socialFacialExpressionIn", Value("/socialFacialExpression:i"), "Input socialFacialExpression port (string)").asString());
   _socialReactionInName = "/" +  getName(rf.check("_socialReactionIn", Value("/socialReaction:i"), "Input socialReaction port (string)").asString());
   _socialReactionSpeechInName = "/" +  getName(rf.check("_socialReactionSpeechIn", Value("/socialReactionSpeech:i"), "Input socialReactionSpeech port (string)").asString());
   _fallingInterruptionInName = "/" +  getName(rf.check("_fallingInterruptionIn", Value("/fallingInterruption:i"), "Input fallingInterruption port (string)").asString());
   _elicitedAttentionInName = "/" +  getName(rf.check("_elicitedAttentionIn", Value("/elicitedAttention:i"), "Input elicitedAttention port (string)").asString());
   _robotMotorFeedbackInName = "/" +  getName(rf.check("_robotMotorFeedbackIn", Value("/robotMotorFeedback:i"), "Input robotMotorFeedback port (string)").asString());
   _selectedActionInName = "/" +  getName(rf.check("_selectedActionIn", Value("/selectedAction:i"), "Input selectedAction port (string)").asString());
   _sandtrayReturnInName = "/" +  getName(rf.check("_sandtrayReturnIn", Value("/sandtrayReturn:i"), "Input sandtrayReturn port (string)").asString());
   _sensorFeedbackInName = "/" +  getName(rf.check("_sensorFeedbackIn", Value("/sensorFeedback:i"), "Input sensorFeedback port (string)").asString());
   _emergencyInName = "/" +  getName(rf.check("_emergencyIn", Value("/emergency:i"), "Input emergency port (string)").asString());

   _robotSensorsOutName = "/" +  getName(rf.check("_robotSensorsOut", Value("/robotSensors:o"), "output robotSensors port (string)").asString());
   _disableRobotOutName = "/" +  getName(rf.check("_disableRobotOut", Value("/disableRobot:o"), "output disableRobot port (string)").asString());
   _enableRobotOutName = "/" +  getName(rf.check("_enableRobotOut", Value("/enableRobot:o"), "output enableRobot port (string)").asString());
   _gripOutName = "/" +  getName(rf.check("_gripOut", Value("/grip:o"), "output grip port (string)").asString());
   _moveHandOutName = "/" +  getName(rf.check("_moveHandOut", Value("/moveHand:o"), "output moveHand port (string)").asString());
   _moveHeadOutName = "/" +  getName(rf.check("_moveHeadOut", Value("/moveHead:o"), "output moveHead port (string)").asString());
   _moveSequenceOutName = "/" +  getName(rf.check("_moveSequenceOut", Value("/moveSequence:o"), "output moveSequence port (string)").asString());
   _moveTorsoOutName = "/" +  getName(rf.check("_moveTorsoOut", Value("/moveTorso:o"), "output moveTorso port (string)").asString());
   _pointAtOutName = "/" +  getName(rf.check("_pointAtOut", Value("/pointAt:o"), "output pointAt port (string)").asString());
   _releaseOutName = "/" +  getName(rf.check("_releaseOut", Value("/release:o"), "output release port (string)").asString());
   _sayOutName = "/" +  getName(rf.check("_sayOut", Value("/say:o"), "output say port (string)").asString());
   _sandtrayCommandOutName = "/" +  getName(rf.check("_sandtrayCommandOut", Value("/sandtrayCommand:o"), "output sandtrayCommand port (string)").asString());
   _actionFeedbackOutName = "/" +  getName(rf.check("_actionFeedbackOut", Value("/actionFeedback:o"), "output actionFeedback port (string)").asString());

   /* open the ports */
   if (!_eyeBlinkingIn.open(_eyeBlinkingInName.c_str())){
      std::cout << getName() << ": unable to open port " << _eyeBlinkingInName << std::endl;
      return false;
   }
   if (!_fallingReactionIn.open(_fallingReactionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _fallingReactionInName << std::endl;
      return false;
   }
   if (!_fallingReactionSpeechIn.open(_fallingReactionSpeechInName.c_str())){
      std::cout << getName() << ": unable to open port " << _fallingReactionSpeechInName << std::endl;
      return false;
   }
   if (!_socialFacialExpressionIn.open(_socialFacialExpressionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _socialFacialExpressionInName << std::endl;
      return false;
   }
   if (!_socialReactionIn.open(_socialReactionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _socialReactionInName << std::endl;
      return false;
   }
   if (!_socialReactionSpeechIn.open(_socialReactionSpeechInName.c_str())){
      std::cout << getName() << ": unable to open port " << _socialReactionSpeechInName << std::endl;
      return false;
   }
   if (!_fallingInterruptionIn.open(_fallingInterruptionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _fallingInterruptionInName << std::endl;
      return false;
   }
   if (!_elicitedAttentionIn.open(_elicitedAttentionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _elicitedAttentionInName << std::endl;
      return false;
   }
   if (!_robotMotorFeedbackIn.open(_robotMotorFeedbackInName.c_str())){
      std::cout << getName() << ": unable to open port " << _robotMotorFeedbackInName << std::endl;
      return false;
   }
   if (!_selectedActionIn.open(_selectedActionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _selectedActionInName << std::endl;
      return false;
   }
   if (!_sandtrayReturnIn.open(_sandtrayReturnInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sandtrayReturnInName << std::endl;
      return false;
   }
   if (!_sensorFeedbackIn.open(_sensorFeedbackInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sensorFeedbackInName << std::endl;
      return false;
   }
   if (!_emergencyIn.open(_emergencyInName.c_str())){
      std::cout << getName() << ": unable to open port " << _emergencyInName << std::endl;
      return false;
   }

   if (!_robotSensorsOut.open(_robotSensorsOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _robotSensorsOutName << std::endl;
      return false;
   }
   if (!_disableRobotOut.open(_disableRobotOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _disableRobotOutName << std::endl;
      return false;
   }
   if (!_enableRobotOut.open(_enableRobotOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _enableRobotOutName << std::endl;
      return false;
   }
   if (!_gripOut.open(_gripOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _gripOutName << std::endl;
      return false;
   }
   if (!_moveHandOut.open(_moveHandOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _moveHandOutName << std::endl;
      return false;
   }
   if (!_moveHeadOut.open(_moveHeadOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _moveHeadOutName << std::endl;
      return false;
   }
   if (!_moveSequenceOut.open(_moveSequenceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _moveSequenceOutName << std::endl;
      return false;
   }
   if (!_moveTorsoOut.open(_moveTorsoOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _moveTorsoOutName << std::endl;
      return false;
   }
   if (!_pointAtOut.open(_pointAtOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _pointAtOutName << std::endl;
      return false;
   }
   if (!_releaseOut.open(_releaseOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _releaseOutName << std::endl;
      return false;
   }
   if (!_sayOut.open(_sayOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sayOutName << std::endl;
      return false;
   }
   if (!_sandtrayCommandOut.open(_sandtrayCommandOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sandtrayCommandOutName << std::endl;
      return false;
   }
   if (!_actionFeedbackOut.open(_actionFeedbackOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _actionFeedbackOutName << std::endl;
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
   _actuationSubsystemYarpInterface = new ActuationSubsystemYarpInterface();

   /* Create the main class: Controller */ 
   _actuationSubsystemController = new ActuationSubsystemController(_actuationSubsystemYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _actuationSubsystemThread = new ActuationSubsystemThread(&_eyeBlinkingIn, &_fallingReactionIn, &_fallingReactionSpeechIn, &_socialFacialExpressionIn, &_socialReactionIn, &_socialReactionSpeechIn, &_fallingInterruptionIn, &_elicitedAttentionIn, &_robotMotorFeedbackIn, &_selectedActionIn, &_sandtrayReturnIn, &_sensorFeedbackIn, &_emergencyIn, &_robotSensorsOut, &_disableRobotOut, &_enableRobotOut, &_gripOut, &_moveHandOut, &_moveHeadOut, &_moveSequenceOut, &_moveTorsoOut, &_pointAtOut, &_releaseOut, &_sayOut, &_sandtrayCommandOut, &_actionFeedbackOut);
   _actuationSubsystemThread->setYarpInterface(_actuationSubsystemYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _actuationSubsystemYarpInterface->init(_actuationSubsystemThread, _actuationSubsystemController);
   
   _actuationSubsystemThread->start();

   return true;
}

bool ActuationSubsystem::interruptModule(){
   _eyeBlinkingIn.interrupt();
   _fallingReactionIn.interrupt();
   _fallingReactionSpeechIn.interrupt();
   _socialFacialExpressionIn.interrupt();
   _socialReactionIn.interrupt();
   _socialReactionSpeechIn.interrupt();
   _fallingInterruptionIn.interrupt();
   _elicitedAttentionIn.interrupt();
   _robotMotorFeedbackIn.interrupt();
   _selectedActionIn.interrupt();
   _sandtrayReturnIn.interrupt();
   _sensorFeedbackIn.interrupt();
   _emergencyIn.interrupt();
   _robotSensorsOut.interrupt();
   _disableRobotOut.interrupt();
   _enableRobotOut.interrupt();
   _gripOut.interrupt();
   _moveHandOut.interrupt();
   _moveHeadOut.interrupt();
   _moveSequenceOut.interrupt();
   _moveTorsoOut.interrupt();
   _pointAtOut.interrupt();
   _releaseOut.interrupt();
   _sayOut.interrupt();
   _sandtrayCommandOut.interrupt();
   _actionFeedbackOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool ActuationSubsystem::close(){
   /* close the ports */
   _eyeBlinkingIn.close();
   _fallingReactionIn.close();
   _fallingReactionSpeechIn.close();
   _socialFacialExpressionIn.close();
   _socialReactionIn.close();
   _socialReactionSpeechIn.close();
   _fallingInterruptionIn.close();
   _elicitedAttentionIn.close();
   _robotMotorFeedbackIn.close();
   _selectedActionIn.close();
   _sandtrayReturnIn.close();
   _sensorFeedbackIn.close();
   _emergencyIn.close();
   _robotSensorsOut.close();
   _disableRobotOut.close();
   _enableRobotOut.close();
   _gripOut.close();
   _moveHandOut.close();
   _moveHeadOut.close();
   _moveSequenceOut.close();
   _moveTorsoOut.close();
   _pointAtOut.close();
   _releaseOut.close();
   _sayOut.close();
   _sandtrayCommandOut.close();
   _actionFeedbackOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _actuationSubsystemThread->stop();

   return true;
}

bool ActuationSubsystem::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool ActuationSubsystem::updateModule(){
   return true;
}

double ActuationSubsystem::getPeriod(){
   return 0.1;
}
