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
 * 20/08/14  First version validated (David Vernon)
 * 06/10/14  Additional ports for threshold input and statistic output (David Vernon)
 *
 */ 

#include "protoComponent.h"


/* 
 * Configure method ... use it to do component coordination, 
 * i.e. to configure your component at runtime
 */

bool ProtoComponent::configure(yarp::os::ResourceFinder &rf)
{    
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("protoComponent"), 
                           "module name (string)").asString();

   /*
    * before continuing, set the module name before getting any other parameters, 
    * specifically the port names which are dependent on the module name
    */
   
   setName(moduleName.c_str());

   /* now, get the rest of the parameters */

   /* get the name of the input and output ports, automatically prefixing the module name by using getName() */

   imageInputPortName    =      "/";
   imageInputPortName   +=      getName(
                                rf.check("imageInputPort", 
                                Value("/image:i"),
                                "Input image port (string)").asString()
                                );
   
   thresholdInputPortName =     "/";
   thresholdInputPortName +=    getName(
                                rf.check("thresholdInputPort", 
                                Value("/threshold:i"),
                                "Threshold input port (string)").asString()
                                );

   imageOutputPortName   =      "/";
   imageOutputPortName  +=      getName(
                                rf.check("imageOutputPort", 
                                Value("/image:o"),
                                "Output image port (string)").asString()
                                );

   statisticsOutputPortName   = "/";
   statisticsOutputPortName  += getName(
                                rf.check("statisticsOutputPort", 
                                Value("/statistics:o"),
                                "Output image port (string)").asString()
                                );


   /* get the threshold value */

   thresholdValue        = rf.check("threshold",
                           Value(8),
                           "Key value (int)").asInt();

   
   /* 
    * get the cameraConfig file and read the required parameter values cx, cy 
    * in both the groups [CAMERA_CALIBRATION_LEFT] and [CAMERA_CALIBRATION_RIGHT]
    */

   cameraConfigFilename  = rf.check("cameraConfig", 
                           Value("cameras.ini"), 
                           "camera configuration filename (string)").asString();

   cameraConfigFilename = (rf.findFile(cameraConfigFilename.c_str())).c_str();

   Property cameraProperties;

   if (cameraProperties.fromConfigFile(cameraConfigFilename.c_str()) == false) {
      cout << "protoComponent: unable to read camera configuration file" << cameraConfigFilename << endl;
      return 0;
   }
   else {
      cxLeft  = (float) cameraProperties.findGroup("CAMERA_CALIBRATION_LEFT").check("cx", Value(160.0), "cx left").asDouble();
      cyLeft  = (float) cameraProperties.findGroup("CAMERA_CALIBRATION_LEFT").check("cy", Value(120.0), "cy left").asDouble();
      cxRight = (float) cameraProperties.findGroup("CAMERA_CALIBRATION_RIGHT").check("cx", Value(160.0), "cx right").asDouble();
      cyRight = (float) cameraProperties.findGroup("CAMERA_CALIBRATION_RIGHT").check("cy", Value(120.0), "cy right").asDouble();
   }


   /* do all initialization here */
     
   /* open ports  */ 
       
   if (!imageIn.open(imageInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << imageInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!thresholdIn.open(thresholdInputPortName.c_str())) {
      cout << getName() << ": unable to open port " << thresholdInputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }


   if (!imageOut.open(imageOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << imageOutputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

   if (!statisticsOut.open(statisticsOutputPortName.c_str())) {
      cout << getName() << ": unable to open port " << statisticsOutputPortName << endl;
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

   protoComponentThread = new ProtoComponentThread(&imageIn, &thresholdIn, &imageOut, &statisticsOut, &thresholdValue);

   /* now start the thread to do the work */

   protoComponentThread->start(); // this calls threadInit() and it if returns true, it then calls run()

   return true ;      // let the RFModule know everything went well
                      // so that it will then run the module
}


bool ProtoComponent::interruptModule()
{
   /* NB you must stop the thread before interrupting the ports and then closing them
      If you don't then gyarpmanger will not be able to stop the module and you will have
      to resort to killing it                                                              */

   protoComponentThread->stop();

   imageIn.interrupt();
   thresholdIn.interrupt();
   imageOut.interrupt();
   handlerPort.interrupt();

   return true;
}


bool ProtoComponent::close()
{
   imageIn.close();
   thresholdIn.close();
   imageOut.close();
   handlerPort.close();

   return true;
}


bool ProtoComponent::respond(const Bottle& command, Bottle& reply) 
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

bool ProtoComponent::updateModule()
{
   return true;
}



double ProtoComponent::getPeriod()
{
   /* module periodicity (seconds), called implicitly by protoComponent */
    
   return 0.1;
}
