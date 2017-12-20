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
  * 23/06/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "sandtrayServerDriver.h"
#include "sandtrayServerDriverYarpInterface.h"
#include "sandtrayServerDriverController.h"

bool SandtrayServerDriver::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("sandtrayServerDriver"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _sandtrayEventInName = "/" +  getName(rf.check("_sandtrayEventIn", Value("/sandtrayEvent:i"), "Input sandtrayEvent port (string)").asString());
   _sandtrayReturnInName = "/" +  getName(rf.check("_sandtrayReturnIn", Value("/sandtrayReturn:i"), "Input sandtrayReturn port (string)").asString());

   _sandtrayCommandOutName = "/" +  getName(rf.check("_sandtrayCommandOut", Value("/sandtrayCommand:o"), "output sandtrayCommand port (string)").asString());

   /* open the ports */
   if (!_sandtrayEventIn.open(_sandtrayEventInName.c_str())) {
      std::cout << getName() << ": unable to open port " << _sandtrayEventInName << std::endl;
      return false;
   }
   if (!_sandtrayReturnIn.open(_sandtrayReturnInName.c_str())) {
      std::cout << getName() << ": unable to open port " << _sandtrayReturnInName << std::endl;
      return false;
   }

   if (!_sandtrayCommandOut.open(_sandtrayCommandOutName.c_str())) {
      std::cout << getName() << ":unable to open port " << _sandtrayCommandOutName << std::endl;
      return false;
   }

   /* create the handler port */
   _handlerPortName = "/";
   _handlerPortName += getName();

   if (!_handlerPort.open(_handlerPortName.c_str())) {
      std::cout << getName() << ":unable to open port " << _handlerPortName << std::endl;
      return false;
   }

   attach(_handlerPort);

   /* Create the YarpInterface used to communicate between the main code in Controller and the other components through the thread class */
   _sandtrayServerDriverYarpInterface = new SandtrayServerDriverYarpInterface();

   /* Create the main class: Controller */ 
   _sandtrayServerDriverController = new SandtrayServerDriverController(_sandtrayServerDriverYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _sandtrayServerDriverThread = new SandtrayServerDriverThread(&_sandtrayEventIn, &_sandtrayReturnIn, &_sandtrayCommandOut);
   _sandtrayServerDriverThread->setYarpInterface(_sandtrayServerDriverYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _sandtrayServerDriverYarpInterface->init(_sandtrayServerDriverThread, _sandtrayServerDriverController);
   
   _sandtrayServerDriverThread->start();

   return true;
}

bool SandtrayServerDriver::interruptModule(){
   _sandtrayEventIn.interrupt();
   _sandtrayReturnIn.interrupt();
   _sandtrayCommandOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool SandtrayServerDriver::close(){
   /* close the ports */
   _sandtrayEventIn.close();
   _sandtrayReturnIn.close();
   _sandtrayCommandOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _sandtrayServerDriverThread->stop();

   return true;
}

bool SandtrayServerDriver::respond(const Bottle &command, Bottle &reply){
	reply.clear();

	if (command.get(0).asString() == "help") {
		reply.addString("help: test | testpattern | verify | newgame | resetgame | quit | lock | unlock | enablebuttons | disablebuttons |");
		reply.addString("speclevel | movedone | userdata | imgleft | libprops | goodmove | badmove | spacemove | turnselect");
		reply.addString("feedon | feedoff | onetimeon | onetimeoff");
	}
	else {
		reply.addString(command.get(0).asString() + " being executed...");

		if (command.get(0).asString() == "test")				{ _sandtrayServerDriverController->testApp(); }
		else if (command.get(0).asString() == "testpattern")	{ _sandtrayServerDriverController->testPattern(); }
		else if (command.get(0).asString() == "verify")			{ _sandtrayServerDriverController->verify(); }
		else if (command.get(0).asString() == "quit")			{ _sandtrayServerDriverController->quitGame(); }
		else if (command.get(0).asString() == "newgame")		{ _sandtrayServerDriverController->newGame(); }
		else if (command.get(0).asString() == "resetgame")		{ _sandtrayServerDriverController->resetGame(); }
		else if (command.get(0).asString() == "enablebuttons")	{ _sandtrayServerDriverController->enableButtons(1); }
		else if (command.get(0).asString() == "disablebuttons") { _sandtrayServerDriverController->enableButtons(0); }
		else if (command.get(0).asString() == "movedone")		{ _sandtrayServerDriverController->moveFinished(); }
		else if (command.get(0).asString() == "userdata")		{ _sandtrayServerDriverController->getUserData(); }
		else if (command.get(0).asString() == "imgleft")		{ _sandtrayServerDriverController->getImagesLeft(); }
		else if (command.get(0).asString() == "speclevel")		{ _sandtrayServerDriverController->specifyLevel(0); }
		else if (command.get(0).asString() == "speclevel1")		{ _sandtrayServerDriverController->specifyLevel(1); }
		else if (command.get(0).asString() == "speclevel2")		{ _sandtrayServerDriverController->specifyLevel(2); }
		else if (command.get(0).asString() == "speclevel3")		{ _sandtrayServerDriverController->specifyLevel(3); }
		else if (command.get(0).asString() == "speclevel4")		{ _sandtrayServerDriverController->specifyLevel(4); }
		else if (command.get(0).asString() == "speclevel5")		{ _sandtrayServerDriverController->specifyLevel(5); }
		else if (command.get(0).asString() == "speclevel6")		{ _sandtrayServerDriverController->specifyLevel(6); }
		else if (command.get(0).asString() == "speclevel7")		{ _sandtrayServerDriverController->specifyLevel(7); }
		else if (command.get(0).asString() == "speclevel8")		{ _sandtrayServerDriverController->specifyLevel(8); }
		else if (command.get(0).asString() == "speclevel9")		{ _sandtrayServerDriverController->specifyLevel(9); }
		else if (command.get(0).asString() == "speclevel10")		{ _sandtrayServerDriverController->specifyLevel(10); }
		else if (command.get(0).asString() == "speclevel11")		{ _sandtrayServerDriverController->specifyLevel(11); }
		else if (command.get(0).asString() == "libprops")		{ _sandtrayServerDriverController->getLibraryProperties(); }
		else if (command.get(0).asString() == "catprops")		{ _sandtrayServerDriverController->getCategoryProperties(); }
		else if (command.get(0).asString() == "onetimeon")		{ _sandtrayServerDriverController->setOneAtTime(1); }
		else if (command.get(0).asString() == "onetimeoff")		{ _sandtrayServerDriverController->setOneAtTime(0); }
		else if (command.get(0).asString() == "lock")			{ _sandtrayServerDriverController->lockImages(); }
		else if (command.get(0).asString() == "unlock")			{ _sandtrayServerDriverController->unlockImages(); }
		else if (command.get(0).asString() == "goodmove")		{ _sandtrayServerDriverController->goodMove(); }
		else if (command.get(0).asString() == "badmove")		{ _sandtrayServerDriverController->badMove(); }
		else if (command.get(0).asString() == "spacemove")		{ _sandtrayServerDriverController->spaceMove(); }
		else if (command.get(0).asString() == "turnselect")		{ _sandtrayServerDriverController->turnTakeSelect(3); }
		else if (command.get(0).asString() == "feedon")			{ _sandtrayServerDriverController->setFeedbackOn(); }
		else if (command.get(0).asString() == "feedoff")		{ _sandtrayServerDriverController->setFeedbackOff(); }
		else { reply.addString("no command recognised"); }

		reply.addString(command.get(0).asString() + " finished");
	}

	return true;
}

bool SandtrayServerDriver::updateModule(){
   return true;
}

double SandtrayServerDriver::getPeriod(){
   return 0.1;
}
