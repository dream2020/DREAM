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
  * 18/08/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "userModelDriver.h"
#include "userModelDriverYarpInterface.h"
#include "userModelDriverController.h"

bool UserModelDriver::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("userModelDriver"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _userDataInName = "/" +  getName(rf.check("_userDataIn", Value("/userData:i"), "Input userData port (string)").asString());

   _userIDOutName = "/" +  getName(rf.check("_userIDOut", Value("/userID:o"), "output userID port (string)").asString());
   _updatedDataOutName = "/" +  getName(rf.check("_updatedDataOut", Value("/updatedData:o"), "output updatedData port (string)").asString());

   /* open the ports */
   if (!_userDataIn.open(_userDataInName.c_str())){
      std::cout << getName() << ": unable to open port " << _userDataInName << std::endl;
      return false;
   }

   if (!_userIDOut.open(_userIDOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _userIDOutName << std::endl;
      return false;
   }
   if (!_updatedDataOut.open(_updatedDataOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _updatedDataOutName << std::endl;
      return false;
   }

   /* create the handler port */
   _handlerPortName = "/";
   _handlerPortName += getName();

   if (!_handlerPort.open(_handlerPortName.c_str())){
      std::cout << getName() << ":unable to open port " << _handlerPortName << std::endl;
      return false;
   }
   
   attach(_handlerPort);

   /* Create the YarpInterface used to communicate between the main code in Controller and the other components through the thread class */
   _userModelDriverYarpInterface = new UserModelDriverYarpInterface();

   /* Create the main class: Controller */ 
   _userModelDriverController = new UserModelDriverController(_userModelDriverYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _userModelDriverThread = new UserModelDriverThread(&_userDataIn, &_userIDOut, &_updatedDataOut);
   _userModelDriverThread->setYarpInterface(_userModelDriverYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _userModelDriverYarpInterface->init(_userModelDriverThread, _userModelDriverController);
   
   _userModelDriverThread->start();

   return true;
}

bool UserModelDriver::interruptModule(){
   _userDataIn.interrupt();
   _userIDOut.interrupt();
   _updatedDataOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool UserModelDriver::close(){
   /* close the ports */
   _userDataIn.close();
   _userIDOut.close();
   _updatedDataOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _userModelDriverThread->stop();

   return true;
}

bool UserModelDriver::respond(const Bottle &command, Bottle &reply){
	reply.clear();

	if (command.get(0).asString() == "help") {
		reply.addString("help: test");
	}
	else {
		reply.addString(command.get(0).asString() + " being executed...");

		if (command.get(0).asString() == "test") { _userModelDriverController->test(); }
		//else if (command.get(0).asString() == "command") { _userModelDriverController->function(); }
		else { reply.addString("no command recognised"); }
	}
	
	return true;
}

bool UserModelDriver::updateModule(){
   return true;
}

double UserModelDriver::getPeriod(){
   return 0.1;
}
