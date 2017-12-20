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

 

#include "sensoryInterpretationLogHandler.h"
#include "yarpUtils.h"
#include <iomanip>

bool SensoryInterpretationLogHandler::init(string logFilePath) {
	string datetimeTag("<datetime>");

	if (_log.is_open()) {
		cout << "SensoryInterpretationLogger: WARNING! Log file allready open! " << logFilePath << endl;
		return false;
	}

	size_t tstart = logFilePath.find(datetimeTag);
	if (tstart != string::npos) {
		time_t rawtime;
		struct tm * timeinfo;
		char tbuffer[80];

		time (&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(tbuffer,sizeof(tbuffer),"%Y%m%d_%H%M%S",timeinfo);
		std::string tstamp(tbuffer);
		logFilePath.replace(tstart,datetimeTag.length(),tstamp);
	}

	_log.open(logFilePath);
	if (_log.fail()) {
		cout << "SensoryInterpretationLogger: WARNING! Unable to open log file! " << logFilePath << endl;
		return false;
	}

	cout << "SensoryInterpretationLogger: Log file open: " << logFilePath << endl;
	_log.setf(ios::fixed);
	//_log.precision(3);
	_colCount = 0;
	return true;
}

void SensoryInterpretationLogHandler::close() {
	_log.close();
	cout << "SensoryInterpretationLogger: Log file closed." << endl;
}

int SensoryInterpretationLogHandler::initColumn(const string& columnHeader) {
	if (_colCount>0) _log << ";\t";
	size_t sep = columnHeader.find_last_of('/');
	_log << columnHeader.substr(sep+1);
	return _colCount++;
}

int SensoryInterpretationLogHandler::initColumn(const string& columnHeader, const unsigned& dataSize) {
	switch (dataSize) {
	case 0: 
		cout << columnHeader << ": Synchronized logging requires data size specification." << endl;
		return -1;
	case 1: 
		return initColumn(columnHeader);
	default:
		for (unsigned i=1; i<=dataSize; i++) {
			initColumn(columnHeader);
			_log << ":" << i;
		}
		return _colCount;
	}
}

void SensoryInterpretationLogHandler::logTime() {
	SYSTEMTIME t;
	GetLocalTime(&t);
	_log << setfill('0') << t.wYear << "-" << setw(2) << t.wMonth << "-" << setw(2) << t.wDay << " " << setw(2) << t.wHour << ":" << setw(2) << t.wMinute << ":" << setw(2) << t.wSecond << "." << setw(3) << t.wMilliseconds;
}

void SensoryInterpretationLogHandler::initLogLine(const string& port) {
	size_t sep = port.find_last_of('/');
	_log << port.substr(sep+1) << ";";
	logTime(); 
	_log << ";";
}

void SensoryInterpretationLogHandler::newLine() {
	_log << endl;
}

template <class T> void SensoryInterpretationLogHandler::logCol(const T* data, unsigned dataSize) {
	for (unsigned i=0; i<dataSize; i++) {
		if (data!=NULL && data->size() > i) _log << ";" << (*data)[i];
		else _log << ";";
	}
}

/* Instanciating templates to avoid linker problems */
template void SensoryInterpretationLogHandler::logCol(const VectorOf<int>* data, unsigned dataSize);
template void SensoryInterpretationLogHandler::logCol(const VectorOf<double>* data, unsigned dataSize);
template void SensoryInterpretationLogHandler::logCol(const VectorOf<long>* data, unsigned dataSize);
//template void SensoryInterpretationLogHandler::logCol(const Bottle* data, unsigned dataSize);
template <> void SensoryInterpretationLogHandler::logCol(const Bottle* data, unsigned dataSize) {
	_log << ";";
	if (data != NULL && data->size() > 0) {
		_log << data->get(0).asString();
		for (unsigned i=1; i<data->size(); i++) {
			_log << " " << data->get(i).asString();
		}
	}
}

template <class T> void SensoryInterpretationLogHandler::logRow(const string& port, const T& data) {
	_mutex.lock();
	initLogLine(port);
	if (data.size() > 0) _log << data[0];
	for (unsigned i=1; i<data.size(); i++) _log << ";" << data[i];
	_log << endl;
	_mutex.unlock();
}

template void SensoryInterpretationLogHandler::logRow(const string& port, const VectorOf<int>& data);
template void SensoryInterpretationLogHandler::logRow(const string& port, const VectorOf<double>& data);
template void SensoryInterpretationLogHandler::logRow(const string& port, const VectorOf<long>& data);
//template void SensoryInterpretationLogHandler::logRow(const string& port, const Bottle& data);
template <> void SensoryInterpretationLogHandler::logRow(const string& port, const Bottle& data) {
	_mutex.lock();
	initLogLine(port);
	if (data.size() > 0) {
		_log << data.get(0).toString();
		for (unsigned i=1; i<data.size(); i++) {
			_log << ";" << data.get(i).toString();
		}
	}
	_log << endl;
	_mutex.unlock();
}