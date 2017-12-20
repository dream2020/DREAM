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
#include "sandtrayEventController.h"
#include "sandtrayMessages.h"

bool SandtrayEvent::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("sandtrayEvent"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */

   _sandtrayEventOutName = "/" +  getName(rf.check("_sandtrayEventOut", Value("/sandtrayEvent:o"), "output sandtrayEvent port (string)").asString());

   /* open the ports */
   if (!_sandtrayEventOut.open(_sandtrayEventOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sandtrayEventOutName << std::endl;
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
   _sandtrayEventYarpInterface = new SandtrayEventYarpInterface();

   /* Create the main class: Controller */ 
   _sandtrayEventController = new SandtrayEventController(_sandtrayEventYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _sandtrayEventThread = new SandtrayEventThread(&_sandtrayEventOut);
   _sandtrayEventThread->setYarpInterface(_sandtrayEventYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _sandtrayEventYarpInterface->init(_sandtrayEventThread, _sandtrayEventController);
   
   _sandtrayEventThread->start();

   return true;
}

bool SandtrayEvent::interruptModule(){
   _sandtrayEventOut.interrupt();
   _handlerPort.interrupt();

   _sandtrayEventController->stop();

   return true;
}

bool SandtrayEvent::close(){
   /* close the ports */
   _sandtrayEventOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _sandtrayEventThread->stop();

   return true;
}

bool SandtrayEvent::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool SandtrayEvent::updateModule(){
   return true;
}

double SandtrayEvent::getPeriod(){
   return 0.1;
}
