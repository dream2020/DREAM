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
#include "scriptManagerDriverController.h"

bool ScriptManagerDriver::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("scriptManagerDriver"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _interventionCommandInName = "/" +  getName(rf.check("_interventionCommandIn", Value("/interventionCommand:i"), "Input interventionCommand port (string)").asString());

   _smsStartStopOutName = "/" +  getName(rf.check("_smsStartStopOut", Value("/smsStartStop:o"), "output smsStartStop port (string)").asString());
   _delibStartStopOutName = "/" +  getName(rf.check("_delibStartStopOut", Value("/delibStartStop:o"), "output delibStartStop port (string)").asString());
   _commandSuccessOutName = "/" +  getName(rf.check("_commandSuccessOut", Value("/commandSuccess:o"), "output commandSuccess port (string)").asString());

   /* open the ports */
   if (!_interventionCommandIn.open(_interventionCommandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _interventionCommandInName << std::endl;
      return false;
   }

   if (!_smsStartStopOut.open(_smsStartStopOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _smsStartStopOutName << std::endl;
      return false;
   }
   if (!_delibStartStopOut.open(_delibStartStopOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _delibStartStopOutName << std::endl;
      return false;
   }
   if (!_commandSuccessOut.open(_commandSuccessOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _commandSuccessOutName << std::endl;
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
   _scriptManagerDriverYarpInterface = new ScriptManagerDriverYarpInterface();

   /* Create the main class: Controller */ 
   _scriptManagerDriverController = new ScriptManagerDriverController(_scriptManagerDriverYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _scriptManagerDriverThread = new ScriptManagerDriverThread(&_interventionCommandIn, &_smsStartStopOut, &_delibStartStopOut, &_commandSuccessOut);
   _scriptManagerDriverThread->setYarpInterface(_scriptManagerDriverYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _scriptManagerDriverYarpInterface->init(_scriptManagerDriverThread, _scriptManagerDriverController);
   
   _scriptManagerDriverThread->start();

   return true;
}

bool ScriptManagerDriver::interruptModule(){
   _interventionCommandIn.interrupt();
   _smsStartStopOut.interrupt();
   _delibStartStopOut.interrupt();
   _commandSuccessOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool ScriptManagerDriver::close(){
   /* close the ports */
   _interventionCommandIn.close();
   _smsStartStopOut.close();
   _delibStartStopOut.close();
   _commandSuccessOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _scriptManagerDriverThread->stop();

   return true;
}

bool ScriptManagerDriver::respond(const Bottle &command, Bottle &reply){
	reply.clear();

	if (command.get(0).asString() == "help") {
		reply.addString("help: test | startscript |");
	}
	else {
		reply.addString(command.get(0).asString() + " being executed...");

		if (command.get(0).asString() == "test")				{ _scriptManagerDriverController->testApp(); }
		else if (command.get(0).asString() == "startscript") { _scriptManagerDriverController->startScript(8); }
		else if (command.get(0).asString() == "startscript1") { _scriptManagerDriverController->startScript(1); }
		else if (command.get(0).asString() == "startscript2") { _scriptManagerDriverController->startScript(2); }
		else if (command.get(0).asString() == "startscript3") { _scriptManagerDriverController->startScript(3); }
		else if (command.get(0).asString() == "startscript4") { _scriptManagerDriverController->startScript(4); }
		else if (command.get(0).asString() == "startscript5") { _scriptManagerDriverController->startScript(5); }
		else if (command.get(0).asString() == "startscript6") { _scriptManagerDriverController->startScript(6); }
		else if (command.get(0).asString() == "startscript7") { _scriptManagerDriverController->startScript(7); }
		else if (command.get(0).asString() == "startscript8") { _scriptManagerDriverController->startScript(8); }
		else if (command.get(0).asString() == "startscript9") { _scriptManagerDriverController->startScript(9); }
		else if (command.get(0).asString() == "startscript10") { _scriptManagerDriverController->startScript(10); }
		else if (command.get(0).asString() == "startscript11") { _scriptManagerDriverController->startScript(11); }
		else if (command.get(0).asString() == "startscript12") { _scriptManagerDriverController->startScript(12); }
		else if (command.get(0).asString() == "startscript13") { _scriptManagerDriverController->startScript(13); }
		else if (command.get(0).asString() == "startscript14") { _scriptManagerDriverController->startScript(14); }
		else if (command.get(0).asString() == "startscript15") { _scriptManagerDriverController->startScript(15); }
		else if (command.get(0).asString() == "startscript16") { _scriptManagerDriverController->startScript(16); }
		else if (command.get(0).asString() == "startscript17") { _scriptManagerDriverController->startScript(17); }
		else if (command.get(0).asString() == "startscript18") { _scriptManagerDriverController->startScript(18); }
		else if (command.get(0).asString() == "startscript19") { _scriptManagerDriverController->startScript(19); }
		else if (command.get(0).asString() == "startscript20") { _scriptManagerDriverController->startScript(20); }
		else if (command.get(0).asString() == "nextstep")		{ _scriptManagerDriverController->nextStep(); }
		else { reply.addString("no command recognised"); }
	}

	return true;
}

bool ScriptManagerDriver::updateModule(){
   return true;
}

double ScriptManagerDriver::getPeriod(){
   return 0.1;
}
