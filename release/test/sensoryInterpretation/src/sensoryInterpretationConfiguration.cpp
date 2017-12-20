/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  David Vernon, University of Skövde 
 * Email:   david.vernon@his.se 
 * Website: www.dream2020.eu  
 *
 * This program comes with ABSOLUTELY NO WARRANTY 
 */
  

/*
 * Audit Trail
 * -----------
 * 23/11/14  First version completed (David Vernon)
 *
 */ 

#include "sensoryInterpretation.h"

bool SensoryInterpretation::configure(yarp::os::ResourceFinder &rf)
{    
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("sensoryInterpretation"), 
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
   
   checkMutualGazeOutputPortName         = "/";
   checkMutualGazeOutputPortName        += getName(rf.check("checkMutualGazeOutputPort", Value("/checkMutualGaze:o"), "checkMutualGaze Output Port (string)").asString());
  
   getArmAngleOutputPortName             = "/";
   getArmAngleOutputPortName            += getName(rf.check("getArmAngleOutputPort", Value("/getArmAngle:o"), "getArmAngle Output Port (string)").asString());

   getBodyOutputPortName                 = "/";
   getBodyOutputPortName                += getName(rf.check("getBodyOutputPort", Value("/getBody:o"), "getBody Output Port (string)").asString());

   getBodyPoseOutputPortName             = "/";
   getBodyPoseOutputPortName            += getName(rf.check("getBodyPoseOutputPort", Value("/getBodyPose:o"), "getBodyPose Output Port port (string)").asString());

   getEyeGazeInputPortName               = "/";
   getEyeGazeInputPortName              += getName(rf.check("getEyeGazeInputPort", Value("/getEyeGaze:i"), "getEyeGaze Input Port (string)").asString());
    
   getEyeGazeOutputPortName              = "/";
   getEyeGazeOutputPortName             += getName(rf.check("getEyeGazeOutputPort", Value("/getEyeGaze:o"), "getEyeGaze Output Port (string)").asString());

   getEyesOutputPortName                 = "/";
   getEyesOutputPortName                += getName(rf.check("getEyesOutputPort", Value("/getEyes:o"), "getEyes Output Port (string)").asString());
    
   getFacesOutputPortName                = "/";
   getFacesOutputPortName               += getName(rf.check("getFacesOutputPort", Value("/getFaces:o"), "getFaces Output Port (string)").asString());

   getGripLocationInputPortName          = "/";
   getGripLocationInputPortName         += getName(rf.check("getGripLocationInputPort", Value("/getGripLocation:i"), "getGripLocation Input Port (string)").asString());

   getGripLocationOutputPortName         = "/";
   getGripLocationOutputPortName        += getName(rf.check("getGripLocationOutputPort", Value("/getGripLocation:o"), "getGripLocation Output Port (string)").asString());

   getHandsOutputPortName                = "/";
   getHandsOutputPortName               += getName(rf.check("getHandsOutputPort", Value("/getHands:o"), "getHands Output Port (string)").asString());

   getHeadOutputPortName                 = "/";
   getHeadOutputPortName                += getName(rf.check("getHeadOutputPort", Value("/getHead:o"), "getHead Output Port (string)").asString());

   getHeadGazeInputPortName              = "/";
   getHeadGazeInputPortName             += getName(rf.check("getHeadGazeInputPort", Value("/getHeadGaze:i"), "getHeadGaze Input Port (string)").asString());

   getHeadGazeOutputPortName             = "/";
   getHeadGazeOutputPortName            += getName(rf.check("getHeadGazeOutputPort", Value("/getHeadGaze:o"), "getHeadGaze Output Port (string)").asString());

   getObjectsOutputPortName              = "/";
   getObjectsOutputPortName             += getName(rf.check("getObjectsOutputPortName", Value("/getObjects:o"), "getObjects Output Port (string)").asString());

   getObjectsInputPortName               = "/";
   getObjectsInputPortName              += getName(rf.check("getObjectsInputPort", Value("/getObjects:i"), "getObjects Input Port (string)").asString());

   getObjectTableDistanceInputPortName   = "/";
   getObjectTableDistanceInputPortName  += getName(rf.check("getObjectTableDistanceInputPort", Value("/getObjectTableDistance:i"), "getObjectTableDistance Input Port (string)").asString());

   getObjectTableDistanceOutputPortName  = "/";
   getObjectTableDistanceOutputPortName += getName(rf.check("getObjectTableDistanceOutputPort", Value("/getObjectTableDistance:o"), "getObjectTableDistance Output Port (string)").asString());

   getSoundDirectionInputPortName        = "/";
   getSoundDirectionInputPortName       += getName(rf.check("getSoundDirectionInputPort", Value("/getSoundDirection:i"), "getSoundDirection Input Port (string)").asString());

   getSoundDirectionOutputPortName       = "/";
   getSoundDirectionOutputPortName      += getName(rf.check("getSoundDirectionOutputPort", Value("/getSoundDirection:o"), "getSoundDirection Output Port (string)").asString());

   identifyFaceInputPortName             = "/";
   identifyFaceInputPortName            += getName(rf.check("identifyFaceInputPort", Value("/identifyFace:i"), "identifyFace Input Port (string)").asString());
 
   identifyFaceOutputPortName            = "/";
   identifyFaceOutputPortName           += getName(rf.check("identifyFaceOutputPort", Value("/identifyFace:o"), "identifyFace Output Port (string)").asString());

   identifyFaceExpressionInputPortName   = "/";
   identifyFaceExpressionInputPortName  += getName(rf.check("identifyFaceExpressionInputPort", Value("/identifyFaceExpression:i"), "identifyFaceExpression Input Port (string)").asString());

   identifyFaceExpressionOutputPortName  = "/";
   identifyFaceExpressionOutputPortName += getName(rf.check("identifyFaceExpressionOutputPort", Value("/identifyFaceExpression:o"), "identifyFaceExpression Output Port (string)").asString());

   identifyObjectInputPortName           = "/";
   identifyObjectInputPortName          += getName(rf.check("identifyObjectInputPort", Value("/identifyObject:i"), "identifyObject Input Port (string)").asString());
 
   identifyObjectOutputPortName          = "/";
   identifyObjectOutputPortName         += getName(rf.check("identifyObjectOutputPort", Value("/identifyObject:o"), "identifyObject Output Port (string)").asString());
 
   identifyTrajectoryInputPortName       = "/";
   identifyTrajectoryInputPortName      += getName(rf.check("identifyTrajectoryInputPort", Value("/identifyTrajectory:i"), "identifyTrajectory Input Port (string)").asString());
   
   identifyTrajectoryOutputPortName      = "/";
   identifyTrajectoryOutputPortName     += getName(rf.check("identifyTrajectoryOutputPortName", Value("/identifyTrajectory:o"), "identifyTrajectory Output Port (string)").asString());
  
   identifyVoiceOutputPortName           = "/";
   identifyVoiceOutputPortName          += getName(rf.check("identifyVoiceOutputPort", Value("/identifyVoice:o"), "identifyVoice Output Port (string)").asString());
   
   recognizeSpeechOutputPortName         = "/";
   recognizeSpeechOutputPortName        += getName(rf.check("recognizeSpeechOutputPortName", Value("/recognizeSpeech:o"), "recognizeSpeech Output Port port (string)").asString());
   
   trackFaceInputPortName                = "/";
   trackFaceInputPortName               += getName(rf.check("trackFaceInputPort", Value("/trackFace:i"), "trackFace Input Port (string)").asString());
   
   trackFaceOutputPortName               = "/";
   trackFaceOutputPortName              += getName(rf.check("trackFaceOutputPort", Value("/trackFace:o"), "trackFace Output Port (string)").asString());
      
   trackHandInputPortName                = "/";
   trackHandInputPortName               += getName(rf.check("trackHandInputPort", Value("/trackHand:i"), "trackHand Input Port (string)").asString());
   
   trackHandOutputPortName               = "/";
   trackHandOutputPortName              += getName(rf.check("trackHandOutputPort", Value("/trackHand:o"), "trackHand Output Port (string)").asString());
   
   trackObjectInputPortName              = "/";
   trackObjectInputPortName             += getName(rf.check("trackObjectInputPort", Value("/trackObject:i"), "trackObject Input Port (string)").asString());
   
   trackObjectOutputPortName             = "/";
   trackObjectOutputPortName            += getName(rf.check("trackObjectOutputPort", Value("/trackObject:o"), "trackObject Output Port (string)").asString());
   
   /* do all initialization here */
     
   /* open ports  */ 
 
   if (!checkMutualGazeOut.open(checkMutualGazeOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << checkMutualGazeOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getArmAngleOut.open(getArmAngleOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getArmAngleOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getBodyOut.open(getBodyOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getBodyOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!getBodyPoseOut.open(getBodyPoseOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getBodyPoseOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getEyeGazeIn.open(getEyeGazeInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getEyeGazeInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getEyeGazeOut.open(getEyeGazeOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getEyeGazeOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getEyesOut.open(getEyesOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getEyesOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getFacesOut.open(getFacesOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getFacesOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getGripLocationIn.open(getGripLocationInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getGripLocationInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getGripLocationOut.open(getGripLocationOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getGripLocationOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHandsOut.open(getHandsOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHandsOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHeadOut.open(getHeadOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHeadOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHeadGazeIn.open(getHeadGazeInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHeadGazeInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHeadGazeOut.open(getHeadGazeOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHeadGazeOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getObjectsIn.open(getObjectsInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectsOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getObjectsOut.open(getObjectsOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectsInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getObjectTableDistanceIn.open(getObjectTableDistanceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectTableDistanceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!getObjectTableDistanceOut.open(getObjectTableDistanceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getObjectTableDistanceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getSoundDirectionIn.open(getSoundDirectionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSoundDirectionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getSoundDirectionOut.open(getSoundDirectionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSoundDirectionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyFaceIn.open(identifyFaceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyFaceOut.open(identifyFaceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyFaceExpressionIn.open(identifyFaceExpressionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceExpressionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!identifyFaceExpressionOut.open(identifyFaceExpressionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceExpressionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyObjectIn.open(identifyObjectInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyObjectInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyObjectOut.open(identifyObjectOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyObjectOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyTrajectoryIn.open(identifyTrajectoryInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyTrajectoryInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyTrajectoryOut.open(identifyTrajectoryOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyTrajectoryOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!identifyVoiceOut.open(identifyVoiceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyVoiceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!recognizeSpeechOut.open(recognizeSpeechOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << recognizeSpeechOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!trackFaceIn.open(trackFaceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackFaceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }


   if (!trackFaceOut.open(trackFaceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackFaceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   
   if (!trackHandIn.open(trackHandInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackHandInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }


   if (!trackHandOut.open(trackHandOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackHandOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!trackObjectIn.open(trackObjectInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackObjectInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!trackObjectOut.open(trackObjectOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << trackObjectOutputPortName << endl;
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

   sensoryInterpretationThread = new SensoryInterpretationThread(
                                                                 &verbose,             // simulation control parameters
                                                                 &cycle,
                                                                 &cyclePeriod,
                                                                 &eventRate,
                                                                 &checkMutualGazeOut,  // functional parameters
                                                                 &getArmAngleOut,
                                                                 &getBodyOut,
                                                                 &getBodyPoseOut,
                                                                 &getEyeGazeIn,
                                                                 &getEyeGazeOut, 
                                                                 &getEyesOut,
                                                                 &getFacesOut,
                                                                 &getGripLocationIn, 
                                                                 &getGripLocationOut,
                                                                 &getHandsOut,
                                                                 &getHeadOut,
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
                                                                 &identifyVoiceOut,
                                                                 &recognizeSpeechOut, 
                                                                 &trackFaceIn,
                                                                 &trackFaceOut, 
                                                                 &trackHandIn,
                                                                 &trackHandOut,  
                                                                 &trackObjectIn,
                                                                 &trackObjectOut);

   /* now start the thread to do the work */

   sensoryInterpretationThread->start(); // this calls threadInit() and it if returns true, it then calls run()

   return true ;      // let the RFModule know everything went well
                      // so that it will then run the module
}


bool SensoryInterpretation::interruptModule()
{
   /* NB you must stop the thread before interrupting the ports and then closing them
      If you don't then gyarpmanger will not be able to stop the module and you will have
      to resort to killing it                                                              */

   sensoryInterpretationThread->stop();

   checkMutualGazeOut.interrupt();
   getArmAngleOut.interrupt();
   getBodyOut.interrupt();
   getBodyPoseOut.interrupt(); 
   getEyeGazeIn.interrupt();
   getEyeGazeOut.interrupt(); 
   getEyesOut.interrupt();
   getFacesOut.interrupt(); 
   getGripLocationIn.interrupt(); 
   getGripLocationOut.interrupt(); 
   getHandsOut.interrupt();
   getHeadOut.interrupt();
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
   identifyVoiceOut.interrupt();
   recognizeSpeechOut.interrupt(); 
   trackFaceIn.interrupt();
   trackFaceOut.interrupt();  
   trackHandIn.interrupt();
   trackHandOut.interrupt();  
   trackObjectIn.interrupt();  
   trackObjectOut.interrupt(); 

   handlerPort.interrupt();

   return true;
}


bool SensoryInterpretation::close()
{
   checkMutualGazeOut.close();
   getArmAngleOut.close();
   getBodyOut.close();
   getBodyPoseOut.close(); 
   getEyeGazeIn.close();
   getEyeGazeOut.close(); 
   getEyesOut.close();
   getFacesOut.close(); 
   getGripLocationIn.close(); 
   getGripLocationOut.close(); 
   getHandsOut.close();
   getHeadOut.close();
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
   identifyVoiceOut.close();
   recognizeSpeechOut.close(); 
   trackFaceIn.close();
   trackFaceOut.close();  
   trackHandIn.close();
   trackHandOut.close();  
   trackObjectIn.close();  
   trackObjectOut.close(); 

   handlerPort.close();

   return true;
}


bool SensoryInterpretation::respond(const Bottle& command, Bottle& reply) 
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

bool SensoryInterpretation::updateModule()
{
   return true;
}



double SensoryInterpretation::getPeriod()
{
   /* module periodicity (seconds), called implicitly by sensoryInterpretation */
    
   return 0.1;
}
