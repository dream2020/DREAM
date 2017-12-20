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
  * 04/07/2016 Version 1.0: ... (James Kennedy)
  * 09/11/2016 Version 1.0.1: Update with the current list of action messages (Hoang-Long Cao)
  */ 

#include "selfMonitoringSubsystemYarpInterface.h"
#include "selfMonitoringSubsystem.h"
#include "selfMonitoringSubsystemController.h"

#include "actionOrigin.h"
#include "actionMessages.h"
#include "moveSequenceId.h"

SelfMonitoringSubsystemYarpInterface::SelfMonitoringSubsystemYarpInterface() {

}

void SelfMonitoringSubsystemYarpInterface::init(SelfMonitoringSubsystemThread* yarpThread, SelfMonitoringSubsystemController* controller) {
   _controller = controller;
   _yarpThread = yarpThread;

   _dblLastEngagement = -1;
}

void SelfMonitoringSubsystemYarpInterface::getFromGetChildBehaviour(VectorOf<double> data) {
	// this does not exist in this version of the system (no input from WP5)
	return;
}

void SelfMonitoringSubsystemYarpInterface::getFromGetChildPerformance(VectorOf<double> data) {
	// this will be a single double in, between 0 and 1; just log here
	stringstream ss;
	ss << data(0);
	ss << ",";
	ss << data(1);

    // add this to the user model
    int iWP5Perf = int(data(0));
	int iWP5Checking = int(data(1));
    _controller->addToWP5Perf(iWP5Perf, iWP5Checking);

    // add to WP5 log
	string sLogMessage = "wp5_performance_in,";
	sLogMessage += ss.str();
	_controller->addToWP5Log(sLogMessage);
	return;
}

void SelfMonitoringSubsystemYarpInterface::getFromEngagementFlag(VectorOf<double> data) {
	double dblThisEngagement = data(0);

	if (_dblLastEngagement != dblThisEngagement) {
		// this will be a single double in, either 0 or 1; just log here
		_dblLastEngagement = dblThisEngagement;

		stringstream ss;
		ss << dblThisEngagement;

		string sLogMessage = "wp5_engagement_change,";
		sLogMessage += ss.str();
		_controller->addToWP5Log(sLogMessage);
	}
	return;
}

void SelfMonitoringSubsystemYarpInterface::getFromActionFeedback(Bottle data) {
	// formulate and send log message
	// int, int, int, int, int: actionUniqueID (Step in the script), actionSpec (which type of action it is), 
    // result (feedback message, 1 = success, !1 = failure), subAction (action subpart to complete an action step)
    // parameter (only for certain action types)
    int stepID = data.get(0).asInt();
    int actionID = data.get(1).asInt();
    int parameter = data.get(4).asInt();
    
    if (!(stepID == _ORIGIN_ATTENTION_REACTION_ && actionID == _ACTION_MOVE_ && parameter == _MOVE_BLINKING_)) {
		if (actionID != _ACTION_ROBOT_CONNECTED_ && actionID != _ACTION_SANDTRAY_TYPE_ && actionID != _ACTION_ENABLE_) {
			stringstream ss;
			ss << "ID:" << stepID;
			ss << ",Action:" << actionID;
			ss << ",Success:" << data.get(2).asInt();
			ss << ",SubAction:" << data.get(3).asInt();
			ss << ",Parameter:" << parameter;

			string sLogMessage = "action_feedback,";
			sLogMessage += ss.str();
			_controller->addToLog(sLogMessage);
		}
    }
	return;
}

void SelfMonitoringSubsystemYarpInterface::getFromFallingInterruption(VectorOf<int> data) {
	// receives from ARS - single int: 0,1,2 - all mean falling reaction has been triggered
	_controller->processInterruption(data(0));
	return;
}

void SelfMonitoringSubsystemYarpInterface::getFromGetInterventionStatus(VectorOf<int> data) {
	// log - no action needed here as delib is generating this
	// script_id, script_step, expected_behaviour_id, expected_behaviour_parameter, expected_behaviour_time_window, on_script
	stringstream ss;
	ss << "script_id:" << data(0);
	ss << ",script_step:" << data(1);
	ss << ",expected_behaviour_id:" << data(2);
	ss << ",expected_behaviour_parameter:" << data(3);
	ss << ",expected_behaviour_time_window:" << data(4);
	ss << ",on_script:" << data(5);
	ss << ",script_type_id:" << data(6);

	string sLogMessage = "intervention_status,";
	sLogMessage += ss.str();
	_controller->addToLog(sLogMessage);
	return;
}

void SelfMonitoringSubsystemYarpInterface::getFromDeliberativeFeedback(Bottle data) {
	// JK in first version of system, this will not be used
	// intention is that it receives updated trained model from delib for SPARC
	// SPARC not used in first system, so no need for this
	return;
}

void SelfMonitoringSubsystemYarpInterface::getFromInterventionCommand(Bottle data)
{
    // log the event in
    stringstream ss;
    ss << data.get(0).asInt();
    ss << ",";
    ss << data.get(1).asInt();
    ss << ",";

    string str = "";
    for (unsigned i = 2; i<data.size(); i++) {
        if (i != 2)
            str += ",";
        str += data.get(i).toString();
    }
    
    string sLogMessage = "intervention_command_in,";
    sLogMessage += ss.str();
    sLogMessage += str;

    _controller->addToLog(sLogMessage);
    return;
}

void SelfMonitoringSubsystemYarpInterface::getFromSensorySummary(Bottle data) {
	// this is the pipe from delib with updates from WP4 - in this version we just have checkMutualGaze
	// in future versions, the first index (pos 0) will give the data type
	stringstream ss;
	ss << data.get(1).asInt();

	string sLogMessage = "wp4_mutual_gaze_in,";
	sLogMessage += ss.str();
	_controller->addToLog(sLogMessage);
	return;
}

void SelfMonitoringSubsystemYarpInterface::getFromSuggestedAction(Bottle data) {
	// in future versions of the system, this will go into the controller, be checked and return
	// for this version, it get pumps straight back out (but logged first)
	int iDelibId = data.get(1).asInt();
	
	string str;
	for (unsigned i = 0; i<data.size(); i++) {
		if (i != 0)
			str += ",";
		str += data.get(i).toString();
	}

	if (iDelibId != _ACTION_ROBOT_CONNECTED_ || iDelibId != _ACTION_SANDTRAY_TYPE_) {
		string sLogMessage = "delib_command_out,";
		sLogMessage += str;
		cout << sLogMessage << endl;
		_controller->addToLog(sLogMessage);
	}
	if (iDelibId == _ACTION_FINISHED_) {
		_controller->processTherapistCommand(_ACTION_SCRIPTFINISHED_, 0);	// bit of a hack, as it isn't a therapist command
	}

	//if (iDelibId == _ACTION_ENABLE_ || iDelibId == _ACTION_DISABLE_)
	//	sendToSelectedAction(data);
	//else
	sendToProposedToSupervisor(data);
	//cout<<"Sent"<<endl;
	return;
}

void SelfMonitoringSubsystemYarpInterface::getFromSelectedBySupervisor(Bottle data) {
    int iDelibId = data.get(1).asInt();

	string str;
	for (unsigned i = 0; i<data.size(); i++) {
		if (i != 0)
			str += ",";
		str += data.get(i).toString();
	}

	int iParam = -1;
	if (iDelibId == _ACTION_RECEIVED_PERF_) {
		iParam = data.get(2).asInt();
	}

	if (iDelibId != _ACTION_SCRIPT_TYPE_) {
		// log the event
		string sLogMessage = "supervisor_command_out,";
		sLogMessage += str;
		_controller->addToLog(sLogMessage);

		// take any actions needed here
		_controller->processSupervisorSelection(iDelibId, iParam);
	}

    // forward to delib
	sendToSelectedAction(data);
	return;
}

void SelfMonitoringSubsystemYarpInterface::getFromTherapistCommand(Bottle data) {
	// possible commands in:
	// _ACTION_SCRIPTLOAD_ >0 (script id)
	// _ACTION_STOPSCRIPT_ 0 (no param)
	// _ACTION_REACTION_ 1,0 (on|off)
	// _ACTION_ATTENTION_ 1,0 (on|off)
	// _ACTION_COUNTDOWN_ 3->10
	// _ACTION_CREATE_ session_name (string), interaction partner (string)
	// _ACTION_STEPMOD_ ; _ACTION_REPEAT_, _ACTION_PREVIOUS_, _ACTION_NEXT_
	
	string sCommand = data.get(0).asString();

	if (sCommand == _ACTION_CREATE_) {
		_controller->setSessionName(data.get(1).asString());
		_controller->setInteractionPartner(data.get(2).asString());
	}
	else {
		double dblParameter = data.get(1).asDouble();
		_controller->processTherapistCommand(sCommand, dblParameter);
	}

	return;
}

void SelfMonitoringSubsystemYarpInterface::getFromUserData(Bottle dataIn) {
    UserDataLibrary userDataIn;

    userDataIn.iUserId = dataIn.get(0).asInt();
    userDataIn.sUserName = dataIn.get(1).asString();
    userDataIn.sGender = dataIn.get(2).asString();
    userDataIn.sPsych = dataIn.get(3).asString();
    userDataIn.sADOSTotal = dataIn.get(4).asString();
    userDataIn.sADOSComm = dataIn.get(5).asString();
    userDataIn.sADOSSocial = dataIn.get(6).asString();
    userDataIn.sADOSPlay = dataIn.get(7).asString();
    userDataIn.sADOSStereo = dataIn.get(8).asString();
    userDataIn.sADOSDiag = dataIn.get(9).asString();
    userDataIn.sADOSSeverity = dataIn.get(10).asString();
    userDataIn.sStereo1 = dataIn.get(11).asString();
    userDataIn.sStereo2 = dataIn.get(12).asString();
    userDataIn.sIQ = dataIn.get(13).asString();

    int iDataSize = dataIn.size();	// assigning this to an int prevents an unsigned mismatch warning

    for (int i = 14; i < iDataSize; i++) {
        Bottle * interaction = dataIn.get(i).asList();
        user_interaction_data_t newInteraction;
        newInteraction.m_iInteractionId = interaction->get(0).asInt();
        newInteraction.m_iScriptId = interaction->get(1).asInt();
        newInteraction.m_sPerformance = interaction->get(2).asString();
        newInteraction.m_iSessionId = interaction->get(3).asInt();
        newInteraction.m_sSessionName = interaction->get(4).asString();
        newInteraction.m_sSessionPartner = interaction->get(5).asString();
        newInteraction.m_sEngagement = interaction->get(6).asString();
        newInteraction.m_sTurnTake = interaction->get(7).asString();
        newInteraction.m_sWP5Perf = interaction->get(8).asString();
		newInteraction.m_sScriptTime = interaction->get(9).asString();
		newInteraction.m_sScriptEnd = interaction->get(10).asString();
		newInteraction.m_sWP5Turn = interaction->get(11).asString();

        userDataIn.m_user_interaction.push_back(newInteraction);
    }

    _controller->receiveUserData(userDataIn);
    	
	// also forward the bottle to the systemGUI
	//sendToUserPref(dataIn);	// JK: not done anymore; GUI reads it directly
	return;
}


void SelfMonitoringSubsystemYarpInterface::sendToAffectiveState(Bottle data) {
	// NOT NEEDED FOR FIRST SYSTEM VERSION
	_yarpThread->sendToAffectiveState(data);
	return;
}

void SelfMonitoringSubsystemYarpInterface::sendToAttentionSwitchOff(bool bSwitchOff) {
	VectorOf<int> iToSend;
	iToSend.clear();
	if (bSwitchOff) {
		iToSend.push_back(1);
	}
	else {
		iToSend.push_back(0);
	}
	_yarpThread->sendToAttentionSwitchOff(iToSend);
	return;
}

void SelfMonitoringSubsystemYarpInterface::sendToReactionSwitchOff(bool bSwitchOff) {
	VectorOf<int> iToSend;
	iToSend.clear();
	if (bSwitchOff) {
		iToSend.push_back(1);
	}
	else {
		iToSend.push_back(0);
	}
	_yarpThread->sendToReactionSwitchOff(iToSend);
	return;
}

void SelfMonitoringSubsystemYarpInterface::sendToTherapistGazeCommand(int xCoord, int yCoord, int zCoord) {
	VectorOf<double> data;
	data.push_back(xCoord);
	data.push_back(yCoord);
	data.push_back(zCoord);
	_yarpThread->sendToTherapistGazeCommand(data);
	return;
}

void SelfMonitoringSubsystemYarpInterface::sendToSelectedAction(Bottle data) { 
	_yarpThread->sendToSelectedAction(data);
	return;
}

void SelfMonitoringSubsystemYarpInterface::sendToUserDelib(Bottle data) {
	// JK in first version of system, this will not be used
	// intention is that it sends delib the trained model for SPARC
	// SPARC not used in first system, so no need for this
	_yarpThread->sendToUserDelib(data);
	return;
}

void SelfMonitoringSubsystemYarpInterface::sendToProposedToSupervisor(Bottle data) {
	int iDelibId = data.get(1).asInt();
	//cout<<"enter proposed"<<endl;
	// script done, get latest data and save it to user model
    if (iDelibId == _ACTION_FINISHED_) {
        user_interaction_data_t interactionData = _controller->getCurrentInteractionData();
        
		Bottle currentInteraction;
		currentInteraction.addInt(interactionData.m_iInteractionId);
		currentInteraction.addInt(interactionData.m_iScriptId);
		currentInteraction.addString(interactionData.m_sPerformance);
		currentInteraction.addInt(interactionData.m_iSessionId);
		currentInteraction.addString(interactionData.m_sSessionName);
		currentInteraction.addString(interactionData.m_sSessionPartner);
		currentInteraction.addString(interactionData.m_sEngagement);
		currentInteraction.addString(interactionData.m_sTurnTake);
        currentInteraction.addString(interactionData.m_sWP5Perf);
		currentInteraction.addString(interactionData.m_sScriptTime);
		currentInteraction.addString(interactionData.m_sScriptEnd);
		currentInteraction.addString(interactionData.m_sWP5Turn);
		//cout<<1<<endl;
		_yarpThread->sendToUpdatedData(currentInteraction);

        _controller->closeLogFiles();
	}
	else {
		//cout<<2<<endl;
		_yarpThread->sendToProposedToSupervisor(data);
	}
	//cout<<"done"<<endl;
	return;
}

void SelfMonitoringSubsystemYarpInterface::sendToSmsSummary(user_interaction_data_t interactionData) {
    // this is used as an event channel from SMS to GUI
    // for now only the interaction data needs to go when performance etc gets updated
    Bottle currentInteraction;
    currentInteraction.addInt(interactionData.m_iInteractionId);
    currentInteraction.addInt(interactionData.m_iScriptId);
    currentInteraction.addString(interactionData.m_sPerformance);
    currentInteraction.addInt(interactionData.m_iSessionId);
    currentInteraction.addString(interactionData.m_sSessionName);
    currentInteraction.addString(interactionData.m_sSessionPartner);
    currentInteraction.addString(interactionData.m_sEngagement);
    currentInteraction.addString(interactionData.m_sTurnTake);
    currentInteraction.addString(interactionData.m_sWP5Perf);
	currentInteraction.addString(interactionData.m_sScriptTime);
	currentInteraction.addString(interactionData.m_sScriptEnd);
	currentInteraction.addString(interactionData.m_sWP5Turn);
    
	_yarpThread->sendToSmsSummary(currentInteraction);
	return;
}

void SelfMonitoringSubsystemYarpInterface::sendToStartStop(int iScriptId, int iStartStop) {
	VectorOf<int> data;
	data.push_back(iScriptId);		// script number
	data.push_back(iStartStop);		// this will start the script too (put as 0 to just load, not start)
	_yarpThread->sendToStartStop(data);
	return;
}

void SelfMonitoringSubsystemYarpInterface::sendToUpdatedData(user_interaction_data_t newInteraction) {
    Bottle dataToSend;

    dataToSend.addInt(newInteraction.m_iInteractionId);
    dataToSend.addInt(newInteraction.m_iScriptId);
    dataToSend.addString(newInteraction.m_sPerformance);
    dataToSend.addInt(newInteraction.m_iSessionId);
    dataToSend.addString(newInteraction.m_sSessionName);
    dataToSend.addString(newInteraction.m_sSessionPartner);
    dataToSend.addString(newInteraction.m_sEngagement);
    dataToSend.addString(newInteraction.m_sTurnTake);
    dataToSend.addString(newInteraction.m_sWP5Perf);
	dataToSend.addString(newInteraction.m_sScriptTime);
	dataToSend.addString(newInteraction.m_sScriptEnd);
	dataToSend.addString(newInteraction.m_sWP5Turn);

    _yarpThread->sendToUpdatedData(dataToSend);
	return;
}

void SelfMonitoringSubsystemYarpInterface::sendToCommandSuccess(int iSuccess, int iScriptMod) {
	VectorOf<int> data;
	data.push_back(iSuccess);		// success/not
	data.push_back(iScriptMod);		// script action (repeat, previous, next)
	_yarpThread->sendToCommandSuccess(data);
	return;
}
