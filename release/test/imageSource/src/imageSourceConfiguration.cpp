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
 * 21/09/09  First version validated   DV
 * 04/01/10  Added encoder functionality DV
 * 05/04/11  Added parameters to specify window shift in x and y directions DV
 * 06/10/14  Moved imageSourceThread->stop() from ImageSource::close() to ImageSource::interruptModule()
 *           to solve a problem with the module hanging when requested to stop DV
 */ 

 
#include "imageSource.h"


bool ImageSource::configure(yarp::os::ResourceFinder &rf)
{    

   debug = false;
   
   /* Process all parameters from both command-line and .ini file */

   /* get the module name which will form the stem of all module port names */

   moduleName            = rf.check("name", 
                           Value("imageSource"), 
                           "module name (string)").asString();

   /*
    * before continuing, set the module name before getting any other parameters, 
    * specifically the port names which are dependent on the module name
    */
   
   setName(moduleName.c_str());

   /* now, get the rest of the parameters */

   /* 
    * get the imageFilename
    */

   imageFilename  = rf.check("imageFile", 
                             Value("image.bmp"), 
                             "image filename (string)").asString();

   imageFilename = (rf.findFile(imageFilename.c_str())).c_str();

   /* get the complete name of the image output port */

   outputPortName        = rf.check("outputPort", 
                           Value("/image:o"),
                           "Output image port (string)").asString();

   /* get the complete name of the gaze output port */

   gazePortName          = rf.check("gazePort", 
                           Value("/gaze:o"),
                           "Output gaze port (string)").asString();

   /* get the complete name of the encoder state output port */

   encoderPortName       = rf.check("encoderPort", 
                           Value("/robot/head/state:o"),
                           "Output encoder port (string)").asString();

   /* get the frequency, width, height, standard deviation, horizontalViewAngle, and verticalViewAngle values */

   frequency             = rf.check("frequency",
                           Value(10),
                           "frequency key value (int)").asInt();

   width                 = rf.check("width",
                           Value(320),
                           "output width key value (int)").asInt();

   height                = rf.check("height",
                           Value(240),
                           "output height key value (int)").asInt();

   noiseLevel            = rf.check("noise",
                           Value(20),
                           "noise level key value (int)").asInt();
 
   window                = rf.check("window",
                           Value(0),
                           "window flag key value (int)").asInt();
 
   random                = rf.check("random",
                           Value(0),
                           "random flag key value (int)").asInt();
 
   xShift                = rf.check("xShift",
                           Value(5),
                           "horizontal shift key value (int)").asInt();

   yShift                = rf.check("yShift",
                           Value(5),
                           "vertical shift key value (int)").asInt();

   horizontalViewAngle   = rf.check("horizontalViewAngle",
                           Value(120.0),
                           "horizontal field of view angle key value (double)").asDouble();

   verticalViewAngle     = rf.check("verticalViewAngle",
                           Value(90.0),
                           "vertical field of view angle key value (double)").asDouble();


   if (debug) {
      cout << "imageSource::configure: image file name   " << imageFilename << endl;
      cout << "imageSource::configure: output port name  " << outputPortName << endl;
      cout << "imageSource::configure: gaze port name    " << gazePortName << endl;
      cout << "imageSource::configure: encoder port name " << encoderPortName << endl;
      cout << "imageSource::configure: frequency         " << frequency << endl;
      cout << "imageSource::configure: width             " << width << endl;
      cout << "imageSource::configure: height            " << height << endl;
      cout << "imageSource::configure: noise level       " << noiseLevel << endl;
      cout << "imageSource::configure: window flag       " << window << endl;
      cout << "imageSource::configure: random flag       " << random << endl;
      cout << "imageSource::configure: x shift           " << xShift << endl;
      cout << "imageSource::configure: y shift           " << yShift << endl;
      cout << "imageSource::configure: horizontal FoV    " << horizontalViewAngle << endl;
      cout << "imageSource::configure: vertical FoV      " << verticalViewAngle << endl;
   }

   /* do all initialization here */
     
   /* open ports  */ 
       
   if (!imageOut.open(outputPortName.c_str())) {
      cout << "imageSource::configure" << ": unable to open port " << outputPortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

          
   if (!gazeOut.open(gazePortName.c_str())) {
      cout << "imageSource::configure" << ": unable to open port " << gazePortName << endl;
      return false;  // unable to open; let RFModule know so that it won't run
   }

             
   if (!encoderOut.open(encoderPortName.c_str())) {
      cout << "imageSource::configure" << ": unable to open port " << encoderPortName << endl;
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

   //cout << "imageSource::configure: calling Thread constructor"   << endl;

   imageSourceThread = new ImageSourceThread(&imageOut, &gazeOut, &encoderOut, &imageFilename, 
                                             (int)(1000 / frequency), &width, &height, &noiseLevel, &window, &random,
                                             &xShift, &yShift,
                                             &horizontalViewAngle, &verticalViewAngle);

   //cout << "imageSource::configure: returning from Thread constructor"   << endl;

   /* now start the thread to do the work */

   imageSourceThread->start(); // this calls threadInit() and it if returns true, it then calls run()

   return true;       // let the RFModule know everything went well
                      // so that it will then run the module
}


bool ImageSource::interruptModule()
{

   /* stop the thread */
   
   imageSourceThread->stop();

   imageOut.interrupt();
   gazeOut.interrupt();
   encoderOut.interrupt();
   handlerPort.interrupt();

   return true;
}


bool ImageSource::close()
{

   imageOut.close();
   gazeOut.close();
   encoderOut.close();
   handlerPort.close();

   return true;
}


bool ImageSource::respond(const Bottle& command, Bottle& reply) 
{
  string helpMessage =  string(getName().c_str()) + 
                        " commands are: \n" +  
                        "help \n" + 
                        "quit \n" + 
                        "set noise <n> ... set the noise level \n" + 
                        "(where <n> is an integer number in the range 0-255) \n";

  reply.clear(); 

  if (command.get(0).asString()=="quit") {
       reply.addString("quitting");
       return false;     
   }
   else if (command.get(0).asString()=="help") {
      cout << helpMessage;
      reply.addString("ok");
   }
   else if (command.get(0).asString()=="set") {
      if (command.get(1).asString()=="noise") {
         noiseLevel = command.get(2).asInt(); // set parameter value
         reply.addString("ok");
      }
   }
   return true;
}


/* Called periodically every getPeriod() seconds */

bool ImageSource::updateModule()
{
   return true;
}



double ImageSource::getPeriod()
{
   /* module periodicity (seconds), called implicitly by imageSource */
    
   return 0.1;
}

 