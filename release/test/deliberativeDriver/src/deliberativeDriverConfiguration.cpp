/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Emmanuel Senft, PLYM
 * Email:   emmanuel.senft@plymouth.ac.uk
 * Website: www.dream2020.eu
 *
 * This program comes with ABSOLUTELY NO WARRANTY
 */

 /*
 * Audit Trail
 * -----------
 * 14/09/2016 Version 1.0: ... (Emmanuel Senft)
 */

#include "deliberativeDriver.h"
#include "deliberativeDriverYarpInterface.h"
#include "deliberativeDriverController.h"

#include "scriptMessages.h"
#include "actionMessages.h"

bool DeliberativeDriver::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("deliberativeDriver"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _getEyeGazeInName = "/" +  getName(rf.check("_getEyeGazeIn", Value("/getEyeGaze:i"), "Input getEyeGaze port (string)").asString());
   _getGripLocationInName = "/" +  getName(rf.check("_getGripLocationIn", Value("/getGripLocation:i"), "Input getGripLocation port (string)").asString());
   _getHeadGazeInName = "/" +  getName(rf.check("_getHeadGazeIn", Value("/getHeadGaze:i"), "Input getHeadGaze port (string)").asString());
   _getObjectsInName = "/" +  getName(rf.check("_getObjectsIn", Value("/getObjects:i"), "Input getObjects port (string)").asString());
   _getObjectTableDistanceInName = "/" +  getName(rf.check("_getObjectTableDistanceIn", Value("/getObjectTableDistance:i"), "Input getObjectTableDistance port (string)").asString());
   _getSoundDirectionInName = "/" +  getName(rf.check("_getSoundDirectionIn", Value("/getSoundDirection:i"), "Input getSoundDirection port (string)").asString());
   _identifyFaceInName = "/" +  getName(rf.check("_identifyFaceIn", Value("/identifyFace:i"), "Input identifyFace port (string)").asString());
   _identifyFaceExpressionInName = "/" +  getName(rf.check("_identifyFaceExpressionIn", Value("/identifyFaceExpression:i"), "Input identifyFaceExpression port (string)").asString());
   _identifyObjectInName = "/" +  getName(rf.check("_identifyObjectIn", Value("/identifyObject:i"), "Input identifyObject port (string)").asString());
   _identifyTrajectoryInName = "/" +  getName(rf.check("_identifyTrajectoryIn", Value("/identifyTrajectory:i"), "Input identifyTrajectory port (string)").asString());
   _trackFaceInName = "/" +  getName(rf.check("_trackFaceIn", Value("/trackFace:i"), "Input trackFace port (string)").asString());
   _trackHandInName = "/" +  getName(rf.check("_trackHandIn", Value("/trackHand:i"), "Input trackHand port (string)").asString());
   _trackObjectInName = "/" +  getName(rf.check("_trackObjectIn", Value("/trackObject:i"), "Input trackObject port (string)").asString());
   _interactionEventInName = "/" +  getName(rf.check("_interactionEventIn", Value("/interactionEvent:i"), "Input interactionEvent port (string)").asString());
   _getInterventionStatusInName = "/" +  getName(rf.check("_getInterventionStatusIn", Value("/getInterventionStatus:i"), "Input getInterventionStatus port (string)").asString());
   _attentionBiasInName = "/" +  getName(rf.check("_attentionBiasIn", Value("/attentionBias:i"), "Input attentionBias port (string)").asString());
   _commandSuccessInName = "/" +  getName(rf.check("_commandSuccessIn", Value("/commandSuccess:i"), "Input commandSuccess port (string)").asString());
   _startStopInName = "/" +  getName(rf.check("_startStopIn", Value("/startStop:i"), "Input startStop port (string)").asString());
   _deliberativeFeedbackInName = "/" +  getName(rf.check("_deliberativeFeedbackIn", Value("/deliberativeFeedback:i"), "Input deliberativeFeedback port (string)").asString());
   _sensorySummaryInName = "/" +  getName(rf.check("_sensorySummaryIn", Value("/sensorySummary:i"), "Input sensorySummary port (string)").asString());
   _sandtrayCommandInName = "/" +  getName(rf.check("_sandtrayCommandIn", Value("/sandtrayCommand:i"), "Input sandtrayCommand port (string)").asString());
   _suggestedActionInName = "/" +  getName(rf.check("_suggestedActionIn", Value("/suggestedAction:i"), "Input suggestedAction port (string)").asString());
  
   _getChildBehaviourOutName = "/" +  getName(rf.check("_getChildBehaviourOut", Value("/getChildBehaviour:o"), "output getChildBehaviour port (string)").asString());
   _getChildPerformanceOutName = "/" +  getName(rf.check("_getChildPerformanceOut", Value("/getChildPerformance:o"), "output getChildPerformance port (string)").asString());
   _checkMutualGazeOutName = "/" +  getName(rf.check("_checkMutualGazeOut", Value("/checkMutualGaze:o"), "output checkMutualGaze port (string)").asString());
   _getArmAngleOutName = "/" +  getName(rf.check("_getArmAngleOut", Value("/getArmAngle:o"), "output getArmAngle port (string)").asString());
   _getBodyOutName = "/" +  getName(rf.check("_getBodyOut", Value("/getBody:o"), "output getBody port (string)").asString());
   _getBodyPoseOutName = "/" +  getName(rf.check("_getBodyPoseOut", Value("/getBodyPose:o"), "output getBodyPose port (string)").asString());
   _getEyeGazeOutName = "/" +  getName(rf.check("_getEyeGazeOut", Value("/getEyeGaze:o"), "output getEyeGaze port (string)").asString());
   _getEyesOutName = "/" +  getName(rf.check("_getEyesOut", Value("/getEyes:o"), "output getEyes port (string)").asString());
   _getFacesOutName = "/" +  getName(rf.check("_getFacesOut", Value("/getFaces:o"), "output getFaces port (string)").asString());
   _getGripLocationOutName = "/" +  getName(rf.check("_getGripLocationOut", Value("/getGripLocation:o"), "output getGripLocation port (string)").asString());
   _getHandsOutName = "/" +  getName(rf.check("_getHandsOut", Value("/getHands:o"), "output getHands port (string)").asString());
   _getHeadOutName = "/" +  getName(rf.check("_getHeadOut", Value("/getHead:o"), "output getHead port (string)").asString());
   _getHeadGazeOutName = "/" +  getName(rf.check("_getHeadGazeOut", Value("/getHeadGaze:o"), "output getHeadGaze port (string)").asString());
   _getObjectsOutName = "/" +  getName(rf.check("_getObjectsOut", Value("/getObjects:o"), "output getObjects port (string)").asString());
   _getObjectTableDistanceOutName = "/" +  getName(rf.check("_getObjectTableDistanceOut", Value("/getObjectTableDistance:o"), "output getObjectTableDistance port (string)").asString());
   _getSoundDirectionOutName = "/" +  getName(rf.check("_getSoundDirectionOut", Value("/getSoundDirection:o"), "output getSoundDirection port (string)").asString());
   _identifyFaceOutName = "/" +  getName(rf.check("_identifyFaceOut", Value("/identifyFace:o"), "output identifyFace port (string)").asString());
   _identifyFaceExpressionOutName = "/" +  getName(rf.check("_identifyFaceExpressionOut", Value("/identifyFaceExpression:o"), "output identifyFaceExpression port (string)").asString());
   _identifyObjectOutName = "/" +  getName(rf.check("_identifyObjectOut", Value("/identifyObject:o"), "output identifyObject port (string)").asString());
   _identifyTrajectoryOutName = "/" +  getName(rf.check("_identifyTrajectoryOut", Value("/identifyTrajectory:o"), "output identifyTrajectory port (string)").asString());
   _identifyVoiceOutName = "/" +  getName(rf.check("_identifyVoiceOut", Value("/identifyVoice:o"), "output identifyVoice port (string)").asString());
   _recognizeSpeechOutName = "/" +  getName(rf.check("_recognizeSpeechOut", Value("/recognizeSpeech:o"), "output recognizeSpeech port (string)").asString());
   _trackFaceOutName = "/" +  getName(rf.check("_trackFaceOut", Value("/trackFace:o"), "output trackFace port (string)").asString());
   _trackObjectOutName = "/" +  getName(rf.check("_trackObjectOut", Value("/trackObject:o"), "output trackObject port (string)").asString());
   _actionFeedbackOutName = "/" +  getName(rf.check("_actionFeedbackOut", Value("/actionFeedback:o"), "output actionFeedback port (string)").asString());
   _interventionCommandOutName = "/" +  getName(rf.check("_interventionCommandOut", Value("/interventionCommand:o"), "output interventionCommand port (string)").asString());
   _selectedActionOutName = "/" +  getName(rf.check("_selectedActionOut", Value("/selectedAction:o"), "output selectedAction port (string)").asString());
   _userDelibOutName = "/" +  getName(rf.check("_userDelibOut", Value("/userDelib:o"), "output userDelib port (string)").asString());
   _sandtrayEventOutName = "/" +  getName(rf.check("_sandtrayEventOut", Value("/sandtrayEvent:o"), "output sandtrayEvent port (string)").asString());
   _sandtrayReturnOutName = "/" +  getName(rf.check("_sandtrayReturnOut", Value("/sandtrayReturn:o"), "output sandtrayReturn port (string)").asString());
   _robotSensorsOutName = "/" + getName(rf.check("_robotSensorsOut", Value("/robotSensors:o"), "output robotSensors port (string)").asString());

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
   if (!_trackFaceIn.open(_trackFaceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _trackFaceInName << std::endl;
      return false;
   }
   if (!_trackHandIn.open(_trackHandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _trackHandInName << std::endl;
      return false;
   }
   if (!_trackObjectIn.open(_trackObjectInName.c_str())){
      std::cout << getName() << ": unable to open port " << _trackObjectInName << std::endl;
      return false;
   }
   if (!_interactionEventIn.open(_interactionEventInName.c_str())){
      std::cout << getName() << ": unable to open port " << _interactionEventInName << std::endl;
      return false;
   }
   if (!_getInterventionStatusIn.open(_getInterventionStatusInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getInterventionStatusInName << std::endl;
      return false;
   }
   if (!_attentionBiasIn.open(_attentionBiasInName.c_str())){
      std::cout << getName() << ": unable to open port " << _attentionBiasInName << std::endl;
      return false;
   }
   if (!_commandSuccessIn.open(_commandSuccessInName.c_str())){
      std::cout << getName() << ": unable to open port " << _commandSuccessInName << std::endl;
      return false;
   }
   if (!_startStopIn.open(_startStopInName.c_str())){
      std::cout << getName() << ": unable to open port " << _startStopInName << std::endl;
      return false;
   }
   if (!_deliberativeFeedbackIn.open(_deliberativeFeedbackInName.c_str())){
      std::cout << getName() << ": unable to open port " << _deliberativeFeedbackInName << std::endl;
      return false;
   }
   if (!_sensorySummaryIn.open(_sensorySummaryInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sensorySummaryInName << std::endl;
      return false;
   }
   if (!_sandtrayCommandIn.open(_sandtrayCommandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sandtrayCommandInName << std::endl;
      return false;
   }
   if (!_suggestedActionIn.open(_suggestedActionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _suggestedActionInName << std::endl;
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
   if (!_checkMutualGazeOut.open(_checkMutualGazeOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _checkMutualGazeOutName << std::endl;
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
   if (!_getBodyPoseOut.open(_getBodyPoseOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getBodyPoseOutName << std::endl;
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
   if (!_getFacesOut.open(_getFacesOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getFacesOutName << std::endl;
      return false;
   }
   if (!_getGripLocationOut.open(_getGripLocationOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getGripLocationOutName << std::endl;
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
   if (!_identifyVoiceOut.open(_identifyVoiceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _identifyVoiceOutName << std::endl;
      return false;
   }
   if (!_recognizeSpeechOut.open(_recognizeSpeechOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _recognizeSpeechOutName << std::endl;
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
   if (!_actionFeedbackOut.open(_actionFeedbackOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _actionFeedbackOutName << std::endl;
      return false;
   }
   if (!_interventionCommandOut.open(_interventionCommandOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _interventionCommandOutName << std::endl;
      return false;
   }
   if (!_selectedActionOut.open(_selectedActionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _selectedActionOutName << std::endl;
      return false;
   }
   if (!_userDelibOut.open(_userDelibOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _userDelibOutName << std::endl;
      return false;
   }
   if (!_sandtrayEventOut.open(_sandtrayEventOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sandtrayEventOutName << std::endl;
      return false;
   }
   if (!_sandtrayReturnOut.open(_sandtrayReturnOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sandtrayReturnOutName << std::endl;
      return false;
   }
   if (!_robotSensorsOut.open(_robotSensorsOutName.c_str())) {
	   std::cout << getName() << ":unable to open port " << _robotSensorsOutName << std::endl;
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
   _deliberativeDriverYarpInterface = new DeliberativeDriverYarpInterface();

   /* Create the main class: Controller */ 
   _deliberativeDriverController = new DeliberativeDriverController(_deliberativeDriverYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _deliberativeDriverThread = new DeliberativeDriverThread(&_getEyeGazeIn, &_getGripLocationIn, &_getHeadGazeIn, &_getObjectsIn, &_getObjectTableDistanceIn, &_getSoundDirectionIn, &_identifyFaceIn, &_identifyFaceExpressionIn, &_identifyObjectIn, &_identifyTrajectoryIn, &_trackFaceIn, &_trackHandIn, &_trackObjectIn, &_interactionEventIn, &_getInterventionStatusIn, &_attentionBiasIn, &_commandSuccessIn, &_startStopIn, &_deliberativeFeedbackIn, &_sensorySummaryIn, &_sandtrayCommandIn, &_suggestedActionIn, &_getChildBehaviourOut, &_getChildPerformanceOut, &_checkMutualGazeOut, &_getArmAngleOut, &_getBodyOut, &_getBodyPoseOut, &_getEyeGazeOut, &_getEyesOut, &_getFacesOut, &_getGripLocationOut, &_getHandsOut, &_getHeadOut, &_getHeadGazeOut, &_getObjectsOut, &_getObjectTableDistanceOut, &_getSoundDirectionOut, &_identifyFaceOut, &_identifyFaceExpressionOut, &_identifyObjectOut, &_identifyTrajectoryOut, &_identifyVoiceOut, &_recognizeSpeechOut, &_trackFaceOut, &_trackObjectOut, &_actionFeedbackOut, &_interventionCommandOut, &_selectedActionOut, &_userDelibOut, &_sandtrayEventOut, &_sandtrayReturnOut, &_robotSensorsOut);
   _deliberativeDriverThread->setYarpInterface(_deliberativeDriverYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _deliberativeDriverYarpInterface->init(_deliberativeDriverThread, _deliberativeDriverController);
   
   _deliberativeDriverThread->start();

   return true;
}

bool DeliberativeDriver::interruptModule(){
   _getEyeGazeIn.interrupt();
   _getGripLocationIn.interrupt();
   _getHeadGazeIn.interrupt();
   _getObjectsIn.interrupt();
   _getObjectTableDistanceIn.interrupt();
   _getSoundDirectionIn.interrupt();
   _identifyFaceIn.interrupt();
   _identifyFaceExpressionIn.interrupt();
   _identifyObjectIn.interrupt();
   _identifyTrajectoryIn.interrupt();
   _trackFaceIn.interrupt();
   _trackHandIn.interrupt();
   _trackObjectIn.interrupt();
   _interactionEventIn.interrupt();
   _getInterventionStatusIn.interrupt();
   _attentionBiasIn.interrupt();
   _commandSuccessIn.interrupt();
   _startStopIn.interrupt();
   _deliberativeFeedbackIn.interrupt();
   _sensorySummaryIn.interrupt();
   _sandtrayCommandIn.interrupt();
   _suggestedActionIn.interrupt();
   _getChildBehaviourOut.interrupt();
   _getChildPerformanceOut.interrupt();
   _checkMutualGazeOut.interrupt();
   _getArmAngleOut.interrupt();
   _getBodyOut.interrupt();
   _getBodyPoseOut.interrupt();
   _getEyeGazeOut.interrupt();
   _getEyesOut.interrupt();
   _getFacesOut.interrupt();
   _getGripLocationOut.interrupt();
   _getHandsOut.interrupt();
   _getHeadOut.interrupt();
   _getHeadGazeOut.interrupt();
   _getObjectsOut.interrupt();
   _getObjectTableDistanceOut.interrupt();
   _getSoundDirectionOut.interrupt();
   _identifyFaceOut.interrupt();
   _identifyFaceExpressionOut.interrupt();
   _identifyObjectOut.interrupt();
   _identifyTrajectoryOut.interrupt();
   _identifyVoiceOut.interrupt();
   _recognizeSpeechOut.interrupt();
   _trackFaceOut.interrupt();
   _trackObjectOut.interrupt();
   _actionFeedbackOut.interrupt();
   _interventionCommandOut.interrupt();
   _selectedActionOut.interrupt();
   _userDelibOut.interrupt();
   _sandtrayEventOut.interrupt();
   _sandtrayReturnOut.interrupt();
   _robotSensorsOut.interrupt();
   _handlerPort.interrupt();

   _deliberativeDriverController->stop();
   return true;
}

bool DeliberativeDriver::close(){
   /* close the ports */
   _getEyeGazeIn.close();
   _getGripLocationIn.close();
   _getHeadGazeIn.close();
   _getObjectsIn.close();
   _getObjectTableDistanceIn.close();
   _getSoundDirectionIn.close();
   _identifyFaceIn.close();
   _identifyFaceExpressionIn.close();
   _identifyObjectIn.close();
   _identifyTrajectoryIn.close();
   _trackFaceIn.close();
   _trackHandIn.close();
   _trackObjectIn.close();
   _interactionEventIn.close();
   _getInterventionStatusIn.close();
   _attentionBiasIn.close();
   _commandSuccessIn.close();
   _startStopIn.close();
   _deliberativeFeedbackIn.close();
   _sensorySummaryIn.close();
   _sandtrayCommandIn.close();
   _suggestedActionIn.close();
   _getChildBehaviourOut.close();
   _getChildPerformanceOut.close();
   _checkMutualGazeOut.close();
   _getArmAngleOut.close();
   _getBodyOut.close();
   _getBodyPoseOut.close();
   _getEyeGazeOut.close();
   _getEyesOut.close();
   _getFacesOut.close();
   _getGripLocationOut.close();
   _getHandsOut.close();
   _getHeadOut.close();
   _getHeadGazeOut.close();
   _getObjectsOut.close();
   _getObjectTableDistanceOut.close();
   _getSoundDirectionOut.close();
   _identifyFaceOut.close();
   _identifyFaceExpressionOut.close();
   _identifyObjectOut.close();
   _identifyTrajectoryOut.close();
   _identifyVoiceOut.close();
   _recognizeSpeechOut.close();
   _trackFaceOut.close();
   _trackObjectOut.close();
   _actionFeedbackOut.close();
   _interventionCommandOut.close();
   _selectedActionOut.close();
   _userDelibOut.close();
   _sandtrayEventOut.close();
   _sandtrayReturnOut.close();
   _robotSensorsOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _deliberativeDriverThread->stop();

   return true;
}

bool DeliberativeDriver::respond(const Bottle &command, Bottle &reply){

	reply.clear();

	if (command.get(0).asString() == "help") {
		reply.addString("test | wait | scriptSay | scriptMove | waitms | wait4perf | scriptGrab |");
		reply.addString("scriptDrop | fbImObj | fbImObjLast | fbImMove | fbImMoveLast | fbTTWait |");
		reply.addString("fbTTSort | fbTTChos | fbTTPat | fbJA | displayWhite|loadLib4 | imgOne | imgAll |");
		reply.addString("enFb | disFb | enTouch | disTouch | scriptRobGood | scriptPoint |");
		reply.addString("scriptLookPointSay | scriptLookPoit | scriptLook | scriptEnd | scriptCorrupt |");
		reply.addString("next | sendPerf | say |move| grab | drop | fbImObjG | fbImObjB | fbImMovG |"); 
		reply.addString("fbImMovB | fbTTWaitG | fbTTWaitB | fbTTSortG | fbTTSortB | fbTTChosG | fbTTChosB |");
		reply.addString("fbTTPatG | fbTTPatB | fbJAG | fbJAB | robGood | point | lookPointSay | lookPoint |");
		reply.addString("look | actionGoodPerf | actionBadPerf | end | corrupt | actionFb | goodPerf |"); 
		reply.addString("badPerf | sendGaze | disableAutoGaze | enableAutoGaze");
	}
	else {
		reply.addString(command.get(0).asString() + " being executed...");

		if (command.get(0).asString() == "wait") 					_deliberativeDriverController->testCommand(1, _SCRIPT_CHECK_GAZE_);
		else if (command.get(0).asString() == "scriptSay") 			_deliberativeDriverController->testCommand(1, _SCRIPT_SAY_, "test");
		else if (command.get(0).asString() == "scriptMove") 		_deliberativeDriverController->testCommand(1, _SCRIPT_MOVE_, "1");
		else if (command.get(0).asString() == "waitms") 			_deliberativeDriverController->testCommand(1, _SCRIPT_WAIT_, 2000);
		else if (command.get(0).asString() == "wait4perf") 			_deliberativeDriverController->testCommand(1, _SCRIPT_WAIT_FOR_PERF_, 1, 1, 2000);
		else if (command.get(0).asString() == "scriptGrab") 		_deliberativeDriverController->testCommand(1, _SCRIPT_GRAB_OBJECT_);
		else if (command.get(0).asString() == "scriptDrop") 		_deliberativeDriverController->testCommand(1, _SCRIPT_DROP_OBJECT_);
		else if (command.get(0).asString() == "fbImObj") 			_deliberativeDriverController->testCommand(1, _SCRIPT_FEEDBACK_IMITATION_OBJECT_);
		else if (command.get(0).asString() == "fbImObjLast") 		_deliberativeDriverController->testCommand(1, _SCRIPT_LAST_FEEDBACK_IMITATION_OBJECT_);
		else if (command.get(0).asString() == "fbImMove") 			_deliberativeDriverController->testCommand(1, _SCRIPT_FEEDBACK_IMITATION_MOVE_);
		else if (command.get(0).asString() == "fbImMoveLast") 		_deliberativeDriverController->testCommand(1, _SCRIPT_LAST_FEEDBACK_IMITATION_MOVE_);
		else if (command.get(0).asString() == "fbTTWait") 			_deliberativeDriverController->testCommand(1, _SCRIPT_FEEDBACK_TT_WAITING_);
		else if (command.get(0).asString() == "fbTTSort") 			_deliberativeDriverController->testCommand(1, _SCRIPT_FEEDBACK_TT_SORTING_);
		else if (command.get(0).asString() == "fbTTChos") 			_deliberativeDriverController->testCommand(1, _SCRIPT_FEEDBACK_TT_CHOOSING_);
		else if (command.get(0).asString() == "fbTTPat") 			_deliberativeDriverController->testCommand(1, _SCRIPT_FEEDBACK_TT_PATTERN_);
		else if (command.get(0).asString() == "fbJA") 				_deliberativeDriverController->testCommand(1, _SCRIPT_FEEDBACK_JA_);
		else if (command.get(0).asString() == "displayWhite") 		_deliberativeDriverController->testCommand(1, _SCRIPT_SANDTRAY_DISPLAY_WHITE_);
		else if (command.get(0).asString() == "loadLib4") 			_deliberativeDriverController->testCommand(1, _SCRIPT_SANDTRAY_LOAD_LIBRARY_, 4);
		else if (command.get(0).asString() == "imgOne") 			_deliberativeDriverController->testCommand(1, _SCRIPT_SANDTRAY_SET_IMAGE_ONE_);
		else if (command.get(0).asString() == "imgAll") 			_deliberativeDriverController->testCommand(1, _SCRIPT_SANDTRAY_SET_IMAGE_ALL_);
		else if (command.get(0).asString() == "enFb") 				_deliberativeDriverController->testCommand(1, _SCRIPT_SANDTRAY_ENABLE_FEEDBACK_);
		else if (command.get(0).asString() == "disFb") 				_deliberativeDriverController->testCommand(1, _SCRIPT_SANDTRAY_DISABLE_FEEDBACK_);
		else if (command.get(0).asString() == "enTouch") 			_deliberativeDriverController->testCommand(1, _SCRIPT_SANDTRAY_ENABLE_TOUCH_);
		else if (command.get(0).asString() == "disTouch") 			_deliberativeDriverController->testCommand(1, _SCRIPT_SANDTRAY_DISABLE_TOUCH_);
		else if (command.get(0).asString() == "scriptRobGood") 		_deliberativeDriverController->testCommand(1, _SCRIPT_ROBOT_GOOD_MOVE_);
		else if (command.get(0).asString() == "scriptPoint") 		_deliberativeDriverController->testCommand(1, _SCRIPT_POINT_IMAGE_);
		else if (command.get(0).asString() == "scriptLookPointSay")	_deliberativeDriverController->testCommand(1, _SCRIPT_LOOK_POINT_SAY_, 0);
		else if (command.get(0).asString() == "scriptLookPoit") 	_deliberativeDriverController->testCommand(1, _SCRIPT_LOOK_POINT_, 1);
		else if (command.get(0).asString() == "scriptLook") 		_deliberativeDriverController->testCommand(1, _SCRIPT_LOOK_, 0);
		else if (command.get(0).asString() == "scriptEnd") 			_deliberativeDriverController->testCommand(1, _SCRIPT_FINISHED_);
		else if (command.get(0).asString() == "scriptCorrupt") 		_deliberativeDriverController->testCommand(1, _SCRIPT_CORRUPTED_);
		else if (command.get(0).asString() == "next") 				_deliberativeDriverController->testSelection(1, _ACTION_GO_NEXT_STEP_);
		else if (command.get(0).asString() == "sendPerf") 			_deliberativeDriverController->testSelection(1, _ACTION_RECEIVED_PERF_, 0.8);
		else if (command.get(0).asString() == "say") 				_deliberativeDriverController->testSelection(1, _ACTION_SAY_, "test");
		else if (command.get(0).asString() == "move") 				_deliberativeDriverController->testSelection(1, _ACTION_MOVE_, "move");
		else if (command.get(0).asString() == "grab") 				_deliberativeDriverController->testSelection(1, _ACTION_GRAB_OBJECT_);
		else if (command.get(0).asString() == "drop") 				_deliberativeDriverController->testSelection(1, _ACTION_DROP_OBJECT_);
		else if (command.get(0).asString() == "fbImObjG") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_);
		else if (command.get(0).asString() == "fbImObjB") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_);
		else if (command.get(0).asString() == "fbImMovG") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_);
		else if (command.get(0).asString() == "fbImMovB") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_IMITATION_MOVE_BAD_);
		else if (command.get(0).asString() == "fbTTWaitG") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_TT_WAITING_GOOD_);
		else if (command.get(0).asString() == "fbTTWaitB") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_TT_WAITING_BAD_);
		else if (command.get(0).asString() == "fbTTSortG") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_TT_SORTING_GOOD_);
		else if (command.get(0).asString() == "fbTTSortB") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_TT_SORTING_BAD_);
		else if (command.get(0).asString() == "fbTTChosG") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_TT_CHOOSING_GOOD_);
		else if (command.get(0).asString() == "fbTTChosB") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_TT_CHOOSING_BAD_);
		else if (command.get(0).asString() == "fbTTPatG") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_TT_PATTERN_GOOD_);
		else if (command.get(0).asString() == "fbTTPatB") 			_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_TT_PATTERN_BAD_);
		else if (command.get(0).asString() == "fbJAG") 				_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_JA_GOOD_);
		else if (command.get(0).asString() == "fbJAB") 				_deliberativeDriverController->testSelection(1, _ACTION_FEEDBACK_JA_BAD_);
		else if (command.get(0).asString() == "robGood") 			_deliberativeDriverController->testSelection(1, _ACTION_ROBOT_GOOD_MOVE_);
		else if (command.get(0).asString() == "point") 				_deliberativeDriverController->testSelection(1, _ACTION_POINT_IMAGE_, 1);
		else if (command.get(0).asString() == "lookPointSay") 		_deliberativeDriverController->testSelection(1, _ACTION_LOOK_POINT_SAY_, 1);
		else if (command.get(0).asString() == "lookPoint") 			_deliberativeDriverController->testSelection(1, _ACTION_LOOK_POINT_, 0);
		else if (command.get(0).asString() == "look") 				_deliberativeDriverController->testSelection(1, _ACTION_LOOK_, 1);
		else if (command.get(0).asString() == "actionGoodPerf") 	_deliberativeDriverController->testSelection(1, _ACTION_RECEIVED_PERF_, 1.);
		else if (command.get(0).asString() == "actionBadPerf") 		_deliberativeDriverController->testSelection(1, _ACTION_RECEIVED_PERF_, 0.);
		else if (command.get(0).asString() == "end") 				_deliberativeDriverController->testSelection(1, _ACTION_FINISHED_);
		else if (command.get(0).asString() == "corrupt") 			_deliberativeDriverController->testSelection(1, _ACTION_CORRUPTED_);
		else if (command.get(0).asString() == "actionFb") 			_deliberativeDriverController->sendToActionFeedback(1, 2, 1);
		else if (command.get(0).asString() == "goodPerf") 			_deliberativeDriverController->sendToPerf(1.);
		else if (command.get(0).asString() == "badPerf") 			_deliberativeDriverController->sendToPerf(0.1);
		else if (command.get(0).asString() == "sendGaze")		 	_deliberativeDriverController->sendMutualGaze(1);
		else if (command.get(0).asString() == "disableAutoGaze") 	_deliberativeDriverController->enableAutoGaze(0);
		else if (command.get(0).asString() == "enableAutoGaze") 	_deliberativeDriverController->enableAutoGaze(1);
		else if (command.get(0).asString() == "test") 				_deliberativeDriverController->testApp();
		else reply.addString("no command recognised");
	}
   return true;
}

bool DeliberativeDriver::updateModule(){
   return true;
}

double DeliberativeDriver::getPeriod(){
   return 0.1;
}
