/** @file videoSource.h  Interface file for the videoSource component
 *
 * Version 2.0
 * 
 * 22/01/2016
 *
 * \defgroup DREAM_videoSource videoSource
 *
 * The videoSource DREAM component
 *
 * \section component_description Component Description
 * 
 * Reads images from video files and streams images to specified port.
 * 
 * \section lib_sec Libraries
 *
 * YARP.
 *
 * \section parameters_sec Parameters
 * 
 * <b>Command-line Parameters</b>
 * 
 * The following key-value pairs can be specified as command-line parameters by prefixing -- to the key 
 * (e.g. \c --from file.ini . The value part can be changed to suit your needs; the default values are shown below. 
 *
 * - \c from \c videoSource.ini     
 *     - The configuration file
 *
 * - \c context \c components/videoSource/config
 *     - specifies the sub-path from \c $DREAM_ROOT to the configuration file
 *
 * - \c name \c videoSource
 *     - The name of the module (used to form the stem of module port names)  
 *
 *
 * <b>Configuration File Parameters</b>
 *
 * The following key-value pairs can be specified as parameters in the configuration file 
 * (they can also be specified as command-line parameters if you so wish). 
 * The value part can be changed to suit your needs; the default values are shown below. 
 *   
 * Key | Value
 * :-- | :----
 * outputPort  |  /image:o
 *
 * The output port name to which the image should be streamed
 *
 * Key | Value
 * :-- | :----
 * heartbeatPort  |  /heartbeat:i
 *
 * The input port name for passive (heartbeat) playback
 *
 * Note: the port names above will be prefixed with component name (i.e. \c /videoSource)
 *  
 * Key | Value
 * :-- | :----
 * videoFile  |  video.avi
 * 
 * specifies the video filename
 * 
 * Key | Value
 * :-- | :----
 * width  |  320
 * 
 * specifies the width of the image to be streamed. 0 indicates default size. 
 * The video stream is rescaled if necessary.
 * 
 * Key | Value
 * :-- | :----
 * height  |  240
 * 
 * specifies the height of the image to be streamed. 0 indicates default size.   
 * The video stream is rescaled if necessary.
 * 
 * Key | Value
 * :-- | :----
 * heartbeatPlayback  |  false
 *
 * If set to true, videoSource streams video frames in response to an incoming heartbeat
 * signal (c.f heartbeatPort parameter). This allow the framerate to be controlled by the 
 * heartbeat and several instances of videoSource can play synchronized video steams. 
 *
 * NOTE: Since the heartbeat playback implements frame reading and sending in callbacks 
 * rather than a single rate thread, playback is distributed over a thread pool allowing
 * very high frame rate in response to a fast heartbeat. 
 *
 * If set to false, videoSource plays video with a fixed frame rate independently of 
 * heartbeat. 
 *
 * Key | Value
 * :-- | :----
 * frameRate  |  0
 *
 * Playback framerate (fps) in fixed (heartbeatPlayback = false) mode. If frameRate = 0, 
 * the frame rate from the source video file is used. 
 *
 * Key | Value
 * :-- | :----
 * repeatPlayback  |  false
 *
 * If true, playback is repeated once the video ends. 
 *        
 * \section portsa_sec Ports Accessed (and possibly later sections).
 * 
 * None. 
 *
 * \section portsc_sec Ports Created
 *
 * <b>Input ports</b>
 *
 *  - \c /videoSource/heartbeat:i
 * 
 * This port is used during passive playback (see heartbeatPlayback configuration parameter)
 *
 *  - \c /videoSource
 * 
 * This port is used to change the parameters of the module at run time or stop the module.
 * The following commands are available
 * 
 *     help
 *     quit
 *
 *    Note that the name of this port mirrors whatever is provided by the \c --name parameter value
 *    The port is attached to the terminal so that you can type in commands and receive replies.
 *    The port can be used by other modules but also interactively by a user through the yarp rpc directive, viz.: \c yarp \c rpc \c /videoSource
 *    This opens a connection from a terminal to the port and allows the user to then type in commands and receive replies.
 *
 * <b>Input ports</b>
 *
 *     /videoSource/heartbeat:i            BufferedPort<VectorOf<long>>
 *
 * <b>Output ports</b>
 *
 *     /videoSource          
 *     /videoSource/image:o                BufferedPort<ImageOf<PixelRgb>>
 *     /videoSource/frameNumber:o          BufferedPort<VectorOf<long>>
 *
 * <b>Port types</b>
 *
 * See spec under input/output ports sections. 
 *
 * \section in_files_sec Input Data Files
 *
 * video.avi, or whatever is specified as a argument for the \c --imageFile \c key-value. 
 *
 * \section out_data_sec Output Data Files
 *
 * None.
 *
 * \section conf_file_sec Configuration Files
 *
 * \c videoSource.ini  in \c $DREAM_ROOT/components/videoSource/config
 * 
 * \section tested_os_sec Tested OS
 *
 * - Windows
 *
 * \section example_sec Example Instantiation of the Component
 * 
 * <tt>videoSource --name videoSource --context components/videoSource/config --from videoSource.ini </tt>
 *
 * \author Larisa Markeeva and Erik Billing
 * 
 * Copyright (C) 2015 Larisa Markeeva
 * Copyright (C) 2016 Erik Billing
 * 
 */

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


 
/*
 * Audit Trail
 * -----------
 * 01/04/15  First version validated   DV for Larisa Markeeva
 * 22/01/16  Version 2.0 completed. Passive (heartbeat) playback implemented. 
 * 17/06/16  Updated docs for integration. 
 */ 

#ifndef VIDEOSOURCE
#define VIDEOSOURCE

#include <iostream>
#include <string>
 
#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/Time.h>
#include <yarp/os/Mutex.h>

#include <opencv/cv.h>      
#include <opencv/highgui.h>

using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;
using namespace yarp::sig::file;

using namespace cv;

class VideoSourceThread : public RateThread
{
private:

   /* class variables */

   int      x, y;
   PixelRgb rgbPixel;
   ImageOf<PixelRgb> inputImage;
   int input_width;
   int input_height;
   int input_depth;
   //int xOffset;
 //  int yOffset;
   bool debug;
  // bool windowFlag;
	long frameNumber;

   /* thread parameters: they are pointers so that they refer to the original variables in videoSource */
   BufferedPort<ImageOf<PixelRgb> > *imagePortOut;
	BufferedPort<VectorOf<long>>  *frameNumberOut;   
	Port *events;
   int *widthValue;     
   int *heightValue; 
	bool *heartbeatPlayback;
	bool *repeatPlayback;

   string videoFile;  
   VideoCapture video;

public:

   /* class methods */
	
   VideoSourceThread(Port *events, BufferedPort<ImageOf<PixelRgb> > *imageOut, BufferedPort<VectorOf<long>> *frameNumberOut, int *width, int *height, bool *heartbeatPlay, double *fps, bool *repeat);
	bool start(bool passivePlayback);
   bool threadInit();     
   void threadRelease();
   void run();
	void stepFrame();
	void stepFrame(long frameIndex);
	void setSourceFile(string sourceFile);
};

class HeartbeatListener : public BufferedPort<VectorOf<long>> {
	VideoSourceThread* _playbackThread;
	virtual void onRead(VectorOf<long>& data);
public: 
	void setThread(VideoSourceThread* thread);
};

class VideoSource:public RFModule
{

   /* class variables */

   bool debug; 

   /* module parameters */

   string moduleName;
	string heartbeatPortName;
   string outputPortName;  
	string frameNumberPortName;
   string handlerPortName;
   string videoFile;
   //int    frequency;
   int    width;
   int    height;
	bool heartbeatPlayback;
	double frameRate;
	bool repeatPlayback;
   /*int    window;
   int    xShift;
   int    yShift;
   double horizontalViewAngle;
   double verticalViewAngle;
   */

   /* class variables */
	HeartbeatListener heartbeatIn; // for passive playback
   BufferedPort<ImageOf<PixelRgb> > imageOut;     // output port
	BufferedPort<VectorOf<long>> frameOut;       // output port for frame number
   //BufferedPort<VectorOf<double> >  encoderOut;   // encoder port
   Port handlerPort;                              // a port to handle messages 

   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   VideoSourceThread *videoSourceThread;

public:
   
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
};

#endif
