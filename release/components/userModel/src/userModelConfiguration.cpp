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
  * 13/06/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "userModel.h"
#include "userModelYarpInterface.h"
#include "userModelController.h"

bool UserModel::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("userModel"), "module name (string)").asString();
   setName(_moduleName.c_str());

   _userIni = rf.check("_userList", Value("userModel.ini"), "user ini file (string)").asString();
   _userIni = (rf.findFile(_userIni.c_str())).c_str();

   /* get the name of the input and output ports */
   _userIDInName = "/" +  getName(rf.check("_userIDIn", Value("/userID:i"), "Input userID port (string)").asString());
   _updatedDataInName = "/" +  getName(rf.check("_updatedDataIn", Value("/updatedData:i"), "Input updatedData port (string)").asString());

   _userDataOutName = "/" +  getName(rf.check("_userDataOut", Value("/userData:o"), "output userData port (string)").asString());

   /* open the ports */
   if (!_userIDIn.open(_userIDInName.c_str())){
      std::cout << getName() << ": unable to open port " << _userIDInName << std::endl;
      return false;
   }
   if (!_updatedDataIn.open(_updatedDataInName.c_str())){
      std::cout << getName() << ": unable to open port " << _updatedDataInName << std::endl;
      return false;
   }

   if (!_userDataOut.open(_userDataOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _userDataOutName << std::endl;
      return false;
   }

   /* create the handler port */
   _handlerPortName = "/";
   _handlerPortName += getName();

   if (!_handlerPort.open(_handlerPortName.c_str())){
      std::cout << getName() << ":unable to open port " << _handlerPortName << std::endl;
      return true;
   }

   /* Create the YarpInterface used to communicate between the main code in Controller and the other components through the thread class */
   _userModelYarpInterface = new UserModelYarpInterface();

   /* Create the main class: Controller */ 
   _userModelController = new UserModelController(_userModelYarpInterface, _userIni);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _userModelThread = new UserModelThread(&_userIDIn, &_updatedDataIn, &_userDataOut);
   _userModelThread->setYarpInterface(_userModelYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _userModelYarpInterface->init(_userModelThread, _userModelController);
   
   _userModelThread->start();

   return true;
}

bool UserModel::interruptModule(){
   _userIDIn.interrupt();
   _updatedDataIn.interrupt();
   _userDataOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool UserModel::close(){
   /* close the ports */
   _userIDIn.close();
   _updatedDataIn.close();
   _userDataOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _userModelThread->stop();

   return true;
}

bool UserModel::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool UserModel::updateModule(){
   return true;
}

double UserModel::getPeriod(){
   return 0.1;
}
