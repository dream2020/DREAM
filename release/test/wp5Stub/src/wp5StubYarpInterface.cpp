/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Emmanuel Senft, Plymouth University 
 * Email:   emmanuel.senft@plymouth.ac.uk 
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
  * 20/09/2016 Version 1.0: ... (Emmanuel Senft)
  */ 

#include "wp5StubYarpInterface.h"
#include "wp5Stub.h"
#include "wp5StubController.h"

using namespace std;

Wp5StubYarpInterface::Wp5StubYarpInterface(){

}

void Wp5StubYarpInterface::init(Wp5StubThread* yarpThread, Wp5StubController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
   _bSpamOn = false;
}

void Wp5StubYarpInterface::getFromGetInterventionStatus(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	cout << "Received expected behav: " << data[2] << endl;
	_expectedBehav = data[2];

	return;
}

void Wp5StubYarpInterface::getFromInteractionEvent(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	cout << "Received interaction event " << data[0]<<" "<<data[1]<<endl;

	if (_expectedBehav == 1) {//good move
		if (data[0] == 0)
			sendToGetChildPerformance(1.);
		else if (data[0] == 1)
			sendToGetChildPerformance(0.);
	}

	if (_expectedBehav == 5) { //look righ
		if (data[0] == 2) {
			if (data[1] == 1)
				sendToGetChildPerformance(1.);
			else if (data[1] == 0)
				sendToGetChildPerformance(0.);
		}
	}

	if (_expectedBehav == 6) { //look left
		if (data[0] == 2) {
			if (data[1] == 0)
				sendToGetChildPerformance(1.);
			else if (data[1] == 1)
				sendToGetChildPerformance(0.);
		}
	}

	if (_expectedBehav == 2) { //Touch an image
		if (data[0] == 2) {
			sendToGetChildPerformance(1.);
		}
	}

	return;
}

void Wp5StubYarpInterface::getFromGetBody(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp5StubYarpInterface::getFromGetBodyPose(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp5StubYarpInterface::getFromGetEyeGaze(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
    _yarpThread->sendToEngagementFlag(data);

	return;
}

void Wp5StubYarpInterface::getFromIdentifyTrajectory(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp5StubYarpInterface::getFromIdentifyVoice(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp5StubYarpInterface::getFromIdentifyFaceExpression(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp5StubYarpInterface::getFromCheckMutualGaze(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	return;
}


//Probably need to change type to allow to be called by non yarp class
void Wp5StubYarpInterface::sendToEngagementFlag(VectorOf<double> data){
	
	return;
}

int Wp5StubYarpInterface::randi(int lo, int hi)
{
    int n = hi - lo + 1;
    int i = rand() % n;
    if (i < 0) i = -i;
    return lo + i;
}

void Wp5StubYarpInterface::sendToGetChildBehaviour(VectorOf<double> data){ 
	_yarpThread->sendToGetChildBehaviour(data);
	return;
}

void Wp5StubYarpInterface::sendToGetChildPerformance(float perf){
	VectorOf<double> data;
	data.push_back(perf);	
	_yarpThread->sendToGetChildPerformance(data);
	return;
}

void Wp5StubYarpInterface::sendToIdentifyTrajectory(VectorOf<int> data){ 
	_yarpThread->sendToIdentifyTrajectory(data);
	return;
}


