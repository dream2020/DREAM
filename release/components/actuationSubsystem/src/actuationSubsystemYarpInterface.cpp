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
  * 20/09/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "actuationSubsystemYarpInterface.h"
#include "actuationSubsystem.h"
#include "actuationSubsystemController.h"
#include "actionMessages.h"
#include <sstream>
#include <althread/almutex.h>

const int DEBUG = 1;

using namespace std;

ActuationSubsystemYarpInterface::ActuationSubsystemYarpInterface(){

}

void ActuationSubsystemYarpInterface::init(ActuationSubsystemThread* yarpThread, ActuationSubsystemController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void ActuationSubsystemYarpInterface::getFromEyeBlinking(Bottle data){
	/* Do Stuff: call methods from controller */
	string repetitions = data.get(0).toString();
	string type = data.get(1).toString();
	string duration = data.get(2).toString();
	int repetitionNumber=0;

	if (repetitions == "Single") 
		repetitionNumber = 1;
	else if (repetitions == "Double") 
		repetitionNumber = 2;
	else if (repetitions == "Triple") 
		repetitionNumber = 3;

	bool fullLeds = type == "Full";

	int durationMilli = 1000 * std::atof(duration.c_str());

	_controller->getFromEyeBlinking(repetitionNumber, fullLeds, durationMilli);
    
	return;
}

void ActuationSubsystemYarpInterface::getFromFallingReaction(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	if (data.size())
		_controller->getFromFallingReaction(data[0]);

	return;
}

void ActuationSubsystemYarpInterface::getFromFallingReactionSpeech(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	if (data.size())
		_controller->getFromFallingReactionSpeech(data[0]);
	return;
}

void ActuationSubsystemYarpInterface::getFromSocialFacialExpression(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	cout << "received from social facial reaction " << data[0] << endl;
	if (data.size())
		_controller->getFromSocialFacialExpression(data[0]);

	return;
}

void ActuationSubsystemYarpInterface::getFromSocialReaction(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	if (data.size())
		_controller->getFromSocialReaction(data[0]);
	return;
}

void ActuationSubsystemYarpInterface::getFromSocialReactionSpeech(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	if (data.size())
		_controller->getFromSocialReactionSpeech(data[0]);
	return;
}

void ActuationSubsystemYarpInterface::getFromFallingInterruption(VectorOf<int> data) {
	/* Do Stuff: call methods from controller */
	if (data.size())
		_controller->getFromInterruption(data[0]);
	return;
}

void ActuationSubsystemYarpInterface::getFromElicitedAttention(VectorOf<double> data) {
	/* Do Stuff: call methods from controller */
	if (data.size())
		_controller->getFromElicitedAttention(data[0], data[1], data[2]);
	return;
}

void ActuationSubsystemYarpInterface::getFromRobotMotorFeedback(Bottle data){
	/* Do Stuff: call methods from controller */
	_controller->getFromMotorFeedback(data.get(0).asInt(), data.get(1).asInt(), data.get(2).asInt(), data.get(3).asInt());
	return;
}

void ActuationSubsystemYarpInterface::getFromSelectedAction(Bottle data){
	/* Do Stuff: call methods from controller */
	string str="";
	int stepId = data.get(0).asInt();
	int actionId = data.get(1).asInt();
	if(actionId==_ACTION_MOVE_HEAD_) {
		if (data.size() == 6)
			_controller->moveHead(data.get(2).asDouble(),data.get(3).asDouble(),data.get(4).asDouble(),data.get(5).asDouble(), stepId);
	}
	else if(actionId==_ACTION_MOVE_HAND_) {
		if (data.size()==7)
			_controller->moveHand(data.get(2).asInt(), data.get(3).asDouble(), data.get(4).asDouble(), data.get(5).asDouble(), data.get(6).asDouble(), stepId);
	}
	else {
		for (unsigned i = 2; i<data.size(); i++) {
			if (i != 2)
				str += ",";
			str += data.get(i).toString();
		}
		_controller->getFromSelectedAction(stepId, actionId, str);
	}
	return;
}

void ActuationSubsystemYarpInterface::getFromSandtrayReturn(Bottle data){
	/* Do Stuff: call methods from controller */
	_controller->getFromSandtrayReturn(data.get(0).toString());
	return;
}

void ActuationSubsystemYarpInterface::getFromSensorFeedback(Bottle data){
	/* Do Stuff: call methods from controller */
	_controller->getFromSensorFeedback(data.get(0).toString(), data.get(1).asInt());
	return;
}

void ActuationSubsystemYarpInterface::getFromEmergency(Bottle data){
	/* Do Stuff: call methods from controller */
	if (data.size() == 2 && data.get(1).asInt() == _ACTION_EMERGENCY_EXIT_) {
		cout << "Actuation - Emergency exit, stop all the waiters" << endl;
		_controller->resetWaitingStates();
		sendToDisableRobot(data.get(0).asInt(), 1);
	}
	return;
}

void ActuationSubsystemYarpInterface::sendToDisableRobot(int stepId) {
	VectorOf<int> data;
	data.push_back(stepId);
	_yarpThread->sendToDisableRobot(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToDisableRobot(int stepId, int param) {
	VectorOf<int> data;
	data.push_back(stepId);
	data.push_back(param);
	_yarpThread->sendToDisableRobot(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToEnableRobot(int stepId) {
	VectorOf<int> data;
	data.push_back(stepId);
	_yarpThread->sendToEnableRobot(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToGrip(bool rightArm, int stepId){
	VectorOf<int> data;
	data.push_back(rightArm);
	data.push_back(stepId);
	_yarpThread->sendToGrip(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToMoveHand(bool rightHand, float x, float y, float z, float roll, int stepId){
	VectorOf<double> data;
	data.push_back(rightHand);
	data.push_back(x);
	data.push_back(y);
	data.push_back(z);
	data.push_back(roll);
	data.push_back(stepId);

	_yarpThread->sendToMoveHand(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToMoveHead(float x, float y, float z, float speed, float stepId){ 
	VectorOf<double> data;
	data.push_back(x);
	data.push_back(y);
	data.push_back(z);
	data.push_back(speed);
	data.push_back(stepId);
	_yarpThread->sendToMoveHead(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToMoveSequence(int stepId, int seqId) {
	VectorOf<int> data;
	data.push_back(stepId);
	data.push_back(seqId);
	_yarpThread->sendToMoveSequence(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToMoveSequence(int stepId, int seqId, int arg1, int arg2, int arg3) {
	VectorOf<int> data;
	data.push_back(stepId);
	data.push_back(seqId);
	data.push_back(arg1);
	data.push_back(arg2);
	data.push_back(arg3);
	_yarpThread->sendToMoveSequence(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToMoveTorso(VectorOf<double> data){ 
	_yarpThread->sendToMoveTorso(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToPointAt(vector<vector<float>> points, bool rightArm, int stepId) {
	VectorOf<float> data;
	for (int i = 0; i < points.size(); i++)
		for (int j = 0; j < points[i].size(); j++)
			data.push_back(points[i][j]);
	data.push_back(rightArm);
	data.push_back(stepId);
	_yarpThread->sendToPointAt(data);
	return;
}


void ActuationSubsystemYarpInterface::sendToPointAt(vector<float> points, bool rightArm, int stepId) {
	VectorOf<float> data;
	for (int i = 0; i < points.size(); i++)
		data.push_back(points[i]);

	data.push_back(rightArm);
	data.push_back(stepId);
	_yarpThread->sendToPointAt(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToRelease(bool rightArm, int stepId){
	VectorOf<int> data;
	data.push_back(rightArm);
	data.push_back(stepId);
	_yarpThread->sendToRelease(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToSay(int stepId, string sentence){
	Bottle data;
	data.addString(sentence);
	data.addInt(stepId);
	_yarpThread->sendToSay(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToSandtrayCommand(string command, string parameter) {
	Bottle data;
	data.addString(command + "," + parameter);
	_yarpThread->sendToSandtrayCommand(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToActionFeedback(int stepId, int actionId, int fbValue, bool subAction, int parameter){ 
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	data.addInt(fbValue);
	data.addInt(subAction);
	data.addInt(parameter);
	_yarpThread->sendToActionFeedback(data);
	return;
}

void ActuationSubsystemYarpInterface::sendToRobotSensors(std::string name, bool value){
	
    Bottle data;

	cout << "Sending back to robotSensors " << name << " " << value << endl;
	if (DEBUG)
		cout << "-actuationSubsystem- Sending back to robotSensors " << name << " " << value << endl;


	data.addString(name);
	data.addInt(value);
    
	_yarpThread->sendToRobotSensors(data);
	return;
}
