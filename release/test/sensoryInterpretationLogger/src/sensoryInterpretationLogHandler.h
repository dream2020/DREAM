 /** @file SensoryInterpretationLogHandler.h 
  * 
  * \defgroup DREAM SensoryInterpretationLogHandler
  * 
  * Class used to for logging to csv file. See sensoryInterpretationLogger.h for details.
  * 
  * \section lib_sec Libraries
  *
  * - YARP.
  *
  * \author 
  * 
  * <Erik Billing>, <University of Skovde>  
  * 
  * Copyright (C) 2014 DREAM Consortium
  * 
  */

/* 
 * Copyright (C) 2015 DREAM Consortium
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

 
 
#ifndef SENSORYINTERPRETATIONLOGHANDLER
#define SENSORYINTERPRETATIONLOGHANDLER

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <Windows.h> // For millisecond time resolution

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
#include <yarp/os/Mutex.h>

using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;

class SensoryInterpretationLoggerThread;
class SensoryInterpretationLoggerController;

class SensoryInterpretationLogHandler {
private:
	SensoryInterpretationLoggerThread* _yarpThread;
	SensoryInterpretationLoggerController* _controller;
	ofstream _log;
	Mutex _mutex;
	int _colCount;

	void initLogLine(const string& port);
public:
	/* class methods */
	bool init(string logFilePath);
	void close();

	int initColumn(const string& columnHeader);
	int initColumn(const string& columnHeader, const unsigned& dataSize);

	void logTime(); 
	void newLine();
	template <class T> void logCol(const T* data, unsigned dataSize);
	template <class T> void logRow(const string& port, const T& data);
};

#endif
