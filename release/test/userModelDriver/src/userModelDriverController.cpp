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
  * 18/08/2016 Version 1.0: ... (James Kennedy)
  */

#include "userModelDriverController.h"
#include "userModelDriverYarpInterface.h"

using namespace std;

UserModelDriverController::UserModelDriverController(UserModelDriverYarpInterface *yarpInterface) {
	_yarpInterface = yarpInterface;

	_bTest = false;
	_iLastInteractionID = 0;
	_iLastScriptID = 0;
    _iLastSessionID = 0;
    _sLastSessionPartner = "";
}

void UserModelDriverController::test() {
	cout << "<<<userModelDriver>>> test started" << endl;
	_bTest = true;
	sendNewID(1);
	cout << "<<<userModelDriver>>> new ID of 1 sent - waiting for user data return" << endl;
}

void UserModelDriverController::sendNewID(int iNewID) {
	_yarpInterface->sendToUserID(iNewID);
}

void UserModelDriverController::sendNewInteraction(user_interaction_data_t newInteraction) {
	_yarpInterface->sendToUpdatedData(newInteraction);
}

void UserModelDriverController::receiveUserData(UserDataLibrary userDataIn) {
	cout << "<<<userModelDriver>>> user data returned from userModel..." << endl;
	cout << "user ID:\t\t" << userDataIn.iUserId << endl;
	cout << "user name:\t\t" << userDataIn.sUserName << endl;
	cout << "gender:\t\t\t" << userDataIn.sGender << endl;
	cout << "diagnosis:\t\t" << userDataIn.sPsych << endl;
	cout << "ADOS total:\t\t" << userDataIn.sADOSTotal << endl;
	cout << "ADOS comm:\t\t" << userDataIn.sADOSComm << endl;
	cout << "ADOS social:\t\t" << userDataIn.sADOSSocial << endl;
	cout << "ADOS play:\t\t" << userDataIn.sADOSPlay << endl;
	cout << "ADOS stereo:\t\t" << userDataIn.sADOSStereo << endl;
	cout << "ADOS diag:\t\t" << userDataIn.sADOSDiag << endl;
	cout << "ADOS severity:\t\t" << userDataIn.sADOSSeverity << endl;
	cout << "Stereo behave 1:\t" << userDataIn.sStereo1 << endl;
	cout << "Stereo behave 2:\t" << userDataIn.sStereo2 << endl;
	cout << "IQ:\t\t\t" << userDataIn.sIQ << endl;

	_iLastInteractionID = 0;
	_iLastScriptID = 0;
    _iLastSessionID = 0;
    _sLastSessionPartner = "";

	cout << "<<<userModelDriver>>> previous interaction data returned from userModel..." << endl;
	for (unsigned int i = 0; i < userDataIn.m_user_interaction.size(); i++) {
		cout << "interaction ID:\t" << userDataIn.m_user_interaction[i].m_iInteractionId << endl;
		Sleep(100);	// just to prevent message crossover
		cout << "script ID:\t" << userDataIn.m_user_interaction[i].m_iScriptId << endl;
        Sleep(100);	// just to prevent message crossover
        cout << "session ID:\t" << userDataIn.m_user_interaction[i].m_iSessionId << endl;
		Sleep(100);	// just to prevent message crossover
		cout << "performance:\t" << userDataIn.m_user_interaction[i].m_sPerformance << endl;
		Sleep(100);	// just to prevent message crossover
        cout << "engagement:\t" << userDataIn.m_user_interaction[i].m_sEngagement << endl;
        Sleep(100);	// just to prevent message crossover
        cout << "turn taking:\t" << userDataIn.m_user_interaction[i].m_sTurnTake << endl;
        Sleep(100);	// just to prevent message crossover
        cout << "session name:\t" << userDataIn.m_user_interaction[i].m_sSessionName << endl;
        Sleep(100);	// just to prevent message crossover
        cout << "session partner:\t" << userDataIn.m_user_interaction[i].m_sSessionPartner << endl;
        Sleep(100);	// just to prevent message crossover
        cout << "WP5 perf:\t" << userDataIn.m_user_interaction[i].m_sWP5Perf << endl;
        Sleep(100);	// just to prevent message crossover
		cout << "<<<userModelDriver>>> end of interaction" << endl;

		// keep track of highest previous values
		if (userDataIn.m_user_interaction[i].m_iInteractionId > _iLastInteractionID) {
			_iLastInteractionID = userDataIn.m_user_interaction[i].m_iInteractionId;
		}
		if (userDataIn.m_user_interaction[i].m_iScriptId > _iLastScriptID) {
			_iLastScriptID = userDataIn.m_user_interaction[i].m_iScriptId;
		}
        if (userDataIn.m_user_interaction[i].m_iSessionId > _iLastSessionID) {
            _iLastSessionID = userDataIn.m_user_interaction[i].m_iSessionId;
        }
		
        _sLastSessionPartner = userDataIn.m_user_interaction[i].m_sSessionPartner;
	}
	cout << "<<<userModelDriver>>> end of interaction history" << endl;

	if (_bTest) {
		simulateInteractionForTest();
        Sleep(900);
        simulateUpdateForTest();
	}
}

void UserModelDriverController::simulateInteractionForTest() {
	cout << "<<<userModelDriver>>> test simulating an interaction (3 seconds)" << endl;
	Sleep(3000);
	
	user_interaction_data_t simulatedInteraction;
	simulatedInteraction.m_iInteractionId = _iLastInteractionID + 1;
	simulatedInteraction.m_iScriptId = _iLastScriptID + 1;
	simulatedInteraction.m_sPerformance = "1,0,1,0";
    simulatedInteraction.m_sEngagement = "0,1,0,1";
    simulatedInteraction.m_sTurnTake = "0,1,1,0";
    simulatedInteraction.m_sWP5Perf = "0,1,1,0";
    simulatedInteraction.m_iSessionId = _iLastSessionID + 1;
    simulatedInteraction.m_sSessionName = "Test Interaction";
	simulatedInteraction.m_sScriptTime = getTimeStamp();
	simulatedInteraction.m_sScriptEnd = getTimeStamp();
	simulatedInteraction.m_sWP5Turn = "0,1,0,1";
	
    if (_sLastSessionPartner == "Robot") {
        simulatedInteraction.m_sSessionPartner = "Therapist";
    }
    else {
        simulatedInteraction.m_sSessionPartner = "Robot";
    }
   
	cout << "<<<userModelDriver>>> interaction simulation done, returning data to userModel" << endl;
	_yarpInterface->sendToUpdatedData(simulatedInteraction);
	Sleep(1000);
    cout << "<<<userModelDriver>>> end of sim 1" << endl;
}

string UserModelDriverController::getTimeStamp() {
    time_t t = time(0);						        // get time now
	struct tm * now = localtime(&t);

    char buffer[80];
    strftime(buffer, 80, "%Y%m%d_%H%M%S", now);   // be careful as this version of C++ doesn't support all standard formats!
    string str(buffer);
    return str;
}

void UserModelDriverController::simulateUpdateForTest() {
    cout << "<<<userModelDriver>>> test simulating an interaction (3 seconds)" << endl;

    user_interaction_data_t simulatedInteraction;
    simulatedInteraction.m_iInteractionId = _iLastInteractionID + 2;
    simulatedInteraction.m_iScriptId = _iLastScriptID + 2;
    simulatedInteraction.m_sPerformance = "1,0,1,1";
    simulatedInteraction.m_sEngagement = "1,1,0,1";
    simulatedInteraction.m_sTurnTake = "0,1,1,0";
    simulatedInteraction.m_sWP5Perf = "0,1,1,0";
    simulatedInteraction.m_iSessionId = _iLastSessionID + 2;
    simulatedInteraction.m_sSessionName = "Test Interaction";
    simulatedInteraction.m_sSessionPartner = "Robot";
	simulatedInteraction.m_sScriptTime = getTimeStamp();
	simulatedInteraction.m_sScriptEnd = getTimeStamp();
	simulatedInteraction.m_sWP5Turn = "1,0,1,0";
	
    _yarpInterface->sendToUpdatedData(simulatedInteraction);

    cout << "<<<userModelDriver>>> test complete - check user_1.dream file for changes" << endl;

}