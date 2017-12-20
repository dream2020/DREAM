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

#include "systemGUIDriverYarpInterface.h"
#include "systemGUIDriver.h"
#include "systemGUIDriverController.h"

SystemGUIDriverYarpInterface::SystemGUIDriverYarpInterface(){

}

void SystemGUIDriverYarpInterface::init(SystemGUIDriverThread* yarpThread, SystemGUIDriverController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void SystemGUIDriverYarpInterface::getFromSelectedBySupervisor(Bottle data){
	/* Do Stuff: call methods from controller */
	Sleep(100);
	int stepID = data.get(0).asInt();
	int actionID = data.get(1).asInt();
	string arg = data.get(2).asString();
	_controller->processSelectedByTherapist(stepID, actionID,arg);
	cout << "systemGUIDriver: selectedYTherapist << " << stepID << " " << actionID << " " << arg << endl;
	return;
}

void SystemGUIDriverYarpInterface::getFromTherapistCommand(Bottle data){
	/* Do Stuff: call methods from controller */
	Sleep(100);
	stringstream ss;
	string sCommand = data.get(0).asString();
	ss << sCommand << " ";

	if (sCommand == "Create Session") {
		ss << data.get(1).asString() << " ";
		ss << data.get(2).asString();
	}
	else {
		double dblParameter = data.get(1).asDouble();
		ss << dblParameter;
		_controller->processTherapistCommand(sCommand, dblParameter);
	}
		
	cout << "systemGUIDriver: therapistCommand << " << ss.str() << endl;
	return;
}

void SystemGUIDriverYarpInterface::getFromUserID(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	Sleep(100);
	cout << "systemGUIDriver: userID << " << data[0] << endl;
	return;
}


//Probably need to change type to allow to be called by non yarp class
void SystemGUIDriverYarpInterface::sendToGetChildBehaviour(VectorOf<double> data){ 
	_yarpThread->sendToGetChildBehaviour(data);
	return;
}

void SystemGUIDriverYarpInterface::sendToGetChildPerformance(VectorOf<double> data){ 
	_yarpThread->sendToGetChildPerformance(data);
	return;
}

void SystemGUIDriverYarpInterface::sendToProposedToSupervisor(int stepId, int actionId, std::string arg1){
	Bottle data;
	data.addInt(stepId);
	data.addInt(actionId);
	if (actionId == _ACTION_RECEIVED_PERF_)
		data.addDouble(atof(arg1.c_str()));
	
	if (actionId == _ACTION_RECEIVED_PERF_|| actionId == _ACTION_POINT_IMAGE_ || actionId == _ACTION_LOOK_POINT_SAY_ || actionId == _ACTION_LOOK_POINT_ || actionId == _ACTION_LOOK_)
		data.addInt(atoi(arg1.c_str()));
	if (actionId == _ACTION_GO_NEXT_STEP_ || actionId == _ACTION_SAY_ || actionId == _ACTION_MOVE_)
		data.addString(arg1);
	cout << "systemGUIDriver: proposedToSUpervisor >> " << stepId  << " " << actionId <<endl;
	Sleep(100);
	_yarpThread->sendToProposedToSupervisor(data);
	return;
}

void SystemGUIDriverYarpInterface::sendToSmsSummary(string arg){ 
	Bottle data;
	data.addInt(0);
	data.addInt(0);
	data.addString(arg);
	data.addInt(0);
	data.addString("");
	data.addString("");
	data.addString("");
	data.addString("");
	cout << "systemGUIDriver: smsSummary >> " << arg <<endl;
	Sleep(100);
	_yarpThread->sendToSmsSummary(data);
	return;
}

