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
  * 18/08/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "userModelDriver.h"
#include "userModelDriverYarpInterface.h"

UserModelDriverThread::UserModelDriverThread(BufferedPort<Bottle>* userDataIn, BufferedPort<VectorOf<int>>* userIDOut, BufferedPort<Bottle>* updatedDataOut){
	/* assign the input and output ports */
	_userDataIn = userDataIn;
	_userIDOut = userIDOut;
	_updatedDataOut = updatedDataOut;
   
	_userDataInAnalyser = new UserDataInAnalyser();
   
	userDataIn->useCallback(*_userDataInAnalyser);
}

void UserModelDriverThread::setYarpInterface(UserModelDriverYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_userDataInAnalyser->setUserModelDriver(_yarpInterface);
}

bool UserModelDriverThread::threadInit(){
	/* initilize the thread */
	return true;
}

void UserModelDriverThread::run(){

}

void UserModelDriverThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void UserModelDriverThread::sendToUserID(VectorOf<int> data){
	VectorOf<int>& container = _userIDOut->prepare();
	container = data;
	_userIDOut->write();
}

void UserModelDriverThread::sendToUpdatedData(Bottle data){
	Bottle& container = _updatedDataOut->prepare();
	container = data;
	_updatedDataOut->write();
}


void UserDataInAnalyser::setUserModelDriver(UserModelDriverYarpInterface* yarpInterface){
   _userModelDriverYarpInterface=yarpInterface;
}



void UserDataInAnalyser::onRead(Bottle& userDataInData){
   _userModelDriverYarpInterface->getFromUserData(userDataInData);
}



