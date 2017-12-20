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

#include "selfMonitoringDriverController.h"
#include "selfMonitoringDriverYarpInterface.h"

using namespace std;

SelfMonitoringDriverController::SelfMonitoringDriverController(SelfMonitoringDriverYarpInterface *yarpInterface){
	_yarpInterface = yarpInterface;
}

void SelfMonitoringDriverController::test() {

    // create session (this would be GUI, loading user and a script)
    _yarpInterface->sendToTherapistCommand("Create Session", 0, "Test Session", "Robot");
    cout << "<<<SMS Driver>>> create session sent to SMS" << endl;
    Sleep(100);

    // simulate receiving user data here (would have been GUI->userModel->SMS as part of create session)
    // this will forward the full user data on to the GUI
    _yarpInterface->sendToUserData();
    cout << "<<<SMS Driver>>> sent user data" << endl;
    Sleep(200);

    // load script - simulate GUI button click
	_yarpInterface->sendToTherapistCommand("scriptload", 8, "", "");
	cout << "<<<SMS Driver>>> send scriptload 8 to SMS" << endl;
    Sleep(2000);

    _yarpInterface->sendToInterventionCommand(1, _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_, "testeroo");
    cout << "<<<SMS Driver>>> intervention command sent" << endl;
    Sleep(2000);

    
    // simulate action from delib
    cout << "<<<SMS Driver>>> delib suggested action" << endl;
    _yarpInterface->sendToSuggestedAction(1,_ACTION_ENABLE_,"");
    Sleep(200);

    // simulate action from delib
    cout << "<<<SMS Driver>>> delib suggested action" << endl;
    _yarpInterface->sendToSuggestedAction(1, _ACTION_DROP_OBJECT_, "");
    Sleep(200);

    // simulate action return from actuation
    _yarpInterface->sendToActionFeedback(1, _ACTION_DROP_OBJECT_, 0, 0, -1);
    cout << "<<<SMS Driver>>> bad action feedback returned" << endl;
    Sleep(200);

    // simulate action from delib - this should trigger updated data to return
    cout << "<<<SMS Driver>>> delib suggested action" << endl;
    _yarpInterface->sendToSuggestedAction(1, _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_, "");
    Sleep(200);

}

void SelfMonitoringDriverController::continueTest() {
    // simulate action return from actuation
    _yarpInterface->sendToActionFeedback(1, _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_, 1, 0, -1);
    cout << "<<<SMS Driver>>> good action feedback returned" << endl;
    Sleep(200);

    // simulate engagement flag in
    _yarpInterface->sendToEngagementFlag(0);
    cout << "<<<SMS Driver>>> sent to engagement flag (0) for logging" << endl;
    Sleep(200);

	cout << "<<<SMS Driver>>> starting engagement flag spam" << endl;
	// now spam engagement flag to check file mutex
	for (int i=0; i < 100; i++) {
		_yarpInterface->sendToEngagementFlag(i);
		cout << "sent: " << i << endl;
		Sleep(30);
	}
	cout << "<<<SMS Driver>>> finished engagement flag spam" << endl;

    // simulate child performance in
    _yarpInterface->sendToGetChildPerformance(1);
    cout << "<<<SMS Driver>>> sent to child performance (1) for logging" << endl;
    Sleep(200);

    // simulate child behaviour in
    _yarpInterface->sendToGetChildBehaviour(5);
    cout << "<<<SMS Driver>>> sent to child behaviour (5) for logging" << endl;
    Sleep(200);

    // simulate robot fall
    cout << "<<<SMS Driver>>> simulating robot fall (only logged)" << endl;
    _yarpInterface->sendToFallingInterruption(0);
    Sleep(2000);

    // simulate getInterventionStatus
    cout << "<<<SMS Driver>>> simulating getInterventionStatus" << endl;
    _yarpInterface->sendToGetInterventionStatus();
    Sleep(200);

    // simulate action from delib
    cout << "<<<SMS Driver>>> delib suggested action" << endl;
    _yarpInterface->sendToSuggestedAction(1, _ACTION_FINISHED_, "");
    Sleep(2000);

    cout << "<<<SMS Driver>>> end of test - check /logs/ folder to ensure logging of SMS correct" << endl;
}