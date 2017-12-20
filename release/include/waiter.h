/** @file waiter.h
*
* Version 1.0
*
* \defgroup DREAM_cognitiveComponent sharedResources
*
* Header class to share waiter class based on boost asio
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
* <Emmanuel Senft>, <Plymouth University>
*
* Copyright (C) 2014 DREAM Consortium
*
*/

/*
* Copyright (C) 2014 DREAM Consortium
* FP7 Project 611391 co-funded by the European Commission
*
* Author:  Emmanuel Senft, Plymouth University
* Email:   emmanuel.senft@plymouth.ac.uk
* Website: www.dream2020.eu
*
* This program comes with ABSOLUTELY NO WARRANTY
*/

/*
* Audit Trail
* -----------
* 07/11/2016 Version 1.0: ... (Emmanuel Senft)
*/
#ifndef WAITER
#define WAITER

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/any.hpp>
#include <iostream>
#include <vector>

class Waiter {
private:
	boost::mutex *_ioMutex;
	boost::condition_variable *_condition;
	boost::mutex::scoped_lock *_scopedLock;
	std::vector<boost::any> _parameters;

protected:
	bool _waiting;
	bool _cancelled;
	
public:
	Waiter(){
		_ioMutex = new boost::mutex;
		_condition = new boost::condition_variable;
		_scopedLock = new boost::mutex::scoped_lock(*_ioMutex);
		_waiting = 0;
		_cancelled = 0;
	}
	
	void wait(){
		_condition->wait(*_scopedLock);
	}

	void notify() {
		if (!_waiting)
			return;
		_waiting = 0;
		_condition->notify_one();
	}

	void notify(boost::any value) {
		if (!_waiting)
			return;
		_parameters.push_back(value);
		_waiting = 0;
		_condition->notify_one();
	}

	void addParameter(boost::any value) {
		_parameters.push_back(value);		
	}

	bool startWaiting() {
		_parameters.clear();
		_waiting = 1;
		_cancelled = 0;
		while (_waiting) wait();
		return _cancelled;
	}

	bool getIsNotWaiting() {
		return !_waiting;
	}

	bool getIsWaiting() {
		return _waiting;
	}

	void setIsWaiting(bool isWaiting) {
		_waiting = isWaiting;
	}

	std::vector<std::string> getParameterAsVectorString(int index) {
		return boost::any_cast<std::vector<std::string>>(_parameters.at(index));
	}

	std::string getParameterAsString(int index) {
		if (index>=_parameters.size())
			throw std::invalid_argument("Index out of bounds");
		return boost::any_cast<std::string>(_parameters.at(index));
	}

	int getParameterAsInt(int index) {
		if (index>=_parameters.size())
			throw std::invalid_argument("Index out of bounds");
		return boost::any_cast<int>(_parameters.at(index));
	}

	void cancel() {
		_cancelled = 1;
		notify();
	}
};

class ActionWaiter : public Waiter {
	int _waitingStep;
	int _waitingActionId;

public:
	ActionWaiter::ActionWaiter(int waitingStep, int waitingAction): _waitingStep(waitingStep), _waitingActionId(waitingAction){}

	bool testCondition(int waitingStep, int waitingActionId) {
		bool not = _waiting && waitingStep == _waitingStep && waitingActionId == _waitingActionId;
		//std::cout << "-Waiter- " << _waiting << " Waiting for " << _waitingStep << " " << _waitingActionId << std::endl;
		//std::cout << "-Waiter- Received " << waitingStep << " " << waitingActionId << " -> Notify: "<< not << std::endl;
		return _waiting && waitingStep == _waitingStep && waitingActionId == _waitingActionId;
	}
};

class SandtrayWaiter : public Waiter {
	std::vector<std::string> _toMatch;

public:
	SandtrayWaiter::SandtrayWaiter(std::vector<std::string> toMatch) : _toMatch(toMatch) {}

	bool testCondition(std::string toMatch) {
		if (!_waiting)
			return 0;
		for (unsigned i = 0; i < _toMatch.size(); i++)
			if (_toMatch[i] == toMatch)
				return 1;
		return 0;
	}
};

class SensorWaiter : public Waiter {
	std::string _sensorName;
	bool _value;

public:
	SensorWaiter::SensorWaiter(std::string sensorName, bool value) : _sensorName(sensorName), _value(value){}

	bool testCondition(std::string sensorName, bool value) {
		//std::cout << "Waiting " << _waiting << " for " << _sensorName << " " << _value << " received " << sensorName << " " << value << std::endl;
		return _waiting && _sensorName == sensorName && _value == value;
	}
};

#endif