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
 * 13/08/15  First version validated (Pablo Gómez)
 */ 

#include "actuationSimulator.h"


/* 
 * Configure method ... use it to do component coordination, 
 * i.e. to configure your component at runtime
 */

bool ActuationSimulator::configure(yarp::os::ResourceFinder &rf)
{    
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("actuationSimulator"), 
                           "module name (string)").asString();

   /*
    * before continuing, set the module name before getting any other parameters, 
    * specifically the port names which are dependent on the module name
    */
   
   setName(moduleName.c_str());

   /* get the name of the input and output ports, automatically prefixing the module name by using getName() */

   getEyeBlinkingInputPortName				= "/";
   getEyeBlinkingInputPortName			   += getName(rf.check("getEyeBlinkingInputPort", Value("/getEyeBlinking:i"), "getEyeBlinking Input Port (string)").asString());
   
   getFallingReactionInputPortName  		= "/";
   getFallingReactionInputPortName		   += getName(rf.check("getFallingReactionInputPort", Value("/getFallingReaction:i"), "getFallingReaction Input Port (string)").asString());

   getSocialFacialExpressionInputPortName   = "/";
   getSocialFacialExpressionInputPortName  += getName(rf.check("getSocialFacialExpressionInputPort", Value("/getSocialFacialExpression:i"), "getSocialFacialExpression Input Port (string)").asString());

   getFallingReactionSpeechInputPortName    = "/";
   getFallingReactionSpeechInputPortName   += getName(rf.check("getFallingReactionSpeechInputPort", Value("/getFallingReactionSpeech:i"), "getFallingReactionSpeech Input Port (string)").asString());

   getSocialReactionInputPortName			= "/";
   getSocialReactionInputPortName		   += getName(rf.check("getSocialReactionInputPort", Value("/getSocialReaction:i"), "getSocialReaction Input Port (string)").asString());

   getSocialReactionSpeechInputPortName		= "/";
   getSocialReactionSpeechInputPortName	   += getName(rf.check("getSocialReactionSpeechInputPort", Value("/getSocialReactionSpeech:i"), "getSocialReactionSpeech Input Port (string)").asString());
   
   getInterruptionInputPortName		        = "/";
   getInterruptionInputPortName	           += getName(rf.check("getInterruptionInputPort", Value("/getInterruption:i"), "getInterruption Input Port (string)").asString());
   
   sayOutputPortName						= "/";
   sayOutputPortName		 			   += getName(rf.check("sayOutputPort", Value("/say:o"), "say Output Port (string)").asString());

   moveSequenceOutputPortName				= "/";
   moveSequenceOutputPortName			   += getName(rf.check("moveSequenceOutputPort", Value("/moveSequence:o"), "moveSequence Output Port (string)").asString());

   motorFeedbackInputPortName			    = "/";
   motorFeedbackInputPortName			   += getName(rf.check("motorFeedbackInputPort", Value("/motorFeedback:i"), "motorFeedback Input Port (string)").asString());

   
   /* do all initialization here */
     
   /* open ports  */ 
       
 if (!getEyeBlinkingIn.open(getEyeBlinkingInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getEyeBlinkingInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 if (!getFallingReactionIn.open(getFallingReactionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getFallingReactionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
 if (!getFallingReactionSpeechIn.open(getFallingReactionSpeechInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getFallingReactionSpeechInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

 if (!getSocialReactionIn.open(getSocialReactionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSocialReactionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

 if (!getSocialReactionSpeechIn.open(getSocialReactionSpeechInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSocialReactionSpeechInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

 if (!getSocialFacialExpressionIn.open(getSocialFacialExpressionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getSocialFacialExpressionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
 if (!getInterruptionIn.open(getInterruptionInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getInterruptionInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
 if (!sayOut.open(sayOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << sayOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

 if (!moveSequenceOut.open(moveSequenceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << moveSequenceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

 if (!motorFeedbackIn.open(motorFeedbackInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << motorFeedbackInputPortName << endl;
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

    actuationSimulatorThread = new ActuationSimulatorThread(&getEyeBlinkingIn, &getFallingReactionIn, &getFallingReactionSpeechIn, &getSocialReactionIn,
		                                                    &getSocialReactionSpeechIn, &getSocialFacialExpressionIn,&getInterruptionIn,&sayOut, 
                                                            &moveSequenceOut,&motorFeedbackIn);

   /* now start the thread to do the work */

   actuationSimulatorThread->start(); // this calls threadInit() and it if returns true, it then calls run()

   return true ;      // let the RFModule know everything went well
                      // so that it will then run the module
}


bool ActuationSimulator::interruptModule()
{
   /* NB you must stop the thread before interrupting the ports and then closing them
      If you don't then gyarpmanger will not be able to stop the module and you will have
      to resort to killing it                                                              */

   actuationSimulatorThread->stop();

   getEyeBlinkingIn.interrupt();
   getFallingReactionIn.interrupt();
   getFallingReactionSpeechIn.interrupt();
   getSocialReactionIn.interrupt();
   getSocialReactionSpeechIn.interrupt();
   getSocialFacialExpressionIn.interrupt();
   getInterruptionIn.interrupt();
   sayOut.interrupt();
   motorFeedbackIn.interrupt();
   moveSequenceOut.interrupt();   
   handlerPort.interrupt();

   return true;
}


bool ActuationSimulator::close()
{
   getEyeBlinkingIn.close();
   getFallingReactionIn.close();
   getFallingReactionSpeechIn.close();
   getSocialReactionIn.close();
   getSocialReactionSpeechIn.close();
   getSocialFacialExpressionIn.close();
   getInterruptionIn.close();
   sayOut.close();
   moveSequenceOut.close();
   motorFeedbackIn.close();
   handlerPort.close();

   return true;
}


bool ActuationSimulator::respond(const Bottle& command, Bottle& reply) 
{
  string helpMessage =  string(getName().c_str()) + 
                        " commands are: \n" +  
                        "help \n" + 
                        "quit \n" + 
                        "(where <n> is an integer number) \n";

  reply.clear(); 

  if (command.get(0).asString()=="quit") {
       reply.addString("quitting");
       return false;     
   }
   else if (command.get(0).asString()=="help") {
      cout << helpMessage;
	  reply.addString("command is: set thr <n>");
   }
   
   return true;
}


/* Called periodically every getPeriod() seconds */

bool ActuationSimulator::updateModule()
{
   return true;
}



double ActuationSimulator::getPeriod()
{
   /* module periodicity (seconds), called implicitly by eyeBlinking */
    
   return 0.1;
}
