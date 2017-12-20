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
  * 23/06/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "sandtrayServerDriverController.h"
#include "sandtrayServerDriverYarpInterface.h"
#include "sandtrayMessages.h"

using namespace std;

namespace common_functions
{
	//methods used to split a string
	vector<string> split(const string &s, char delim, vector<string> &elems) {
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	vector<string> split(const string &s, char delim) {
		vector<string> elems;
		split(s, delim, elems);
		return elems;
	}

	//method used to remove a string from another string
	string removeString(string base, string toRemove) {
		string::size_type index = base.find(toRemove);
		if (index != string::npos) {
			base.erase(index, toRemove.length());
		}

		return base;
	}
}

using namespace common_functions;

SandtrayServerDriverController::SandtrayServerDriverController(SandtrayServerDriverYarpInterface *yarpInterface) {
	_yarpInterface = yarpInterface;
	_bPatternTest = false;
}

void SandtrayServerDriverController::testApp() {
	cout << "**********************sandtrayDrivertest started - 3 second delay between actions" << endl;
	cout << "**********************sandtrayDrivertest calling new game" << endl;
	newGame();
	Sleep(3000);
	cout << "**********************sandtrayDrivertest resetting game" << endl;
	resetGame();
	Sleep(3000);
	cout << "**********************sandtrayDrivertest turning on feedback" << endl;
	setFeedbackOn();
	Sleep(3000);
	cout << "**********************sandtrayDrivertest doing good move" << endl;
	goodMove();
	Sleep(6000);
	cout << "**********************sandtrayDrivertest turning off feedback" << endl;
	setFeedbackOff();
	Sleep(3000);
	cout << "**********************sandtrayDrivertest doing bad move" << endl;
	badMove();
	Sleep(6000);
	cout << "**********************sandtrayDrivertest doing space move" << endl;
	spaceMove();
	Sleep(6000);
	cout << "**********************sandtrayDrivertest disabling buttons" << endl;
	enableButtons(0);
	Sleep(3000);
	cout << "**********************sandtrayDrivertest enabling buttons" << endl;
	enableButtons(1);
	Sleep(3000);
	cout << "**********************sandtrayDrivertest specify level 3" << endl;
	specifyLevel(3);
	Sleep(3000);
	cout << "**********************sandtrayDrivertest getting number of images left" << endl;
	getImagesLeft();
	Sleep(3000);
	cout << "**********************sandtrayDrivertest getting library properties" << endl;
	getLibraryProperties();
	Sleep(3000);
	cout << "**********************sandtrayDrivertest getting category properties" << endl;
	getCategoryProperties();
	Sleep(3000);
	cout << "**********************sandtrayDrivertest locking images" << endl;
	lockImages();
	Sleep(3000);
	cout << "**********************sandtrayDrivertest unlocking images" << endl;
	unlockImages();
	Sleep(3000);
	cout << "**********************sandtrayDrivertest selecting image 3" << endl;
	turnTakeSelect(3);
	Sleep(6000);
	cout << "**********************sandtrayDrivertest quitting game" << endl;
	Sleep(1000);
	quitGame();
	Sleep(2000);
	cout << "**********************sandtrayDrivertest finished" << endl;
}

void SandtrayServerDriverController::testPattern() {
	cout << "**********************sandtrayDriverPatternTest started - 3 second delay between automated actions" << endl;
	cout << "**********************sandtrayDriverPatternTest calling new game" << endl;
	newGame();
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest resetting game" << endl;
	resetGame();
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest turning off feedback" << endl;
	setFeedbackOff();
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest disabling buttons" << endl;
	enableButtons(0);
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest enabling buttons" << endl;
	enableButtons(1);
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest getting number of images left" << endl;
	getImagesLeft();
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest getting library properties" << endl;
	getLibraryProperties();
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest getting category properties" << endl;
	getCategoryProperties();
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest locking images" << endl;
	lockImages();
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest unlocking images" << endl;
	unlockImages();
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest waiting for user move" << endl;
	_bPatternTest = true;		// flag for events - on will continue next function
}

void SandtrayServerDriverController::continuePatternTest() {
	_bPatternTest = false;		// flag for events - off to prevent another continue

	cout << "**********************sandtrayDriverPatternTest specify level 2" << endl;
	specifyLevel(2);
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest turning on feedback" << endl;
	setFeedbackOn();
	Sleep(3000);
	cout << "**********************sandtrayDriverPatternTest doing good move" << endl;
	goodMove();
	Sleep(6000);
	cout << "**********************sandtrayDriverPatternTest quitting game" << endl;
	Sleep(1000);
	quitGame();
	Sleep(2000);
	cout << "**********************sandtrayDriverPatternTest finished" << endl;
}

void SandtrayServerDriverController::verify() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_VERIFY_, "0");
}

void SandtrayServerDriverController::quitGame() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SHUTDOWN_, "0");
}

void SandtrayServerDriverController::fail() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_FAILURE_, "0");
}

void SandtrayServerDriverController::newGame() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_NEW_GAME_, "0");
}

void SandtrayServerDriverController::resetGame() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_RESET_BOARD_, "0");
}

void SandtrayServerDriverController::specifyLevel(int iLevel) {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SPECIFY_LEVEL_, to_string((_ULonglong)iLevel));
}

void SandtrayServerDriverController::enableButtons(int iEnable) {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SET_BUTTONS_, to_string((_ULonglong)iEnable));
}

void SandtrayServerDriverController::moveFinished() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_MOVE_FINISHED_, "0");
}

void SandtrayServerDriverController::getUserData() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_GET_USER_DATA_, "0");
}

void SandtrayServerDriverController::getImageCoordinates(int iImageId) {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_GET_IMAGE_COORDINATES_, to_string((_ULonglong)iImageId));
}

void SandtrayServerDriverController::getImagesLeft() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_GET_IMAGES_LEFT_, "0");
}

void SandtrayServerDriverController::getImageIdsCanMove() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_GET_ID_IMAGES_CAN_MOVE_, "0");
}

void SandtrayServerDriverController::getLastImageProperties() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_GET_LAST_IMAGE_PROPS_, "0");
}

void SandtrayServerDriverController::goodMove() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_MAKE_GOOD_MOVE_, "0");
}

void SandtrayServerDriverController::badMove() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_MAKE_BAD_MOVE_, "0");
}

void SandtrayServerDriverController::spaceMove() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_MAKE_SPACE_MOVE_, "0");
}

void SandtrayServerDriverController::getLibraryProperties() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_GET_LIBRARY_PROPS_, "0");
}

void SandtrayServerDriverController::getCategoryProperties() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_GET_CATEGORY_PROPS_, "0");
}

void SandtrayServerDriverController::lockImages() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_LOCK_IMAGES_, "0");
}

void SandtrayServerDriverController::unlockImages() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_UNLOCK_IMAGES_, "0");
}

void SandtrayServerDriverController::setFeedbackOn() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SET_FEEDBACK_ON_, "0");
}

void SandtrayServerDriverController::setFeedbackOff() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SET_FEEDBACK_OFF_, "0");
}

void SandtrayServerDriverController::setOneAtTime(int iOneTime) {
	if (iOneTime == 0) {
		_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SET_ONE_AT_TIME_, "false");
	}
	else {
		_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SET_ONE_AT_TIME_, "true");
	}
}

void SandtrayServerDriverController::getShownImgProps() {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_GET_SHOWN_IM_PROPS_, "0");
}

void SandtrayServerDriverController::setSpeed(int iSpeed) {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_SET_SPEED_, to_string((_ULonglong)iSpeed));
}

void SandtrayServerDriverController::turnTakeSelect(int iImageId) {
	_yarpInterface->sendToSandtrayCommand(_SANDTRAY_ROBOT_TURN_SELECTION_, to_string((_ULonglong)iImageId));
}

void SandtrayServerDriverController::processEvents(string str) {
	cout << "<<SandtrayServerDriver>> event received: " << str << endl;

	vector<string> inputs = split(str, ',');

	if (_bPatternTest) {
		if (inputs[0] == _SANDTRAY_PLAYER_GOOD_MOVE_ || inputs[0] == _SANDTRAY_PLAYER_BAD_MOVE_) {
			continuePatternTest();
		}
	}

	// this would be the receiving end in delib
	// we would filter these and send out to WP5 where relevant (e.g., good move, bad move)
}

void SandtrayServerDriverController::processReturns(string str) {
	cout << "<<SandtrayServerDriver>> return received: " << str << endl;

	vector<string> inputs = split(str, ',');

	// this would be the receiving end in the actuationSubsystem
	// it would take the move data and calculate robot arm bezier curves for _SANDTRAY_MOVE_DATA_
	// or just do the inverse kinematics for _SANDTRAY_ROBOT_TURN_LOCATION_
	// it would send the move to be done, and when the motorFeedback indicates that the initial 
	// arm lift is complete it would return _SANDTRAY_READY_ and simultaneously begin the bezier
	// move if necessary (sending the ready signal starts on-screen move, or selection)

	// send a READY command if move related after robot lifts arm
	if (inputs[0] == _SANDTRAY_MOVE_DATA_ || inputs[0] == _SANDTRAY_ROBOT_TURN_LOCATION_) {
		cout << "<<SandtrayServerDriver>> simulating time of robot arm lift" << endl;
		Sleep(1000);
		cout << "<<SandtrayServerDriver>> robot arm lift complete, starting point" << endl;
		_yarpInterface->sendToSandtrayCommand(_SANDTRAY_READY_, "0");
	}
	
}