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
 * 08/12/2014  First version validated (David Vernon))
 *
 */ 

#include "systemArchitectureGUI.h"


/* 
 * Configure method ... use it to do component coordination, 
 * i.e. to configure your component at runtime
 */

bool SystemArchitectureGUI::configure(yarp::os::ResourceFinder &rf)
{    
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("systemArchitectureGUI"), 
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

   sensoryInterpretationOutputPortName          = "/";
   sensoryInterpretationOutputPortName         += getName(rf.check("sensoryInterpretationOutputPort", Value("/sensoryInterpretation:o"), "sensoryInterpretation Output Port (string)").asString());

   getChildBehaviourInputPortName               = "/";
   getChildBehaviourInputPortName              += getName(rf.check("getChildBehaviourInputPort", Value("/getChildBehaviour:i"), "getChildBehaviour Input Port (string)").asString());

   getChildMotivationInputPortName              = "/";
   getChildMotivationInputPortName             += getName(rf.check("getChildMotivationInputPort", Value("/getChildMotivation:i"), "getChildMotivation Input Port (string)").asString());

   getChildPerformanceInputPortName             = "/";
   getChildPerformanceInputPortName            += getName(rf.check("getChildPerformanceInputPort", Value("/getChildPerformance:i"), "getChildPerformance Input Port (string)").asString());

   childBehaviourClassificationOutputPortName   = "/";
   childBehaviourClassificationOutputPortName  += getName(rf.check("childBehaviourClassificationOutputPort", Value("/childBehaviourClassification:o"), "childBehaviourClassification Output Port (string)").asString());

   gripOutputPortName                           = "/";
   gripOutputPortName                          += getName(rf.check("gripOutputPort", Value("/grip:o"), "grip Output Port (string)").asString());

   moveHandOutputPortName                       = "/";
   moveHandOutputPortName                      += getName(rf.check("moveHandOutputPort", Value("/moveHand:o"), "moveHand Output Port (string)").asString());

   moveHeadOutputPortName                       = "/";
   moveHeadOutputPortName                      += getName(rf.check("moveHeadOutputPort", Value("/moveHead:o"), "moveHead Output Port (string)").asString());

   moveSequenceOutputPortName                   = "/";
   moveSequenceOutputPortName                  += getName(rf.check("moveSequenceOutputPort", Value("/moveSequence:o"), "moveSequence Output Port (string)").asString());

   moveTorsoOutputPortName                      = "/";
   moveTorsoOutputPortName                     += getName(rf.check("moveTorsoOutputPort", Value("/moveTorso:o"), "moveTorso Output Port (string)").asString());

   sayOutputPortName                            = "/";
   sayOutputPortName                           += getName(rf.check("sayOutputPort", Value("/say:o"), "sayInput Output Port (string)").asString());

   enableRobotOutputPortName                    = "/";
   enableRobotOutputPortName                   += getName(rf.check("enableRobotOutputPort", Value("/enableRobot:o"), "enableRobot Output Port (string)").asString());

   getInterventionStateInputPortName            = "/";
   getInterventionStateInputPortName           += getName(rf.check("getInterventionStateInputPort", Value("/getInterventionState:i"), "getInterventionState Input Port (string)").asString());

   cognitiveControlOutputPortName               = "/";
   cognitiveControlOutputPortName              += getName(rf.check("cognitiveControlOutputPort", Value("/cognitiveControl:o"), "cognitiveControl Output Port (string)").asString());
  
   
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

   if (!childBehaviourClassificationOut.open(childBehaviourClassificationOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << childBehaviourClassificationOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }


   if (!gripOut.open(gripOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << gripOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!moveHandOut.open(moveHandOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << moveHandOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
      
   if (!moveHeadOut.open(moveHeadOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << moveHeadOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!moveSequenceOut.open(moveSequenceOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << moveSequenceOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!moveTorsoOut.open(moveTorsoOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << moveTorsoOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
   
   if (!sayOut.open(sayOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << sayOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!enableRobotOut.open(enableRobotOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << enableRobotOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!getInterventionStateIn.open(getInterventionStateInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << getInterventionStateInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }
 
   if (!cognitiveControlOut.open(cognitiveControlOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << cognitiveControlOutputPortName << endl;
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

   systemArchitectureGUIThread = new SystemArchitectureGUIThread(
                                                                 &verbose,             // simulation control parameters
                                                                 &cycle,
                                                                 &cyclePeriod,
                                                                 &eventRate,
                                                                 &sensoryInterpretationOut,
                                                                 &getChildBehaviourIn,
                                                                 &getChildMotivationIn,
                                                                 &getChildPerformanceIn,
                                                                 &childBehaviourClassificationOut,
                                                                 &getInterventionStateIn,
                                                                 &gripOut,
                                                                 &moveHandOut,
                                                                 &moveHeadOut,
                                                                 &moveSequenceOut,
                                                                 &moveTorsoOut, 
                                                                 &sayOut,
                                                                 &enableRobotOut,
                                                                 &cognitiveControlOut,
                                                                 logoFilename);

   /* now start the thread to do the work */

   systemArchitectureGUIThread->start(); // this calls threadInit() and it if returns true, it then calls run()



   return true ;      // let the RFModule know everything went well
                      // so that it will then run the module
}


bool SystemArchitectureGUI::interruptModule()
{
   /* NB you must stop the thread before interrupting the ports and then closing them
      If you don't then gyarpmanger will not be able to stop the module and you will have
      to resort to killing it                                                              */

   systemArchitectureGUIThread->stop();  
  
   sensoryInterpretationOut.interrupt();
   getChildBehaviourIn.interrupt();
   getChildMotivationIn.interrupt();
   getChildPerformanceIn.interrupt();
   childBehaviourClassificationOut.interrupt();
   gripOut.interrupt();
   moveHandOut.interrupt();
   moveHeadOut.interrupt();
   moveSequenceOut.interrupt();
   moveTorsoOut.interrupt(); 
   sayOut.interrupt();
   enableRobotOut.interrupt();
   getInterventionStateIn.interrupt();
   cognitiveControlOut.interrupt();
   handlerPort.interrupt();

   return true;
}


bool SystemArchitectureGUI::close()
{
   sensoryInterpretationOut.close();
   getChildBehaviourIn.close();
   getChildMotivationIn.close();
   getChildPerformanceIn.close();
   childBehaviourClassificationOut.close();
   gripOut.close();
   moveHandOut.interrupt();
   moveHeadOut.close();
   moveSequenceOut.close();
   moveTorsoOut.close(); 
   sayOut.close();
   enableRobotOut.close();
   getInterventionStateIn.close();
   cognitiveControlOut.close();
   handlerPort.close();

   return true;
}

bool SystemArchitectureGUI::respond(const Bottle& command, Bottle& reply) 
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

bool SystemArchitectureGUI::updateModule()
{
   return true;
}



double SystemArchitectureGUI::getPeriod()
{
   /* module periodicity (seconds), called implicitly by systemArchitectureGUI */
    
   return 10;
}
