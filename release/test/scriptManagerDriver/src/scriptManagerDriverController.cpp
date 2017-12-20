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
  * 07/07/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "scriptManagerDriverController.h"
#include "scriptManagerDriverYarpInterface.h"
#include "scriptMessages.h"

using namespace std;

ScriptManagerDriverController::ScriptManagerDriverController(ScriptManagerDriverYarpInterface *yarpInterface){
	_yarpInterface = yarpInterface;
}

void ScriptManagerDriverController::testApp() {
	cout << "**********************scriptManagerDrivertest started - 1 second delay between actions" << endl;
	cout << "**********************scriptManagerDrivertest started - loading script 8" << endl;
	startScript(1);
}

void ScriptManagerDriverController::startScript(int iScriptId) {
	_yarpInterface->sendToSmsStartStop(iScriptId);
}

void ScriptManagerDriverController::nextStep() {
	_yarpInterface->sendToCommandSuccess(1);
}

void ScriptManagerDriverController::repeatStep() {
	_yarpInterface->sendToCommandSuccess(0);
}

void ScriptManagerDriverController::receivedFromScriptManager(int iStepID, int iDelibID, string sArgOut, int iArgOut1, int iArgOut2) {
	cout << "scriptManagerDriver received step ID: " << iStepID << endl;
	cout << "scriptManagerDriver received delib ID: " << iDelibID << endl;
	cout << "scriptManagerDriver received string arg: " << sArgOut << endl;
	cout << "scriptManagerDriver received int arg 1: " << iArgOut1 << endl;
	cout << "scriptManagerDriver received int arg 2: " << iArgOut2 << endl;

	// logic to call next step - assume it was fine here :)
	if (iStepID == _SCRIPT_FINISHED_) {
		cout << "**********************scriptManagerDrivertest - script finished - test complete" << endl;
	}
	else if (iStepID == _SCRIPT_TYPE_) {
		cout << "**********************scriptManagerDrivertest - script type:" << iArgOut1 << endl;
		nextStep();
	}
	else {
		Sleep(1000);
		cout << "**********************scriptManagerDrivertest - going to next step" << endl;
		nextStep();
	}
}