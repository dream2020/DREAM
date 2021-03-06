 /** @file deliberativeSubsystemController.h 
  *
  * Version 1.0
  * 
  * Generated by YarpGenerator, Emmanuel Senft, PLYM
  *
  *
  * \defgroup DREAM_cognitiveComponent deliberativeSubsystem
  * 
  * Main class with code, use the yarpInterface class to communicate with the other components.
  * 
  * \section lib_sec Libraries
  *
  * None
  *
  * \section in_files_sec Input Data Files
  *
  * None
  *
  * \section out_data_sec Output Data Files
  *
  * None
  *
  * \author 
  * 
  * <James Kennedy>, <Plymouth University>  
  * 
  * Copyright (C) 2014 DREAM Consortium
  * 
  */

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
  * 10/08/2016 Version 1.0: ... (James Kennedy)
  */ 
 
#ifndef DELIBERATIVESUBSYSTEMCONTROLLER
#define DELIBERATIVESUBSYSTEMCONTROLLER

#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

class DeliberativeSubsystemYarpInterface;
class DeliberativeSubsystemController;
class Waiter;
class ActionWaiter;
class CommonFunctionalities;
class PeriodicTimer {
private:
	int _initDuration;
	int _duration;
	boost::asio::io_service* _io;
	boost::asio::deadline_timer* _t;
	boost::thread* _thr;
	DeliberativeSubsystemController* _controller;
public:
	PeriodicTimer(int duration, int initDuration, DeliberativeSubsystemController* controller, boost::asio::io_service* io);
	~PeriodicTimer();
	void callback(const boost::system::error_code &e);
	void start();
	void stop();
};

class DeliberativeSubsystemController{
private:
	DeliberativeSubsystemYarpInterface* _yarpInterface;
	PeriodicTimer* _timer;
	CommonFunctionalities* _common;
	Waiter* _waiterGaze;
	Waiter* _waiterPerf;
	Waiter* _waiterSand;
	std::vector<ActionWaiter*> _actionWaiters;
	float _fPerf;
	bool _bMutualGaze;
	bool _bAskLook;
	bool _bWaitingForPerf;
	boost::asio::io_service *_io;
	int _IscriptId;
	int _IwaitingAction; 
	int _IwaitingStep; 
	bool _Bwaiting4action;
	int _IscriptType;
	bool _BonScript;
	int _IstepId;
	bool _BscriptFinished;
	bool _BobjectGrabbed;
	bool _Bdiagnose;
	bool _Btherapist;
	bool _BtherapistMove;
	bool _BtherapistTouch;
	bool _bSandtrayConnected;
	bool _bSandtrayCategory;
	bool _bLastFeedbackGood;

public:
	/* class methods */
	DeliberativeSubsystemController(DeliberativeSubsystemYarpInterface *yarpInterface);
    //Need to add the methods calling/being called by the YarpInterface
	void getFromInterventionCommand(int stepId, int commandId, std::string arg);
	void getFromSelectedAction(int stepId, int actionId, std::string arg);
	void sendToComandSuccess(bool success);
	void getFromCheckMutualGaze(bool mutualGaze);
	void getFromChildPerformance(float perf, bool fromWP5 = 0);

	void goNextStep(bool forced = 0);
	void checkGaze();
	void waitms(int millisec);
	void waitForPerf(int stepId, std::string arg);
	void sandtrayDisplayWhite();
	void sandtrayNewGame();
	void sandtrayLoadLibrary(std::string libName);
	void sandtraySetImageOne();
	void sandtraySetImageAll();
	void sandtrayEnableFeedback(bool enable);
	void sandtrayEnableTouch(bool enable);
	void scriptCorrupted(int stepId);
	void testRobotConnection();
	void getFromActionFeedback(int step, int action, int feedback, bool subAction);
	void askLook();
	void stopTimer();
	void resetWaitingStates();

	void processEvents(std::string str);
	void processReturns(std::string str);

	void offScript();
	void onScript();

	bool getOnScript();

	bool waitForAction(int stepId, int actionId, int& parameter);
	void purgeActionWaiter();

	void close();
};

#endif
