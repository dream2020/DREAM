/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Emmanuel Senft, PLYM 
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

#include "wp3Stub.h"
#include "wp3StubYarpInterface.h"
#include "wp3StubController.h"

bool Wp3Stub::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("wp3Stub"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _getEyeGazeInName = "/" +  getName(rf.check("_getEyeGazeIn", Value("/getEyeGaze:i"), "Input getEyeGaze port (string)").asString());
   _getGripLocationInName = "/" +  getName(rf.check("_getGripLocationIn", Value("/getGripLocation:i"), "Input getGripLocation port (string)").asString());
   _getHeadGazeInName = "/" +  getName(rf.check("_getHeadGazeIn", Value("/getHeadGaze:i"), "Input getHeadGaze port (string)").asString());
   _getObjectTableDistanceInName = "/" +  getName(rf.check("_getObjectTableDistanceIn", Value("/getObjectTableDistance:i"), "Input getObjectTableDistance port (string)").asString());
   _getObjectsInName = "/" +  getName(rf.check("_getObjectsIn", Value("/getObjects:i"), "Input getObjects port (string)").asString());
   _identifyFaceInName = "/" +  getName(rf.check("_identifyFaceIn", Value("/identifyFace:i"), "Input identifyFace port (string)").asString());
   _identifyFaceExpressionInName = "/" +  getName(rf.check("_identifyFaceExpressionIn", Value("/identifyFaceExpression:i"), "Input identifyFaceExpression port (string)").asString());
   _identifyObjectInName = "/" +  getName(rf.check("_identifyObjectIn", Value("/identifyObject:i"), "Input identifyObject port (string)").asString());
   _identifyTrajectoryInName = "/" +  getName(rf.check("_identifyTrajectoryIn", Value("/identifyTrajectory:i"), "Input identifyTrajectory port (string)").asString());
   _trackFaceInName = "/" +  getName(rf.check("_trackFaceIn", Value("/trackFace:i"), "Input trackFace port (string)").asString());
   _trackObjectInName = "/" +  getName(rf.check("_trackObjectIn", Value("/trackObject:i"), "Input trackObject port (string)").asString());
   _trackHandInName = "/" +  getName(rf.check("_trackHandIn", Value("/trackHand:i"), "Input trackHand port (string)").asString());
   _getSoundDirectionInName = "/" +  getName(rf.check("_getSoundDirectionIn", Value("/getSoundDirection:i"), "Input getSoundDirection port (string)").asString());

   _checkMutualGazeOutName = "/" +  getName(rf.check("_checkMutualGazeOut", Value("/checkMutualGaze:o"), "output checkMutualGaze port (string)").asString());
   _getFacesOutName = "/" +  getName(rf.check("_getFacesOut", Value("/getFaces:o"), "output getFaces port (string)").asString());
   _getSoundDirectionOutName = "/" +  getName(rf.check("_getSoundDirectionOut", Value("/getSoundDirection:o"), "output getSoundDirection port (string)").asString());
   _identifyFaceExpressionOutName = "/" +  getName(rf.check("_identifyFaceExpressionOut", Value("/identifyFaceExpression:o"), "output identifyFaceExpression port (string)").asString());
   _recognizeSpeechOutName = "/" +  getName(rf.check("_recognizeSpeechOut", Value("/recognizeSpeech:o"), "output recognizeSpeech port (string)").asString());
   _getEyeGazeOutName = "/" +  getName(rf.check("_getEyeGazeOut", Value("/getEyeGaze:o"), "output getEyeGaze port (string)").asString());
   _getEyesOutName = "/" +  getName(rf.check("_getEyesOut", Value("/getEyes:o"), "output getEyes port (string)").asString());
   _getArmAngleOutName = "/" +  getName(rf.check("_getArmAngleOut", Value("/getArmAngle:o"), "output getArmAngle port (string)").asString());
   _getBodyOutName = "/" +  getName(rf.check("_getBodyOut", Value("/getBody:o"), "output getBody port (string)").asString());
   _getGripLocationOutName = "/" +  getName(rf.check("_getGripLocationOut", Value("/getGripLocation:o"), "output getGripLocation port (string)").asString());
   _getBodyPoseOutName = "/" +  getName(rf.check("_getBodyPoseOut", Value("/getBodyPose:o"), "output getBodyPose port (string)").asString());
   _getHandsOutName = "/" +  getName(rf.check("_getHandsOut", Value("/getHands:o"), "output getHands port (string)").asString());
   _getHeadOutName = "/" +  getName(rf.check("_getHeadOut", Value("/getHead:o"), "output getHead port (string)").asString());
   _getHeadGazeOutName = "/" +  getName(rf.check("_getHeadGazeOut", Value("/getHeadGaze:o"), "output getHeadGaze port (string)").asString());
   _getObjectsOutName = "/" +  getName(rf.check("_getObjectsOut", Value("/getObjects:o"), "output getObjects port (string)").asString());
   _getObjectTableDistanceOutName = "/" +  getName(rf.check("_getObjectTableDistanceOut", Value("/getObjectTableDistance:o"), "output getObjectTableDistance port (string)").asString());
   _identifyFaceOutName = "/" +  getName(rf.check("_identifyFaceOut", Value("/identifyFace:o"), "output identifyFace port (string)").asString());
   _identifyObjectOutName = "/" +  getName(rf.check("_identifyObjectOut", Value("/identifyObject:o"), "output identifyObject port (string)").asString());
   _identifyTrajectoryOutName = "/" +  getName(rf.check("_identifyTrajectoryOut", Value("/identifyTrajectory:o"), "output identifyTrajectory port (string)").asString());
   _identifyVoiceOutName = "/" +  getName(rf.check("_identifyVoiceOut", Value("/identifyVoice:o"), "output identifyVoice port (string)").asString());
   _trackFaceOutName = "/" +  getName(rf.check("_trackFaceOut", Value("/trackFace:o"), "output trackFace port (string)").asString());
   _trackObjectOutName = "/" +  getName(rf.check("_trackObjectOut", Value("/trackObject:o"), "output trackObject port (string)").asString());
   _getHeadGazePointOutName = "/" +  getName(rf.check("_getHeadGazePointOut", Value("/getHeadGazePoint:o"), "output getHeadGazePoint port (string)").asString());

   /* open the ports */
   if (!_getEyeGazeIn.open(_getEyeGazeInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getEyeGazeInName << std::endl;
      return false;
   }
   if (!_getGripLocationIn.open(_getGripLocationInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getGripLocationInName << std::endl;
      return false;
   }
   if (!_getHeadGazeIn.open(_getHeadGazeInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getHeadGazeInName << std::endl;
      return false;
   }
   if (!_getObjectTableDistanceIn.open(_getObjectTableDistanceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getObjectTableDistanceInName << std::endl;
      return false;
   }
   if (!_getObjectsIn.open(_getObjectsInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getObjectsInName << std::endl;
      return false;
   }
   if (!_identifyFaceIn.open(_identifyFaceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _identifyFaceInName << std::endl;
      return false;
   }
   if (!_identifyFaceExpressionIn.open(_identifyFaceExpressionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _identifyFaceExpressionInName << std::endl;
      return false;
   }
   if (!_identifyObjectIn.open(_identifyObjectInName.c_str())){
      std::cout << getName() << ": unable to open port " << _identifyObjectInName << std::endl;
      return false;
   }
   if (!_identifyTrajectoryIn.open(_identifyTrajectoryInName.c_str())){
      std::cout << getName() << ": unable to open port " << _identifyTrajectoryInName << std::endl;
      return false;
   }
   if (!_trackFaceIn.open(_trackFaceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _trackFaceInName << std::endl;
      return false;
   }
   if (!_trackObjectIn.open(_trackObjectInName.c_str())){
      std::cout << getName() << ": unable to open port " << _trackObjectInName << std::endl;
      return false;
   }
   if (!_trackHandIn.open(_trackHandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _trackHandInName << std::endl;
      return false;
   }
   if (!_getSoundDirectionIn.open(_getSoundDirectionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getSoundDirectionInName << std::endl;
      return false;
   }

   if (!_checkMutualGazeOut.open(_checkMutualGazeOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _checkMutualGazeOutName << std::endl;
      return false;
   }
   if (!_getFacesOut.open(_getFacesOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getFacesOutName << std::endl;
      return false;
   }
   if (!_getSoundDirectionOut.open(_getSoundDirectionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getSoundDirectionOutName << std::endl;
      return false;
   }
   if (!_identifyFaceExpressionOut.open(_identifyFaceExpressionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _identifyFaceExpressionOutName << std::endl;
      return false;
   }
   if (!_recognizeSpeechOut.open(_recognizeSpeechOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _recognizeSpeechOutName << std::endl;
      return false;
   }
   if (!_getEyeGazeOut.open(_getEyeGazeOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getEyeGazeOutName << std::endl;
      return false;
   }
   if (!_getEyesOut.open(_getEyesOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getEyesOutName << std::endl;
      return false;
   }
   if (!_getArmAngleOut.open(_getArmAngleOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getArmAngleOutName << std::endl;
      return false;
   }
   if (!_getBodyOut.open(_getBodyOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getBodyOutName << std::endl;
      return false;
   }
   if (!_getGripLocationOut.open(_getGripLocationOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getGripLocationOutName << std::endl;
      return false;
   }
   if (!_getBodyPoseOut.open(_getBodyPoseOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getBodyPoseOutName << std::endl;
      return false;
   }
   if (!_getHandsOut.open(_getHandsOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getHandsOutName << std::endl;
      return false;
   }
   if (!_getHeadOut.open(_getHeadOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getHeadOutName << std::endl;
      return false;
   }
   if (!_getHeadGazeOut.open(_getHeadGazeOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getHeadGazeOutName << std::endl;
      return false;
   }
   if (!_getObjectsOut.open(_getObjectsOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getObjectsOutName << std::endl;
      return false;
   }
   if (!_getObjectTableDistanceOut.open(_getObjectTableDistanceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getObjectTableDistanceOutName << std::endl;
      return false;
   }
   if (!_identifyFaceOut.open(_identifyFaceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _identifyFaceOutName << std::endl;
      return false;
   }
   if (!_identifyObjectOut.open(_identifyObjectOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _identifyObjectOutName << std::endl;
      return false;
   }
   if (!_identifyTrajectoryOut.open(_identifyTrajectoryOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _identifyTrajectoryOutName << std::endl;
      return false;
   }
   if (!_identifyVoiceOut.open(_identifyVoiceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _identifyVoiceOutName << std::endl;
      return false;
   }
   if (!_trackFaceOut.open(_trackFaceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _trackFaceOutName << std::endl;
      return false;
   }
   if (!_trackObjectOut.open(_trackObjectOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _trackObjectOutName << std::endl;
      return false;
   }
   if (!_getHeadGazePointOut.open(_getHeadGazePointOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getHeadGazePointOutName << std::endl;
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
   _wp3StubYarpInterface = new Wp3StubYarpInterface();

   /* Create the main class: Controller */ 
   _wp3StubController = new Wp3StubController(_wp3StubYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _wp3StubThread = new Wp3StubThread(&_getEyeGazeIn, &_getGripLocationIn, &_getHeadGazeIn, &_getObjectTableDistanceIn, &_getObjectsIn, &_identifyFaceIn, &_identifyFaceExpressionIn, &_identifyObjectIn, &_identifyTrajectoryIn, &_trackFaceIn, &_trackObjectIn, &_trackHandIn, &_getSoundDirectionIn, &_checkMutualGazeOut, &_getFacesOut, &_getSoundDirectionOut, &_identifyFaceExpressionOut, &_recognizeSpeechOut, &_getEyeGazeOut, &_getEyesOut, &_getArmAngleOut, &_getBodyOut, &_getGripLocationOut, &_getBodyPoseOut, &_getHandsOut, &_getHeadOut, &_getHeadGazeOut, &_getObjectsOut, &_getObjectTableDistanceOut, &_identifyFaceOut, &_identifyObjectOut, &_identifyTrajectoryOut, &_identifyVoiceOut, &_trackFaceOut, &_trackObjectOut, &_getHeadGazePointOut);
   _wp3StubThread->setYarpInterface(_wp3StubYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _wp3StubYarpInterface->init(_wp3StubThread, _wp3StubController);
   
   _wp3StubThread->start();

   return true;
}

bool Wp3Stub::interruptModule(){
   _getEyeGazeIn.interrupt();
   _getGripLocationIn.interrupt();
   _getHeadGazeIn.interrupt();
   _getObjectTableDistanceIn.interrupt();
   _getObjectsIn.interrupt();
   _identifyFaceIn.interrupt();
   _identifyFaceExpressionIn.interrupt();
   _identifyObjectIn.interrupt();
   _identifyTrajectoryIn.interrupt();
   _trackFaceIn.interrupt();
   _trackObjectIn.interrupt();
   _trackHandIn.interrupt();
   _getSoundDirectionIn.interrupt();
   _checkMutualGazeOut.interrupt();
   _getFacesOut.interrupt();
   _getSoundDirectionOut.interrupt();
   _identifyFaceExpressionOut.interrupt();
   _recognizeSpeechOut.interrupt();
   _getEyeGazeOut.interrupt();
   _getEyesOut.interrupt();
   _getArmAngleOut.interrupt();
   _getBodyOut.interrupt();
   _getGripLocationOut.interrupt();
   _getBodyPoseOut.interrupt();
   _getHandsOut.interrupt();
   _getHeadOut.interrupt();
   _getHeadGazeOut.interrupt();
   _getObjectsOut.interrupt();
   _getObjectTableDistanceOut.interrupt();
   _identifyFaceOut.interrupt();
   _identifyObjectOut.interrupt();
   _identifyTrajectoryOut.interrupt();
   _identifyVoiceOut.interrupt();
   _trackFaceOut.interrupt();
   _trackObjectOut.interrupt();
   _getHeadGazePointOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool Wp3Stub::close(){
   /* close the ports */
   _getEyeGazeIn.close();
   _getGripLocationIn.close();
   _getHeadGazeIn.close();
   _getObjectTableDistanceIn.close();
   _getObjectsIn.close();
   _identifyFaceIn.close();
   _identifyFaceExpressionIn.close();
   _identifyObjectIn.close();
   _identifyTrajectoryIn.close();
   _trackFaceIn.close();
   _trackObjectIn.close();
   _trackHandIn.close();
   _getSoundDirectionIn.close();
   _checkMutualGazeOut.close();
   _getFacesOut.close();
   _getSoundDirectionOut.close();
   _identifyFaceExpressionOut.close();
   _recognizeSpeechOut.close();
   _getEyeGazeOut.close();
   _getEyesOut.close();
   _getArmAngleOut.close();
   _getBodyOut.close();
   _getGripLocationOut.close();
   _getBodyPoseOut.close();
   _getHandsOut.close();
   _getHeadOut.close();
   _getHeadGazeOut.close();
   _getObjectsOut.close();
   _getObjectTableDistanceOut.close();
   _identifyFaceOut.close();
   _identifyObjectOut.close();
   _identifyTrajectoryOut.close();
   _identifyVoiceOut.close();
   _trackFaceOut.close();
   _trackObjectOut.close();
   _getHeadGazePointOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _wp3StubThread->stop();

   return true;
}

bool Wp3Stub::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool Wp3Stub::updateModule(){
   return true;
}

double Wp3Stub::getPeriod(){
   return 0.1;
}
