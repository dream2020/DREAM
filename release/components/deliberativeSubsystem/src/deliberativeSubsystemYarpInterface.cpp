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
  * 10/08/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "deliberativeSubsystemYarpInterface.h"
#include "deliberativeSubsystem.h"
#include "deliberativeSubsystemController.h"
#include "scriptMessages.h"
#include "actionMessages.h"


using namespace std;

DeliberativeSubsystemYarpInterface::DeliberativeSubsystemYarpInterface(){

}

void DeliberativeSubsystemYarpInterface::init(DeliberativeSubsystemThread* yarpThread, DeliberativeSubsystemController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void DeliberativeSubsystemYarpInterface::getFromGetChildBehaviour(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetChildPerformance(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	_controller->getFromChildPerformance(data[0],1);
	return;
}

void DeliberativeSubsystemYarpInterface::getFromCheckMutualGaze(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	_controller->getFromCheckMutualGaze(data[0]);
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetArmAngle(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetBody(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetBodyPose(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetEyeGaze(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetEyes(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetFaces(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetGripLocation(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetHands(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetHead(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetHeadGaze(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetObjects(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetObjectTableDistance(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromGetSoundDirection(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromIdentifyFace(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromIdentifyFaceExpression(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromIdentifyObject(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromIdentifyTrajectory(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromIdentifyVoice(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromRecognizeSpeech(Bottle data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromTrackFace(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromTrackObject(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromActionFeedback(Bottle data){
	/* Do Stuff: call methods from controller */
	int step = data.get(0).asInt();
	int actionId = data.get(1).asInt();
	int feedback = data.get(2).asInt();
	bool subAction = data.get(3).asInt();
	_controller->getFromActionFeedback(step, actionId, feedback, subAction);
	return;
}

void DeliberativeSubsystemYarpInterface::getFromInterventionCommand(Bottle data){
	/* Do Stuff: call methods from controller */
	cout << " -deliberativeSubsystem- in yarpInterface - get from intervention command " << data.toString()<<endl;
	int stepId = data.get(0).asInt();
	int commandId = data.get(1).asInt();

	string str;
	for (unsigned i = 2; i<data.size(); i++) {
		if (i != 2)
			str += ",";
		str += data.get(i).toString();
	}
	/*
	if (commandId == _SCRIPT_SAY_ || commandId == _SCRIPT_MOVE_)
		ss << data.get(2).asString();

	if (commandId == _SCRIPT_TYPE_ || commandId == _SCRIPT_ID_ || commandId == _SCRIPT_WAIT_ || commandId == _SCRIPT_SANDTRAY_LOAD_LIBRARY_ || commandId == _SCRIPT_POINT_IMAGE_ || commandId == _SCRIPT_LOOK_POINT_SAY_ || commandId == _SCRIPT_LOOK_POINT_ || commandId == _SCRIPT_LOOK_) {
		ss << data.get(2).asInt();
	}
	if (commandId == _SCRIPT_WAIT_FOR_PERF_) {
		ss << data.get(2).asInt()<<","<< data.get(3).asInt() << "," << data.get(4).asInt() << "," << data.get(5).asInt();
	}
*/
	_controller->getFromInterventionCommand(stepId, commandId, str);
	return;
}

void DeliberativeSubsystemYarpInterface::getFromSelectedAction(Bottle data){
	/* Do Stuff: call methods from controller */
	stringstream ss;
	int stepId = data.get(0).asInt();
	int actionId = data.get(1).asInt();
	//cout << "Bottle contains " << data.size() << endl;
	
	string str;
	for (unsigned i = 2; i<data.size(); i++) {
		if (i != 2)
			str += ",";
		str += data.get(i).toString();
	}

	_controller->getFromSelectedAction(stepId, actionId, str);
	return;
}

void DeliberativeSubsystemYarpInterface::getFromUserDelib(Bottle data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeSubsystemYarpInterface::getFromSandtrayEvent(Bottle data){
	if (!data.get(0).isString()) return;				//check that bottle contents is string
	if (data.get(0).asString().length() < 1) return;	//if empty string then don't unpack bottle
	_controller->processEvents(data.get(0).asString());
	return;
}

void DeliberativeSubsystemYarpInterface::getFromSandtrayReturn(Bottle data){
	if (!data.get(0).isString()) return;				//check that bottle contents is string
	if (data.get(0).asString().length() < 1) return;	//if empty string then don't unpack bottle
	_controller->processReturns(data.get(0).asString());
	return;
}

void DeliberativeSubsystemYarpInterface::getFromRobotSensors(Bottle data) {
	/* Do Stuff: call methods from controller */
	//DO Stuff
	return;
}

void DeliberativeSubsystemYarpInterface::getFromEmergency(Bottle data) {
	/* Do Stuff: call methods from controller */

	if (data.size() == 2 && data.get(1).asInt() == _ACTION_EMERGENCY_EXIT_) {
		cout << "Delib - Emergency exit, stop all the waiters" << endl;
		_controller->resetWaitingStates();
	}
	return;
}

//Probably need to change type to allow to be called by non yarp class
void DeliberativeSubsystemYarpInterface::sendToGetEyeGaze(VectorOf<double> data){ 
	_yarpThread->sendToGetEyeGaze(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToGetGripLocation(VectorOf<double> data){ 
	_yarpThread->sendToGetGripLocation(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToGetHeadGaze(VectorOf<double> data){ 
	_yarpThread->sendToGetHeadGaze(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToGetObjects(VectorOf<double> data){ 
	_yarpThread->sendToGetObjects(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToGetObjectTableDistance(VectorOf<double> data){ 
	_yarpThread->sendToGetObjectTableDistance(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToGetSoundDirection(VectorOf<double> data){ 
	_yarpThread->sendToGetSoundDirection(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToIdentifyFace(VectorOf<double> data){ 
	_yarpThread->sendToIdentifyFace(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToIdentifyFaceExpression(VectorOf<double> data){ 
	_yarpThread->sendToIdentifyFaceExpression(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToIdentifyObject(VectorOf<double> data){ 
	_yarpThread->sendToIdentifyObject(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToIdentifyTrajectory(VectorOf<int> data){ 
	_yarpThread->sendToIdentifyTrajectory(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToTrackFace(VectorOf<double> data){ 
	_yarpThread->sendToTrackFace(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToTrackHand(VectorOf<double> data){ 
	_yarpThread->sendToTrackHand(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToTrackObject(VectorOf<double> data){ 
	_yarpThread->sendToTrackObject(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToInteractionEvent(int id, int arg){ 
	VectorOf<int> data;
	data.push_back(id);
	data.push_back(arg);
	_yarpThread->sendToInteractionEvent(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToGetInterventionStatus(int scriptId, int scriptType, int stepId, int expBehav, int param, int time, bool onScript){
	VectorOf<int> data;
	data.push_back(scriptId);
	data.push_back(stepId);
	data.push_back(expBehav);
	data.push_back(param);
	data.push_back(time);
	data.push_back(onScript);
	data.push_back(scriptType);
	_yarpThread->sendToGetInterventionStatus(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToAttentionBias(Bottle data){ 
	_yarpThread->sendToAttentionBias(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToCommandSuccess(bool success){ 
	VectorOf<int> data;
	data.push_back(success);
	_yarpThread->sendToCommandSuccess(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToStartStop(VectorOf<int> data){ 
	_yarpThread->sendToStartStop(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToDeliberativeFeedback(string type, int value){ 
	Bottle data;
	data.addString(type);
	data.addInt(value);
	_yarpThread->sendToDeliberativeFeedback(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToSensorySummary(Bottle data){ 
	_yarpThread->sendToSensorySummary(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToSandtrayCommand(string command, string parameter) {
	cout << "-deliberativeSubsystem- sending to Sandtray " << command << "," << parameter << endl;
	_controller->waitms(50);
	Bottle data;
	data.addString(command + "," + parameter);
	_yarpThread->sendToSandtrayCommand(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToSuggestedAction(int stepId, int actionId, std::string arg1) {
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	if (actionId == _ACTION_RECEIVED_PERF_ || actionId == _ACTION_POINT_IMAGE_ || actionId == _ACTION_LOOK_POINT_SAY_ ||
		actionId == _ACTION_MOVE_ || actionId == _ACTION_LOOK_POINT_ || actionId == _ACTION_LOOK_)
		data.addInt(atoi(arg1.c_str()));
	if (actionId == _ACTION_GO_NEXT_STEP_ || actionId == _ACTION_SAY_)
		data.addString(arg1);
	cout<<"Sending suggested action "<<data.toString()<<endl;
	_yarpThread->sendToSuggestedAction(data);
	return;
}

void DeliberativeSubsystemYarpInterface::sendToSuggestedAction(int stepId, int actionId, int arg1) {
	if (!_controller->getOnScript())
		return;
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	data.addInt(arg1);

	_yarpThread->sendToSuggestedAction(data);
	return;
}

