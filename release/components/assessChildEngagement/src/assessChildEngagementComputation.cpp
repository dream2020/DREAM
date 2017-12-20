/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Serge Thill, University of Skövde 
 * Email:   serge.thill@his.se 
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
  * 08/11/2016 Version 1.0: ... (Serge Thill)
  */ 

#include "assessChildEngagement.h"
#include "assessChildEngagementYarpInterface.h"

AssessChildEngagementThread::AssessChildEngagementThread(BufferedPort<VectorOf<int> >* identifyFaceExpressionIn, BufferedPort<VectorOf<int> >* checkMutualGazeIn, BufferedPort<VectorOf<double> >* getBodyIn, BufferedPort<VectorOf<double> >* engagementFlagOut){
	/* assign the input and output ports */
	_identifyFaceExpressionIn = identifyFaceExpressionIn;
	_checkMutualGazeIn = checkMutualGazeIn;
	_getBodyIn = getBodyIn;
	_engagementFlagOut = engagementFlagOut;
   
	_identifyFaceExpressionInAnalyser = new IdentifyFaceExpressionInAnalyser();
	_checkMutualGazeInAnalyser = new CheckMutualGazeInAnalyser();
	_getBodyInAnalyser = new GetBodyInAnalyser();
   
	identifyFaceExpressionIn->useCallback(*_identifyFaceExpressionInAnalyser);
	checkMutualGazeIn->useCallback(*_checkMutualGazeInAnalyser);
	getBodyIn->useCallback(*_getBodyInAnalyser);
}

void AssessChildEngagementThread::setYarpInterface(AssessChildEngagementYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_identifyFaceExpressionInAnalyser->setAssessChildEngagement(_yarpInterface);
	_checkMutualGazeInAnalyser->setAssessChildEngagement(_yarpInterface);
	_getBodyInAnalyser->setAssessChildEngagement(_yarpInterface);
}

bool AssessChildEngagementThread::threadInit(){
	/* initilize the thread */
	return true;
}

void AssessChildEngagementThread::run(){

}

void AssessChildEngagementThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void AssessChildEngagementThread::sendToEngagementFlag(VectorOf<double> data){
	VectorOf<double>& container = _engagementFlagOut->prepare();
	container = data;
	_engagementFlagOut->write();
}


void IdentifyFaceExpressionInAnalyser::setAssessChildEngagement(AssessChildEngagementYarpInterface* yarpInterface){
   _assessChildEngagementYarpInterface=yarpInterface;
}

void CheckMutualGazeInAnalyser::setAssessChildEngagement(AssessChildEngagementYarpInterface* yarpInterface){
   _assessChildEngagementYarpInterface=yarpInterface;
}

void GetBodyInAnalyser::setAssessChildEngagement(AssessChildEngagementYarpInterface* yarpInterface){
   _assessChildEngagementYarpInterface=yarpInterface;
}



void IdentifyFaceExpressionInAnalyser::onRead(VectorOf<int>& identifyFaceExpressionInData){
   _assessChildEngagementYarpInterface->getFromIdentifyFaceExpression(identifyFaceExpressionInData);
}

void CheckMutualGazeInAnalyser::onRead(VectorOf<int>& checkMutualGazeInData){
   _assessChildEngagementYarpInterface->getFromCheckMutualGaze(checkMutualGazeInData);
}

void GetBodyInAnalyser::onRead(VectorOf<double>& getBodyInData){
   _assessChildEngagementYarpInterface->getFromGetBody(getBodyInData);
}



