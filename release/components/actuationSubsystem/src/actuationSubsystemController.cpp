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
  * 20/09/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "actuationSubsystemController.h"
#include "actuationSubsystemYarpInterface.h"
#include "actionMessages.h"
#include "sandtrayMessages.h"
#include "moveSequenceId.h"
#include "actionOrigin.h"
#include "waiter.h"
#include "commonFonctionalities.h"
#include "feedbackMessages.h"

const int DEBUG = 1;
using namespace std;

ActuationSubsystemController::ActuationSubsystemController(ActuationSubsystemYarpInterface *yarpInterface){
	_common = new CommonFunctionalities();
	
	_yarpInterface = yarpInterface;
	_bIsInterrupted = 0;
	_Btherapist = 0;
	_Bdiagnose = 0;
	_iFallingReactionPrevious = 0; 
	_iFaceExpressionPrevious = 0;
	_iSocialReactionPrevious = 99;
	_fallingSpeechSentences = std::vector<std::string>(_fallingSpeech, _fallingSpeech + sizeof(_fallingSpeech) / sizeof(std::string));
	_positiveSpeechSentences = std::vector<std::string>(_positiveSpeech, _positiveSpeech + sizeof(_positiveSpeech) / sizeof(std::string));
	_negativeSpeechSentences = std::vector<std::string>(_negativeSpeech, _negativeSpeech + sizeof(_negativeSpeech) / sizeof(std::string));
}

void ActuationSubsystemController::getFromSelectedAction(int stepId, int actionId, std::string arg) {
	Sleep(500);
	if (DEBUG)
		cout << "-actuationSubsystem- Received selected action " << actionId << " with arg " << arg << endl;
	Sleep(500);
	if (_Btherapist&&actionId!= _ACTION_SCRIPT_TYPE_)
		return;
	vector<string> list;
	// If we are in diagnose, we go next step
	if (actionId == _ACTION_FEEDBACK_BAD_NO_ACTION_ ||
		(_Bdiagnose || atoi(arg.c_str()) == 0) &&
		(actionId == _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_ ||
		actionId == _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_ ||
		actionId == _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_ ||
		actionId == _ACTION_FEEDBACK_IMITATION_MOVE_BAD_ ||
		actionId == _ACTION_FEEDBACK_TT_WAITING_GOOD_ ||
		actionId == _ACTION_FEEDBACK_TT_WAITING_BAD_ ||
		actionId == _ACTION_FEEDBACK_LAST_TT_WAITING_BAD_ ||
		actionId == _ACTION_FEEDBACK_TT_SORTING_GOOD_ ||
		actionId == _ACTION_FEEDBACK_TT_SORTING_BAD_ ||
		actionId == _ACTION_FEEDBACK_TT_CHOOSING_GOOD_ ||
		actionId == _ACTION_FEEDBACK_TT_CHOOSING_BAD_ ||
		actionId == _ACTION_FEEDBACK_TT_PATTERN_GOOD_ ||
		actionId == _ACTION_FEEDBACK_TT_PATTERN_BAD_ ||
		actionId == _ACTION_FEEDBACK_JA_GOOD_ ||
		actionId == _ACTION_FEEDBACK_JA_BAD_)) {
		Sleep(50);
		_yarpInterface->sendToActionFeedback(stepId, actionId, 1, 0);
		return;
	}

	switch (actionId)
	{
	case(_ACTION_RECEIVED_PERF_):
		break;
	case(_ACTION_GO_OFF_SCRIPT_):
		resetWaitingStates();
		break;
	case(_ACTION_SAY_):
		say(stepId, arg);
		break;
	case(_ACTION_REQUEST_GAZE_):
		say(stepId, "Uităte la mine! Hai să ne jucăm împreună!");
		break;
	case(_ACTION_MOVE_):
		moveSequence(stepId, atoi(arg.c_str()));
		break;
	case(_ACTION_STAND_):
		_yarpInterface->sendToMoveSequence(stepId, _MOVE_STAND_);
		waitAndSendFeedbackAction(stepId, _ACTION_MOVE_, _ACTION_STAND_);
		break;
	case(_ACTION_GRAB_OBJECT_):
		grabObject(stepId);
		break;
	case(_ACTION_DROP_OBJECT_):
		dropObject(stepId);
		break;
	case(_ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_):
		feedbackImitationObject(stepId, 1);
		break;
	case(_ACTION_FEEDBACK_IMITATION_OBJECT_BAD_):
		feedbackImitationObject(stepId, 0);
		break;
	case(_ACTION_FEEDBACK_IMITATION_MOVE_GOOD_):
		feedbackImitationMove(stepId, 1);
		break;
	case(_ACTION_FEEDBACK_IMITATION_MOVE_BAD_):
		feedbackImitationMove(stepId, 0);
		break;
	case(_ACTION_FEEDBACK_TT_WAITING_GOOD_):
		feedbackTTWaiting(stepId, 1);
		break;
	case(_ACTION_FEEDBACK_TT_WAITING_BAD_):
		feedbackTTWaiting(stepId, 0);
		break;
	case(_ACTION_FEEDBACK_TT_SORTING_GOOD_):
		feedbackTTSorting(stepId, 1);
		break;
	case(_ACTION_FEEDBACK_TT_SORTING_BAD_):
		feedbackTTSorting(stepId, 0);
		break;
	case(_ACTION_FEEDBACK_TT_CHOOSING_GOOD_):
		feedbackTTChoosing(stepId, 1);
		break;
	case(_ACTION_FEEDBACK_TT_CHOOSING_BAD_):
		feedbackTTChoosing(stepId, 0);
		break;
	case(_ACTION_FEEDBACK_TT_PATTERN_GOOD_):
		feedbackTTPattern(stepId, 1);
		break;
	case(_ACTION_FEEDBACK_TT_PATTERN_BAD_):
		feedbackTTPattern(stepId, 0);
		break;
	case(_ACTION_FEEDBACK_JA_GOOD_):
		feedbackJA(stepId, 1);
		break;
	case(_ACTION_FEEDBACK_JA_BAD_):
		feedbackJA(stepId, 0);
		break;
	case(_ACTION_FEEDBACK_LAST_TT_WAITING_BAD_):
		//_yarpInterface->sendToSay(stepId, "You have to wait! It’s my turn.");
		//_yarpInterface->sendToSay(stepId, "Tu trebuie să aștepți, este rândul meu.");
		_yarpInterface->sendToSay(stepId, "Tu trebuie să aștepț. este rândul meu.");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_LAST_TT_WAITING_BAD_);
		break;
	case(_ACTION_ROBOT_GOOD_MOVE_):
		robotGoodMove(stepId);
		break;
	case(_ACTION_POINT_IMAGE_):
		robotPointAtImage(stepId, atoi(arg.c_str()));
		break;
	case(_ACTION_LOOK_POINT_SAY_):
		lookPointSay(stepId, atoi(arg.c_str()));
		break;
	case(_ACTION_LOOK_POINT_):
		lookPoint(stepId, atoi(arg.c_str()));
		break;
	case(_ACTION_LOOK_):
		look(stepId, atoi(arg.c_str()));
		break;
	case(_ACTION_ENABLE_):
		_yarpInterface->sendToEnableRobot(stepId);
		waitAndSendFeedbackAction(stepId, _ACTION_ENABLE_, _ACTION_ENABLE_);
		break;
	case(_ACTION_DISABLE_):
		_yarpInterface->sendToDisableRobot(stepId);
		waitAndSendFeedbackAction(stepId, _ACTION_DISABLE_, _ACTION_DISABLE_);
		break;
	case(_ACTION_OPEN_HAND_):
		_yarpInterface->sendToRelease(atoi(arg.c_str()), stepId);
		waitAndSendFeedbackAction(stepId, _ACTION_OPEN_HAND_, _ACTION_OPEN_HAND_);
		break;
	case(_ACTION_CLOSE_HAND_):
		_yarpInterface->sendToGrip(atoi(arg.c_str()), stepId);
		waitAndSendFeedbackAction(stepId, _ACTION_CLOSE_HAND_, _ACTION_CLOSE_HAND_);
		break;
	case(_ACTION_SCRIPT_TYPE_):
		list = _common->split(arg, ',');
		_Bdiagnose = atoi(list[0].c_str());
		_Btherapist = !atoi(list[1].c_str());
		Sleep(500);
		if (DEBUG) {
			cout << "-actuationSubsystem- Current partner: " << (_Btherapist ? "Therapist" : "Robot") << endl;
			cout << "-actuationSubsystem- Interaction file: " << (_Bdiagnose ? "Diagnose" : "Intervention") << endl;
		}
		Sleep(50);
		_yarpInterface->sendToEnableRobot(_ORIGIN_ACTUATION_);
		waitAndSendFeedbackAction(_ORIGIN_ACTUATION_, _ACTION_ENABLE_, _ACTION_ENABLE_);
		break;
	case(_ACTION_CORRUPTED_):
		break;
	case(_ACTION_FINISHED_):
		break;
	default:
		break;
	}
}

void ActuationSubsystemController::getFromSensorFeedback(std::string name, bool value) {
	if(DEBUG)
		cout << "-actuationSubsystem- From sensor: " << name << endl;
	for (int i = 0; i < _sensorWaiters.size(); i++) {
		if (_sensorWaiters[i]->testCondition(name, value))
			_sensorWaiters[i]->notify();
	}
	_yarpInterface->sendToRobotSensors(name, value);
}


void ActuationSubsystemController::getFromMotorFeedback(int stepId, int actionId, int fbValue, int parameter){
	if (DEBUG) {
		_common->waitms(40);
		cout << "-actuationSubsystem- From motor feedback: step " << stepId << " action " << actionId << " value " << fbValue << " parameter " << parameter << endl;
	}
	_yarpInterface->sendToActionFeedback(stepId, actionId, fbValue, 1, parameter);
	if (stepId == _ORIGIN_ATTENTION_REACTION_) {
		_common->waitms(20);
		_yarpInterface->sendToActionFeedback(stepId, actionId, fbValue, 0, parameter);
	}

	_common->waitms(10);

	for (int i = 0; i < _actionWaiters.size(); i++)
		if (_actionWaiters[i]->testCondition(stepId, actionId))
			_actionWaiters[i]->notify(fbValue);
	
}

void ActuationSubsystemController::getFromEyeBlinking(int repetitions, bool fullLeds, int duration) {
	if(!_bIsInterrupted)
		_yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_BLINKING_, repetitions, fullLeds, duration);
}

void ActuationSubsystemController::getFromInterruption(bool interrupted) {
	if (DEBUG)
		cout << "-actuationSubsystem- Interrupted " << interrupted << endl;
	_bIsInterrupted = interrupted;
}

void ActuationSubsystemController::getFromElicitedAttention(double x, double y, double z) {
//	if (_bElicitedAttention)
//		return;
//	_bElicitedAttention = 1;
	_yarpInterface->sendToMoveHead(x, y, z, 0.2, _ORIGIN_ATTENTION_REACTION_);
}

void ActuationSubsystemController::getFromFallingReaction(int fallingReaction) {
	if (fallingReaction == _iFallingReactionPrevious)
		return;

	/*This information is sent but not implemented in Nao as it already includes a falling reaction module by default*/
	if ((fallingReaction == 1) )
		_yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_DAMAGE_AVOIDANCE_);
	else if (fallingReaction == 2) 
		_yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_GET_UP_);

	_iFallingReactionPrevious = fallingReaction;
}

void ActuationSubsystemController::getFromFallingReactionSpeech(int fallingReactionSpeech) {
    cout << "-actuationSubsystem- getFromFallingReactionSpeech: deprecated use" << endl;
	return;
	// _falling_Speech_Sentences define in moveSequenceId for easy modification later
	if (fallingReactionSpeech != 1)
		return;

	int random = rand() % _fallingSpeechSentences.size();
	_yarpInterface->sendToSay(_ORIGIN_ATTENTION_REACTION_, _fallingSpeechSentences[random]);
}


void ActuationSubsystemController::getFromSocialFacialExpression(int socialFacialReaction) {
	if (socialFacialReaction == _iFaceExpressionPrevious)
		return;

	switch (socialFacialReaction) {
	case 0: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_NEUTRAL_FACE_);
		break;
	case 1: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_HAPPY_FACE_);
		break;
	case 2: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_SAD_FACE_);
		break;
	case 3: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_ANGRY_FACE_);
		break;
	case 4: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_FEAR_FACE_);
		break;
	default:
		cout << "Unknow behaviour" << endl;
		return;
	}
	_iFaceExpressionPrevious = socialFacialReaction;
	waitAndSendFeedbackAction(_ORIGIN_ATTENTION_REACTION_, _ACTION_MOVE_, _ACTION_FACIAL_EXPRESSION_);
}

void ActuationSubsystemController::getFromSocialReaction(int socialReaction) {
	cout << "ACTUATION received " << socialReaction << " had " << _iSocialReactionPrevious << endl;
//	if (socialReaction == _iSocialReactionPrevious)
//		return;

	switch (socialReaction) {
	case -1: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_STOP_MICRO_MOTIONS_);
		break;
	case 0: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_MICRO_MOTIONS_);
		break;
/*	case 1: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_MICRO_EXPLAIN_3_);
		break;
	case 2: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_MICRO_EXPLAIN_8_);
		break;
	case 3: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_MICRO_THIS_);
		break;
	case 4: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_MICRO_WHATSTHIS_2_);
		break;
	case 5: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_MICRO_WHATSTHIS_5_);
		break;
	case 6: _yarpInterface->sendToMoveSequence(_ORIGIN_ATTENTION_REACTION_, _MOVE_MICRO_YOUKNOWWHAT_);
		break;
	*/
		default: return;
	}
	_iSocialReactionPrevious = socialReaction;
	waitAndSendFeedbackAction(_ORIGIN_ATTENTION_REACTION_, _ACTION_MOVE_, _ACTION_SOCIAL_REACTION_);
}

void ActuationSubsystemController::getFromSocialReactionSpeech(int socialReactionSpeech) {
    cout << "-actuationSubsystem- getFromSocialReactionSpeech: deprecated use" << endl;
	return;
	if (socialReactionSpeech>0) {
		int random = rand() % _positiveSpeechSentences.size();
		_yarpInterface->sendToSay(_ORIGIN_ATTENTION_REACTION_, _positiveSpeechSentences[random]);
	}
	if (socialReactionSpeech<0) {
		int random = rand() % _negativeSpeechSentences.size();
		_yarpInterface->sendToSay(_ORIGIN_ATTENTION_REACTION_, _negativeSpeechSentences[random]);
	}
	waitAndSendFeedbackAction(_ORIGIN_ATTENTION_REACTION_, _ACTION_SAY_, _ACTION_SOCIAL_REACTION_SPEECH_);
}

void ActuationSubsystemController::robotPointAtImage(int stepId, int imageId) {
	//To move from human counting to informatic counting
	imageId--;
	stringstream ss;
	ss << imageId;
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_GET_IMAGE_COORDINATES_, ss.str());
	const char *vinit[] = { "coordinates", "movefail" };
	vector<string> expectedString(vinit, end(vinit));
	vector<string> coordinates;

	if (!waitForSandtray(expectedString, coordinates)) {
		_yarpInterface->sendToActionFeedback(stepId, _ACTION_POINT_IMAGE_, _FEEDBACK_INCORRECT_DATA_,0);
		return;
	}

	if (coordinates[0] == "movefail") {
		if(DEBUG)
			cout << "-actuationSubsystem- action impossible" << endl;
		_yarpInterface->sendToActionFeedback(stepId, _ACTION_POINT_IMAGE_, _FEEDBACK_IMPOSSIBLE_,0);
		return;
	}

	vector<float> pixels;
	pixels.push_back(stof(coordinates[1]));
	pixels.push_back(stof(coordinates[2]));

	vector<float> goal = pixelToRobotPoint(pixels);
	bool rightArm = goal[1] < 0 ? 1 : 0;
	//Add time
	goal.push_back(1.);
	/*
	cout << "goal" << endl;
	for (int i = 0; i < goal.size(); i++)
		cout << goal[i] << " ";
	cout << endl; */
	_yarpInterface->sendToPointAt(goal, rightArm, stepId);
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_ROBOT_TURN_SELECTION_, ss.str());
	_common->waitms(1000);
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_READY_, ss.str());
	waitAndSendFeedbackAction(stepId, _ACTION_POINT_AT_, _ACTION_POINT_IMAGE_);
}

void ActuationSubsystemController::lookPointSay(int stepId, int direction) {
	//_yarpInterface->sendToSay(stepId, "Look");
	_yarpInterface->sendToSay(stepId, "Uite!");
	lookPoint(stepId, direction,_ACTION_LOOK_POINT_SAY_);
}

void ActuationSubsystemController::lookPoint(int stepId, int direction, int actionId) {
	vector<float> goal;
	int feedback = getCoordinatesImage(direction, goal);
	if(feedback != _FEEDBACK_SUCCESS_) {
		_yarpInterface->sendToActionFeedback(stepId, actionId, feedback,0);
		return;
	}

	bool rightArm = goal[1] < 0 ? 1 : 0;
	//Add time
	goal.push_back(1.);
	/*
	for (int i = 0; i < goal.size(); i++)
		cout << goal[i] << " ";
	cout << endl; */
	_yarpInterface->sendToPointAt(goal, rightArm, stepId);
	waitAndSendFeedbackAction(stepId, _ACTION_POINT_AT_, actionId);
}

void ActuationSubsystemController::look(int stepId, int direction) {
	vector<float> goal;

	int feedback = getCoordinatesImage(direction, goal);
	if (feedback != _FEEDBACK_SUCCESS_) {
		_yarpInterface->sendToActionFeedback(stepId, _ACTION_LOOK_, feedback,0);
		return;
	}

	float speed = 0.1;
//	cout << "About to look at";
//	cout << goal[0] << " " << goal[1] << " " << goal[2] << endl;
	_yarpInterface->sendToMoveHead(goal[0], goal[1], goal[2], speed, stepId);
	waitAndSendFeedbackAction(stepId, _ACTION_MOVE_HEAD_, _ACTION_LOOK_);
}

void ActuationSubsystemController::moveHead(float x, float y, float z, float speed, int stepId) {
	_yarpInterface->sendToMoveHead(x, y, z, speed, stepId);
	waitAndSendFeedbackAction(stepId, _ACTION_MOVE_HEAD_, _ACTION_MOVE_HEAD_);
}

void ActuationSubsystemController::moveHand(bool rightArm, float x, float y, float z, float roll, int stepId) {
	_yarpInterface->sendToMoveHand(rightArm, x, y, z, roll, stepId);
	waitAndSendFeedbackAction(stepId, _ACTION_MOVE_HAND_, _ACTION_MOVE_HAND_);
}


void ActuationSubsystemController::say(int stepId, string sentence) {
	_yarpInterface->sendToSay(stepId, sentence);
	waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_SAY_);
}

void ActuationSubsystemController::moveSequence(int stepId, int id) {
	_yarpInterface->sendToMoveSequence(stepId, id);
	waitAndSendFeedbackAction(stepId, _ACTION_MOVE_, _ACTION_MOVE_);
}

void ActuationSubsystemController::grabObject(int stepId) {
	_yarpInterface->sendToMoveSequence(stepId, _MOVE_WAITING_GRAB_);
	bool test = waitForSensor("MiddleTactil", 1);
	if (!test) {
		_yarpInterface->sendToActionFeedback(stepId, _ACTION_DROP_OBJECT_, _FEEDBACK_FAILURE_, 0);
		return;
	}
	_yarpInterface->sendToGrip(_LEFT_ARM_, stepId);
	waitAndSendFeedbackAction(stepId, _ACTION_CLOSE_HAND_, _ACTION_GRAB_OBJECT_);
	/*
	if (handOpen == 1)
	straightenArm(includes openHand)						***** Probably, have to check with Cristina
	while (!touchHand)
	;
	closeHand
	*/
}

void ActuationSubsystemController::dropObject(int stepId) {
	_yarpInterface->sendToMoveSequence(stepId, _MOVE_WAITING_DROP_);
	int actionResult = 0;
	bool test = waitForAction(stepId, _ACTION_MOVE_, actionResult);
	if (!test) {
		_yarpInterface->sendToActionFeedback(stepId, _ACTION_DROP_OBJECT_, _FEEDBACK_INCORRECT_DATA_, 0);
		return;
	}
	_yarpInterface->sendToRelease(_LEFT_ARM_, stepId); 
	test = waitForSensor("MiddleTactil", 1);
	if (!test) {
		_yarpInterface->sendToActionFeedback(stepId, _ACTION_DROP_OBJECT_, _FEEDBACK_FAILURE_, 0);
		return;
	}

	_yarpInterface->sendToMoveSequence(stepId, _MOVE_STAND_);
	waitAndSendFeedbackAction(stepId, _ACTION_MOVE_, _ACTION_DROP_OBJECT_);
}

void ActuationSubsystemController::feedbackImitationObject(int stepId, bool good) {
	if (good) {
		_yarpInterface->sendToMoveSequence(stepId, _MOVE_WAITING_DROP_);
		int actionResult = 0;
		bool test = waitForAction(stepId, _ACTION_MOVE_, actionResult);
		if (!test) {
			_yarpInterface->sendToActionFeedback(stepId, _ACTION_DROP_OBJECT_, _FEEDBACK_INCORRECT_DATA_, 0);
			return;
		}
		_yarpInterface->sendToRelease(_LEFT_ARM_, stepId);
		test = waitForSensor("MiddleTactil", 1);
		if (!test) {
			_yarpInterface->sendToActionFeedback(stepId, _ACTION_DROP_OBJECT_, _FEEDBACK_FAILURE_, 0);
			return;
		}
		int seqId = _MOVE_GOOD_FB_IMITATION_OBJECT_;  
		_yarpInterface->sendToMoveSequence(stepId, seqId);
		waitAndSendFeedbackAction(stepId, _ACTION_MOVE_, _ACTION_FEEDBACK_IMITATION_OBJECT_GOOD_);
	}
	else {
		//_yarpInterface->sendToSay(stepId, "Try again");
		//_yarpInterface->sendToSay(stepId, "Mai încearcă!");
		_yarpInterface->sendToSay(stepId, "Mai âncearcă!");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_IMITATION_OBJECT_BAD_);
	}
}

void ActuationSubsystemController::feedbackImitationMove(int stepId, bool good) {
	if (good) {
		int seqId = _MOVE_GOOD_FB_IMITATION_MOVE_; 
		_yarpInterface->sendToMoveSequence(stepId, seqId);
		waitAndSendFeedbackAction(stepId, _ACTION_MOVE_, _ACTION_FEEDBACK_IMITATION_MOVE_GOOD_);
	}
	else {
		//_yarpInterface->sendToSay(stepId, "Try again");
		//_yarpInterface->sendToSay(stepId, "Mai încearcă!");
		_yarpInterface->sendToSay(stepId, "Mai âncearcă!");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_IMITATION_MOVE_BAD_);
	}
}

void ActuationSubsystemController::feedbackTTWaiting(int stepId, bool good) {
	if (good) {
		//_yarpInterface->sendToSay(stepId, "You have waited very nicely");
		//_yarpInterface->sendToSay(stepId, "Ai așteptat frumos!");
		_yarpInterface->sendToSay(stepId, "Aai așteptat frumos.");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_TT_WAITING_GOOD_);
	}
	else {
		//_yarpInterface->sendToSay(stepId, "Try again");
		//_yarpInterface->sendToSay(stepId, "Mai încearcă!");
		//_yarpInterface->sendToSay(stepId, "Mai âncearcă!");
		//The wrong sentence was selected
		//_yarpInterface->sendToSay(stepId, "You have to wait! It’s my turn.");
		//_yarpInterface->sendToSay(stepId, "Tu trebuie să aștepți, este rândul meu.");
		_yarpInterface->sendToSay(stepId, "Tu trebuie să aștepț. este rândul meu.");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_TT_WAITING_BAD_);
	}
}

void ActuationSubsystemController::feedbackTTSorting(int stepId, bool good) {
	if (good) {
		//_yarpInterface->sendToSay(stepId, "You sorted correctly the picture. Well done!");
		//_yarpInterface->sendToSay(stepId, "Ai potrivit corect poza. Foarte bine!");
		_yarpInterface->sendToSay(stepId, "Ai potrivit corect poza; Foarte bine!");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_TT_SORTING_GOOD_);
	}
	else {
		//_yarpInterface->sendToSay(stepId, "You sorted incorrectly. Try again next time.");
		//_yarpInterface->sendToSay(stepId, "Ai potrivit greșit poza. Mai încearcă! ");
		_yarpInterface->sendToSay(stepId, "Ai potrivit greșit poza; Mai âncearcă! ");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_TT_SORTING_BAD_);
	}
}

void ActuationSubsystemController::feedbackTTChoosing(int stepId, bool good) {
	if (good) {
		//_yarpInterface->sendToSay(stepId, "You showed me very nicely what you like");
		//_yarpInterface->sendToSay(stepId, "Mi-ai arătat foarte frumos ce îți place!");
		_yarpInterface->sendToSay(stepId, "Mi-ai arătat foarte frumos ce-ți place!");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_TT_CHOOSING_GOOD_);
	}
	else { //Disabled, replaced by _ACTION_FEEDBACK_BAD_NO_ACTION_
		_common->waitms(500);
		if (DEBUG)
			cout << "-actuactionSubsystem- No action should be executed" << endl;
		_yarpInterface->sendToActionFeedback(stepId, _ACTION_FEEDBACK_TT_CHOOSING_BAD_, 1, 0);
	}
}

void ActuationSubsystemController::feedbackTTPattern(int stepId, bool good) {
	if (good) {
		//_yarpInterface->sendToSay(stepId, "You have matched correctly the picture. Well done!");
		//_yarpInterface->sendToSay(stepId, "Ai potrivit corect poza. Foarte bine!");
		_yarpInterface->sendToSay(stepId, "Ai potrivit corect poza; Foarte bine!");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_TT_PATTERN_GOOD_);
	}
	else {
		//_yarpInterface->sendToSay(stepId, "You have matched incorrectly the picture. Try again next time.");
		//_yarpInterface->sendToSay(stepId, "Ai potrivit greșit poza. Mai încearcă! ");
		_yarpInterface->sendToSay(stepId, "Ai potrivit greșit poza; Mai âncearcă! ");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_TT_PATTERN_BAD_);
	}
}

void ActuationSubsystemController::feedbackJA(int stepId, bool good) {
	if (good) {
		//_yarpInterface->sendToSay(stepId, "Well done!");
		_yarpInterface->sendToSay(stepId, "Foarte bine!");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_JA_GOOD_);
	}
	else {
		//_yarpInterface->sendToSay(stepId, "Try again next time");
		//_yarpInterface->sendToSay(stepId, "Mai încearcă!");
		_yarpInterface->sendToSay(stepId, "Mai âncearcă!");
		waitAndSendFeedbackAction(stepId, _ACTION_SAY_, _ACTION_FEEDBACK_JA_BAD_);
	}
}

void ActuationSubsystemController::getFromSandtrayReturn(std::string message) {
	if (DEBUG) {
		_common->waitms(50);
		cout << "-actuationSubsystem- Received from sandtray: " << message << endl;
		_common->waitms(50);
	}
	vector<string> inputs = _common->split(message, ',');

	//At the start, the sandtray send "i am a touchscreen" both to command and event, and it is not processed by the command
	//part, so the first time a command is asked by the controller, the sentence "i am a touchscreen" is sent in addition to 
	//the actual command, need to be removed
	inputs[0] = _common->removeString(inputs[0], "i am a touchscreen");

	for (int i = 0; i<_sandtrayWaiters.size(); i++)
		if (_sandtrayWaiters[i]->testCondition(inputs[0]))
			_sandtrayWaiters[i]->notify(inputs);
}

void ActuationSubsystemController::robotGoodMove(int stepId) {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_MAKE_GOOD_MOVE_, "0");
	const char *vinit[] = { "movedata", "movefail"};
	vector<string> expectedString(vinit, end(vinit));
	vector<string> coordinates; 
	
	if (!waitForSandtray(expectedString, coordinates)) {
		_yarpInterface->sendToActionFeedback(stepId, _ACTION_ROBOT_GOOD_MOVE_, _FEEDBACK_INCORRECT_DATA_,0);
		return;
	}

	if(coordinates[0]=="movefail") {
		if(DEBUG)
			cout << "-actuationSubsystem- Impossible" << endl;
		_yarpInterface->sendToActionFeedback(stepId, _ACTION_ROBOT_GOOD_MOVE_, _FEEDBACK_IMPOSSIBLE_,0);
		return;
	}

	vector<vector<float>> goal = startBezier(coordinates);
	//Decide which arm choose
	float totalY = 0;
	for (unsigned i = 0; i < goal.size(); i++)
		totalY += goal[i][1];
	bool rightArm = totalY < 0 ? 1 : 0;

	//Extract first move to point to an image
	vector<float> initialMove = goal[0];
	initialMove[3] = 2;
	//Remove first move as it has already been done
	goal.erase(goal.begin(), goal.begin() + 1);

	_yarpInterface->sendToPointAt(initialMove, rightArm, stepId);
	int actionResult = 0;
	if (!waitForAction(stepId, _ACTION_POINT_AT_, actionResult)) {
		_yarpInterface->sendToActionFeedback(stepId, _ACTION_ROBOT_GOOD_MOVE_, _FEEDBACK_INCORRECT_DATA_,0);
		return;
	}

	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_READY_, "0");
	_yarpInterface->sendToPointAt(goal, rightArm, stepId);
	if (!waitForAction(stepId, _ACTION_POINT_AT_, actionResult)) {
		_yarpInterface->sendToActionFeedback(stepId, _ACTION_ROBOT_GOOD_MOVE_, _FEEDBACK_INCORRECT_DATA_, 0);
		return;
	}
	_yarpInterface->sendToMoveSequence(stepId, _MOVE_STAND_);
	waitAndSendFeedbackAction(stepId, _ACTION_MOVE_, _ACTION_ROBOT_GOOD_MOVE_);
}


vector<vector<float>> ActuationSubsystemController::startBezier(vector<string> inputs) {
	vector<vector<float>> bezier;
	float time = stof(inputs[14]);
	unsigned actionNumber = 4;

	//extract the bezier parameter from the inputs parameter received from the Sandtray
	for (unsigned i = 0; i<4; i++) {
		vector<float> temp;
		temp.push_back(stof(inputs[5 + 2 * i]));
		temp.push_back(stof(inputs[6 + 2 * i]));
		bezier.push_back(temp); //bezier(4 x (x,y))
	}

	//Extract the 5 intermediate points from the bezier function
	vector<vector<float>> points;
	for (unsigned i = 0; i<actionNumber + 1; i++) {
		points.push_back(getBezier(bezier, float(i) / actionNumber));
	}

	//processing to extract time in function of the linear between the different points
	vector<float> d; //linear distances  between points
	for (unsigned i = 0; i<actionNumber; i++)
		d.push_back(getDistance(points[i], points[i + 1]));

	float totalDistance = 0;
	for (unsigned i = 0; i<actionNumber; i++) {
		totalDistance += d[i];
	}
	vector<float> times;
	times.push_back(0.);
	for (unsigned i = 1; i<actionNumber; i++) {
		times.push_back(times[i - 1] + time*d[i] / totalDistance);
	}
	times.push_back(time);

	//Transform a vector a point in pixel and time to a vector of floats usable by pointAt
	vector<vector<float>> goals;
	float totalY = 0;
	for (unsigned i = 0; i<actionNumber + 1; i++) {
		vector<float> temp = pixelToRobotMove(points[i]);
		temp.push_back(times[i]);
		goals.push_back(temp);
	}
	return goals;
}

//compute bezier
vector<float> ActuationSubsystemController::getBezier(vector<vector<float>> points, float t) {
	unsigned i = points.size() - 1;
	while (i>0) {
		for (unsigned k = 0; k<i; k++) {
			points[k][0] = points[k][0] + t * (points[k + 1][0] - points[k][0]);
			points[k][1] = points[k][1] + t * (points[k + 1][1] - points[k][1]);
		}
		i--;
	}
	return points[0];
}

float ActuationSubsystemController::getDistance(vector<float> p1, vector<float> p2) {
	return sqrt(pow(p2[0] - p1[0], 2) + pow(p2[1] - p1[1], 2));
}

vector<float> ActuationSubsystemController::pixelToRobotMove(vector<float> pixel) {
	vector<float> point;
	point.push_back(_SCREEN_LENGTH_*pixel[1] / _SCREEN_RESOLUTION_Y_ + _DISTANCE_ROBOT_SCREEN_X_);
	point.push_back(_SCREEN_WIDTH_*(pixel[0] / _SCREEN_RESOLUTION_X_ - 0.5));
	point.push_back(_SCREEN_HEIGHT_);
	return point;
}

vector<float> ActuationSubsystemController::pixelToRobotPoint(vector<float> pixel) {
	vector<float> point;
	point.push_back(_SCREEN_LENGTH_*(pixel[1] / _SCREEN_RESOLUTION_Y_ + 0.5f) + _DISTANCE_ROBOT_SCREEN_X_);
	point.push_back(_SCREEN_WIDTH_*(pixel[0] / _SCREEN_RESOLUTION_X_));
	point.push_back(_SCREEN_HEIGHT_);
	return point;
}

bool ActuationSubsystemController::waitForAction(int stepId, int actionId, int& parameter) {
	if (DEBUG)
		cout << "-actuationSubsystem- Start waiting for step "<<stepId<<" action "<<actionId << endl;

	ActionWaiter* waiter = new ActionWaiter(stepId, actionId);
	_actionWaiters.push_back(waiter);
	if (waiter->startWaiting())
		return 0;

	if (DEBUG)
		cout << "-actuationSubsystem- Stop waiting for action " << endl;

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

void ActuationSubsystemController::purgeActionWaiter() {
	vector<ActionWaiter*>::iterator it;
	for (it = _actionWaiters.begin(); it != _actionWaiters.end(); )
		if ((*it)->getIsNotWaiting()){
			delete * it;
			it = _actionWaiters.erase(it);
		}
	   else 
		   ++it; 
}

bool ActuationSubsystemController::waitForSandtray(vector<string> toMatch, vector<string>& parameter) {
	if (DEBUG)
		cout << "-actuationSubsystem- Start waiting for sandtray " << endl;

	SandtrayWaiter* waiter = new SandtrayWaiter(toMatch);
	_sandtrayWaiters.push_back(waiter);
	if (waiter->startWaiting())
		return 0;
	if (DEBUG)
		cout << "-actuationSubsystem- Stop waiting for sandtray " << endl;

	try {
		parameter = waiter->getParameterAsVectorString(0);
	}
	catch (...) {
		purgeSandtrayWaiter();
		return 0;
	}
	purgeSandtrayWaiter();
	return 1;
}

void ActuationSubsystemController::purgeSandtrayWaiter() {
	vector<SandtrayWaiter*>::iterator it;
	for (it = _sandtrayWaiters.begin(); it != _sandtrayWaiters.end(); )
		if ((*it)->getIsNotWaiting()) {
			delete * it;
			it = _sandtrayWaiters.erase(it);
		}
		else
			++it;
}


bool ActuationSubsystemController::waitForSensor(std::string name, bool value) {
	if (DEBUG)
		cout << "-actuationSubsystem- Start waiting for sensor " << name << endl;

	SensorWaiter* waiter = new SensorWaiter(name, value);
	_sensorWaiters.push_back(waiter);
	if (waiter->startWaiting())
		return 0;
	purgeSandtrayWaiter();
	if (DEBUG)
		cout << "-actuationSubsystem- Stop waiting for sensor " << name << endl;
	return 1;
}

void ActuationSubsystemController::purgeSensorWaiter() {
	vector<SensorWaiter*>::iterator it;
	for (it = _sensorWaiters.begin(); it != _sensorWaiters.end(); )
		if ((*it)->getIsNotWaiting()) {
			delete * it;
			it = _sensorWaiters.erase(it);
		}
		else
			++it;
}

int ActuationSubsystemController::getCoordinatesImage(int imageId, std::vector<float>& goal) {
	stringstream ss;
	ss << imageId;
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_GET_IMAGE_COORDINATES_, ss.str());
	const char *vinit[] = { "coordinates", "movefail" };
	vector<string> expectedString(vinit, end(vinit));
	vector<string> coordinates;

	if (!waitForSandtray(expectedString, coordinates)) 
		return _FEEDBACK_INCORRECT_DATA_;

	if (coordinates[0] == "movefail")
		return _FEEDBACK_IMPOSSIBLE_;


	vector<float> pixels;
	pixels.push_back(stof(coordinates[1]));
	pixels.push_back(stof(coordinates[2]));

	goal = pixelToRobotPoint(pixels);
	return _FEEDBACK_SUCCESS_;

}

void ActuationSubsystemController::waitAndSendFeedbackAction(int stepId, int waitingAction, int resolvingAction) {
	if (DEBUG) {
		_common->waitms(20);
		cout << "-actuationSubsystem- Start waiting for feedback: " << stepId << " " << waitingAction << " for " << resolvingAction << endl;
	}
	_common->waitms(10);
	int actionResult = 0;
	if (waitForAction(stepId, waitingAction, actionResult)) {
		if (DEBUG)
			cout << "-actuationSubsystem- Sending success " << stepId << " " << resolvingAction << " " << actionResult << endl;
		if (stepId == _ORIGIN_ACTUATION_) // Adding delay to let delib time to start waiting
			_common->waitms(200);
		_yarpInterface->sendToActionFeedback(stepId, resolvingAction, actionResult, 0);
	}
	else
		_yarpInterface->sendToActionFeedback(stepId, resolvingAction, _FEEDBACK_INCORRECT_DATA_, 0);
}

bool ActuationSubsystemController::getTherapist() {
	return _Btherapist;
}

void ActuationSubsystemController::resetWaitingStates() {
	Sleep(100);

	vector<SensorWaiter*>::iterator it;
	for (it = _sensorWaiters.begin(); it != _sensorWaiters.end(); ++it)
		(*it)->cancel();
	vector<ActionWaiter*>::iterator it2;
	for (it2 = _actionWaiters.begin(); it2 != _actionWaiters.end(); ++it2)
		(*it2)->cancel();
	vector<SandtrayWaiter*>::iterator it3;
	for (it3 = _sandtrayWaiters.begin(); it3 != _sandtrayWaiters.end(); ++it3)
		(*it3)->cancel();

	purgeActionWaiter();
	purgeSandtrayWaiter();
	purgeSensorWaiter();
	
}
