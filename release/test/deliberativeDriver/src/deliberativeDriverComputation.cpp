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
  * 25/11/2016 Version 1.0: ... (Emmanuel Senft)
  */ 

#include "deliberativeDriver.h"
#include "deliberativeDriverYarpInterface.h"

DeliberativeDriverThread::DeliberativeDriverThread(BufferedPort<VectorOf<double>>* getEyeGazeIn, BufferedPort<VectorOf<double>>* getGripLocationIn, BufferedPort<VectorOf<double>>* getHeadGazeIn, BufferedPort<VectorOf<double>>* getObjectsIn, BufferedPort<VectorOf<double>>* getObjectTableDistanceIn, BufferedPort<VectorOf<double>>* getSoundDirectionIn, BufferedPort<VectorOf<double>>* identifyFaceIn, BufferedPort<VectorOf<int>>* identifyFaceExpressionIn, BufferedPort<VectorOf<double>>* identifyObjectIn, BufferedPort<VectorOf<int>>* identifyTrajectoryIn, BufferedPort<VectorOf<double>>* trackFaceIn, BufferedPort<VectorOf<double>>* trackHandIn, BufferedPort<VectorOf<double>>* trackObjectIn, BufferedPort<VectorOf<int>>* interactionEventIn, BufferedPort<VectorOf<int>>* getInterventionStatusIn, BufferedPort<Bottle>* attentionBiasIn, BufferedPort<VectorOf<int>>* commandSuccessIn, BufferedPort<VectorOf<int>>* startStopIn, BufferedPort<Bottle>* deliberativeFeedbackIn, BufferedPort<Bottle>* sensorySummaryIn, BufferedPort<Bottle>* sandtrayCommandIn, BufferedPort<Bottle>* suggestedActionIn, BufferedPort<VectorOf<double>>* getChildBehaviourOut, BufferedPort<VectorOf<double>>* getChildPerformanceOut, BufferedPort<VectorOf<int>>* checkMutualGazeOut, BufferedPort<VectorOf<double>>* getArmAngleOut, BufferedPort<VectorOf<double>>* getBodyOut, BufferedPort<VectorOf<double>>* getBodyPoseOut, BufferedPort<VectorOf<double>>* getEyeGazeOut, BufferedPort<VectorOf<double>>* getEyesOut, BufferedPort<VectorOf<double>>* getFacesOut, BufferedPort<VectorOf<double>>* getGripLocationOut, BufferedPort<VectorOf<double>>* getHandsOut, BufferedPort<VectorOf<double>>* getHeadOut, BufferedPort<VectorOf<double>>* getHeadGazeOut, BufferedPort<VectorOf<double>>* getObjectsOut, BufferedPort<VectorOf<double>>* getObjectTableDistanceOut, BufferedPort<VectorOf<double>>* getSoundDirectionOut, BufferedPort<VectorOf<double>>* identifyFaceOut, BufferedPort<VectorOf<int>>* identifyFaceExpressionOut, BufferedPort<VectorOf<double>>* identifyObjectOut, BufferedPort<VectorOf<double>>* identifyTrajectoryOut, BufferedPort<VectorOf<int>>* identifyVoiceOut, BufferedPort<Bottle>* recognizeSpeechOut, BufferedPort<VectorOf<double>>* trackFaceOut, BufferedPort<VectorOf<double>>* trackObjectOut, BufferedPort<Bottle>* actionFeedbackOut, BufferedPort<Bottle>* interventionCommandOut, BufferedPort<Bottle>* selectedActionOut, BufferedPort<Bottle>* userDelibOut, BufferedPort<Bottle>* sandtrayEventOut, BufferedPort<Bottle>* sandtrayReturnOut, BufferedPort<Bottle>* robotSensorsOut){
	/* assign the input and output ports */
	_getEyeGazeIn = getEyeGazeIn;
	_getGripLocationIn = getGripLocationIn;
	_getHeadGazeIn = getHeadGazeIn;
	_getObjectsIn = getObjectsIn;
	_getObjectTableDistanceIn = getObjectTableDistanceIn;
	_getSoundDirectionIn = getSoundDirectionIn;
	_identifyFaceIn = identifyFaceIn;
	_identifyFaceExpressionIn = identifyFaceExpressionIn;
	_identifyObjectIn = identifyObjectIn;
	_identifyTrajectoryIn = identifyTrajectoryIn;
	_trackFaceIn = trackFaceIn;
	_trackHandIn = trackHandIn;
	_trackObjectIn = trackObjectIn;
	_interactionEventIn = interactionEventIn;
	_getInterventionStatusIn = getInterventionStatusIn;
	_attentionBiasIn = attentionBiasIn;
	_commandSuccessIn = commandSuccessIn;
	_startStopIn = startStopIn;
	_deliberativeFeedbackIn = deliberativeFeedbackIn;
	_sensorySummaryIn = sensorySummaryIn;
	_sandtrayCommandIn = sandtrayCommandIn;
	_suggestedActionIn = suggestedActionIn;
	_getChildBehaviourOut = getChildBehaviourOut;
	_getChildPerformanceOut = getChildPerformanceOut;
	_checkMutualGazeOut = checkMutualGazeOut;
	_getArmAngleOut = getArmAngleOut;
	_getBodyOut = getBodyOut;
	_getBodyPoseOut = getBodyPoseOut;
	_getEyeGazeOut = getEyeGazeOut;
	_getEyesOut = getEyesOut;
	_getFacesOut = getFacesOut;
	_getGripLocationOut = getGripLocationOut;
	_getHandsOut = getHandsOut;
	_getHeadOut = getHeadOut;
	_getHeadGazeOut = getHeadGazeOut;
	_getObjectsOut = getObjectsOut;
	_getObjectTableDistanceOut = getObjectTableDistanceOut;
	_getSoundDirectionOut = getSoundDirectionOut;
	_identifyFaceOut = identifyFaceOut;
	_identifyFaceExpressionOut = identifyFaceExpressionOut;
	_identifyObjectOut = identifyObjectOut;
	_identifyTrajectoryOut = identifyTrajectoryOut;
	_identifyVoiceOut = identifyVoiceOut;
	_recognizeSpeechOut = recognizeSpeechOut;
	_trackFaceOut = trackFaceOut;
	_trackObjectOut = trackObjectOut;
	_actionFeedbackOut = actionFeedbackOut;
	_interventionCommandOut = interventionCommandOut;
	_selectedActionOut = selectedActionOut;
	_userDelibOut = userDelibOut;
	_sandtrayEventOut = sandtrayEventOut;
	_sandtrayReturnOut = sandtrayReturnOut;
	_robotSensorsOut = robotSensorsOut;
   
	_getEyeGazeInAnalyser = new GetEyeGazeInAnalyser();
	_getGripLocationInAnalyser = new GetGripLocationInAnalyser();
	_getHeadGazeInAnalyser = new GetHeadGazeInAnalyser();
	_getObjectsInAnalyser = new GetObjectsInAnalyser();
	_getObjectTableDistanceInAnalyser = new GetObjectTableDistanceInAnalyser();
	_getSoundDirectionInAnalyser = new GetSoundDirectionInAnalyser();
	_identifyFaceInAnalyser = new IdentifyFaceInAnalyser();
	_identifyFaceExpressionInAnalyser = new IdentifyFaceExpressionInAnalyser();
	_identifyObjectInAnalyser = new IdentifyObjectInAnalyser();
	_identifyTrajectoryInAnalyser = new IdentifyTrajectoryInAnalyser();
	_trackFaceInAnalyser = new TrackFaceInAnalyser();
	_trackHandInAnalyser = new TrackHandInAnalyser();
	_trackObjectInAnalyser = new TrackObjectInAnalyser();
	_interactionEventInAnalyser = new InteractionEventInAnalyser();
	_getInterventionStatusInAnalyser = new GetInterventionStatusInAnalyser();
	_attentionBiasInAnalyser = new AttentionBiasInAnalyser();
	_commandSuccessInAnalyser = new CommandSuccessInAnalyser();
	_startStopInAnalyser = new StartStopInAnalyser();
	_deliberativeFeedbackInAnalyser = new DeliberativeFeedbackInAnalyser();
	_sensorySummaryInAnalyser = new SensorySummaryInAnalyser();
	_sandtrayCommandInAnalyser = new SandtrayCommandInAnalyser();
	_suggestedActionInAnalyser = new SuggestedActionInAnalyser();
   
	getEyeGazeIn->useCallback(*_getEyeGazeInAnalyser);
	getGripLocationIn->useCallback(*_getGripLocationInAnalyser);
	getHeadGazeIn->useCallback(*_getHeadGazeInAnalyser);
	getObjectsIn->useCallback(*_getObjectsInAnalyser);
	getObjectTableDistanceIn->useCallback(*_getObjectTableDistanceInAnalyser);
	getSoundDirectionIn->useCallback(*_getSoundDirectionInAnalyser);
	identifyFaceIn->useCallback(*_identifyFaceInAnalyser);
	identifyFaceExpressionIn->useCallback(*_identifyFaceExpressionInAnalyser);
	identifyObjectIn->useCallback(*_identifyObjectInAnalyser);
	identifyTrajectoryIn->useCallback(*_identifyTrajectoryInAnalyser);
	trackFaceIn->useCallback(*_trackFaceInAnalyser);
	trackHandIn->useCallback(*_trackHandInAnalyser);
	trackObjectIn->useCallback(*_trackObjectInAnalyser);
	interactionEventIn->useCallback(*_interactionEventInAnalyser);
	getInterventionStatusIn->useCallback(*_getInterventionStatusInAnalyser);
	attentionBiasIn->useCallback(*_attentionBiasInAnalyser);
	commandSuccessIn->useCallback(*_commandSuccessInAnalyser);
	startStopIn->useCallback(*_startStopInAnalyser);
	deliberativeFeedbackIn->useCallback(*_deliberativeFeedbackInAnalyser);
	sensorySummaryIn->useCallback(*_sensorySummaryInAnalyser);
	sandtrayCommandIn->useCallback(*_sandtrayCommandInAnalyser);
	suggestedActionIn->useCallback(*_suggestedActionInAnalyser);
}

void DeliberativeDriverThread::setYarpInterface(DeliberativeDriverYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_getEyeGazeInAnalyser->setDeliberativeDriver(_yarpInterface);
	_getGripLocationInAnalyser->setDeliberativeDriver(_yarpInterface);
	_getHeadGazeInAnalyser->setDeliberativeDriver(_yarpInterface);
	_getObjectsInAnalyser->setDeliberativeDriver(_yarpInterface);
	_getObjectTableDistanceInAnalyser->setDeliberativeDriver(_yarpInterface);
	_getSoundDirectionInAnalyser->setDeliberativeDriver(_yarpInterface);
	_identifyFaceInAnalyser->setDeliberativeDriver(_yarpInterface);
	_identifyFaceExpressionInAnalyser->setDeliberativeDriver(_yarpInterface);
	_identifyObjectInAnalyser->setDeliberativeDriver(_yarpInterface);
	_identifyTrajectoryInAnalyser->setDeliberativeDriver(_yarpInterface);
	_trackFaceInAnalyser->setDeliberativeDriver(_yarpInterface);
	_trackHandInAnalyser->setDeliberativeDriver(_yarpInterface);
	_trackObjectInAnalyser->setDeliberativeDriver(_yarpInterface);
	_interactionEventInAnalyser->setDeliberativeDriver(_yarpInterface);
	_getInterventionStatusInAnalyser->setDeliberativeDriver(_yarpInterface);
	_attentionBiasInAnalyser->setDeliberativeDriver(_yarpInterface);
	_commandSuccessInAnalyser->setDeliberativeDriver(_yarpInterface);
	_startStopInAnalyser->setDeliberativeDriver(_yarpInterface);
	_deliberativeFeedbackInAnalyser->setDeliberativeDriver(_yarpInterface);
	_sensorySummaryInAnalyser->setDeliberativeDriver(_yarpInterface);
	_sandtrayCommandInAnalyser->setDeliberativeDriver(_yarpInterface);
	_suggestedActionInAnalyser->setDeliberativeDriver(_yarpInterface);
}

bool DeliberativeDriverThread::threadInit(){
	/* initilize the thread */
	return true;
}

void DeliberativeDriverThread::run(){

}

void DeliberativeDriverThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void DeliberativeDriverThread::sendToGetChildBehaviour(VectorOf<double> data){
	VectorOf<double>& container = _getChildBehaviourOut->prepare();
	container = data;
	_getChildBehaviourOut->write();
}

void DeliberativeDriverThread::sendToGetChildPerformance(VectorOf<double> data){
	VectorOf<double>& container = _getChildPerformanceOut->prepare();
	container = data;
	_getChildPerformanceOut->write();
}

void DeliberativeDriverThread::sendToCheckMutualGaze(VectorOf<int> data){
	VectorOf<int>& container = _checkMutualGazeOut->prepare();
	container = data;
	_checkMutualGazeOut->write();
}

void DeliberativeDriverThread::sendToGetArmAngle(VectorOf<double> data){
	VectorOf<double>& container = _getArmAngleOut->prepare();
	container = data;
	_getArmAngleOut->write();
}

void DeliberativeDriverThread::sendToGetBody(VectorOf<double> data){
	VectorOf<double>& container = _getBodyOut->prepare();
	container = data;
	_getBodyOut->write();
}

void DeliberativeDriverThread::sendToGetBodyPose(VectorOf<double> data){
	VectorOf<double>& container = _getBodyPoseOut->prepare();
	container = data;
	_getBodyPoseOut->write();
}

void DeliberativeDriverThread::sendToGetEyeGaze(VectorOf<double> data){
	VectorOf<double>& container = _getEyeGazeOut->prepare();
	container = data;
	_getEyeGazeOut->write();
}

void DeliberativeDriverThread::sendToGetEyes(VectorOf<double> data){
	VectorOf<double>& container = _getEyesOut->prepare();
	container = data;
	_getEyesOut->write();
}

void DeliberativeDriverThread::sendToGetFaces(VectorOf<double> data){
	VectorOf<double>& container = _getFacesOut->prepare();
	container = data;
	_getFacesOut->write();
}

void DeliberativeDriverThread::sendToGetGripLocation(VectorOf<double> data){
	VectorOf<double>& container = _getGripLocationOut->prepare();
	container = data;
	_getGripLocationOut->write();
}

void DeliberativeDriverThread::sendToGetHands(VectorOf<double> data){
	VectorOf<double>& container = _getHandsOut->prepare();
	container = data;
	_getHandsOut->write();
}

void DeliberativeDriverThread::sendToGetHead(VectorOf<double> data){
	VectorOf<double>& container = _getHeadOut->prepare();
	container = data;
	_getHeadOut->write();
}

void DeliberativeDriverThread::sendToGetHeadGaze(VectorOf<double> data){
	VectorOf<double>& container = _getHeadGazeOut->prepare();
	container = data;
	_getHeadGazeOut->write();
}

void DeliberativeDriverThread::sendToGetObjects(VectorOf<double> data){
	VectorOf<double>& container = _getObjectsOut->prepare();
	container = data;
	_getObjectsOut->write();
}

void DeliberativeDriverThread::sendToGetObjectTableDistance(VectorOf<double> data){
	VectorOf<double>& container = _getObjectTableDistanceOut->prepare();
	container = data;
	_getObjectTableDistanceOut->write();
}

void DeliberativeDriverThread::sendToGetSoundDirection(VectorOf<double> data){
	VectorOf<double>& container = _getSoundDirectionOut->prepare();
	container = data;
	_getSoundDirectionOut->write();
}

void DeliberativeDriverThread::sendToIdentifyFace(VectorOf<double> data){
	VectorOf<double>& container = _identifyFaceOut->prepare();
	container = data;
	_identifyFaceOut->write();
}

void DeliberativeDriverThread::sendToIdentifyFaceExpression(VectorOf<int> data){
	VectorOf<int>& container = _identifyFaceExpressionOut->prepare();
	container = data;
	_identifyFaceExpressionOut->write();
}

void DeliberativeDriverThread::sendToIdentifyObject(VectorOf<double> data){
	VectorOf<double>& container = _identifyObjectOut->prepare();
	container = data;
	_identifyObjectOut->write();
}

void DeliberativeDriverThread::sendToIdentifyTrajectory(VectorOf<double> data){
	VectorOf<double>& container = _identifyTrajectoryOut->prepare();
	container = data;
	_identifyTrajectoryOut->write();
}

void DeliberativeDriverThread::sendToIdentifyVoice(VectorOf<int> data){
	VectorOf<int>& container = _identifyVoiceOut->prepare();
	container = data;
	_identifyVoiceOut->write();
}

void DeliberativeDriverThread::sendToRecognizeSpeech(Bottle data){
	Bottle& container = _recognizeSpeechOut->prepare();
	container = data;
	_recognizeSpeechOut->write();
}

void DeliberativeDriverThread::sendToTrackFace(VectorOf<double> data){
	VectorOf<double>& container = _trackFaceOut->prepare();
	container = data;
	_trackFaceOut->write();
}

void DeliberativeDriverThread::sendToTrackObject(VectorOf<double> data){
	VectorOf<double>& container = _trackObjectOut->prepare();
	container = data;
	_trackObjectOut->write();
}

void DeliberativeDriverThread::sendToActionFeedback(Bottle data){
	Bottle& container = _actionFeedbackOut->prepare();
	container = data;
	_actionFeedbackOut->write();
}

void DeliberativeDriverThread::sendToInterventionCommand(Bottle data){
	Bottle& container = _interventionCommandOut->prepare();
	container = data;
	_interventionCommandOut->write();
}

void DeliberativeDriverThread::sendToSelectedAction(Bottle data){
	Bottle& container = _selectedActionOut->prepare();
	container = data;
	_selectedActionOut->write();
}

void DeliberativeDriverThread::sendToUserDelib(Bottle data){
	Bottle& container = _userDelibOut->prepare();
	container = data;
	_userDelibOut->write();
}

void DeliberativeDriverThread::sendToSandtrayEvent(Bottle data){
	Bottle& container = _sandtrayEventOut->prepare();
	container = data;
	_sandtrayEventOut->write();
}

void DeliberativeDriverThread::sendToSandtrayReturn(Bottle data){
	Bottle& container = _sandtrayReturnOut->prepare();
	container = data;
	_sandtrayReturnOut->write();
}

void DeliberativeDriverThread::sendToRobotSensors(Bottle data) {
	Bottle& container = _robotSensorsOut->prepare();
	container = data;
	_robotSensorsOut->write();
}


void GetEyeGazeInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void GetGripLocationInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void GetHeadGazeInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void GetObjectsInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void GetObjectTableDistanceInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void GetSoundDirectionInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void IdentifyFaceInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void IdentifyFaceExpressionInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void IdentifyObjectInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void IdentifyTrajectoryInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void TrackFaceInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void TrackHandInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void TrackObjectInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void InteractionEventInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void GetInterventionStatusInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void AttentionBiasInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void CommandSuccessInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void StartStopInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void DeliberativeFeedbackInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void SensorySummaryInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void SandtrayCommandInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}

void SuggestedActionInAnalyser::setDeliberativeDriver(DeliberativeDriverYarpInterface* yarpInterface){
   _deliberativeDriverYarpInterface=yarpInterface;
}



void GetEyeGazeInAnalyser::onRead(VectorOf<double>& getEyeGazeInData){
   _deliberativeDriverYarpInterface->getFromGetEyeGaze(getEyeGazeInData);
}

void GetGripLocationInAnalyser::onRead(VectorOf<double>& getGripLocationInData){
   _deliberativeDriverYarpInterface->getFromGetGripLocation(getGripLocationInData);
}

void GetHeadGazeInAnalyser::onRead(VectorOf<double>& getHeadGazeInData){
   _deliberativeDriverYarpInterface->getFromGetHeadGaze(getHeadGazeInData);
}

void GetObjectsInAnalyser::onRead(VectorOf<double>& getObjectsInData){
   _deliberativeDriverYarpInterface->getFromGetObjects(getObjectsInData);
}

void GetObjectTableDistanceInAnalyser::onRead(VectorOf<double>& getObjectTableDistanceInData){
   _deliberativeDriverYarpInterface->getFromGetObjectTableDistance(getObjectTableDistanceInData);
}

void GetSoundDirectionInAnalyser::onRead(VectorOf<double>& getSoundDirectionInData){
   _deliberativeDriverYarpInterface->getFromGetSoundDirection(getSoundDirectionInData);
}

void IdentifyFaceInAnalyser::onRead(VectorOf<double>& identifyFaceInData){
   _deliberativeDriverYarpInterface->getFromIdentifyFace(identifyFaceInData);
}

void IdentifyFaceExpressionInAnalyser::onRead(VectorOf<int>& identifyFaceExpressionInData){
   _deliberativeDriverYarpInterface->getFromIdentifyFaceExpression(identifyFaceExpressionInData);
}

void IdentifyObjectInAnalyser::onRead(VectorOf<double>& identifyObjectInData){
   _deliberativeDriverYarpInterface->getFromIdentifyObject(identifyObjectInData);
}

void IdentifyTrajectoryInAnalyser::onRead(VectorOf<int>& identifyTrajectoryInData){
   _deliberativeDriverYarpInterface->getFromIdentifyTrajectory(identifyTrajectoryInData);
}

void TrackFaceInAnalyser::onRead(VectorOf<double>& trackFaceInData){
   _deliberativeDriverYarpInterface->getFromTrackFace(trackFaceInData);
}

void TrackHandInAnalyser::onRead(VectorOf<double>& trackHandInData){
   _deliberativeDriverYarpInterface->getFromTrackHand(trackHandInData);
}

void TrackObjectInAnalyser::onRead(VectorOf<double>& trackObjectInData){
   _deliberativeDriverYarpInterface->getFromTrackObject(trackObjectInData);
}

void InteractionEventInAnalyser::onRead(VectorOf<int>& interactionEventInData){
   _deliberativeDriverYarpInterface->getFromInteractionEvent(interactionEventInData);
}

void GetInterventionStatusInAnalyser::onRead(VectorOf<int>& getInterventionStatusInData){
   _deliberativeDriverYarpInterface->getFromGetInterventionStatus(getInterventionStatusInData);
}

void AttentionBiasInAnalyser::onRead(Bottle& attentionBiasInData){
   _deliberativeDriverYarpInterface->getFromAttentionBias(attentionBiasInData);
}

void CommandSuccessInAnalyser::onRead(VectorOf<int>& commandSuccessInData){
   _deliberativeDriverYarpInterface->getFromCommandSuccess(commandSuccessInData);
}

void StartStopInAnalyser::onRead(VectorOf<int>& startStopInData){
   _deliberativeDriverYarpInterface->getFromStartStop(startStopInData);
}

void DeliberativeFeedbackInAnalyser::onRead(Bottle& deliberativeFeedbackInData){
   _deliberativeDriverYarpInterface->getFromDeliberativeFeedback(deliberativeFeedbackInData);
}

void SensorySummaryInAnalyser::onRead(Bottle& sensorySummaryInData){
   _deliberativeDriverYarpInterface->getFromSensorySummary(sensorySummaryInData);
}

void SandtrayCommandInAnalyser::onRead(Bottle& sandtrayCommandInData){
   _deliberativeDriverYarpInterface->getFromSandtrayCommand(sandtrayCommandInData);
}

void SuggestedActionInAnalyser::onRead(Bottle& suggestedActionInData){
   _deliberativeDriverYarpInterface->getFromSuggestedAction(suggestedActionInData);
}

