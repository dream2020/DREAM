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
 * 30/10/14  First version validated (David Vernon))
 *
 */ 

#include "protoComponentGUI.h"


/* 
 * Configure method ... use it to do component coordination, 
 * i.e. to configure your component at runtime
 */

bool ProtoComponentGUI::configure(yarp::os::ResourceFinder &rf)
{    
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("protoComponentGUI"), 
                           "module name (string)").asString();

   /*
    * before continuing, set the module name before getting any other parameters, 
    * specifically the port names which are dependent on the module name
    */
   
   setName(moduleName.c_str());

   /* now, get the rest of the parameters */

   /* get the name of the input and output ports, automatically prefixing the module name by using getName() */

   colourImageInputPortName    =      "/";
   colourImageInputPortName   +=      getName(
                                      rf.check("colourImageInputPort", 
                                      Value("/colourimage:i"),
                                      "Colour input image port (string)").asString()
                                      );

   binaryImageInputPortName   =      "/";
   binaryImageInputPortName  +=      getName(
                                      rf.check("binaryImageInputPort", 
                                      Value("/binaryimage:i"),
                                      "Binary input image port (string)").asString()
                                      );

   statisticsInputPortName   =       "/";
   statisticsInputPortName  +=       getName(
                                      rf.check("statisticsInputPort", 
                                      Value("/statistics:i"),
                                      "Statistics output port (string)").asString()
                                      );
   
   thresholdOutputPortName =          "/";
   thresholdOutputPortName +=         getName(
                                      rf.check("thresholdInputPort", 
                                      Value("/threshold:o"),
                                      "Threshold output port (string)").asString()
                                       );


   /* get the threshold value */

   thresholdValue             =       rf.check("threshold",
                                      Value(128),
                                      "Key value (int)").asInt();

   
   /* get the DREAM logo file  */

   logoFilename  = rf.check("dreamLogo", 
                            Value("DREAM_223x59.png"), 
                            "DREAM logo filename (string)").asString();

   logoFilename = (rf.findFile(logoFilename.c_str())).c_str();
 

   /* do all initialization here */
     
   /* open ports  */ 
       
   if (!colourImageIn.open(colourImageInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << colourImageInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!binaryImageIn.open(binaryImageInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << binaryImageInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   } 

   if (!statisticsIn.open(statisticsInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << statisticsInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!thresholdOut.open(thresholdOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << thresholdOutputPortName << endl;
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

   protoComponentGUIThread = new ProtoComponentGUIThread(&colourImageIn, &binaryImageIn, &statisticsIn, &thresholdOut, &thresholdValue, logoFilename);

   /* now start the thread to do the work */

   protoComponentGUIThread->start(); // this calls threadInit() and it if returns true, it then calls run()



   return true ;      // let the RFModule know everything went well
                      // so that it will then run the module
}


bool ProtoComponentGUI::interruptModule()
{
   /* NB you must stop the thread before interrupting the ports and then closing them
      If you don't then gyarpmanger will not be able to stop the module and you will have
      to resort to killing it                                                              */

   protoComponentGUIThread->stop();  
  
   colourImageIn.interrupt();
   binaryImageIn.interrupt();
   statisticsIn.interrupt();
   thresholdOut.interrupt(); 
   handlerPort.interrupt();

   return true;
}


bool ProtoComponentGUI::close()
{
   colourImageIn.close();
   binaryImageIn.close();
   statisticsIn.close();
   thresholdOut.close(); 
   handlerPort.close();

   return true;
}


bool ProtoComponentGUI::respond(const Bottle& command, Bottle& reply) 
{
  string helpMessage =  string(getName().c_str()) + 
                        " commands are: \n" +  
                        "help \n" + 
                        "quit \n" + 
                        "set thr <n> ... set the threshold \n" + 
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
   else if (command.get(0).asString()=="set") {
      if (command.get(1).asString()=="thr") {
         thresholdValue = command.get(2).asInt(); // set parameter value
         reply.addString("ok");
      }
   }
   return true;
}


/* Called periodically every getPeriod() seconds */

bool ProtoComponentGUI::updateModule()
{
   return true;
}



double ProtoComponentGUI::getPeriod()
{
   /* module periodicity (seconds), called implicitly by protoComponentGUI */
    
   return 0.1;
}
