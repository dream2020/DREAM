/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Emmanuel Senft, PLYM 
 * Email:   emmanuel.senft@plymouth.ac.uk 
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
  * 16/08/2016 Version 1.0: ... (Emmanuel Senft)
  */ 

#include "naoInterfaceDriver.h"
#include "naoInterfaceDriverYarpInterface.h"
#include "naoInterfaceDriverController.h"
#include "moveSequenceId.h"
#include "commonFonctionalities.h"

bool NaoInterfaceDriver::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("naoInterfaceDriver"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _sensorFeedbackInName = "/" +  getName(rf.check("_sensorFeedbackIn", Value("/sensorFeedback:i"), "Input sensorFeedback port (string)").asString());
   _robotMotorFeedbackInName = "/" +  getName(rf.check("_robotMotorFeedbackIn", Value("/robotMotorFeedback:i"), "Input robotMotorFeedback port (string)").asString());

   _disableRobotOutName = "/" +  getName(rf.check("_disableRobotOut", Value("/disableRobot:o"), "output disableRobot port (string)").asString());
   _enableRobotOutName = "/" +  getName(rf.check("_enableRobotOut", Value("/enableRobot:o"), "output enableRobot port (string)").asString());
   _gripOutName = "/" +  getName(rf.check("_gripOut", Value("/grip:o"), "output grip port (string)").asString());
   _moveHandOutName = "/" +  getName(rf.check("_moveHandOut", Value("/moveHand:o"), "output moveHand port (string)").asString());
   _moveTorsoOutName = "/" +  getName(rf.check("_moveTorsoOut", Value("/moveTorso:o"), "output moveTorso port (string)").asString());
   _releaseOutName = "/" +  getName(rf.check("_releaseOut", Value("/release:o"), "output release port (string)").asString());
   _sayOutName = "/" +  getName(rf.check("_sayOut", Value("/say:o"), "output say port (string)").asString());
   _moveSequenceOutName = "/" +  getName(rf.check("_moveSequenceOut", Value("/moveSequence:o"), "output moveSequence port (string)").asString());
   _moveHeadOutName = "/" +  getName(rf.check("_moveHeadOut", Value("/moveHead:o"), "output moveHead port (string)").asString());
   _pointAtOutName = "/" +  getName(rf.check("_pointAtOut", Value("/pointAt:o"), "output pointAt port (string)").asString());

   /* open the ports */
   if (!_sensorFeedbackIn.open(_sensorFeedbackInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sensorFeedbackInName << std::endl;
      return false;
   }
   if (!_robotMotorFeedbackIn.open(_robotMotorFeedbackInName.c_str())){
      std::cout << getName() << ": unable to open port " << _robotMotorFeedbackInName << std::endl;
      return false;
   }

   if (!_disableRobotOut.open(_disableRobotOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _disableRobotOutName << std::endl;
      return false;
   }
   if (!_enableRobotOut.open(_enableRobotOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _enableRobotOutName << std::endl;
      return false;
   }
   if (!_gripOut.open(_gripOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _gripOutName << std::endl;
      return false;
   }
   if (!_moveHandOut.open(_moveHandOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _moveHandOutName << std::endl;
      return false;
   }
   if (!_moveTorsoOut.open(_moveTorsoOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _moveTorsoOutName << std::endl;
      return false;
   }
   if (!_releaseOut.open(_releaseOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _releaseOutName << std::endl;
      return false;
   }
   if (!_sayOut.open(_sayOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sayOutName << std::endl;
      return false;
   }
   if (!_moveSequenceOut.open(_moveSequenceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _moveSequenceOutName << std::endl;
      return false;
   }
   if (!_moveHeadOut.open(_moveHeadOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _moveHeadOutName << std::endl;
      return false;
   }
   if (!_pointAtOut.open(_pointAtOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _pointAtOutName << std::endl;
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
   _naoInterfaceDriverYarpInterface = new NaoInterfaceDriverYarpInterface();

   /* Create the main class: Controller */ 
   _naoInterfaceDriverController = new NaoInterfaceDriverController(_naoInterfaceDriverYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _naoInterfaceDriverThread = new NaoInterfaceDriverThread(&_sensorFeedbackIn, &_robotMotorFeedbackIn, &_disableRobotOut, &_enableRobotOut, &_gripOut, &_moveHandOut, &_moveTorsoOut, &_releaseOut, &_sayOut, &_moveSequenceOut, &_moveHeadOut, &_pointAtOut);
   _naoInterfaceDriverThread->setYarpInterface(_naoInterfaceDriverYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _naoInterfaceDriverYarpInterface->init(_naoInterfaceDriverThread, _naoInterfaceDriverController);
   
   _naoInterfaceDriverThread->start();

   return true;
}

bool NaoInterfaceDriver::interruptModule(){
   _sensorFeedbackIn.interrupt();
   _robotMotorFeedbackIn.interrupt();
   _disableRobotOut.interrupt();
   _enableRobotOut.interrupt();
   _gripOut.interrupt();
   _moveHandOut.interrupt();
   _moveTorsoOut.interrupt();
   _releaseOut.interrupt();
   _sayOut.interrupt();
   _moveSequenceOut.interrupt();
   _moveHeadOut.interrupt();
   _pointAtOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool NaoInterfaceDriver::close(){
   /* close the ports */
   _sensorFeedbackIn.close();
   _robotMotorFeedbackIn.close();
   _disableRobotOut.close();
   _enableRobotOut.close();
   _gripOut.close();
   _moveHandOut.close();
   _moveTorsoOut.close();
   _releaseOut.close();
   _sayOut.close();
   _moveSequenceOut.close();
   _moveHeadOut.close();
   _pointAtOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _naoInterfaceDriverThread->stop();

   return true;
}

bool NaoInterfaceDriver::respond(const Bottle &command, Bottle &reply){
	reply.clear();
	CommonFunctionalities common;

	if (command.get(0).asString() == "help") {
		reply.addString("help: test | closeHand | openHand | enable | disable | say,string | move,int | lookAt | pointAt | moveHead | moveHand | blink");
	}
	else {
		std::string instruction = command.get(0).asString();
		reply.addString(instruction + " being executed...");
		if (command.get(1).isString()) {
			if (instruction == "say")				_naoInterfaceDriverController->say(command.get(1).asString(), 1);
		}
		else if (command.get(1).isInt()) {
			if (instruction == "move")				_naoInterfaceDriverController->move(0, command.get(1).asInt());
		}
		else if (instruction== "test")				_naoInterfaceDriverController->testAll(); 
		else if (instruction== "closeHand")			_naoInterfaceDriverController->closeHand(1,1,1); 
		else if (instruction== "openHand")			_naoInterfaceDriverController->closeHand(1,0,1);
		else if (instruction== "enable")			_naoInterfaceDriverController->enableRobot(1, 1);
		else if (instruction== "disable")			_naoInterfaceDriverController->enableRobot(0, 1);
		else if (instruction== "stand")				_naoInterfaceDriverController->move(0, _MOVE_STAND_);
		else if (instruction== "lookAt")			_naoInterfaceDriverController->lookAt(0.5,0,0.3,0.3, 1);
		else if (instruction== "pointAt")			_naoInterfaceDriverController->generatePointAt(1);
		else if (instruction== "moveHead")			_naoInterfaceDriverController->lookAt(0, -1, 0.5, 0.2, 7);
		else if (instruction== "moveHand")			_naoInterfaceDriverController->moveHand();
		else if (instruction== "blink")				_naoInterfaceDriverController->blink(1, 3, 0, 2000); 
		else { reply.addString("no command recognised"); }

		reply.addString(instruction+ " finished");
	}
   return true;
}

bool NaoInterfaceDriver::updateModule(){
   return true;
}

double NaoInterfaceDriver::getPeriod(){
   return 0.1;
}
