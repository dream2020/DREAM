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
  * 22/06/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "naoInterface.h"
#include "naoInterfaceYarpInterface.h"

NaoInterfaceThread::NaoInterfaceThread(BufferedPort<VectorOf<int>>* disableRobotIn, BufferedPort<VectorOf<int>>* enableRobotIn, BufferedPort<VectorOf<int>>* gripIn, BufferedPort<VectorOf<double>>* moveHandIn, BufferedPort<VectorOf<double>>* moveTorsoIn, BufferedPort<VectorOf<int>>* releaseIn, BufferedPort<Bottle>* sayIn, BufferedPort<VectorOf<int>>* moveSequenceIn, BufferedPort<VectorOf<double>>* moveHeadIn, BufferedPort<VectorOf<float>>* pointAtIn, BufferedPort<Bottle>* sensorFeedbackOut, BufferedPort<Bottle>* robotMotorFeedbackOut){
	/* assign the input and output ports */
	_disableRobotIn = disableRobotIn;
	_enableRobotIn = enableRobotIn;
	_gripIn = gripIn;
	_moveHandIn = moveHandIn;
	_moveTorsoIn = moveTorsoIn;
	_releaseIn = releaseIn;
	_sayIn = sayIn;
	_moveSequenceIn = moveSequenceIn;
	_moveHeadIn = moveHeadIn;
	_pointAtIn = pointAtIn;
	_sensorFeedbackOut = sensorFeedbackOut;
	_robotMotorFeedbackOut = robotMotorFeedbackOut;
   
	_disableRobotInAnalyser = new DisableRobotInAnalyser();
	_enableRobotInAnalyser = new EnableRobotInAnalyser();
	_gripInAnalyser = new GripInAnalyser();
	_moveHandInAnalyser = new MoveHandInAnalyser();
	_moveTorsoInAnalyser = new MoveTorsoInAnalyser();
	_releaseInAnalyser = new ReleaseInAnalyser();
	_sayInAnalyser = new SayInAnalyser();
	_moveSequenceInAnalyser = new MoveSequenceInAnalyser();
	_moveHeadInAnalyser = new MoveHeadInAnalyser();
	_pointAtInAnalyser = new PointAtInAnalyser();
   
	disableRobotIn->useCallback(*_disableRobotInAnalyser);
	enableRobotIn->useCallback(*_enableRobotInAnalyser);
	gripIn->useCallback(*_gripInAnalyser);
	moveHandIn->useCallback(*_moveHandInAnalyser);
	moveTorsoIn->useCallback(*_moveTorsoInAnalyser);
	releaseIn->useCallback(*_releaseInAnalyser);
	sayIn->useCallback(*_sayInAnalyser);
	moveSequenceIn->useCallback(*_moveSequenceInAnalyser);
	moveHeadIn->useCallback(*_moveHeadInAnalyser);
	pointAtIn->useCallback(*_pointAtInAnalyser);
}

void NaoInterfaceThread::setYarpInterface(NaoInterfaceYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_disableRobotInAnalyser->setNaoInterface(_yarpInterface);
	_enableRobotInAnalyser->setNaoInterface(_yarpInterface);
	_gripInAnalyser->setNaoInterface(_yarpInterface);
	_moveHandInAnalyser->setNaoInterface(_yarpInterface);
	_moveTorsoInAnalyser->setNaoInterface(_yarpInterface);
	_releaseInAnalyser->setNaoInterface(_yarpInterface);
	_sayInAnalyser->setNaoInterface(_yarpInterface);
	_moveSequenceInAnalyser->setNaoInterface(_yarpInterface);
	_moveHeadInAnalyser->setNaoInterface(_yarpInterface);
	_pointAtInAnalyser->setNaoInterface(_yarpInterface);
}

bool NaoInterfaceThread::threadInit(){
	/* initilize the thread */
	return true;
}

void NaoInterfaceThread::run(){

}

void NaoInterfaceThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void NaoInterfaceThread::sendToSensorFeedback(Bottle data){
	Bottle& container = _sensorFeedbackOut->prepare();
	container = data;
	_sensorFeedbackOut->write();
}

void NaoInterfaceThread::sendToRobotMotorFeedback(Bottle data){
	Bottle& container = _robotMotorFeedbackOut->prepare();
	container = data;
	_robotMotorFeedbackOut->write();
}


void DisableRobotInAnalyser::setNaoInterface(NaoInterfaceYarpInterface* yarpInterface){
   _naoInterfaceYarpInterface=yarpInterface;
}

void EnableRobotInAnalyser::setNaoInterface(NaoInterfaceYarpInterface* yarpInterface){
   _naoInterfaceYarpInterface=yarpInterface;
}

void GripInAnalyser::setNaoInterface(NaoInterfaceYarpInterface* yarpInterface){
   _naoInterfaceYarpInterface=yarpInterface;
}

void MoveHandInAnalyser::setNaoInterface(NaoInterfaceYarpInterface* yarpInterface){
   _naoInterfaceYarpInterface=yarpInterface;
}

void MoveTorsoInAnalyser::setNaoInterface(NaoInterfaceYarpInterface* yarpInterface){
   _naoInterfaceYarpInterface=yarpInterface;
}

void ReleaseInAnalyser::setNaoInterface(NaoInterfaceYarpInterface* yarpInterface){
   _naoInterfaceYarpInterface=yarpInterface;
}

void SayInAnalyser::setNaoInterface(NaoInterfaceYarpInterface* yarpInterface){
   _naoInterfaceYarpInterface=yarpInterface;
}

void MoveSequenceInAnalyser::setNaoInterface(NaoInterfaceYarpInterface* yarpInterface){
   _naoInterfaceYarpInterface=yarpInterface;
}

void MoveHeadInAnalyser::setNaoInterface(NaoInterfaceYarpInterface* yarpInterface){
   _naoInterfaceYarpInterface=yarpInterface;
}

void PointAtInAnalyser::setNaoInterface(NaoInterfaceYarpInterface* yarpInterface){
   _naoInterfaceYarpInterface=yarpInterface;
}



void DisableRobotInAnalyser::onRead(VectorOf<int>& disableRobotInData){
   _naoInterfaceYarpInterface->getFromDisableRobot(disableRobotInData);
}

void EnableRobotInAnalyser::onRead(VectorOf<int>& enableRobotInData){
   _naoInterfaceYarpInterface->getFromEnableRobot(enableRobotInData);
}

void GripInAnalyser::onRead(VectorOf<int>& gripInData){
   _naoInterfaceYarpInterface->getFromGrip(gripInData);
}

void MoveHandInAnalyser::onRead(VectorOf<double>& moveHandInData){
   _naoInterfaceYarpInterface->getFromMoveHand(moveHandInData);
}

void MoveTorsoInAnalyser::onRead(VectorOf<double>& moveTorsoInData){
   _naoInterfaceYarpInterface->getFromMoveTorso(moveTorsoInData);
}

void ReleaseInAnalyser::onRead(VectorOf<int>& releaseInData){
   _naoInterfaceYarpInterface->getFromRelease(releaseInData);
}

void SayInAnalyser::onRead(Bottle& sayInData){
   _naoInterfaceYarpInterface->getFromSay(sayInData);
}

void MoveSequenceInAnalyser::onRead(VectorOf<int>& moveSequenceInData){
   _naoInterfaceYarpInterface->getFromMoveSequence(moveSequenceInData);
}

void MoveHeadInAnalyser::onRead(VectorOf<double>& moveHeadInData){
   _naoInterfaceYarpInterface->getFromMoveHead(moveHeadInData);
}

void PointAtInAnalyser::onRead(VectorOf<float>& pointAtInData){
   _naoInterfaceYarpInterface->getFromPointAt(pointAtInData);
}



