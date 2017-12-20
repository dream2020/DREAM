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
  * 23/06/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "sandtrayServerDriver.h"
#include "sandtrayServerDriverYarpInterface.h"

using namespace std;

SandtrayServerDriverThread::SandtrayServerDriverThread(BufferedPort<Bottle>* sandtrayEventIn, BufferedPort<Bottle>* sandtrayReturnIn, BufferedPort<Bottle>* sandtrayCommandOut){
	/* assign the input and output ports */
	_sandtrayEventIn = sandtrayEventIn;
	_sandtrayReturnIn = sandtrayReturnIn;
	_sandtrayCommandOut = sandtrayCommandOut;
   
	_sandtrayEventInAnalyser = new SandtrayEventInAnalyser();
	_sandtrayReturnInAnalyser = new SandtrayReturnInAnalyser();
   
	sandtrayEventIn->useCallback(*_sandtrayEventInAnalyser);
	sandtrayReturnIn->useCallback(*_sandtrayReturnInAnalyser);
}

void SandtrayServerDriverThread::setYarpInterface(SandtrayServerDriverYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_sandtrayEventInAnalyser->setSandtrayServerDriver(_yarpInterface);
	_sandtrayReturnInAnalyser->setSandtrayServerDriver(_yarpInterface);
}

bool SandtrayServerDriverThread::threadInit(){
	/* initilize the thread */
	return true;
}

void SandtrayServerDriverThread::run(){

}

void SandtrayServerDriverThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void SandtrayServerDriverThread::sendToSandtrayCommand(Bottle data) {
		Bottle& container = _sandtrayCommandOut->prepare();
		container = data;
		_sandtrayCommandOut->write();
}


void SandtrayEventInAnalyser::setSandtrayServerDriver(SandtrayServerDriverYarpInterface* yarpInterface) {
   _sandtrayServerDriverYarpInterface=yarpInterface;
}

void SandtrayReturnInAnalyser::setSandtrayServerDriver(SandtrayServerDriverYarpInterface* yarpInterface) {
   _sandtrayServerDriverYarpInterface=yarpInterface;
}



void SandtrayEventInAnalyser::onRead(Bottle& sandtrayEventInData) {
   _sandtrayServerDriverYarpInterface->getFromSandtrayEvent(sandtrayEventInData);
}

void SandtrayReturnInAnalyser::onRead(Bottle& sandtrayReturnInData) {
   _sandtrayServerDriverYarpInterface->getFromSandtrayReturn(sandtrayReturnInData);
}
