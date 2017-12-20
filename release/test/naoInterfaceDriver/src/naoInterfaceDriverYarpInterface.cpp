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

#include "naoInterfaceDriverYarpInterface.h"
#include "naoInterfaceDriver.h"
#include "naoInterfaceDriverController.h"

using namespace std;

NaoInterfaceDriverYarpInterface::NaoInterfaceDriverYarpInterface(){

}

void NaoInterfaceDriverYarpInterface::init(NaoInterfaceDriverThread* yarpThread, NaoInterfaceDriverController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void NaoInterfaceDriverYarpInterface::getFromSensorFeedback(Bottle data){
	/* Do Stuff: call methods from controller */
	_controller->getFromSensorsFeedbacks(data.get(0).asString(), data.get(1).asInt());
	return;
}

void NaoInterfaceDriverYarpInterface::getFromRobotMotorFeedback(Bottle data){
	/* Do Stuff: call methods from controller */
	_controller->getFromMotorFeedbacks(data.get(0).asInt(), data.get(1).asInt(), data.get(2).asInt());
	return;
}


//Probably need to change type to allow to be called by non yarp class
void NaoInterfaceDriverYarpInterface::sendToDisableRobot(int stepId){
	VectorOf<int> data;
	data.push_back(stepId);
	_yarpThread->sendToDisableRobot(data);
	return;
}

void NaoInterfaceDriverYarpInterface::sendToEnableRobot(int stepId){
	VectorOf<int> data;
	data.push_back(stepId);
	_yarpThread->sendToEnableRobot(data);
	return;
}

void NaoInterfaceDriverYarpInterface::sendToGrip(bool rightHand, int stepId){
	VectorOf<int> data;
	data.push_back(rightHand);
	data.push_back(stepId);
	_yarpThread->sendToGrip(data);
	return;
}

void NaoInterfaceDriverYarpInterface::sendToMoveHand(vector<double> values, int stepId){
	VectorOf<double> data;
	for(unsigned i=0;i<values.size();i++)
		data.push_back(values[i]);

	data.push_back(stepId);
	_yarpThread->sendToMoveHand(data);
	return;
}

void NaoInterfaceDriverYarpInterface::sendToMoveTorso(vector<double> values, int stepId){
	VectorOf<double> data; 
	for(unsigned i=0;i<values.size();i++)
		data.push_back(values[i]);
	data.push_back(stepId);
	_yarpThread->sendToMoveTorso(data);
	return;
}

void NaoInterfaceDriverYarpInterface::sendToRelease(bool rightHand, int stepId){
	VectorOf<int> data;
	data.push_back(rightHand);
	data.push_back(stepId);
	_yarpThread->sendToRelease(data);
	return;
}

void NaoInterfaceDriverYarpInterface::sendToSay(string sentence, int stepId){
	Bottle data;
	data.addString(sentence);
	data.addInt(stepId);
	_yarpThread->sendToSay(data);
	return;
}

void NaoInterfaceDriverYarpInterface::sendToMoveSequence(int stepId, int actionId) {
	VectorOf<int> data;
	data.push_back(stepId);
	data.push_back(actionId);
	_yarpThread->sendToMoveSequence(data);
	return;
}

void NaoInterfaceDriverYarpInterface::sendToMoveSequence(int stepId, int actionId, int repetitions, int fullLed, int duration) {
	VectorOf<int> data;
	data.push_back(stepId);
	data.push_back(actionId);
	data.push_back(repetitions);
	data.push_back(fullLed);
	data.push_back(duration);
	_yarpThread->sendToMoveSequence(data);
	return;
}

void NaoInterfaceDriverYarpInterface::sendToMoveHead(vector<double> values, int stepId){
	VectorOf<double> data;
	for(unsigned i=0;i<values.size();i++)
		data.push_back(values[i]);
	data.push_back(stepId);
	_yarpThread->sendToMoveHead(data);
	return;
}

void NaoInterfaceDriverYarpInterface::sendToPointAt(vector<float> values, int stepId){
	VectorOf<float> data;
	for(unsigned i=0;i<values.size();i++)
		data.push_back(values[i]);
	data.push_back(stepId);
	_yarpThread->sendToPointAt(data);
	return;
}


