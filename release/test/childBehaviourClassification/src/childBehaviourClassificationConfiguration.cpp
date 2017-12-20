/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  David Vernon, University of Skövde 
 * Email:   david.vernon@his.se 
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
 * 28/11/14  First version completed (David Vernon)
 *
 */ 

#include "childBehaviourClassification.h"

bool ChildBehaviourClassification::configure(yarp::os::ResourceFinder &rf)
{    
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("childBehaviourClassification"), 
                           "module name (string)").asString();

   /*
    * before continuing, set the module name before getting any other parameters, 
    * specifically the port names which are dependent on the module name
    */
   
   setName(moduleName.c_str());

   /* now, get the rest of the parameters */

   /* get the simulation control parameters first */

   verboseParameter                      = rf.check("verbose",      Value("on"), "Key value (string)").asString();
   cycleParameter                        = rf.check("cycle",        Value("on"), "Key value (string)").asString();
   cyclePeriod                           = rf.check("cycle_period", Value(0.5), "Key value (double)").asDouble();
   eventRate                             = rf.check("event_rate",   Value(20.0), "Key value (double)").asDouble();

   if (verboseParameter == "on") 
      verbose = true;
   else 
      verbose = false;

   if (cycleParameter == "on") 
      cycle = true;
   else 
      cycle = false;


   /* get the name of the input and output ports, automatically prefixing the module name by using getName() */
   
   checkMutualGazeInputPortName         = "/";
   checkMutualGazeInputPortName        += getName(rf.check("checkMutualGazeInputPort", Value("/checkMutualGaze:i"), "checkMutualGaze Input Port (string)").asString());
  
   getArmAngleInputPortName             = "/";
   getArmAngleInputPortName            += getName(rf.check("getArmAngleInputPort", Value("/getArmAngle:i"), "getArmAngle Input Port (string)").asString());

   getBodyInputPortName                 = "/";
   getBodyInputPortName                += getName(rf.check("getBodyInputPort", Value("/getBody:i"), "getBody Input Port (string)").asString());

   getBodyPoseInputPortName             = "/";
   getBodyPoseInputPortName            += getName(rf.check("getBodyPoseInputPort", Value("/getBodyPose:i"), "getBodyPose Input Port port (string)").asString());
    
   getEyeGazeInputPortName              = "/";
   getEyeGazeInputPortName             += getName(rf.check("getEyeGazeInputPort", Value("/getEyeGaze:i"), "getEyeGaze Input Port (string)").asString());

   getEyesInputPortName                 = "/";
   getEyesInputPortName                += getName(rf.check("getEyesInputPort", Value("/getEyes:i"), "getEyes Input Port (string)").asString());
    
   getFacesInputPortName                = "/";
   getFacesInputPortName               += getName(rf.check("getFacesInputPort", Value("/getFaces:i"), "getFaces Input Port (string)").asString());

   getGripLocationInputPortName         = "/";
   getGripLocationInputPortName        += getName(rf.check("getGripLocationInputPort", Value("/getGripLocation:i"), "getGripLocation Input Port (string)").asString());

   getHandsInputPortName                = "/";
   getHandsInputPortName               += getName(rf.check("getHandsInputPort", Value("/getHands:i"), "getHands Input Port (string)").asString());

   getHeadInputPortName                 = "/";
   getHeadInputPortName                += getName(rf.check("getHeadInputPort", Value("/getHead:i"), "getHead Input Port (string)").asString());

   getHeadGazeInputPortName             = "/";
   getHeadGazeInputPortName            += getName(rf.check("getHeadGazeInputPort", Value("/getHeadGaze:i"), "getHeadGaze Input Port (string)").asString());

   getObjectsInputPortName              = "/";
   getObjectsInputPortName             += getName(rf.check("getObjectsInputPortName", Value("/getObjects:i"), "getObjects Input Port (string)").asString());

   getObjectTableDistanceInputPortName  = "/";
   getObjectTableDistanceInputPortName += getName(rf.check("getObjectTableDistanceInputPort", Value("/getObjectTableDistance:i"), "getObjectTableDistance Input Port (string)").asString());

   getSoundDirectionInputPortName       = "/";
   getSoundDirectionInputPortName      += getName(rf.check("getSoundDirectionInputPort", Value("/getSoundDirection:i"), "getSoundDirection Input Port (string)").asString());

   identifyFaceInputPortName            = "/";
   identifyFaceInputPortName           += getName(rf.check("identifyFaceInputPort", Value("/identifyFace:i"), "identifyFace Input Port (string)").asString());

   identifyFaceExpressionInputPortName  = "/";
   identifyFaceExpressionInputPortName += getName(rf.check("identifyFaceExpressionInputPort", Value("/identifyFaceExpression:i"), "identifyFaceExpression Input Port (string)").asString());
 
   identifyObjectInputPortName          = "/";
   identifyObjectInputPortName         += getName(rf.check("identifyObjectInputPort", Value("/identifyObject:i"), "identifyObject Input Port (string)").asString());
 
   identifyTrajectoryInputPortName      = "/";
   identifyTrajectoryInputPortName     += getName(rf.check("identifyTrajectoryInputPort", Value("/identifyTrajectory:i"), "identifyTrajectory Input Port (string)").asString());
  
   identifyVoiceInputPortName           = "/";
   identifyVoiceInputPortName          += getName(rf.check("identifyVoiceInputPort", Value("/identifyVoice:i"), "identifyVoice Input Port (string)").asString());
   
   recognizeSpeechInputPortName         = "/";
   recognizeSpeechInputPortName        += getName(rf.check("recognizeSpeechInputPort", Value("/recognizeSpeech:i"), "recognizeSpeech Input Port port (string)").asString());
   
   trackFaceInputPortName               = "/";
   trackFaceInputPortName              += getName(rf.check("trackFaceInputPort", Value("/trackFace:i"), "trackFace Input Port (string)").asString());
      
   trackHandInputPortName               = "/";
   trackHandInputPortName              += getName(rf.check("trackHandInputPort", Value("/trackHand:i"), "trackHand Input Port (string)").asString());
     
   trackObjectInputPortName             = "/";
   trackObjectInputPortName            += getName(rf.check("trackObjectInputPort", Value("/trackObject:i"), "trackObject Input Port (string)").asString());
   
   getChildBehaviourOutputPortName      = "/";
   getChildBehaviourOutputPortName     += getName(rf.check("getChildBehaviourOutputPort", Value("/getChildBehaviour:o"), "getChildBehaviour Output Port (string)").asString());
    
   getChildMotivationOutputPortName     = "/";
   getChildMotivationOutputPortName    += getName(rf.check("getChildMotivationOutputPort", Value("/getChildMotivation:o"), "getChildMotivation Output Port (string)").asString());
     
   getChildPerformanceOutputPortName      = "/";
   getChildPerformanceOutputPortName     += getName(rf.check("getChildPerformanceOutputPort", Value("/getChildPerformance:o"), "getChildPerformance Output Port (string)").asString());
 
   /* do all initialization here */
     
   /* open ports  */ 
 
   if (!checkMutualGazeIn.open(checkMutualGazeInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << checkMutualGazeInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getArmAngleIn.open(getArmAngleInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getArmAngleInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getBodyIn.open(getBodyInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getBodyInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!getBodyPoseIn.open(getBodyPoseInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getBodyPoseInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getEyeGazeIn.open(getEyeGazeInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getEyeGazeInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getEyesIn.open(getEyesInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getEyesInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getFacesIn.open(getFacesInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getFacesInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getGripLocationIn.open(getGripLocationInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getGripLocationInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHandsIn.open(getHandsInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHandsInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHeadIn.open(getHeadInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHeadInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHeadGazeIn.open(getHeadGazeInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHeadGazeInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getObjectsIn.open(getObjectsInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectsInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getObjectTableDistanceIn.open(getObjectTableDistanceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectTableDistanceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getSoundDirectionIn.open(getSoundDirectionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSoundDirectionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyFaceIn.open(identifyFaceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyFaceExpressionIn.open(identifyFaceExpressionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceExpressionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyObjectIn.open(identifyObjectInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyObjectInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyTrajectoryIn.open(identifyTrajectoryInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyTrajectoryInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyVoiceIn.open(identifyVoiceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyVoiceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!recognizeSpeechIn.open(recognizeSpeechInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << recognizeSpeechInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!trackFaceIn.open(trackFaceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackFaceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!trackHandIn.open(trackHandInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackHandInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!trackObjectIn.open(trackObjectInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackObjectInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }


   if (!getChildBehaviourOut.open(getChildBehaviourOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getChildBehaviourOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getChildMotivationOut.open(getChildMotivationOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getChildMotivationOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getChildPerformanceOut.open(getChildPerformanceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getChildPerformanceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   /*
    * attach a port of the same name as the module (prefixed with a /) to the module
    * so that messages received from the port are redirected to the respond method
    */

   handlerPortName =  "/";
   handlerPortName += getName();         // use getName() rather than a literal 
 
   if (!handlerPort.open(handlerPortName.c_str())) {           
      cout << getName() << ": Unable to open port " << handlerPortName << endl;  
      return false;
   }

   attach(handlerPort);                  // attach to port
 
   /* create the thread and pass pointers to the module parameters */

   childBehaviourClassificationThread = new ChildBehaviourClassificationThread(
                                                                 &verbose,             // simulation control parameters
                                                                 &cycle,
                                                                 &cyclePeriod,
                                                                 &eventRate,
                                                                 &checkMutualGazeIn,  // functional parameters
                                                                 &getArmAngleIn,
                                                                 &getBodyIn,
                                                                 &getBodyPoseIn,
                                                                 &getEyeGazeIn,
                                                                 &getEyesIn,
                                                                 &getFacesIn,
                                                                 &getGripLocationIn,
                                                                 &getHandsIn,
                                                                 &getHeadIn,
                                                                 &getHeadGazeIn,
                                                                 &getObjectsIn,
                                                                 &getObjectTableDistanceIn,
                                                                 &getSoundDirectionIn,
                                                                 &identifyFaceIn,
                                                                 &identifyFaceExpressionIn,  
                                                                 &identifyObjectIn,
                                                                 &identifyTrajectoryIn, 
                                                                 &identifyVoiceIn,
                                                                 &recognizeSpeechIn, 
                                                                 &trackFaceIn,
                                                                 &trackHandIn, 
                                                                 &trackObjectIn,
                                                                 &getChildBehaviourOut,
                                                                 &getChildMotivationOut,
                                                                 &getChildPerformanceOut);

   /* now start the thread to do the work */

   childBehaviourClassificationThread->start(); // this calls threadInit() and it if returns true, it then calls run()

   return true ;      // let the RFModule know everything went well
                      // so that it will then run the module
}


bool ChildBehaviourClassification::interruptModule()
{
   /* NB you must stop the thread before interrupting the ports and then closing them
      If you don't then gyarpmanger will not be able to stop the module and you will have
      to resort to killing it                                                              */

   childBehaviourClassificationThread->stop();

   checkMutualGazeIn.interrupt();
   getArmAngleIn.interrupt();
   getBodyIn.interrupt();
   getBodyPoseIn.interrupt(); 
   getEyeGazeIn.interrupt();
   getEyesIn.interrupt();
   getFacesIn.interrupt(); 
   getGripLocationIn.interrupt(); 
   getHandsIn.interrupt();
   getHeadIn.interrupt();
   getHeadGazeIn.interrupt(); 
   getObjectsIn.interrupt(); 
   getObjectTableDistanceIn.interrupt();  
   getSoundDirectionIn.interrupt();  
   identifyFaceIn.interrupt();
   identifyFaceExpressionIn.interrupt();  
   identifyObjectIn.interrupt(); 
   identifyTrajectoryIn.interrupt();  
   identifyVoiceIn.interrupt();
   recognizeSpeechIn.interrupt(); 
   trackFaceIn.interrupt(); 
   trackHandIn.interrupt(); 
   trackObjectIn.interrupt();  
   getChildBehaviourOut.interrupt(); 
   getChildMotivationOut.interrupt(); 
   getChildPerformanceOut.interrupt(); 
   handlerPort.interrupt();

   return true;
}


bool ChildBehaviourClassification::close()
{
   checkMutualGazeIn.close();
   getArmAngleIn.close();
   getBodyIn.close();
   getBodyPoseIn.close(); 
   getEyeGazeIn.close(); 
   getEyesIn.close();
   getFacesIn.close(); 
   getGripLocationIn.close(); 
   getHandsIn.close();
   getHeadIn.close();
   getHeadGazeIn.close(); 
   getObjectsIn.close();
   getObjectTableDistanceIn.close(); 
   getSoundDirectionIn.close();
   identifyFaceIn.close();
   identifyFaceExpressionIn.close();   
   identifyObjectIn.close();
   identifyTrajectoryIn.close();   
   identifyVoiceIn.close();
   recognizeSpeechIn.close(); 
   trackFaceIn.close();  
   trackHandIn.close();
   trackObjectIn.close();  
   getChildBehaviourOut.close(); 
   getChildMotivationOut.close(); 
   getChildPerformanceOut.close(); 

   handlerPort.close();

   return true;
}


bool ChildBehaviourClassification::respond(const Bottle& command, Bottle& reply) 
{
  string helpMessage =  string(getName().c_str()) + 
                        " commands are: verbose on|off, cycle on|off, cycle_period <n>, event_rate <n>";

  reply.clear(); 

   if (command.get(0).asString()=="help") {
	   reply.addString(helpMessage);
   }
   else if (command.get(0).asString()=="verbose") {
      verboseParameter = command.get(1).asString(); // set parameter value
      if (verboseParameter == "on") {
         verbose = true;
         reply.addString("Verbose on");
      } 
      else if (verboseParameter == "off") {
        verbose = false;
        reply.addString("Verbose off");
      }
      else {
         reply.addString("Invalid command");
      }
   }
   else if (command.get(0).asString()=="cycle") {
      cycleParameter = command.get(1).asString(); // set parameter value
      if (cycleParameter == "on") {
         cycle = true;
         reply.addString("Cycle on");
      } 
      else if (cycleParameter == "off") {
        cycle = false;
        reply.addString("Cycle off");
      }
      else {
         reply.addString("Invalid command");
      }
   }
   else if (command.get(0).asString()=="cycle_period") {
      cyclePeriod = command.get(1).asDouble(); // set parameter value
      reply.addString("Cycle period set");
   }
   else if (command.get(0).asString()=="event_rate") {
      eventRate = command.get(1).asDouble(); // set parameter value
      reply.addString("Event rate set");
   }
   else {
      reply.addString("Invalid command");
   }
   return true;
}


/* Called periodically every getPeriod() seconds */

bool ChildBehaviourClassification::updateModule()
{
   return true;
}



double ChildBehaviourClassification::getPeriod()
{
   /* module periodicity (seconds), called implicitly by childBehaviourClassification */
    
   return 0.1;
}
