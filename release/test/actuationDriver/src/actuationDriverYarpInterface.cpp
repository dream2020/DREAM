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

#include "actuationDriverYarpInterface.h"
#include "actuationDriver.h"
#include "actuationDriverController.h"
#include "actionMessages.h"

using namespace std;

ActuationDriverYarpInterface::ActuationDriverYarpInterface(){

}

void ActuationDriverYarpInterface::init(ActuationDriverThread* yarpThread, ActuationDriverController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void ActuationDriverYarpInterface::getFromDisableRobot(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(50);
	cout << "-actuationDriver- Received disable robot with step: " << data[0] << endl;
	_controller->simulateExecution(data[0],_ACTION_DISABLE_);
	return;
}

void ActuationDriverYarpInterface::getFromEnableRobot(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(50);
	cout << "-actuationDriver- Received enable robot with step: " << data[0] << endl;
	_controller->simulateExecution(data[0], _ACTION_ENABLE_);
	return;
}

void ActuationDriverYarpInterface::getFromGrip(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(50);
	cout << "-actuationDriver- Received grip robot with arguments: " << data[0] << " step " << data[1] << endl;
	_controller->simulateExecution(data[1], _ACTION_CLOSE_HAND_);
	return;
}

void ActuationDriverYarpInterface::getFromMoveHand(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(50);
	cout << "-actuationDriver- Received moveHand robot with parameters: " << data[0] <<" "<< data[1] << " " << data[2] << " " << data[3] << " " << data[4] << " " << data[5]<< endl;
	_controller->simulateExecution(data[0], _ACTION_MOVE_HAND_);
	return;
}

void ActuationDriverYarpInterface::getFromMoveHead(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(50);
	cout << "-actuationDriver- Received moveHead with parameters: " << data[0] << " " << data[1] << " " << data[2] << " " << data[3] << " " << data[4] << endl;
	_controller->simulateExecution(data[4], _ACTION_MOVE_HEAD_);
	return;
}

void ActuationDriverYarpInterface::getFromMoveSequence(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(50);
	if (data.size() == 2) 
		cout << "-actuationDriver- Received moveSequence with arguments: step " << data[0] << " action " << data[1] << endl;
	else if(data.size() == 5) 
		cout << "-actuationDriver- Received moveSequence with arguments: step " << data[0] << " action " << data[1] << endl;
	
	_controller->simulateExecution(data[0], _ACTION_MOVE_);

	return;
}

void ActuationDriverYarpInterface::getFromMoveTorso(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void ActuationDriverYarpInterface::getFromPointAt(VectorOf<float> data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(50);
	cout << "-actuationDriver- Received pointAt with "<< data.size()<<" parameters"<< endl;
	//The stepId is in after the n points of 4 floats and after the arm identifier, i.e. in 4n+1 position
	_controller->simulateExecution(data[4*(int)(data.size() / 4.f) + 1], _ACTION_POINT_AT_);
	return;
}

void ActuationDriverYarpInterface::getFromRelease(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(50);
	cout << "-actuationDriver- Received release with arguments: " << data[0] << " step " << data[1] << endl;
	_controller->simulateExecution(data[1], _ACTION_OPEN_HAND_);
	return;
}

void ActuationDriverYarpInterface::getFromSay(Bottle data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(50);
	cout << "-actuationDriver- Received say with parameters: " << data.get(0).asString()<<" step "<<data.get(1).asInt()<< endl;
	_controller->simulateExecution(data.get(1).asInt(), _ACTION_SAY_);
	return;
}

void ActuationDriverYarpInterface::getFromSandtrayCommand(Bottle data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(50);
	cout << "-actuationDriver- Received on sandtrayCommand: " << data.get(0).asString() << endl;
	_controller->getFromSandtrayCommand(data.get(0).asString());
	return;
}

void ActuationDriverYarpInterface::getFromActionFeedback(Bottle data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(100);
	cout << "-actuationDriver- got from action feedback: step " << data.get(0).asInt() << " Action " << data.get(1).asInt() << " Result " << data.get(2).asInt() << " Subaction " << data.get(3).asInt()<<endl;
	if(!data.get(3).asInt())
		_controller->goNextStep();
	else if (_controller->_touchExpectedGrab) {
		_controller->waitms(1000);
		cout << "-actuationDriver- a touch is expected, simulating touch" << endl;
		_controller->waitms(1000);
		sendToSensorFeedback("MiddleTactil", 1);	
		_controller->_touchExpectedGrab = 0;
	}
	else if (_controller->_touchExpectedDrop && data.get(1).asInt() == 40) {
		_controller->waitms(1000);
		cout << "-actuationDriver- a touch is expected, simulating touch" << endl;
		_controller->waitms(1000);
		sendToSensorFeedback("MiddleTactil", 1);
		_controller->_touchExpectedDrop = 0;
	}
	return;
}

void ActuationDriverYarpInterface::getFromRobotSensors(Bottle data){
	/* Do Stuff: call methods from controller */
	_controller->waitms(50);
	cout << "-actuationDriver- got from robot sensors: " << data.get(0).asString() << "  " << data.get(1).asInt() << endl;
	return;
}


//Probably need to change type to allow to be called by non yarp class
void ActuationDriverYarpInterface::sendToEyeBlinking(string repetition, string type, string duration){ 
	Bottle data;
	data.addString(repetition);
	data.addString(type);
	data.addString(duration);
	_yarpThread->sendToEyeBlinking(data);
	return;
}

void ActuationDriverYarpInterface::sendToFallingReaction(bool falling){
	VectorOf<int> data;
	data.push_back(falling+1);
	_yarpThread->sendToFallingReaction(data);
	return;
}

void ActuationDriverYarpInterface::sendToFallingReactionSpeech(bool falling) {
	VectorOf<int> data; 
	data.push_back(falling+1);
	_yarpThread->sendToFallingReactionSpeech(data);
	return;
}

void ActuationDriverYarpInterface::sendToSocialFacialExpression(int socialFacialReaction){
	VectorOf<int> data;
	data.push_back(socialFacialReaction);
	_yarpThread->sendToSocialFacialExpression(data);
	return;
}

void ActuationDriverYarpInterface::sendToSocialReaction(int socialReaction){
	VectorOf<int> data;
	data.push_back(socialReaction);
	_yarpThread->sendToSocialReaction(data);
	return;
}

void ActuationDriverYarpInterface::sendToSocialReactionSpeech(int socialReactionSpeech){
	VectorOf<int> data;
	data.push_back(socialReactionSpeech);
	_yarpThread->sendToSocialReactionSpeech(data);
	return;
}

void ActuationDriverYarpInterface::sendToRobotMotorFeedback(int step, int action){
	Bottle data;
	data.addInt(step);
	data.addInt(action);
	data.addInt(1);
	_yarpThread->sendToRobotMotorFeedback(data);
	return;
}

void ActuationDriverYarpInterface::sendToSelectedAction(int stepId, int actionId, std::string argument) {
	cout << "-actuationDriver- send to selected action: step " << stepId << " Selected action " << actionId << " argument " << argument << endl;
	
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	data.addString(argument);
	_yarpThread->sendToSelectedAction(data);
	return;
}

void ActuationDriverYarpInterface::sendToSelectedAction(int stepId, int actionId, int argument) {
	cout << "-actuationDriver- send to selected action: step " << stepId << " Selected action " << actionId << " argument " << argument << endl;
	
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	data.addInt(argument);
	_yarpThread->sendToSelectedAction(data);
	return;
}

void ActuationDriverYarpInterface::sendToSelectedAction(int stepId, int actionId) {
	cout << "-actuationDriver- send to selected action: step " << stepId << " Selected action " << actionId << endl;
	
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	_yarpThread->sendToSelectedAction(data);
	return;
}

void ActuationDriverYarpInterface::sendToSandtrayReturn(string message){
	Bottle data;
	data.addString(message);
	_yarpThread->sendToSandtrayReturn(data);
	return;
}

void ActuationDriverYarpInterface::sendToSensorFeedback(string name, bool value){
	cout <<"-actuationDriver- Sending to sensor feedback "<< name << " " << value << endl;
	Bottle data;
	data.addString(name);
	data.addInt(value);
	_yarpThread->sendToSensorFeedback(data);
	return;
}

void ActuationDriverYarpInterface::sendToMoveHead(VectorOf<double> data){ 
	_yarpThread->sendToMoveHead(data);
	return;
}

void ActuationDriverYarpInterface::sendToFallingInterruption(VectorOf<int> data){ 
	_yarpThread->sendToFallingInterruption(data);
	return;
}

void ActuationDriverYarpInterface::sendToElicitedAttention(double x, double y, double z){
	VectorOf<double> data;
	data.push_back(x);
	data.push_back(y);
	data.push_back(z);
	_yarpThread->sendToElicitedAttention(data);
	return;
}


