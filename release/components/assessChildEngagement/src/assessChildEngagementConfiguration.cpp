/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Serge Thill, University of Skövde 
 * Email:   serge.thill@his.se 
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
  * 08/11/2016 Version 1.0: ... (Serge Thill)
  */ 

#include "assessChildEngagement.h"
#include "assessChildEngagementYarpInterface.h"
#include "assessChildEngagementController.h"

bool AssessChildEngagement::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("assessChildEngagement"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _identifyFaceExpressionInName = "/" +  getName(rf.check("_identifyFaceExpressionIn", Value("/identifyFaceExpression:i"), "Input identifyFaceExpression port (string)").asString());
   _checkMutualGazeInName = "/" +  getName(rf.check("_checkMutualGazeIn", Value("/checkMutualGaze:i"), "Input checkMutualGaze port (string)").asString());
   _getBodyInName = "/" +  getName(rf.check("_getBodyIn", Value("/getBody:i"), "Input getBody port (string)").asString());

   _engagementFlagOutName = "/" +  getName(rf.check("_engagementFlagOut", Value("/engagementFlag:o"), "output engagementFlag port (string)").asString());

   /* open the ports */
   if (!_identifyFaceExpressionIn.open(_identifyFaceExpressionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _identifyFaceExpressionInName << std::endl;
      return false;
   }
   if (!_checkMutualGazeIn.open(_checkMutualGazeInName.c_str())){
      std::cout << getName() << ": unable to open port " << _checkMutualGazeInName << std::endl;
      return false;
   }
   if (!_getBodyIn.open(_getBodyInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getBodyInName << std::endl;
      return false;
   }

   if (!_engagementFlagOut.open(_engagementFlagOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _engagementFlagOutName << std::endl;
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
   _assessChildEngagementYarpInterface = new AssessChildEngagementYarpInterface();

   /* Create the main class: Controller */ 
   _assessChildEngagementController = new AssessChildEngagementController(_assessChildEngagementYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _assessChildEngagementThread = new AssessChildEngagementThread(&_identifyFaceExpressionIn, &_checkMutualGazeIn, &_getBodyIn, &_engagementFlagOut);
   _assessChildEngagementThread->setYarpInterface(_assessChildEngagementYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _assessChildEngagementYarpInterface->init(_assessChildEngagementThread, _assessChildEngagementController);
   
   _assessChildEngagementThread->start();

   /* Parameters that can be tweaked */
   _assessChildEngagementController->body_thr = rf.check("centre_tolerance", Value(40), "off-centre position tolerance for the child (in mm)").asDouble();
   _assessChildEngagementController->centre_pos = rf.check("centre_pos", Value(0), "center position of the child (in mm)").asDouble();


   return true;
}

bool AssessChildEngagement::interruptModule(){
   _identifyFaceExpressionIn.interrupt();
   _checkMutualGazeIn.interrupt();
   _getBodyIn.interrupt();
   _engagementFlagOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool AssessChildEngagement::close(){
   /* close the ports */
   _identifyFaceExpressionIn.close();
   _checkMutualGazeIn.close();
   _getBodyIn.close();
   _engagementFlagOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _assessChildEngagementThread->stop();

   return true;
}

bool AssessChildEngagement::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool AssessChildEngagement::updateModule(){
   return true;
}

double AssessChildEngagement::getPeriod(){
   return 0.1;
}
