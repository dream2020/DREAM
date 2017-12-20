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
 * 13/08/15  First version validated (Pablo Gomez)
 */  

#include "attentionReactionSystemGUI.h"

bool AttentionReactionSystemGUI::configure(yarp::os::ResourceFinder &rf)
{    
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("attentionReactionSystemGUI"), 
                           "module name (string)").asString();

   /*
    * before continuing, set the module name before getting any other parameters, 
    * specifically the port names which are dependent on the module name
    */
   
   setName(moduleName.c_str());

   /* now, get the rest of the parameters */

   /* get the name of the input and output ports, automatically prefixing the module name by using getName() */

   recognizeSpeechOutputPortName                   = "/";
   recognizeSpeechOutputPortName                  += getName(rf.check("recognizeSpeechOutputPort", Value("/recognizeSpeech:o"), "recognizeSpeech Output Port (string)").asString());

   checkMutualGazeOutputPortName                   = "/";
   checkMutualGazeOutputPortName                  += getName(rf.check("checkMutualGazeOutputPort", Value("/checkMutualGaze:o"), "checkMutualGaze Output Port (string)").asString());

   identifyFaceExpressionOutputPortName            = "/";
   identifyFaceExpressionOutputPortName           += getName(rf.check("identifyFaceExpressionOutputPort", Value("/identifyFaceExpression:o"), "identifyFaceExpression Output Port (string)").asString());

   affectiveStateOutputPortName                    = "/";
   affectiveStateOutputPortName                   += getName(rf.check("affectiveStateOutputPort", Value("/affectiveState:o"), "affectiveState Output Port (string)").asString());

   robotSensorsOutputPortName                    = "/";
   robotSensorsOutputPortName                   += getName(rf.check("robotSensorsOutputPort", Value("/robotSensors:o"), "robotSensors Output Port (string)").asString());

   eyeBlinkingInputPortName                     = "/";
   eyeBlinkingInputPortName                    += getName(rf.check("eyeBlinkingInputPort", Value("/eyeBlinking:i"), "eyeBlinking Input Port (string)").asString());
   
   reactiveSwitchOffOutputPortName                 = "/";
   reactiveSwitchOffOutputPortName                += getName(rf.check("reactiveSwitchOffOutputPort", Value("/reactionSwitchOff:o"), "reactiveSwitchOff Output Port (string)").asString());
   
   fallingInterruptionInputPortName                    = "/";
   fallingInterruptionInputPortName                   += getName(rf.check("fallingInterruptionInputPort", Value("/fallingInterruption:i"), "fallingInterruption Input Port (string)").asString());
   
   fallingReactionSpeechInputPortName           = "/";
   fallingReactionSpeechInputPortName          += getName(rf.check("fallingReactionSpeechInputPort", Value("/fallingReactionSpeech:i"), "fallingReactionSpeech Input Port (string)").asString());
   
   moveSequenceInputPortName                       = "/";
   moveSequenceInputPortName                      += getName(rf.check("moveSequenceInputPort", Value("/moveSequence:i"), "moveSequence Input Port (string)").asString());
   
   sayInputPortName                                = "/";
   sayInputPortName                               += getName(rf.check("sayInputPort", Value("/say:i"), "say Input Port (string)").asString());
  
   getFacesOutputPortName                          = "/"; 
   getFacesOutputPortName                         += getName(rf.check("getFacesOutputPort", Value("/getFaces:o"), "getFaces Output Port (string)").asString());

   getSoundDirectionOutputPortName                 = "/"; 
   getSoundDirectionOutputPortName                += getName(rf.check("getSoundDirectionOutputPort", Value("/getSoundDirection:o"), "getSoundDirection Output Port (string)").asString());

   attentionBiasOutputPortName                     = "/"; 
   attentionBiasOutputPortName                    += getName(rf.check("attentionBiasOutputPort", Value("/attentionBias:o"), "attentionBias Output Port (string)").asString());

   therapistGazeCommandOutputPortName              = "/"; 
   therapistGazeCommandOutputPortName             += getName(rf.check("therapistGazeCommandOutputPort", Value("/therapistGazeCommand:o"), "therapistGazeCommand Output Port (string)").asString());

   attentionSwitchOffOutputPortName                = "/"; 
   attentionSwitchOffOutputPortName               += getName(rf.check("attentionSwitchOffOutputPort", Value("/attentionSwitchOff:o"), "attentionSwitchOff Output Port (string)").asString());


   /* get the DREAM logo file  */

   logoFilename  = rf.check("dreamLogo", 
                            Value("DREAM_223x59.png"), 
                            "DREAM logo filename (string)").asString();

   logoFilename = (rf.findFile(logoFilename.c_str())).c_str();
 

   /* do all initialization here */
     
   /* open ports  */ 

   if (!recognizeSpeechOut.open(recognizeSpeechOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << recognizeSpeechOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!checkMutualGazeOut.open(checkMutualGazeOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << checkMutualGazeOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
     
   if (!identifyFaceExpressionOut.open(identifyFaceExpressionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << identifyFaceExpressionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!affectiveStateOut.open(affectiveStateOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << affectiveStateOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
    
    if (!robotSensorsOut.open(robotSensorsOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << robotSensorsOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!eyeBlinkingIn.open(eyeBlinkingInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << eyeBlinkingInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!reactiveSwitchOffOut.open(reactiveSwitchOffOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << reactiveSwitchOffOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!fallingInterruptionIn.open(fallingInterruptionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << fallingInterruptionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   if (!fallingReactionSpeechIn.open(fallingReactionSpeechInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << fallingReactionSpeechInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

	if (!moveSequenceIn.open(moveSequenceInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << moveSequenceInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!sayIn.open(sayInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << sayInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }


   if (!getFacesOut.open(getFacesOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getFacesOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!getSoundDirectionOut.open(getSoundDirectionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSoundDirectionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!attentionBiasOut.open(attentionBiasOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << attentionBiasOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!therapistGazeCommandOut.open(therapistGazeCommandOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << therapistGazeCommandOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
      
   if (!attentionSwitchOffOut.open(attentionSwitchOffOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << attentionSwitchOffOutputPortName << endl;
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

   attentionReactionSystemGUIThread = new AttentionReactionSystemGUIThread(                                                                 
																 &recognizeSpeechOut,   
																 &checkMutualGazeOut, 
																 &affectiveStateOut,
																 &eyeBlinkingIn,
																 &fallingInterruptionIn,
																 &fallingReactionSpeechIn,
                                                                 &moveSequenceIn,
                                                                 &sayIn,
                                                                 &getFacesOut,
                                                                 &getSoundDirectionOut,
                                                                 &identifyFaceExpressionOut, 
																 &robotSensorsOut,
                                                                 &attentionBiasOut,
                                                                 &therapistGazeCommandOut,
                                                                 &attentionSwitchOffOut,
                                                                 &reactiveSwitchOffOut,
																 logoFilename);

   /* now start the thread to do the work */

   attentionReactionSystemGUIThread->start(); // this calls threadInit() and it if returns true, it then calls run()



   return true ;      // let the RFModule know everything went well
                      // so that it will then run the module
}


bool AttentionReactionSystemGUI::interruptModule()
{
   /* NB you must stop the thread before interrupting the ports and then closing them
      If you don't then gyarpmanger will not be able to stop the module and you will have
      to resort to killing it                                                              */

   attentionReactionSystemGUIThread->stop();  
  
   recognizeSpeechOut.interrupt();
   checkMutualGazeOut.interrupt();
   identifyFaceExpressionOut.interrupt();   
   affectiveStateOut.interrupt();
   robotSensorsOut.interrupt();
   eyeBlinkingIn.interrupt();
   reactiveSwitchOffOut.interrupt();
   fallingInterruptionIn.interrupt();
   fallingReactionSpeechIn.interrupt();
   moveSequenceIn.interrupt();
   sayIn.interrupt();
   getFacesOut.interrupt();
   getSoundDirectionOut.interrupt();
   attentionBiasOut.interrupt();
   therapistGazeCommandOut.interrupt();
   attentionSwitchOffOut.interrupt();

   handlerPort.interrupt();

   return true;
}


bool AttentionReactionSystemGUI::close()
{
   recognizeSpeechOut.close();
   checkMutualGazeOut.close();
   identifyFaceExpressionOut.close();
   affectiveStateOut.close();
   robotSensorsOut.close();
   eyeBlinkingIn.close();
   reactiveSwitchOffOut.close();
   fallingInterruptionIn.close();
   fallingReactionSpeechIn.close();
   moveSequenceIn.close();
   sayIn.close();
   getFacesOut.close();
   getSoundDirectionOut.close();
   attentionBiasOut.close();
   therapistGazeCommandOut.close();
   attentionSwitchOffOut.close();
   handlerPort.close();

   return true;
}

bool AttentionReactionSystemGUI::respond(const Bottle& command, Bottle& reply) 
{
   return true;
}


/* Called periodically every getPeriod() seconds */

bool AttentionReactionSystemGUI::updateModule()
{
   return true;
}



double AttentionReactionSystemGUI::getPeriod()
{
   /* module periodicity (seconds), called implicitly by reactiveSystemGUI */
    
   return 10;
}
