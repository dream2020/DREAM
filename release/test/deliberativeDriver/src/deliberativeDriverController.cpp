/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Emmanuel Senft, PLYM 
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
  * 14/09/2016 Version 1.0: ... (Emmanuel Senft)
  */

#include "deliberativeDriverController.h"
#include "deliberativeDriverYarpInterface.h"
#include "scriptMessages.h"
#include "waiter.h"
#include "actionMessages.h"
#include "sandtrayMessages.h"

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <stdlib.h>
#include "moveSequenceId.h"
#include <actionOrigin.h>

const int DEBUG = 1;

using namespace std;
using namespace boost::asio;
using namespace std;

DeliberativeDriverController::DeliberativeDriverController(DeliberativeDriverYarpInterface *yarpInterface){
	_yarpInterface = yarpInterface;
	_waiter = new Waiter();
	_io = new io_service();
	_t = new deadline_timer(*_io);
	_Itime = 500;
	wait();
	_thr = new boost::thread(boost::bind(&io_service::run, _io));
	_sendPerf = 1;
	_waitingInteractionEvent = 0;
}

void DeliberativeDriverController::emulateMutualGaze(const boost::system::error_code &e) {
	if(!_sendingMutualGaze)
		return;
	if (e)
		return;
	sendMutualGaze(rand() % 100 <5);
	wait();
}


void DeliberativeDriverController::wait() {
	_t->expires_from_now(boost::posix_time::milliseconds(_Itime)); //repeat rate here
	_t->async_wait(boost::bind(&DeliberativeDriverController::emulateMutualGaze, this, boost::asio::placeholders::error));
}

void DeliberativeDriverController::stop() {
	delete _t;
}

void DeliberativeDriverController::testCommand(int stepId, int actionId) {
	_yarpInterface->sendToInterventionCommand(stepId, actionId);
}

void DeliberativeDriverController::testCommand(int stepId, int actionId, string arg) {
	_yarpInterface->sendToInterventionCommand(stepId, actionId, arg);
}

void DeliberativeDriverController::testCommand(int stepId, int actionId, int arg) {
	_yarpInterface->sendToInterventionCommand(stepId, actionId, arg);
}

void DeliberativeDriverController::testCommand(int stepId, int actionId, int arg1, int arg2, int arg3) {
	_yarpInterface->sendToInterventionCommand(stepId, actionId, arg1, arg2, arg3);
}

void DeliberativeDriverController::sendToActionFeedback(int step, int actionId, bool success) {
	_yarpInterface->sendToActionFeedback(step, actionId, success);

}

void DeliberativeDriverController::testSelection(int stepId, int actionId) {
	_yarpInterface->sendToSelectedAction(stepId, actionId);
}

void DeliberativeDriverController::testSelection(int stepId, int actionId, int arg) {
	_yarpInterface->sendToSelectedAction(stepId, actionId, arg);
}

void DeliberativeDriverController::testSelection(int stepId, int actionId, double arg) {
	_yarpInterface->sendToSelectedAction(stepId, actionId, arg);
}

void DeliberativeDriverController::testSelection(int stepId, int actionId, string arg) {
	_yarpInterface->sendToSelectedAction(stepId, actionId, arg);
}


void DeliberativeDriverController::sendToPerf(float perf) {
	_yarpInterface->sendToGetChildPerformance(perf);
}

void DeliberativeDriverController::sendToSantrayEvent(std::string str) {
	_yarpInterface->sendToSandtrayEvent(str);
}

void DeliberativeDriverController::sendToSantrayReturn(std::string str) {
	_yarpInterface->sendToSandtrayReturn(str);
}

void DeliberativeDriverController::simulateExecution(int stepId, int actionId) {
	if (DEBUG) {
		Sleep(100);
		cout << "-deliberativeDriver- Simulate Execution step " << stepId << " action " << actionId << endl;
	}
	Sleep(2000);
	_yarpInterface->sendToActionFeedback(stepId, actionId, 1);
}

void DeliberativeDriverController::sendMutualGaze(bool gaze) {
	_yarpInterface->sendToCheckMutualGaze(gaze);
}

void DeliberativeDriverController::enableAutoGaze(bool enable) {
	if (_sendingMutualGaze == enable)
		return;

	_sendingMutualGaze = enable;
	if (enable)
		wait();
}

void DeliberativeDriverController::receiveSuccess() {
	Sleep(200);
	cout << "--------------------NEXT-STEP------------------------" << endl;
	Sleep(1000);
	_waiter->notify();
}

void DeliberativeDriverController::testApp() {
	cout << "-deliberativeDriver- Starting test" << endl;
	cout << "-deliberativeDriver- Disabling automatic gaze and perf response for the test" << endl;
	enableAutoGaze(0);
	setSendPerf(0);

	cout << endl << "----------------------------------------------------" << endl;
	cout << "-deliberativeDriver- Testing Script steps, simulating script" << endl;
	cout << "----------------------------------------------------" << endl << endl;

	cout << "--------------------FIRST-STEP------------------------" << endl;
	cout << "-deliberativeDriver- Sending script type to start" << endl;
	_yarpInterface->sendToInterventionCommand(0, _SCRIPT_TYPE_, 8);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Enabling robot" << endl;
	_yarpInterface->sendToInterventionCommand(0, _SCRIPT_ENABLE_ROBOT_);
	_waiter->startWaiting();
	
	cout << "-deliberativeDriver- Waiting for gaze (gaze sent after 15 seconds)" << endl;
	_yarpInterface->sendToInterventionCommand(1,_SCRIPT_CHECK_GAZE_);
	Sleep(15000);
	cout << "-deliberativeDriver- Sending gaze" << endl;
	sendMutualGaze(1);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Script say" << endl;
 	_yarpInterface->sendToInterventionCommand(2, _SCRIPT_SAY_, "My name is nao and I am happy to meet you");
	_waiter->startWaiting();	

	cout << "-deliberativeDriver- Script move " << endl;
	_yarpInterface->sendToInterventionCommand(3 ,_SCRIPT_MOVE_, "0");	
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Script wait 2 seconds" << endl;
	_yarpInterface->sendToInterventionCommand(4, _SCRIPT_WAIT_, 2000);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Script grab" << endl;
	_yarpInterface->sendToInterventionCommand(5, _SCRIPT_GRAB_OBJECT_);
	_waiter->startWaiting();

	_yarpInterface->sendToInterventionCommand(6,_SCRIPT_DROP_OBJECT_);

	cout << "-deliberativeDriver- Feedback with positive performance" << endl;
	cout << "-deliberativeDriver- Feedback imitation object" << endl;
	_yarpInterface->sendToInterventionCommand(7, _SCRIPT_FEEDBACK_IMITATION_OBJECT_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Script wait for performance (sent after 5 seconds)" << endl;
	_yarpInterface->sendToInterventionCommand(8, _SCRIPT_WAIT_FOR_PERF_, 1, 1, 2000);
	Sleep(5000);
	_yarpInterface->sendToGetChildPerformance(1.);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Last feedback imitation object" << endl;
	_yarpInterface->sendToInterventionCommand(9, _SCRIPT_LAST_FEEDBACK_IMITATION_OBJECT_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback imitation move" << endl;
	_yarpInterface->sendToInterventionCommand(10, _SCRIPT_FEEDBACK_IMITATION_MOVE_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Last feedback imitation move" << endl;
	_yarpInterface->sendToInterventionCommand(11, _SCRIPT_LAST_FEEDBACK_IMITATION_MOVE_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback TT waiting" << endl;
	_yarpInterface->sendToInterventionCommand(12, _SCRIPT_FEEDBACK_TT_WAITING_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback TT sorting" << endl;
	_yarpInterface->sendToInterventionCommand(13, _SCRIPT_FEEDBACK_TT_SORTING_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback TT choosing" << endl;
	_yarpInterface->sendToInterventionCommand(14, _SCRIPT_FEEDBACK_TT_CHOOSING_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback TT pattern" << endl;
	_yarpInterface->sendToInterventionCommand(15, _SCRIPT_FEEDBACK_TT_PATTERN_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback JA" << endl;
	_yarpInterface->sendToInterventionCommand(16, _SCRIPT_FEEDBACK_JA_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Last feedback JA" << endl;
	_yarpInterface->sendToInterventionCommand(17, _SCRIPT_LAST_FEEDBACK_JA_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Last feedback imitation object" << endl;

	cout << endl;
	cout << "-deliberativeDriver- Feedback with negative performance" << endl;
	cout << "-deliberativeDriver- Set negative performance" << endl;
	_yarpInterface->sendToSelectedAction(18, _ACTION_RECEIVED_PERF_, 0.);
	Sleep(1000);
	cout << "-deliberativeDriver- Last feedback imitation object" << endl;
	_yarpInterface->sendToInterventionCommand(19, _SCRIPT_LAST_FEEDBACK_IMITATION_OBJECT_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback imitation move" << endl;
	_yarpInterface->sendToInterventionCommand(20, _SCRIPT_FEEDBACK_IMITATION_MOVE_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Last feedback imitation move" << endl;
	_yarpInterface->sendToInterventionCommand(21, _SCRIPT_LAST_FEEDBACK_IMITATION_MOVE_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback TT waiting" << endl;
	_yarpInterface->sendToInterventionCommand(22, _SCRIPT_FEEDBACK_TT_WAITING_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback TT sorting" << endl;
	_yarpInterface->sendToInterventionCommand(23, _SCRIPT_FEEDBACK_TT_SORTING_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback TT choosing" << endl;
	_yarpInterface->sendToInterventionCommand(24, _SCRIPT_FEEDBACK_TT_CHOOSING_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback TT pattern" << endl;
	_yarpInterface->sendToInterventionCommand(25, _SCRIPT_FEEDBACK_TT_PATTERN_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Feedback JA" << endl;
	_yarpInterface->sendToInterventionCommand(26, _SCRIPT_FEEDBACK_JA_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Last feedback JA" << endl;
	_yarpInterface->sendToInterventionCommand(26, _SCRIPT_LAST_FEEDBACK_JA_);
	_waiter->startWaiting();

	cout << endl << "----------------------------------------------------" << endl;
	cout << "-deliberativeDriver- Testing Sandtray Functionalities" << endl;
	cout << "----------------------------------------------------" << endl << endl;
	cout << "-deliberativeDriver- Display white" << endl; 
	_yarpInterface->sendToInterventionCommand(27,_SCRIPT_SANDTRAY_DISPLAY_WHITE_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Set images one by one" << endl;
	_yarpInterface->sendToInterventionCommand(28, _SCRIPT_SANDTRAY_SET_IMAGE_ONE_);
	_waiter->startWaiting(); 

	cout << "-deliberativeDriver- Load library (8)" << endl;
	_yarpInterface->sendToInterventionCommand(29, _SCRIPT_SANDTRAY_LOAD_LIBRARY_, 8);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Set images all" << endl;
	_yarpInterface->sendToInterventionCommand(30, _SCRIPT_SANDTRAY_SET_IMAGE_ALL_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Load library (8)" << endl;
	_yarpInterface->sendToInterventionCommand(31, _SCRIPT_SANDTRAY_LOAD_LIBRARY_, 8);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Enable feedback" << endl;
	_yarpInterface->sendToInterventionCommand(32, _SCRIPT_SANDTRAY_ENABLE_FEEDBACK_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Disable feedback" << endl;
	_yarpInterface->sendToInterventionCommand(33, _SCRIPT_SANDTRAY_DISABLE_FEEDBACK_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Enable touch" << endl;
	_yarpInterface->sendToInterventionCommand(34, _SCRIPT_SANDTRAY_ENABLE_TOUCH_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Disable touch" << endl;
	_yarpInterface->sendToInterventionCommand(35, _SCRIPT_SANDTRAY_DISABLE_TOUCH_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Robot good move" << endl;
	_yarpInterface->sendToInterventionCommand(36, _SCRIPT_ROBOT_GOOD_MOVE_);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Robot Stand to init position" << endl;
	_yarpInterface->sendToInterventionCommand(37, _SCRIPT_MOVE_, "111");
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Load library (6)" << endl;
	_yarpInterface->sendToInterventionCommand(38, _SCRIPT_SANDTRAY_LOAD_LIBRARY_, 6);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Robot point image" << endl;
	_yarpInterface->sendToInterventionCommand(39, _SCRIPT_POINT_IMAGE_, 1);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Robot Stand to init position" << endl;
	_yarpInterface->sendToInterventionCommand(40, _SCRIPT_MOVE_, "111");
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Robot Look point say" << endl;
	_yarpInterface->sendToInterventionCommand(41, _SCRIPT_LOOK_POINT_SAY_, 1);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Robot Stand to init position" << endl;
	_yarpInterface->sendToInterventionCommand(42, _SCRIPT_MOVE_, "111");
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Robot Look point" << endl;
	_yarpInterface->sendToInterventionCommand(43, _SCRIPT_LOOK_POINT_, 0);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Robot Stand to init position" << endl;
	_yarpInterface->sendToInterventionCommand(44, _SCRIPT_MOVE_, "111");
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Robot Look" << endl;
	_yarpInterface->sendToInterventionCommand(45, _SCRIPT_LOOK_, 1);
	_waiter->startWaiting();

	cout << "-deliberativeDriver- Display white" << endl;
	_yarpInterface->sendToInterventionCommand(46, _SCRIPT_SANDTRAY_DISPLAY_WHITE_);
	_waiter->startWaiting();

	cout << endl << "----------------------------------------------------" << endl;
	cout << "-deliberativeDriver- Test functionalities offered to the therapist (sending perf and gaze already demonstrated)" << endl;
	cout << "----------------------------------------------------" << endl << endl;

	cout << "-deliberativeDriver- Go off script (delib will not send next action during that time)" << endl;
	_yarpInterface->sendToSelectedAction(_ORIGIN_THERAPIST_, _ACTION_GO_OFF_SCRIPT_);
	Sleep(2000);

	cout << "----------------------------------------------------" << endl;
	cout << "-deliberativeDriver- Test interpretation of sandtray events" << endl;
	cout << "----------------------------------------------------" << endl << endl;

	cout << "-deliberativeDriver- Example: Script move does not provoke a next step." << endl;
	_yarpInterface->sendToInterventionCommand(47, _SCRIPT_MOVE_, "0");
	Sleep(4000);
	cout << "-deliberativeDriver- Manually move to next step" << endl;
	Sleep(1000);
	cout << "----------------------------------------------------" << endl;
	Sleep(1000);
	//cout << "-deliberativeDriver- Go next step" << endl;   //THIS TEST IS DEPRECATED
	//_yarpInterface->sendToSelectedAction(_ORIGIN_THERAPIST_, _ACTION_GO_NEXT_STEP_);
	//_waiter->startWaiting();

	cout << "-deliberativeDriver- Return to script" << endl;
	_yarpInterface->sendToSelectedAction(_ORIGIN_THERAPIST_, _ACTION_BACK_ON_SCRIPT_);
	Sleep(4000);
	cout << "----------------------------------------------------" << endl;

	cout << "-deliberativeDriver- Simulate good move on the Sandtray" << endl;
	_yarpInterface->sendToSandtrayEvent("50, 6, 57.009, 256.484, apple, fruits");
	_waitingInteractionEvent = 1;
	_waiter->startWaiting();
	cout << "----------------------------------------------------" << endl;
	Sleep(1000);

	cout << "-deliberativeDriver- Simulate bad move on the Sandtray" << endl;
	_yarpInterface->sendToSandtrayEvent("51, 7, 1.172, 1025.29, cabbage, fruits");
	_waitingInteractionEvent = 1;
	_waiter->startWaiting();
	cout << "----------------------------------------------------" << endl;
	Sleep(1000);

	cout << "-deliberativeDriver- Simulate touches on the Sandtray" << endl;
	Sleep(1000);
	_yarpInterface->sendToSandtrayEvent("playertouch,0");
	_waitingInteractionEvent = 1;
	_waiter->startWaiting();
	cout << "----------------------------------------------------" << endl;
	Sleep(1000);

	cout << "-deliberativeDriver- Simulate release on the Sandtray" << endl;
	_yarpInterface->sendToSandtrayEvent("playerrelease,2");
	_waitingInteractionEvent = 1;
	_waiter->startWaiting();
	cout << "----------------------------------------------------" << endl;
	Sleep(1000);
	
	cout << "-deliberativeDriver- Robot Script finished" << endl;
	_yarpInterface->sendToInterventionCommand(45, _SCRIPT_FINISHED_);
	Sleep(8000);
	cout << "----------------------------------------------------" << endl;
	cout << "Test finished" << endl;
	cout << "----------------------------------------------------" << endl;
}
	

void DeliberativeDriverController::setSendPerf(bool value) {
	_sendPerf = value;
}

bool DeliberativeDriverController::getSendPerf() {
	return _sendPerf;
}

void DeliberativeDriverController::getFromInteractionEvent(int eventId, int eventParam) {
	if (_waitingInteractionEvent) {
		_waitingInteractionEvent = 0;
		_waiter->notify();
	}
}