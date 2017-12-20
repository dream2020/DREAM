/* 
 * Author:  Larisa Markeeva, Innopolis University and Erik Billing, University of Skovde
 *
 * Copyright (C) 2016 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Erik Billing, University of Skovde 
 * Email:   erik.billing@his.se 
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


 
#include "videoSource.h"


bool VideoSource::configure(yarp::os::ResourceFinder &rf)
{    

	debug = false;
   
	/* Process all parameters from both command-line and .ini file */

	/* get the module name which will form the stem of all module port names */

	moduleName            = rf.check("name", Value("videoSource"), "module name (string)").asString();

	/*
	* before continuing, set the module name before getting any other parameters, 
	* specifically the port names which are dependent on the module name
	*/
   
	setName(moduleName.c_str());

	/* now, get the rest of the parameters */

	/* 
	* get the imageFilename
	*/

	videoFile  = rf.check("videoFile", Value(""), "image filename (string)").asString();
	if (!videoFile.empty()) videoFile = rf.findFile(videoFile);

	/* get the complete name of the image output port */

	heartbeatPortName     = "/" + getName(rf.check("heartbeatPort", Value("/heartbeat:i"), "Heartbeat port name for passive playback (string)").asString());
	outputPortName        = "/" + getName(rf.check("outputPort", Value("/image:o"), "Output image port (string)").asString());
	frameNumberPortName   = "/" + getName(rf.check("frameNumberPort", Value("/frameNumber:o"), "Output frame number port (string)").asString());

	/* get the complete name of the gaze output port */

	width                 = rf.check("width", Value(0), "output width key value (int)").asInt();
	height                = rf.check("height", Value(0), "output height key value (int)").asInt();
	heartbeatPlayback     = rf.check("heartbeatPlayback", Value("false"), "if set to 'true', playback is pasive in response to incoming heartbeats").asBool();
	frameRate             = rf.check("frameRate", Value(0.0), "playback frame rate (frames/sec)").asDouble();
	repeatPlayback        = rf.check("repeatPlayback", Value("false"), "if set to 'true', playback is repeated when the video ends").asBool();

	if (debug) {
		cout << "VideoSource::configure: image file name   " << videoFile << endl;
		cout << "VideoSource::configure: output port name  " << outputPortName << endl;
		cout << "VideoSource::configure: width             " << width << endl;
		cout << "VideoSource::configure: height            " << height << endl;
		cout << "VideoSource::configure: heartbeatPlayback " << heartbeatPlayback << endl;
	}

	/* do all initialization here */
     
	/* open ports  */ 
   
	if (!heartbeatIn.open(heartbeatPortName.c_str())) {
		cout << "VideoSource::configure" << ": unable to open port " << outputPortName << endl;
		return false;  // unable to open; let RFModule know so that it won't run
	}

	if (!imageOut.open(outputPortName.c_str())) {
		cout << "VideoSource::configure" << ": unable to open port " << outputPortName << endl;
		return false;  // unable to open; let RFModule know so that it won't run
	}
	if (!frameOut.open(frameNumberPortName.c_str())) {
		cout << "VideoSource::configure" << ": unable to open port " << frameNumberPortName << endl;
		return false;  // unable to open; let RFModule know so that it won't run
	}
	imageOut.setTargetPeriod(0);    

	handlerPortName =  "/";
	handlerPortName += getName();         // use getName() rather than a literal 
 
	if (!handlerPort.open(handlerPortName.c_str())) {           
		cout << getName() << ": Unable to open port " << handlerPortName << endl;  
		return false;
	}

	attach(handlerPort);                  // attach to port
 
	/* create the thread and pass pointers to the module parameters */

	//cout << "imageSource:: heartbeat playback " << heartbeatPlayback  << endl;
	   
	videoSourceThread = new VideoSourceThread(&handlerPort, &imageOut, &frameOut, &width, &height, &heartbeatPlayback, &frameRate, &repeatPlayback);
	if (!videoFile.empty()) videoSourceThread->setSourceFile(videoFile);

	heartbeatIn.setThread(videoSourceThread);
	if (heartbeatPlayback) heartbeatIn.useCallback();

	//cout << "imageSource::configure: returning from Thread constructor"   << endl;

	/* now start the thread to do the work */
	videoSourceThread->start(heartbeatPlayback);
	
	return true;       // let the RFModule know everything went well
						// so that it will then run the module
}
 

bool VideoSource::interruptModule()
{

   /* stop the thread */
   
   videoSourceThread->stop();
	heartbeatIn.interrupt();
   imageOut.interrupt();
	frameOut.interrupt();
   handlerPort.interrupt();

   return true;
}


bool VideoSource::close()
{
	heartbeatIn.close();
   imageOut.close();
	frameOut.close();
   handlerPort.close();

   return true;
}


bool VideoSource::respond(const Bottle& command, Bottle& reply) 
{
	string helpMessage = getName() + 
                        " commands are: \n" +  
                        "help \n" + 
                        "quit \n";
	reply.clear();
	const string cmd = command.size() == 0 ? "" : command.get(0).asString();

	if (cmd=="quit") {
       reply.addString("quitting");
       return false;     
   }
   else if (cmd=="help") {
      reply.addString(helpMessage);
   }
   else if (cmd=="play") {
		if (command.size() > 1) {
			videoSourceThread->setSourceFile(command.get(1).asString());
		}
		videoSourceThread->start(heartbeatPlayback);
		reply.addString("playing");
   }
	else if (cmd=="stop") {
		videoSourceThread->stop();
		reply.addString("stopped");
	}
   return true;
}


/* Called periodically every getPeriod() seconds */

bool VideoSource::updateModule()
{
   return true;
}



double VideoSource::getPeriod()
{
   /* module periodicity (seconds), called implicitly by videoSource */
   return 0.0;
}

 