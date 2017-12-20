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
 * 04/12/14  First version completed (David Vernon)
 *
 */ 

#include "cognitiveControl.h"

bool CognitiveControl::configure(yarp::os::ResourceFinder &rf)
{    
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("cognitiveControl"), 
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
   
   checkMutualGazeInputPortName          = "/";
   checkMutualGazeInputPortName         += getName(rf.check("checkMutualGazeInputPort", Value("/checkMutualGaze:i"), "checkMutualGaze Input Port (string)").asString());
  
   getArmAngleInputPortName              = "/";
   getArmAngleInputPortName             += getName(rf.check("getArmAngleInputPort", Value("/getArmAngle:i"), "getArmAngle Input Port (string)").asString());

   getBodyInputPortName                  = "/";
   getBodyInputPortName                 += getName(rf.check("getBodyInputPort", Value("/getBody:i"), "getBody Input Port (string)").asString());

   getBodyPoseInputPortName              = "/";
   getBodyPoseInputPortName             += getName(rf.check("getBodyPoseInputPort", Value("/getBodyPose:i"), "getBodyPose Input Port port (string)").asString());

   getEyeGazeOutputPortName              = "/";
   getEyeGazeOutputPortName             += getName(rf.check("getEyeGazeOutputPort", Value("/getEyeGaze:o"), "getEyeGaze Output Port (string)").asString());
    
   getEyeGazeInputPortName               = "/";
   getEyeGazeInputPortName              += getName(rf.check("getEyeGazeInputPort", Value("/getEyeGaze:i"), "getEyeGaze Input Port (string)").asString());

   getEyesInputPortName                  = "/";
   getEyesInputPortName                 += getName(rf.check("getEyesInputPort", Value("/getEyes:i"), "getEyes Input Port (string)").asString());
    
   getFacesInputPortName                 = "/";
   getFacesInputPortName                += getName(rf.check("getFacesInputPort", Value("/getFaces:i"), "getFaces Input Port (string)").asString());

   getGripLocationOutputPortName         = "/";
   getGripLocationOutputPortName        += getName(rf.check("getGripLocationOutputPort", Value("/getGripLocation:o"), "getGripLocation Output Port (string)").asString());

   getGripLocationInputPortName          = "/";
   getGripLocationInputPortName         += getName(rf.check("getGripLocationInputPort", Value("/getGripLocation:i"), "getGripLocation Input Port (string)").asString());

   getHandsInputPortName                 = "/";
   getHandsInputPortName                += getName(rf.check("getHandsInputPort", Value("/getHands:i"), "getHands Input Port (string)").asString());

   getHeadInputPortName                  = "/";
   getHeadInputPortName                 += getName(rf.check("getHeadInputPort", Value("/getHead:i"), "getHead Input Port (string)").asString());

   getHeadGazeOutputPortName             = "/";
   getHeadGazeOutputPortName            += getName(rf.check("getHeadGazeOutputPort", Value("/getHeadGaze:o"), "getHeadGaze Output Port (string)").asString());

   getHeadGazeInputPortName              = "/";
   getHeadGazeInputPortName             += getName(rf.check("getHeadGazeInputPort", Value("/getHeadGaze:i"), "getHeadGaze Input Port (string)").asString());

   getObjectsInputPortName               = "/";
   getObjectsInputPortName              += getName(rf.check("getObjectsInputPortName", Value("/getObjects:i"), "getObjects Input Port (string)").asString());

   getObjectsOutputPortName              = "/";
   getObjectsOutputPortName             += getName(rf.check("getObjectsOutputPort", Value("/getObjects:o"), "getObjects Input Port (string)").asString());

   getObjectTableDistanceOutputPortName  = "/";
   getObjectTableDistanceOutputPortName += getName(rf.check("getObjectTableDistanceOutputPort", Value("/getObjectTableDistance:o"), "getObjectTableDistance Output Port (string)").asString());

   getObjectTableDistanceInputPortName   = "/";
   getObjectTableDistanceInputPortName  += getName(rf.check("getObjectTableDistanceInputPort", Value("/getObjectTableDistance:i"), "getObjectTableDistance Input Port (string)").asString());

   getSoundDirectionOutputPortName       = "/";
   getSoundDirectionOutputPortName      += getName(rf.check("getSoundDirectionOutputPort", Value("/getSoundDirection:o"), "getSoundDirection Output Port (string)").asString());

   getSoundDirectionInputPortName        = "/";
   getSoundDirectionInputPortName       += getName(rf.check("getSoundDirectionInputPort", Value("/getSoundDirection:i"), "getSoundDirection Input Port (string)").asString());

   identifyFaceOutputPortName            = "/";
   identifyFaceOutputPortName           += getName(rf.check("identifyFaceOutputPort", Value("/identifyFace:o"), "identifyFace Input Port (string)").asString());
 
   identifyFaceInputPortName             = "/";
   identifyFaceInputPortName            += getName(rf.check("identifyFaceInputPort", Value("/identifyFace:i"), "identifyFace Input Port (string)").asString());

   identifyFaceExpressionOutputPortName  = "/";
   identifyFaceExpressionOutputPortName += getName(rf.check("identifyFaceExpressionOutputPort", Value("/identifyFaceExpression:o"), "identifyFaceExpression Output Port (string)").asString());

   identifyFaceExpressionInputPortName   = "/";
   identifyFaceExpressionInputPortName  += getName(rf.check("identifyFaceExpressionInputPort", Value("/identifyFaceExpression:i"), "identifyFaceExpression Input Port (string)").asString());

   identifyObjectOutputPortName          = "/";
   identifyObjectOutputPortName         += getName(rf.check("identifyObjectOutputPort", Value("/identifyObject:o"), "identifyObject Output Port (string)").asString());
 
   identifyObjectInputPortName           = "/";
   identifyObjectInputPortName          += getName(rf.check("identifyObjectInputPort", Value("/identifyObject:i"), "identifyObject Input Port (string)").asString());
 
   identifyTrajectoryOutputPortName      = "/";
   identifyTrajectoryOutputPortName     += getName(rf.check("identifyTrajectoryOutputPort", Value("/identifyTrajectory:o"), "identifyTrajectory Output Port (string)").asString());
   
   identifyTrajectoryInputPortName       = "/";
   identifyTrajectoryInputPortName      += getName(rf.check("identifyTrajectoryInputPortName", Value("/identifyTrajectory:i"), "identifyTrajectory Input Port (string)").asString());
  
   identifyVoiceInputPortName            = "/";
   identifyVoiceInputPortName           += getName(rf.check("identifyVoiceInputPort", Value("/identifyVoice:i"), "identifyVoice Input Port (string)").asString());
   
   recognizeSpeechInputPortName          = "/";
   recognizeSpeechInputPortName         += getName(rf.check("recognizeSpeechInputPortName", Value("/recognizeSpeech:i"), "recognizeSpeech Input Port port (string)").asString());
   
   trackFaceOutputPortName               = "/";
   trackFaceOutputPortName              += getName(rf.check("trackFaceOutputPort", Value("/trackFace:o"), "trackFace Output Port (string)").asString());
   
   trackFaceInputPortName                = "/";
   trackFaceInputPortName               += getName(rf.check("trackFaceInputPort", Value("/trackFace:i"), "trackFace Input Port (string)").asString());
      
   trackHandOutputPortName               = "/";
   trackHandOutputPortName              += getName(rf.check("trackHandOutputPort", Value("/trackHand:o"), "trackHand Output Port (string)").asString());
   
   trackHandInputPortName                = "/";
   trackHandInputPortName               += getName(rf.check("trackHandInputPort", Value("/trackHand:i"), "trackHand Input Port (string)").asString());
   
   trackObjectOutputPortName             = "/";
   trackObjectOutputPortName            += getName(rf.check("trackObjectOutputPort", Value("/trackObject:o"), "trackObject Output Port (string)").asString());
   
   trackObjectInputPortName              = "/";
   trackObjectInputPortName             += getName(rf.check("trackObjectInputPort", Value("/trackObject:i"), "trackObject Input Port (string)").asString());

   getChildBehaviourInputPortName        = "/";
   getChildBehaviourInputPortName       += getName(rf.check("getChildBehaviourInputPort", Value("/getChildBehaviour:i"), "getChildBehaviour Input Port (string)").asString());

   getChildMotivationInputPortName       = "/";
   getChildMotivationInputPortName      += getName(rf.check("getChildMotivationInputPort", Value("/getChildMotivation:i"), "getChildMotivation Input Port (string)").asString());

   getChildPerformanceInputPortName      = "/";
   getChildPerformanceInputPortName     += getName(rf.check("getChildPerformanceInputPort", Value("/getChildPerformance:i"), "getChildPerformance Input Port (string)").asString());

   gripInputPortName                     = "/";
   gripInputPortName                    += getName(rf.check("gripInputPort", Value("/grip:i"), "grip Input Port (string)").asString());

   moveHandInputPortName                 = "/";
   moveHandInputPortName                += getName(rf.check("moveHandInputPort", Value("/moveHand:i"), "moveHand Input Port (string)").asString());

   moveHeadInputPortName                 = "/";
   moveHeadInputPortName                += getName(rf.check("moveHeadInputPort", Value("/moveHead:i"), "moveHead Input Port (string)").asString());

   moveSequenceInputPortName             = "/";
   moveSequenceInputPortName            += getName(rf.check("moveSequenceInputPort", Value("/moveSequence:i"), "moveSequence Input Port (string)").asString());

   moveTorsoInputPortName                = "/";
   moveTorsoInputPortName               += getName(rf.check("moveTorsoInputPort", Value("/moveTorso:i"), "moveTorso Input Port (string)").asString());

   sayInputPortName                      = "/";
   sayInputPortName                     += getName(rf.check("sayInputPort", Value("/say:i"), "sayInput Input Port (string)").asString());

   enableRobotInputPortName              = "/";
   enableRobotInputPortName             += getName(rf.check("enableRobotInputPort", Value("/enableRobot:i"), "enableRobot Input Port (string)").asString());

   getInterventionStateOutputPortName    = "/";
   getInterventionStateOutputPortName   += getName(rf.check("getInterventionStateOutputPort", Value("/getInterventionState:o"), "getInterventionState Output Port (string)").asString());

  

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

   if (!getEyeGazeOut.open(getEyeGazeOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getEyeGazeOutputPortName << endl;
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

   if (!getGripLocationOut.open(getGripLocationOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getGripLocationOutputPortName << endl;
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

   if (!getHeadGazeOut.open(getHeadGazeOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHeadGazeOutputPortName << endl;
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

   if (!getObjectsOut.open(getObjectsOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectsOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getObjectTableDistanceOut.open(getObjectTableDistanceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectTableDistanceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!getObjectTableDistanceIn.open(getObjectTableDistanceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectTableDistanceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getSoundDirectionOut.open(getSoundDirectionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSoundDirectionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getSoundDirectionIn.open(getSoundDirectionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSoundDirectionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyFaceOut.open(identifyFaceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyFaceIn.open(identifyFaceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyFaceExpressionOut.open(identifyFaceExpressionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceExpressionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!identifyFaceExpressionIn.open(identifyFaceExpressionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceExpressionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyObjectOut.open(identifyObjectOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyObjectOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyObjectIn.open(identifyObjectInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyObjectInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyTrajectoryOut.open(identifyTrajectoryOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyTrajectoryOutputPortName << endl;
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

   if (!trackFaceOut.open(trackFaceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackFaceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }


   if (!trackFaceIn.open(trackFaceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackFaceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!trackHandOut.open(trackHandOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackHandOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!trackHandIn.open(trackHandInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackHandInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!trackObjectOut.open(trackObjectOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackObjectOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!trackObjectIn.open(trackObjectInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackObjectInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getChildBehaviourIn.open(getChildBehaviourInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getChildBehaviourInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!getChildMotivationIn.open(getChildMotivationInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getChildMotivationInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
     
   if (!getChildPerformanceIn.open(getChildPerformanceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getChildPerformanceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!gripIn.open(gripInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << gripInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!moveHandIn.open(moveHandInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << moveHandInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
      
   if (!moveHeadIn.open(moveHeadInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << moveHeadInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!moveSequenceIn.open(moveSequenceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << moveSequenceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!moveTorsoIn.open(moveTorsoInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << moveTorsoInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!sayIn.open(sayInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << sayInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!enableRobotIn.open(enableRobotInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << enableRobotInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!getInterventionStateOut.open(getInterventionStateOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getInterventionStateOutputPortName << endl;
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

   cognitiveControlThread = new CognitiveControlThread(
                                                       &verbose,             // simulation control parameters
                                                       &cycle,
                                                       &cyclePeriod,
                                                       &eventRate,
                                                       &checkMutualGazeIn,  // functional parameters
                                                       &getArmAngleIn,
                                                       &getBodyIn,
                                                       &getBodyPoseIn,
                                                       &getEyeGazeIn,
                                                       &getEyeGazeOut, 
                                                       &getEyesIn,
                                                       &getFacesIn,
                                                       &getGripLocationIn, 
                                                       &getGripLocationOut,
                                                       &getHandsIn,
                                                       &getHeadIn,
                                                       &getHeadGazeIn, 
                                                       &getHeadGazeOut, 
                                                       &getObjectsIn,
                                                       &getObjectsOut,
                                                       &getObjectTableDistanceIn,
                                                       &getObjectTableDistanceOut,
                                                       &getSoundDirectionIn,
                                                       &getSoundDirectionOut,  
                                                       &identifyFaceIn,
                                                       &identifyFaceOut,
                                                       &identifyFaceExpressionIn,  
                                                       &identifyFaceExpressionOut,  
                                                       &identifyObjectIn,
                                                       &identifyObjectOut,
                                                       &identifyTrajectoryIn, 
                                                       &identifyTrajectoryOut,  
                                                       &identifyVoiceIn,
                                                       &recognizeSpeechIn, 
                                                       &trackFaceIn,
                                                       &trackFaceOut, 
                                                       &trackHandIn,
                                                       &trackHandOut,  
                                                       &trackObjectIn,
                                                       &trackObjectOut,
                                                       &getChildBehaviourIn,
                                                       &getChildMotivationIn,
                                                       &getChildPerformanceIn,
                                                       &gripIn,
                                                       &moveHandIn,
                                                       &moveHeadIn,
                                                       &moveSequenceIn,
                                                       &moveTorsoIn,
                                                       &sayIn,
                                                       &enableRobotIn,
                                                       &getInterventionStateOut);

   /* now start the thread to do the work */

   cognitiveControlThread->start(); // this calls threadInit() and it if returns true, it then calls run()

   return true ;      // let the RFModule know everything went well
                      // so that it will then run the module
}


bool CognitiveControl::interruptModule()
{
   /* NB you must stop the thread before interrupting the ports and then closing them
      If you don't then gyarpmanger will not be able to stop the module and you will have
      to resort to killing it                                                              */

   cognitiveControlThread->stop();

   checkMutualGazeIn.interrupt();
   getArmAngleIn.interrupt();
   getBodyIn.interrupt();
   getBodyPoseIn.interrupt(); 
   getEyeGazeIn.interrupt();
   getEyeGazeOut.interrupt(); 
   getEyesIn.interrupt();
   getFacesIn.interrupt(); 
   getGripLocationIn.interrupt(); 
   getGripLocationOut.interrupt(); 
   getHandsIn.interrupt();
   getHeadIn.interrupt();
   getHeadGazeIn.interrupt(); 
   getHeadGazeOut.interrupt(); 
   getObjectsIn.interrupt();
   getObjectsOut.interrupt(); 
   getObjectTableDistanceIn.interrupt(); 
   getObjectTableDistanceOut.interrupt(); 
   getSoundDirectionIn.interrupt();
   getSoundDirectionOut.interrupt();  
   identifyFaceIn.interrupt();
   identifyFaceOut.interrupt();  
   identifyFaceExpressionIn.interrupt();  
   identifyFaceExpressionOut.interrupt();  
   identifyObjectIn.interrupt();
   identifyObjectOut.interrupt(); 
   identifyTrajectoryIn.interrupt();  
   identifyTrajectoryOut.interrupt();  
   identifyVoiceIn.interrupt();
   recognizeSpeechIn.interrupt(); 
   trackFaceIn.interrupt();
   trackFaceOut.interrupt();  
   trackHandIn.interrupt();
   trackHandOut.interrupt();  
   trackObjectIn.interrupt();  
   trackObjectOut.interrupt(); 
   getChildBehaviourIn.interrupt();
   getChildMotivationIn.interrupt();
   getChildPerformanceIn.interrupt();
   gripIn.interrupt();
   moveHandIn.interrupt();
   moveHeadIn.interrupt();
   moveSequenceIn.interrupt();
   moveTorsoIn.interrupt();
   sayIn.interrupt();
   enableRobotIn.interrupt();
   getInterventionStateOut.interrupt();

   handlerPort.interrupt();

   return true;
}


bool CognitiveControl::close()
{
   checkMutualGazeIn.close();
   getArmAngleIn.close();
   getBodyIn.close();
   getBodyPoseIn.close(); 
   getEyeGazeIn.close();
   getEyeGazeOut.close(); 
   getEyesIn.close();
   getFacesIn.close(); 
   getGripLocationIn.close(); 
   getGripLocationOut.close(); 
   getHandsIn.close();
   getHeadIn.close();
   getHeadGazeIn.close(); 
   getHeadGazeOut.close(); 
   getObjectsIn.close();
   getObjectsOut.close(); 
   getObjectTableDistanceIn.close(); 
   getObjectTableDistanceOut.close(); 
   getSoundDirectionIn.close();
   getSoundDirectionOut.close();  
   identifyFaceIn.close();
   identifyFaceOut.close();  
   identifyFaceExpressionIn.close();  
   identifyFaceExpressionOut.close();  
   identifyObjectIn.close();
   identifyObjectOut.close(); 
   identifyTrajectoryIn.close();  
   identifyTrajectoryOut.close();  
   identifyVoiceIn.close();
   recognizeSpeechIn.close(); 
   trackFaceIn.close();
   trackFaceOut.close();  
   trackHandIn.close();
   trackHandOut.close();  
   trackObjectIn.close();  
   trackObjectOut.close(); 
   getChildBehaviourIn.close();
   getChildMotivationIn.close(); 
   getChildPerformanceIn.close(); 
   gripIn.close(); 
   moveHandIn.close(); 
   moveHeadIn.close(); 
   moveSequenceIn.close(); 
   moveTorsoIn.close(); 
   sayIn.close(); 
   enableRobotIn.close(); 
   getInterventionStateOut.close(); 

   handlerPort.close();

   return true;
}


bool CognitiveControl::respond(const Bottle& command, Bottle& reply) 
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

bool CognitiveControl::updateModule()
{
   return true;
}



double CognitiveControl::getPeriod()
{
   /* module periodicity (seconds), called implicitly by cognitiveControl */
    
   return 0.1;
}
