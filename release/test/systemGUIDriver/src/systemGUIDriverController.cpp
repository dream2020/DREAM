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
  * 14/11/2016 Version 1.0: ... (Hoang-Long Cao)
  */ 

#include "systemGUIDriverController.h"
#include "systemGUIDriverYarpInterface.h"

using namespace std;

SystemGUIDriverController::SystemGUIDriverController(SystemGUIDriverYarpInterface *yarpInterface){
	_yarpInterface = yarpInterface;
	_isOnScript = false;
	_isStartScript = false;
	_isScriptRunning = false;
}

void SystemGUIDriverController::processTherapistCommand(string sCommand, double dblParameter){
	if (sCommand=="scriptload"){
		_isStartScript = true;
		_isOnScript = true;
		_isScriptRunning = true;
		_tActionLoop = new boost::thread(&SystemGUIDriverController::sendProposedActionLoop,this);
	}
	else if (sCommand=="stopscript"){
		_isStartScript = false;
		_isOnScript = false;
		_isScriptRunning = false;
	}

}

void SystemGUIDriverController::processSelectedByTherapist(int stepID, int actionID, string arg){
	if (stepID == -2){
		if (actionID == _ACTION_GO_OFF_SCRIPT_){
			_isOnScript = false;
			_isScriptRunning = false;
		}
		else if (actionID == _ACTION_BACK_ON_SCRIPT_){
			_isOnScript = true;
			_isScriptRunning = true;
		}
	}
	else _isScriptRunning = true;
}

void SystemGUIDriverController::sendProposedActionLoop(){
	vector<int> stepIDList, actionIDList;
	vector<string> arg1List;

	// Add test data for JA3 Intervention
	stepIDList.push_back(0);
	actionIDList.push_back(_ACTION_SAY_);
	arg1List.push_back("Now we will play a game");
	stepIDList.push_back(0);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(1);
	actionIDList.push_back(_ACTION_SAY_);
	arg1List.push_back("On this screen will appear two different pictures");
	stepIDList.push_back(1);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(2);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	stepIDList.push_back(3);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	stepIDList.push_back(4);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	stepIDList.push_back(5);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(6);
	actionIDList.push_back(_ACTION_SAY_);
	arg1List.push_back("Please pay attention to what am I looking");
	stepIDList.push_back(6);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(7);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(8);
	actionIDList.push_back(_ACTION_LOOK_);
	arg1List.push_back("0");
	stepIDList.push_back(8);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	
	stepIDList.push_back(9);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	stepIDList.push_back(10);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	stepIDList.push_back(11);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	
	stepIDList.push_back(12);
	actionIDList.push_back(_ACTION_LOOK_);
	arg1List.push_back("2");
	stepIDList.push_back(12);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(13);
	actionIDList.push_back(_ACTION_FEEDBACK_JA_GOOD_);
	arg1List.push_back("");
	stepIDList.push_back(13);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(14);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(15);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(16);
	actionIDList.push_back(_ACTION_SAY_);
	arg1List.push_back("Please pay attention to what am I looking");
	stepIDList.push_back(16);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(17);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(18);
	actionIDList.push_back(_ACTION_LOOK_);
	arg1List.push_back("0");
	stepIDList.push_back(18);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(19);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	stepIDList.push_back(20);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	stepIDList.push_back(21);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	
	stepIDList.push_back(22);
	actionIDList.push_back(_ACTION_LOOK_);
	arg1List.push_back("2");
	stepIDList.push_back(22);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(23);
	actionIDList.push_back(_ACTION_FEEDBACK_JA_BAD_);
	arg1List.push_back("");
	stepIDList.push_back(23);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(24);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	stepIDList.push_back(25);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(26);
	actionIDList.push_back(_ACTION_SAY_);
	arg1List.push_back("Please pay attention to what am I looking");
	stepIDList.push_back(26);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(27);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(28);
	actionIDList.push_back(_ACTION_LOOK_);
	arg1List.push_back("1");
	stepIDList.push_back(28);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(29);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	stepIDList.push_back(30);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	stepIDList.push_back(31);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(32);
	actionIDList.push_back(_ACTION_LOOK_);
	arg1List.push_back("2");
	stepIDList.push_back(32);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");

	stepIDList.push_back(33);
	actionIDList.push_back(_ACTION_FEEDBACK_JA_GOOD_);
	arg1List.push_back("");
	stepIDList.push_back(33);
	actionIDList.push_back(_ACTION_GO_NEXT_STEP_);
	arg1List.push_back("");
	
	vector<string> performance;
	performance.push_back("1");
	performance.push_back("1,0");
	performance.push_back("1,0,1");
	int performanceCount = 0;
	
	for (unsigned int i=0; i< stepIDList.size(); i++){
		if (_isStartScript==false) return;
		while (!_isScriptRunning) Sleep(50);
		_yarpInterface->sendToProposedToSupervisor(stepIDList[i], actionIDList[i], arg1List[i]);
		if (actionIDList[i]!=_ACTION_GO_NEXT_STEP_)	_isScriptRunning = false;
		
		if ((actionIDList[i] == _ACTION_FEEDBACK_JA_GOOD_)||(actionIDList[i] == _ACTION_FEEDBACK_JA_BAD_)) {
			Sleep(100);
			_yarpInterface->sendToSmsSummary(performance[performanceCount]);
			performanceCount++;
		}
		Sleep(1000);
	}
	performanceCount=0;
	_isScriptRunning = false;
	_isOnScript = false;
	_isStartScript = false;
}
