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
  * 16/08/2016 Version 1.0: ... (Emmanuel Senft)
  */ 

#include "naoInterfaceDriverController.h"
#include "naoInterfaceDriverYarpInterface.h"
#include "waiter.h"
#include "commonFonctionalities.h"
#include "feedbackMessages.h"
#include  "moveSequenceId.h"

using namespace std;

NaoInterfaceDriverController::NaoInterfaceDriverController(NaoInterfaceDriverYarpInterface *yarpInterface){
	_common = new CommonFunctionalities();
	_yarpInterface = yarpInterface;
	_waiter = new Waiter();
}


void NaoInterfaceDriverController::getFromSensorsFeedbacks(std::string message, int value){
	cout << "-naoInterfaceDriver- Received from sensor Feedback " << message << " " << value << endl;
}

void NaoInterfaceDriverController::getFromMotorFeedbacks(int stepId, int actionId, int message){
	cout << "-naoInterfaceDriver- Received from motor Feedback step " << stepId << " action " << actionId << " message " << message;
	switch (message) {
		case _FEEDBACK_INCORRECT_DATA_: cout << " _FEEDBACK_INCORRECT_DATA_ " << endl; break;
		case _FEEDBACK_SUCCESS_: cout << " _FEEDBACK_SUCCESS_ " << endl; break;
		case _FEEDBACK_FAILURE_: cout << " _FEEDBACK_FAILURE_ " << endl; break;
		case _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_: cout << " _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_ " << endl; break;
		case _FEEDBACK_IMPOSSIBLE_: cout << " _FEEDBACK_IMPOSSIBLE_ " << endl; break;
		case _FEEDBACK_NOT_CONNECTED_: cout << " _FEEDBACK_NOT_CONNECTED_ " << endl; break;
		default: break;
	}
	_waiter->notify();
}

void NaoInterfaceDriverController::closeHand(bool rightHand, bool close, int stepId){
	if(close)
		_yarpInterface->sendToGrip(rightHand, stepId);
	else
		_yarpInterface->sendToRelease(rightHand, stepId);
}

void NaoInterfaceDriverController::enableRobot(bool enable, int stepId){
	if(enable)
		_yarpInterface->sendToEnableRobot(stepId);
	else
		_yarpInterface->sendToDisableRobot(stepId);
}

void NaoInterfaceDriverController::say(std::string sentence, int stepId){
	_yarpInterface->sendToSay(sentence,stepId);
}

void NaoInterfaceDriverController::move(int stepId, int actionId){
	_yarpInterface->sendToMoveSequence(stepId, actionId);
}

void NaoInterfaceDriverController::lookAt(float x, float y, float z, float speed, int stepId){
	vector<double> vec;
	vec.push_back(x);
	vec.push_back(y);
	vec.push_back(z);
	vec.push_back(speed);
	_yarpInterface->sendToMoveHead(vec, stepId);
}

void NaoInterfaceDriverController::pointAt(std::vector<std::vector<float>> goals, bool rightArm, int stepId){
	vector<float> vec;
	for(unsigned i=0;i<goals.size();i++)
		for(unsigned j=0;j<goals[i].size();j++)
			vec.push_back(goals[i][j]);

	vec.push_back(rightArm);
	_yarpInterface->sendToPointAt(vec, stepId);
}

void NaoInterfaceDriverController::generatePointAt(int stepId){
	vector<vector<float>> goals;
	vector<float> vec;
	vec.push_back(1);
	vec.push_back(0);
	vec.push_back(0);
	vec.push_back(4);
	goals.push_back(vec);
	vec.clear();
	vec.push_back(0);
	vec.push_back(1);
	vec.push_back(0);
	vec.push_back(8);
	goals.push_back(vec);
	vec.clear();
	vec.push_back(2);
	vec.push_back(0);
	vec.push_back(0.5);
	vec.push_back(12);
	goals.push_back(vec);
	pointAt(goals, 1,stepId);
}


void NaoInterfaceDriverController::testAll() {
	
	cout << "-naoInterfaceDriver- Enable" << endl;
	enableRobot(1, 1);	
	goNextStep();

	cout << "-naoInterfaceDriver- Closing right hand" << endl;
	closeHand(1, 1, 2);
	goNextStep();

	cout << "-naoInterfaceDriver- Closing left hand" << endl;
	closeHand(0, 1, 3);
	goNextStep();

	cout << "-naoInterfaceDriver- Opening left hand" << endl;
	closeHand(0, 0, 4);	
	goNextStep();
	
	cout << "-naoInterfaceDriver- Opening right hand" << endl;
	closeHand(1, 0, 5);	
	goNextStep();
	
	cout << "-naoInterfaceDriver-Say \"Hello. I'm Nao.\"" << endl;
	say("Hello. I'm Nao", 6);	
	goNextStep();
	
	cout << "-naoInterfaceDriver- look slowly left using MoveHead" << endl;
	lookAt(0, -1, 0.5, 0.2, 7);	
	goNextStep();

	cout << "-naoInterfaceDriver- look quickly right using MoveHead" << endl;
	lookAt(0, 1, 0.5, 0.8, 8);
	goNextStep();

	cout << "-naoInterfaceDriver- move hand using MoveHand" << endl;
	cout << "-naoInterfaceDriver- uses naoQi positionInterpolations not totally supported" << endl;
	moveHand();
	goNextStep();

	cout << "-naoInterfaceDriver- Stand" << endl;
	move(10, _MOVE_STAND_);
	goNextStep();

	cout << "-naoInterfaceDriver- Point and look at 3 different points using pointAt" << endl;
	generatePointAt(11);	
	goNextStep();
		
	cout << "-------------GOING-THROUGH-ALL-THE-MOVE-SEQUENCES----------------" << endl;

	cout << "-naoInterfaceDriver- Take hard-coded position: _MOVE_WAITING_GRAB_ " << endl;
	move(12, _MOVE_WAITING_GRAB_);
	goNextStep();

	cout << "-naoInterfaceDriver- Take hard-coded position: _MOVE_WAITING_DROP_ " << endl;
	move(13, _MOVE_WAITING_DROP_);
	goNextStep();

	cout << "-naoInterfaceDriver- Take hard-coded position: _MOVE_STAND_ " << endl;
	move(14, _MOVE_STAND_);
	goNextStep();

	cout << "-------------NEXT-BEHAVIOURS-NOT-WORKING-ON-SIMULATOR----------------" << endl;

	cout << "-naoInterfaceDriver- Run Choregraphe behaviour from config file \"Move car\" " << endl;
	move(15, 0);
	goNextStep();

	cout << "-naoInterfaceDriver- Run hard coded choregraphe behaviour \"Clapping\" (used for _MOVE_GOOD_FB_IMITATION_OBJECT_ and _MOVE_GOOD_FB_IMITATION_MOVE_" << endl;
	move(16, _MOVE_GOOD_FB_IMITATION_OBJECT_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_HAPPY_FACE_ " << endl;
	move(17, _MOVE_HAPPY_FACE_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_SAD_FACE_ " << endl;
	move(18, _MOVE_SAD_FACE_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_ANGRY_FACE_ " << endl;
	move(19, _MOVE_ANGRY_FACE_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_FEAR_FACE_ " << endl;
	move(20, _MOVE_FEAR_FACE_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_NEUTRAL_FACE_ " << endl;
	move(21, _MOVE_NEUTRAL_FACE_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_DAMAGE_AVOIDANCE_ " << endl;
	cout << "-naoInterfaceDriver- Not implemented as the session would probably have to stop if happened " << endl;
	move(22, _MOVE_DAMAGE_AVOIDANCE_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_GET_UP_ " << endl;
	cout << "-naoInterfaceDriver- Not implemented as the session would probably have to stop if happened " << endl;
	move(23, _MOVE_GET_UP_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_MICRO_EXPLAIN_3_ " << endl;
	move(24, _MOVE_MICRO_EXPLAIN_3_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_MICRO_EXPLAIN_8_ " << endl;
	move(25, _MOVE_MICRO_EXPLAIN_8_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_MICRO_THIS_ " << endl;
	move(26, _MOVE_MICRO_THIS_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_MICRO_WHATSTHIS_2_ " << endl;
	move(27, _MOVE_MICRO_WHATSTHIS_2_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_MICRO_WHATSTHIS_5_ " << endl;
	move(28, _MOVE_MICRO_WHATSTHIS_5_);
	goNextStep();

	cout << "-naoInterfaceDriver- Run Attention-Reaction behaviour: _MOVE_MICRO_YOUKNOWWHAT_ " << endl;
	move(29, _MOVE_MICRO_YOUKNOWWHAT_);
	goNextStep();

	cout << "-naoInterfaceDriver- Disable robot" << endl;
	enableRobot(0, 30);
	_waiter->startWaiting();

	cout << "-naoInterfaceDriver- Activate micro-motion (autonomous life): _MOVE_MICRO_MOTIONS_ " << endl;
	cout << "-naoInterfaceDriver- Disabled on simulator" << endl;
	move(31, _MOVE_MICRO_MOTIONS_);
	goNextStep();

	cout << "-naoInterfaceDriver- Stop micro-motion (autonomous life): _MOVE_STOP_MICRO_MOTIONS_ " << endl;
	cout << "-naoInterfaceDriver- Disabled on simulator" << endl;
	move(32, _MOVE_STOP_MICRO_MOTIONS_);
	goNextStep();

	cout << "-naoInterfaceDriver- Blink: _MOVE_BLINKING_ + repetition (3), full leds ( 0), duration (2000)" << endl;
	blink(33, 3, 0, 2000);
	//goNextStep();
    waitms(1000);

	cout << "----------------------------------------------------" << endl;
	cout << "Test finished" << endl;
	cout << "----------------------------------------------------" << endl;
}

void NaoInterfaceDriverController::waitms(int millisec) {
	_common->waitms(millisec);
}

void NaoInterfaceDriverController::goNextStep() {
	_waiter->startWaiting();
	waitms(1000);
	cout << "--------------------NEXT-ACTION-----------------------------" << endl;
	waitms(1000);
}


void NaoInterfaceDriverController::blink(int step, int repetition, int fullLeds, int duration) {
	_yarpInterface->sendToMoveSequence(step, _MOVE_BLINKING_, repetition, fullLeds, duration);
}

void NaoInterfaceDriverController::moveHand() {
	vector<double> vec;
	vec.push_back(1);		//rightHand
	vec.push_back(0.5);		//x in meters
	vec.push_back(-0.1);	//y
	vec.push_back(0.);		//z
	vec.push_back(180);		//roll degrees
	_yarpInterface->sendToMoveHand(vec, 9);
}