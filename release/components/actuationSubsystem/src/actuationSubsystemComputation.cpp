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

ActuationSubsystemThread::ActuationSubsystemThread(BufferedPort<Bottle>* eyeBlinkingIn, BufferedPort<VectorOf<int>>* fallingReactionIn, BufferedPort<VectorOf<int>>* fallingReactionSpeechIn, BufferedPort<VectorOf<int>>* socialFacialExpressionIn, BufferedPort<VectorOf<int>>* socialReactionIn, BufferedPort<VectorOf<int>>* socialReactionSpeechIn, BufferedPort<VectorOf<int>>* fallingInterruptionIn, BufferedPort<VectorOf<double>>* elicitedAttentionIn, BufferedPort<Bottle>* robotMotorFeedbackIn, BufferedPort<Bottle>* selectedActionIn, BufferedPort<Bottle>* sandtrayReturnIn, BufferedPort<Bottle>* sensorFeedbackIn, BufferedPort<Bottle>* emergencyIn, BufferedPort<Bottle>* robotSensorsOut, BufferedPort<VectorOf<int>>* disableRobotOut, BufferedPort<VectorOf<int>>* enableRobotOut, BufferedPort<VectorOf<int>>* gripOut, BufferedPort<VectorOf<double>>* moveHandOut, BufferedPort<VectorOf<double>>* moveHeadOut, BufferedPort<VectorOf<int>>* moveSequenceOut, BufferedPort<VectorOf<double>>* moveTorsoOut, BufferedPort<VectorOf<float>>* pointAtOut, BufferedPort<VectorOf<int>>* releaseOut, BufferedPort<Bottle>* sayOut, BufferedPort<Bottle>* sandtrayCommandOut, BufferedPort<Bottle>* actionFeedbackOut){
	/* assign the input and output ports */
	_eyeBlinkingIn = eyeBlinkingIn;
	_fallingReactionIn = fallingReactionIn;
	_fallingReactionSpeechIn = fallingReactionSpeechIn;
	_socialFacialExpressionIn = socialFacialExpressionIn;
	_socialReactionIn = socialReactionIn;
	_socialReactionSpeechIn = socialReactionSpeechIn;
	_fallingInterruptionIn = fallingInterruptionIn;
	_elicitedAttentionIn = elicitedAttentionIn;
	_robotMotorFeedbackIn = robotMotorFeedbackIn;
	_selectedActionIn = selectedActionIn;
	_sandtrayReturnIn = sandtrayReturnIn;
	_sensorFeedbackIn = sensorFeedbackIn;
	_emergencyIn = emergencyIn;
	_robotSensorsOut = robotSensorsOut;
	_disableRobotOut = disableRobotOut;
	_enableRobotOut = enableRobotOut;
	_gripOut = gripOut;
	_moveHandOut = moveHandOut;
	_moveHeadOut = moveHeadOut;
	_moveSequenceOut = moveSequenceOut;
	_moveTorsoOut = moveTorsoOut;
	_pointAtOut = pointAtOut;
	_releaseOut = releaseOut;
	_sayOut = sayOut;
	_sandtrayCommandOut = sandtrayCommandOut;
	_actionFeedbackOut = actionFeedbackOut;
   
	_eyeBlinkingInAnalyser = new EyeBlinkingInAnalyser();
	_fallingReactionInAnalyser = new FallingReactionInAnalyser();
	_fallingReactionSpeechInAnalyser = new FallingReactionSpeechInAnalyser();
	_socialFacialExpressionInAnalyser = new SocialFacialExpressionInAnalyser();
	_socialReactionInAnalyser = new SocialReactionInAnalyser();
	_socialReactionSpeechInAnalyser = new SocialReactionSpeechInAnalyser();
	_fallingInterruptionInAnalyser = new FallingInterruptionInAnalyser();
	_elicitedAttentionInAnalyser = new ElicitedAttentionInAnalyser();
	_robotMotorFeedbackInAnalyser = new RobotMotorFeedbackInAnalyser();
	_selectedActionInAnalyser = new SelectedActionInAnalyser();
	_sandtrayReturnInAnalyser = new SandtrayReturnInAnalyser();
	_sensorFeedbackInAnalyser = new SensorFeedbackInAnalyser();
	_emergencyInAnalyser = new EmergencyInAnalyser();
   
	eyeBlinkingIn->useCallback(*_eyeBlinkingInAnalyser);
	fallingReactionIn->useCallback(*_fallingReactionInAnalyser);
	fallingReactionSpeechIn->useCallback(*_fallingReactionSpeechInAnalyser);
	socialFacialExpressionIn->useCallback(*_socialFacialExpressionInAnalyser);
	socialReactionIn->useCallback(*_socialReactionInAnalyser);
	socialReactionSpeechIn->useCallback(*_socialReactionSpeechInAnalyser);
	fallingInterruptionIn->useCallback(*_fallingInterruptionInAnalyser);
	elicitedAttentionIn->useCallback(*_elicitedAttentionInAnalyser);
	robotMotorFeedbackIn->useCallback(*_robotMotorFeedbackInAnalyser);
	selectedActionIn->useCallback(*_selectedActionInAnalyser);
	sandtrayReturnIn->useCallback(*_sandtrayReturnInAnalyser);
	sensorFeedbackIn->useCallback(*_sensorFeedbackInAnalyser);
	emergencyIn->useCallback(*_emergencyInAnalyser);
}

void ActuationSubsystemThread::setYarpInterface(ActuationSubsystemYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_eyeBlinkingInAnalyser->setActuationSubsystem(_yarpInterface);
	_fallingReactionInAnalyser->setActuationSubsystem(_yarpInterface);
	_fallingReactionSpeechInAnalyser->setActuationSubsystem(_yarpInterface);
	_socialFacialExpressionInAnalyser->setActuationSubsystem(_yarpInterface);
	_socialReactionInAnalyser->setActuationSubsystem(_yarpInterface);
	_socialReactionSpeechInAnalyser->setActuationSubsystem(_yarpInterface);
	_fallingInterruptionInAnalyser->setActuationSubsystem(_yarpInterface);
	_elicitedAttentionInAnalyser->setActuationSubsystem(_yarpInterface);
	_robotMotorFeedbackInAnalyser->setActuationSubsystem(_yarpInterface);
	_selectedActionInAnalyser->setActuationSubsystem(_yarpInterface);
	_sandtrayReturnInAnalyser->setActuationSubsystem(_yarpInterface);
	_sensorFeedbackInAnalyser->setActuationSubsystem(_yarpInterface);
	_emergencyInAnalyser->setActuationSubsystem(_yarpInterface);
}

bool ActuationSubsystemThread::threadInit(){
	/* initilize the thread */
	return true;
}

void ActuationSubsystemThread::run(){

}

void ActuationSubsystemThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void ActuationSubsystemThread::sendToRobotSensors(Bottle data){
	Bottle& container = _robotSensorsOut->prepare();
	container = data;
	_robotSensorsOut->write();
}

void ActuationSubsystemThread::sendToDisableRobot(VectorOf<int> data){
	VectorOf<int>& container = _disableRobotOut->prepare();
	container = data;
	_disableRobotOut->write();
}

void ActuationSubsystemThread::sendToEnableRobot(VectorOf<int> data){
	VectorOf<int>& container = _enableRobotOut->prepare();
	container = data;
	_enableRobotOut->write();
}

void ActuationSubsystemThread::sendToGrip(VectorOf<int> data){
	VectorOf<int>& container = _gripOut->prepare();
	container = data;
	_gripOut->write();
}

void ActuationSubsystemThread::sendToMoveHand(VectorOf<double> data){
	VectorOf<double>& container = _moveHandOut->prepare();
	container = data;
	_moveHandOut->write();
}

void ActuationSubsystemThread::sendToMoveHead(VectorOf<double> data){
	VectorOf<double>& container = _moveHeadOut->prepare();
	container = data;
	_moveHeadOut->write();
}

void ActuationSubsystemThread::sendToMoveSequence(VectorOf<int> data){
	VectorOf<int>& container = _moveSequenceOut->prepare();
	container = data;
	_moveSequenceOut->write();
}

void ActuationSubsystemThread::sendToMoveTorso(VectorOf<double> data){
	VectorOf<double>& container = _moveTorsoOut->prepare();
	container = data;
	_moveTorsoOut->write();
}

void ActuationSubsystemThread::sendToPointAt(VectorOf<float> data){
	VectorOf<float>& container = _pointAtOut->prepare();
	container = data;
	_pointAtOut->write();
}

void ActuationSubsystemThread::sendToRelease(VectorOf<int> data){
	VectorOf<int>& container = _releaseOut->prepare();
	container = data;
	_releaseOut->write();
}

void ActuationSubsystemThread::sendToSay(Bottle data){
	Bottle& container = _sayOut->prepare();
	container = data;
	_sayOut->write();
}

void ActuationSubsystemThread::sendToSandtrayCommand(Bottle data){
	Bottle& container = _sandtrayCommandOut->prepare();
	container = data;
	_sandtrayCommandOut->write();
}

void ActuationSubsystemThread::sendToActionFeedback(Bottle data){
	Bottle& container = _actionFeedbackOut->prepare();
	container = data;
	_actionFeedbackOut->write();
}


void EyeBlinkingInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void FallingReactionInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void FallingReactionSpeechInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void SocialFacialExpressionInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void SocialReactionInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void SocialReactionSpeechInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void FallingInterruptionInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void ElicitedAttentionInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void RobotMotorFeedbackInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void SelectedActionInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void SandtrayReturnInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void SensorFeedbackInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}

void EmergencyInAnalyser::setActuationSubsystem(ActuationSubsystemYarpInterface* yarpInterface){
   _actuationSubsystemYarpInterface=yarpInterface;
}



void EyeBlinkingInAnalyser::onRead(Bottle& eyeBlinkingInData){
   _actuationSubsystemYarpInterface->getFromEyeBlinking(eyeBlinkingInData);
}

void FallingReactionInAnalyser::onRead(VectorOf<int>& fallingReactionInData){
   _actuationSubsystemYarpInterface->getFromFallingReaction(fallingReactionInData);
}

void FallingReactionSpeechInAnalyser::onRead(VectorOf<int>& fallingReactionSpeechInData){
   _actuationSubsystemYarpInterface->getFromFallingReactionSpeech(fallingReactionSpeechInData);
}

void SocialFacialExpressionInAnalyser::onRead(VectorOf<int>& socialFacialExpressionInData){
   _actuationSubsystemYarpInterface->getFromSocialFacialExpression(socialFacialExpressionInData);
}

void SocialReactionInAnalyser::onRead(VectorOf<int>& socialReactionInData){
   _actuationSubsystemYarpInterface->getFromSocialReaction(socialReactionInData);
}

void SocialReactionSpeechInAnalyser::onRead(VectorOf<int>& socialReactionSpeechInData){
   _actuationSubsystemYarpInterface->getFromSocialReactionSpeech(socialReactionSpeechInData);
}

void FallingInterruptionInAnalyser::onRead(VectorOf<int>& fallingInterruptionInData){
   _actuationSubsystemYarpInterface->getFromFallingInterruption(fallingInterruptionInData);
}

void ElicitedAttentionInAnalyser::onRead(VectorOf<double>& elicitedAttentionInData){
   _actuationSubsystemYarpInterface->getFromElicitedAttention(elicitedAttentionInData);
}

void RobotMotorFeedbackInAnalyser::onRead(Bottle& robotMotorFeedbackInData){
   _actuationSubsystemYarpInterface->getFromRobotMotorFeedback(robotMotorFeedbackInData);
}

void SelectedActionInAnalyser::onRead(Bottle& selectedActionInData){
   _actuationSubsystemYarpInterface->getFromSelectedAction(selectedActionInData);
}

void SandtrayReturnInAnalyser::onRead(Bottle& sandtrayReturnInData){
   _actuationSubsystemYarpInterface->getFromSandtrayReturn(sandtrayReturnInData);
}

void SensorFeedbackInAnalyser::onRead(Bottle& sensorFeedbackInData){
   _actuationSubsystemYarpInterface->getFromSensorFeedback(sensorFeedbackInData);
}

void EmergencyInAnalyser::onRead(Bottle& emergencyInData){
   _actuationSubsystemYarpInterface->getFromEmergency(emergencyInData);
}



