/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Erik Billing, University of Skovde 
 * Email:   erik.billing@his.se 
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
 

#include "sensoryInterpretationLogger.h"
#include "sensoryInterpretationLoggerController.h"

bool SensoryInterpretationLogger::configure(yarp::os::ResourceFinder &rf){
	/* Create the handler for log events */
	_logHandler = new SensoryInterpretationLogHandler();
	
	/* get and set the module name */
	_moduleName = rf.check("name", Value("sensoryInterpretationLogger"), "module name (string)").asString();
	setName(_moduleName.c_str());

	_logPeriod = rf.check("period", Value(0), "Period for synchronous logging (ms)").asInt();
	_logFilePath = rf.check("logFilePath", Value("sensoryInterpretation.log"), "Path to log file (string)").asString();
	if (!_logFilePath.empty() && _logFilePath.at(0) != '/' && _logFilePath.find(":") == string::npos) _logFilePath = rf.getContextPath() + "/" + _logFilePath;

	/* get the name of the input ports to be logged */
	Bottle ports = rf.findGroup("LOGPORTS").tail();
	for (int i=0; i<ports.size(); i++) {
		Value p = ports.get(i);
		if (p.isList() && p.asList()->size() >= 2) {
			Bottle* pb = p.asList();
			int dataSize = pb->size() > 2 ? pb->get(2).asInt() : 0;
			configurePort(pb->get(0).asString(), pb->get(1).asString(), dataSize);
		} else {
			cout << getName() << ": Warning! Unexpected LOGPORT value: " << p.toString() << endl;
		}
	}
	
   /* create the handler port */
   _handlerPortName = "/" + getName();

   if (!_handlerPort.open(_handlerPortName.c_str())){
      cout << getName() << ":unable to open port " << _handlerPortName << endl;
      return true;
   }

	attach(_handlerPort);

   /* Create the main class: Controller */ 
   _sensoryInterpretationLoggerController = new SensoryInterpretationLoggerController(_logHandler);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
	_sensoryInterpretationLoggerThread = new SensoryInterpretationLoggerThread(_logPeriod, &_ports, _logHandler);
   
   /* Init the YarpInterface to allow the communication */
   
   _logHandler->init(_logFilePath);
   
	if (_logPeriod > 0) _sensoryInterpretationLoggerThread->start();

   return true;
}

bool SensoryInterpretationLogger::configurePort(const string& portName, const string& portType, const int& dataSize) {
	if (portType == "VectorOf<int>") return configurePortType<VectorOf<int>>(portName, dataSize);
	if (portType == "VectorOf<double>") return configurePortType<VectorOf<double>>(portName, dataSize);
	if (portType == "VectorOf<long>") return configurePortType<VectorOf<long>>(portName, dataSize);
	if (portType == "Bottle") return configurePortType<Bottle>(portName, dataSize);
	cout << getName() << ": Warning! Undefined port type \"" << portType << "\"" << endl;
}

template <class T> bool SensoryInterpretationLogger::configurePortType(const string& portName, const int& dataSize) {
	BufferedLogPort<T>* p = new BufferedLogPort<T>();
	p->setLogHandler(_logHandler);
	p->setDataSize(dataSize);
	if (!p->open("/" + getName("/" + portName))) {
		return false;
	}
	if (_logPeriod==0) p->useCallback();
	_ports.push_back(shared_ptr<LogPort>(p));
};

bool SensoryInterpretationLogger::interruptModule(){
	/* stop the computation thread */
   _sensoryInterpretationLoggerThread->stop();

   
	for (unsigned i=0; i<_ports.size(); i++) {
		_ports.at(i)->interrupt();
	}
   _handlerPort.interrupt();

   return true;
}

bool SensoryInterpretationLogger::close(){

	for (unsigned i=0; i<_ports.size(); i++) {
		_ports.at(i)->close();
	}
   _handlerPort.close();

   return true;
}

bool SensoryInterpretationLogger::respond(const Bottle &command, Bottle &reply){
   string cmd = command.size() > 0 ? command.get(0).asString() : "";
	string cmd1 = command.size() > 1 ? command.get(1).asString() : "";
	string cmd2 = command.size() > 2 ? command.get(2).asString() : "";

	if (cmd == "set") {
		if (cmd1 == "log") {
			if (_logHandler->init(cmd2)) {
				if (_logPeriod > 0) _sensoryInterpretationLoggerThread->start();
				reply.addString("ok");
			} else {
				reply.addString("command failed");
			}
		} else if (cmd1 == "path") {
			if (_logHandler->init(cmd2 + "/" + _logFilePath)) {
				reply.addString("ok");
				if (_logPeriod > 0) _sensoryInterpretationLoggerThread->start();
			} else {
				reply.addString("command failed");
			}
		}
	} else if (cmd == "close") {
		_sensoryInterpretationLoggerThread->stop();	
		_logHandler->close();
		reply.addString("ok");
	}

   return true;
}

bool SensoryInterpretationLogger::updateModule(){
   return true;
}

