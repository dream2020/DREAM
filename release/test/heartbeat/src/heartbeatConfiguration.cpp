/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Erik Billing, University of Skovde 
 * Email:   erik.billing@his.se 
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


#include "heartbeat.h"

bool Heartbeat::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("/heartbeat"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */

   _heartbeatOutName = getName(rf.check("heartbeatOut", Value("/heartbeat:o"), "output heartbeat port (string)").asString());

	_period = rf.check("period", Value(30), "heartbeat period (ms)").asInt();

   /* open the ports */
   if (!_heartbeatOut.open(_heartbeatOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _heartbeatOutName << std::endl;
      return false;
   }

   /* create the handler port */
   _handlerPortName = getName();

   if (!_handlerPort.open(_handlerPortName.c_str())){
      std::cout << getName() << ":unable to open port " << _handlerPortName << std::endl;
      return true;
   }
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _heartbeatThread = new HeartbeatThread(_period, &_heartbeatOut);
   _heartbeatThread->start();

   return true;
}

bool Heartbeat::interruptModule(){
	_heartbeatThread->stop();

   _heartbeatOut.interrupt();
   _handlerPort.interrupt();
   return true;
}

bool Heartbeat::close(){
	//_heartbeatThread->stop();
	
   /* close the ports */
   _heartbeatOut.close();
   _handlerPort.close();
   
   return true;
}

bool Heartbeat::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool Heartbeat::updateModule(){
   return true;
}

double Heartbeat::getPeriod(){
   return 0.1;
}
