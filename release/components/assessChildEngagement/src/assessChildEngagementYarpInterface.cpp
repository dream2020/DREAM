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

#include "assessChildEngagementYarpInterface.h"
#include "assessChildEngagement.h"
#include "assessChildEngagementController.h"

using namespace std;

AssessChildEngagementYarpInterface::AssessChildEngagementYarpInterface(){

}

void AssessChildEngagementYarpInterface::init(AssessChildEngagementThread* yarpThread, AssessChildEngagementController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void AssessChildEngagementYarpInterface::getFromIdentifyFaceExpression(VectorOf<int> data){
	
	// (used to be) 2 values - the first one IDs the expression (0 is happy); the second one whether the ID is valid (0 is no)
	int smiles;

	smiles = data[0];
	//smiles[1] = data[1];

	_controller->gotSmile(smiles);

	return;
}

void AssessChildEngagementYarpInterface::getFromCheckMutualGaze(VectorOf<int> data){

	// returns 0 or 1 depending on whether or not mutual gaze is detected.
	_controller->gotGaze(data[0]);
	return;
}

void AssessChildEngagementYarpInterface::getFromGetBody(VectorOf<double> data){

	// returns coordinates of the center of the body - this should be roughly 0 on the x axis
	// Interestingly, we seem to assume that the robot never moves - should this ever change, 
	// there will be a need to open a new port to get the robot's position as well.
	_controller->gotBody(data[0]);
	return;
}


//Probably need to change type to allow to be called by non yarp class
void AssessChildEngagementYarpInterface::sendToEngagementFlag(double engagement){ 

	VectorOf<double> data;
	data.resize(1);

	data[0] = engagement;	

	_yarpThread->sendToEngagementFlag(data);
	return;
}


