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

#include "reactiveSystemGUI.h"

bool ReactiveSystemGUI::configure(yarp::os::ResourceFinder &rf)
{    
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("reactiveSystemGUI"), 
                           "module name (string)").asString();

   /*
    * before continuing, set the module name before getting any other parameters, 
    * specifically the port names which are dependent on the module name
    */
   
   setName(moduleName.c_str());

   /* now, get the rest of the parameters */

   /* get the name of the input and output ports, automatically prefixing the module name by using getName() */

   sensoryInterpretationOutputPortName             = "/";
   sensoryInterpretationOutputPortName            += getName(rf.check("sensoryInterpretationOutputPort", Value("/sensoryInterpretation:o"), "sensoryInterpretation Output Port (string)").asString());

   recognizeSpeechOutputPortName                   = "/";
   recognizeSpeechOutputPortName                  += getName(rf.check("recognizeSpeechOutputPort", Value("/recognizeSpeech:o"), "recognizeSpeech Output Port (string)").asString());

   checkMutualGazeOutputPortName                   = "/";
   checkMutualGazeOutputPortName                  += getName(rf.check("checkMutualGazeOutputPort", Value("/checkMutualGaze:o"), "checkMutualGaze Output Port (string)").asString());

   identifyFaceExpressionOutputPortName            = "/";
   identifyFaceExpressionOutputPortName           += getName(rf.check("identifyFaceExpressionOutputPort", Value("/identifyFaceExpression:o"), "identifyFaceExpression Output Port (string)").asString());

   getDeliberativeFacialExpressionOutputPortName   = "/";
   getDeliberativeFacialExpressionOutputPortName  += getName(rf.check("getDeliberativeFacialExpressionOutputPort", Value("/getDeliberativeFacialExpression:o"), "getDeliberativeFacialExpression Output Port (string)").asString());

   getSpeechOutputPortName                         = "/";
   getSpeechOutputPortName                        += getName(rf.check("getSpeechOutputPort", Value("/getSpeech:o"), "getSpeech Output Port (string)").asString());

   getGazeVariationOutputPortName                  = "/";
   getGazeVariationOutputPortName                 += getName(rf.check("getGazeVariationOutputPort", Value("/getGazeVariation:o"), "getGazeVariation Output Port (string)").asString());

   getBalanceOutputPortName                        = "/";
   getBalanceOutputPortName                       += getName(rf.check("getBalanceOutputPort", Value("/getBalance:o"), "getBalance Output Port (string)").asString());

   getHeadTactileOutputPortName                    = "/";
   getHeadTactileOutputPortName                   += getName(rf.check("getHeadTactileOutputPort", Value("/getHeadTactile:o"), "getHeadTactile Output Port (string)").asString());

   getHandTactileOutputPortName                    = "/";
   getHandTactileOutputPortName                   += getName(rf.check("getHandTactileOutputPort", Value("/getHandTactile:o"), "getHandTactile Output Port (string)").asString());

   getBumperTactileOutputPortName                  = "/";
   getBumperTactileOutputPortName                 += getName(rf.check("getBumperTactileOutputPort", Value("/getBumperTactile:o"), "getBumperTactile Output Port (string)").asString());

   getEyeBlinkingInputPortName                     = "/";
   getEyeBlinkingInputPortName                    += getName(rf.check("getEyeBlinkingInputPort", Value("/getEyeBlinking:i"), "getEyeBlinking Input Port (string)").asString());
   
   eyeBlinkingSwitchOffOutputPortName              = "/";
   eyeBlinkingSwitchOffOutputPortName             += getName(rf.check("eyeBlinkingSwitchOffOutputPort", Value("/eyeBlinkingSwitchOff:o"), "eyeBlinkingSwitchOff Output Port (string)").asString());
   
   getInterruptionInputPortName                    = "/";
   getInterruptionInputPortName                   += getName(rf.check("getInterruptionInputPort", Value("/getInterruption:i"), "getInterruption Input Port (string)").asString());
   
   fallingReactionSwitchOffOutputPortName          = "/";
   fallingReactionSwitchOffOutputPortName         += getName(rf.check("fallingReactionSwitchOffOutputPort", Value("/fallingReactionSwitchOff:o"), "fallingReactionSwitchOff Output Port (string)").asString());
   
   socialReactionSwitchOffOutputPortName           = "/";
   socialReactionSwitchOffOutputPortName          += getName(rf.check("socialReactionSwitchOffOutputPort", Value("/socialReactionSwitchOff:o"), "socialReactionSwitchOff Output Port (string)").asString());

   getSocialReactionSpeechInputPortName            = "/";
   getSocialReactionSpeechInputPortName           += getName(rf.check("getSocialReactionSpeechInputPort", Value("/getSocialReactionSpeech:i"), "getSocialReactionSpeech Input Port (string)").asString());
   
   getFallingReactionSpeechInputPortName           = "/";
   getFallingReactionSpeechInputPortName          += getName(rf.check("getFallingReactionSpeechInputPort", Value("/getFallingReactionSpeech:i"), "getFallingReactionSpeech Input Port (string)").asString());
   
   moveSequenceInputPortName                       = "/";
   moveSequenceInputPortName                      += getName(rf.check("moveSequenceInputPort", Value("/moveSequence:i"), "moveSequence Input Port (string)").asString());
   
   sayInputPortName                                = "/";
   sayInputPortName                               += getName(rf.check("sayInputPort", Value("/say:i"), "say Input Port (string)").asString());
  
   /* get the DREAM logo file  */

   logoFilename  = rf.check("dreamLogo", 
                            Value("DREAM_223x59.png"), 
                            "DREAM logo filename (string)").asString();

   logoFilename = (rf.findFile(logoFilename.c_str())).c_str();
 

   /* do all initialization here */
     
   /* open ports  */ 

   
   if (!sensoryInterpretationOut.open(sensoryInterpretationOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << sensoryInterpretationOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

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

   if (!getDeliberativeFacialExpressionOut.open(getDeliberativeFacialExpressionOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getDeliberativeFacialExpressionOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getSpeechOut.open(getSpeechOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSpeechOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
      
   if (!getGazeVariationOut.open(getGazeVariationOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getGazeVariationOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getBalanceOut.open(getBalanceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getBalanceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!getHeadTactileOut.open(getHeadTactileOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHeadTactileOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getHandTactileOut.open(getHandTactileOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getHandTactileOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
    
    if (!getBumperTactileOut.open(getBumperTactileOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getBumperTactileOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!getEyeBlinkingIn.open(getEyeBlinkingInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getEyeBlinkingInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!eyeBlinkingSwitchOffOut.open(eyeBlinkingSwitchOffOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << eyeBlinkingSwitchOffOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getInterruptionIn.open(getInterruptionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getInterruptionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!fallingReactionSwitchOffOut.open(fallingReactionSwitchOffOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << fallingReactionSwitchOffOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

	if (!socialReactionSwitchOffOut.open(socialReactionSwitchOffOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << socialReactionSwitchOffOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

    if (!getSocialReactionSpeechIn.open(getSocialReactionSpeechInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSocialReactionSpeechInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

    if (!getFallingReactionSpeechIn.open(getFallingReactionSpeechInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getFallingReactionSpeechInputPortName << endl;
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

   reactiveSystemGUIThread = new ReactiveSystemGUIThread(
                                                                 &sensoryInterpretationOut,                                                                 
																 &recognizeSpeechOut,   
																 &checkMutualGazeOut, 
																 &identifyFaceExpressionOut, 
																 &getDeliberativeFacialExpressionOut,
																 &getSpeechOut,
																 &getGazeVariationOut,
																 &getBalanceOut,
																 &getHeadTactileOut,
																 &getHandTactileOut,
																 &getBumperTactileOut,
																 &getEyeBlinkingIn,
																 &eyeBlinkingSwitchOffOut,
																 &getInterruptionIn,
																 &fallingReactionSwitchOffOut,
																 &socialReactionSwitchOffOut,
                                                                 &getSocialReactionSpeechIn,
                                                                 &getFallingReactionSpeechIn,
                                                                 &moveSequenceIn,
                                                                 &sayIn,
																 logoFilename);

   /* now start the thread to do the work */

   reactiveSystemGUIThread->start(); // this calls threadInit() and it if returns true, it then calls run()



   return true ;      // let the RFModule know everything went well
                      // so that it will then run the module
}


bool ReactiveSystemGUI::interruptModule()
{
   /* NB you must stop the thread before interrupting the ports and then closing them
      If you don't then gyarpmanger will not be able to stop the module and you will have
      to resort to killing it                                                              */

   reactiveSystemGUIThread->stop();  
  
   sensoryInterpretationOut.interrupt();
   recognizeSpeechOut.interrupt();
   checkMutualGazeOut.interrupt();
   identifyFaceExpressionOut.interrupt();   
   getDeliberativeFacialExpressionOut.interrupt();
   getSpeechOut.interrupt();
   getGazeVariationOut.interrupt();
   getBalanceOut.interrupt();
   getHeadTactileOut.interrupt();
   getHandTactileOut.interrupt();
   getBumperTactileOut.interrupt();
   getEyeBlinkingIn.interrupt();
   eyeBlinkingSwitchOffOut.interrupt();
   getInterruptionIn.interrupt();
   fallingReactionSwitchOffOut.interrupt();
   socialReactionSwitchOffOut.interrupt();
   getSocialReactionSpeechIn.interrupt();
   getFallingReactionSpeechIn.interrupt();
   moveSequenceIn.interrupt();
   sayIn.interrupt();
   handlerPort.interrupt();

   return true;
}


bool ReactiveSystemGUI::close()
{
   sensoryInterpretationOut.close();
   recognizeSpeechOut.close();
   checkMutualGazeOut.close();
   identifyFaceExpressionOut.close();
   getDeliberativeFacialExpressionOut.close();
   getSpeechOut.close();
   getGazeVariationOut.close();
   getBalanceOut.close();
   getHeadTactileOut.close();
   getHandTactileOut.close();
   getBumperTactileOut.close();
   getEyeBlinkingIn.close();
   eyeBlinkingSwitchOffOut.close();
   getInterruptionIn.close();
   fallingReactionSwitchOffOut.close();
   socialReactionSwitchOffOut.close();
   getSocialReactionSpeechIn.close();
   getFallingReactionSpeechIn.close();
   moveSequenceIn.close();
   sayIn.close();
  
   handlerPort.close();

   return true;
}

bool ReactiveSystemGUI::respond(const Bottle& command, Bottle& reply) 
{
   return true;
}


/* Called periodically every getPeriod() seconds */

bool ReactiveSystemGUI::updateModule()
{
   return true;
}



double ReactiveSystemGUI::getPeriod()
{
   /* module periodicity (seconds), called implicitly by reactiveSystemGUI */
    
   return 10;
}
