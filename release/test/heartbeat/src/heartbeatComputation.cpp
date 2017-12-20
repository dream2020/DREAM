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


#include "heartbeat.h"

HeartbeatThread::HeartbeatThread(int period, BufferedPort<VectorOf<long>>* heartbeatOut):
	RateThread(period),
	_beatIndex(-100)
{
	/* assign the input and output ports */
	_heartbeatOut = heartbeatOut;
}

bool HeartbeatThread::threadInit() {
	/* initilize the thread */
	return true;
}

void HeartbeatThread::run(){
	++_beatIndex;
	if (_beatIndex >= 0) {
		VectorOf<long> &beat = _heartbeatOut->prepare();
		beat.resize(1);
		beat(0) = _beatIndex;
		_heartbeatOut->write();
	}
	//cout << _beatIndex << endl;
}

void HeartbeatThread::threadRelease(){
	/* release any dynamically created resources */
}





