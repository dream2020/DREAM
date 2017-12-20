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
  * 22/06/2016 Version 1.0: shell creation (James Kennedy)
  * 24/06/2016 Version 1.1: updated to forward to correct functions (James Kennedy)
  */ 

#include "sandtrayServerYarpInterface.h"
#include "sandtrayServer.h"
#include "sandtrayServerController.h"

using namespace std;

SandtrayServerYarpInterface::SandtrayServerYarpInterface(){

}

SandtrayServerYarpInterface::~SandtrayServerYarpInterface(){
	//_yarpThread->stop();
	delete _controller;
}

void SandtrayServerYarpInterface::init(SandtrayServerThread* yarpThread, SandtrayServerController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void SandtrayServerYarpInterface::getFromSandtrayCommand(Bottle data){
	if (!data.get(0).isString()) return;				//check that bottle contents is string
	if (data.get(0).asString().length() < 1) return;	//if empty string then don't unpack bottle
	_controller->getFromCommands(data.get(0).asString());
	return;
}

void SandtrayServerYarpInterface::sendToSandtrayReturn(string str){
	if (str == "") return;			//if empty string then don't send bottle
	Bottle data;
	data.addString(str);
	_yarpThread->sendToSandtrayReturn(data);
	return;
}