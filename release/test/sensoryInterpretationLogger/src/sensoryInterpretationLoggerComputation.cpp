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

SensoryInterpretationLoggerThread::SensoryInterpretationLoggerThread(int period, vector<shared_ptr<LogPort>>* ports, SensoryInterpretationLogHandler* logHandler) :
	RateThread(period)
{
	_ports = ports;
	_logHandler = logHandler;
}
	
bool SensoryInterpretationLoggerThread::threadInit(){
	/* initilize the thread */
	_logHandler->initColumn("Timestamp");
	for (unsigned i=0; i<_ports->size(); i++) {
		shared_ptr<LogPort> p = _ports->at(i);
		_logHandler->initColumn(p->getName(), p->getDataSize());
	}
	return true;
}

void SensoryInterpretationLoggerThread::run() {
	shared_ptr<LogPort> p;
	shared_ptr<BufferedLogPort<VectorOf<int>>> pInt;
	shared_ptr<BufferedLogPort<VectorOf<double>>> pDouble;
	shared_ptr<BufferedLogPort<VectorOf<long>>> pLong;
	shared_ptr<BufferedLogPort<Bottle>> pBottle;

	_logHandler->newLine();
	_logHandler->logTime();
	for (unsigned i=0; i<_ports->size(); i++) {
		p = _ports->at(i);
		pInt = dynamic_pointer_cast<BufferedLogPort<VectorOf<int>>>(p);
		if (pInt != NULL) {
			_logHandler->logCol(pInt->read(false),pInt->getDataSize());
			continue;
		} 
		pDouble = dynamic_pointer_cast<BufferedLogPort<VectorOf<double>>>(p);
		if (pDouble != NULL) {
			_logHandler->logCol(pDouble->read(false),pDouble->getDataSize());
			continue;
		}
		pLong = dynamic_pointer_cast<BufferedLogPort<VectorOf<long>>>(p);
		if (pLong != NULL) {
			_logHandler->logCol(pLong->read(false),pLong->getDataSize());
			continue;
		}
		pBottle = dynamic_pointer_cast<BufferedLogPort<Bottle>>(p);
		if (pBottle != NULL) {
			_logHandler->logCol(pBottle->read(false), 1);
		}
	}
}

void SensoryInterpretationLoggerThread::threadRelease() {
		
}
