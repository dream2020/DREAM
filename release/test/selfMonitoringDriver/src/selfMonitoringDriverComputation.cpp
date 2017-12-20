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
  * 15/09/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "selfMonitoringDriver.h"
#include "selfMonitoringDriverYarpInterface.h"

SelfMonitoringDriverThread::SelfMonitoringDriverThread(BufferedPort<Bottle>* proposedToSupervisorIn, BufferedPort<Bottle>* smsSummaryIn, BufferedPort<Bottle>* updatedDataIn, BufferedPort<Bottle>* affectiveStateIn, BufferedPort<VectorOf<int>>* attentionSwitchOffIn, BufferedPort<VectorOf<int>>* reactionSwitchOffIn, BufferedPort<VectorOf<double>>* therapistGazeCommandIn, BufferedPort<Bottle>* selectedActionIn, BufferedPort<Bottle>* userDelibIn, BufferedPort<VectorOf<int>>* startStopIn, BufferedPort<VectorOf<int>>* commandSuccessIn, BufferedPort<VectorOf<double>>* getChildBehaviourOut, BufferedPort<VectorOf<double>>* getChildPerformanceOut, BufferedPort<VectorOf<double>>* engagementFlagOut, BufferedPort<Bottle>* selectedBySupervisorOut, BufferedPort<Bottle>* therapistCommandOut, BufferedPort<Bottle>* deliberativeFeedbackOut, BufferedPort<VectorOf<int>>* getInterventionStatusOut, BufferedPort<Bottle>* sensorySummaryOut, BufferedPort<Bottle>* suggestedActionOut, BufferedPort<Bottle>* actionFeedbackOut, BufferedPort<Bottle>* userDataOut, BufferedPort<VectorOf<int>>* fallingInterruptionOut, BufferedPort<Bottle>* interventionCommandOut){
	/* assign the input and output ports */
	_proposedToSupervisorIn = proposedToSupervisorIn;
	_smsSummaryIn = smsSummaryIn;
	_updatedDataIn = updatedDataIn;
	_affectiveStateIn = affectiveStateIn;
	_attentionSwitchOffIn = attentionSwitchOffIn;
	_reactionSwitchOffIn = reactionSwitchOffIn;
	_therapistGazeCommandIn = therapistGazeCommandIn;
	_selectedActionIn = selectedActionIn;
	_userDelibIn = userDelibIn;
	_startStopIn = startStopIn;
	_commandSuccessIn = commandSuccessIn;
	_getChildBehaviourOut = getChildBehaviourOut;
	_getChildPerformanceOut = getChildPerformanceOut;
	_engagementFlagOut = engagementFlagOut;
	_selectedBySupervisorOut = selectedBySupervisorOut;
	_therapistCommandOut = therapistCommandOut;
	_deliberativeFeedbackOut = deliberativeFeedbackOut;
	_getInterventionStatusOut = getInterventionStatusOut;
	_sensorySummaryOut = sensorySummaryOut;
	_suggestedActionOut = suggestedActionOut;
	_actionFeedbackOut = actionFeedbackOut;
	_userDataOut = userDataOut;
	_fallingInterruptionOut = fallingInterruptionOut;
    _interventionCommandOut = interventionCommandOut;
   
	_proposedToSupervisorInAnalyser = new ProposedToSupervisorInAnalyser();
	_smsSummaryInAnalyser = new SmsSummaryInAnalyser();
	_updatedDataInAnalyser = new UpdatedDataInAnalyser();
	_affectiveStateInAnalyser = new AffectiveStateInAnalyser();
	_attentionSwitchOffInAnalyser = new AttentionSwitchOffInAnalyser();
	_reactionSwitchOffInAnalyser = new ReactionSwitchOffInAnalyser();
	_therapistGazeCommandInAnalyser = new TherapistGazeCommandInAnalyser();
	_selectedActionInAnalyser = new SelectedActionInAnalyser();
	_userDelibInAnalyser = new UserDelibInAnalyser();
	_startStopInAnalyser = new StartStopInAnalyser();
	_commandSuccessInAnalyser = new CommandSuccessInAnalyser();
   
	proposedToSupervisorIn->useCallback(*_proposedToSupervisorInAnalyser);
	smsSummaryIn->useCallback(*_smsSummaryInAnalyser);
	updatedDataIn->useCallback(*_updatedDataInAnalyser);
	affectiveStateIn->useCallback(*_affectiveStateInAnalyser);
	attentionSwitchOffIn->useCallback(*_attentionSwitchOffInAnalyser);
	reactionSwitchOffIn->useCallback(*_reactionSwitchOffInAnalyser);
	therapistGazeCommandIn->useCallback(*_therapistGazeCommandInAnalyser);
	selectedActionIn->useCallback(*_selectedActionInAnalyser);
	userDelibIn->useCallback(*_userDelibInAnalyser);
	startStopIn->useCallback(*_startStopInAnalyser);
	commandSuccessIn->useCallback(*_commandSuccessInAnalyser);
}

void SelfMonitoringDriverThread::setYarpInterface(SelfMonitoringDriverYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_proposedToSupervisorInAnalyser->setSelfMonitoringDriver(_yarpInterface);
	_smsSummaryInAnalyser->setSelfMonitoringDriver(_yarpInterface);
	_updatedDataInAnalyser->setSelfMonitoringDriver(_yarpInterface);
	_affectiveStateInAnalyser->setSelfMonitoringDriver(_yarpInterface);
	_attentionSwitchOffInAnalyser->setSelfMonitoringDriver(_yarpInterface);
	_reactionSwitchOffInAnalyser->setSelfMonitoringDriver(_yarpInterface);
	_therapistGazeCommandInAnalyser->setSelfMonitoringDriver(_yarpInterface);
	_selectedActionInAnalyser->setSelfMonitoringDriver(_yarpInterface);
	_userDelibInAnalyser->setSelfMonitoringDriver(_yarpInterface);
	_startStopInAnalyser->setSelfMonitoringDriver(_yarpInterface);
	_commandSuccessInAnalyser->setSelfMonitoringDriver(_yarpInterface);
}

bool SelfMonitoringDriverThread::threadInit(){
	/* initilize the thread */
	return true;
}

void SelfMonitoringDriverThread::run(){

}

void SelfMonitoringDriverThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void SelfMonitoringDriverThread::sendToGetChildBehaviour(VectorOf<double> data){
	VectorOf<double>& container = _getChildBehaviourOut->prepare();
	container = data;
	_getChildBehaviourOut->write();
}

void SelfMonitoringDriverThread::sendToGetChildPerformance(VectorOf<double> data){
	VectorOf<double>& container = _getChildPerformanceOut->prepare();
	container = data;
	_getChildPerformanceOut->write();
}

void SelfMonitoringDriverThread::sendToEngagementFlag(VectorOf<double> data){
	VectorOf<double>& container = _engagementFlagOut->prepare();
	container = data;
	_engagementFlagOut->write();
}

void SelfMonitoringDriverThread::sendToSelectedBySupervisor(Bottle data){
	Bottle& container = _selectedBySupervisorOut->prepare();
	container = data;
	_selectedBySupervisorOut->write();
}

void SelfMonitoringDriverThread::sendToTherapistCommand(Bottle data){
	Bottle& container = _therapistCommandOut->prepare();
	container = data;
	_therapistCommandOut->write();
}

void SelfMonitoringDriverThread::sendToDeliberativeFeedback(Bottle data){
	Bottle& container = _deliberativeFeedbackOut->prepare();
	container = data;
	_deliberativeFeedbackOut->write();
}

void SelfMonitoringDriverThread::sendToGetInterventionStatus(VectorOf<int> data){
	VectorOf<int>& container = _getInterventionStatusOut->prepare();
	container = data;
	_getInterventionStatusOut->write();
}

void SelfMonitoringDriverThread::sendToSensorySummary(Bottle data){
	Bottle& container = _sensorySummaryOut->prepare();
	container = data;
	_sensorySummaryOut->write();
}

void SelfMonitoringDriverThread::sendToSuggestedAction(Bottle data){
	Bottle& container = _suggestedActionOut->prepare();
	container = data;
	_suggestedActionOut->write();
}

void SelfMonitoringDriverThread::sendToActionFeedback(Bottle data){
	Bottle& container = _actionFeedbackOut->prepare();
	container = data;
	_actionFeedbackOut->write();
}

void SelfMonitoringDriverThread::sendToUserData(Bottle data){
	Bottle& container = _userDataOut->prepare();
	container = data;
	_userDataOut->write();
}

void SelfMonitoringDriverThread::sendToFallingInterruption(VectorOf<int> data){
	VectorOf<int>& container = _fallingInterruptionOut->prepare();
	container = data;
	_fallingInterruptionOut->write();
}

void SelfMonitoringDriverThread::sendToInterventionCommand(Bottle data) {
    Bottle& container = _interventionCommandOut->prepare();
    container = data;
    _interventionCommandOut->write();
}

void ProposedToSupervisorInAnalyser::setSelfMonitoringDriver(SelfMonitoringDriverYarpInterface* yarpInterface){
   _selfMonitoringDriverYarpInterface=yarpInterface;
}

void SmsSummaryInAnalyser::setSelfMonitoringDriver(SelfMonitoringDriverYarpInterface* yarpInterface){
   _selfMonitoringDriverYarpInterface=yarpInterface;
}

void UpdatedDataInAnalyser::setSelfMonitoringDriver(SelfMonitoringDriverYarpInterface* yarpInterface){
   _selfMonitoringDriverYarpInterface=yarpInterface;
}

void AffectiveStateInAnalyser::setSelfMonitoringDriver(SelfMonitoringDriverYarpInterface* yarpInterface){
   _selfMonitoringDriverYarpInterface=yarpInterface;
}

void AttentionSwitchOffInAnalyser::setSelfMonitoringDriver(SelfMonitoringDriverYarpInterface* yarpInterface){
   _selfMonitoringDriverYarpInterface=yarpInterface;
}

void ReactionSwitchOffInAnalyser::setSelfMonitoringDriver(SelfMonitoringDriverYarpInterface* yarpInterface){
   _selfMonitoringDriverYarpInterface=yarpInterface;
}

void TherapistGazeCommandInAnalyser::setSelfMonitoringDriver(SelfMonitoringDriverYarpInterface* yarpInterface){
   _selfMonitoringDriverYarpInterface=yarpInterface;
}

void SelectedActionInAnalyser::setSelfMonitoringDriver(SelfMonitoringDriverYarpInterface* yarpInterface){
   _selfMonitoringDriverYarpInterface=yarpInterface;
}

void UserDelibInAnalyser::setSelfMonitoringDriver(SelfMonitoringDriverYarpInterface* yarpInterface){
   _selfMonitoringDriverYarpInterface=yarpInterface;
}

void StartStopInAnalyser::setSelfMonitoringDriver(SelfMonitoringDriverYarpInterface* yarpInterface){
   _selfMonitoringDriverYarpInterface=yarpInterface;
}

void CommandSuccessInAnalyser::setSelfMonitoringDriver(SelfMonitoringDriverYarpInterface* yarpInterface) {
	_selfMonitoringDriverYarpInterface = yarpInterface;
}



void ProposedToSupervisorInAnalyser::onRead(Bottle& proposedToSupervisorInData){
   _selfMonitoringDriverYarpInterface->getFromProposedToSupervisor(proposedToSupervisorInData);
}

void SmsSummaryInAnalyser::onRead(Bottle& smsSummaryInData){
   _selfMonitoringDriverYarpInterface->getFromSmsSummary(smsSummaryInData);
}

void UpdatedDataInAnalyser::onRead(Bottle& updatedDataInData){
   _selfMonitoringDriverYarpInterface->getFromUpdatedData(updatedDataInData);
}

void AffectiveStateInAnalyser::onRead(Bottle& affectiveStateInData){
   _selfMonitoringDriverYarpInterface->getFromAffectiveState(affectiveStateInData);
}

void AttentionSwitchOffInAnalyser::onRead(VectorOf<int>& attentionSwitchOffInData){
   _selfMonitoringDriverYarpInterface->getFromAttentionSwitchOff(attentionSwitchOffInData);
}

void ReactionSwitchOffInAnalyser::onRead(VectorOf<int>& reactionSwitchOffInData){
   _selfMonitoringDriverYarpInterface->getFromReactionSwitchOff(reactionSwitchOffInData);
}

void TherapistGazeCommandInAnalyser::onRead(VectorOf<double>& therapistGazeCommandInData){
   _selfMonitoringDriverYarpInterface->getFromTherapistGazeCommand(therapistGazeCommandInData);
}

void SelectedActionInAnalyser::onRead(Bottle& selectedActionInData){
   _selfMonitoringDriverYarpInterface->getFromSelectedAction(selectedActionInData);
}

void UserDelibInAnalyser::onRead(Bottle& userDelibInData){
   _selfMonitoringDriverYarpInterface->getFromUserDelib(userDelibInData);
}

void StartStopInAnalyser::onRead(VectorOf<int>& startStopInData){
   _selfMonitoringDriverYarpInterface->getFromStartStop(startStopInData);
}

void CommandSuccessInAnalyser::onRead(VectorOf<int>& commandSuccessInData) {
	_selfMonitoringDriverYarpInterface->getFromCommandSuccess(commandSuccessInData);
}
