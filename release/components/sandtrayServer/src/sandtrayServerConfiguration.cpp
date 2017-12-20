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
  * 22/06/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "sandtrayServer.h"
#include "sandtrayServerYarpInterface.h"
#include "sandtrayServerController.h"
#include "sandtrayMessages.h"

bool SandtrayServer::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("sandtrayServer"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _sandtrayCommandInName = "/" +  getName(rf.check("_sandtrayCommandIn", Value("/sandtrayCommand:i"), "Input sandtrayCommand port (string)").asString());

   _sandtrayReturnOutName = "/" +  getName(rf.check("_sandtrayReturnOut", Value("/sandtrayReturn:o"), "output sandtrayReturn port (string)").asString());

   /* open the ports */
   if (!_sandtrayCommandIn.open(_sandtrayCommandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sandtrayCommandInName << std::endl;
      return false;
   }

   if (!_sandtrayReturnOut.open(_sandtrayReturnOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sandtrayReturnOutName << std::endl;
      return false;
   }

   /* create the handler port */
   _handlerPortName = "/";
   _handlerPortName += getName();

   if (!_handlerPort.open(_handlerPortName.c_str())){
      std::cout << getName() << ":unable to open port " << _handlerPortName << std::endl;
      return true;
   }

   attach(_handlerPort);

   /* Create the YarpInterface used to communicate between the main code in Controller and the other components through the thread class */
   _sandtrayServerYarpInterface = new SandtrayServerYarpInterface();

   /* Create the main class: Controller */ 
   _sandtrayServerController = new SandtrayServerController(_sandtrayServerYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _sandtrayServerThread = new SandtrayServerThread(&_sandtrayCommandIn, &_sandtrayReturnOut);
   _sandtrayServerThread->setYarpInterface(_sandtrayServerYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _sandtrayServerYarpInterface->init(_sandtrayServerThread, _sandtrayServerController);
   
   _sandtrayServerThread->start();

   return true;
}

bool SandtrayServer::interruptModule(){
   _sandtrayCommandIn.interrupt();
   _sandtrayReturnOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool SandtrayServer::close(){
   /* close the ports */
   _sandtrayCommandIn.close();
   _sandtrayReturnOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _sandtrayServerThread->stop();

   return true;
}

bool SandtrayServer::respond(const Bottle &command, Bottle &reply){
	reply.clear();
	reply.addString("help: no direct communication intended - if test, connect to /sandtrayServerDriver, if full system, then use SMS calls");

	return true;
}

bool SandtrayServer::updateModule(){
   return true;
}

double SandtrayServer::getPeriod(){
   return 0.1;
}
