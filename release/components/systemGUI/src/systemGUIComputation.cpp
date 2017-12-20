/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Hoang-Long Cao, VUB 
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
  * 22/06/2016 Version 1.0: ... (Hoang-Long Cao)
  * 25/01/2017 Version 2.0: ... (Hoang-Long Cao)
  */ 

#include "systemGUI.h"
#include "systemGUIYarpInterface.h"

SystemGUIThread::SystemGUIThread(BufferedPort<VectorOf<double>>* getChildBehaviourIn, BufferedPort<VectorOf<double>>* getChildPerformanceIn, BufferedPort<Bottle>* proposedToSupervisorIn, BufferedPort<Bottle>* smsSummaryIn, BufferedPort<Bottle>* selectedBySupervisorOut, BufferedPort<Bottle>* therapistCommandOut, BufferedPort<VectorOf<int>>* userIDOut, BufferedPort<Bottle>* interventionCommandOut){
	/* assign the input and output ports */
	_getChildBehaviourIn = getChildBehaviourIn;
	_getChildPerformanceIn = getChildPerformanceIn;
	_proposedToSupervisorIn = proposedToSupervisorIn;
	_smsSummaryIn = smsSummaryIn;
	_selectedBySupervisorOut = selectedBySupervisorOut;
	_therapistCommandOut = therapistCommandOut;
	_userIDOut = userIDOut;
	_interventionCommandOut= interventionCommandOut;
   
	_getChildBehaviourInAnalyser = new GetChildBehaviourInAnalyser();
	_getChildPerformanceInAnalyser = new GetChildPerformanceInAnalyser();
	_proposedToSupervisorInAnalyser = new ProposedToSupervisorInAnalyser();
	_smsSummaryInAnalyser = new SmsSummaryInAnalyser();
   
	getChildBehaviourIn->useCallback(*_getChildBehaviourInAnalyser);
	getChildPerformanceIn->useCallback(*_getChildPerformanceInAnalyser);
	proposedToSupervisorIn->useCallback(*_proposedToSupervisorInAnalyser);
	smsSummaryIn->useCallback(*_smsSummaryInAnalyser);
}

void SystemGUIThread::setYarpInterface(SystemGUIYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_getChildBehaviourInAnalyser->setSystemGUI(_yarpInterface);
	_getChildPerformanceInAnalyser->setSystemGUI(_yarpInterface);
	_proposedToSupervisorInAnalyser->setSystemGUI(_yarpInterface);
	_smsSummaryInAnalyser->setSystemGUI(_yarpInterface);
}

bool SystemGUIThread::threadInit(){
	/* initilize the thread */
	return true;
}

void SystemGUIThread::run(){

}

void SystemGUIThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void SystemGUIThread::sendToSelectedBySupervisor(Bottle data){
	Bottle& container = _selectedBySupervisorOut->prepare();
	container = data;
	_selectedBySupervisorOut->write();
}

void SystemGUIThread::sendToTherapistCommand(Bottle data){
	Bottle& container = _therapistCommandOut->prepare();
	container = data;
	_therapistCommandOut->write();
}

void SystemGUIThread::sendToUserID(VectorOf<int> data){
	VectorOf<int>& container = _userIDOut->prepare();
	container = data;
	_userIDOut->write();
}

void SystemGUIThread::sendToInterventionCommand(Bottle data){
	Bottle& container = _interventionCommandOut->prepare();
	container = data;
	_interventionCommandOut->write();
}

void GetChildBehaviourInAnalyser::setSystemGUI(SystemGUIYarpInterface* yarpInterface){
   _systemGUIYarpInterface=yarpInterface;
}

void GetChildPerformanceInAnalyser::setSystemGUI(SystemGUIYarpInterface* yarpInterface){
   _systemGUIYarpInterface=yarpInterface;
}

void ProposedToSupervisorInAnalyser::setSystemGUI(SystemGUIYarpInterface* yarpInterface){
   _systemGUIYarpInterface=yarpInterface;
}

void SmsSummaryInAnalyser::setSystemGUI(SystemGUIYarpInterface* yarpInterface){
   _systemGUIYarpInterface=yarpInterface;
}



void GetChildBehaviourInAnalyser::onRead(VectorOf<double>& getChildBehaviourInData){
   _systemGUIYarpInterface->getFromGetChildBehaviour(getChildBehaviourInData);
}

void GetChildPerformanceInAnalyser::onRead(VectorOf<double>& getChildPerformanceInData){
   _systemGUIYarpInterface->getFromGetChildPerformance(getChildPerformanceInData);
}

void ProposedToSupervisorInAnalyser::onRead(Bottle& proposedToSupervisorInData){
   _systemGUIYarpInterface->getFromProposedToSupervisor(proposedToSupervisorInData);
}

void SmsSummaryInAnalyser::onRead(Bottle& smsSummaryInData){
   _systemGUIYarpInterface->getFromSmsSummary(smsSummaryInData);
}



