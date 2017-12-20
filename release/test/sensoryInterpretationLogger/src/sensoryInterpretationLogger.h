 /** @file sensoryInterpretationLogger.h Utility component to read data from specified input ports and log the data to  a file
  *
  * Version 1.0
  * 
  * 18/04/2016
  * 
  * \defgroup DREAM_childBehaviorClassiciation sensoryInterpretationLogger
  *
  * Utility component to read data from specified input ports and log the data to  a file.
  *
  * \section component_description Component Description
  *
  * This is a utility component and does not form part of the DREAM system architecture.  
  * It reads data from specified input ports and logs to a file. The component can be 
  * configured to log any number of input ports with a limited number of data formats. See Section 
  * Parameters for details.
  * 
  * Two modes for logging are supported: Synchronous and Event Based. We here assume that the 
  * component is configured to log from two ports: 
  * - portA of type VectorOf<int> with a single element
  * - portB of type VectorOf<double> with three elements
  *
  * <b>Synchronous</b> logging writes data to csv file on the following form: 
  * Timestamp;    <portname>;   <portname>:1  <portname>:2  <portname>:3
  * <timestamp>;  <value>;      <value 1>;    <value 2>;    <value 3>
  * e.g., 
  * Timestamp;    portA;   portB:1  portB:2  portB:3
  * <timestamp>;  10;      1.100;   2.200;   3.300
  * ...
  *
  * <b>Event Based</b> logging writes data to csv file on the following form: 
  * <portname>;<timestamp>;<value 1>;<value 2>;...
  * e.g., 
  * portA;<timestamp>;10
  * portB;<timestamp>;1.100;2.200;3.300
  * ...
  *
  * <timestamp> specifies date and time on the following form: <YYYY-MM-DD hh:mm:ss.sss>.
  *
  * \section lib_sec Libraries
  *
  * - YARP.
  *
  * \section parameters_sec Parameters
  * 
  * <b>Command-line Parameters </b> 
  * 
  * The following key-value pairs can be specified as command-line parameters 
  * by prefixing \c -- to the key e.g. \c --from file.ini. 
  * The value part can be changed to suit your needs; the default values are 
  * shown below. 
  *
  * - \c from \c sensoryInterpretationLogger.ini 
  *    - specifies the configuration file
  *
  * - \c context \c sensoryInterpretationLogger/configuration
  *   - specifies the sub-childBehaviourClassification from \c $DREAM_ROOT/sensoryInterpretationLogger/configuration 
  *   to the configuration file
  *
  * - \c name \c sensoryInterpretationLogger   
  *   - specifies the name of the module (used to form the stem of module port names)  
  *
  *
  * <b>Configuration File Parameters </b>
  *
  * The following key-value pairs can be specified as parameters in the configuration file 
  * (they can also be specified as command-line parameters if you so wish). 
  * The value part can be changed to suit your needs; the default values are shown below. 
  * 
  * Key | Value
  * :-- | :----  
  * logFilePath | sensoryInterpretation.log
  *
  * - specifies the path to the log file
  *   
  * Key | Value
  * :-- | :---- 
  * period | 0
  *
  * - specifies the thread period in ms for synchronous. A period=0 indicates event based logging. 
  * 
  * The configuration file should also contain a section [LOGPORTS] listing input ports to be logged. Ports 
  * are specified on the form: <portname> <portType> <vectorSize>. E.g., 
  *
  * [LOGPORTS]
  * frameIndex:i             VectorOf<long>   1
  * getEyeGaze:i             VectorOf<double> 3
  * cameraId:i               VectorOf<int>    1
  *
  * Port type must be one of VectorOf<int>, VectorOf<double>, or VectorOf<long>. Vector size 
  * specification is optional for event based logging, but mandatory in synchronous mode.
  *
  * \section portsa_sec Ports Accessed
  * 
  * - None
  *                      
  * \section portsc_sec Ports Created
  *
  * <b>Input ports</b>
  *
  * - \c /sensoryInterpretationLogger
  * - \c input ports to be logged, as specified inthe [LOGPORTS] section of the configuration file. 
  *
  * <b>Output ports</b>
  *
  *  - \c /sensoryInterpretationLogger
  *
  * <b>Port types </b>
  *
  * The type of each input port is specified in the configuration file, as specified in section <i>Configuration File Parameters </i>. 
  *
  * \section in_files_sec Input Data Files
  *
  * None
  *
  * \section out_data_sec Output Data Files
  *
  * sensoryInterpretation.log - taeget log file, may be changed by modifying logFilePath property. 
  *
  * \section conf_file_sec Configuration Files
  *
  * \c sensoryInterpretationLogger.ini   
  *
  * \section example_sec Example Instantiation of the Component
  * 
  * <tt>sensoryInterpretationLogger --name sensoryInterpretationLogger --context components/sensoryInterpretationLogger/config --from sensoryInterpretationLogger.ini</tt>
  *
  * \author 
  * 
  * Erik Billing, University of Skovde
  * 
  */
  
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

  
 /* 
  * Audit Trail
  * -----------
  * 2016-04-18 Version 1.0: First integrated version (Erik Billing)
  */ 
 
#ifndef SENSORYINTERPRETATIONLOGGER
#define SENSORYINTERPRETATIONLOGGER

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>

#include "sensoryInterpretationLogHandler.h"

using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;

class SensoryInterpretationLoggerController;

class LogPort {
public:
	virtual void interrupt() {}
	virtual void close() {}
	virtual ConstString getName() = 0;
	virtual unsigned getDataSize() = 0;
};

template <class T> class BufferedLogPort : public LogPort, public BufferedPort<T> {
	SensoryInterpretationLogHandler* _logHandler;
	unsigned _dataSize;
	
	void onRead(VectorOf<int>& data) {
		_logHandler->logRow(getName(), data);
	}
	void onRead(VectorOf<long>& data) {
		_logHandler->logRow(getName(), data);
	}
	virtual void onRead(VectorOf<double>& data) {
		_logHandler->logRow(getName(), data);
	}
	virtual void onRead(Bottle& data) {
		_logHandler->logRow(getName(), data);
	}
public:
	void setLogHandler(SensoryInterpretationLogHandler* logHandler) {
		_logHandler = logHandler;
	}
	unsigned getDataSize() {
		return _dataSize;
	}
	void setDataSize(const unsigned& size) {
		_dataSize = size;
	}
	void interrupt() {
		BufferedPort::interrupt();
	}
	void close() {
		BufferedPort::close();
	}
	ConstString getName() {
		return BufferedPort::getName();
	}
};

/*
	Size of bottle data is always a single column. 
*/
template <> unsigned BufferedLogPort<Bottle>::getDataSize() {
	return 1;
};

class SensoryInterpretationLoggerThread : public RateThread {
private:
	/* class variables */
	vector<shared_ptr<LogPort>>* _ports;
	
	SensoryInterpretationLogHandler* _logHandler;
	
public:
	/* class methods */
	SensoryInterpretationLoggerThread::SensoryInterpretationLoggerThread(int period, vector<shared_ptr<LogPort>>* ports, SensoryInterpretationLogHandler* logHandler);   
	//void setYarpInterface(SensoryInterpretationLogHandler* yarpInterface);
	bool threadInit();
	void threadRelease();
	void run();
};

class SensoryInterpretationLogger : public RFModule{
private:
	/* module parameters */
	string _logFilePath;
	string _moduleName;
	string _handlerPortName;
	int _logPeriod;

	/* class variables */
	vector<shared_ptr<LogPort>> _ports;

	Port _handlerPort;

	/* pointer to a new thread */
	SensoryInterpretationLoggerThread* _sensoryInterpretationLoggerThread;
   SensoryInterpretationLoggerController* _sensoryInterpretationLoggerController;
   SensoryInterpretationLogHandler* _logHandler;

	bool configurePort(const string &portName, const string &portType, const int& dataSize);
	template <class T> bool configurePortType(const string& portName, const int& dataSize);
public:
	/* class methods */
	bool configure(yarp::os::ResourceFinder &rf);
	bool interruptModule();
	bool close();
	bool respond(const yarp::os::Bottle& command, Bottle& reply);
	bool updateModule();
};

#endif
