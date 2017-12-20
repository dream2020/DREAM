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

#include "sandtrayEvent.h"
#include "sandtrayEventYarpInterface.h"

SandtrayEventThread::SandtrayEventThread(BufferedPort<Bottle>* sandtrayEventOut){
	/* assign the input and output ports */
	_sandtrayEventOut = sandtrayEventOut;
}

void SandtrayEventThread::setYarpInterface(SandtrayEventYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
}

bool SandtrayEventThread::threadInit(){
	/* initilize the thread */
	return true;
}

void SandtrayEventThread::run(){

}

void SandtrayEventThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void SandtrayEventThread::sendToSandtrayEvent(Bottle data){
	Bottle& container = _sandtrayEventOut->prepare();
	container = data;
	_sandtrayEventOut->write();
}
