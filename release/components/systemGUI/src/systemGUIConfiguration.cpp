/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Hoang-Long Cao, VUB 
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
  * 22/06/2016 Version 1.0: ... (Hoang-Long Cao)
  * 25/01/2017 Version 2.0: ... (Hoang-Long Cao)
  */ 

#include "systemGUI.h"
#include "systemGUIYarpInterface.h"
#include "systemGUIController.h"

bool SystemGUI::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("systemGUI"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get module parameters */
   _logo  = rf.check("_logo", Value("logo.png"), "default logo (string)").asString();
   _logo = (rf.findFile(_logo.c_str())).c_str();

   /* get the name of the input and output ports */
   _getChildBehaviourInName = "/" +  getName(rf.check("_getChildBehaviourIn", Value("/getChildBehaviour:i"), "Input getChildBehaviour port (string)").asString());
   _getChildPerformanceInName = "/" +  getName(rf.check("_getChildPerformanceIn", Value("/getChildPerformance:i"), "Input getChildPerformance port (string)").asString());
   _proposedToSupervisorInName = "/" +  getName(rf.check("_proposedToSupervisorIn", Value("/proposedToSupervisor:i"), "Input proposedToSupervisor port (string)").asString());
   _smsSummaryInName = "/" +  getName(rf.check("_smsSummaryIn", Value("/smsSummary:i"), "Input smsSummary port (string)").asString());

   _selectedBySupervisorOutName = "/" +  getName(rf.check("_selectedBySupervisorOut", Value("/selectedBySupervisor:o"), "output selectedBySupervisor port (string)").asString());
   _therapistCommandOutName = "/" +  getName(rf.check("_therapistCommandOut", Value("/therapistCommand:o"), "output therapistCommand port (string)").asString());
   _userIDOutName = "/" +  getName(rf.check("_userIDOut", Value("/userID:o"), "output userID port (string)").asString());
   _interventionCommandOutName = "/" +  getName(rf.check("_interventionCommandOut", Value("/interventionCommand:o"), "output interventionCommand port (string)").asString());
   
   /* open the ports */
   if (!_getChildBehaviourIn.open(_getChildBehaviourInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getChildBehaviourInName << std::endl;
      return false;
   }
   if (!_getChildPerformanceIn.open(_getChildPerformanceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getChildPerformanceInName << std::endl;
      return false;
   }
   if (!_proposedToSupervisorIn.open(_proposedToSupervisorInName.c_str())){
      std::cout << getName() << ": unable to open port " << _proposedToSupervisorInName << std::endl;
      return false;
   }
   if (!_smsSummaryIn.open(_smsSummaryInName.c_str())){
      std::cout << getName() << ": unable to open port " << _smsSummaryInName << std::endl;
      return false;
   }

   if (!_selectedBySupervisorOut.open(_selectedBySupervisorOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _selectedBySupervisorOutName << std::endl;
      return false;
   }
   if (!_therapistCommandOut.open(_therapistCommandOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _therapistCommandOutName << std::endl;
      return false;
   }
   if (!_userIDOut.open(_userIDOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _userIDOutName << std::endl;
      return false;
   }
   if (!_interventionCommandOut.open(_interventionCommandOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _interventionCommandOutName << std::endl;
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
   _systemGUIYarpInterface = new SystemGUIYarpInterface();

   /* Create the main class: Controller */ 
    _systemGUIController = new SystemGUIController(_systemGUIYarpInterface,_logo);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _systemGUIThread = new SystemGUIThread(&_getChildBehaviourIn, &_getChildPerformanceIn, &_proposedToSupervisorIn, &_smsSummaryIn, &_selectedBySupervisorOut, &_therapistCommandOut, &_userIDOut,&_interventionCommandOut);
   _systemGUIThread->setYarpInterface(_systemGUIYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _systemGUIYarpInterface->init(_systemGUIThread, _systemGUIController);
   
   _systemGUIThread->start();

   return true;
}

bool SystemGUI::interruptModule(){
   _getChildBehaviourIn.interrupt();
   _getChildPerformanceIn.interrupt();
   _proposedToSupervisorIn.interrupt();
   _smsSummaryIn.interrupt();
   _selectedBySupervisorOut.interrupt();
   _therapistCommandOut.interrupt();
   _userIDOut.interrupt();
   _interventionCommandOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool SystemGUI::close(){
   /* close the ports */
   _getChildBehaviourIn.close();
   _getChildPerformanceIn.close();
   _proposedToSupervisorIn.close();
   _smsSummaryIn.close();
   _selectedBySupervisorOut.close();
   _therapistCommandOut.close();
   _userIDOut.close();
   _interventionCommandOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _systemGUIThread->stop();

   return true;
}

bool SystemGUI::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool SystemGUI::updateModule(){
   return true;
}

double SystemGUI::getPeriod(){
   return 0.1;
}
