/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Serge Thill, University of Skövde 
 * Email:   serge.thill@his.se 
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
  * 08/11/2016 Version 1.0: ... (Serge Thill)
  */ 

#include "assessChildPerformance.h"
#include "assessChildPerformanceYarpInterface.h"

AssessChildPerformanceThread::AssessChildPerformanceThread(BufferedPort<VectorOf<int> >* identifyVoiceIn, BufferedPort<VectorOf<double> >* getEyeGazeIn, BufferedPort<VectorOf<double> >* getBodyPoseIn, BufferedPort<VectorOf<double> >* identifyTrajectoryIn, BufferedPort<VectorOf<int> >* getInterventionStatusIn, BufferedPort<VectorOf<int> >* interactionEventIn, BufferedPort<VectorOf<double> >* getChildBehaviourOut, BufferedPort<VectorOf<int> >* identifyTrajectoryOut, BufferedPort<VectorOf<double> >* getChildPerformanceOut){
	/* assign the input and output ports */
	_identifyVoiceIn = identifyVoiceIn;
	_getEyeGazeIn = getEyeGazeIn;
	_getBodyPoseIn = getBodyPoseIn;
	_identifyTrajectoryIn = identifyTrajectoryIn;
	_getInterventionStatusIn = getInterventionStatusIn;
	_interactionEventIn = interactionEventIn;
	_getChildBehaviourOut = getChildBehaviourOut;
	_identifyTrajectoryOut = identifyTrajectoryOut;
	_getChildPerformanceOut = getChildPerformanceOut;
   
	_identifyVoiceInAnalyser = new IdentifyVoiceInAnalyser();
	_getEyeGazeInAnalyser = new GetEyeGazeInAnalyser();
	_getBodyPoseInAnalyser = new GetBodyPoseInAnalyser();
	_identifyTrajectoryInAnalyser = new IdentifyTrajectoryInAnalyser();
	_getInterventionStatusInAnalyser = new GetInterventionStatusInAnalyser();
	_interactionEventInAnalyser = new InteractionEventInAnalyser();
   
	identifyVoiceIn->useCallback(*_identifyVoiceInAnalyser);
	getEyeGazeIn->useCallback(*_getEyeGazeInAnalyser);
	getBodyPoseIn->useCallback(*_getBodyPoseInAnalyser);
	identifyTrajectoryIn->useCallback(*_identifyTrajectoryInAnalyser);
	getInterventionStatusIn->useCallback(*_getInterventionStatusInAnalyser);
	interactionEventIn->useCallback(*_interactionEventInAnalyser);
}

void AssessChildPerformanceThread::setYarpInterface(AssessChildPerformanceYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_identifyVoiceInAnalyser->setAssessChildPerformance(_yarpInterface);
	_getEyeGazeInAnalyser->setAssessChildPerformance(_yarpInterface);
	_getBodyPoseInAnalyser->setAssessChildPerformance(_yarpInterface);
	_identifyTrajectoryInAnalyser->setAssessChildPerformance(_yarpInterface);
	_getInterventionStatusInAnalyser->setAssessChildPerformance(_yarpInterface);
	_interactionEventInAnalyser->setAssessChildPerformance(_yarpInterface);
}

bool AssessChildPerformanceThread::threadInit(){
	/* initilize the thread */
	return true;
}

void AssessChildPerformanceThread::run(){

}

void AssessChildPerformanceThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void AssessChildPerformanceThread::sendToGetChildBehaviour(VectorOf<double> data){
	VectorOf<double>& container = _getChildBehaviourOut->prepare();
	container = data;
	_getChildBehaviourOut->write();
}

void AssessChildPerformanceThread::sendToIdentifyTrajectory(VectorOf<int> data){
	VectorOf<int>& container = _identifyTrajectoryOut->prepare();
	container = data;
	_identifyTrajectoryOut->write();
}

void AssessChildPerformanceThread::sendToGetChildPerformance(VectorOf<double> data){
	VectorOf<double>& container = _getChildPerformanceOut->prepare();
	container = data;
	_getChildPerformanceOut->write();
}


void IdentifyVoiceInAnalyser::setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface){
   _assessChildPerformanceYarpInterface=yarpInterface;
}

void GetEyeGazeInAnalyser::setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface){
   _assessChildPerformanceYarpInterface=yarpInterface;
}

void GetBodyPoseInAnalyser::setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface){
   _assessChildPerformanceYarpInterface=yarpInterface;
}

void IdentifyTrajectoryInAnalyser::setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface){
   _assessChildPerformanceYarpInterface=yarpInterface;
}

void GetInterventionStatusInAnalyser::setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface){
   _assessChildPerformanceYarpInterface=yarpInterface;
}

void InteractionEventInAnalyser::setAssessChildPerformance(AssessChildPerformanceYarpInterface* yarpInterface){
   _assessChildPerformanceYarpInterface=yarpInterface;
}



void IdentifyVoiceInAnalyser::onRead(VectorOf<int>& identifyVoiceInData){
   _assessChildPerformanceYarpInterface->getFromIdentifyVoice(identifyVoiceInData);
}

void GetEyeGazeInAnalyser::onRead(VectorOf<double>& getEyeGazeInData){
   _assessChildPerformanceYarpInterface->getFromGetEyeGaze(getEyeGazeInData);
}

void GetBodyPoseInAnalyser::onRead(VectorOf<double>& getBodyPoseInData){
   _assessChildPerformanceYarpInterface->getFromGetBodyPose(getBodyPoseInData);
}

void IdentifyTrajectoryInAnalyser::onRead(VectorOf<double>& identifyTrajectoryInData){
   _assessChildPerformanceYarpInterface->getFromIdentifyTrajectory(identifyTrajectoryInData);
}

void GetInterventionStatusInAnalyser::onRead(VectorOf<int>& getInterventionStatusInData){
   _assessChildPerformanceYarpInterface->getFromGetInterventionStatus(getInterventionStatusInData);
}

void InteractionEventInAnalyser::onRead(VectorOf<int>& interactionEventInData){
   _assessChildPerformanceYarpInterface->getFromInteractionEvent(interactionEventInData);
}



