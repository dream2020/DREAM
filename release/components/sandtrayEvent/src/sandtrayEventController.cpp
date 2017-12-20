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
  * 29/06/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "sandtrayEventController.h"
#include "sandtrayEventYarpInterface.h"
#include "sandtrayMessages.h"

using namespace std;

SandtrayEventController::SandtrayEventController(SandtrayEventYarpInterface *yarpInterface){
	_yarpInterface = yarpInterface;

	//for sandtrayEventThread
	sandtrayEventIn = new SandtrayEventIn(yarpInterface);
	sandtrayEventIn->start();
}

SandtrayEventController::~SandtrayEventController() {
}

void SandtrayEventController::stop() {
        sandtrayEventIn->stop();
        delete sandtrayEventIn;
}


/***************************************************************/
/*                     sandtrayEvent                           */
/***************************************************************/

SandtrayEventIn::SandtrayEventIn(SandtrayEventYarpInterface* yarpInterface) {
	_yarpInterface = yarpInterface;
	name = "sandtrayEvent";
	debug = true;
	isConnected = false;
    requestStopping = false;
	eventListenSocket = INVALID_SOCKET;
	eventClientSocket = INVALID_SOCKET;
	recvbuflen = DEFAULT_BUFLEN;
}

SandtrayEventIn::~SandtrayEventIn() {
    socketClose();
}

bool SandtrayEventIn::socketInit() {
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//resolve local address and port to be used by server
	iResult = getaddrinfo(NULL, DEFAULT_EVENT_PORT, &hints, &result);
	if (iResult != 0) {
		cout << "<<" << name << ">> error in address resolving: " << iResult << endl;
		WSACleanup();
		return false;
	}

	//create socket to listen for client connections
	eventListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (eventListenSocket == INVALID_SOCKET) {
		cout << "<<" << name << ">> error at socket: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	//setup tcp listening socket
	iResult = bind(eventListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		cout << "<<" << name << ">> bind failed with error: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		closesocket(eventListenSocket);
		WSACleanup();
		return false;
	}

	//assume setup complete - free memory
	freeaddrinfo(result);

	//listening on the socket
	if (listen(eventListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "<<" << name << ">> listen failed with error: " << WSAGetLastError() << endl;
		closesocket(eventListenSocket);
		WSACleanup();
		return false;
	}

	//accept a connection on the socket
	eventClientSocket = accept(eventListenSocket, NULL, NULL);
	if (eventClientSocket == INVALID_SOCKET) {
		cout << "<<" << name << ">> accept connection failed with error: " << WSAGetLastError() << endl;
		closesocket(eventListenSocket);
		WSACleanup();
		return false;
	}

	cout << "<<" << name << ">> client connected" << endl;

	//if this point reached, then all is well, and the data receiving/handling can start
	isConnected = true;
	return true;
}

bool SandtrayEventIn::socketClose() {
    cout << "<<" << name << ">> entered socketClose()" << endl;
    
    try {
		iResult = shutdown(eventClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			cout << "<<" << name << ">> shutdown failed with error: " << WSAGetLastError() << endl;
			closesocket(eventClientSocket);
			WSACleanup();
			return false;
		}
	}
	catch (int e) {
		cout << "<<" << name << ">> sandtray event shutdown failed: " << e << endl;
	}

	closesocket(eventClientSocket);
	WSACleanup();

	cout << "<<" << name << ">> socket closed" << endl;
	return true;
}

bool SandtrayEventIn::threadInit() {
	return true;
}

void SandtrayEventIn::threadRelease() {
	if (debug) {
		cout << "<<" << name << ">> thread released" << endl;
	}

	socketClose();
	isConnected = false;
    requestStopping = true;
}

void SandtrayEventIn::run() {
	//the main control loop for this thread
	// - waits for events from the GameEngine
	// - if there is, then send on Yarp port eventPortOut
    bool flag = false;
    flag = socketInit();
    isFirstLoop = true;

    cout << "<<" << name << ">> status of threadInit: " << flag << endl;

	if (debug) {
		cout << "<<" << name << ">> starting to run..." << endl;
	}

	//check that socket is set up and ready to connect
	if (isConnected != true) {
		cout << "<<" << name << ">> failure of socket setup with error: " << WSAGetLastError() << endl;
		cout << "<<" << name << ">> try again..." << endl;
		WSACleanup();
		return;
	}

	if (debug) {
		cout << "<<" << name << ">> entering while loop..." << endl;
	}

	string sInEvent = "";

	while (!requestStopping && !isStopping()) {
		//wait to receive an event from the GameEngine
		sInEvent = listenEvent();

		//when this happens, then repackage and send out as a bottle
		if (sInEvent != "") {
			_yarpInterface->sendToSandtrayEvent(sInEvent);
		}

		//reset string input
		sInEvent = "";
	}
}

string SandtrayEventIn::listenEvent() {
	string toReturn = "";

	//empty the receive buffer
	char _fill = '\0';
	memset(recvbuf, _fill, sizeof(recvbuf));

	//get data from the socket
	int sizeIn = -1;

	try {
		//cout << "receiving" << endl;
		sizeIn = recv(eventClientSocket, recvbuf, recvbuflen, 0);
		//cout << "end receiving; size: " << sizeIn << endl;
	}
	catch (int e) {
		cout << "<<" << name << ">> sandtray listen error: " << e << endl;
		return "";
	}
	catch (...) {
		cout << "<<" << name << ">> sandtray listen unhandled error" << endl;
	}

	//convert to a string if data...
	if (sizeIn > 0) {
		try {
			//recvbuf[sizeIn] = '\0';
			string temp(recvbuf);
			toReturn = temp;
			if (debug) {
				cout << "<<" << name << ">> data received: " << toReturn << endl;
			}
		}
		catch (int e) {
			cout << "<<" << name << ">> sandtray event listen conversion error: " << e << endl;
			toReturn = "";
		}
	}

	//empty the receive buffer
	char _fill2 = '\0';
	memset(recvbuf, _fill2, sizeof(recvbuf));

	if (isFirstLoop) {
		isFirstLoop = false;
	}

	//and then return for processing
	return toReturn;
}

//method used to remove a string from another string
string SandtrayEventIn::removeString(string base, string toRemove) {
	string::size_type index = base.find(toRemove);
	if (index != string::npos) {
		base.erase(index, toRemove.length());
	}

	return base;
}