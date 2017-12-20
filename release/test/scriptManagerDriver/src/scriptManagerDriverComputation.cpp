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
  * 07/07/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "scriptManagerDriver.h"
#include "scriptManagerDriverYarpInterface.h"

ScriptManagerDriverThread::ScriptManagerDriverThread(BufferedPort<Bottle>* interventionCommandIn, BufferedPort<VectorOf<int>>* smsStartStopOut, BufferedPort<VectorOf<int>>* delibStartStopOut, BufferedPort<VectorOf<int>>* commandSuccessOut){
	/* assign the input and output ports */
	_interventionCommandIn = interventionCommandIn;
	_smsStartStopOut = smsStartStopOut;
	_delibStartStopOut = delibStartStopOut;
	_commandSuccessOut = commandSuccessOut;
   
	_interventionCommandInAnalyser = new InterventionCommandInAnalyser();
   
	interventionCommandIn->useCallback(*_interventionCommandInAnalyser);
}

void ScriptManagerDriverThread::setYarpInterface(ScriptManagerDriverYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_interventionCommandInAnalyser->setScriptManagerDriver(_yarpInterface);
}

bool ScriptManagerDriverThread::threadInit(){
	/* initilize the thread */
	return true;
}

void ScriptManagerDriverThread::run(){

}

void ScriptManagerDriverThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void ScriptManagerDriverThread::sendToSmsStartStop(VectorOf<int> data){
	VectorOf<int>& container = _smsStartStopOut->prepare();
	container = data;
	_smsStartStopOut->write();
}

void ScriptManagerDriverThread::sendToDelibStartStop(VectorOf<int> data){
	VectorOf<int>& container = _delibStartStopOut->prepare();
	container = data;
	_delibStartStopOut->write();
}

void ScriptManagerDriverThread::sendToCommandSuccess(VectorOf<int> data){
	VectorOf<int>& container = _commandSuccessOut->prepare();
	container = data;
	_commandSuccessOut->write();
}


void InterventionCommandInAnalyser::setScriptManagerDriver(ScriptManagerDriverYarpInterface* yarpInterface){
   _scriptManagerDriverYarpInterface=yarpInterface;
}



void InterventionCommandInAnalyser::onRead(Bottle& interventionCommandInData){
   _scriptManagerDriverYarpInterface->getFromInterventionCommand(interventionCommandInData);
}



