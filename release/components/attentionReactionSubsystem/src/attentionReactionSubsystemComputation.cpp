/* 
 * Copyright (C) 2015 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Pablo Gomez, Vrije Universiteit Brussel
 * Email:   pablo.gomez.esteban@vub.ac.be
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
 * 10/08/16  First version validated (Pablo Gomez)
 */  
 

#include "attentionReactionSubsystem.h"
#include "attentionReactionYarpInterface.h"


AttentionReactionSubsystemThread::AttentionReactionSubsystemThread(
                                           BufferedPort<VectorOf<double>>    *getFacesIn,
                                           BufferedPort<VectorOf<double>>    *getSoundDirectionIn,
                                           BufferedPort<VectorOf<double>>    *attentionBiasIn, 
                                           BufferedPort<VectorOf<double>>    *therapistGazeCommandIn, 
                                           BufferedPort<VectorOf<int>>       *checkMutualGazeIn,
                                           BufferedPort<Bottle>			     *recognizeSpeechIn,
                                           BufferedPort<Bottle>              *affectiveStateIn, 
                                           BufferedPort<VectorOf<int>>	     *identifyFaceExpressionIn,  
                                           BufferedPort<Bottle>			     *robotSensorsIn,
                                           BufferedPort<VectorOf<int>>       *attentionSwitchOffIn,
                                           BufferedPort<VectorOf<int>>       *reactionSwitchOffIn,
                                           BufferedPort<Bottle>			     *actionFeedbackIn,
                                           BufferedPort<VectorOf<double>>	 *elicitedAttentionOut,
                                           BufferedPort<Bottle>			     *eyeBlinkingOut,
                                           BufferedPort<VectorOf<int>>       *fallingInterruptionOut,
                                           BufferedPort<VectorOf<int>>	     *fallingReactionOut,
                                           BufferedPort<VectorOf<int>>	     *fallingReactionSpeechOut,
                                           BufferedPort<VectorOf<int>>       *socialReactionOut,
                                           BufferedPort<VectorOf<int>>	     *socialReactionSpeechOut,
                                           BufferedPort<VectorOf<int>>       *socialFacialExpressionOut
                                           )
{
   getFacesPortIn			                    = getFacesIn;
   getSoundDirectionPortIn                      = getSoundDirectionIn;
   attentionBiasPortIn			                = attentionBiasIn;
   therapistGazeCommandPortIn	                = therapistGazeCommandIn;
   checkMutualGazePortIn	                    = checkMutualGazeIn;
   recognizeSpeechPortIn			            = recognizeSpeechIn;
   affectiveStatePortIn	                        = affectiveStateIn;
   identifyFaceExpressionPortIn	                = identifyFaceExpressionIn;
   robotSensorsPortIn                           = robotSensorsIn;
   attentionSwitchOffPortIn			            = attentionSwitchOffIn;
   reactionSwitchOffPortIn	                    = reactionSwitchOffIn;
   actionFeedbackPortIn	                        = actionFeedbackIn;
   elicitedAttentionPortOut	                    = elicitedAttentionOut;
   eyeBlinkingPortOut	                        = eyeBlinkingOut;
   fallingInterruptionPortOut				    = fallingInterruptionOut;
   fallingReactionPortOut				        = fallingReactionOut;
   fallingReactionSpeechPortOut			        = fallingReactionSpeechOut;
   socialReactionPortOut                        = socialReactionOut;
   socialReactionSpeechPortOut			        = socialReactionSpeechOut;
   socialFacialExpressionPortOut	            = socialFacialExpressionOut;  

   getFacesInAnalyser			                = new GetFacesInAnalyser();
   getSoundDirectionInAnalyser			        = new GetSoundDirectionInAnalyser();
   attentionBiasInAnalyser		                = new AttentionBiasInAnalyser();
   therapistGazeCommandInAnalyser			    = new TherapistGazeCommandInAnalyser();
   checkMutualGazeInAnalyser	                = new CheckMutualGazeInAnalyser();
   recognizeSpeechInAnalyser	                = new RecognizeSpeechInAnalyser();
   affectiveStateInAnalyser		                = new AffectiveStateInAnalyser();   
   identifyFaceExpressionInAnalyser             = new IdentifyFaceExpressionInAnalyser();
   robotSensorsInAnalyser	        		    = new RobotSensorsInAnalyser();
   attentionSwitchOffInAnalyser	                = new AttentionSwitchOffInAnalyser();
   reactionSwitchOffInAnalyser	                = new ReactionSwitchOffInAnalyser();
   actionFeedbackInAnalyser		                = new ActionFeedbackInAnalyser();

   getFacesIn->useCallback(*getFacesInAnalyser);
   getSoundDirectionIn->useCallback(*getSoundDirectionInAnalyser);
   attentionBiasIn->useCallback(*attentionBiasInAnalyser);
   therapistGazeCommandIn->useCallback(*therapistGazeCommandInAnalyser);
   checkMutualGazeIn->useCallback(*checkMutualGazeInAnalyser);
   recognizeSpeechIn->useCallback(*recognizeSpeechInAnalyser);
   affectiveStateIn->useCallback(*affectiveStateInAnalyser);
   identifyFaceExpressionIn->useCallback(*identifyFaceExpressionInAnalyser);
   robotSensorsIn->useCallback(*robotSensorsInAnalyser);
   attentionSwitchOffIn->useCallback(*attentionSwitchOffInAnalyser);
   reactionSwitchOffIn->useCallback(*reactionSwitchOffInAnalyser);
   actionFeedbackIn->useCallback(*actionFeedbackInAnalyser);
}
void AttentionReactionSubsystemThread::setYarpInterface(AttentionReactionYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	getFacesInAnalyser->setAttentionReactionInterface(_yarpInterface);
	getSoundDirectionInAnalyser->setAttentionReactionInterface(_yarpInterface);
    attentionBiasInAnalyser->setAttentionReactionInterface(_yarpInterface);
	therapistGazeCommandInAnalyser->setAttentionReactionInterface(_yarpInterface);
	checkMutualGazeInAnalyser->setAttentionReactionInterface(_yarpInterface);
    recognizeSpeechInAnalyser->setAttentionReactionInterface(_yarpInterface);
	affectiveStateInAnalyser->setAttentionReactionInterface(_yarpInterface);
	identifyFaceExpressionInAnalyser->setAttentionReactionInterface(_yarpInterface);    
    robotSensorsInAnalyser->setAttentionReactionInterface(_yarpInterface);
	attentionSwitchOffInAnalyser->setAttentionReactionInterface(_yarpInterface);
	reactionSwitchOffInAnalyser->setAttentionReactionInterface(_yarpInterface);
    actionFeedbackInAnalyser->setAttentionReactionInterface(_yarpInterface);
}
bool AttentionReactionSubsystemThread::threadInit() 
{
   /* initialize variables and create data-structures if needed */
   debug = false;

   return true;
}

void AttentionReactionSubsystemThread::run(){

}

void AttentionReactionSubsystemThread::threadRelease() 
{
   /* for example, delete dynamically created data-structures */
}

void AttentionReactionSubsystemThread::sendElicitedAttentionToActuation(double coordX, double coordY, double coordZ){
	
    VectorOf<double> &dataVector = elicitedAttentionPortOut->prepare();
	dataVector.resize(3);
		
	dataVector(0) = coordX;
	dataVector(1) = coordY;
    dataVector(2) = coordZ;
	elicitedAttentionPortOut->write();

    return;
}

void AttentionReactionSubsystemThread::sendSocialFacialExpressionToActuation(int socialFaceExpression){
	
    VectorOf<int> &dataVector = socialFacialExpressionPortOut->prepare();
	dataVector.resize(1);
	dataVector(0) = socialFaceExpression;    
	socialFacialExpressionPortOut->write();
    return;
}

void AttentionReactionSubsystemThread::sendSocialReactionSpeechToActuation(int socialSpeech){
	
    VectorOf<int> &socialSpeechVector = socialReactionSpeechPortOut->prepare();
	socialSpeechVector.resize(1);
	socialSpeechVector(0) = socialSpeech;
	socialReactionSpeechPortOut->write();
    return;
}

void AttentionReactionSubsystemThread::sendSocialReactionToActuation(int socialReaction){
	
    VectorOf<int> &dataAuxVector = socialReactionPortOut->prepare();
	dataAuxVector.resize(1);
	dataAuxVector(0) = socialReaction;
	socialReactionPortOut->write();
    return;
}

void AttentionReactionSubsystemThread::sendEyeBlinkingToActuation(Bottle eyeBlinking){
	
    Bottle &dataVector = eyeBlinkingPortOut->prepare();
	dataVector.clear();
	dataVector.addString(eyeBlinking.get(0).asString());
	dataVector.addString(eyeBlinking.get(1).asString());
	dataVector.addString(eyeBlinking.get(2).asString());
	eyeBlinkingPortOut->write(); 
    return;
}
void AttentionReactionSubsystemThread::sendFallingReactionToActuation(int fallingReaction){
	
    VectorOf<int> &reactionVector = fallingReactionPortOut->prepare();
	reactionVector.resize(1);
	reactionVector(0) = fallingReaction; 
	fallingReactionPortOut->write();
    return;
}

void AttentionReactionSubsystemThread::sendFallingReactionSpeechToActuation(int fallingSpeech){
	
    VectorOf<int> &dataVector = fallingReactionSpeechPortOut->prepare(); 
	dataVector.resize(1);
	dataVector(0) = fallingSpeech; 
	fallingReactionSpeechPortOut->write();
    return;
}

void AttentionReactionSubsystemThread::sendInterruptionToSelfMonitoring(int interruption){
	
    VectorOf<int> &boolVector = fallingInterruptionPortOut->prepare();
	boolVector.resize(1);
	boolVector(0) = interruption; 
	fallingInterruptionPortOut->write();
    return;
}

void GetFacesInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}

void GetSoundDirectionInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}

void AttentionBiasInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}

void TherapistGazeCommandInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}

void CheckMutualGazeInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}

void RecognizeSpeechInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}

void AffectiveStateInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}

void IdentifyFaceExpressionInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}

void RobotSensorsInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}
void AttentionSwitchOffInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}

void ReactionSwitchOffInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}

void ActionFeedbackInAnalyser::setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface){
   attentionReactionYarpInterface=yarpInterface;
}

void GetFacesInAnalyser::onRead(VectorOf<double>& getFacesInData){
   attentionReactionYarpInterface->getFromFaces(getFacesInData);
}

void GetSoundDirectionInAnalyser::onRead(VectorOf<double>& getSoundDirectionInData){
   attentionReactionYarpInterface->getFromSound(getSoundDirectionInData);
}

void AttentionBiasInAnalyser::onRead(VectorOf<double>& attentionBiasInData){
   attentionReactionYarpInterface->getFromAttentionBias(attentionBiasInData);
}

void TherapistGazeCommandInAnalyser::onRead(VectorOf<double>& therapistGazeCommandInData){
   attentionReactionYarpInterface->getFromGazeCommand(therapistGazeCommandInData);
}

void AttentionSwitchOffInAnalyser::onRead(VectorOf<int>& attentionSwitchOffInData){
   attentionReactionYarpInterface->getFromAttentionSwitchOff(attentionSwitchOffInData);
}


void IdentifyFaceExpressionInAnalyser::onRead(VectorOf<int>& identifyFaceExpressionInData){
   attentionReactionYarpInterface->getFromIdentifyFaceExpression(identifyFaceExpressionInData);
}

void RobotSensorsInAnalyser::onRead(Bottle& robotSensorsInData){
   attentionReactionYarpInterface->getFromRobotSensors(robotSensorsInData);
}
void ReactionSwitchOffInAnalyser::onRead(VectorOf<int>& reactionSwitchOffInData){
   attentionReactionYarpInterface->getFromReactionSwitchOff(reactionSwitchOffInData);
}

void CheckMutualGazeInAnalyser::onRead(VectorOf<int>& checkMutualGazeInData){
   attentionReactionYarpInterface->getFromCheckMutualGaze(checkMutualGazeInData);
}

void RecognizeSpeechInAnalyser::onRead(Bottle& recognizeSpeechInData){
   attentionReactionYarpInterface->getFromRecognizeSpeech(recognizeSpeechInData);
}
void AffectiveStateInAnalyser::onRead(Bottle& affectiveStateInData){
   attentionReactionYarpInterface->getFromAffectiveState(affectiveStateInData);
}

void ActionFeedbackInAnalyser::onRead(Bottle& actionFeedbackInData){
   attentionReactionYarpInterface->getFromActionFeedback(actionFeedbackInData);
}