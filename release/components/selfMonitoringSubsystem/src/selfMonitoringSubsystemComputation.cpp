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
  * 04/07/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "selfMonitoringSubsystem.h"
#include "selfMonitoringSubsystemYarpInterface.h"

SelfMonitoringSubsystemThread::SelfMonitoringSubsystemThread(BufferedPort<VectorOf<double>>* getChildBehaviourIn, BufferedPort<VectorOf<double>>* getChildPerformanceIn, BufferedPort<VectorOf<double>>* engagementFlagIn, BufferedPort<Bottle>* actionFeedbackIn, BufferedPort<VectorOf<int>>* fallingInterruptionIn, BufferedPort<VectorOf<int>>* getInterventionStatusIn, BufferedPort<Bottle>* deliberativeFeedbackIn, BufferedPort<Bottle>* sensorySummaryIn, BufferedPort<Bottle>* suggestedActionIn, BufferedPort<Bottle>* selectedBySupervisorIn, BufferedPort<Bottle>* therapistCommandIn, BufferedPort<Bottle>* userDataIn, BufferedPort<Bottle>* interventionCommandIn, BufferedPort<Bottle>* affectiveStateOut, BufferedPort<VectorOf<int>>* attentionSwitchOffOut, BufferedPort<VectorOf<int>>* reactionSwitchOffOut, BufferedPort<VectorOf<double>>* therapistGazeCommandOut, BufferedPort<Bottle>* selectedActionOut, BufferedPort<Bottle>* userDelibOut, BufferedPort<Bottle>* proposedToSupervisorOut, BufferedPort<Bottle>* smsSummaryOut, BufferedPort<VectorOf<int>>* startStopOut, BufferedPort<Bottle>* updatedDataOut, BufferedPort<VectorOf<int>>* commandSuccessOut){
	/* assign the input and output ports */
	_getChildBehaviourIn = getChildBehaviourIn;
	_getChildPerformanceIn = getChildPerformanceIn;
	_engagementFlagIn = engagementFlagIn;
	_actionFeedbackIn = actionFeedbackIn;
	_fallingInterruptionIn = fallingInterruptionIn;
	_getInterventionStatusIn = getInterventionStatusIn;
	_deliberativeFeedbackIn = deliberativeFeedbackIn;
    _interventionCommandIn = interventionCommandIn;
	_sensorySummaryIn = sensorySummaryIn;
	_suggestedActionIn = suggestedActionIn;
	_selectedBySupervisorIn = selectedBySupervisorIn;
	_therapistCommandIn = therapistCommandIn;
	_userDataIn = userDataIn;
	_affectiveStateOut = affectiveStateOut;
	_attentionSwitchOffOut = attentionSwitchOffOut;
	_reactionSwitchOffOut = reactionSwitchOffOut;
	_therapistGazeCommandOut = therapistGazeCommandOut;
	_selectedActionOut = selectedActionOut;
	_userDelibOut = userDelibOut;
	_proposedToSupervisorOut = proposedToSupervisorOut;
	_smsSummaryOut = smsSummaryOut;
	_startStopOut = startStopOut;
	_updatedDataOut = updatedDataOut;
	_commandSuccessOut = commandSuccessOut;
   
	_getChildBehaviourInAnalyser = new GetChildBehaviourInAnalyser();
	_getChildPerformanceInAnalyser = new GetChildPerformanceInAnalyser();
	_engagementFlagInAnalyser = new EngagementFlagInAnalyser();
	_actionFeedbackInAnalyser = new ActionFeedbackInAnalyser();
	_fallingInterruptionInAnalyser = new FallingInterruptionInAnalyser();
	_getInterventionStatusInAnalyser = new GetInterventionStatusInAnalyser();
	_deliberativeFeedbackInAnalyser = new DeliberativeFeedbackInAnalyser();
    _interventionCommandInAnalyser = new InterventionCommandInAnalyser();
	_sensorySummaryInAnalyser = new SensorySummaryInAnalyser();
	_suggestedActionInAnalyser = new SuggestedActionInAnalyser();
	_selectedBySupervisorInAnalyser = new SelectedBySupervisorInAnalyser();
	_therapistCommandInAnalyser = new TherapistCommandInAnalyser();
	_userDataInAnalyser = new UserDataInAnalyser();
   
	getChildBehaviourIn->useCallback(*_getChildBehaviourInAnalyser);
	getChildPerformanceIn->useCallback(*_getChildPerformanceInAnalyser);
	engagementFlagIn->useCallback(*_engagementFlagInAnalyser);
	actionFeedbackIn->useCallback(*_actionFeedbackInAnalyser);
    interventionCommandIn->useCallback(*_interventionCommandInAnalyser);
	fallingInterruptionIn->useCallback(*_fallingInterruptionInAnalyser);
	getInterventionStatusIn->useCallback(*_getInterventionStatusInAnalyser);
	deliberativeFeedbackIn->useCallback(*_deliberativeFeedbackInAnalyser);
	sensorySummaryIn->useCallback(*_sensorySummaryInAnalyser);
	suggestedActionIn->useCallback(*_suggestedActionInAnalyser);
	selectedBySupervisorIn->useCallback(*_selectedBySupervisorInAnalyser);
	therapistCommandIn->useCallback(*_therapistCommandInAnalyser);
	userDataIn->useCallback(*_userDataInAnalyser);
}

void SelfMonitoringSubsystemThread::setYarpInterface(SelfMonitoringSubsystemYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_getChildBehaviourInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
	_getChildPerformanceInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
	_engagementFlagInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
	_actionFeedbackInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
	_fallingInterruptionInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
	_getInterventionStatusInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
	_deliberativeFeedbackInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
	_sensorySummaryInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
	_suggestedActionInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
	_selectedBySupervisorInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
	_therapistCommandInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
	_userDataInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
    _interventionCommandInAnalyser->setSelfMonitoringSubsystem(_yarpInterface);
}

bool SelfMonitoringSubsystemThread::threadInit(){
	/* initilize the thread */
	return true;
}

void SelfMonitoringSubsystemThread::run(){

}

void SelfMonitoringSubsystemThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void SelfMonitoringSubsystemThread::sendToAffectiveState(Bottle data){
	Bottle& container = _affectiveStateOut->prepare();
	container = data;
	_affectiveStateOut->write();
}

void SelfMonitoringSubsystemThread::sendToAttentionSwitchOff(VectorOf<int> data){
	VectorOf<int>& container = _attentionSwitchOffOut->prepare();
	container = data;
	_attentionSwitchOffOut->write();
}

void SelfMonitoringSubsystemThread::sendToReactionSwitchOff(VectorOf<int> data){
	VectorOf<int>& container = _reactionSwitchOffOut->prepare();
	container = data;
	_reactionSwitchOffOut->write();
}

void SelfMonitoringSubsystemThread::sendToTherapistGazeCommand(VectorOf<double> data){
	VectorOf<double>& container = _therapistGazeCommandOut->prepare();
	container = data;
	_therapistGazeCommandOut->write();
}

void SelfMonitoringSubsystemThread::sendToSelectedAction(Bottle data){
	Bottle& container = _selectedActionOut->prepare();
	container = data;
	_selectedActionOut->write();
}

void SelfMonitoringSubsystemThread::sendToUserDelib(Bottle data){
	Bottle& container = _userDelibOut->prepare();
	container = data;
	_userDelibOut->write();
}

void SelfMonitoringSubsystemThread::sendToProposedToSupervisor(Bottle data){
	Bottle& container = _proposedToSupervisorOut->prepare();
	container = data;
	_proposedToSupervisorOut->write();
}

void SelfMonitoringSubsystemThread::sendToSmsSummary(Bottle data){
	Bottle& container = _smsSummaryOut->prepare();
	container = data;
	_smsSummaryOut->write();
}

void SelfMonitoringSubsystemThread::sendToStartStop(VectorOf<int> data){
	VectorOf<int>& container = _startStopOut->prepare();
	container = data;
	_startStopOut->write();
}

void SelfMonitoringSubsystemThread::sendToCommandSuccess(VectorOf<int> data) {
	VectorOf<int>& container = _commandSuccessOut->prepare();
	container = data;
	_commandSuccessOut->write();
}

void SelfMonitoringSubsystemThread::sendToUpdatedData(Bottle data){
	Bottle& container = _updatedDataOut->prepare();
	container = data;
	_updatedDataOut->write();
}


void GetChildBehaviourInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}

void GetChildPerformanceInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}

void EngagementFlagInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}

void ActionFeedbackInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}

void FallingInterruptionInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}

void GetInterventionStatusInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}

void DeliberativeFeedbackInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}

void SensorySummaryInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}

void SuggestedActionInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}

void SelectedBySupervisorInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}

void TherapistCommandInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}

void InterventionCommandInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface) {
    _selfMonitoringSubsystemYarpInterface = yarpInterface;
}

void UserDataInAnalyser::setSelfMonitoringSubsystem(SelfMonitoringSubsystemYarpInterface* yarpInterface){
   _selfMonitoringSubsystemYarpInterface=yarpInterface;
}



void GetChildBehaviourInAnalyser::onRead(VectorOf<double>& getChildBehaviourInData){
   _selfMonitoringSubsystemYarpInterface->getFromGetChildBehaviour(getChildBehaviourInData);
}

void GetChildPerformanceInAnalyser::onRead(VectorOf<double>& getChildPerformanceInData){
   _selfMonitoringSubsystemYarpInterface->getFromGetChildPerformance(getChildPerformanceInData);
}

void EngagementFlagInAnalyser::onRead(VectorOf<double>& engagementFlagInData){
   _selfMonitoringSubsystemYarpInterface->getFromEngagementFlag(engagementFlagInData);
}

void ActionFeedbackInAnalyser::onRead(Bottle& actionFeedbackInData){
   _selfMonitoringSubsystemYarpInterface->getFromActionFeedback(actionFeedbackInData);
}

void InterventionCommandInAnalyser::onRead(Bottle& interventionCommandInData) {
    _selfMonitoringSubsystemYarpInterface->getFromInterventionCommand(interventionCommandInData);
}

void FallingInterruptionInAnalyser::onRead(VectorOf<int>& fallingInterruptionInData){
   _selfMonitoringSubsystemYarpInterface->getFromFallingInterruption(fallingInterruptionInData);
}

void GetInterventionStatusInAnalyser::onRead(VectorOf<int>& getInterventionStatusInData){
   _selfMonitoringSubsystemYarpInterface->getFromGetInterventionStatus(getInterventionStatusInData);
}

void DeliberativeFeedbackInAnalyser::onRead(Bottle& deliberativeFeedbackInData){
   _selfMonitoringSubsystemYarpInterface->getFromDeliberativeFeedback(deliberativeFeedbackInData);
}

void SensorySummaryInAnalyser::onRead(Bottle& sensorySummaryInData){
   _selfMonitoringSubsystemYarpInterface->getFromSensorySummary(sensorySummaryInData);
}

void SuggestedActionInAnalyser::onRead(Bottle& suggestedActionInData){
   _selfMonitoringSubsystemYarpInterface->getFromSuggestedAction(suggestedActionInData);
}

void SelectedBySupervisorInAnalyser::onRead(Bottle& selectedBySupervisorInData){
   _selfMonitoringSubsystemYarpInterface->getFromSelectedBySupervisor(selectedBySupervisorInData);
}

void TherapistCommandInAnalyser::onRead(Bottle& therapistCommandInData){
   _selfMonitoringSubsystemYarpInterface->getFromTherapistCommand(therapistCommandInData);
}

void UserDataInAnalyser::onRead(Bottle& userDataInData){
   _selfMonitoringSubsystemYarpInterface->getFromUserData(userDataInData);
}



