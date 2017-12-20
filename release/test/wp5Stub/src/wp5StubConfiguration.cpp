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
  * 20/09/2016 Version 1.0: ... (Emmanuel Senft)
  */ 

#include "wp5Stub.h"
#include "wp5StubYarpInterface.h"
#include "wp5StubController.h"

bool Wp5Stub::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("wp5Stub"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _getInterventionStatusInName = "/" +  getName(rf.check("_getInterventionStatusIn", Value("/getInterventionStatus:i"), "Input getInterventionStatus port (string)").asString());
   _interactionEventInName = "/" +  getName(rf.check("_interactionEventIn", Value("/interactionEvent:i"), "Input interactionEvent port (string)").asString());
   _getBodyInName = "/" +  getName(rf.check("_getBodyIn", Value("/getBody:i"), "Input getBody port (string)").asString());
   _getBodyPoseInName = "/" +  getName(rf.check("_getBodyPoseIn", Value("/getBodyPose:i"), "Input getBodyPose port (string)").asString());
   _getEyeGazeInName = "/" +  getName(rf.check("_getEyeGazeIn", Value("/getEyeGaze:i"), "Input getEyeGaze port (string)").asString());
   _identifyTrajectoryInName = "/" +  getName(rf.check("_identifyTrajectoryIn", Value("/identifyTrajectory:i"), "Input identifyTrajectory port (string)").asString());
   _identifyVoiceInName = "/" +  getName(rf.check("_identifyVoiceIn", Value("/identifyVoice:i"), "Input identifyVoice port (string)").asString());
   _identifyFaceExpressionInName = "/" +  getName(rf.check("_identifyFaceExpressionIn", Value("/identifyFaceExpression:i"), "Input identifyFaceExpression port (string)").asString());
   _checkMutualGazeInName = "/" +  getName(rf.check("_checkMutualGazeIn", Value("/checkMutualGaze:i"), "Input checkMutualGaze port (string)").asString());

   _engagementFlagOutName = "/" +  getName(rf.check("_engagementFlagOut", Value("/engagementFlag:o"), "output engagementFlag port (string)").asString());
   _getChildBehaviourOutName = "/" +  getName(rf.check("_getChildBehaviourOut", Value("/getChildBehaviour:o"), "output getChildBehaviour port (string)").asString());
   _getChildPerformanceOutName = "/" +  getName(rf.check("_getChildPerformanceOut", Value("/getChildPerformance:o"), "output getChildPerformance port (string)").asString());
   _identifyTrajectoryOutName = "/" +  getName(rf.check("_identifyTrajectoryOut", Value("/identifyTrajectory:o"), "output identifyTrajectory port (string)").asString());

   /* open the ports */
   if (!_getInterventionStatusIn.open(_getInterventionStatusInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getInterventionStatusInName << std::endl;
      return false;
   }
   if (!_interactionEventIn.open(_interactionEventInName.c_str())){
      std::cout << getName() << ": unable to open port " << _interactionEventInName << std::endl;
      return false;
   }
   if (!_getBodyIn.open(_getBodyInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getBodyInName << std::endl;
      return false;
   }
   if (!_getBodyPoseIn.open(_getBodyPoseInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getBodyPoseInName << std::endl;
      return false;
   }
   if (!_getEyeGazeIn.open(_getEyeGazeInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getEyeGazeInName << std::endl;
      return false;
   }
   if (!_identifyTrajectoryIn.open(_identifyTrajectoryInName.c_str())){
      std::cout << getName() << ": unable to open port " << _identifyTrajectoryInName << std::endl;
      return false;
   }
   if (!_identifyVoiceIn.open(_identifyVoiceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _identifyVoiceInName << std::endl;
      return false;
   }
   if (!_identifyFaceExpressionIn.open(_identifyFaceExpressionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _identifyFaceExpressionInName << std::endl;
      return false;
   }
   if (!_checkMutualGazeIn.open(_checkMutualGazeInName.c_str())){
      std::cout << getName() << ": unable to open port " << _checkMutualGazeInName << std::endl;
      return false;
   }

   if (!_engagementFlagOut.open(_engagementFlagOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _engagementFlagOutName << std::endl;
      return false;
   }
   if (!_getChildBehaviourOut.open(_getChildBehaviourOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getChildBehaviourOutName << std::endl;
      return false;
   }
   if (!_getChildPerformanceOut.open(_getChildPerformanceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getChildPerformanceOutName << std::endl;
      return false;
   }
   if (!_identifyTrajectoryOut.open(_identifyTrajectoryOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _identifyTrajectoryOutName << std::endl;
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
   _wp5StubYarpInterface = new Wp5StubYarpInterface();

   /* Create the main class: Controller */ 
   _wp5StubController = new Wp5StubController(_wp5StubYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _wp5StubThread = new Wp5StubThread(&_getInterventionStatusIn, &_interactionEventIn, &_getBodyIn, &_getBodyPoseIn, &_getEyeGazeIn, &_identifyTrajectoryIn, &_identifyVoiceIn, &_identifyFaceExpressionIn, &_checkMutualGazeIn, &_engagementFlagOut, &_getChildBehaviourOut, &_getChildPerformanceOut, &_identifyTrajectoryOut);
   _wp5StubThread->setYarpInterface(_wp5StubYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _wp5StubYarpInterface->init(_wp5StubThread, _wp5StubController);
   
   _wp5StubThread->start();

   return true;
}

bool Wp5Stub::interruptModule(){
   _getInterventionStatusIn.interrupt();
   _interactionEventIn.interrupt();
   _getBodyIn.interrupt();
   _getBodyPoseIn.interrupt();
   _getEyeGazeIn.interrupt();
   _identifyTrajectoryIn.interrupt();
   _identifyVoiceIn.interrupt();
   _identifyFaceExpressionIn.interrupt();
   _checkMutualGazeIn.interrupt();
   _engagementFlagOut.interrupt();
   _getChildBehaviourOut.interrupt();
   _getChildPerformanceOut.interrupt();
   _identifyTrajectoryOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool Wp5Stub::close(){
   /* close the ports */
   _getInterventionStatusIn.close();
   _interactionEventIn.close();
   _getBodyIn.close();
   _getBodyPoseIn.close();
   _getEyeGazeIn.close();
   _identifyTrajectoryIn.close();
   _identifyVoiceIn.close();
   _identifyFaceExpressionIn.close();
   _checkMutualGazeIn.close();
   _engagementFlagOut.close();
   _getChildBehaviourOut.close();
   _getChildPerformanceOut.close();
   _identifyTrajectoryOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _wp5StubThread->stop();

   return true;
}

bool Wp5Stub::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool Wp5Stub::updateModule(){
   return true;
}

double Wp5Stub::getPeriod(){
   return 0.1;
}
