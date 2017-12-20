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

#include "assessChildPerformanceYarpInterface.h"
#include "assessChildPerformance.h"
#include "assessChildPerformanceController.h"

using namespace std;

AssessChildPerformanceYarpInterface::AssessChildPerformanceYarpInterface(){

}

void AssessChildPerformanceYarpInterface::init(AssessChildPerformanceThread* yarpThread, AssessChildPerformanceController* controller){
   _controller = controller;
   _yarpThread = yarpThread;
   _dblExpected = -1;
}

void AssessChildPerformanceYarpInterface::getFromIdentifyVoice(VectorOf<int> data){
	// Needed to assess the presence of a voice.
	// NOTE: the value IDs the person 
	_controller->gotVoice(data[0]);

	return;
}

void AssessChildPerformanceYarpInterface::getFromGetEyeGaze(VectorOf<double> data){
	// Needed to assess eye gaze
	_controller->gotGaze(data[0], data[1], data[2]);
	return;
}

void AssessChildPerformanceYarpInterface::getFromGetBodyPose(VectorOf<double> data){
	// Needed to assess the presence of a voice.
	  
	double pose[30];
	
	for (int i = 0; i < 30; i++) {
		pose[i] = data[i];
	}  	   
	_controller->gotPose(pose);

	return;
}

void AssessChildPerformanceYarpInterface::getFromIdentifyTrajectory(VectorOf<double> data){
	/* This is where we receive the identified trajectories from sensory analysis.
	   Note that we expect either the first 6 or the last 4 elements to be 0 depending
	   on what was requested in the first place (see HIS-PORT meeting, Sept 2016) */

	double traj[12];
	
	for (int i = 0; i < 12; i++) {
		traj[i] = data[i];
	}   

	_controller->gotTrajectory(traj);   
	return;
}

void AssessChildPerformanceYarpInterface::getFromGetInterventionStatus(VectorOf<int> data){
	/* This is the event that triggers most of this component:
		- we receive a desired behaviour and a time window in which to expect it
		- we then set up the controller to check for that event */

	//A time window of -1 means there is no time limit, so re-interpret this as waiting for
	//24 hours
	if (data[4] == -1) data[4] = 1000*24*60*60;

	_dblExpected = data[2];

	//what we do depends on the type of event we expect
	switch (data[2]) {

		//Sandtray related behaviour expected
		case 1: // child perform good sandtray move 
		case 2: // child touch sandtray image 
		case 3: // child does not touch sandtray 
		case 4: // child touch robot-owned sandtray image

		_controller->checkSandtray(data[2], data[4]); //do we need to add image ID?
		break;

		//non-sensory analysis related movements
		case 5:  // look left
		case 6:  // look right
		case 7:  // point left
		case 8:  // point right
		case 9:  // no movement
		case 10: // child speaks
		_controller->checkOther(data[2], data[4]);
		break;

		//sensory-analysis related movements
		case 11: // hand wave (1)
		case 12: // hands covering eyes (2) 
		case 13: // hands on head (3)
		case 14: // fly (4)
		case 15: // drive car (5)
		case 16: // drink/smell (6)
		case 17: // new complex traj 1 (7)
		case 18: // new complex traj 2 (8)
		case 19: // new complex traj 3 (9)
		case 20: // new complex traj 4 (10)
		case 21: // new complex traj 5 (11)

		_controller->checkSA(data[2], data[4]);
		break;

		//nothing expected (explicitly so)  or unknown parameter - we don't do anything
		case 0:
		default:
		break;
	}
	return;
}

void AssessChildPerformanceYarpInterface::getFromInteractionEvent(VectorOf<int> data){
	/* Sandtray sends an event - we check if we were expecting this. */

	_controller->gotSandtray(data[0]);
	return;
}


//Unused in the current runtime system - IdentifyTrajectory from WP4 replaces this functionality
//Future versions of the system may implement this.
void AssessChildPerformanceYarpInterface::sendToGetChildBehaviour(VectorOf<double> data){ 
	_yarpThread->sendToGetChildBehaviour(data);
	return;
}

void AssessChildPerformanceYarpInterface::sendToIdentifyTrajectory(int behaviour, int window){ 
	/* This is where we request data from sensory analysis. */

	VectorOf<int> data;
	data.resize(2);

	data[0] = window;
	data[1] = behaviour;

	_yarpThread->sendToIdentifyTrajectory(data);
	return;
}

void AssessChildPerformanceYarpInterface::sendToGetChildPerformance(double performance){ 
	/* Our main output:
		-1: Failed to gather the required data in time (not yet agreed on and therefore not implemented)
		 0: Child did not perform according to expectations
		 1: Child did perform according to expectations
		 Graded values possible */

	VectorOf<double> data;
	data.resize(2);

	data[0] = performance;
	data[1] = _dblExpected;

	_yarpThread->sendToGetChildPerformance(data);

	return;
}


