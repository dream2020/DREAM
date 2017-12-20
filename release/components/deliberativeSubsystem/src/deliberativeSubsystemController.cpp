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

#include "deliberativeSubsystemController.h"
#include "deliberativeSubsystemYarpInterface.h"
#include "scriptMessages.h"
#include "sandtrayMessages.h"
#include "actionMessages.h"
#include "sandtrayMessages.h"
#include "feedbackMessages.h"
#include "actionOrigin.h"
#include "waiter.h"
#include "commonFonctionalities.h"

const int DEBUG =  1;

using namespace std;
using namespace boost::asio;

PeriodicTimer::PeriodicTimer(int duration, int initDuration, DeliberativeSubsystemController* controller, boost::asio::io_service* io){
	_io = io;
	_t = new deadline_timer(*_io);
	_duration = duration;
	_initDuration = initDuration;
	_controller = controller;
}

PeriodicTimer::~PeriodicTimer(){
	_t->cancel();
	if(_thr)
		delete _thr;
	delete _t;
}

void PeriodicTimer::start() {
	_t->expires_from_now(boost::posix_time::milliseconds(_initDuration));
	_t->async_wait(boost::bind(&PeriodicTimer::callback, this, boost::asio::placeholders::error));
	_io->reset();
	_thr = new boost::thread(boost::bind(&io_service::run, _io));
}

void PeriodicTimer::stop() {
	_t->cancel();
}

void PeriodicTimer::callback(const boost::system::error_code &e) {
	if (e) {
		if (DEBUG)
			cout << "-deliberativeSubsystem- timer cancelled " << endl;// e.message() << endl;
		return;
	}
	_controller->askLook();
	_t->expires_from_now(boost::posix_time::milliseconds(_duration));
	_t->async_wait(boost::bind(&PeriodicTimer::callback, this, boost::asio::placeholders::error));
}

DeliberativeSubsystemController::DeliberativeSubsystemController(DeliberativeSubsystemYarpInterface *yarpInterface){
	_yarpInterface = yarpInterface;
	_common = new CommonFunctionalities();

	_fPerf = -1;
	_waiterGaze = new Waiter();
	_waiterPerf = new Waiter();
	_waiterSand = new Waiter();
	_io = new io_service();
	_timer = new PeriodicTimer(9000, 3000, this, _io);

	_IscriptId = -1;
	
	_bMutualGaze = 0;
	_bAskLook = 0;
	_IwaitingAction = -1;
	_IwaitingStep = -1;
	_Bwaiting4action = 0;
	_IscriptType = -1;
	_BonScript = 0;
	_IstepId = 0;
	_BscriptFinished = 0;
	_BobjectGrabbed = 0;
	_bWaitingForPerf = 0;
	_Bdiagnose = 0;
	_Btherapist = 0;
	_bSandtrayConnected = 0;
	_bSandtrayCategory = 0;
	_BtherapistMove = 0;
	_BtherapistTouch = 0;
	_bLastFeedbackGood = 0;
}

void DeliberativeSubsystemController::getFromInterventionCommand(int stepId, int commandId, std::string arg){
	Sleep(50);
	_IstepId = stepId;
	//_yarpInterface->sendToSuggestedAction(stepId, _ACTION_CURRENT_STEP_, stepId);
	if(DEBUG)
		cout << "-deliberativeSubsystem- Received script command " << commandId << " with arg " << arg <<  endl;

	switch (commandId)
	{
	case(_SCRIPT_TYPE_):
		_BonScript = 1;
		_BscriptFinished = 0;
		_IscriptType = atoi(arg.c_str());
		//Send next step without sending it to the GUI to prevent confusion
		if (DEBUG) {
			waitms(20);
			cout << "-deliberativeSubsystem- Sending next step" << endl;
			waitms(20);
		}
		goNextStep();
		break;
	case(_SCRIPT_ID_):
		_IscriptId = atoi(arg.c_str());
		break;
	case(_SCRIPT_CHECK_GAZE_):
		checkGaze();
		break;
	case(_SCRIPT_SAY_):
		_yarpInterface->sendToSuggestedAction(stepId,_ACTION_SAY_, arg);
		break;
	case(_SCRIPT_SAY_IF_GOOD_PERF_):
		if (_bLastFeedbackGood)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_SAY_, arg);
		else
			goNextStep();
		break;
	case(_SCRIPT_MOVE_):
		_yarpInterface->sendToSuggestedAction(stepId,_ACTION_MOVE_, arg);
		break;
	case(_SCRIPT_WAIT_):
		waitms(atoi(arg.c_str()));
		goNextStep();
		break;
	case(_SCRIPT_WAIT_FOR_PERF_):
		waitForPerf(stepId,arg);
		break;
	case(_SCRIPT_GRAB_OBJECT_):
		if (!_BobjectGrabbed)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_GRAB_OBJECT_, arg);
		else
			goNextStep();
		break;
	case(_SCRIPT_DROP_OBJECT_):
		if (_BobjectGrabbed)
			_yarpInterface->sendToSuggestedAction(stepId,_ACTION_DROP_OBJECT_, arg);
		else
			goNextStep();
		break;
	case(_SCRIPT_FEEDBACK_IMITATION_OBJECT_):
		if(_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId,_ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId,_ACTION_FEEDBACK_IMITATION_OBJECT_BAD_, !_Bdiagnose);
		break;
	case(_SCRIPT_LAST_FEEDBACK_IMITATION_OBJECT_):
		if (_fPerf > 0.5)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_BAD_NO_ACTION_, !_Bdiagnose);
		break;
	case(_SCRIPT_FEEDBACK_IMITATION_MOVE_):
		cout<<"PERFORMANCE USED: "<<_fPerf<<endl;
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId,_ACTION_FEEDBACK_IMITATION_MOVE_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId,_ACTION_FEEDBACK_IMITATION_MOVE_BAD_, !_Bdiagnose);
		break;
	case(_SCRIPT_LAST_FEEDBACK_IMITATION_MOVE_):
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId,_ACTION_FEEDBACK_IMITATION_MOVE_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_BAD_NO_ACTION_, !_Bdiagnose);
		break;
	case(_SCRIPT_FEEDBACK_TT_WAITING_):
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_TT_WAITING_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_TT_WAITING_BAD_, !_Bdiagnose);
		break;
	case(_SCRIPT_LAST_FEEDBACK_TT_WAITING_):
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_TT_WAITING_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_LAST_TT_WAITING_BAD_, !_Bdiagnose);
		break;
	case(_SCRIPT_FEEDBACK_TT_SORTING_):
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_TT_SORTING_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_TT_SORTING_BAD_, !_Bdiagnose);
		break;
	case(_SCRIPT_LAST_FEEDBACK_TT_SORTING_):
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_TT_SORTING_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_BAD_NO_ACTION_, !_Bdiagnose);
		break;
	case(_SCRIPT_FEEDBACK_TT_CHOOSING_):
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_TT_CHOOSING_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_BAD_NO_ACTION_, !_Bdiagnose);
		break;
	case(_SCRIPT_LAST_FEEDBACK_TT_CHOOSING_):
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_TT_CHOOSING_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_BAD_NO_ACTION_, !_Bdiagnose);
		break;
	case(_SCRIPT_FEEDBACK_TT_PATTERN_):
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_TT_PATTERN_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_TT_PATTERN_BAD_, !_Bdiagnose);
		break;
	case(_SCRIPT_LAST_FEEDBACK_TT_PATTERN_):
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_TT_PATTERN_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_BAD_NO_ACTION_, !_Bdiagnose);
		break;
	case(_SCRIPT_FEEDBACK_JA_):
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_JA_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_JA_BAD_, !_Bdiagnose);
		break;
	case(_SCRIPT_LAST_FEEDBACK_JA_):
		if (_fPerf>0.5)
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_JA_GOOD_, !_Bdiagnose);
		else
			_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FEEDBACK_BAD_NO_ACTION_, !_Bdiagnose);
		break;
	case(_SCRIPT_SANDTRAY_DISPLAY_WHITE_):
		sandtrayDisplayWhite();
		if (_waiterSand->startWaiting())
			return;
		goNextStep();
		break;
	case(_SCRIPT_SANDTRAY_NEW_GAME_):
		sandtrayNewGame();
		if (_waiterSand->startWaiting())
			return;
		goNextStep();
		break;
	case(_SCRIPT_SANDTRAY_LOAD_LIBRARY_):
		sandtrayLoadLibrary(arg);
		if (_waiterSand->startWaiting())
			return;
		goNextStep();
		break;
	case(_SCRIPT_SANDTRAY_SET_IMAGE_ONE_):
		sandtraySetImageOne();
		if (_waiterSand->startWaiting())
			return;
		goNextStep();
		break;
	case(_SCRIPT_SANDTRAY_SET_IMAGE_ALL_):
		sandtraySetImageAll();
		if (_waiterSand->startWaiting())
			return;
		goNextStep();
		break;
	case(_SCRIPT_SANDTRAY_ENABLE_FEEDBACK_):
		sandtrayEnableFeedback(!_Bdiagnose);
		if (_waiterSand->startWaiting())
			return;
		goNextStep();
		break;
	case(_SCRIPT_SANDTRAY_DISABLE_FEEDBACK_):
		sandtrayEnableFeedback(0);
		if (_waiterSand->startWaiting())
			return;
		goNextStep();
		break;
	case(_SCRIPT_SANDTRAY_ENABLE_TOUCH_):
		sandtrayEnableTouch(1);
		if (_waiterSand->startWaiting())
			return;
		goNextStep();
		break;
	case(_SCRIPT_SANDTRAY_DISABLE_TOUCH_):
		sandtrayEnableTouch(0);
		if (_waiterSand->startWaiting())
			return;
		goNextStep();
		break;
	case(_SCRIPT_ROBOT_GOOD_MOVE_):
		_yarpInterface->sendToSuggestedAction(stepId,_ACTION_ROBOT_GOOD_MOVE_, arg);
		if(_Btherapist) {
			sandtrayEnableTouch(1);
			_BtherapistMove = 1;
		}
		break;
	case(_SCRIPT_POINT_IMAGE_):
		_yarpInterface->sendToSuggestedAction(stepId,_ACTION_POINT_IMAGE_, arg);
		if (_Btherapist) {
			sandtrayEnableTouch(1);
			_BtherapistTouch = 1;
		}
		break;
	case(_SCRIPT_LOOK_POINT_SAY_):
		_yarpInterface->sendToSuggestedAction(stepId,_ACTION_LOOK_POINT_SAY_, arg);
		break;
	case(_SCRIPT_LOOK_POINT_):
		_yarpInterface->sendToSuggestedAction(stepId,_ACTION_LOOK_POINT_, arg);
		break;
	case(_SCRIPT_LOOK_):
		_yarpInterface->sendToSuggestedAction(stepId, _ACTION_LOOK_, arg);
		break;
	case(_SCRIPT_ENABLE_ROBOT_):
		_yarpInterface->sendToSuggestedAction(stepId, _ACTION_ENABLE_, arg);
		break;
	case(_SCRIPT_DISABLE_ROBOT_):
		_yarpInterface->sendToSuggestedAction(stepId, _SCRIPT_DISABLE_ROBOT_, arg);
		break;
	case(_SCRIPT_STAND_):
		_yarpInterface->sendToSuggestedAction(stepId, _ACTION_STAND_, arg);
		break;
	case(_SCRIPT_CORRUPTED_):
		scriptCorrupted(stepId);
		break;
	case(_SCRIPT_FINISHED_):
		_BscriptFinished = 1;
		_yarpInterface->sendToSuggestedAction(stepId, _ACTION_FINISHED_, arg);
		break;
	default:
		scriptCorrupted(stepId);
		break;
	}
}

void DeliberativeSubsystemController::getFromSelectedAction(int stepId, int actionId, std::string arg) {
	if (DEBUG) {
		waitms(50);
		cout << "-deliberativeSubsystem- Received selected action step " << stepId << " action " << actionId << " with arg " << arg << endl;
		waitms(50);
	}

	vector<string> list;
	switch (actionId)
	{
	case(_ACTION_GO_NEXT_STEP_):
		//if (stepId != _ORIGIN_DELIBERATIVE_) {
		//	resetWaitingStates();
		//}
		break;
	case(_ACTION_RECEIVED_PERF_):
		_fPerf = atof(arg.c_str());
		if(DEBUG)
			cout << "-deliberativeSubsystem- Received action performance " << _fPerf << endl;
		getFromChildPerformance(_fPerf);
		break;
	case(_ACTION_RECEIVED_GAZE_):
		_bMutualGaze = atoi(arg.c_str());
		if (_bMutualGaze) {
			if (DEBUG)
				;// cout << "-deliberativeSubsystem- Received look" << endl;
			_waiterGaze->notify();
		}
		break;

	case(_ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_):
	case(_ACTION_FEEDBACK_IMITATION_MOVE_GOOD_):
	case(_ACTION_FEEDBACK_TT_WAITING_GOOD_):
	case(_ACTION_FEEDBACK_TT_SORTING_GOOD_):
	case(_ACTION_FEEDBACK_TT_CHOOSING_GOOD_):
	case(_ACTION_FEEDBACK_TT_PATTERN_GOOD_):
	case(_ACTION_FEEDBACK_JA_GOOD_):
		_bLastFeedbackGood = 1;
		if (_Btherapist)
			return;
		if (stepId >= 0 && !_BscriptFinished) {
			_IwaitingAction = actionId;
			_IwaitingStep = stepId;
			_Bwaiting4action = 1;
			waitms(20);
			if (DEBUG)
				cout << "-deliberativeSubsystem- waiting for action " << actionId << endl;
		}
		break;
	case(_ACTION_FEEDBACK_IMITATION_OBJECT_BAD_):
	case(_ACTION_FEEDBACK_IMITATION_MOVE_BAD_):
	case(_ACTION_FEEDBACK_TT_WAITING_BAD_):
	case(_ACTION_FEEDBACK_LAST_TT_WAITING_BAD_):
	case(_ACTION_FEEDBACK_TT_SORTING_BAD_):
	case(_ACTION_FEEDBACK_TT_CHOOSING_BAD_):
	case(_ACTION_FEEDBACK_TT_PATTERN_BAD_):
	case(_ACTION_FEEDBACK_JA_BAD_):
		// In the case of the diagnose, encouragin feedback is never delivered, so the action have to be executed
		_bLastFeedbackGood = _Bdiagnose;
		if (_Btherapist)
			return;
		if (stepId >= 0 && !_BscriptFinished) {
			_IwaitingAction = actionId;
			_IwaitingStep = stepId;
			_Bwaiting4action = 1;
			waitms(20);
			if (DEBUG)
				cout << "-deliberativeSubsystem- waiting for action " << actionId << endl;
		}
		break;
	case(_ACTION_ROBOT_GOOD_MOVE_):
	case(_ACTION_POINT_IMAGE_):
	case(_ACTION_LOOK_POINT_SAY_):
	case(_ACTION_LOOK_POINT_):
	case(_ACTION_LOOK_):
	case(_ACTION_STAND_):
	case(_ACTION_SAY_):
	case(_ACTION_ENABLE_):
	case(_ACTION_MOVE_):
	case(_ACTION_GRAB_OBJECT_):
	case(_ACTION_DROP_OBJECT_):
		if (_Btherapist)
			return;
		if (stepId >= 0 && !_BscriptFinished) {
			_IwaitingAction = actionId;
			_IwaitingStep = stepId;
			_Bwaiting4action = 1;
			waitms(20);
			if (DEBUG)
				cout << "-deliberativeSubsystem- waiting for action " << actionId << endl;
		}
		break;
	case(_ACTION_FEEDBACK_BAD_NO_ACTION_):
		if (_Btherapist)
			return;
		if (stepId >= 0 && !_BscriptFinished)
			goNextStep(1);
		break;
	case(_ACTION_GO_OFF_SCRIPT_):
		offScript();
		break;		
	case(_ACTION_CHANGE_STEP_):
		resetWaitingStates();
		break;
	case(_ACTION_BACK_ON_SCRIPT_):
		onScript();
		break;
	case(_ACTION_CORRUPTED_):
		scriptCorrupted(stepId);
		break;
	case(_ACTION_FINISHED_):
		break;
	case(_ACTION_SANDTRAY_LOAD_LIBRARY_):
		sandtrayLoadLibrary(arg);
		break;
	case(_ACTION_SANDTRAY_ENABLE_FEEDBACK_):
		sandtrayEnableFeedback(atoi(arg.c_str()));
		break;
	case(_ACTION_SANDTRAY_ENABLE_TOUCH_):
		sandtrayEnableTouch(atoi(arg.c_str()));
		break;
	case(_ACTION_SANDTRAY_ONE_BY_ONE_):
		if (arg == "1")
			sandtraySetImageOne();
		if (arg == "0")
			sandtraySetImageAll();
		break;	
	case(_ACTION_SCRIPT_TYPE_):
		list = _common->split(arg,',');
		_Bdiagnose = atoi(list[0].c_str());
		_Btherapist = !atoi(list[1].c_str());
		if (DEBUG) {
			cout << "-deliberativeSubsystem- Current type: " << (_Bdiagnose ? "Diagnose" : "Intervention") << endl;
			cout << "-deliberativeSubsystem- Current partner: " << (_Btherapist ? "Therapist" : "Robot") << endl;
		}
		testRobotConnection();
		break;
	default:
		break;
	}
}

void DeliberativeSubsystemController::sendToComandSuccess(bool success) {
	if (!_BonScript)
		return;
	cout << "-deliberativeSubsystem- Sending to command success " << success << endl;
	waitms(50);
	_yarpInterface->sendToCommandSuccess(success);
}

void DeliberativeSubsystemController::getFromCheckMutualGaze(bool mutualGaze) {
	//waitms(10);
	_bMutualGaze = mutualGaze;
	if (mutualGaze) {
		if (DEBUG)
			;// cout << "-deliberativeSubsystem- Received look" << endl;
		if (_waiterGaze->getIsWaiting()&&_Btherapist)
			_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_, _ACTION_RECEIVED_GAZE_,1);
		_waiterGaze->notify();
	}
}

void DeliberativeSubsystemController::goNextStep(bool forced) {
	if (DEBUG)
		cout << "-deliberativeSubsystem- sending next step - onscript "<<_BonScript << endl;
	if(_BonScript){
		_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_, _ACTION_GO_NEXT_STEP_, _IstepId);
	}
}

void DeliberativeSubsystemController::checkGaze() { 
	int currentStep = _IstepId;
	if(DEBUG)
		cout << "-deliberativeSubsystem- Waiting for gaze" << endl;
	_bAskLook = 1;
	if(!_Btherapist){
		_timer->start();
	}
	if (_waiterGaze->startWaiting())
		return;
	_bAskLook = 0;
	stopTimer();
	
	if(DEBUG)
		cout << "-deliberativeSubsystem- finish waiting" << endl;

	if(currentStep == _IstepId && _BonScript)
		goNextStep();
}

void DeliberativeSubsystemController::askLook() {
	if (!_bAskLook) {
		stopTimer();
		return;
	}
	//_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_,_ACTION_SAY_,"Look at me. Lets play together");
	//_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_,_ACTION_SAY_,"Uită-te la mine! Hai să ne jucăm împreună!");
	//_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_, _ACTION_SAY_, "Uităte la mine! Hai să ne jucăm împreună!");
	_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_, _ACTION_REQUEST_GAZE_,"");
}

void DeliberativeSubsystemController::stopTimer() {
	if (_timer)
		_timer->stop();
}

void DeliberativeSubsystemController::waitms(int millisec) {
	_common->waitms(millisec);
}

void DeliberativeSubsystemController::waitForPerf(int stepId, string arg) {
	cout << "-deliberativeSubsystem- Asking for perf" << endl;
	int currentStep = _IstepId;
	vector<string> arguments = _common->split(arg, ',');
	if (arguments.size() != 3 && arguments.size() != 4) {
		cout << "-deliberativeSubsystem- Script corrupted" << endl;
		return;
	}
	int expBehav = atoi(arguments[0].c_str());
	int param = atoi(arguments[1].c_str());
	int time = atoi(arguments[2].c_str());
	int waitingType = atoi(arguments[3].c_str());

	cout << "Waiting type " << waitingType << endl;

	if (waitingType == _WAITING_PERF_STANDARD_) { //Standard blocking wait
		_fPerf = -1;
		_yarpInterface->sendToGetInterventionStatus(_IscriptId, _IscriptType, stepId, expBehav, param, time, 1);
		if (_waiterPerf->startWaiting())
			return;
		Sleep(50);
		if (currentStep == _IstepId){
			goNextStep();
		}
	}
	else if (waitingType == _WAITING_PERF_REQUEST_) {  //Non-blocking request: make the request but doesn't wait
		_fPerf = -1;
		_yarpInterface->sendToGetInterventionStatus(_IscriptId, _IscriptType, stepId, expBehav, param, time, 1);
		_bWaitingForPerf = 1;
		goNextStep();
	}
	else if (waitingType == _WAITING_PERF_BLOCKING_) {  //Only block after request: wait if no perf has been received and does not send the request
		Sleep(500);

		cout << "BLOCKING WAIT " << _bWaitingForPerf << " " << _fPerf << endl;
		if (_bWaitingForPerf)
			if (_waiterPerf->startWaiting())
				return;

		if (currentStep == _IstepId) {
			goNextStep();
		}
	}
}


void DeliberativeSubsystemController::getFromChildPerformance(float perf, bool fromWP5) {
	cout << "-deliberativeSubsystem- Received performance from port: " << perf << endl;
	if (_waiterPerf->getIsWaiting() || _bWaitingForPerf) {
		_fPerf = perf;

		if (_waiterPerf->getIsWaiting())
			_waiterPerf->notify();

		if(_Btherapist && fromWP5)
			_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_, _ACTION_RECEIVED_PERF_, _fPerf);

		cout<<"Setting perf"<<_fPerf<<endl;
		_bWaitingForPerf = 0;
	}
}

void DeliberativeSubsystemController::sandtrayDisplayWhite() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SPECIFY_LEVEL_, "0");
}

void DeliberativeSubsystemController::sandtrayNewGame() {
	waitms(1000);
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_NEW_GAME_, "0");
	waitms(1000);
	//_yarpInterface->sendToSandtrayCommand(_SANDTRAY_RESET_BOARD_, "0");
}

void DeliberativeSubsystemController::sandtrayLoadLibrary(std::string libName) {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SPECIFY_LEVEL_, libName);
	}

void DeliberativeSubsystemController::sandtraySetImageOne() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SET_ONE_AT_TIME_, "true");
}

void DeliberativeSubsystemController::sandtraySetImageAll() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SET_ONE_AT_TIME_, "false");
}

void DeliberativeSubsystemController::sandtrayEnableFeedback(bool enable) {
	if (enable)
		_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SET_FEEDBACK_ON_, "0");
	else
		_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SET_FEEDBACK_OFF_, "0");
}

void DeliberativeSubsystemController::sandtrayEnableTouch(bool enable) {
	if(enable)
		_yarpInterface->sendToSandtrayCommand(_SANDTRAY_UNLOCK_IMAGES_, "0");
	else
		_yarpInterface->sendToSandtrayCommand(_SANDTRAY_LOCK_IMAGES_, "0");
}

void DeliberativeSubsystemController::scriptCorrupted(int stepId){
	if(DEBUG)
		cout << "-deliberativeSubsystem- scriptCorrupted" << endl;
	offScript();
		_yarpInterface->sendToSuggestedAction(stepId,_ACTION_CORRUPTED_,"");
}

void DeliberativeSubsystemController::getFromActionFeedback(int step, int action, int feedback, bool subAction){
	if (DEBUG) {
		cout << "-deliberativeSubsystem- Received action: " << step << " " << action << " " << feedback << " " << subAction << endl;
		if(_IwaitingAction>=0)
			cout << "-deliberativeSubsystem- waiting for: " << _IwaitingStep << " " << _IwaitingAction << " " << !(subAction && _Bwaiting4action && step == _IwaitingStep && action == _IwaitingAction) << endl;
	}
	if (action == _ACTION_GRAB_OBJECT_ && feedback == _FEEDBACK_SUCCESS_)
		_BobjectGrabbed = 1;
	if ((action == _ACTION_DROP_OBJECT_ || action == _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_) && feedback == _FEEDBACK_SUCCESS_)
		_BobjectGrabbed = 0;
	if (!subAction && _Bwaiting4action && step == _IwaitingStep && action == _IwaitingAction) {
		if (DEBUG)
			cout << "-deliberativeSubsystem- Finish waiting" << endl; 
		waitms(100);
		if (feedback == _FEEDBACK_SUCCESS_)
			goNextStep();
//		sendToComandSuccess(success);
		_Bwaiting4action = 0;
		_IwaitingStep = -1;
		_IwaitingAction = -1;
	}

	for (int i = 0; i < _actionWaiters.size(); i++)
		if (_actionWaiters[i]->testCondition(step, action))
			_actionWaiters[i]->notify(feedback);

}

void DeliberativeSubsystemController::processEvents(string str) {
	if(DEBUG)
		cout << "-deliberativeSubsystem- Sandtray event received: " << str << endl;
    if (str.find(_SANDTRAY_GREET_CATEGORY_) != string::npos) {
		if (!_bSandtrayConnected)
			_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_, _ACTION_SANDTRAY_TYPE_, _SANDTRAY_CATEGORY_);
		_bSandtrayConnected = 1;
		_bSandtrayCategory = 1;
	}
    if (str.find(_SANDTRAY_GREET_PATTERN_) != string::npos) {
		if (!_bSandtrayConnected)
			_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_, _ACTION_SANDTRAY_TYPE_, _SANDTRAY_PATTERN_);
		_bSandtrayConnected = 1;
		_bSandtrayCategory = 0;
	}
	vector<string> inputs = _common->split(str, ',');
	// this would be the receiving end in delib
	// we would filter these and send out to WP5 where relevant (e.g., good move, bad move)
	
	if (inputs[0] == _SANDTRAY_PLAYER_GOOD_MOVE_) {
		_yarpInterface->sendToInteractionEvent(0, atoi(inputs[1].c_str()));
		if(_BtherapistMove) {
			_BtherapistMove = 0;
			sandtrayEnableTouch(0);
		}
	}
	if (inputs[0] == _SANDTRAY_PLAYER_BAD_MOVE_) {
		_yarpInterface->sendToInteractionEvent(1,  atoi(inputs[1].c_str()));
	}
	if (inputs[0] == _SANDTRAY_PLAYER_TOUCH_IMAGE_) {
		_yarpInterface->sendToInteractionEvent(2,  atoi(inputs[1].c_str()));
		//sandtrayEnableTouch(0);
	}
	if (inputs[0] == _SANDTRAY_PLAYER_RELEASE_IMAGE_) {
		_yarpInterface->sendToInteractionEvent(3,  atoi(inputs[1].c_str()));
		if (_BtherapistTouch) {
			_BtherapistTouch = 0;
			sandtrayEnableTouch(0);
		}
	}
		
}

void DeliberativeSubsystemController::processReturns(string str) {
	//if(DEBUG)
		cout << "-deliberativeSubsystem- Sandtray return received: " << str << endl;

	vector<string> inputs = _common->split(str, ',');

	//Sleep(50);

	_waiterSand->notify();
	// this would be the receiving end in the actuationSubsystem
	// it would take the move data and calculate robot arm bezier curves for _SANDTRAY_MOVE_DATA_
	// or just do the inverse kinematics for _SANDTRAY_ROBOT_TURN_LOCATION_
	// it would send the move to be done, and when the motorFeedback indicates that the initial 
	// arm lift is complete it would return _SANDTRAY_READY_ and simultaneously begin the bezier
	// move if necessary (sending the ready signal starts on-screen move, or selection)

	// send a READY command if move related after robot lifts arm
	
	//ES: in delib, should we send the info to sms for log?
}

void DeliberativeSubsystemController::offScript() {
	if(DEBUG)
		cout << "-deliberativeSubsystem- going offscript" << endl;

	_BonScript = 0;
	resetWaitingStates();
}

void DeliberativeSubsystemController::onScript() {
	_BonScript = 1;
}

void DeliberativeSubsystemController::close(){
	_BonScript = 0;
	_waiterGaze->notify();
	_waiterPerf->notify();
	_waiterSand->notify();
	_timer->stop();
	delete _timer;
}

void DeliberativeSubsystemController::testRobotConnection() {
	int actionResult = 0;
	if (waitForAction(_ORIGIN_ACTUATION_, _ACTION_ENABLE_, actionResult)) {
		if(actionResult == _FEEDBACK_SUCCESS_) {
			if (DEBUG)
				cout << "-deliberativeSubsystem- Robot Connected" << endl;
			_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_, _ACTION_ROBOT_CONNECTED_,1);
		}
		if (actionResult == _FEEDBACK_NOT_CONNECTED_) {
			if(DEBUG)
				cout << "-deliberativeSubsystem- ERROR ROBOT NOT CONNECTED" << endl;
			_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_, _ACTION_ROBOT_CONNECTED_, 0);
		}

		if (_bSandtrayConnected) {
			if (DEBUG)
				cout << "-deliberativeSubsystem- Sandtray connected" << endl;
			_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_, _ACTION_SANDTRAY_TYPE_, _bSandtrayCategory?_SANDTRAY_CATEGORY_:_SANDTRAY_PATTERN_);
		}
		else {
			if(DEBUG)
				cout << "-deliberativeSubsystem- Sandtray not connected" << endl;
			_yarpInterface->sendToSuggestedAction(_ORIGIN_DELIBERATIVE_, _ACTION_SANDTRAY_TYPE_, _SANDTRAY_NOT_CONNECTED_);
		}
		//_yarpInterface->sendToActionFeedback(stepId, resolvingAction, actionResult, 0);
	}
	else
		cout << "-deliberativeSubsystem- Incorrect data" << endl;
}

bool DeliberativeSubsystemController::waitForAction(int stepId, int actionId, int& parameter) {
	if (DEBUG)
		cout << "-deliberativeSubsystem- Start waiting for step " << stepId << " action " << actionId << endl;

	ActionWaiter* waiter = new ActionWaiter(stepId, actionId);
	_actionWaiters.push_back(waiter);
	if (!waiter->startWaiting())
		return 0;

	if (DEBUG)
		cout << "-deliberativeSubsystem- Stop waiting for action " << endl;

	try {
		parameter = waiter->getParameterAsInt(0);
	}
	catch (...) {
		purgeActionWaiter();
		return 0;
	}
	purgeActionWaiter();
	return 1;
}

void DeliberativeSubsystemController::purgeActionWaiter() {
	vector<ActionWaiter*>::iterator it;
	for (it = _actionWaiters.begin(); it != _actionWaiters.end(); )
		if ((*it)->getIsNotWaiting()) {
			delete * it;
			it = _actionWaiters.erase(it);
		}
		else
			++it;
}

bool DeliberativeSubsystemController::getOnScript() {
	return _BonScript;
}

void DeliberativeSubsystemController::resetWaitingStates() {
	_waiterGaze->cancel();
	_waiterPerf->cancel();
	_waiterSand->cancel();
	_timer->stop();
	purgeActionWaiter();

	_IwaitingAction = -1;
	_IwaitingStep = -1;
	_Bwaiting4action = 0;
	_bWaitingForPerf = 0;

	_bAskLook = 0;
}
