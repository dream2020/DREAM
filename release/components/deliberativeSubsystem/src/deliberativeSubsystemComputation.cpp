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

DeliberativeSubsystemThread::DeliberativeSubsystemThread(BufferedPort<VectorOf<double>>* getChildBehaviourIn, BufferedPort<VectorOf<double>>* getChildPerformanceIn, BufferedPort<VectorOf<int>>* checkMutualGazeIn, BufferedPort<VectorOf<double>>* getArmAngleIn, BufferedPort<VectorOf<double>>* getBodyIn, BufferedPort<VectorOf<double>>* getBodyPoseIn, BufferedPort<VectorOf<double>>* getEyeGazeIn, BufferedPort<VectorOf<double>>* getEyesIn, BufferedPort<VectorOf<double>>* getFacesIn, BufferedPort<VectorOf<double>>* getGripLocationIn, BufferedPort<VectorOf<double>>* getHandsIn, BufferedPort<VectorOf<double>>* getHeadIn, BufferedPort<VectorOf<double>>* getHeadGazeIn, BufferedPort<VectorOf<double>>* getObjectsIn, BufferedPort<VectorOf<double>>* getObjectTableDistanceIn, BufferedPort<VectorOf<double>>* getSoundDirectionIn, BufferedPort<VectorOf<double>>* identifyFaceIn, BufferedPort<VectorOf<int>>* identifyFaceExpressionIn, BufferedPort<VectorOf<double>>* identifyObjectIn, BufferedPort<VectorOf<double>>* identifyTrajectoryIn, BufferedPort<VectorOf<int>>* identifyVoiceIn, BufferedPort<Bottle>* recognizeSpeechIn, BufferedPort<VectorOf<double>>* trackFaceIn, BufferedPort<VectorOf<double>>* trackObjectIn, BufferedPort<Bottle>* actionFeedbackIn, BufferedPort<Bottle>* interventionCommandIn, BufferedPort<Bottle>* selectedActionIn, BufferedPort<Bottle>* userDelibIn, BufferedPort<Bottle>* sandtrayEventIn, BufferedPort<Bottle>* sandtrayReturnIn, BufferedPort<Bottle>* robotSensorsIn, BufferedPort<Bottle>* emergencyIn, BufferedPort<VectorOf<double>>* getEyeGazeOut, BufferedPort<VectorOf<double>>* getGripLocationOut, BufferedPort<VectorOf<double>>* getHeadGazeOut, BufferedPort<VectorOf<double>>* getObjectsOut, BufferedPort<VectorOf<double>>* getObjectTableDistanceOut, BufferedPort<VectorOf<double>>* getSoundDirectionOut, BufferedPort<VectorOf<double>>* identifyFaceOut, BufferedPort<VectorOf<double>>* identifyFaceExpressionOut, BufferedPort<VectorOf<double>>* identifyObjectOut, BufferedPort<VectorOf<int>>* identifyTrajectoryOut, BufferedPort<VectorOf<double>>* trackFaceOut, BufferedPort<VectorOf<double>>* trackHandOut, BufferedPort<VectorOf<double>>* trackObjectOut, BufferedPort<VectorOf<int>>* interactionEventOut, BufferedPort<VectorOf<int>>* getInterventionStatusOut, BufferedPort<Bottle>* attentionBiasOut, BufferedPort<VectorOf<int>>* commandSuccessOut, BufferedPort<VectorOf<int>>* startStopOut, BufferedPort<Bottle>* deliberativeFeedbackOut, BufferedPort<Bottle>* sensorySummaryOut, BufferedPort<Bottle>* sandtrayCommandOut, BufferedPort<Bottle>* suggestedActionOut){
	/* assign the input and output ports */
	_getChildBehaviourIn = getChildBehaviourIn;
	_getChildPerformanceIn = getChildPerformanceIn;
	_checkMutualGazeIn = checkMutualGazeIn;
	_getArmAngleIn = getArmAngleIn;
	_getBodyIn = getBodyIn;
	_getBodyPoseIn = getBodyPoseIn;
	_getEyeGazeIn = getEyeGazeIn;
	_getEyesIn = getEyesIn;
	_getFacesIn = getFacesIn;
	_getGripLocationIn = getGripLocationIn;
	_getHandsIn = getHandsIn;
	_getHeadIn = getHeadIn;
	_getHeadGazeIn = getHeadGazeIn;
	_getObjectsIn = getObjectsIn;
	_getObjectTableDistanceIn = getObjectTableDistanceIn;
	_getSoundDirectionIn = getSoundDirectionIn;
	_identifyFaceIn = identifyFaceIn;
	_identifyFaceExpressionIn = identifyFaceExpressionIn;
	_identifyObjectIn = identifyObjectIn;
	_identifyTrajectoryIn = identifyTrajectoryIn;
	_identifyVoiceIn = identifyVoiceIn;
	_recognizeSpeechIn = recognizeSpeechIn;
	_trackFaceIn = trackFaceIn;
	_trackObjectIn = trackObjectIn;
	_actionFeedbackIn = actionFeedbackIn;
	_interventionCommandIn = interventionCommandIn;
	_selectedActionIn = selectedActionIn;
	_userDelibIn = userDelibIn;
	_sandtrayEventIn = sandtrayEventIn;
	_sandtrayReturnIn = sandtrayReturnIn;
	_robotSensorsIn = robotSensorsIn;
	_emergencyIn = emergencyIn;
	_getEyeGazeOut = getEyeGazeOut;
	_getGripLocationOut = getGripLocationOut;
	_getHeadGazeOut = getHeadGazeOut;
	_getObjectsOut = getObjectsOut;
	_getObjectTableDistanceOut = getObjectTableDistanceOut;
	_getSoundDirectionOut = getSoundDirectionOut;
	_identifyFaceOut = identifyFaceOut;
	_identifyFaceExpressionOut = identifyFaceExpressionOut;
	_identifyObjectOut = identifyObjectOut;
	_identifyTrajectoryOut = identifyTrajectoryOut;
	_trackFaceOut = trackFaceOut;
	_trackHandOut = trackHandOut;
	_trackObjectOut = trackObjectOut;
	_interactionEventOut = interactionEventOut;
	_getInterventionStatusOut = getInterventionStatusOut;
	_attentionBiasOut = attentionBiasOut;
	_commandSuccessOut = commandSuccessOut;
	_startStopOut = startStopOut;
	_deliberativeFeedbackOut = deliberativeFeedbackOut;
	_sensorySummaryOut = sensorySummaryOut;
	_sandtrayCommandOut = sandtrayCommandOut;
	_suggestedActionOut = suggestedActionOut;
   
	_getChildBehaviourInAnalyser = new GetChildBehaviourInAnalyser();
	_getChildPerformanceInAnalyser = new GetChildPerformanceInAnalyser();
	_checkMutualGazeInAnalyser = new CheckMutualGazeInAnalyser();
	_getArmAngleInAnalyser = new GetArmAngleInAnalyser();
	_getBodyInAnalyser = new GetBodyInAnalyser();
	_getBodyPoseInAnalyser = new GetBodyPoseInAnalyser();
	_getEyeGazeInAnalyser = new GetEyeGazeInAnalyser();
	_getEyesInAnalyser = new GetEyesInAnalyser();
	_getFacesInAnalyser = new GetFacesInAnalyser();
	_getGripLocationInAnalyser = new GetGripLocationInAnalyser();
	_getHandsInAnalyser = new GetHandsInAnalyser();
	_getHeadInAnalyser = new GetHeadInAnalyser();
	_getHeadGazeInAnalyser = new GetHeadGazeInAnalyser();
	_getObjectsInAnalyser = new GetObjectsInAnalyser();
	_getObjectTableDistanceInAnalyser = new GetObjectTableDistanceInAnalyser();
	_getSoundDirectionInAnalyser = new GetSoundDirectionInAnalyser();
	_identifyFaceInAnalyser = new IdentifyFaceInAnalyser();
	_identifyFaceExpressionInAnalyser = new IdentifyFaceExpressionInAnalyser();
	_identifyObjectInAnalyser = new IdentifyObjectInAnalyser();
	_identifyTrajectoryInAnalyser = new IdentifyTrajectoryInAnalyser();
	_identifyVoiceInAnalyser = new IdentifyVoiceInAnalyser();
	_recognizeSpeechInAnalyser = new RecognizeSpeechInAnalyser();
	_trackFaceInAnalyser = new TrackFaceInAnalyser();
	_trackObjectInAnalyser = new TrackObjectInAnalyser();
	_actionFeedbackInAnalyser = new ActionFeedbackInAnalyser();
	_interventionCommandInAnalyser = new InterventionCommandInAnalyser();
	_selectedActionInAnalyser = new SelectedActionInAnalyser();
	_userDelibInAnalyser = new UserDelibInAnalyser();
	_sandtrayEventInAnalyser = new SandtrayEventInAnalyser();
	_sandtrayReturnInAnalyser = new SandtrayReturnInAnalyser();
	_robotSensorsInAnalyser = new RobotSensorsInAnalyser();
	_emergencyInAnalyser = new EmergencyInAnalyser();
   
	getChildBehaviourIn->useCallback(*_getChildBehaviourInAnalyser);
	getChildPerformanceIn->useCallback(*_getChildPerformanceInAnalyser);
	checkMutualGazeIn->useCallback(*_checkMutualGazeInAnalyser);
	getArmAngleIn->useCallback(*_getArmAngleInAnalyser);
	getBodyIn->useCallback(*_getBodyInAnalyser);
	getBodyPoseIn->useCallback(*_getBodyPoseInAnalyser);
	getEyeGazeIn->useCallback(*_getEyeGazeInAnalyser);
	getEyesIn->useCallback(*_getEyesInAnalyser);
	getFacesIn->useCallback(*_getFacesInAnalyser);
	getGripLocationIn->useCallback(*_getGripLocationInAnalyser);
	getHandsIn->useCallback(*_getHandsInAnalyser);
	getHeadIn->useCallback(*_getHeadInAnalyser);
	getHeadGazeIn->useCallback(*_getHeadGazeInAnalyser);
	getObjectsIn->useCallback(*_getObjectsInAnalyser);
	getObjectTableDistanceIn->useCallback(*_getObjectTableDistanceInAnalyser);
	getSoundDirectionIn->useCallback(*_getSoundDirectionInAnalyser);
	identifyFaceIn->useCallback(*_identifyFaceInAnalyser);
	identifyFaceExpressionIn->useCallback(*_identifyFaceExpressionInAnalyser);
	identifyObjectIn->useCallback(*_identifyObjectInAnalyser);
	identifyTrajectoryIn->useCallback(*_identifyTrajectoryInAnalyser);
	identifyVoiceIn->useCallback(*_identifyVoiceInAnalyser);
	recognizeSpeechIn->useCallback(*_recognizeSpeechInAnalyser);
	trackFaceIn->useCallback(*_trackFaceInAnalyser);
	trackObjectIn->useCallback(*_trackObjectInAnalyser);
	actionFeedbackIn->useCallback(*_actionFeedbackInAnalyser);
	interventionCommandIn->useCallback(*_interventionCommandInAnalyser);
	selectedActionIn->useCallback(*_selectedActionInAnalyser);
	userDelibIn->useCallback(*_userDelibInAnalyser);
	sandtrayEventIn->useCallback(*_sandtrayEventInAnalyser);
	sandtrayReturnIn->useCallback(*_sandtrayReturnInAnalyser);
	robotSensorsIn->useCallback(*_robotSensorsInAnalyser);
	emergencyIn->useCallback(*_emergencyInAnalyser);
}

void DeliberativeSubsystemThread::setYarpInterface(DeliberativeSubsystemYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_getChildBehaviourInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getChildPerformanceInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_checkMutualGazeInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getArmAngleInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getBodyInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getBodyPoseInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getEyeGazeInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getEyesInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getFacesInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getGripLocationInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getHandsInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getHeadInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getHeadGazeInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getObjectsInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getObjectTableDistanceInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_getSoundDirectionInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_identifyFaceInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_identifyFaceExpressionInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_identifyObjectInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_identifyTrajectoryInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_identifyVoiceInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_recognizeSpeechInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_trackFaceInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_trackObjectInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_actionFeedbackInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_interventionCommandInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_selectedActionInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_userDelibInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_sandtrayEventInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_sandtrayReturnInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_robotSensorsInAnalyser->setDeliberativeSubsystem(_yarpInterface);
	_emergencyInAnalyser->setDeliberativeSubsystem(_yarpInterface);
}

bool DeliberativeSubsystemThread::threadInit(){
	/* initilize the thread */
	return true;
}

void DeliberativeSubsystemThread::run(){

}

void DeliberativeSubsystemThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void DeliberativeSubsystemThread::sendToGetEyeGaze(VectorOf<double> data){
	VectorOf<double>& container = _getEyeGazeOut->prepare();
	container = data;
	_getEyeGazeOut->write();
}

void DeliberativeSubsystemThread::sendToGetGripLocation(VectorOf<double> data){
	VectorOf<double>& container = _getGripLocationOut->prepare();
	container = data;
	_getGripLocationOut->write();
}

void DeliberativeSubsystemThread::sendToGetHeadGaze(VectorOf<double> data){
	VectorOf<double>& container = _getHeadGazeOut->prepare();
	container = data;
	_getHeadGazeOut->write();
}

void DeliberativeSubsystemThread::sendToGetObjects(VectorOf<double> data){
	VectorOf<double>& container = _getObjectsOut->prepare();
	container = data;
	_getObjectsOut->write();
}

void DeliberativeSubsystemThread::sendToGetObjectTableDistance(VectorOf<double> data){
	VectorOf<double>& container = _getObjectTableDistanceOut->prepare();
	container = data;
	_getObjectTableDistanceOut->write();
}

void DeliberativeSubsystemThread::sendToGetSoundDirection(VectorOf<double> data){
	VectorOf<double>& container = _getSoundDirectionOut->prepare();
	container = data;
	_getSoundDirectionOut->write();
}

void DeliberativeSubsystemThread::sendToIdentifyFace(VectorOf<double> data){
	VectorOf<double>& container = _identifyFaceOut->prepare();
	container = data;
	_identifyFaceOut->write();
}

void DeliberativeSubsystemThread::sendToIdentifyFaceExpression(VectorOf<double> data){
	VectorOf<double>& container = _identifyFaceExpressionOut->prepare();
	container = data;
	_identifyFaceExpressionOut->write();
}

void DeliberativeSubsystemThread::sendToIdentifyObject(VectorOf<double> data){
	VectorOf<double>& container = _identifyObjectOut->prepare();
	container = data;
	_identifyObjectOut->write();
}

void DeliberativeSubsystemThread::sendToIdentifyTrajectory(VectorOf<int> data){
	VectorOf<int>& container = _identifyTrajectoryOut->prepare();
	container = data;
	_identifyTrajectoryOut->write();
}

void DeliberativeSubsystemThread::sendToTrackFace(VectorOf<double> data){
	VectorOf<double>& container = _trackFaceOut->prepare();
	container = data;
	_trackFaceOut->write();
}

void DeliberativeSubsystemThread::sendToTrackHand(VectorOf<double> data){
	VectorOf<double>& container = _trackHandOut->prepare();
	container = data;
	_trackHandOut->write();
}

void DeliberativeSubsystemThread::sendToTrackObject(VectorOf<double> data){
	VectorOf<double>& container = _trackObjectOut->prepare();
	container = data;
	_trackObjectOut->write();
}

void DeliberativeSubsystemThread::sendToInteractionEvent(VectorOf<int> data){
	VectorOf<int>& container = _interactionEventOut->prepare();
	container = data;
	_interactionEventOut->write();
}

void DeliberativeSubsystemThread::sendToGetInterventionStatus(VectorOf<int> data){
	VectorOf<int>& container = _getInterventionStatusOut->prepare();
	container = data;
	_getInterventionStatusOut->write();
}

void DeliberativeSubsystemThread::sendToAttentionBias(Bottle data){
	Bottle& container = _attentionBiasOut->prepare();
	container = data;
	_attentionBiasOut->write();
}

void DeliberativeSubsystemThread::sendToCommandSuccess(VectorOf<int> data){
	VectorOf<int>& container = _commandSuccessOut->prepare();
	container = data;
	_commandSuccessOut->write();
}

void DeliberativeSubsystemThread::sendToStartStop(VectorOf<int> data){
	VectorOf<int>& container = _startStopOut->prepare();
	container = data;
	_startStopOut->write();
}

void DeliberativeSubsystemThread::sendToDeliberativeFeedback(Bottle data){
	Bottle& container = _deliberativeFeedbackOut->prepare();
	container = data;
	_deliberativeFeedbackOut->write();
}

void DeliberativeSubsystemThread::sendToSensorySummary(Bottle data){
	Bottle& container = _sensorySummaryOut->prepare();
	container = data;
	_sensorySummaryOut->write();
}

void DeliberativeSubsystemThread::sendToSandtrayCommand(Bottle data){
	Bottle& container = _sandtrayCommandOut->prepare();
	container = data;
	_sandtrayCommandOut->write();
}

void DeliberativeSubsystemThread::sendToSuggestedAction(Bottle data){
	Bottle& container = _suggestedActionOut->prepare();
	container = data;
	_suggestedActionOut->write();
}


void GetChildBehaviourInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetChildPerformanceInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void CheckMutualGazeInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetArmAngleInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetBodyInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetBodyPoseInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetEyeGazeInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetEyesInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetFacesInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetGripLocationInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetHandsInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetHeadInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetHeadGazeInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetObjectsInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetObjectTableDistanceInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void GetSoundDirectionInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void IdentifyFaceInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void IdentifyFaceExpressionInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void IdentifyObjectInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void IdentifyTrajectoryInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void IdentifyVoiceInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void RecognizeSpeechInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void TrackFaceInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void TrackObjectInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void ActionFeedbackInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void InterventionCommandInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void SelectedActionInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void UserDelibInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void SandtrayEventInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void SandtrayReturnInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void RobotSensorsInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}

void EmergencyInAnalyser::setDeliberativeSubsystem(DeliberativeSubsystemYarpInterface* yarpInterface){
   _deliberativeSubsystemYarpInterface=yarpInterface;
}



void GetChildBehaviourInAnalyser::onRead(VectorOf<double>& getChildBehaviourInData){
   _deliberativeSubsystemYarpInterface->getFromGetChildBehaviour(getChildBehaviourInData);
}

void GetChildPerformanceInAnalyser::onRead(VectorOf<double>& getChildPerformanceInData){
   _deliberativeSubsystemYarpInterface->getFromGetChildPerformance(getChildPerformanceInData);
}

void CheckMutualGazeInAnalyser::onRead(VectorOf<int>& checkMutualGazeInData){
   _deliberativeSubsystemYarpInterface->getFromCheckMutualGaze(checkMutualGazeInData);
}

void GetArmAngleInAnalyser::onRead(VectorOf<double>& getArmAngleInData){
   _deliberativeSubsystemYarpInterface->getFromGetArmAngle(getArmAngleInData);
}

void GetBodyInAnalyser::onRead(VectorOf<double>& getBodyInData){
   _deliberativeSubsystemYarpInterface->getFromGetBody(getBodyInData);
}

void GetBodyPoseInAnalyser::onRead(VectorOf<double>& getBodyPoseInData){
   _deliberativeSubsystemYarpInterface->getFromGetBodyPose(getBodyPoseInData);
}

void GetEyeGazeInAnalyser::onRead(VectorOf<double>& getEyeGazeInData){
   _deliberativeSubsystemYarpInterface->getFromGetEyeGaze(getEyeGazeInData);
}

void GetEyesInAnalyser::onRead(VectorOf<double>& getEyesInData){
   _deliberativeSubsystemYarpInterface->getFromGetEyes(getEyesInData);
}

void GetFacesInAnalyser::onRead(VectorOf<double>& getFacesInData){
   _deliberativeSubsystemYarpInterface->getFromGetFaces(getFacesInData);
}

void GetGripLocationInAnalyser::onRead(VectorOf<double>& getGripLocationInData){
   _deliberativeSubsystemYarpInterface->getFromGetGripLocation(getGripLocationInData);
}

void GetHandsInAnalyser::onRead(VectorOf<double>& getHandsInData){
   _deliberativeSubsystemYarpInterface->getFromGetHands(getHandsInData);
}

void GetHeadInAnalyser::onRead(VectorOf<double>& getHeadInData){
   _deliberativeSubsystemYarpInterface->getFromGetHead(getHeadInData);
}

void GetHeadGazeInAnalyser::onRead(VectorOf<double>& getHeadGazeInData){
   _deliberativeSubsystemYarpInterface->getFromGetHeadGaze(getHeadGazeInData);
}

void GetObjectsInAnalyser::onRead(VectorOf<double>& getObjectsInData){
   _deliberativeSubsystemYarpInterface->getFromGetObjects(getObjectsInData);
}

void GetObjectTableDistanceInAnalyser::onRead(VectorOf<double>& getObjectTableDistanceInData){
   _deliberativeSubsystemYarpInterface->getFromGetObjectTableDistance(getObjectTableDistanceInData);
}

void GetSoundDirectionInAnalyser::onRead(VectorOf<double>& getSoundDirectionInData){
   _deliberativeSubsystemYarpInterface->getFromGetSoundDirection(getSoundDirectionInData);
}

void IdentifyFaceInAnalyser::onRead(VectorOf<double>& identifyFaceInData){
   _deliberativeSubsystemYarpInterface->getFromIdentifyFace(identifyFaceInData);
}

void IdentifyFaceExpressionInAnalyser::onRead(VectorOf<int>& identifyFaceExpressionInData){
   _deliberativeSubsystemYarpInterface->getFromIdentifyFaceExpression(identifyFaceExpressionInData);
}

void IdentifyObjectInAnalyser::onRead(VectorOf<double>& identifyObjectInData){
   _deliberativeSubsystemYarpInterface->getFromIdentifyObject(identifyObjectInData);
}

void IdentifyTrajectoryInAnalyser::onRead(VectorOf<double>& identifyTrajectoryInData){
   _deliberativeSubsystemYarpInterface->getFromIdentifyTrajectory(identifyTrajectoryInData);
}

void IdentifyVoiceInAnalyser::onRead(VectorOf<int>& identifyVoiceInData){
   _deliberativeSubsystemYarpInterface->getFromIdentifyVoice(identifyVoiceInData);
}

void RecognizeSpeechInAnalyser::onRead(Bottle& recognizeSpeechInData){
   _deliberativeSubsystemYarpInterface->getFromRecognizeSpeech(recognizeSpeechInData);
}

void TrackFaceInAnalyser::onRead(VectorOf<double>& trackFaceInData){
   _deliberativeSubsystemYarpInterface->getFromTrackFace(trackFaceInData);
}

void TrackObjectInAnalyser::onRead(VectorOf<double>& trackObjectInData){
   _deliberativeSubsystemYarpInterface->getFromTrackObject(trackObjectInData);
}

void ActionFeedbackInAnalyser::onRead(Bottle& actionFeedbackInData){
   _deliberativeSubsystemYarpInterface->getFromActionFeedback(actionFeedbackInData);
}

void InterventionCommandInAnalyser::onRead(Bottle& interventionCommandInData){
   _deliberativeSubsystemYarpInterface->getFromInterventionCommand(interventionCommandInData);
}

void SelectedActionInAnalyser::onRead(Bottle& selectedActionInData){
   _deliberativeSubsystemYarpInterface->getFromSelectedAction(selectedActionInData);
}

void UserDelibInAnalyser::onRead(Bottle& userDelibInData){
   _deliberativeSubsystemYarpInterface->getFromUserDelib(userDelibInData);
}

void SandtrayEventInAnalyser::onRead(Bottle& sandtrayEventInData){
   _deliberativeSubsystemYarpInterface->getFromSandtrayEvent(sandtrayEventInData);
}

void SandtrayReturnInAnalyser::onRead(Bottle& sandtrayReturnInData){
   _deliberativeSubsystemYarpInterface->getFromSandtrayReturn(sandtrayReturnInData);
}

void RobotSensorsInAnalyser::onRead(Bottle& robotSensorsInData){
   _deliberativeSubsystemYarpInterface->getFromRobotSensors(robotSensorsInData);
}

void EmergencyInAnalyser::onRead(Bottle& emergencyInData){
   _deliberativeSubsystemYarpInterface->getFromEmergency(emergencyInData);
}



