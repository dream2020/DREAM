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
  */ 

#include "selfMonitoringSubsystemController.h"
#include "selfMonitoringSubsystemYarpInterface.h"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

using namespace std;

SelfMonitoringSubsystemController::SelfMonitoringSubsystemController(SelfMonitoringSubsystemYarpInterface *yarpInterface, string sContextPath){
	_yarpInterface = yarpInterface;
	_iUserID = -1;
	_sSessionName = "";
    _iSessionID = -1;
	_sInteractionPartner = "";
    _iLastSessionID = -1;
    _iLastInteractionID = -1;
	_bInteractionInProgress = false;

    // go up 3 levels from the context, then into the log directory, then our directory
    _logPath = fs::path(sContextPath.c_str()).parent_path().parent_path().parent_path().string();
    _logPath = _logPath + "/logs/SMS/";

    clearCurrentInteraction();
}

void SelfMonitoringSubsystemController::clearCurrentInteraction() {
    _iIntTurnPerformance.clear();
    _iIntPerformance.clear();
    _iIntEngagement.clear();
    _iWP5Performance.clear();
	_iWP5TurnTake.clear();
    _iCurrentScript = -1;

    _currentInteraction.m_iInteractionId = -1;
    _currentInteraction.m_sPerformance = "";
    _currentInteraction.m_sEngagement = "";
    _currentInteraction.m_sTurnTake = "";
    _currentInteraction.m_sWP5Perf = "";
    _currentInteraction.m_iScriptId = -1;
    _currentInteraction.m_iSessionId = -1;
    _currentInteraction.m_sSessionName = "";
    _currentInteraction.m_sSessionPartner = "";
	_currentInteraction.m_sScriptTime = "";
	_currentInteraction.m_sScriptEnd = "";
}

string SelfMonitoringSubsystemController::createLogFile(int iScriptId) {
	string sTimeNow = getTimeStamp();
	string sUserId = int2str(_iUserID);
	string sScriptId = int2str(iScriptId);

    fs::create_directories(_logPath);
	_sFileName = _logPath + sTimeNow + "_User" + sUserId + "_Script" + sScriptId + ".log";		// start file name with timestamp
	_sWP5FileName = _logPath + sTimeNow + "_User" + sUserId + "_Script" + sScriptId + "_WP5.log";

	_wp5LogFile.open(_sWP5FileName, fstream::out);
	_logFile.open(_sFileName, fstream::out);
	if (_logFile.is_open()) {
		_logFile << sTimeNow << ",script_started\n";
		_logFile << sTimeNow << ",script_selected:" << sScriptId << "\n";
		_logFile.flush();
	}
	else {
		cout << "<<<SMS>>> failed to create log file" << endl;
	}

	return sTimeNow;
}

void SelfMonitoringSubsystemController::closeLogFiles()
{
	_bInteractionInProgress = false;

    if (_logFile.is_open()) {
        _logFile.flush();
        _logFile.close();
    }

    if (_wp5LogFile.is_open()) {
        _wp5LogFile.flush();
        _wp5LogFile.close();
    }
}

void SelfMonitoringSubsystemController::addToLog(string sLogMessage) {
	string sTimeNow = getTimeStamp();

	//if (_logFile.is_open())
	//	return;

	//_logFile.open(_sFileName, fstream::app);
    _logMutex.lock();
	if (_logFile.is_open()) {
		_logFile << sTimeNow << "," << sLogMessage << "\n";
        _logFile.flush();
	}
	else {
		cout << "<<<SMS>>> failed to re-open log file for writing" << endl;
	}
    _logMutex.unlock();
}

void SelfMonitoringSubsystemController::addToWP5Log(string sLogMessage) {
	string sTimeNow = getTimeStamp();
	
	//if (_wp5LogFile.is_open())
	//	return;

	if (_bInteractionInProgress) {
		//_wp5LogFile.open(_sWP5FileName, fstream::app);
		_wp5Mutex.lock();
		if (_wp5LogFile.is_open()) {
			_wp5LogFile << sTimeNow << "," << sLogMessage << "\n";
		}
		else {
			cout << "<<<SMS>>> failed to re-open WP5 log file for writing" << endl;
		}
		_wp5Mutex.unlock();
	}
}

string SelfMonitoringSubsystemController::getTimeStamp() {
    time_t t = time(0);						        // get time now
	struct tm * now = localtime(&t);

    char buffer[80];
    strftime(buffer, 80, "%Y%m%d_%H%M%S", now);   // be careful as this version of C++ doesn't support all standard formats!
    string str(buffer);
    return str;
}

void SelfMonitoringSubsystemController::receiveUserData(UserDataLibrary userDataIn) {
    _iUserID = userDataIn.iUserId;

    for (unsigned int i = 0; i < userDataIn.m_user_interaction.size(); i++) {
        // keep track of highest previous values
        if (userDataIn.m_user_interaction[i].m_iInteractionId > _iLastInteractionID) {
            _iLastInteractionID = userDataIn.m_user_interaction[i].m_iInteractionId;
        }
        if (userDataIn.m_user_interaction[i].m_iSessionId > _iLastSessionID) {
            _iLastSessionID = userDataIn.m_user_interaction[i].m_iSessionId;
        }
    }

    _iSessionID = _iLastSessionID + 1;  // create the session ID for this session
}

void SelfMonitoringSubsystemController::processTherapistCommand(string sCommand, double dblParameter) {
	// possible commands in:
	// scriptload >0 (script id)
	// stopscript 0 (no param)
	// Reaction Subsystem 1,0 (on|off)
	// Attention Subsystem 1,0 (on|off)
	// countdown 3->10

	int iParameter = (int)dblParameter;

	if (sCommand == _ACTION_SCRIPTLOAD_) {
		string sScriptTime = createLogFile(iParameter);
		clearCurrentInteraction();
		_iCurrentScript = iParameter;							// get script ID
		_currentInteraction.m_iScriptId = _iCurrentScript;		// this gets sent to UM at end, update now
		_currentInteraction.m_iSessionId = _iSessionID;
		_currentInteraction.m_sSessionName = _sSessionName;
		_currentInteraction.m_sSessionPartner = _sInteractionPartner;
		_currentInteraction.m_iInteractionId = _iLastInteractionID + 1;
		_currentInteraction.m_sScriptTime = sScriptTime;
		_iLastInteractionID = _currentInteraction.m_iInteractionId;
		
		_yarpInterface->sendToAttentionSwitchOff(1);			// turn off attention
		_yarpInterface->sendToReactionSwitchOff(1);				// turn off reaction

		_bInteractionInProgress = true;

		user_interaction_data_t interactionData;
		interactionData = getCurrentInteractionData();
		_yarpInterface->sendToUpdatedData(interactionData);     // send for saving in userModel
		//_yarpInterface->sendToStartStop(iParameter, 1);			// load and start script   // old scriptManager code
	}
	else if (sCommand == _ACTION_STOPSCRIPT_) {
		//_yarpInterface->sendToStartStop(_iCurrentScript, 0);	// stop script   // old scriptManager code
		addToLog("gui_emergency_stop");

		_bInteractionInProgress = false;

		user_interaction_data_t interactionData;
		interactionData = getCurrentInteractionData();
		_yarpInterface->sendToUpdatedData(interactionData);     // send for saving in userModel
	}
	else if (sCommand == _ACTION_SCRIPTFINISHED_) {
		_bInteractionInProgress = false;

		user_interaction_data_t interactionData;
		interactionData = getCurrentInteractionData();
		_yarpInterface->sendToUpdatedData(interactionData);     // send for saving in userModel
	}
	else if (sCommand == _ACTION_REACTION_) {
		// we get in 1 for on, 0 for off - so invert and bool to send
		_yarpInterface->sendToReactionSwitchOff((1 - iParameter) != 0);
		addToLog("reactive_switch," + int2str(iParameter));
	}
	else if (sCommand == _ACTION_ATTENTION_) {
		// we get in 1 for on, 0 for off - so invert and bool to send
		_yarpInterface->sendToAttentionSwitchOff((1 - iParameter) != 0);
		addToLog("attention_switch," + int2str(iParameter));
	}
	else if (sCommand == _ACTION_COUNTDOWN_) {
		addToLog("countdown_change," + int2str(iParameter));
	}
	else if (sCommand == _ACTION_STEPMOD_) {
        addToLog("stepmod_action," + int2str(iParameter));
		//_yarpInterface->sendToCommandSuccess(0, iParameter);  // old scriptManager code
	}
	else {
		cout << "<<<SMS>>> therapist command in: no command recognised" << endl;
	}
}

void SelfMonitoringSubsystemController::processInterruption(int iInterruption) {
	// any int here means the robot has fallen
	// should send a stop to the delib/SM
	notifyDelibAndSMStop();
	addToLog("falling_stop," + int2str(iInterruption));
}

void SelfMonitoringSubsystemController::notifyDelibAndSMStop() {
	// we have received an emergency stop of some form - let GUI and SM know
    // TODO
}

void SelfMonitoringSubsystemController::processSupervisorSelection(int iDelibId, int iParam) {
    user_interaction_data_t interactionData;
    
    // switch on the delibID to take any actions
    switch (iDelibId) {
        // these are all the options for a 1 performance
        case _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_:
        case _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_:
        case _ACTION_FEEDBACK_TT_SORTING_GOOD_:
        case _ACTION_FEEDBACK_TT_CHOOSING_GOOD_:
        case _ACTION_FEEDBACK_TT_PATTERN_GOOD_:
        case _ACTION_FEEDBACK_JA_GOOD_:
            addToPerformance(1);
            interactionData = getCurrentInteractionData();
            _yarpInterface->sendToSmsSummary(interactionData);      // send to GUI
            _yarpInterface->sendToUpdatedData(interactionData);     // send for saving in userModel
            break;

        // these are all the options for a 0 performance
        case _ACTION_FEEDBACK_IMITATION_MOVE_BAD_:
        case _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_:
        case _ACTION_FEEDBACK_TT_SORTING_BAD_:
        case _ACTION_FEEDBACK_TT_CHOOSING_BAD_:
        case _ACTION_FEEDBACK_TT_PATTERN_BAD_:
        case _ACTION_FEEDBACK_JA_BAD_:
		case _ACTION_FEEDBACK_BAD_NO_ACTION_:
            addToPerformance(0);
            interactionData = getCurrentInteractionData();
            _yarpInterface->sendToSmsSummary(interactionData);      // send to GUI
            _yarpInterface->sendToUpdatedData(interactionData);     // send for saving in userModel
            break;

        // options for good turn taking
        case _ACTION_FEEDBACK_TT_WAITING_GOOD_:
            addToTurnTake(1);
            interactionData = getCurrentInteractionData();
            _yarpInterface->sendToSmsSummary(interactionData);      // send to GUI
            _yarpInterface->sendToUpdatedData(interactionData);     // send for saving in userModel
            break;

        // options for bad turn taking
        case _ACTION_FEEDBACK_LAST_TT_WAITING_BAD_:
        case _ACTION_FEEDBACK_TT_WAITING_BAD_:
            addToTurnTake(0);
            interactionData = getCurrentInteractionData();
            _yarpInterface->sendToSmsSummary(interactionData);      // send to GUI
            _yarpInterface->sendToUpdatedData(interactionData);     // send for saving in userModel
            break;

		/*case _ACTION_RECEIVED_PERF_:
			addToPerformance(iParam);
			interactionData = getCurrentInteractionData();
			_yarpInterface->sendToSmsSummary(interactionData);      // send to GUI
			_yarpInterface->sendToUpdatedData(interactionData);     // send for saving in userModel
			break;*/
    }
}

int SelfMonitoringSubsystemController::getCurrentPerformance() {
    int iCurrentPerf = 0;

    for (unsigned int i = 0; i < _iIntPerformance.size(); i++) {
        iCurrentPerf += 1;
    }

	return iCurrentPerf;
}

int SelfMonitoringSubsystemController::getCurrentEngagement() {
    int iCurrentPerf = 0;

    for (unsigned int i = 0; i < _iIntEngagement.size(); i++) {
        iCurrentPerf += 1;
    }

    return iCurrentPerf;
}

int SelfMonitoringSubsystemController::getCurrentTurnTake() {
    int iCurrentPerf = 0;

    for (unsigned int i = 0; i < _iIntTurnPerformance.size(); i++) {
        iCurrentPerf += 1;
    }

    return iCurrentPerf;
}

int SelfMonitoringSubsystemController::getCurrentWP5Perf() {
    int iCurrentPerf = 0;

    for (unsigned int i = 0; i < _iWP5Performance.size(); i++) {
        iCurrentPerf += 1;
    }

    return iCurrentPerf;
}

int SelfMonitoringSubsystemController::getCurrentWP5TurnTake() {
    int iCurrentPerf = 0;

    for (unsigned int i = 0; i < _iWP5TurnTake.size(); i++) {
        iCurrentPerf += 1;
    }

    return iCurrentPerf;
}

user_interaction_data_t SelfMonitoringSubsystemController::getCurrentInteractionData() {
    string sTimeNow = getTimeStamp();
	
	// this is called before a save, so put in all updated performance data
    string sPerformance = performanceArrayToString(_iIntPerformance);
    string sEngagement = performanceArrayToString(_iIntEngagement);
    string sTurnTake = performanceArrayToString(_iIntTurnPerformance);
    string sWP5Perf = performanceArrayToString(_iWP5Performance);
	string sWP5Turn = performanceArrayToString(_iWP5TurnTake);

    _currentInteraction.m_sPerformance = sPerformance;
    _currentInteraction.m_sEngagement = sEngagement;
    _currentInteraction.m_sTurnTake = sTurnTake;
    _currentInteraction.m_sWP5Perf = sWP5Perf;
	_currentInteraction.m_sScriptEnd = sTimeNow;
	_currentInteraction.m_sWP5Turn = sWP5Turn;
	
	return _currentInteraction;
}

void SelfMonitoringSubsystemController::setInteractionPartner(string sPartner) {
	_sInteractionPartner = sPartner;
}

void SelfMonitoringSubsystemController::setSessionName(string sSession) {
	_sSessionName = sSession;
}

void SelfMonitoringSubsystemController::addToPerformance(int iPerformance) {
    _iIntPerformance.push_back(iPerformance);
}

void SelfMonitoringSubsystemController::addToEngagement(int iEngagement) {
    _iIntEngagement.push_back(iEngagement);
}

void SelfMonitoringSubsystemController::addToTurnTake(int iTurnTake) {
    _iIntTurnPerformance.push_back(iTurnTake);
}

void SelfMonitoringSubsystemController::addToWP5Perf(int iWP5Perf, int iWP5Checking) {
	// separate logging of turn taking vs task based performances
	if (iWP5Checking == _PERF_SANDTRAY_NO_TOUCH_ || iWP5Checking == _PERF_SANDTRAY_TOUCH_ || iWP5Checking == _PERF_NO_MOVEMENT_) {
		_iWP5TurnTake.push_back(iWP5Perf);
	} 
	else {
		_iWP5Performance.push_back(iWP5Perf);
	}
    
}

string SelfMonitoringSubsystemController::int2str(int iInput) {
	stringstream ss;
	ss << iInput;
	return ss.str();
}

string SelfMonitoringSubsystemController::performanceArrayToString(vector<int> iPerformanceIn) {
    if (iPerformanceIn.size() < 1) {
		return "";
	}
	
	stringstream ss;
    ss.clear();
    const char* separator = "";

    for (unsigned int i = 0; i < iPerformanceIn.size(); i++) {
        ss << separator << iPerformanceIn[i];
        separator = ",";
    }

    return ss.str();
}