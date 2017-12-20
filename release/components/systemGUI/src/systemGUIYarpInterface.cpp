/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Hoang-Long Cao, Vrije Universiteit Brussel 
 * Email:   hoang.long.cao@vub.ac.be
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
  * 22/06/2016 Version 1.0: ... (Hoang-Long Cao)
  * 25/01/2017 Version 2.0: ... (Hoang-Long Cao)
  */ 

#include "systemGUIYarpInterface.h"
#include "systemGUI.h"
#include "systemGUIController.h"

enum STR2INT_ERROR { SUCCESS, OVER, UNDER, INCONVERTIBLE };

int SystemGUIYarpInterface::str2int(int &i, char const *s, int base) {
	char *end;
	long  l;
	errno = 0;
	l = strtol(s, &end, base);
	if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
		return OVER;
	}
	if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
		return UNDER;
	}
	if (*s == '\0' || *end != '\0') {
		return INCONVERTIBLE;
	}
	i = l;
	return SUCCESS;
}

int SystemGUIYarpInterface::strToIntForMessage(string sInput) {
	int iReturn;

	if (str2int(iReturn, sInput.c_str(), 10) == SUCCESS) {
		return iReturn;
	}
	else {
		return _SCRIPT_CORRUPTED_;
	}
}

SystemGUIYarpInterface::SystemGUIYarpInterface(){

}

void SystemGUIYarpInterface::init(SystemGUIThread* yarpThread, SystemGUIController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void SystemGUIYarpInterface::getFromGetChildBehaviour(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void SystemGUIYarpInterface::getFromGetChildPerformance(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void SystemGUIYarpInterface::getFromProposedToSupervisor(Bottle data){
	/* Do Stuff: call methods from controller */
	stringstream ss;
    ss <<  data.get(0).asInt() <<","<< data.get(1).asInt();

	int iDelibId = data.get(1).asInt();

    // first switch on the delibID to create a good log message
    if (iDelibId == _ACTION_SAY_) {
        ss << "," << data.get(2).asString();
		cout << "systemGUI: proposedToSupervisor << " << data.get(0).asInt() << " " << data.get(1).asInt() << " " << data.get(2).asString() << endl;
    }
    else if (iDelibId == _ACTION_RECEIVED_PERF_) {
        ss << "," << data.get(2).asDouble();
		cout << "systemGUI: proposedToSupervisor << " << data.get(0).asInt() << " " << data.get(1).asInt() << " " << data.get(2).asDouble() << endl;
    }
    else if ( iDelibId == _ACTION_POINT_IMAGE_ ||
				iDelibId == _ACTION_LOOK_POINT_SAY_ ||
				iDelibId == _ACTION_LOOK_POINT_ ||
				iDelibId == _ACTION_LOOK_||
				iDelibId == _ACTION_CLOSE_HAND_||
				iDelibId == _ACTION_OPEN_HAND_||
				iDelibId == _ACTION_MOVE_||
				iDelibId == _ACTION_RECEIVED_GAZE_||
				iDelibId == _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_IMITATION_MOVE_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_WAITING_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_WAITING_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_LAST_TT_WAITING_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_SORTING_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_SORTING_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_CHOOSING_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_CHOOSING_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_PATTERN_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_PATTERN_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_BAD_NO_ACTION_ ||
				iDelibId == _ACTION_FEEDBACK_JA_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_JA_BAD_|| 	
				iDelibId == _ACTION_ROBOT_CONNECTED_||			
				iDelibId ==  _ACTION_SANDTRAY_TYPE_) {

        ss << "," << data.get(2).asInt();
		cout << "systemGUI: proposedToSupervisor << " << data.get(0).asInt() << " " << data.get(1).asInt() << " " << data.get(2).asInt() << endl;
    }

	else if (iDelibId == _ACTION_MOVE_HEAD_) {
        ss << "," << data.get(2).asDouble() << "," << data.get(3).asDouble() << "," << data.get(4).asDouble() << "," << data.get(5).asDouble();
		cout << "systemGUI: proposedToSupervisor << " << data.get(0).asInt() << " " << data.get(1).asInt() << " " << data.get(2).asDouble() << " " << data.get(3).asDouble() << " " << data.get(4).asDouble() << " " << data.get(5).asDouble() << endl;
    }
	else if (iDelibId == _ACTION_MOVE_HAND_) {
        ss << "," << data.get(2).asInt() << "," << data.get(3).asDouble() << "," << data.get(4).asDouble() << "," << data.get(5).asDouble() << "," << data.get(6).asDouble();
		cout << "systemGUI: proposedToSupervisor << " << data.get(0).asInt() << " " << data.get(1).asInt() << " " << data.get(2).asInt() << " " << data.get(3).asDouble() << " " << data.get(4).asDouble() << " " << data.get(5).asDouble() << " " << data.get(6).asDouble() << endl;
    }

	else cout << "systemGUI: proposedToSupervisor << " <<  data.get(0).asInt() << " " << data.get(1).asInt() << endl;

	string proposedToSupervisorData = ss.str();
	_controller->processProposedToSupervisor(proposedToSupervisorData);
	return;
}

void SystemGUIYarpInterface::getFromSmsSummary(Bottle data){
	/* Do Stuff: call methods from controller */
	user_interaction_data_t interactionData;
	interactionData.m_iInteractionId = data.get(0).asInt();
    interactionData.m_iScriptId = data.get(1).asInt();
	interactionData.m_sPerformance = data.get(2).asString();
    interactionData.m_iSessionId = data.get(3).asInt();
	interactionData.m_sSessionName = data.get(4).asString();
    interactionData.m_sSessionPartner = data.get(5).asString();
    interactionData.m_sEngagement = data.get(6).asString();
    interactionData.m_sTurnTake = data.get(7).asString();
	interactionData.m_sWP5Perf = data.get(8).asString();
	interactionData.m_sScriptTime = data.get(9).asString();
	_controller->processSmsSummary(interactionData);
	cout << "systemGUI: smsSummary << " <<	data.get(0).asInt() <<
											data.get(1).asInt() <<
											data.get(2).asString() <<
											data.get(3).asInt() <<
											data.get(4).asString() <<
											data.get(5).asString() <<
											data.get(6).asString() <<
											data.get(7).asString() <<
											data.get(8).asString() << 
											data.get(9).asString() << endl;
	return;
}

//Probably need to change type to allow to be called by non yarp class

void SystemGUIYarpInterface::sendToSelectedBySupervisor(vector<string> value){ 
	Bottle data;

	// stepID, action ID, int/string/double parameter(s)
	// Add stepID
	int stepId = atoi(value[0].c_str());
	data.addInt(stepId);
	// Add actionID
	int iDelibId = atoi(value[1].c_str());
	data.addInt(iDelibId);

	// Add parameter(s)

    // first switch on the delibID to create a good log message
    if (iDelibId == _ACTION_SAY_) {
		data.addString(value[2]);
    }
    else if (iDelibId == _ACTION_RECEIVED_PERF_) {
        data.addDouble(atof(value[2].c_str()));
    }
    else if ( iDelibId == _ACTION_POINT_IMAGE_ ||
				iDelibId == _ACTION_LOOK_POINT_SAY_ ||
				iDelibId == _ACTION_LOOK_POINT_ ||
				iDelibId == _ACTION_LOOK_||
				iDelibId ==_ACTION_CLOSE_HAND_||
				iDelibId == _ACTION_OPEN_HAND_||
				iDelibId == _ACTION_MOVE_||
				iDelibId == _ACTION_SANDTRAY_LOAD_LIBRARY_ ||
				iDelibId == _ACTION_SANDTRAY_ENABLE_FEEDBACK_ ||
				iDelibId == _ACTION_SANDTRAY_ENABLE_TOUCH_ ||
				iDelibId == _ACTION_SANDTRAY_ONE_BY_ONE_ ||
				iDelibId == _ACTION_RECEIVED_GAZE_ ||
				iDelibId == _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_IMITATION_MOVE_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_WAITING_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_WAITING_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_LAST_TT_WAITING_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_SORTING_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_SORTING_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_CHOOSING_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_CHOOSING_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_PATTERN_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_TT_PATTERN_BAD_ ||
				iDelibId == _ACTION_FEEDBACK_BAD_NO_ACTION_ ||
				iDelibId == _ACTION_FEEDBACK_JA_GOOD_ ||
				iDelibId == _ACTION_FEEDBACK_JA_BAD_|| 	
				iDelibId == _ACTION_ROBOT_CONNECTED_||			
				iDelibId ==  _ACTION_SANDTRAY_TYPE_||
				iDelibId ==  _ACTION_CHANGE_STEP_) {
        data.addInt(atoi(value[2].c_str()));
    }

	else if (iDelibId == _ACTION_MOVE_HEAD_) {
        data.addDouble(atof(value[2].c_str()));
		data.addDouble(atof(value[3].c_str()));
		data.addDouble(atof(value[4].c_str()));
		data.addDouble(atof(value[5].c_str()));
    }
	else if (iDelibId == _ACTION_MOVE_HAND_) {
        data.addInt(atoi(value[2].c_str()));
		data.addDouble(atof(value[3].c_str()));
		data.addDouble(atof(value[4].c_str()));
		data.addDouble(atof(value[5].c_str()));
		data.addDouble(atof(value[6].c_str()));
	}

	stringstream ss;
	for (unsigned int i=0;i<value.size();i++){
		ss << value[i] << " ";
	}
	cout << "systemGUI: selectedBySupervisor >> " << ss.str() << endl;		
	_yarpThread->sendToSelectedBySupervisor(data);
	return;
}

void SystemGUIYarpInterface::sendToSelectedBySupervisor(int origin, int scriptType, string partner, string type){ 
	Bottle data;
	data.addInt(origin);
	data.addInt(scriptType);
	if (type == "Intervention") data.addInt(0);	
	else if (type == "Diagnosis") data.addInt(1);
	if (partner == "Therapist") data.addInt(0);
	else if (partner == "Robot") data.addInt(1);
	cout << "systemGUI: selectedBySupervisor >> " << origin << " " << scriptType << " " << partner << " " << type << endl;		
	_yarpThread->sendToSelectedBySupervisor(data);
	return;
}

//JK TODO: this is where we will send the 'load' script message (probably other different things too)
void SystemGUIYarpInterface::sendToTherapistCommand(string command){ 
	Bottle data;
	data.addString(command);
	cout << "systemGUI: therapistCommand >> " << command << endl;
	_yarpThread->sendToTherapistCommand(data);
	return;
}

void SystemGUIYarpInterface::sendToTherapistCommand(string command, double value){ 
	Bottle data;
	data.addString(command);
	data.addDouble(value);
	cout << "systemGUI: therapistCommand >> " << command << " " << value << endl;
	_yarpThread->sendToTherapistCommand(data);
	return;
}

void SystemGUIYarpInterface::sendToTherapistCommand(string command, string sessionName, string sessionPartner){ 
	Bottle data;
	data.addString(command);
	data.addString(sessionName);
	data.addString(sessionPartner);
	cout << "systemGUI: therapistCommand >> " << command << " " << sessionName << " " << sessionPartner << endl;
	_yarpThread->sendToTherapistCommand(data);
	return;
}

void SystemGUIYarpInterface::sendToTherapistCommand(string action_stepmod, int mod){
	Bottle data;
	data.addString(action_stepmod);
	data.addInt(mod);
	cout << "systemGUI: therapistCommand >> " << action_stepmod << " " << mod << endl;
	_yarpThread->sendToTherapistCommand(data);
	return;
}

void SystemGUIYarpInterface::sendToUserID(int iNewID) {
	VectorOf<int> iDataToSend;
	iDataToSend.clear();
	iDataToSend.push_back(iNewID);
	cout << "systemGUI: userID >> " << iNewID << endl;
	_yarpThread->sendToUserID(iDataToSend);
	return;
}

void SystemGUIYarpInterface::sendToInterventionCommand(vector<string> data) {
	Bottle botToSend;
	/*
	cout << "Intervention command:";
	for (int i=0; i<data.size();i++){
		cout << data[i] << "," ;
	}
	cout << endl;*/

	// check we have at least 2 elements
	if (data.size() > 1) {
		botToSend.addInt(strToIntForMessage(data[0]));	// add step ID

		int iDelibId = strToIntForMessage(data[1]);		// add delib ID
		botToSend.addInt(iDelibId);
		
		// depending on deliberative command, we may have some arugments - check and add
		if (iDelibId == _SCRIPT_SAY_ || iDelibId == _SCRIPT_MOVE_ || iDelibId == _SCRIPT_SAY_IF_GOOD_PERF_) {
			// expecting 1 further arg as string
			if (data.size() > 2) {
				botToSend.addString(data[2]);
			}
			else {
				botToSend.addInt(_SCRIPT_CORRUPTED_);
			}
		}
		else if (iDelibId == _SCRIPT_WAIT_FOR_PERF_) {
			// expecting 4 further arg as ints
			if (data.size() > 4) {
				botToSend.addInt(strToIntForMessage(data[2]));
				botToSend.addInt(strToIntForMessage(data[3]));
				botToSend.addInt(strToIntForMessage(data[4]));
				if(data.size()==5)
					botToSend.addInt(0);
				else
					botToSend.addInt(strToIntForMessage(data[5]));
			}
			else {
				botToSend.addInt(_SCRIPT_CORRUPTED_);
				botToSend.addInt(_SCRIPT_CORRUPTED_);
			}
		}
		else if (iDelibId == _SCRIPT_WAIT_ || iDelibId == _SCRIPT_POINT_IMAGE_ || iDelibId == _SCRIPT_LOOK_POINT_SAY_ || iDelibId == _SCRIPT_LOOK_POINT_ || iDelibId == _SCRIPT_LOOK_ || iDelibId == _SCRIPT_SANDTRAY_LOAD_LIBRARY_) {
			// expecting 1 further arg as int
			if (data.size() > 2) {
				botToSend.addInt(strToIntForMessage(data[2]));
			}
			else {
				botToSend.addInt(_SCRIPT_CORRUPTED_);
			}
		}
	}
	else {
		// didn't have 2 elements, so something has gone wrong or we are finished
		if (data.size() > 0) {
			// convert 1st entry back to int and see if it is finished message
			int iOutput;
			if (str2int(iOutput, data[0].c_str(), 10) == SUCCESS) {
				if (iOutput == _SCRIPT_FINISHED_) {
					botToSend.addInt(_SCRIPT_FINISHED_);
					botToSend.addInt(_SCRIPT_FINISHED_);
				}
				else
				{
					botToSend.addInt(_SCRIPT_CORRUPTED_);
					botToSend.addInt(_SCRIPT_CORRUPTED_);
				}
			}
			else {
				botToSend.addInt(_SCRIPT_CORRUPTED_);
				botToSend.addInt(_SCRIPT_CORRUPTED_);
			}
		}
		else {
			botToSend.addInt(_SCRIPT_CORRUPTED_);
			botToSend.addInt(_SCRIPT_CORRUPTED_);
		}
	}
	cout << "sM - sending to InterventionCommand" << endl;
	_yarpThread->sendToInterventionCommand(botToSend);
	return;
}