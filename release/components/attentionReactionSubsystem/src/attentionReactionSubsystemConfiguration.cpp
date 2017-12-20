/* 
 * Copyright (C) 2015 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Pablo Gomez, Vrije Universiteit Brussel
 * Email:   pablo.gomez.esteban@vub.ac.be
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
 * 10/08/16  First version validated (Pablo Gómez)
 */ 

#include "attentionReactionSubsystem.h"
#include "attentionReactionYarpInterface.h"
#include "attentionReactionController.h"
#include "attentionSystem.h"
#include "eyeBlinking.h"

/* 
 * Configure method ... use it to do component coordination, 
 * i.e. to configure your component at runtime
 */

bool AttentionReactionSubsystem::configure(yarp::os::ResourceFinder &rf)
{    
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("attentionReactionSubsystem"), 
                           "module name (string)").asString();

   /*
    * before continuing, set the module name before getting any other parameters, 
    * specifically the port names which are dependent on the module name
    */
   
   setName(moduleName.c_str());

   /* get the name of the input and output ports, automatically prefixing the module name by using getName() */

   getFacesInputPortName  		            = "/";
   getFacesInputPortName		            += getName(rf.check("getFacesInputPort", Value("/getFaces:i"), "getFaces Input Port (string)").asString());

   getSoundDirectionInputPortName           = "/";
   getSoundDirectionInputPortName           += getName(rf.check("getSoundDirectionInputPort", Value("/getSoundDirection:i"), "getSoundDirection Input Port (string)").asString());

   attentionBiasInputPortName		        = "/";
   attentionBiasInputPortName	            += getName(rf.check("attentionBiasInputPort", Value("/attentionBias:i"), "attentionBias Input Port (string)").asString());

   therapistGazeCommandInputPortName		= "/";
   therapistGazeCommandInputPortName	    += getName(rf.check("therapistGazeCommandInputPort", Value("/therapistGazeCommand:i"), "therapistGazeCommand Input Port (string)").asString());
   
   checkMutualGazeInputPortName				= "/";
   checkMutualGazeInputPortName				+= getName(rf.check("checkMutualGazeInputPort", Value("/checkMutualGaze:i"), "checkMutualGaze Input Port (string)").asString());
   
   recognizeSpeechInputPortName				= "/";
   recognizeSpeechInputPortName				+= getName(rf.check("recognizeSpeechInputPort", Value("/recognizeSpeech:i"), "recognizeSpeech Input Port (string)").asString());

   affectiveStateInputPortName				= "/";
   affectiveStateInputPortName				+= getName(rf.check("affectiveStateInputPort", Value("/affectiveState:i"), "affectiveState Input Port (string)").asString());
   
   identifyFaceExpressionInputPortName		= "/";
   identifyFaceExpressionInputPortName		+= getName(rf.check("identifyFaceExpressionInputPort", Value("/identifyFaceExpression:i"), "identifyFaceExpression Input Port (string)").asString());

   robotSensorsInputPortName					= "/";
   robotSensorsInputPortName					+= getName(rf.check("robotSensorsInputPort", Value("/robotSensors:i"), "robotSensors Input Port (string)").asString());
   
   attentionSwitchOffInputPortName	        = "/";
   attentionSwitchOffInputPortName	        += getName(rf.check("attentionSwitchOffInputPort", Value("/attentionSwitchOff:i"), "attentionSwitchOff Input Port (string)").asString());
   
   reactionSwitchOffInputPortName	        = "/";
   reactionSwitchOffInputPortName	        += getName(rf.check("reactionSwitchOffInputPort", Value("/reactionSwitchOff:i"), "reactionSwitchOff Input Port (string)").asString());
   
   actionFeedbackInputPortName			    = "/";
   actionFeedbackInputPortName		        += getName(rf.check("actionFeedbackInputPort", Value("/actionFeedback:i"), "actionFeedback Input Port (string)").asString());
     
   elicitedAttentionOutputPortName	    = "/";
   elicitedAttentionOutputPortName       += getName(rf.check("elicitedAttentionOutputPort", Value("/elicitedAttention:o"), "elicitedAttention Output Port (string)").asString());
  
   eyeBlinkingOutputPortName				= "/";
   eyeBlinkingOutputPortName				+= getName(rf.check("eyeBlinkingOutputPort", Value("/eyeBlinking:o"), "eyeBlinking Output Port (string)").asString());
 
   fallingInterruptionOutputPortName		    = "/";
   fallingInterruptionOutputPortName			+= getName(rf.check("fallingInterruptionOutputPort", Value("/fallingInterruption:o"), "fallingInterruption Output Port (string)").asString());

   fallingReactionOutputPortName			= "/";
   fallingReactionOutputPortName			+= getName(rf.check("fallingReactionOutputPort", Value("/fallingReaction:o"), "fallingReaction Output Port (string)").asString());

   fallingReactionSpeechOutputPortName   = "/";
   fallingReactionSpeechOutputPortName   += getName(rf.check("fallingReactionSpeechOutputPort", Value("/fallingReactionSpeech:o"), "fallingReactionSpeech Output Port (string)").asString());
   
   socialReactionOutputPortName			= "/";
   socialReactionOutputPortName			+= getName(rf.check("socialReactionOutputPort", Value("/socialReaction:o"), "socialReaction Output Port (string)").asString());

   socialReactionSpeechOutputPortName    = "/";
   socialReactionSpeechOutputPortName    += getName(rf.check("socialReactionSpeechOutputPort", Value("/socialReactionSpeech:o"), "socialReactionSpeech Output Port (string)").asString());

   socialFacialExpressionOutputPortName  = "/";
   socialFacialExpressionOutputPortName  += getName(rf.check("socialFacialExpressionOutputPort", Value("/socialFacialExpression:o"), "socialFacialExpression Output Port (string)").asString());
   
   /* do all initialization here */
     
   /* open ports  */ 
       
 if (!getFacesIn.open(getFacesInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getFacesInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
 if (!getSoundDirectionIn.open(getSoundDirectionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSoundDirectionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
 if (!attentionBiasIn.open(attentionBiasInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << attentionBiasInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

 if (!therapistGazeCommandIn.open(therapistGazeCommandInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << therapistGazeCommandInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
 if (!checkMutualGazeIn.open(checkMutualGazeInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << checkMutualGazeInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

 if (!recognizeSpeechIn.open(recognizeSpeechInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << recognizeSpeechInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

 if (!affectiveStateIn.open(affectiveStateInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << affectiveStateInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
 if (!identifyFaceExpressionIn.open(identifyFaceExpressionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceExpressionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

 if (!robotSensorsIn.open(robotSensorsInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << robotSensorsInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
 if (!attentionSwitchOffIn.open(attentionSwitchOffInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << attentionSwitchOffInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!reactionSwitchOffIn.open(reactionSwitchOffInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << reactionSwitchOffInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!actionFeedbackIn.open(actionFeedbackInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << actionFeedbackInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!elicitedAttentionOut.open(elicitedAttentionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << elicitedAttentionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!eyeBlinkingOut.open(eyeBlinkingOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << eyeBlinkingOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!fallingInterruptionOut.open(fallingInterruptionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << fallingInterruptionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!fallingReactionOut.open(fallingReactionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << fallingReactionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!fallingReactionSpeechOut.open(fallingReactionSpeechOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << fallingReactionSpeechOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!socialReactionOut.open(socialReactionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << socialReactionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!socialReactionSpeechOut.open(socialReactionSpeechOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << socialReactionSpeechOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   if (!socialFacialExpressionOut.open(socialFacialExpressionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << socialFacialExpressionOutputPortName << endl;
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

   /* Create the YarpInterface used to communicate between the main code in Controller and the other components through the thread class */
   attentionReactionYarpInterface = new AttentionReactionYarpInterface();
   attentionSystem = new AttentionSystemThread(attentionReactionYarpInterface);
   eyeBlinking = new EyeBlinkingThread(attentionReactionYarpInterface);

   /* Create the main class: Controller */ 
   
   attentionReactionController = new AttentionReactionController(attentionReactionYarpInterface, attentionSystem, eyeBlinking);

   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/   

   attach(handlerPort);                  // attach to port
 
   /* create the thread and pass pointers to the module parameters */

    attentionReactionSubsystemThread = new AttentionReactionSubsystemThread(&getFacesIn, &getSoundDirectionIn, &attentionBiasIn,
		                                                    &therapistGazeCommandIn, &checkMutualGazeIn,&recognizeSpeechIn,
                                                            &affectiveStateIn, &identifyFaceExpressionIn, &robotSensorsIn, &attentionSwitchOffIn,
		                                                    &reactionSwitchOffIn, &actionFeedbackIn, &elicitedAttentionOut,&eyeBlinkingOut,
                                                            &fallingInterruptionOut, &fallingReactionOut, &fallingReactionSpeechOut,&socialReactionOut,
                                                            &socialReactionSpeechOut,&socialFacialExpressionOut);
   /* now start the thread to do the work */

   attentionReactionSubsystemThread->setYarpInterface(attentionReactionYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   attentionReactionYarpInterface->init(attentionReactionSubsystemThread, attentionReactionController);
   
   attentionReactionSubsystemThread->start(); // this calls threadInit() and it if returns true, it then calls run()
   attentionReactionController->start();
   attentionSystem->start();
   eyeBlinking->start();
   
   return true ;      // let the RFModule know everything went well
                      // so that it will then run the module
}


bool AttentionReactionSubsystem::interruptModule()
{
   /* NB you must stop the thread before interrupting the ports and then closing them
      If you don't then gyarpmanger will not be able to stop the module and you will have
      to resort to killing it                                                              */

   attentionReactionSubsystemThread->stop();
  
   getFacesIn.interrupt();
   getSoundDirectionIn.interrupt();
   attentionBiasIn.interrupt();
   therapistGazeCommandIn.interrupt();
   checkMutualGazeIn.interrupt();
   recognizeSpeechIn.interrupt();
   affectiveStateIn.interrupt();
   identifyFaceExpressionIn.interrupt();   
   robotSensorsIn.interrupt();
   attentionSwitchOffIn.interrupt();
   reactionSwitchOffIn.interrupt();
   actionFeedbackIn.interrupt();
   elicitedAttentionOut.interrupt();
   eyeBlinkingOut.interrupt();
   fallingInterruptionOut.interrupt();
   fallingReactionOut.interrupt();
   fallingReactionSpeechOut.interrupt();
   socialReactionOut.interrupt();
   socialReactionSpeechOut.interrupt();
   socialFacialExpressionOut.interrupt();
   
   handlerPort.interrupt();

   return true;
}


bool AttentionReactionSubsystem::close()
{   
   getFacesIn.close();
   getSoundDirectionIn.close();
   attentionBiasIn.close();
   therapistGazeCommandIn.close();
   checkMutualGazeIn.close();
   recognizeSpeechIn.close();
   affectiveStateIn.close();
   identifyFaceExpressionIn.close();   
   robotSensorsIn.close();
   attentionSwitchOffIn.close();
   reactionSwitchOffIn.close();
   actionFeedbackIn.close();
   elicitedAttentionOut.close();
   eyeBlinkingOut.close();
   fallingInterruptionOut.close();
   fallingReactionOut.close();
   fallingReactionSpeechOut.close();
   socialReactionOut.close();
   socialReactionSpeechOut.close();
   socialFacialExpressionOut.close();
   handlerPort.close();

   return true;
}


bool AttentionReactionSubsystem::respond(const Bottle& command, Bottle& reply) 
{
  
   return true;
}


/* Called periodically every getPeriod() seconds */

bool AttentionReactionSubsystem::updateModule()
{
   return true;
}



double AttentionReactionSubsystem::getPeriod()
{
   /* module periodicity (seconds), called implicitly by eyeBlinking */
    
   return 0.1;
}
