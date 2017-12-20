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

#include "actuationDriverController.h"
#include "actuationDriverYarpInterface.h"
#include "waiter.h"
#include "commonFonctionalities.h"
#include "actionMessages.h"
#include "moveSequenceId.h"
#include "sandtrayMessages.h"

using namespace std;

ActuationDriverController::ActuationDriverController(ActuationDriverYarpInterface *yarpInterface){
	_common = new CommonFunctionalities();
	_yarpInterface = yarpInterface;
	_waiter = new Waiter();
	_touchExpectedGrab = 0;
	_touchExpectedDrop = 0;
}

void ActuationDriverController::testSelection(int stepId, int actionId, std::string argument) {
	_yarpInterface->sendToSelectedAction(stepId, actionId, argument);
}

void ActuationDriverController::testSelection(int stepId, int actionId, int argument) {
	_yarpInterface->sendToSelectedAction(stepId, actionId, argument);
}

void ActuationDriverController::testSelection(int stepId, int actionId) {
	_yarpInterface->sendToSelectedAction(stepId, actionId);	
}

void ActuationDriverController::goNextStep() {
	cout << "--------------------NEXT-ACTION-----------------------------" << endl;
	waitms(1000);
	_waiter->notify();
}

void ActuationDriverController::test() {
	cout << "-actuationDriver- Entering full test of all actions" << endl;
	cout << "-------------------------------------------------" << endl;

	cout << "-actuationDriver- Sending _ACTION_ENABLE_" << endl;
	testSelection(0, _ACTION_ENABLE_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_DISABLE_" << endl;
	testSelection(1, _ACTION_DISABLE_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_ENABLE_" << endl;
	testSelection(2, _ACTION_ENABLE_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_OPEN_HAND_" << endl;
	testSelection(3, _ACTION_OPEN_HAND_, _RIGHT_ARM_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_CLOSE_HAND_" << endl;
	testSelection(4, _ACTION_CLOSE_HAND_, _RIGHT_ARM_);
	_waiter->startWaiting(); 

	cout << "-actuationDriver- Sending _ACTION_SAY_ with param -test-" << endl;
	testSelection(5, _ACTION_SAY_, "test");
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_MOVE_ with param -5-" << endl;
	testSelection(6, _ACTION_MOVE_, 5);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _MOVE_STAND_" << endl;
	testSelection(7, _ACTION_MOVE_, _MOVE_STAND_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_GRAB_OBJECT_" << endl;
	_touchExpectedGrab = 1;
	testSelection(8, _ACTION_GRAB_OBJECT_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_DROP_OBJECT_" << endl;
	testSelection(9, _ACTION_DROP_OBJECT_);
	_touchExpectedDrop = 1;
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_" << endl;
	testSelection(10, _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_);
	_touchExpectedDrop = 1;
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_" << endl;
	testSelection(11, _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_" << endl;
	testSelection(12, _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_IMITATION_MOVE_BAD_" << endl;
	testSelection(13, _ACTION_FEEDBACK_IMITATION_MOVE_BAD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_TT_WAITING_GOOD_" << endl;
	testSelection(14, _ACTION_FEEDBACK_TT_WAITING_GOOD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_TT_WAITING_BAD_" << endl;
	testSelection(15, _ACTION_FEEDBACK_TT_WAITING_BAD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_TT_SORTING_GOOD_" << endl;
	testSelection(16, _ACTION_FEEDBACK_TT_SORTING_GOOD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_TT_SORTING_BAD_" << endl;
	testSelection(17, _ACTION_FEEDBACK_TT_SORTING_BAD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_TT_CHOOSING_GOOD_" << endl;
	testSelection(18, _ACTION_FEEDBACK_TT_CHOOSING_GOOD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_TT_CHOOSING_BAD_ (no action is expected...)" << endl;
	testSelection(19, _ACTION_FEEDBACK_TT_CHOOSING_BAD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_TT_PATTERN_GOOD_" << endl;
	testSelection(20, _ACTION_FEEDBACK_TT_PATTERN_GOOD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_TT_PATTERN_BAD_" << endl;
	testSelection(21, _ACTION_FEEDBACK_TT_PATTERN_BAD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_JA_GOOD_" << endl;
	testSelection(22, _ACTION_FEEDBACK_JA_GOOD_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_FEEDBACK_JA_BAD_" << endl;
	testSelection(23, _ACTION_FEEDBACK_JA_BAD_);
	_waiter->startWaiting();
	
	cout << "-actuationDriver- Sending _ACTION_ROBOT_GOOD_MOVE_" << endl;
	testSelection(24, _ACTION_ROBOT_GOOD_MOVE_);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_POINT_IMAGE_" << endl;
	testSelection(25, _ACTION_POINT_IMAGE_, 1);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_LOOK_POINT_SAY_" << endl;
	testSelection(26, _ACTION_LOOK_POINT_SAY_, 2);
	_waiter->startWaiting();

	cout << "-actuationDriver- Sending _ACTION_LOOK_POINT_" << endl;
	testSelection(27, _ACTION_LOOK_POINT_, 1);
	_waiter->startWaiting();


	cout << "-actuationDriver- Sending _ACTION_LOOK_" << endl;
	testSelection(29, _ACTION_LOOK_, 1);
	_waiter->startWaiting();

	cout << "Action without reaction: _ACTION_RECEIVED_PERF_ - _ACTION_CORRUPTED_ - _ACTION_FINISHED_"<<endl;

	cout << "--------------PORTS-USED-BY-ATTENTION-REACTION---------------" << endl;

	cout << "-actuationDriver- Test eye blinking" << endl;
	_yarpInterface->sendToEyeBlinking("Single", "Full", "2");
	//_waiter->startWaiting();
    _common->waitms(3000);

	cout << "-actuationDriver- falling reaction" << endl;
	_yarpInterface->sendToFallingReaction(0);
	_waiter->startWaiting();

	cout << "-actuationDriver- falling reaction speech" << endl;
	_yarpInterface->sendToFallingReactionSpeech(0);
	//_waiter->startWaiting();
    _common->waitms(3000);

	cout << "-actuationDriver- social facial expression 4" << endl;
	_yarpInterface->sendToSocialFacialExpression(4);
	_waiter->startWaiting();

	cout << "-actuationDriver- social facial expression 0" << endl;
	_yarpInterface->sendToSocialFacialExpression(0);
	_waiter->startWaiting();

	cout << "-actuationDriver- social reaction 0" << endl;
	_yarpInterface->sendToSocialReaction(0);
	_waiter->startWaiting();

	//cout << "-actuationDriver- social reaction 3" << endl;
	//_yarpInterface->sendToSocialReaction(3);
	//_waiter->startWaiting();

	cout << "-actuationDriver- social reaction speech positive: 1" << endl;
	_yarpInterface->sendToSocialReactionSpeech(1);
	//_waiter->startWaiting();
    _common->waitms(3000);


	cout << "-actuationDriver- social reaction speech negative: -1" << endl;
	_yarpInterface->sendToSocialReactionSpeech(-1);
	//_waiter->startWaiting();
    _common->waitms(3000);

	cout << "-actuationDriver- falling interruption (expect no answer)" << endl;
	_yarpInterface->sendToFallingInterruption(1);
	_common->waitms(3000);

	cout << "-actuationDriver- elicited attention" << endl;
	_yarpInterface->sendToElicitedAttention(0.2, 0.5, 0.8);
	_waiter->startWaiting();

	cout << "-actuationDriver- social reaction -1" << endl;
	_yarpInterface->sendToSocialReaction(-1);
	_waiter->startWaiting();

	cout << "----------------------------------------------------" << endl;
	cout << "Test finished" << endl;
	cout << "----------------------------------------------------" << endl;
}

void ActuationDriverController::waitms(int millisec) {
	_common->waitms(millisec);
}

void ActuationDriverController::simulateExecution(int step, int action) {
	waitms(100);
	cout << "-actuationDriver- Simulating action being executed" << endl;
	waitms(2000);
	cout << "-actuationDriver- Sending success" << endl;
	_yarpInterface->sendToRobotMotorFeedback(step, action);
}

void ActuationDriverController::getFromSandtrayCommand(string message) {
	vector<string> commands = _common->split(message, ',');
	if (commands[0] == _SANDTRAY_MAKE_GOOD_MOVE_) {
		cout << "Expect images coodinates for _ROBOT_GOOD_MOVE_" << endl;
		_yarpInterface->sendToSandtrayReturn("movedata,0,971,696,400,971,696,630.613,813.205,457.414,848.58,272,540,20,2.022,apple,fruits");
	}
	if(commands[0] == _SANDTRAY_GET_IMAGE_COORDINATES_) {
		cout << "Expect images coodinates for _ACTION_POINT_IMAGE_" << endl;
		_yarpInterface->sendToSandtrayReturn("coordinates,-640,0");
	}
}
