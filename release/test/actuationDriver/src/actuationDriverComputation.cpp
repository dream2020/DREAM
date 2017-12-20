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

ActuationDriverThread::ActuationDriverThread(BufferedPort<VectorOf<int>>* disableRobotIn, BufferedPort<VectorOf<int>>* enableRobotIn, BufferedPort<VectorOf<int>>* gripIn, BufferedPort<VectorOf<double>>* moveHandIn, BufferedPort<VectorOf<double>>* moveHeadIn, BufferedPort<VectorOf<int>>* moveSequenceIn, BufferedPort<VectorOf<double>>* moveTorsoIn, BufferedPort<VectorOf<float>>* pointAtIn, BufferedPort<VectorOf<int>>* releaseIn, BufferedPort<Bottle>* sayIn, BufferedPort<Bottle>* sandtrayCommandIn, BufferedPort<Bottle>* actionFeedbackIn, BufferedPort<Bottle>* robotSensorsIn, BufferedPort<Bottle>* eyeBlinkingOut, BufferedPort<VectorOf<int>>* fallingReactionOut, BufferedPort<VectorOf<int>>* fallingReactionSpeechOut, BufferedPort<VectorOf<int>>* socialFacialExpressionOut, BufferedPort<VectorOf<int>>* socialReactionOut, BufferedPort<VectorOf<int>>* socialReactionSpeechOut, BufferedPort<Bottle>* robotMotorFeedbackOut, BufferedPort<Bottle>* selectedActionOut, BufferedPort<Bottle>* sandtrayReturnOut, BufferedPort<Bottle>* sensorFeedbackOut, BufferedPort<VectorOf<double>>* moveHeadOut, BufferedPort<VectorOf<int>>* fallingInterruptionOut, BufferedPort<VectorOf<double>>* elicitedAttentionOut){
	/* assign the input and output ports */
	_disableRobotIn = disableRobotIn;
	_enableRobotIn = enableRobotIn;
	_gripIn = gripIn;
	_moveHandIn = moveHandIn;
	_moveHeadIn = moveHeadIn;
	_moveSequenceIn = moveSequenceIn;
	_moveTorsoIn = moveTorsoIn;
	_pointAtIn = pointAtIn;
	_releaseIn = releaseIn;
	_sayIn = sayIn;
	_sandtrayCommandIn = sandtrayCommandIn;
	_actionFeedbackIn = actionFeedbackIn;
	_robotSensorsIn = robotSensorsIn;
	_eyeBlinkingOut = eyeBlinkingOut;
	_fallingReactionOut = fallingReactionOut;
	_fallingReactionSpeechOut = fallingReactionSpeechOut;
	_socialFacialExpressionOut = socialFacialExpressionOut;
	_socialReactionOut = socialReactionOut;
	_socialReactionSpeechOut = socialReactionSpeechOut;
	_robotMotorFeedbackOut = robotMotorFeedbackOut;
	_selectedActionOut = selectedActionOut;
	_sandtrayReturnOut = sandtrayReturnOut;
	_sensorFeedbackOut = sensorFeedbackOut;
	_moveHeadOut = moveHeadOut;
	_fallingInterruptionOut = fallingInterruptionOut;
	_elicitedAttentionOut = elicitedAttentionOut;
   
	_disableRobotInAnalyser = new DisableRobotInAnalyser();
	_enableRobotInAnalyser = new EnableRobotInAnalyser();
	_gripInAnalyser = new GripInAnalyser();
	_moveHandInAnalyser = new MoveHandInAnalyser();
	_moveHeadInAnalyser = new MoveHeadInAnalyser();
	_moveSequenceInAnalyser = new MoveSequenceInAnalyser();
	_moveTorsoInAnalyser = new MoveTorsoInAnalyser();
	_pointAtInAnalyser = new PointAtInAnalyser();
	_releaseInAnalyser = new ReleaseInAnalyser();
	_sayInAnalyser = new SayInAnalyser();
	_sandtrayCommandInAnalyser = new SandtrayCommandInAnalyser();
	_actionFeedbackInAnalyser = new ActionFeedbackInAnalyser();
	_robotSensorsInAnalyser = new RobotSensorsInAnalyser();
   
	disableRobotIn->useCallback(*_disableRobotInAnalyser);
	enableRobotIn->useCallback(*_enableRobotInAnalyser);
	gripIn->useCallback(*_gripInAnalyser);
	moveHandIn->useCallback(*_moveHandInAnalyser);
	moveHeadIn->useCallback(*_moveHeadInAnalyser);
	moveSequenceIn->useCallback(*_moveSequenceInAnalyser);
	moveTorsoIn->useCallback(*_moveTorsoInAnalyser);
	pointAtIn->useCallback(*_pointAtInAnalyser);
	releaseIn->useCallback(*_releaseInAnalyser);
	sayIn->useCallback(*_sayInAnalyser);
	sandtrayCommandIn->useCallback(*_sandtrayCommandInAnalyser);
	actionFeedbackIn->useCallback(*_actionFeedbackInAnalyser);
	robotSensorsIn->useCallback(*_robotSensorsInAnalyser);
}

void ActuationDriverThread::setYarpInterface(ActuationDriverYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_disableRobotInAnalyser->setActuationDriver(_yarpInterface);
	_enableRobotInAnalyser->setActuationDriver(_yarpInterface);
	_gripInAnalyser->setActuationDriver(_yarpInterface);
	_moveHandInAnalyser->setActuationDriver(_yarpInterface);
	_moveHeadInAnalyser->setActuationDriver(_yarpInterface);
	_moveSequenceInAnalyser->setActuationDriver(_yarpInterface);
	_moveTorsoInAnalyser->setActuationDriver(_yarpInterface);
	_pointAtInAnalyser->setActuationDriver(_yarpInterface);
	_releaseInAnalyser->setActuationDriver(_yarpInterface);
	_sayInAnalyser->setActuationDriver(_yarpInterface);
	_sandtrayCommandInAnalyser->setActuationDriver(_yarpInterface);
	_actionFeedbackInAnalyser->setActuationDriver(_yarpInterface);
	_robotSensorsInAnalyser->setActuationDriver(_yarpInterface);
}

bool ActuationDriverThread::threadInit(){
	/* initilize the thread */
	return true;
}

void ActuationDriverThread::run(){

}

void ActuationDriverThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void ActuationDriverThread::sendToEyeBlinking(Bottle data){
	Bottle& container = _eyeBlinkingOut->prepare();
	container = data;
	_eyeBlinkingOut->write();
}

void ActuationDriverThread::sendToFallingReaction(VectorOf<int> data){
	VectorOf<int>& container = _fallingReactionOut->prepare();
	container = data;
	_fallingReactionOut->write();
}

void ActuationDriverThread::sendToFallingReactionSpeech(VectorOf<int> data){
	VectorOf<int>& container = _fallingReactionSpeechOut->prepare();
	container = data;
	_fallingReactionSpeechOut->write();
}

void ActuationDriverThread::sendToSocialFacialExpression(VectorOf<int> data){
	VectorOf<int>& container = _socialFacialExpressionOut->prepare();
	container = data;
	_socialFacialExpressionOut->write();
}

void ActuationDriverThread::sendToSocialReaction(VectorOf<int> data){
	VectorOf<int>& container = _socialReactionOut->prepare();
	container = data;
	_socialReactionOut->write();
}

void ActuationDriverThread::sendToSocialReactionSpeech(VectorOf<int> data){
	VectorOf<int>& container = _socialReactionSpeechOut->prepare();
	container = data;
	_socialReactionSpeechOut->write();
}

void ActuationDriverThread::sendToRobotMotorFeedback(Bottle data){
	Bottle& container = _robotMotorFeedbackOut->prepare();
	container = data;
	_robotMotorFeedbackOut->write();
}

void ActuationDriverThread::sendToSelectedAction(Bottle data){
	Bottle& container = _selectedActionOut->prepare();
	container = data;
	_selectedActionOut->write();
}

void ActuationDriverThread::sendToSandtrayReturn(Bottle data){
	Bottle& container = _sandtrayReturnOut->prepare();
	container = data;
	_sandtrayReturnOut->write();
}

void ActuationDriverThread::sendToSensorFeedback(Bottle data){
	Bottle& container = _sensorFeedbackOut->prepare();
	container = data;
	_sensorFeedbackOut->write();
}

void ActuationDriverThread::sendToMoveHead(VectorOf<double> data){
	VectorOf<double>& container = _moveHeadOut->prepare();
	container = data;
	_moveHeadOut->write();
}

void ActuationDriverThread::sendToFallingInterruption(VectorOf<int> data){
	VectorOf<int>& container = _fallingInterruptionOut->prepare();
	container = data;
	_fallingInterruptionOut->write();
}

void ActuationDriverThread::sendToElicitedAttention(VectorOf<double> data){
	VectorOf<double>& container = _elicitedAttentionOut->prepare();
	container = data;
	_elicitedAttentionOut->write();
}


void DisableRobotInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void EnableRobotInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void GripInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void MoveHandInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void MoveHeadInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void MoveSequenceInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void MoveTorsoInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void PointAtInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void ReleaseInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void SayInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void SandtrayCommandInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void ActionFeedbackInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}

void RobotSensorsInAnalyser::setActuationDriver(ActuationDriverYarpInterface* yarpInterface){
   _actuationDriverYarpInterface=yarpInterface;
}



void DisableRobotInAnalyser::onRead(VectorOf<int>& disableRobotInData){
   _actuationDriverYarpInterface->getFromDisableRobot(disableRobotInData);
}

void EnableRobotInAnalyser::onRead(VectorOf<int>& enableRobotInData){
   _actuationDriverYarpInterface->getFromEnableRobot(enableRobotInData);
}

void GripInAnalyser::onRead(VectorOf<int>& gripInData){
   _actuationDriverYarpInterface->getFromGrip(gripInData);
}

void MoveHandInAnalyser::onRead(VectorOf<double>& moveHandInData){
   _actuationDriverYarpInterface->getFromMoveHand(moveHandInData);
}

void MoveHeadInAnalyser::onRead(VectorOf<double>& moveHeadInData){
   _actuationDriverYarpInterface->getFromMoveHead(moveHeadInData);
}

void MoveSequenceInAnalyser::onRead(VectorOf<int>& moveSequenceInData){
   _actuationDriverYarpInterface->getFromMoveSequence(moveSequenceInData);
}

void MoveTorsoInAnalyser::onRead(VectorOf<double>& moveTorsoInData){
   _actuationDriverYarpInterface->getFromMoveTorso(moveTorsoInData);
}

void PointAtInAnalyser::onRead(VectorOf<float>& pointAtInData){
   _actuationDriverYarpInterface->getFromPointAt(pointAtInData);
}

void ReleaseInAnalyser::onRead(VectorOf<int>& releaseInData){
   _actuationDriverYarpInterface->getFromRelease(releaseInData);
}

void SayInAnalyser::onRead(Bottle& sayInData){
   _actuationDriverYarpInterface->getFromSay(sayInData);
}

void SandtrayCommandInAnalyser::onRead(Bottle& sandtrayCommandInData){
   _actuationDriverYarpInterface->getFromSandtrayCommand(sandtrayCommandInData);
}

void ActionFeedbackInAnalyser::onRead(Bottle& actionFeedbackInData){
   _actuationDriverYarpInterface->getFromActionFeedback(actionFeedbackInData);
}

void RobotSensorsInAnalyser::onRead(Bottle& robotSensorsInData){
   _actuationDriverYarpInterface->getFromRobotSensors(robotSensorsInData);
}



