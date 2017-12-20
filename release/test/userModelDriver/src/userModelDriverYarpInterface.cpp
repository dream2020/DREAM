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

#include "userModelDriverYarpInterface.h"
#include "userModelDriver.h"
#include "userModelDriverController.h"

UserModelDriverYarpInterface::UserModelDriverYarpInterface() {

}

void UserModelDriverYarpInterface::init(UserModelDriverThread* yarpThread, UserModelDriverController* controller) {
   _controller = controller;
   _yarpThread = yarpThread;   
}

void UserModelDriverYarpInterface::getFromUserData(Bottle dataIn) {
	UserDataLibrary userDataIn;

	userDataIn.iUserId = dataIn.get(0).asInt();
	userDataIn.sUserName = dataIn.get(1).asString();
	userDataIn.sGender = dataIn.get(2).asString();
	userDataIn.sPsych = dataIn.get(3).asString();
	userDataIn.sADOSTotal = dataIn.get(4).asString();
	userDataIn.sADOSComm = dataIn.get(5).asString(); 
	userDataIn.sADOSSocial = dataIn.get(6).asString();
	userDataIn.sADOSPlay = dataIn.get(7).asString();
	userDataIn.sADOSStereo = dataIn.get(8).asString();
	userDataIn.sADOSDiag = dataIn.get(9).asString();
	userDataIn.sADOSSeverity = dataIn.get(10).asString();
	userDataIn.sStereo1 = dataIn.get(11).asString();
	userDataIn.sStereo2 = dataIn.get(12).asString();
	userDataIn.sIQ = dataIn.get(13).asString();

	int iDataSize = dataIn.size();	// assigning this to an int prevents an unsigned mismatch warning

	for (int i = 14; i < iDataSize; i++) {
		Bottle * interaction = dataIn.get(i).asList();
		user_interaction_data_t newInteraction;
        newInteraction.m_iInteractionId = interaction->get(0).asInt();
        newInteraction.m_iScriptId = interaction->get(1).asInt();
        newInteraction.m_sPerformance = interaction->get(2).asString();
        newInteraction.m_iSessionId = interaction->get(3).asInt();
        newInteraction.m_sSessionName = interaction->get(4).asString();
        newInteraction.m_sSessionPartner = interaction->get(5).asString();
        newInteraction.m_sEngagement = interaction->get(6).asString();
        newInteraction.m_sTurnTake = interaction->get(7).asString();
        newInteraction.m_sWP5Perf = interaction->get(8).asString();
		newInteraction.m_sScriptTime = interaction->get(9).asString();
		newInteraction.m_sScriptEnd = interaction->get(10).asString();
		newInteraction.m_sWP5Turn = interaction->get(11).asString();

		userDataIn.m_user_interaction.push_back(newInteraction);
	}

	_controller->receiveUserData(userDataIn);
	return;
}

void UserModelDriverYarpInterface::sendToUserID(int iNewID) {
	VectorOf<int> iDataToSend;
	iDataToSend.clear();
	iDataToSend.push_back(iNewID);
	_yarpThread->sendToUserID(iDataToSend);
	return;
}

void UserModelDriverYarpInterface::sendToUpdatedData(user_interaction_data_t newInteraction) {
	Bottle dataToSend;

    dataToSend.addInt(newInteraction.m_iInteractionId);
    dataToSend.addInt(newInteraction.m_iScriptId);
    dataToSend.addString(newInteraction.m_sPerformance);
    dataToSend.addInt(newInteraction.m_iSessionId);
    dataToSend.addString(newInteraction.m_sSessionName);
    dataToSend.addString(newInteraction.m_sSessionPartner);
    dataToSend.addString(newInteraction.m_sEngagement);
    dataToSend.addString(newInteraction.m_sTurnTake);
    dataToSend.addString(newInteraction.m_sWP5Perf);
	dataToSend.addString(newInteraction.m_sScriptTime);
	dataToSend.addString(newInteraction.m_sScriptEnd);
	dataToSend.addString(newInteraction.m_sWP5Turn);
	
	_yarpThread->sendToUpdatedData(dataToSend);
	return;
}


