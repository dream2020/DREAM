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
  * 22/06/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "naoInterfaceController.h"
#include "naoInterfaceYarpInterface.h"
#include "naoInterfaceModule.h"

#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <feedbackMessages.h>
#include <actionMessages.h>
#include <moveSequenceId.h>

using namespace std;

const int DEBUG = 1;

const char * const NaoInterfaceController::behavioursHardCoded[] = {"behaviours-13b7b4/clapping"};
const char * const NaoInterfaceController::bodyList[] = { "HeadPitch","HeadYaw","LAnklePitch","LAnkleRoll","LElbowRoll","LElbowYaw","LHand","LHipPitch","LHipRoll","LHipYawPitch","LKneePitch","LShoulderPitch","LShoulderRoll","LWristYaw","RAnklePitch","RAnkleRoll","RElbowRoll","RElbowYaw","RHand","RHipPitch","RHipRoll","RHipYawPitch","RKneePitch","RShoulderPitch","RShoulderRoll","RWristYaw"};
static const float _anglesWaitingObject[] = { -0.147306, -0.0107799, 0.0904641, -0.122678, -0.224006, -2.07239, 0.9816, 0.124296, 0.104354, -0.162562, -0.090548, 0.11816, -0.0858622, -1.11824, 0.093616, 0.135034, 0.404934, 1.21037, 0.2868, 0.131882, -0.102736, -0.162562, -0.082794, 1.45112, -0.237728, -0.07359 }; 
static const float _anglesDropingObject[] = { -0.147306, -0.0107799, 0.0904641, -0.122678, -0.224006, -2.07239, 0.1, 0.124296, 0.104354, -0.162562, -0.090548, 0.11816, -0.0858622, -1.11824, 0.093616, 0.135034, 0.404934, 1.21037, 0.2868, 0.131882, -0.102736, -0.162562, -0.082794, 1.45112, -0.237728, -0.07359 };

NaoInterfaceController::NaoInterfaceController(NaoInterfaceYarpInterface *yarpInterface, std::string ip, int port, std::vector<std::string> behaviourFromConfig, float initialVolume){
	_yarpInterface = yarpInterface;
	_behavioursHardCoded = vector<string>(behavioursHardCoded, end(behavioursHardCoded));
	_behaviourFromConfig = behaviourFromConfig;
	_Bconnected = false;
	_BusingSimulator = ip == "127.0.0.1";

	/* Try to connect to the Nao */
	boost::shared_ptr<AL::ALBroker> broker;
	try
	{
		broker = AL::ALBroker::createBroker("naoBroker", "0.0.0.0", 0, ip, port);
	}
	catch (...)
	{
		std::cerr << "-naoInterface- Fail to connect broker to: " << ip << ":" << port << std::endl;
		AL::ALBrokerManager::getInstance()->killAllBroker();
		AL::ALBrokerManager::kill();
		_module = NULL;
		return;
	}
	AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
	AL::ALBrokerManager::getInstance()->addBroker(broker);
	
	/* Associate the broker with a module to handle the functions */
	try
	{
		_module = (AL::ALModule::createModule<NaoInterfaceModule>(broker, "NaoInterfaceModule")).get();

	}
	catch (int e)
	{
		std::cout << "-naoInterface- ERROR: failed to set up module" << std::endl;
		AL::ALBrokerManager::getInstance()->killAllBroker();
		AL::ALBrokerManager::kill();
		_module = NULL;
		return;
	}
	_module->wakeUp();
	_module->stand();
	_module->setController(this);
	_module->setVolume(initialVolume);
	_Bconnected = true;
}


void NaoInterfaceController::enableRobot(bool enable, int stepId) {
	if (DEBUG) {
		cout << "-naoInterface- Enter enable "<< enable<<" with step " << stepId << endl;
	}
	if(!_Bconnected) {
		_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_ENABLE_, _FEEDBACK_NOT_CONNECTED_);
		return;
	}

	if (enable) {
		_module->wakeUp();
		_module->stand();
	}
	else
		_module->rest();
	_yarpInterface->sendToRobotMotorFeedback(stepId, enable ? _ACTION_ENABLE_ : _ACTION_DISABLE_, _FEEDBACK_SUCCESS_);
}

void NaoInterfaceController::closeHand(bool rightHand, bool close, int stepId) {
	if (DEBUG) {
		cout << "-naoInterface- Enter close "<<close<<" hand " << rightHand << " with step " << stepId << endl;
	}
	if (!_Bconnected) {
		_yarpInterface->sendToRobotMotorFeedback(stepId, close ? _ACTION_CLOSE_HAND_ : _ACTION_OPEN_HAND_, _FEEDBACK_NOT_CONNECTED_);
		return;
	}
	_module->closeHand(rightHand, close);
	_yarpInterface->sendToRobotMotorFeedback(stepId, close ? _ACTION_CLOSE_HAND_ : _ACTION_OPEN_HAND_,_FEEDBACK_SUCCESS_);
}

void NaoInterfaceController::getFromMoveHand(bool rightHand, float x, float y, float z, float roll, int stepId) {
	if (DEBUG) {
		cout << "-naoInterface- Enter moveHand " << rightHand << "  with step " << stepId << " and coordinates " << x << " " << y << " " << z << " " << roll << endl;
	}
	if (!_Bconnected) {
		_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_HAND_, _FEEDBACK_NOT_CONNECTED_);
		return;
	}
	_module->moveHand(rightHand, x, y, z, roll);
	_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_HAND_, _FEEDBACK_SUCCESS_);
}

void NaoInterfaceController::getFromMoveTorso(float x, float y, float z, int stepId) {

}

void NaoInterfaceController::say(std::string sentence, int stepId) {

	if (DEBUG) {
		Sleep(40);
		cout << "-naoInterface- Enter say " << sentence  << " with step " << stepId << endl;
	}
	if (!_Bconnected) {
		_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_SAY_, _FEEDBACK_NOT_CONNECTED_);
		return;
	}
	_module->say(sentence);
	_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_SAY_, _FEEDBACK_SUCCESS_);
}

void NaoInterfaceController::getFromMoveSequence(int stepId, int seqId) {
	if (DEBUG) {
		Sleep(40);
		cout << "-naoInterface- Enter move sequence with sequenceId " << seqId << endl;
	}
	if (!_Bconnected) {
		Sleep(40);
		_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_NOT_CONNECTED_, seqId);
		return;
	}
	if (seqId < _MOVE_LIMIT_) {
		if (seqId >= _behaviourFromConfig.size()) {
			if(DEBUG)
				cout << "-naoInterface- Behaviour not installed" << endl;
			Sleep(40);
			_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
		else {
			if(runBehaviour(stepId, _behaviourFromConfig[seqId]))		
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
			else
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
	}
	else {
		if (seqId == _MOVE_STAND_) {
			_module->stand();
			_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
		} 
		else if (seqId == _MOVE_HAPPY_FACE_ || seqId == _MOVE_SAD_FACE_ || seqId == _MOVE_ANGRY_FACE_
			|| seqId == _MOVE_FEAR_FACE_ || seqId == _MOVE_NEUTRAL_FACE_){ 
			_module->setFace(seqId);
			_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
		}
		else if (seqId == _MOVE_MICRO_MOTIONS_) {
			if(_BusingSimulator) {
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
				return;
			}
			_module->setMicromotions(1);
			_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
		}
		else if (seqId == _MOVE_STOP_MICRO_MOTIONS_) {
			if (_BusingSimulator) {
				Sleep(40);
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
				return;
			}
			_module->setMicromotions(0);
			_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
		}
		else if (seqId == _MOVE_WAITING_GRAB_) {
			vector<float> vecAngles(_anglesWaitingObject, _anglesWaitingObject + sizeof(_anglesWaitingObject) / sizeof(_anglesWaitingObject[0]));
			vector<string> vecBody(bodyList, bodyList + sizeof(bodyList) / sizeof(bodyList[0]));
			_module->takePosition(vecBody, vecAngles);
			_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
		}
		else if (seqId == _MOVE_WAITING_DROP_) {
			vector<float> vecAngles(_anglesDropingObject, _anglesDropingObject + sizeof(_anglesDropingObject) / sizeof(_anglesDropingObject[0]));
			vector<string> vecBody(bodyList, bodyList + sizeof(bodyList) / sizeof(bodyList[0]));
			_module->takePosition(vecBody, vecAngles);
			_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
		}
		else if (seqId == _MOVE_DAMAGE_AVOIDANCE_) {
			if (runBehaviour(stepId, "damageAvoidance"))
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_,seqId);
			else
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
		else if (seqId == _MOVE_GET_UP_) {
			if (runBehaviour(stepId, "gettingUp"))
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
			else
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
		else if (seqId == _MOVE_MICRO_EXPLAIN_3_) {
			if (runBehaviour(stepId, "Stand/Gestures/Explain_3"))
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
			else
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
		else if (seqId == _MOVE_MICRO_EXPLAIN_8_) {
			if (runBehaviour(stepId, "Stand/Gestures/Explain_8"))
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
			else
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
		else if (seqId == _MOVE_MICRO_THIS_) {
			if (runBehaviour(stepId, "Stand/Gestures/This_1"))
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
			else
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
		else if (seqId == _MOVE_MICRO_WHATSTHIS_2_) {
			if (runBehaviour(stepId, "Stand/Gestures/WhatSThis_2"))
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
			else
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
		else if (seqId == _MOVE_MICRO_WHATSTHIS_5_) {
			if (runBehaviour(stepId, "Stand/Gestures/WhatSThis_5"))
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
			else
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
		else if (seqId == _MOVE_MICRO_YOUKNOWWHAT_) {
			if (runBehaviour(stepId, "Stand/Gestures/YouKnowWhat_6"))
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
			else
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
		else if (seqId == _MOVE_GOOD_FB_IMITATION_OBJECT_) {
			if (runBehaviour(stepId, _behavioursHardCoded[0]))
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
			else
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
		else if (seqId == _MOVE_GOOD_FB_IMITATION_MOVE_) {
			if (runBehaviour(stepId, _behavioursHardCoded[0]))
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_, seqId);
			else
				_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
		}
		else
			_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_BEHAVIOUR_NOT_INSTALLED_, seqId);
	}
}

bool NaoInterfaceController::runBehaviour(int stepId, string behaviourName) {
	if (DEBUG)
		cout << "-naoInterface- Entering run behaviour: " << behaviourName << endl;
	if (_module->getBehaviourInstalled(behaviourName)) {
		_module->runBehaviour(behaviourName);
		return 1;
	}
	else {
		if (DEBUG)
			cout << "-naoInterface- NaoInterface - Error Behaviour " << behaviourName << " not installed" << endl;
		Sleep(50);
		return 0;
	}
}

void NaoInterfaceController::getFromMoveSequence(int stepId, int seqId, int arg1, int arg2, int arg3) {
	if (DEBUG) {
		cout << "-naoInterface- Enter move sequence with 5 parameters " << stepId << " " << seqId << " " << arg1 << " " << arg2 << " " << arg3 << endl;
	}
	if (!_Bconnected) {
		_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_NOT_CONNECTED_);
		return;
	}
	if (seqId == _MOVE_BLINKING_) {
		_module->blink(arg1, arg2, arg3);
		//_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_, _FEEDBACK_SUCCESS_);
	}
}

void NaoInterfaceController::moveHead(float x, float y, float z, float speed, int stepId) {
	if (DEBUG) {
		cout << "-naoInterface- Enter moveHead with step " << stepId << " and coordinates " << x << " " << y << " " << z << " " << speed << endl;
	}
	if (!_Bconnected) {
		_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_HEAD_, _FEEDBACK_NOT_CONNECTED_);
		return;
	}
	_module->moveHead(x, y, z, speed);
	_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_MOVE_HEAD_, _FEEDBACK_SUCCESS_);
}

void NaoInterfaceController::pointAt(vector<vector<float>> goals, bool rightArm, int stepId) {
	if (DEBUG) {
		cout << "-naoInterface- Enter pointAt " << rightArm << " with step " << stepId << " and many parameters" << endl;
		for (int x = 0; x < goals.size(); x++)
			for (int y = 0; y < goals[x].size(); y++)
				cout << goals[x][y] << endl;
	}
	if (!_Bconnected) {
		_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_POINT_AT_, _FEEDBACK_NOT_CONNECTED_);
		return;
	}
	unsigned nbrActions = goals.size();
	
	//_module->moveHand(rightArm,goals[0][0], goals[0][1], goals[0][2]);

	vector<float> headYaw;
	vector<float> headPitch;
	vector<float> shoulderRoll;
	vector<float> elbowRoll;
	vector<float> elbowYaw;
	vector<float> shoulderPitch;
	vector<float> wristYaw;
	vector<float> hand;
	vector<float> times;
	int X_OFFSET = 5;
	
	vector<int> distanceHeadArm;
	int lx=0;      //Distance Head-Arm 
	int ly;
	if(rightArm)     
		ly=-10;
	else
		ly=10;
	int lz=-5;

	for (unsigned i = 0; i < nbrActions; i++) {
		//Conversion from meters to centimeters and world frame to robot frame
		float x = goals[i][0]*100;
		float y = goals[i][1]*100;
		float z = goals[i][2]*100-Z_OFFSET;
		if (i > 0 && goals[i][3] < times[i - 1]) {
			_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_POINT_AT_, _FEEDBACK_IMPOSSIBLE_);
			return;
		}

		times.push_back(goals[i][3]);
		headYaw.push_back(atan2(y,x));
		headPitch.push_back(-atan2(z,x));
		shoulderRoll.push_back(atan2(y-ly,x-lx));
		elbowYaw.push_back(0.);
		wristYaw.push_back(0.);
      
		float armPitch = -atan2(z-lz,x-lx);

		//If movement not feasible without bending the elbow
		if((rightArm && shoulderRoll[i]>0)||((!rightArm) && shoulderRoll[i]<0)){
			shoulderRoll[i]=0.;
			vector<float> values = calculateShoulderPitchElbowRoll(x-lx+X_OFFSET, y-ly, z-lz, armPitch);
			shoulderPitch.push_back(values[0]);
			elbowRoll.push_back(values[1]);
		}   
		else{
			shoulderPitch.push_back(armPitch);
			elbowRoll.push_back(0.);
		}
		hand.push_back(0.8f);
	}

	   //Transform parameters
   vector<string> bodyParts;
   if(rightArm){
      const char* args[] = {"HeadYaw", "HeadPitch","RShoulderPitch","RShoulderRoll","RElbowYaw", "RElbowRoll", "RWristYaw", "RHand"};
      bodyParts = vector<string>(args, args+8);
   }
   else{
      const char* args[] = {"HeadYaw", "HeadPitch","LShoulderPitch","LShoulderRoll","LElbowYaw", "LElbowRoll", "LWristYaw", "LHand"};
      bodyParts = vector<string>(args, args+8);
   }
   
   if(times.size()==4){
		if (times[0] < 0.1) { times[0] = 0.1f; };
		if (times[1] < 0.2) { times[1] = 0.2f; };
		if (times[2] < 0.3) { times[2] = 0.3f; };
		if (times[3] < 0.4) { times[3] = 0.4f; };
   }
   vector<vector<float>> angleList;
   angleList.push_back(headYaw);
   angleList.push_back(headPitch);
   angleList.push_back(shoulderPitch);
   angleList.push_back(shoulderRoll);
   angleList.push_back(elbowYaw);
   angleList.push_back(elbowRoll);
   angleList.push_back(wristYaw);
   angleList.push_back(hand);

   vector<vector<float>> timeList(angleList.size(), times);

   _module->move(bodyParts, angleList, timeList);
   _yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_POINT_AT_, _FEEDBACK_SUCCESS_);
}

vector<float> NaoInterfaceController::calculateShoulderPitchElbowRoll(float x, float y, float z, float armPitch){
   int d = 10; //Distance shoulder elbow, should be global constant
   float dx = cos(armPitch)*d; //projection on x
   float dz = sin(armPitch)*d; //projection on z
   
   float tempArmPitch = -atan2(z-dz,x-dx);
   
   if(abs(armPitch-tempArmPitch)>0.01){
      tempArmPitch = armPitch-(armPitch-tempArmPitch)/2;
      return calculateShoulderPitchElbowRoll(x,y,z,tempArmPitch);
   }
   else{
      vector<float> toReturn;
      toReturn.push_back(armPitch);
      toReturn.push_back(atan2(y,x-dx));
      return toReturn;
   }
}

void NaoInterfaceController::sensors(std::string name, bool value) {
	_yarpInterface->sendToSensorFeedback(name, value);
}

void NaoInterfaceController::stopAllBehaviours(int stepId) {
	_module->stopAll();
	_yarpInterface->sendToRobotMotorFeedback(stepId, _ACTION_DISABLE_, _FEEDBACK_SUCCESS_);
}
