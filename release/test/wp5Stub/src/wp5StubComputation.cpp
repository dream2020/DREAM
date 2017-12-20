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

Wp5StubThread::Wp5StubThread(BufferedPort<VectorOf<int>>* getInterventionStatusIn, BufferedPort<VectorOf<int>>* interactionEventIn, BufferedPort<VectorOf<double>>* getBodyIn, BufferedPort<VectorOf<double>>* getBodyPoseIn, BufferedPort<VectorOf<double>>* getEyeGazeIn, BufferedPort<VectorOf<double>>* identifyTrajectoryIn, BufferedPort<VectorOf<int>>* identifyVoiceIn, BufferedPort<VectorOf<double>>* identifyFaceExpressionIn, BufferedPort<VectorOf<int>>* checkMutualGazeIn, BufferedPort<VectorOf<double>>* engagementFlagOut, BufferedPort<VectorOf<double>>* getChildBehaviourOut, BufferedPort<VectorOf<double>>* getChildPerformanceOut, BufferedPort<VectorOf<int>>* identifyTrajectoryOut){
	/* assign the input and output ports */
	_getInterventionStatusIn = getInterventionStatusIn;
	_interactionEventIn = interactionEventIn;
	_getBodyIn = getBodyIn;
	_getBodyPoseIn = getBodyPoseIn;
	_getEyeGazeIn = getEyeGazeIn;
	_identifyTrajectoryIn = identifyTrajectoryIn;
	_identifyVoiceIn = identifyVoiceIn;
	_identifyFaceExpressionIn = identifyFaceExpressionIn;
	_checkMutualGazeIn = checkMutualGazeIn;
	_engagementFlagOut = engagementFlagOut;
	_getChildBehaviourOut = getChildBehaviourOut;
	_getChildPerformanceOut = getChildPerformanceOut;
	_identifyTrajectoryOut = identifyTrajectoryOut;
   
	_getInterventionStatusInAnalyser = new GetInterventionStatusInAnalyser();
	_interactionEventInAnalyser = new InteractionEventInAnalyser();
	_getBodyInAnalyser = new GetBodyInAnalyser();
	_getBodyPoseInAnalyser = new GetBodyPoseInAnalyser();
	_getEyeGazeInAnalyser = new GetEyeGazeInAnalyser();
	_identifyTrajectoryInAnalyser = new IdentifyTrajectoryInAnalyser();
	_identifyVoiceInAnalyser = new IdentifyVoiceInAnalyser();
	_identifyFaceExpressionInAnalyser = new IdentifyFaceExpressionInAnalyser();
	_checkMutualGazeInAnalyser = new CheckMutualGazeInAnalyser();
   
	getInterventionStatusIn->useCallback(*_getInterventionStatusInAnalyser);
	interactionEventIn->useCallback(*_interactionEventInAnalyser);
	getBodyIn->useCallback(*_getBodyInAnalyser);
	getBodyPoseIn->useCallback(*_getBodyPoseInAnalyser);
	getEyeGazeIn->useCallback(*_getEyeGazeInAnalyser);
	identifyTrajectoryIn->useCallback(*_identifyTrajectoryInAnalyser);
	identifyVoiceIn->useCallback(*_identifyVoiceInAnalyser);
	identifyFaceExpressionIn->useCallback(*_identifyFaceExpressionInAnalyser);
	checkMutualGazeIn->useCallback(*_checkMutualGazeInAnalyser);
}

void Wp5StubThread::setYarpInterface(Wp5StubYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_getInterventionStatusInAnalyser->setWp5Stub(_yarpInterface);
	_interactionEventInAnalyser->setWp5Stub(_yarpInterface);
	_getBodyInAnalyser->setWp5Stub(_yarpInterface);
	_getBodyPoseInAnalyser->setWp5Stub(_yarpInterface);
	_getEyeGazeInAnalyser->setWp5Stub(_yarpInterface);
	_identifyTrajectoryInAnalyser->setWp5Stub(_yarpInterface);
	_identifyVoiceInAnalyser->setWp5Stub(_yarpInterface);
	_identifyFaceExpressionInAnalyser->setWp5Stub(_yarpInterface);
	_checkMutualGazeInAnalyser->setWp5Stub(_yarpInterface);
}

bool Wp5StubThread::threadInit(){
	/* initilize the thread */
	return true;
}

void Wp5StubThread::run(){

}

void Wp5StubThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void Wp5StubThread::sendToEngagementFlag(VectorOf<double> data){
	VectorOf<double>& container = _engagementFlagOut->prepare();
	container = data;
	_engagementFlagOut->write();
}

void Wp5StubThread::sendToGetChildBehaviour(VectorOf<double> data){
	VectorOf<double>& container = _getChildBehaviourOut->prepare();
	container = data;
	_getChildBehaviourOut->write();
}

void Wp5StubThread::sendToGetChildPerformance(VectorOf<double> data){
	VectorOf<double>& container = _getChildPerformanceOut->prepare();
	container = data;
	_getChildPerformanceOut->write();
}

void Wp5StubThread::sendToIdentifyTrajectory(VectorOf<int> data){
	VectorOf<int>& container = _identifyTrajectoryOut->prepare();
	container = data;
	_identifyTrajectoryOut->write();
}


void GetInterventionStatusInAnalyser::setWp5Stub(Wp5StubYarpInterface* yarpInterface){
   _wp5StubYarpInterface=yarpInterface;
}

void InteractionEventInAnalyser::setWp5Stub(Wp5StubYarpInterface* yarpInterface){
   _wp5StubYarpInterface=yarpInterface;
}

void GetBodyInAnalyser::setWp5Stub(Wp5StubYarpInterface* yarpInterface){
   _wp5StubYarpInterface=yarpInterface;
}

void GetBodyPoseInAnalyser::setWp5Stub(Wp5StubYarpInterface* yarpInterface){
   _wp5StubYarpInterface=yarpInterface;
}

void GetEyeGazeInAnalyser::setWp5Stub(Wp5StubYarpInterface* yarpInterface){
   _wp5StubYarpInterface=yarpInterface;
}

void IdentifyTrajectoryInAnalyser::setWp5Stub(Wp5StubYarpInterface* yarpInterface){
   _wp5StubYarpInterface=yarpInterface;
}

void IdentifyVoiceInAnalyser::setWp5Stub(Wp5StubYarpInterface* yarpInterface){
   _wp5StubYarpInterface=yarpInterface;
}

void IdentifyFaceExpressionInAnalyser::setWp5Stub(Wp5StubYarpInterface* yarpInterface){
   _wp5StubYarpInterface=yarpInterface;
}

void CheckMutualGazeInAnalyser::setWp5Stub(Wp5StubYarpInterface* yarpInterface){
   _wp5StubYarpInterface=yarpInterface;
}



void GetInterventionStatusInAnalyser::onRead(VectorOf<int>& getInterventionStatusInData){
   _wp5StubYarpInterface->getFromGetInterventionStatus(getInterventionStatusInData);
}

void InteractionEventInAnalyser::onRead(VectorOf<int>& interactionEventInData){
   _wp5StubYarpInterface->getFromInteractionEvent(interactionEventInData);
}

void GetBodyInAnalyser::onRead(VectorOf<double>& getBodyInData){
   _wp5StubYarpInterface->getFromGetBody(getBodyInData);
}

void GetBodyPoseInAnalyser::onRead(VectorOf<double>& getBodyPoseInData){
   _wp5StubYarpInterface->getFromGetBodyPose(getBodyPoseInData);
}

void GetEyeGazeInAnalyser::onRead(VectorOf<double>& getEyeGazeInData){
   _wp5StubYarpInterface->getFromGetEyeGaze(getEyeGazeInData);
}

void IdentifyTrajectoryInAnalyser::onRead(VectorOf<double>& identifyTrajectoryInData){
   _wp5StubYarpInterface->getFromIdentifyTrajectory(identifyTrajectoryInData);
}

void IdentifyVoiceInAnalyser::onRead(VectorOf<int>& identifyVoiceInData){
   _wp5StubYarpInterface->getFromIdentifyVoice(identifyVoiceInData);
}

void IdentifyFaceExpressionInAnalyser::onRead(VectorOf<double>& identifyFaceExpressionInData){
   _wp5StubYarpInterface->getFromIdentifyFaceExpression(identifyFaceExpressionInData);
}

void CheckMutualGazeInAnalyser::onRead(VectorOf<int>& checkMutualGazeInData){
   _wp5StubYarpInterface->getFromCheckMutualGaze(checkMutualGazeInData);
}



