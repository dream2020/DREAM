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
  * 15/09/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "selfMonitoringDriverYarpInterface.h"
#include "selfMonitoringDriver.h"
#include "selfMonitoringDriverController.h"
#include "actionMessages.h"
#include "userDataLibrary.h"

using namespace std;

SelfMonitoringDriverYarpInterface::SelfMonitoringDriverYarpInterface(){
    _FirstRound = true;
}

void SelfMonitoringDriverYarpInterface::init(SelfMonitoringDriverThread* yarpThread, SelfMonitoringDriverController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void SelfMonitoringDriverYarpInterface::getFromProposedToSupervisor(Bottle data){
    cout << "<<<SMS Driver>>> proposed to supervisor, action: " << data.get(1).asDouble() << endl;
    
    // we are going to assume the therapist selected the suggested action for now and just loop it round
    sendToSelectedBySupervisor(data);
    return;
}

void SelfMonitoringDriverYarpInterface::getFromSmsSummary(Bottle data){
    if (_FirstRound) {
		_FirstRound = false;

		user_interaction_data_t newInteraction;

		// expecting a bottle with details from a new interaction
		newInteraction.m_iInteractionId = data.get(0).asInt();
		newInteraction.m_iScriptId = data.get(1).asInt();
		newInteraction.m_sPerformance = data.get(2).asString();
		newInteraction.m_iSessionId = data.get(3).asInt();
		newInteraction.m_sSessionName = data.get(4).asString();
		newInteraction.m_sSessionPartner = data.get(5).asString();
		newInteraction.m_sEngagement = data.get(6).asString();
		newInteraction.m_sTurnTake = data.get(7).asString();
		
		cout << "<<<SMS Driver>>> updated interaction data received for displaying from SMS" << endl;
		Sleep(100);
		cout << "<<<SMS Driver>>> interaction ID: " << newInteraction.m_iInteractionId << endl;
		Sleep(100);
		cout << "<<<SMS Driver>>> session ID: " << newInteraction.m_iSessionId << endl;
		Sleep(100);
		cout << "<<<SMS Driver>>> session partner: " << newInteraction.m_sSessionPartner << endl;
		Sleep(100);
		cout << "<<<SMS Driver>>> session name: " << newInteraction.m_sSessionName << endl;    
        
        _controller->continueTest();
    }
    
	return;
}

void SelfMonitoringDriverYarpInterface::getFromUpdatedData(Bottle data){
    user_interaction_data_t newInteraction;

    // expecting a bottle with details from a new interaction
    newInteraction.m_iInteractionId = data.get(0).asInt();
    newInteraction.m_iScriptId = data.get(1).asInt();
    newInteraction.m_sPerformance = data.get(2).asString();
    newInteraction.m_iSessionId = data.get(3).asInt();
    newInteraction.m_sSessionName = data.get(4).asString();
    newInteraction.m_sSessionPartner = data.get(5).asString();
    newInteraction.m_sEngagement = data.get(6).asString();
    newInteraction.m_sTurnTake = data.get(7).asString();

    cout << "<<<SMS Driver>>> updated interactiond data received for saving from SMS" << endl;
    Sleep(100);
    cout << "<<<SMS Driver>>> interaction ID: " << newInteraction.m_iInteractionId << endl;
    Sleep(100);
    cout << "<<<SMS Driver>>> session ID: " << newInteraction.m_iSessionId << endl;
    Sleep(100);
    cout << "<<<SMS Driver>>> session partner: " << newInteraction.m_sSessionPartner << endl;
    Sleep(100);
    cout << "<<<SMS Driver>>> session name: " << newInteraction.m_sSessionName << endl;

	return;
}

void SelfMonitoringDriverYarpInterface::getFromAffectiveState(Bottle data){
	cout << "<<<SMS Driver>>> affective state" << endl;
	return;
}

void SelfMonitoringDriverYarpInterface::getFromAttentionSwitchOff(VectorOf<int> data){
    if (data[0] == 1) {
        cout << "<<<SMS Driver>>> Attention switched off" << endl;
    }
    else {
        cout << "<<<SMS Driver>>> Attention switched on" << endl;
    }
	return;
}

void SelfMonitoringDriverYarpInterface::getFromReactionSwitchOff(VectorOf<int> data){
    if (data[0] == 1) {
        cout << "<<<SMS Driver>>> Reaction switched off" << endl;
    }
    else {
        cout << "<<<SMS Driver>>> Reaction switched on" << endl;
    }
    
	return;
}

void SelfMonitoringDriverYarpInterface::getFromTherapistGazeCommand(VectorOf<double> data){
	// not used in this version of the system
	return;
}

void SelfMonitoringDriverYarpInterface::getFromSelectedAction(Bottle data){
	stringstream ss;
	int stepId = data.get(0).asInt();
	int actionId = data.get(1).asInt();

	if (actionId == _ACTION_SAY_ || actionId == _ACTION_MOVE_)
		ss << data.get(2).asString();

	if (actionId == _ACTION_MOVE_ || actionId == _ACTION_POINT_IMAGE_ || actionId == _ACTION_LOOK_POINT_SAY_ || actionId == _ACTION_LOOK_POINT_ || actionId == _ACTION_LOOK_)
		ss << data.get(2).asInt();

	cout << "<<<SMS Driver>>> Received selected action " << actionId << " with arg " << ss.str() << endl;
	return;
}

void SelfMonitoringDriverYarpInterface::getFromUserDelib(Bottle data){
	// not used in this version of the system
	return;
}

void SelfMonitoringDriverYarpInterface::getFromStartStop(VectorOf<int> data){
	vector<int> iInput;
	for (unsigned i = 0; i < data.size(); i++) {
		iInput.push_back(data[i]);
	}
	int iScriptToLoad = data[0];
	cout << "<<<SMS Driver>>> script " << iScriptToLoad << " selected for loading" << endl;
	return;
}

void SelfMonitoringDriverYarpInterface::getFromCommandSuccess(VectorOf<int> data) {
	vector<int> iInput;
	for (unsigned i = 0; i < data.size(); i++) {
		iInput.push_back(data[i]);
	}
	
	// TODO: stuff

	return;
}


void SelfMonitoringDriverYarpInterface::sendToGetChildBehaviour(double dblIn1){
    // note: this will be a series of doubles in reality, but not specified in Wiki yet
    VectorOf<double> dblToSend;
    dblToSend.push_back(dblIn1);
	_yarpThread->sendToGetChildBehaviour(dblToSend);
	return;
}

void SelfMonitoringDriverYarpInterface::sendToGetChildPerformance(double dblIn1){ 
	VectorOf<double> dblToSend;
	dblToSend.push_back(dblIn1);
	_yarpThread->sendToGetChildPerformance(dblToSend);
	return;
}

void SelfMonitoringDriverYarpInterface::sendToInterventionCommand(int iOut1, int iOut2, std::string sMessage) {
    Bottle data;

    data.addInt(iOut1);
    data.addInt(iOut2);
    data.addString(sMessage);
    _yarpThread->sendToInterventionCommand(data);
    return;
}

void SelfMonitoringDriverYarpInterface::sendToEngagementFlag(double dblIn){
	VectorOf<double> dblToSend;
	dblToSend.push_back(dblIn);
	_yarpThread->sendToEngagementFlag(dblToSend);
	return;
}

void SelfMonitoringDriverYarpInterface::sendToSelectedBySupervisor(Bottle data){ 
	_yarpThread->sendToSelectedBySupervisor(data);
	return;
}

void SelfMonitoringDriverYarpInterface::sendToTherapistCommand(string sCommand, double dblParameter, string sParam1, string sParam2){
	Bottle data;

    data.addString(sCommand);

    if (sCommand == "Create Session") {
        data.addString(sParam1);
        data.addString(sParam2);
    }
    else {
        data.addDouble(dblParameter);
    }

	_yarpThread->sendToTherapistCommand(data);
	return;
}

void SelfMonitoringDriverYarpInterface::sendToDeliberativeFeedback(Bottle data){ 
	// Not used in this version of the system
	_yarpThread->sendToDeliberativeFeedback(data);
	return;
}

void SelfMonitoringDriverYarpInterface::sendToGetInterventionStatus(){ 
    VectorOf<int> iToSend;
    iToSend.push_back(8);   // script ID
    iToSend.push_back(1);   // script step
    iToSend.push_back(31);   // expected behave
    iToSend.push_back(0);   // param
    iToSend.push_back(3);   // time
    iToSend.push_back(1);   // on script
    iToSend.push_back(2);   // script type

	_yarpThread->sendToGetInterventionStatus(iToSend);
	return;
}

void SelfMonitoringDriverYarpInterface::sendToSensorySummary(Bottle data){
    // not used in this version of the system
	_yarpThread->sendToSensorySummary(data);
	return;
}

void SelfMonitoringDriverYarpInterface::sendToSuggestedAction(int stepId, int actionId, std::string arg1){
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	if (actionId == _ACTION_RECEIVED_PERF_ || actionId == _ACTION_POINT_IMAGE_ || actionId == _ACTION_LOOK_POINT_SAY_ || actionId == _ACTION_LOOK_POINT_ || actionId == _ACTION_LOOK_)
		data.addInt(atof(arg1.c_str()));
	if (actionId == _ACTION_GO_NEXT_STEP_ || actionId == _ACTION_SAY_ || actionId == _ACTION_MOVE_)
		data.addString(arg1);

	_yarpThread->sendToSuggestedAction(data);
	return;
}

void SelfMonitoringDriverYarpInterface::sendToActionFeedback(int iID, int iAction, int iSuccess, int iSubpart, int iParameter){ 
    Bottle bottleToSend;
    bottleToSend.addInt(iID);
    bottleToSend.addInt(iAction);
    bottleToSend.addInt(iSuccess);
    bottleToSend.addInt(iSubpart);
    bottleToSend.addInt(iParameter);
	_yarpThread->sendToActionFeedback(bottleToSend);
	return;
}

void SelfMonitoringDriverYarpInterface::sendToUserData(){
    Bottle dataToSend;

    dataToSend.addInt(99);
    dataToSend.addString("test user");
    dataToSend.addString("test gender");
    dataToSend.addString("test psych");
    dataToSend.addString("ados total");
    dataToSend.addString("ados comm");
    dataToSend.addString("ados social");
    dataToSend.addString("ados play");
    dataToSend.addString("ados stereo");
    dataToSend.addString("ados diag");
    dataToSend.addString("ados severity");
    dataToSend.addString("ados stereo1");
    dataToSend.addString("ados stereo2");
    dataToSend.addString("98");

    Bottle newInteraction;
    newInteraction.addInt(1);
    newInteraction.addInt(8);
    newInteraction.addString("");
    newInteraction.addInt(1);
    newInteraction.addString("Test Session");
    newInteraction.addString("Robot");
    newInteraction.addString("");
    newInteraction.addString("");
    dataToSend.addList() = newInteraction;

    _yarpThread->sendToUserData(dataToSend);
	return;
}

void SelfMonitoringDriverYarpInterface::sendToFallingInterruption(int iIn){ 
    VectorOf<int> iData;
    iData.push_back(iIn);
	_yarpThread->sendToFallingInterruption(iData);
	return;
}
