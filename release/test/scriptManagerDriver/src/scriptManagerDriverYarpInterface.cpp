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

#include "scriptManagerDriverYarpInterface.h"
#include "scriptManagerDriver.h"
#include "scriptManagerDriverController.h"
#include "scriptMessages.h"

using namespace std;

ScriptManagerDriverYarpInterface::ScriptManagerDriverYarpInterface(){

}

void ScriptManagerDriverYarpInterface::init(ScriptManagerDriverThread* yarpThread, ScriptManagerDriverController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void ScriptManagerDriverYarpInterface::getFromInterventionCommand(Bottle data){
	string sArgOut = "";
	int iArgOut1 = -1;
	int iArgOut2 = -1;
	
	int iStepId = data.get(0).asInt();
	int iDelibId = data.get(1).asInt();
	
	if (iDelibId == _SCRIPT_SAY_ || iDelibId == _SCRIPT_MOVE_) {
		sArgOut = data.get(2).asString();
	}
	else if (iDelibId == _SCRIPT_WAIT_FOR_PERF_) {
		iArgOut1 = data.get(2).asInt();
		iArgOut2 = data.get(3).asInt();
	}
	else if (iDelibId == _SCRIPT_WAIT_ || iDelibId == _SCRIPT_POINT_IMAGE_ || iDelibId == _SCRIPT_LOOK_POINT_SAY_ || iDelibId == _SCRIPT_LOOK_POINT_ || iDelibId == _SCRIPT_LOOK_ || iDelibId == _SCRIPT_SANDTRAY_LOAD_LIBRARY_ || iDelibId ==_SCRIPT_TYPE_) {
		iArgOut1 = data.get(2).asInt();
	}

	_controller->receivedFromScriptManager(iStepId, iDelibId, sArgOut, iArgOut1, iArgOut2);
	return;
}

// item[0] = delivery system: 0=SMS | 1=delib
// item[1] = script id
// item[2] = start/stop (1/0)
void ScriptManagerDriverYarpInterface::sendToSmsStartStop(int iScriptId){
	VectorOf<int> data;
	data.push_back(iScriptId);		// script number
	data.push_back(1);				// this will start the script too (put as 0 to just load, not start)
	 _yarpThread->sendToSmsStartStop(data);
	return;
}

void ScriptManagerDriverYarpInterface::sendToDelibStartStop(int iStartStop){ 
	VectorOf<int> data;
	data.push_back(-1);				// dummy - not used from delib
	data.push_back(iStartStop);		// 1 = go, 0 = stop
	_yarpThread->sendToDelibStartStop(data);
	return;
}

void ScriptManagerDriverYarpInterface::sendToCommandSuccess(int iSuccess){ 
	VectorOf<int> data;
	data.push_back(iSuccess);
	_yarpThread->sendToCommandSuccess(data);
	return;
}


