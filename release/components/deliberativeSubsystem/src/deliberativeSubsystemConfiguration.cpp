/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  James Kennedy, Plymouth University 
 * Email:   james.kennedy@plymouth.ac.uk 
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
  * 21/02/2017 Version 1.0: ... (James Kennedy)
  */ 

#include "deliberativeSubsystem.h"
#include "deliberativeSubsystemYarpInterface.h"
#include "deliberativeSubsystemController.h"

bool DeliberativeSubsystem::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("deliberativeSubsystem"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _getChildBehaviourInName = "/" +  getName(rf.check("_getChildBehaviourIn", Value("/getChildBehaviour:i"), "Input getChildBehaviour port (string)").asString());
   _getChildPerformanceInName = "/" +  getName(rf.check("_getChildPerformanceIn", Value("/getChildPerformance:i"), "Input getChildPerformance port (string)").asString());
   _checkMutualGazeInName = "/" +  getName(rf.check("_checkMutualGazeIn", Value("/checkMutualGaze:i"), "Input checkMutualGaze port (string)").asString());
   _getArmAngleInName = "/" +  getName(rf.check("_getArmAngleIn", Value("/getArmAngle:i"), "Input getArmAngle port (string)").asString());
   _getBodyInName = "/" +  getName(rf.check("_getBodyIn", Value("/getBody:i"), "Input getBody port (string)").asString());
   _getBodyPoseInName = "/" +  getName(rf.check("_getBodyPoseIn", Value("/getBodyPose:i"), "Input getBodyPose port (string)").asString());
   _getEyeGazeInName = "/" +  getName(rf.check("_getEyeGazeIn", Value("/getEyeGaze:i"), "Input getEyeGaze port (string)").asString());
   _getEyesInName = "/" +  getName(rf.check("_getEyesIn", Value("/getEyes:i"), "Input getEyes port (string)").asString());
   _getFacesInName = "/" +  getName(rf.check("_getFacesIn", Value("/getFaces:i"), "Input getFaces port (string)").asString());
   _getGripLocationInName = "/" +  getName(rf.check("_getGripLocationIn", Value("/getGripLocation:i"), "Input getGripLocation port (string)").asString());
   _getHandsInName = "/" +  getName(rf.check("_getHandsIn", Value("/getHands:i"), "Input getHands port (string)").asString());
   _getHeadInName = "/" +  getName(rf.check("_getHeadIn", Value("/getHead:i"), "Input getHead port (string)").asString());
   _getHeadGazeInName = "/" +  getName(rf.check("_getHeadGazeIn", Value("/getHeadGaze:i"), "Input getHeadGaze port (string)").asString());
   _getObjectsInName = "/" +  getName(rf.check("_getObjectsIn", Value("/getObjects:i"), "Input getObjects port (string)").asString());
   _getObjectTableDistanceInName = "/" +  getName(rf.check("_getObjectTableDistanceIn", Value("/getObjectTableDistance:i"), "Input getObjectTableDistance port (string)").asString());
   _getSoundDirectionInName = "/" +  getName(rf.check("_getSoundDirectionIn", Value("/getSoundDirection:i"), "Input getSoundDirection port (string)").asString());
   _identifyFaceInName = "/" +  getName(rf.check("_identifyFaceIn", Value("/identifyFace:i"), "Input identifyFace port (string)").asString());
   _identifyFaceExpressionInName = "/" +  getName(rf.check("_identifyFaceExpressionIn", Value("/identifyFaceExpression:i"), "Input identifyFaceExpression port (string)").asString());
   _identifyObjectInName = "/" +  getName(rf.check("_identifyObjectIn", Value("/identifyObject:i"), "Input identifyObject port (string)").asString());
   _identifyTrajectoryInName = "/" +  getName(rf.check("_identifyTrajectoryIn", Value("/identifyTrajectory:i"), "Input identifyTrajectory port (string)").asString());
   _identifyVoiceInName = "/" +  getName(rf.check("_identifyVoiceIn", Value("/identifyVoice:i"), "Input identifyVoice port (string)").asString());
   _recognizeSpeechInName = "/" +  getName(rf.check("_recognizeSpeechIn", Value("/recognizeSpeech:i"), "Input recognizeSpeech port (string)").asString());
   _trackFaceInName = "/" +  getName(rf.check("_trackFaceIn", Value("/trackFace:i"), "Input trackFace port (string)").asString());
   _trackObjectInName = "/" +  getName(rf.check("_trackObjectIn", Value("/trackObject:i"), "Input trackObject port (string)").asString());
   _actionFeedbackInName = "/" +  getName(rf.check("_actionFeedbackIn", Value("/actionFeedback:i"), "Input actionFeedback port (string)").asString());
   _interventionCommandInName = "/" +  getName(rf.check("_interventionCommandIn", Value("/interventionCommand:i"), "Input interventionCommand port (string)").asString());
   _selectedActionInName = "/" +  getName(rf.check("_selectedActionIn", Value("/selectedAction:i"), "Input selectedAction port (string)").asString());
   _userDelibInName = "/" +  getName(rf.check("_userDelibIn", Value("/userDelib:i"), "Input userDelib port (string)").asString());
   _sandtrayEventInName = "/" +  getName(rf.check("_sandtrayEventIn", Value("/sandtrayEvent:i"), "Input sandtrayEvent port (string)").asString());
   _sandtrayReturnInName = "/" +  getName(rf.check("_sandtrayReturnIn", Value("/sandtrayReturn:i"), "Input sandtrayReturn port (string)").asString());
   _robotSensorsInName = "/" +  getName(rf.check("_robotSensorsIn", Value("/robotSensors:i"), "Input robotSensors port (string)").asString());
   _emergencyInName = "/" +  getName(rf.check("_emergencyIn", Value("/emergency:i"), "Input emergency port (string)").asString());

   _getEyeGazeOutName = "/" +  getName(rf.check("_getEyeGazeOut", Value("/getEyeGaze:o"), "output getEyeGaze port (string)").asString());
   _getGripLocationOutName = "/" +  getName(rf.check("_getGripLocationOut", Value("/getGripLocation:o"), "output getGripLocation port (string)").asString());
   _getHeadGazeOutName = "/" +  getName(rf.check("_getHeadGazeOut", Value("/getHeadGaze:o"), "output getHeadGaze port (string)").asString());
   _getObjectsOutName = "/" +  getName(rf.check("_getObjectsOut", Value("/getObjects:o"), "output getObjects port (string)").asString());
   _getObjectTableDistanceOutName = "/" +  getName(rf.check("_getObjectTableDistanceOut", Value("/getObjectTableDistance:o"), "output getObjectTableDistance port (string)").asString());
   _getSoundDirectionOutName = "/" +  getName(rf.check("_getSoundDirectionOut", Value("/getSoundDirection:o"), "output getSoundDirection port (string)").asString());
   _identifyFaceOutName = "/" +  getName(rf.check("_identifyFaceOut", Value("/identifyFace:o"), "output identifyFace port (string)").asString());
   _identifyFaceExpressionOutName = "/" +  getName(rf.check("_identifyFaceExpressionOut", Value("/identifyFaceExpression:o"), "output identifyFaceExpression port (string)").asString());
   _identifyObjectOutName = "/" +  getName(rf.check("_identifyObjectOut", Value("/identifyObject:o"), "output identifyObject port (string)").asString());
   _identifyTrajectoryOutName = "/" +  getName(rf.check("_identifyTrajectoryOut", Value("/identifyTrajectory:o"), "output identifyTrajectory port (string)").asString());
   _trackFaceOutName = "/" +  getName(rf.check("_trackFaceOut", Value("/trackFace:o"), "output trackFace port (string)").asString());
   _trackHandOutName = "/" +  getName(rf.check("_trackHandOut", Value("/trackHand:o"), "output trackHand port (string)").asString());
   _trackObjectOutName = "/" +  getName(rf.check("_trackObjectOut", Value("/trackObject:o"), "output trackObject port (string)").asString());
   _interactionEventOutName = "/" +  getName(rf.check("_interactionEventOut", Value("/interactionEvent:o"), "output interactionEvent port (string)").asString());
   _getInterventionStatusOutName = "/" +  getName(rf.check("_getInterventionStatusOut", Value("/getInterventionStatus:o"), "output getInterventionStatus port (string)").asString());
   _attentionBiasOutName = "/" +  getName(rf.check("_attentionBiasOut", Value("/attentionBias:o"), "output attentionBias port (string)").asString());
   _commandSuccessOutName = "/" +  getName(rf.check("_commandSuccessOut", Value("/commandSuccess:o"), "output commandSuccess port (string)").asString());
   _startStopOutName = "/" +  getName(rf.check("_startStopOut", Value("/startStop:o"), "output startStop port (string)").asString());
   _deliberativeFeedbackOutName = "/" +  getName(rf.check("_deliberativeFeedbackOut", Value("/deliberativeFeedback:o"), "output deliberativeFeedback port (string)").asString());
   _sensorySummaryOutName = "/" +  getName(rf.check("_sensorySummaryOut", Value("/sensorySummary:o"), "output sensorySummary port (string)").asString());
   _sandtrayCommandOutName = "/" +  getName(rf.check("_sandtrayCommandOut", Value("/sandtrayCommand:o"), "output sandtrayCommand port (string)").asString());
   _suggestedActionOutName = "/" +  getName(rf.check("_suggestedActionOut", Value("/suggestedAction:o"), "output suggestedAction port (string)").asString());

   /* open the ports */
   if (!_getChildBehaviourIn.open(_getChildBehaviourInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getChildBehaviourInName << std::endl;
      return false;
   }
   if (!_getChildPerformanceIn.open(_getChildPerformanceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getChildPerformanceInName << std::endl;
      return false;
   }
   if (!_checkMutualGazeIn.open(_checkMutualGazeInName.c_str())){
      std::cout << getName() << ": unable to open port " << _checkMutualGazeInName << std::endl;
      return false;
   }
   if (!_getArmAngleIn.open(_getArmAngleInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getArmAngleInName << std::endl;
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
   if (!_getEyesIn.open(_getEyesInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getEyesInName << std::endl;
      return false;
   }
   if (!_getFacesIn.open(_getFacesInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getFacesInName << std::endl;
      return false;
   }
   if (!_getGripLocationIn.open(_getGripLocationInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getGripLocationInName << std::endl;
      return false;
   }
   if (!_getHandsIn.open(_getHandsInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getHandsInName << std::endl;
      return false;
   }
   if (!_getHeadIn.open(_getHeadInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getHeadInName << std::endl;
      return false;
   }
   if (!_getHeadGazeIn.open(_getHeadGazeInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getHeadGazeInName << std::endl;
      return false;
   }
   if (!_getObjectsIn.open(_getObjectsInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getObjectsInName << std::endl;
      return false;
   }
   if (!_getObjectTableDistanceIn.open(_getObjectTableDistanceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getObjectTableDistanceInName << std::endl;
      return false;
   }
   if (!_getSoundDirectionIn.open(_getSoundDirectionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getSoundDirectionInName << std::endl;
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
   if (!_identifyVoiceIn.open(_identifyVoiceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _identifyVoiceInName << std::endl;
      return false;
   }
   if (!_recognizeSpeechIn.open(_recognizeSpeechInName.c_str())){
      std::cout << getName() << ": unable to open port " << _recognizeSpeechInName << std::endl;
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
   if (!_actionFeedbackIn.open(_actionFeedbackInName.c_str())){
      std::cout << getName() << ": unable to open port " << _actionFeedbackInName << std::endl;
      return false;
   }
   if (!_interventionCommandIn.open(_interventionCommandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _interventionCommandInName << std::endl;
      return false;
   }
   if (!_selectedActionIn.open(_selectedActionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _selectedActionInName << std::endl;
      return false;
   }
   if (!_userDelibIn.open(_userDelibInName.c_str())){
      std::cout << getName() << ": unable to open port " << _userDelibInName << std::endl;
      return false;
   }
   if (!_sandtrayEventIn.open(_sandtrayEventInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sandtrayEventInName << std::endl;
      return false;
   }
   if (!_sandtrayReturnIn.open(_sandtrayReturnInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sandtrayReturnInName << std::endl;
      return false;
   }
   if (!_robotSensorsIn.open(_robotSensorsInName.c_str())){
      std::cout << getName() << ": unable to open port " << _robotSensorsInName << std::endl;
      return false;
   }
   if (!_emergencyIn.open(_emergencyInName.c_str())){
      std::cout << getName() << ": unable to open port " << _emergencyInName << std::endl;
      return false;
   }

   if (!_getEyeGazeOut.open(_getEyeGazeOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getEyeGazeOutName << std::endl;
      return false;
   }
   if (!_getGripLocationOut.open(_getGripLocationOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getGripLocationOutName << std::endl;
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
   if (!_getSoundDirectionOut.open(_getSoundDirectionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getSoundDirectionOutName << std::endl;
      return false;
   }
   if (!_identifyFaceOut.open(_identifyFaceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _identifyFaceOutName << std::endl;
      return false;
   }
   if (!_identifyFaceExpressionOut.open(_identifyFaceExpressionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _identifyFaceExpressionOutName << std::endl;
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
   if (!_trackFaceOut.open(_trackFaceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _trackFaceOutName << std::endl;
      return false;
   }
   if (!_trackHandOut.open(_trackHandOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _trackHandOutName << std::endl;
      return false;
   }
   if (!_trackObjectOut.open(_trackObjectOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _trackObjectOutName << std::endl;
      return false;
   }
   if (!_interactionEventOut.open(_interactionEventOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _interactionEventOutName << std::endl;
      return false;
   }
   if (!_getInterventionStatusOut.open(_getInterventionStatusOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getInterventionStatusOutName << std::endl;
      return false;
   }
   if (!_attentionBiasOut.open(_attentionBiasOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _attentionBiasOutName << std::endl;
      return false;
   }
   if (!_commandSuccessOut.open(_commandSuccessOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _commandSuccessOutName << std::endl;
      return false;
   }
   if (!_startStopOut.open(_startStopOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _startStopOutName << std::endl;
      return false;
   }
   if (!_deliberativeFeedbackOut.open(_deliberativeFeedbackOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _deliberativeFeedbackOutName << std::endl;
      return false;
   }
   if (!_sensorySummaryOut.open(_sensorySummaryOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sensorySummaryOutName << std::endl;
      return false;
   }
   if (!_sandtrayCommandOut.open(_sandtrayCommandOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sandtrayCommandOutName << std::endl;
      return false;
   }
   if (!_suggestedActionOut.open(_suggestedActionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _suggestedActionOutName << std::endl;
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
   _deliberativeSubsystemYarpInterface = new DeliberativeSubsystemYarpInterface();

   /* Create the main class: Controller */ 
   _deliberativeSubsystemController = new DeliberativeSubsystemController(_deliberativeSubsystemYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _deliberativeSubsystemThread = new DeliberativeSubsystemThread(&_getChildBehaviourIn, &_getChildPerformanceIn, &_checkMutualGazeIn, &_getArmAngleIn, &_getBodyIn, &_getBodyPoseIn, &_getEyeGazeIn, &_getEyesIn, &_getFacesIn, &_getGripLocationIn, &_getHandsIn, &_getHeadIn, &_getHeadGazeIn, &_getObjectsIn, &_getObjectTableDistanceIn, &_getSoundDirectionIn, &_identifyFaceIn, &_identifyFaceExpressionIn, &_identifyObjectIn, &_identifyTrajectoryIn, &_identifyVoiceIn, &_recognizeSpeechIn, &_trackFaceIn, &_trackObjectIn, &_actionFeedbackIn, &_interventionCommandIn, &_selectedActionIn, &_userDelibIn, &_sandtrayEventIn, &_sandtrayReturnIn, &_robotSensorsIn, &_emergencyIn, &_getEyeGazeOut, &_getGripLocationOut, &_getHeadGazeOut, &_getObjectsOut, &_getObjectTableDistanceOut, &_getSoundDirectionOut, &_identifyFaceOut, &_identifyFaceExpressionOut, &_identifyObjectOut, &_identifyTrajectoryOut, &_trackFaceOut, &_trackHandOut, &_trackObjectOut, &_interactionEventOut, &_getInterventionStatusOut, &_attentionBiasOut, &_commandSuccessOut, &_startStopOut, &_deliberativeFeedbackOut, &_sensorySummaryOut, &_sandtrayCommandOut, &_suggestedActionOut);
   _deliberativeSubsystemThread->setYarpInterface(_deliberativeSubsystemYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _deliberativeSubsystemYarpInterface->init(_deliberativeSubsystemThread, _deliberativeSubsystemController);
   
   _deliberativeSubsystemThread->start();

   return true;
}

bool DeliberativeSubsystem::interruptModule(){
   _getChildBehaviourIn.interrupt();
   _getChildPerformanceIn.interrupt();
   _checkMutualGazeIn.interrupt();
   _getArmAngleIn.interrupt();
   _getBodyIn.interrupt();
   _getBodyPoseIn.interrupt();
   _getEyeGazeIn.interrupt();
   _getEyesIn.interrupt();
   _getFacesIn.interrupt();
   _getGripLocationIn.interrupt();
   _getHandsIn.interrupt();
   _getHeadIn.interrupt();
   _getHeadGazeIn.interrupt();
   _getObjectsIn.interrupt();
   _getObjectTableDistanceIn.interrupt();
   _getSoundDirectionIn.interrupt();
   _identifyFaceIn.interrupt();
   _identifyFaceExpressionIn.interrupt();
   _identifyObjectIn.interrupt();
   _identifyTrajectoryIn.interrupt();
   _identifyVoiceIn.interrupt();
   _recognizeSpeechIn.interrupt();
   _trackFaceIn.interrupt();
   _trackObjectIn.interrupt();
   _actionFeedbackIn.interrupt();
   _interventionCommandIn.interrupt();
   _selectedActionIn.interrupt();
   _userDelibIn.interrupt();
   _sandtrayEventIn.interrupt();
   _sandtrayReturnIn.interrupt();
   _robotSensorsIn.interrupt();
   _emergencyIn.interrupt();
   _getEyeGazeOut.interrupt();
   _getGripLocationOut.interrupt();
   _getHeadGazeOut.interrupt();
   _getObjectsOut.interrupt();
   _getObjectTableDistanceOut.interrupt();
   _getSoundDirectionOut.interrupt();
   _identifyFaceOut.interrupt();
   _identifyFaceExpressionOut.interrupt();
   _identifyObjectOut.interrupt();
   _identifyTrajectoryOut.interrupt();
   _trackFaceOut.interrupt();
   _trackHandOut.interrupt();
   _trackObjectOut.interrupt();
   _interactionEventOut.interrupt();
   _getInterventionStatusOut.interrupt();
   _attentionBiasOut.interrupt();
   _commandSuccessOut.interrupt();
   _startStopOut.interrupt();
   _deliberativeFeedbackOut.interrupt();
   _sensorySummaryOut.interrupt();
   _sandtrayCommandOut.interrupt();
   _suggestedActionOut.interrupt();
   _handlerPort.interrupt();


   _deliberativeSubsystemController->close();   return true;
}

bool DeliberativeSubsystem::close(){
   /* close the ports */
   _getChildBehaviourIn.close();
   _getChildPerformanceIn.close();
   _checkMutualGazeIn.close();
   _getArmAngleIn.close();
   _getBodyIn.close();
   _getBodyPoseIn.close();
   _getEyeGazeIn.close();
   _getEyesIn.close();
   _getFacesIn.close();
   _getGripLocationIn.close();
   _getHandsIn.close();
   _getHeadIn.close();
   _getHeadGazeIn.close();
   _getObjectsIn.close();
   _getObjectTableDistanceIn.close();
   _getSoundDirectionIn.close();
   _identifyFaceIn.close();
   _identifyFaceExpressionIn.close();
   _identifyObjectIn.close();
   _identifyTrajectoryIn.close();
   _identifyVoiceIn.close();
   _recognizeSpeechIn.close();
   _trackFaceIn.close();
   _trackObjectIn.close();
   _actionFeedbackIn.close();
   _interventionCommandIn.close();
   _selectedActionIn.close();
   _userDelibIn.close();
   _sandtrayEventIn.close();
   _sandtrayReturnIn.close();
   _robotSensorsIn.close();
   _emergencyIn.close();
   _getEyeGazeOut.close();
   _getGripLocationOut.close();
   _getHeadGazeOut.close();
   _getObjectsOut.close();
   _getObjectTableDistanceOut.close();
   _getSoundDirectionOut.close();
   _identifyFaceOut.close();
   _identifyFaceExpressionOut.close();
   _identifyObjectOut.close();
   _identifyTrajectoryOut.close();
   _trackFaceOut.close();
   _trackHandOut.close();
   _trackObjectOut.close();
   _interactionEventOut.close();
   _getInterventionStatusOut.close();
   _attentionBiasOut.close();
   _commandSuccessOut.close();
   _startStopOut.close();
   _deliberativeFeedbackOut.close();
   _sensorySummaryOut.close();
   _sandtrayCommandOut.close();
   _suggestedActionOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _deliberativeSubsystemThread->stop();

   return true;
}

bool DeliberativeSubsystem::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool DeliberativeSubsystem::updateModule(){
   return true;
}

double DeliberativeSubsystem::getPeriod(){
   return 0.1;
}
