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

#include "systemGUIDriver.h"
#include "systemGUIDriverYarpInterface.h"

SystemGUIDriverThread::SystemGUIDriverThread(BufferedPort<Bottle>* selectedBySupervisorIn, BufferedPort<Bottle>* therapistCommandIn, BufferedPort<VectorOf<int>>* userIDIn, BufferedPort<VectorOf<double>>* getChildBehaviourOut, BufferedPort<VectorOf<double>>* getChildPerformanceOut, BufferedPort<Bottle>* proposedToSupervisorOut, BufferedPort<Bottle>* smsSummaryOut){
	/* assign the input and output ports */
	_selectedBySupervisorIn = selectedBySupervisorIn;
	_therapistCommandIn = therapistCommandIn;
	_userIDIn = userIDIn;
	_getChildBehaviourOut = getChildBehaviourOut;
	_getChildPerformanceOut = getChildPerformanceOut;
	_proposedToSupervisorOut = proposedToSupervisorOut;
	_smsSummaryOut = smsSummaryOut;
   
	_selectedBySupervisorInAnalyser = new SelectedBySupervisorInAnalyser();
	_therapistCommandInAnalyser = new TherapistCommandInAnalyser();
	_userIDInAnalyser = new UserIDInAnalyser();
   
	selectedBySupervisorIn->useCallback(*_selectedBySupervisorInAnalyser);
	therapistCommandIn->useCallback(*_therapistCommandInAnalyser);
	userIDIn->useCallback(*_userIDInAnalyser);
}

void SystemGUIDriverThread::setYarpInterface(SystemGUIDriverYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_selectedBySupervisorInAnalyser->setSystemGUIDriver(_yarpInterface);
	_therapistCommandInAnalyser->setSystemGUIDriver(_yarpInterface);
	_userIDInAnalyser->setSystemGUIDriver(_yarpInterface);
}

bool SystemGUIDriverThread::threadInit(){
	/* initilize the thread */
	return true;
}

void SystemGUIDriverThread::run(){

}

void SystemGUIDriverThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void SystemGUIDriverThread::sendToGetChildBehaviour(VectorOf<double> data){
	VectorOf<double>& container = _getChildBehaviourOut->prepare();
	container = data;
	_getChildBehaviourOut->write();
}

void SystemGUIDriverThread::sendToGetChildPerformance(VectorOf<double> data){
	VectorOf<double>& container = _getChildPerformanceOut->prepare();
	container = data;
	_getChildPerformanceOut->write();
}

void SystemGUIDriverThread::sendToProposedToSupervisor(Bottle data){
	Bottle& container = _proposedToSupervisorOut->prepare();
	container = data;
	_proposedToSupervisorOut->write();
}

void SystemGUIDriverThread::sendToSmsSummary(Bottle data){
	Bottle& container = _smsSummaryOut->prepare();
	container = data;
	_smsSummaryOut->write();
}

void SelectedBySupervisorInAnalyser::setSystemGUIDriver(SystemGUIDriverYarpInterface* yarpInterface){
   _systemGUIDriverYarpInterface=yarpInterface;
}

void TherapistCommandInAnalyser::setSystemGUIDriver(SystemGUIDriverYarpInterface* yarpInterface){
   _systemGUIDriverYarpInterface=yarpInterface;
}

void UserIDInAnalyser::setSystemGUIDriver(SystemGUIDriverYarpInterface* yarpInterface){
   _systemGUIDriverYarpInterface=yarpInterface;
}


void SelectedBySupervisorInAnalyser::onRead(Bottle& selectedBySupervisorInData){
   _systemGUIDriverYarpInterface->getFromSelectedBySupervisor(selectedBySupervisorInData);
}

void TherapistCommandInAnalyser::onRead(Bottle& therapistCommandInData){
   _systemGUIDriverYarpInterface->getFromTherapistCommand(therapistCommandInData);
}

void UserIDInAnalyser::onRead(VectorOf<int>& userIDInData){
   _systemGUIDriverYarpInterface->getFromUserID(userIDInData);
}

