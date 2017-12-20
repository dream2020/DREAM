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

#include "naoInterfaceYarpInterface.h"
#include "naoInterface.h"
#include "naoInterfaceController.h"

#include "actionMessages.h"
#include "feedbackMessages.h"

using namespace std;

NaoInterfaceYarpInterface::NaoInterfaceYarpInterface(){

}

void NaoInterfaceYarpInterface::init(NaoInterfaceThread* yarpThread, NaoInterfaceController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void NaoInterfaceYarpInterface::getFromDisableRobot(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	int stepId = -1;
	if (data.size())
		stepId = data[0];
	if (data.size() == 2&&data[1]==1)
		_controller->stopAllBehaviours(stepId);
	else
		_controller->enableRobot(0, stepId);
	return;
}

void NaoInterfaceYarpInterface::getFromEnableRobot(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	int stepId = -1;

	if (data.size())
		stepId = data[0];
	
	_controller->enableRobot(1, stepId);
	return;
}

//close hand >< 0-> left, 1->right
void NaoInterfaceYarpInterface::getFromGrip(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	
	if (!data.size()) {
		sendToRobotMotorFeedback(-1,_ACTION_GRAB_OBJECT_, _FEEDBACK_INCORRECT_DATA_);
		return;
	}

	int stepId = -1;
	if (data.size()==2)
		stepId = data[1];
	
	_controller->closeHand(data[0],1, stepId);
	return;
}

//handDescriptor, x, y, z, roll
//Will not be implemented
void NaoInterfaceYarpInterface::getFromMoveHand(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	if (data.size() == 6)
		_controller->getFromMoveHand(data[0], data[1], data[2], data[3], data[4], data[5]);

	return;
}

//normal vector x,y,z
//Will not be implemented
void NaoInterfaceYarpInterface::getFromMoveTorso(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

//open hand
void NaoInterfaceYarpInterface::getFromRelease(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	if (!data.size()) {
		sendToRobotMotorFeedback(-1, _ACTION_DROP_OBJECT_, _FEEDBACK_INCORRECT_DATA_);
		return;
	}

	int stepId = -1;
	if (data.size()==2)
		stepId = data[1];
	
	_controller->closeHand(data[0],0, stepId);
	return;
}

//data[0] string to say, data[1] id
void NaoInterfaceYarpInterface::getFromSay(Bottle data){
	/* Do Stuff: call methods from controller */
	if (!data.get(0).isString()) {
		sendToRobotMotorFeedback(-1, _ACTION_SAY_, _FEEDBACK_INCORRECT_DATA_);
		return;
	}

	int stepId = -1;
	if (data.get(1).isInt())
		stepId = data.get(1).asInt();

	_controller->say(data.get(0).asString(), stepId);
	return;
}

//use int descriptor -> will need a header for links
void NaoInterfaceYarpInterface::getFromMoveSequence(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	if (!data.size()) {
		sendToRobotMotorFeedback(-1, _ACTION_MOVE_, _FEEDBACK_INCORRECT_DATA_);
		return;
	}
	int seqId;
	int stepId = -1;
	if (data.size() == 2) {
		stepId = data[0];
		seqId = data[1];
		_controller->getFromMoveSequence(stepId,seqId);
	}
	if (data.size() == 5){
		stepId = data[0];
		seqId = data[1];
		int arg1 = data[2];
		int arg2 = data[3];
		int arg3 = data[4];
		_controller->getFromMoveSequence(stepId, seqId, arg1, arg2, arg3);

	}
	return;
}

//normal vector x,y,z,speed -> lookAt
void NaoInterfaceYarpInterface::getFromMoveHead(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	if (data.size()<3) {
		sendToRobotMotorFeedback(-1, _ACTION_MOVE_HEAD_, _FEEDBACK_INCORRECT_DATA_);
		return;
	}

	float speed = 0.5;
	int stepId = -1;
	if (data.size() == 5){
		speed = data[3];
		stepId = data[4];
	}
	if (speed <= 0)
		speed = 0.5;
	_controller->moveHead(data[0],data[1], data[2], speed, stepId);
	return;
}

// <x,y,z,t> in meters in world reference - bool rightArm
void NaoInterfaceYarpInterface::getFromPointAt(VectorOf<float> data){
	unsigned nbrActions = (int)(data.size() / 4.f);
	if (data.size() % 4 > 2||!nbrActions) {
		sendToRobotMotorFeedback(-1, _ACTION_POINT_AT_, _FEEDBACK_INCORRECT_DATA_);
		return;
	}
	vector<vector<float>> vec;
	for (int i = 0; i<nbrActions; i++) {
		vector<float> temp;
		for (unsigned j = 0; j<4; j++)
			temp.push_back(data[4 * i + j]);
		vec.push_back(temp);
	}

	bool rightArm = 1;
	int stepId = -1;
	if (data.size() % 4 > 0)
		rightArm = data[4 * nbrActions];
	if (data.size() % 4 > 1) 
		stepId = data[4 * nbrActions + 1];

	_controller->pointAt(vec, rightArm, stepId);
	return;
}

void NaoInterfaceYarpInterface::sendToSensorFeedback(string name, bool value){
	Bottle data;
	data.addString(name);
	data.addInt(value); 
	_yarpThread->sendToSensorFeedback(data);
	return;
}

void NaoInterfaceYarpInterface::sendToRobotMotorFeedback(int step, int action, int message, int parameter) {
	Bottle data;
	data.addInt(step);
	data.addInt(action);
	data.addInt(message);
	data.addInt(parameter);
	_yarpThread->sendToRobotMotorFeedback(data);
	return;
}


