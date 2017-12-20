/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author: Emmanuel Senft, PLYM 
 * Email:  emmanuel.senft@plymouth.ac.uk 
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
 * 14/09/2016 Version 1.0: ... (Emmanuel Senft)
 */ 

#include "deliberativeDriverYarpInterface.h"
#include "deliberativeDriver.h"
#include "deliberativeDriverController.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <stdlib.h>
#include "actionOrigin.h"
#include "actionMessages.h"

const int DEBUG = 1;

using namespace std;

DeliberativeDriverYarpInterface::DeliberativeDriverYarpInterface(){

}

void DeliberativeDriverYarpInterface::init(DeliberativeDriverThread* yarpThread, DeliberativeDriverController* controller){
  _controller = controller;
  _yarpThread = yarpThread;  
}

void DeliberativeDriverYarpInterface::getFromGetEyeGaze(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromGetGripLocation(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromGetHeadGaze(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromGetObjects(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromGetObjectTableDistance(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromGetSoundDirection(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromIdentifyFace(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromIdentifyFaceExpression(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromIdentifyObject(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromIdentifyTrajectory(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromTrackFace(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromTrackHand(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromTrackObject(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromInteractionEvent(VectorOf<int> data){
	/* Do Stuff: call methods from controller */

	if(DEBUG)
		cout << "-deliberativeDriver- Received interaction event " << data[0] << " " << data[1] << endl;
	_controller->getFromInteractionEvent(data[0], data[1]);
	return;
}

void DeliberativeDriverYarpInterface::getFromGetInterventionStatus(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	if (DEBUG)
		cout << "-deliberativeDriver- Received Intervention status " << data[0] << " " << data[1] << " " << data[2] << " " << data[3] << " " << data[4] << endl;
	if (!_controller->getSendPerf())
		return;
	int t = rand() % 1000 + 500;
	boost::this_thread::sleep(boost::posix_time::milliseconds(t));
	float perf = float(rand() % 100) / 100;
	sendToGetChildPerformance(perf);
	return;
}

void DeliberativeDriverYarpInterface::getFromAttentionBias(Bottle data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromCommandSuccess(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	if (DEBUG) {
		Sleep(50);
		cout << "-deliberativeDriver- Received on command success: " << data[0] << endl;
	}
	_controller->receiveSuccess();
	return;
}

void DeliberativeDriverYarpInterface::getFromStartStop(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromDeliberativeFeedback(Bottle data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromSensorySummary(Bottle data){
	/* Do Stuff: call methods from controller */
	return;
}

void DeliberativeDriverYarpInterface::getFromSandtrayCommand(Bottle data){
	/* Do Stuff: call methods from controller */
	if (DEBUG) {
		Sleep(70);
		cout << "-deliberativeDriver- Received on Sandtray command: " << data.get(0).asInt() << endl;
	}
    _controller->sendToSantrayReturn("Return Message");
 	return;
}

void DeliberativeDriverYarpInterface::getFromSuggestedAction(Bottle data){
	/* Do Stuff: call methods from controller */
	if (DEBUG) {
		Sleep(70);
		cout << "-deliberativeDriver- Received from suggested action step " << data.get(0).asInt() << " action " << data.get(1).asInt() << endl;
		Sleep(50);
	}
    
	if (data.get(0).asInt() != _ORIGIN_DELIBERATIVE_) {        
		sendToSelectedAction(data);
		_controller->simulateExecution(data.get(0).asInt(), data.get(1).asInt());
	}
    if (data.get(0).asInt() == _ORIGIN_DELIBERATIVE_ && data.get(1).asInt() == _ACTION_GO_NEXT_STEP_) {        
        _controller->receiveSuccess();
    }
    
	return;
}

//Probably need to change type to allow to be called by non yarp class
void DeliberativeDriverYarpInterface::sendToGetChildBehaviour(VectorOf<double> data){ 
	_yarpThread->sendToGetChildBehaviour(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetChildPerformance(float perf){ 
	if (DEBUG)
		cout << "-deliberativeDriver- sending performance of " << perf << endl;
	VectorOf<double> data;
	data.push_back(perf);
	_yarpThread->sendToGetChildPerformance(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToCheckMutualGaze(bool mutualGaze){ 
	VectorOf<int> data;
	data.push_back(mutualGaze);
	_yarpThread->sendToCheckMutualGaze(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetArmAngle(VectorOf<double> data){ 
	_yarpThread->sendToGetArmAngle(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetBody(VectorOf<double> data){ 
	_yarpThread->sendToGetBody(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetBodyPose(VectorOf<double> data){ 
	_yarpThread->sendToGetBodyPose(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetEyeGaze(VectorOf<double> data){ 
	_yarpThread->sendToGetEyeGaze(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetEyes(VectorOf<double> data){ 
	_yarpThread->sendToGetEyes(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetFaces(VectorOf<double> data){ 
	_yarpThread->sendToGetFaces(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetGripLocation(VectorOf<double> data){ 
	_yarpThread->sendToGetGripLocation(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetHands(VectorOf<double> data){ 
	_yarpThread->sendToGetHands(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetHead(VectorOf<double> data){ 
	_yarpThread->sendToGetHead(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetHeadGaze(VectorOf<double> data){ 
	_yarpThread->sendToGetHeadGaze(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetObjects(VectorOf<double> data){ 
	_yarpThread->sendToGetObjects(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetObjectTableDistance(VectorOf<double> data){ 
	_yarpThread->sendToGetObjectTableDistance(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToGetSoundDirection(VectorOf<double> data){ 
	_yarpThread->sendToGetSoundDirection(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToIdentifyFace(VectorOf<double> data){ 
	_yarpThread->sendToIdentifyFace(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToIdentifyFaceExpression(VectorOf<int> data){ 
	_yarpThread->sendToIdentifyFaceExpression(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToIdentifyObject(VectorOf<double> data){ 
	_yarpThread->sendToIdentifyObject(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToIdentifyTrajectory(VectorOf<double> data){ 
	_yarpThread->sendToIdentifyTrajectory(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToIdentifyVoice(VectorOf<int> data){ 
	_yarpThread->sendToIdentifyVoice(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToRecognizeSpeech(Bottle data){ 
	_yarpThread->sendToRecognizeSpeech(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToTrackFace(VectorOf<double> data){ 
	_yarpThread->sendToTrackFace(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToTrackObject(VectorOf<double> data){ 
	_yarpThread->sendToTrackObject(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToActionFeedback(int step, int actionId, bool success){
	Bottle data;
	data.addInt(step);
	data.addInt(actionId);
	data.addInt(success);
	_yarpThread->sendToActionFeedback(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToInterventionCommand(int stepId, int commandId, string arg) {
	Bottle data;
	data.addInt(stepId);
	data.addInt(commandId);
	data.addString(arg);
	_yarpThread->sendToInterventionCommand(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToInterventionCommand(int stepId, int commandId, int arg) {
	Bottle data;
	data.addInt(stepId);
	data.addInt(commandId);
	data.addInt(arg);
	_yarpThread->sendToInterventionCommand(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToInterventionCommand(int stepId, int commandId, int arg1, int arg2, int arg3) {
	Bottle data;
	data.addInt(stepId);
	data.addInt(commandId);
	data.addInt(arg1);
	data.addInt(arg2);
	data.addInt(arg3);
	_yarpThread->sendToInterventionCommand(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToInterventionCommand(int stepId, int commandId) {
	Bottle data;
	data.addInt(stepId);
	data.addInt(commandId);
	_yarpThread->sendToInterventionCommand(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToSelectedAction(Bottle data) {
	_yarpThread->sendToSelectedAction(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToSelectedAction(int stepId, int actionId) {
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	_yarpThread->sendToSelectedAction(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToSelectedAction(int stepId, int actionId, int arg) {
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	data.addInt(arg);
	_yarpThread->sendToSelectedAction(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToSelectedAction(int stepId, int actionId, double arg) {
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	data.addDouble(arg);
	_yarpThread->sendToSelectedAction(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToSelectedAction(int stepId, int actionId, string arg) {
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	data.addString(arg);
	_yarpThread->sendToSelectedAction(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToUserDelib(Bottle data){ 
	_yarpThread->sendToUserDelib(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToSandtrayEvent(std::string str){
	Bottle data;
	data.addString(str);
	_yarpThread->sendToSandtrayEvent(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToSandtrayReturn(std::string str){
	Bottle data;
	data.addString(str);
	_yarpThread->sendToSandtrayReturn(data);
	return;
}

void DeliberativeDriverYarpInterface::sendToRobotSensors(Bottle data) {
	_yarpThread->sendToRobotSensors(data);
	return;
}
