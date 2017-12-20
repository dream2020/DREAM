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
  * 16/08/2016 Version 1.0: ... (Emmanuel Senft)
  */ 

#include "naoInterfaceDriver.h"
#include "naoInterfaceDriverYarpInterface.h"

NaoInterfaceDriverThread::NaoInterfaceDriverThread(BufferedPort<Bottle>* sensorFeedbackIn, BufferedPort<Bottle>* robotMotorFeedbackIn, BufferedPort<VectorOf<int>>* disableRobotOut, BufferedPort<VectorOf<int>>* enableRobotOut, BufferedPort<VectorOf<int>>* gripOut, BufferedPort<VectorOf<double>>* moveHandOut, BufferedPort<VectorOf<double>>* moveTorsoOut, BufferedPort<VectorOf<int>>* releaseOut, BufferedPort<Bottle>* sayOut, BufferedPort<VectorOf<int>>* moveSequenceOut, BufferedPort<VectorOf<double>>* moveHeadOut, BufferedPort<VectorOf<float>>* pointAtOut){
	/* assign the input and output ports */
	_sensorFeedbackIn = sensorFeedbackIn;
	_robotMotorFeedbackIn = robotMotorFeedbackIn;
	_disableRobotOut = disableRobotOut;
	_enableRobotOut = enableRobotOut;
	_gripOut = gripOut;
	_moveHandOut = moveHandOut;
	_moveTorsoOut = moveTorsoOut;
	_releaseOut = releaseOut;
	_sayOut = sayOut;
	_moveSequenceOut = moveSequenceOut;
	_moveHeadOut = moveHeadOut;
	_pointAtOut = pointAtOut;
   
	_sensorFeedbackInAnalyser = new SensorFeedbackInAnalyser();
	_robotMotorFeedbackInAnalyser = new RobotMotorFeedbackInAnalyser();
   
	sensorFeedbackIn->useCallback(*_sensorFeedbackInAnalyser);
	robotMotorFeedbackIn->useCallback(*_robotMotorFeedbackInAnalyser);
}

void NaoInterfaceDriverThread::setYarpInterface(NaoInterfaceDriverYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_sensorFeedbackInAnalyser->setNaoInterfaceDriver(_yarpInterface);
	_robotMotorFeedbackInAnalyser->setNaoInterfaceDriver(_yarpInterface);
}

bool NaoInterfaceDriverThread::threadInit(){
	/* initilize the thread */
	return true;
}

void NaoInterfaceDriverThread::run(){

}

void NaoInterfaceDriverThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void NaoInterfaceDriverThread::sendToDisableRobot(VectorOf<int> data){
	VectorOf<int>& container = _disableRobotOut->prepare();
	container = data;
	_disableRobotOut->write();
}

void NaoInterfaceDriverThread::sendToEnableRobot(VectorOf<int> data){
	VectorOf<int>& container = _enableRobotOut->prepare();
	container = data;
	_enableRobotOut->write();
}

void NaoInterfaceDriverThread::sendToGrip(VectorOf<int> data){
	VectorOf<int>& container = _gripOut->prepare();
	container = data;
	_gripOut->write();
}

void NaoInterfaceDriverThread::sendToMoveHand(VectorOf<double> data){
	VectorOf<double>& container = _moveHandOut->prepare();
	container = data;
	_moveHandOut->write();
}

void NaoInterfaceDriverThread::sendToMoveTorso(VectorOf<double> data){
	VectorOf<double>& container = _moveTorsoOut->prepare();
	container = data;
	_moveTorsoOut->write();
}

void NaoInterfaceDriverThread::sendToRelease(VectorOf<int> data){
	VectorOf<int>& container = _releaseOut->prepare();
	container = data;
	_releaseOut->write();
}

void NaoInterfaceDriverThread::sendToSay(Bottle data){
	Bottle& container = _sayOut->prepare();
	container = data;
	_sayOut->write();
}

void NaoInterfaceDriverThread::sendToMoveSequence(VectorOf<int> data){
	VectorOf<int>& container = _moveSequenceOut->prepare();
	container = data;
	_moveSequenceOut->write();
}

void NaoInterfaceDriverThread::sendToMoveHead(VectorOf<double> data){
	VectorOf<double>& container = _moveHeadOut->prepare();
	container = data;
	_moveHeadOut->write();
}

void NaoInterfaceDriverThread::sendToPointAt(VectorOf<float> data){
	VectorOf<float>& container = _pointAtOut->prepare();
	container = data;
	_pointAtOut->write();
}


void SensorFeedbackInAnalyser::setNaoInterfaceDriver(NaoInterfaceDriverYarpInterface* yarpInterface){
   _naoInterfaceDriverYarpInterface=yarpInterface;
}

void RobotMotorFeedbackInAnalyser::setNaoInterfaceDriver(NaoInterfaceDriverYarpInterface* yarpInterface){
   _naoInterfaceDriverYarpInterface=yarpInterface;
}



void SensorFeedbackInAnalyser::onRead(Bottle& sensorFeedbackInData){
   _naoInterfaceDriverYarpInterface->getFromSensorFeedback(sensorFeedbackInData);
}

void RobotMotorFeedbackInAnalyser::onRead(Bottle& robotMotorFeedbackInData){
   _naoInterfaceDriverYarpInterface->getFromRobotMotorFeedback(robotMotorFeedbackInData);
}



