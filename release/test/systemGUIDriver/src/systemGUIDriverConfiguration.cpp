/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Hoang-Long Cao, Vrije Universiteit Brussel 
 * Email:   hoang.long.cao@vub.ac.be 
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
  * 14/11/2016 Version 1.0: ... (Hoang-Long Cao)
  */ 

#include "systemGUIDriver.h"
#include "systemGUIDriverYarpInterface.h"
#include "systemGUIDriverController.h"

bool SystemGUIDriver::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("systemGUIDriver"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _selectedBySupervisorInName = "/" +  getName(rf.check("_selectedBySupervisorIn", Value("/selectedBySupervisor:i"), "Input selectedBySupervisor port (string)").asString());
   _therapistCommandInName = "/" +  getName(rf.check("_therapistCommandIn", Value("/therapistCommand:i"), "Input therapistCommand port (string)").asString());
   _userIDInName = "/" +  getName(rf.check("_userIDIn", Value("/userID:i"), "Input userID port (string)").asString());

   _getChildBehaviourOutName = "/" +  getName(rf.check("_getChildBehaviourOut", Value("/getChildBehaviour:o"), "output getChildBehaviour port (string)").asString());
   _getChildPerformanceOutName = "/" +  getName(rf.check("_getChildPerformanceOut", Value("/getChildPerformance:o"), "output getChildPerformance port (string)").asString());
   _proposedToSupervisorOutName = "/" +  getName(rf.check("_proposedToSupervisorOut", Value("/proposedToSupervisor:o"), "output proposedToSupervisor port (string)").asString());
   _smsSummaryOutName = "/" +  getName(rf.check("_smsSummaryOut", Value("/smsSummary:o"), "output smsSummary port (string)").asString());
   
   /* open the ports */
   if (!_selectedBySupervisorIn.open(_selectedBySupervisorInName.c_str())){
      std::cout << getName() << ": unable to open port " << _selectedBySupervisorInName << std::endl;
      return false;
   }
   if (!_therapistCommandIn.open(_therapistCommandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _therapistCommandInName << std::endl;
      return false;
   }
   if (!_userIDIn.open(_userIDInName.c_str())){
      std::cout << getName() << ": unable to open port " << _userIDInName << std::endl;
      return false;
   }

   if (!_getChildBehaviourOut.open(_getChildBehaviourOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getChildBehaviourOutName << std::endl;
      return false;
   }
   if (!_getChildPerformanceOut.open(_getChildPerformanceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getChildPerformanceOutName << std::endl;
      return false;
   }
   if (!_proposedToSupervisorOut.open(_proposedToSupervisorOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _proposedToSupervisorOutName << std::endl;
      return false;
   }
   if (!_smsSummaryOut.open(_smsSummaryOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _smsSummaryOutName << std::endl;
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
   _systemGUIDriverYarpInterface = new SystemGUIDriverYarpInterface();

   /* Create the main class: Controller */ 
   _systemGUIDriverController = new SystemGUIDriverController(_systemGUIDriverYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _systemGUIDriverThread = new SystemGUIDriverThread(&_selectedBySupervisorIn, &_therapistCommandIn, &_userIDIn, &_getChildBehaviourOut, &_getChildPerformanceOut, &_proposedToSupervisorOut, &_smsSummaryOut);
   _systemGUIDriverThread->setYarpInterface(_systemGUIDriverYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _systemGUIDriverYarpInterface->init(_systemGUIDriverThread, _systemGUIDriverController);
   
   _systemGUIDriverThread->start();

   return true;
}

bool SystemGUIDriver::interruptModule(){
   _selectedBySupervisorIn.interrupt();
   _therapistCommandIn.interrupt();
   _userIDIn.interrupt();
   _getChildBehaviourOut.interrupt();
   _getChildPerformanceOut.interrupt();
   _proposedToSupervisorOut.interrupt();
   _smsSummaryOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool SystemGUIDriver::close(){
   /* close the ports */
   _selectedBySupervisorIn.close();
   _therapistCommandIn.close();
   _userIDIn.close();
   _getChildBehaviourOut.close();
   _getChildPerformanceOut.close();
   _proposedToSupervisorOut.close();
   _smsSummaryOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _systemGUIDriverThread->stop();

   return true;
}

bool SystemGUIDriver::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool SystemGUIDriver::updateModule(){
   return true;
}

double SystemGUIDriver::getPeriod(){
   return 0.1;
}
