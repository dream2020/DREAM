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

#include "userModelYarpInterface.h"
#include "userModel.h"
#include "userModelController.h"

UserModelYarpInterface::UserModelYarpInterface(){

}

void UserModelYarpInterface::init(UserModelThread* yarpThread, UserModelController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void UserModelYarpInterface::getFromUserID(VectorOf<int> data){
	// here we receive the selected user ID from the GUI
	// send to controller to check, load user data and send to SMS
	_controller->newUserID(data[0]);
	return;
}

void UserModelYarpInterface::getFromUpdatedData(Bottle data){
	// inverse unpack into a userDataLibrary object
	user_interaction_data_t newInteraction;

	// expecting a bottle with details from a new interaction
	newInteraction.m_iInteractionId = data.get(0).asInt();
	newInteraction.m_iScriptId = data.get(1).asInt();
	newInteraction.m_sPerformance = data.get(2).asString();
    newInteraction.m_iSessionId = data.get(3).asInt();
    newInteraction.m_sSessionName = data.get(4).asString();
    newInteraction.m_sSessionPartner = data.get(5).asString();
    newInteraction.m_sEngagement = data.get(6).asString();
    newInteraction.m_sTurnTake = data.get(7).asString();
    newInteraction.m_sWP5Perf = data.get(8).asString();
	newInteraction.m_sScriptTime = data.get(9).asString();
	newInteraction.m_sScriptEnd = data.get(10).asString();
	newInteraction.m_sWP5Turn = data.get(11).asString();

	bool bSuccess = _controller->saveNewInteractionToFile(newInteraction);
	if (!bSuccess) {
		cout << "<<<userModel>>> error: user data might not be saved!" << endl;
	}

	return;
}


void UserModelYarpInterface::sendToUserData(UserDataLibrary userDataIn){
	Bottle dataToSend;
	
	dataToSend.addInt(userDataIn.iUserId);
	dataToSend.addString(userDataIn.sUserName);
	dataToSend.addString(userDataIn.sGender);
	dataToSend.addString(userDataIn.sPsych);
	dataToSend.addString(userDataIn.sADOSTotal);
	dataToSend.addString(userDataIn.sADOSComm);
	dataToSend.addString(userDataIn.sADOSSocial);
	dataToSend.addString(userDataIn.sADOSPlay);
	dataToSend.addString(userDataIn.sADOSStereo);
	dataToSend.addString(userDataIn.sADOSDiag);
	dataToSend.addString(userDataIn.sADOSSeverity);
	dataToSend.addString(userDataIn.sStereo1);
	dataToSend.addString(userDataIn.sStereo2);
	dataToSend.addString(userDataIn.sIQ);

	for (unsigned int i = 0; i < userDataIn.m_user_interaction.size(); i++) {
		Bottle newInteraction;
		newInteraction.addInt(userDataIn.m_user_interaction[i].m_iInteractionId);
        newInteraction.addInt(userDataIn.m_user_interaction[i].m_iScriptId);
        newInteraction.addString(userDataIn.m_user_interaction[i].m_sPerformance);
        newInteraction.addInt(userDataIn.m_user_interaction[i].m_iSessionId);
        newInteraction.addString(userDataIn.m_user_interaction[i].m_sSessionName);
        newInteraction.addString(userDataIn.m_user_interaction[i].m_sSessionPartner);
        newInteraction.addString(userDataIn.m_user_interaction[i].m_sEngagement);
        newInteraction.addString(userDataIn.m_user_interaction[i].m_sTurnTake);
        newInteraction.addString(userDataIn.m_user_interaction[i].m_sWP5Perf);
		newInteraction.addString(userDataIn.m_user_interaction[i].m_sScriptTime);
		newInteraction.addString(userDataIn.m_user_interaction[i].m_sScriptEnd);
		newInteraction.addString(userDataIn.m_user_interaction[i].m_sWP5Turn);
		dataToSend.addList() = newInteraction;
	}

	_yarpThread->sendToUserData(dataToSend);
	return;
}

