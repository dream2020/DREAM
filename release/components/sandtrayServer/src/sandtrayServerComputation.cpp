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
  * 22/06/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "sandtrayServer.h"
#include "sandtrayServerYarpInterface.h"

SandtrayServerThread::SandtrayServerThread(BufferedPort<Bottle>* sandtrayCommandIn, BufferedPort<Bottle>* sandtrayReturnOut){
	/* assign the input and output ports */
	_sandtrayCommandIn = sandtrayCommandIn;
	_sandtrayReturnOut = sandtrayReturnOut;

	_sandtrayCommandInAnalyser = new SandtrayCommandInAnalyser();
   
	sandtrayCommandIn->useCallback(*_sandtrayCommandInAnalyser);
}

void SandtrayServerThread::setYarpInterface(SandtrayServerYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_sandtrayCommandInAnalyser->setSandtrayServer(_yarpInterface);
}

bool SandtrayServerThread::threadInit(){
	/* initilize the thread */
	return true;
}

void SandtrayServerThread::run(){
	//while (isStopping() != true)	// the thread continues to run until isStopping() returns true 
	//{
	//
	//}
}

void SandtrayServerThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void SandtrayServerThread::sendToSandtrayReturn(Bottle data){
	Bottle& container = _sandtrayReturnOut->prepare();
	container = data;
	_sandtrayReturnOut->write();
}

void SandtrayCommandInAnalyser::setSandtrayServer(SandtrayServerYarpInterface* yarpInterface){
   _sandtrayServerYarpInterface=yarpInterface;
}

void SandtrayCommandInAnalyser::onRead(Bottle& sandtrayCommandInData){
   _sandtrayServerYarpInterface->getFromSandtrayCommand(sandtrayCommandInData);
}