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
  * 22/06/2016 Version 1.0: created shell (James Kennedy)
  * 24/06/2016 Version 1.1: copied in code from PEB/ES (James Kennedy)
  * 28/06/2016 Version 1.2: added functionality (James Kennedy)
  */

#include "sandtrayServerController.h"
#include "sandtrayServerYarpInterface.h"
#include "sandtrayMessages.h"

using namespace std;

namespace common_functions
{
	//methods used to split a string
	vector<string> split(const string &s, char delim, vector<string> &elems) {
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	vector<string> split(const string &s, char delim) {
		vector<string> elems;
		split(s, delim, elems);
		return elems;
	}

	//method used to remove a string from another string
	string removeString(string base, string toRemove) {
		string::size_type index = base.find(toRemove);
		if (index != string::npos) {
			base.erase(index, toRemove.length());
		}

		return base;
	}
}

using namespace common_functions;

SandtrayServerController::SandtrayServerController(SandtrayServerYarpInterface *yarpInterface, float distanceRobotScreenX, float distanceRobotScreenZ) {
	_yarpInterface = yarpInterface;
	_commandRequestBusy = false;		//not currently busy

	//initialize Winsock
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "WSAStartup failed:" << iResult << endl;
	}
	
	//for sandtrayControlThread
	sandtrayControl = new SandtrayControl(yarpInterface, this);
	sandtrayControl->init();
	sandtrayControl->run();
}

SandtrayServerController::~SandtrayServerController() {

}

void SandtrayServerController::getFromCommands(string str) {
	cout << "sandtrayServer - received from commands " << str << endl;
	vector<string> inputs = split(str, ',');

	//on connection, the sandtray sends "i am a touchscreen" both to command and event, and it is not processed by the command
	//part, so the first time a command is asked by the controller it needs to be removed
	inputs[0] = removeString(inputs[0], _SANDTRAY_GREET_CATEGORY_ + ",");
	inputs[0] = removeString(inputs[0], _SANDTRAY_GREET_PATTERN_ + ",");

	processInput(inputs);

	return;
}

string SandtrayServerController::processInput(vector<string> dataIn) {
	if (DEBUG) {
		cout << "<<sandtrayControl>> command in: " << dataIn[0] << "; param: " << dataIn[1] << endl;
	}

	//first check we have data
	if (dataIn[0] == "") {
		return "";
	}

	//if busy then dont do anything
	if (_commandRequestBusy == true) {
		return "";
	}

	//from now on don't accept new commands until this one done
	_commandRequestBusy = true;		
	string sResponse = "";

	//switch on the command in and formulate response
	if (dataIn[0] == _SANDTRAY_VERIFY_) {
		sResponse = _SANDTRAY_VERIFY_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_GREET_PATTERN_) {
		sResponse = _SANDTRAY_CONFIRM_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_GREET_CATEGORY_) {
		sResponse = _SANDTRAY_CONFIRM_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_SHUTDOWN_) {
		sResponse = _SANDTRAY_SHUTDOWN_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_FAILURE_) {
		sResponse = _SANDTRAY_CONFIRM_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_NEW_GAME_) {
		sResponse = _SANDTRAY_NEW_GAME_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_RESET_BOARD_) {
		sResponse = _SANDTRAY_RESET_BOARD_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_SPECIFY_LEVEL_) {
		sResponse = _SANDTRAY_SPECIFY_LEVEL_ + "," + dataIn[1];
	}
	else if (dataIn[0] == _SANDTRAY_SET_BUTTONS_) {
		sResponse = _SANDTRAY_SET_BUTTONS_ + "," + dataIn[1];
	}
	else if (dataIn[0] == _SANDTRAY_MOVE_FINISHED_) {
		sResponse = _SANDTRAY_MOVE_FINISHED_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_GET_USER_DATA_) {
		sResponse = _SANDTRAY_GET_USER_DATA_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_GET_IMAGE_COORDINATES_) {
		sResponse = _SANDTRAY_GET_IMAGE_COORDINATES_ + "," + dataIn[1];
	}
	else if (dataIn[0] == _SANDTRAY_GET_IMAGES_LEFT_) {
		sResponse = _SANDTRAY_GET_IMAGES_LEFT_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_GET_ID_IMAGES_CAN_MOVE_) {
		sResponse = _SANDTRAY_GET_ID_IMAGES_CAN_MOVE_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_GET_LAST_IMAGE_PROPS_) {
		sResponse = _SANDTRAY_GET_LAST_IMAGE_PROPS_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_GET_LIBRARY_PROPS_) {
		sResponse = _SANDTRAY_GET_LIBRARY_PROPS_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_GET_CATEGORY_PROPS_) {
		sResponse = _SANDTRAY_GET_CATEGORY_PROPS_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_LOCK_IMAGES_) {
		sResponse = _SANDTRAY_LOCK_IMAGES_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_UNLOCK_IMAGES_) {
		sResponse = _SANDTRAY_UNLOCK_IMAGES_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_SET_ONE_AT_TIME_) {
		sResponse = _SANDTRAY_SET_ONE_AT_TIME_ + "," + dataIn[1];
	}
	else if (dataIn[0] == _SANDTRAY_GET_SHOWN_IM_PROPS_) {
		sResponse = _SANDTRAY_GET_SHOWN_IM_PROPS_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_SET_SPEED_) {
		sResponse = _SANDTRAY_SET_SPEED_ + "," + dataIn[1];
	}
	else if (dataIn[0] == _SANDTRAY_SET_FEEDBACK_ON_) {
		sResponse = _SANDTRAY_SET_FEEDBACK_ON_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_SET_FEEDBACK_OFF_) {
		sResponse = _SANDTRAY_SET_FEEDBACK_OFF_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_ROBOT_TURN_SELECTION_) {
		sResponse = _SANDTRAY_ROBOT_TURN_SELECTION_ + "," + dataIn[1];
	}
	else if (dataIn[0] == _SANDTRAY_BEZIER_DATA_) {
		sResponse = _SANDTRAY_BEZIER_DATA_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_MAKE_GOOD_MOVE_) {
		sResponse = _SANDTRAY_PREPARE_MOVE_ + "," + _SANDTRAY_MAKE_GOOD_MOVE_;
	}
	else if (dataIn[0] == _SANDTRAY_MAKE_BAD_MOVE_) {
		sResponse = _SANDTRAY_PREPARE_MOVE_ + "," + _SANDTRAY_MAKE_BAD_MOVE_;
	}
	else if (dataIn[0] == _SANDTRAY_MAKE_SPACE_MOVE_) {
		sResponse = _SANDTRAY_MOVE_TO_SPACE_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_MOVE_DATA_) {
		sResponse = _SANDTRAY_MOVE_DATA_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_ROBOT_TURN_LOCATION_) {
		sResponse = _SANDTRAY_ROBOT_TURN_LOCATION_ + ",0";
	}
	else if (dataIn[0] == _SANDTRAY_READY_) {
		sResponse = _SANDTRAY_READY_ + ",0";
	}
	
	if (DEBUG) {
		cout << "<<sandtrayControl>> sandtrayServer sending: " << sResponse << endl;
	}

	if (sResponse != "") {
		sandtrayControl->getCommand(sResponse);
	}
	_commandRequestBusy = false;		//flag change ensures future commands are accepted
	return "";
}

void SandtrayServerController::close() {
	sandtrayControl->release();
}


/***************************************************************/
/*                     sandtrayControl                   */
/***************************************************************/

SandtrayControl::SandtrayControl(SandtrayServerYarpInterface* yarpInterface, SandtrayServerController* sandtrayController) {
	_yarpInterface = yarpInterface;
	_sandtrayServerController = sandtrayController;

	//constructor
	name = "sandtrayControl";
	debug = false;
	isConnected = false;
	isStopping = false;
	controlListenSocket = INVALID_SOCKET;
	controlClientSocket = INVALID_SOCKET;
	recvbuflen = DEFAULT_BUFLEN;
}

bool SandtrayControl::socketInit() {
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//resolve local address and port to be used by server
	iResult = getaddrinfo(NULL, DEFAULT_CONTROL_PORT, &hints, &result);
	if (iResult != 0) {
		cout << "<<" << name << ">> error in address resolving: " << iResult << endl;
		WSACleanup();
		return false;
	}

	//create socket to listen for client connections
	controlListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (controlListenSocket == INVALID_SOCKET) {
		cout << "<<" << name << ">> error at socket: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	//setup tcp listening socket
	iResult = bind(controlListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		cout << "<<" << name << ">> bind failed with error: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		closesocket(controlListenSocket);
		WSACleanup();
		return false;
	}

	//assume setup complete - free memory
	freeaddrinfo(result);

	//listening on the socket
	if (listen(controlListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "<<" << name << ">> listen failed with error: " << WSAGetLastError() << endl;
		closesocket(controlListenSocket);
		WSACleanup();
		return false;
	}

	sockaddr_in from;
	int fromlen = sizeof(from);
	
	//accept a connection on the socket
	controlClientSocket = accept(controlListenSocket, (struct sockaddr*)&from, &fromlen);
	if (controlClientSocket == INVALID_SOCKET) {
		cout << "<<" << name << ">> accept connection failed with error: " << WSAGetLastError() << endl;
		closesocket(controlListenSocket);
		WSACleanup();
		return false;
	}

	//if this point reached, then all is well, and the data receiving/handling can start
	isConnected = true;
	return true;
}

bool SandtrayControl::socketClose() {
	try{
		iResult = shutdown(controlClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			cout << "<<" << name << ">> shutdown failed with error: " << WSAGetLastError() << endl;
			closesocket(controlClientSocket);
			WSACleanup();
			return false;
		}
	}
	catch (int e) {
		cout << "<<" << name << ">> sandtray control shutdown failed: " << e << endl;
	}

	closesocket(controlClientSocket);
	WSACleanup();

	return true;
}

bool SandtrayControl::init() {
	//debug = false;
	bool flag = false;
	flag = socketInit();
	isFirstLoop = true;

	cout << "<<" << name << ">> status of threadInit: " << flag << endl;

	return flag;
}

void SandtrayControl::release() {
	socketClose();
	isConnected = false;
}

void SandtrayControl::run() {
	//the main control loop for this thread
	// - needs to check for commands from elsewhere (Yarp port)
	// - if there's a command, then send it and wait for 
	if (debug) {
		cout << "<<" << name << ">> starting to run..." << endl;
	}

	bool sendFlag = false;		//if true, then have sent something to GameEngine

	//check that socket is set up and ready to connect
	if (isConnected != true) {
		cout << "<<" << name << ">> failure of socket setup with error: " << WSAGetLastError() << endl;
		cout << "<<" << name << ">> try again..." << endl;
		WSACleanup();
		return;
	}
}

void SandtrayControl::getCommand(string outMessage) {
	//send command to GameEngine on open socket
	bool sendFlag = sendCommand(outMessage);
	string sReturnMessage;

	//only need to do the following if something has been sent...
	if (sendFlag) {
		//wait for command to be processed... (arbitrary delay)
		Time::delay(0.2);

		//listen to return info from GameEngine (blocking?)
		sReturnMessage = listenCommand();

		//process return info from GameEngine (send back out on sandtrayReturnOut)
		if (sReturnMessage != "") {
			if (DEBUG) {
				cout << "<<" << name << ">> sandtray returned: " << sReturnMessage << endl;
			}

			_yarpInterface->sendToSandtrayReturn(sReturnMessage);
		}
	}
}

bool SandtrayControl::sendCommand(string message) {
	//assume at the moment that the passed string was correctly defined
	// - assume correct format already
	int messageLength = (int)message.length();

	//if message length is 0, then nothing to send
	if (messageLength == 0) {
		return false;
	}

	char *toSend = new char[messageLength + 1];
	toSend[messageLength] = 0;
	memcpy(toSend, message.c_str(), messageLength);

	try{
		int mess = -1;
		if (debug) {
			cout << "<<" << name << ">> data to send: " << toSend << endl;
		}

		mess = send(controlClientSocket, toSend, (int)strlen(toSend), 0);
		if (mess != strlen(toSend)) {
			cout << "<<" << name << ">> failure to send data, with error: " << WSAGetLastError() << endl;
			delete toSend;
			return false;
		}
	}
	catch (int e) {
		cout << "<<" << name << ">> send operation failure, with code: " << e << endl;
		delete toSend;
		return false;
	}

	//all's well
	delete toSend;
	return true;
}

string SandtrayControl::listenCommand()
{
	string toReturn = "";

	//empty the receive buffer
	char _fill = '\0';
	memset(recvbuf, _fill, sizeof(recvbuf));

	//get data from the socket
	int sizeIn = -1;

	try{
		sizeIn = recv(controlClientSocket, recvbuf, recvbuflen, 0);
	}
	catch (int e) {
		cout << "<<" << name << ">> sandtray command listen error: " << e << endl;
		return "";
	}

	//convert to a string if data...
	if (sizeIn > 0) {
		try{
			//recvbuf[sizeIn] = '\0';
			string temp(recvbuf);
			toReturn = temp;
			if (debug) {
				cout << "<<" << name << ">> data received: " << toReturn << endl;
			}
		}
		catch (int e) {
			cout << "<<" << name << ">> sandtray command listen conversion error: " << e << endl;
			toReturn = "";
		}
	}
	else{
		//..nothing there
		//if (debug) std::cout << "<<" << name << ">> no data to process, waiting..." << endl;
	}

	//empty the receive buffer
	char _fill2 = '\0';
	memset(recvbuf, _fill2, sizeof(recvbuf));

	if (isFirstLoop) {
		toReturn = removeString(toReturn, _SANDTRAY_GREET_PATTERN_);
		toReturn = removeString(toReturn, _SANDTRAY_GREET_CATEGORY_);
		isFirstLoop = false;
	}

	//and then return for processing
	return toReturn;
}