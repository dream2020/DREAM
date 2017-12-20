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
  * 13/06/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "userModel.h"
#include "userModelYarpInterface.h"

UserModelThread::UserModelThread(BufferedPort<VectorOf<int>>* userIDIn, BufferedPort<Bottle>* updatedDataIn, BufferedPort<Bottle>* userDataOut){
	/* assign the input and output ports */
	_userIDIn = userIDIn;
	_updatedDataIn = updatedDataIn;
	_userDataOut = userDataOut;
   
	_userIDInAnalyser = new UserIDInAnalyser();
	_updatedDataInAnalyser = new UpdatedDataInAnalyser();
   
	userIDIn->useCallback(*_userIDInAnalyser);
	updatedDataIn->useCallback(*_updatedDataInAnalyser);
}

void UserModelThread::setYarpInterface(UserModelYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_userIDInAnalyser->setUserModel(_yarpInterface);
	_updatedDataInAnalyser->setUserModel(_yarpInterface);
}

bool UserModelThread::threadInit(){
	/* initilize the thread */
	return true;
}

void UserModelThread::run(){

}

void UserModelThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void UserModelThread::sendToUserData(Bottle data){
	Bottle& container = _userDataOut->prepare();
	container = data;
	_userDataOut->write();
}


void UserIDInAnalyser::setUserModel(UserModelYarpInterface* yarpInterface){
   _userModelYarpInterface=yarpInterface;
}

void UpdatedDataInAnalyser::setUserModel(UserModelYarpInterface* yarpInterface){
   _userModelYarpInterface=yarpInterface;
}



void UserIDInAnalyser::onRead(VectorOf<int>& userIDInData){
   _userModelYarpInterface->getFromUserID(userIDInData);
}

void UpdatedDataInAnalyser::onRead(Bottle& updatedDataInData){
   _userModelYarpInterface->getFromUpdatedData(updatedDataInData);
}



