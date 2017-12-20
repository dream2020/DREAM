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

#include "naoInterface.h"
#include "naoInterfaceYarpInterface.h"
#include "naoInterfaceController.h"
#include <sstream>
#include <vector>

bool NaoInterface::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("naoInterface"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _disableRobotInName = "/" +  getName(rf.check("_disableRobotIn", Value("/disableRobot:i"), "Input disableRobot port (string)").asString());
   _enableRobotInName = "/" +  getName(rf.check("_enableRobotIn", Value("/enableRobot:i"), "Input enableRobot port (string)").asString());
   _gripInName = "/" +  getName(rf.check("_gripIn", Value("/grip:i"), "Input grip port (string)").asString());
   _moveHandInName = "/" +  getName(rf.check("_moveHandIn", Value("/moveHand:i"), "Input moveHand port (string)").asString());
   _moveTorsoInName = "/" +  getName(rf.check("_moveTorsoIn", Value("/moveTorso:i"), "Input moveTorso port (string)").asString());
   _releaseInName = "/" +  getName(rf.check("_releaseIn", Value("/release:i"), "Input release port (string)").asString());
   _sayInName = "/" +  getName(rf.check("_sayIn", Value("/say:i"), "Input say port (string)").asString());
   _moveSequenceInName = "/" +  getName(rf.check("_moveSequenceIn", Value("/moveSequence:i"), "Input moveSequence port (string)").asString());
   _moveHeadInName = "/" +  getName(rf.check("_moveHeadIn", Value("/moveHead:i"), "Input moveHead port (string)").asString());
   _pointAtInName = "/" +  getName(rf.check("_pointAtIn", Value("/pointAt:i"), "Input pointAt port (string)").asString());

   _sensorFeedbackOutName = "/" +  getName(rf.check("_sensorFeedbackOut", Value("/sensorFeedback:o"), "output sensorFeedback port (string)").asString());
   _robotMotorFeedbackOutName = "/" +  getName(rf.check("_robotMotorFeedbackOut", Value("/robotMotorFeedback:o"), "output robotMotorFeedback port (string)").asString());

   /* open the ports */
   if (!_disableRobotIn.open(_disableRobotInName.c_str())){
      std::cout << getName() << ": unable to open port " << _disableRobotInName << std::endl;
      return false;
   }
   if (!_enableRobotIn.open(_enableRobotInName.c_str())){
      std::cout << getName() << ": unable to open port " << _enableRobotInName << std::endl;
      return false;
   }
   if (!_gripIn.open(_gripInName.c_str())){
      std::cout << getName() << ": unable to open port " << _gripInName << std::endl;
      return false;
   }
   if (!_moveHandIn.open(_moveHandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _moveHandInName << std::endl;
      return false;
   }
   if (!_moveTorsoIn.open(_moveTorsoInName.c_str())){
      std::cout << getName() << ": unable to open port " << _moveTorsoInName << std::endl;
      return false;
   }
   if (!_releaseIn.open(_releaseInName.c_str())){
      std::cout << getName() << ": unable to open port " << _releaseInName << std::endl;
      return false;
   }
   if (!_sayIn.open(_sayInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sayInName << std::endl;
      return false;
   }
   if (!_moveSequenceIn.open(_moveSequenceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _moveSequenceInName << std::endl;
      return false;
   }
   if (!_moveHeadIn.open(_moveHeadInName.c_str())){
      std::cout << getName() << ": unable to open port " << _moveHeadInName << std::endl;
      return false;
   }
   if (!_pointAtIn.open(_pointAtInName.c_str())){
      std::cout << getName() << ": unable to open port " << _pointAtInName << std::endl;
      return false;
   }

   if (!_sensorFeedbackOut.open(_sensorFeedbackOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sensorFeedbackOutName << std::endl;
      return false;
   }
   if (!_robotMotorFeedbackOut.open(_robotMotorFeedbackOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _robotMotorFeedbackOutName << std::endl;
      return false;
   }

   /* create the handler port */
   _handlerPortName = "/";
   _handlerPortName += getName();

   if (!_handlerPort.open(_handlerPortName.c_str())){
      std::cout << getName() << ":unable to open port " << _handlerPortName << std::endl;
      return true;
   }

   /* get the ip by config file*/
   std::string ip= rf.check("ip", Value("127.0.0.1"), "robot ip (string)").asString();
   int port = rf.check("port", Value(58116), "robot port (int)").asInt();
   float volume = rf.check("volume", Value(0.3), "robot initial volume (float)").asDouble();

   std::vector<std::string > behaviours;
   int i = 0;
   bool stop = 0;
	while(!stop){
		std::stringstream ss;
		ss << "behaviour" << i;
		std::string behaviourName = rf.check(ss.str().c_str(), Value("nan"), ss.str().c_str()).asString();
		if (behaviourName=="nan") {
			stop = 1;
			break;
		}
		i++;
		behaviours.push_back(behaviourName);
	}
   /* Create the YarpInterface used to communicate between the main code in Controller and the other components through the thread class */
   _naoInterfaceYarpInterface = new NaoInterfaceYarpInterface();

   /* Create the main class: Controller */ 
   _naoInterfaceController = new NaoInterfaceController(_naoInterfaceYarpInterface, ip, port, behaviours, volume);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _naoInterfaceThread = new NaoInterfaceThread(&_disableRobotIn, &_enableRobotIn, &_gripIn, &_moveHandIn, &_moveTorsoIn, &_releaseIn, &_sayIn, &_moveSequenceIn, &_moveHeadIn, &_pointAtIn, &_sensorFeedbackOut, &_robotMotorFeedbackOut);
   _naoInterfaceThread->setYarpInterface(_naoInterfaceYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _naoInterfaceYarpInterface->init(_naoInterfaceThread, _naoInterfaceController);
   
   _naoInterfaceThread->start();

   return true;
}

bool NaoInterface::interruptModule(){
   _disableRobotIn.interrupt();
   _enableRobotIn.interrupt();
   _gripIn.interrupt();
   _moveHandIn.interrupt();
   _moveTorsoIn.interrupt();
   _releaseIn.interrupt();
   _sayIn.interrupt();
   _moveSequenceIn.interrupt();
   _moveHeadIn.interrupt();
   _pointAtIn.interrupt();
   _sensorFeedbackOut.interrupt();
   _robotMotorFeedbackOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool NaoInterface::close(){
   /* close the ports */
   _disableRobotIn.close();
   _enableRobotIn.close();
   _gripIn.close();
   _moveHandIn.close();
   _moveTorsoIn.close();
   _releaseIn.close();
   _sayIn.close();
   _moveSequenceIn.close();
   _moveHeadIn.close();
   _pointAtIn.close();
   _sensorFeedbackOut.close();
   _robotMotorFeedbackOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _naoInterfaceThread->stop();

   return true;
}

bool NaoInterface::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool NaoInterface::updateModule(){
   return true;
}

double NaoInterface::getPeriod(){
   return 0.1;
}
