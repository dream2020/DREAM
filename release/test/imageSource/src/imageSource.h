/** @file imageSource.h  Interface file for the imageSourse DREAM component
 *
 * Version 1.6
 * 
 * 18/11/2014
 *
 * \defgroup DREAM_imageSource imageSource
 * 
 * Interface file for the imageSourse DREAM component
 *
 * \section component_description Component Description
 * 
 * Read an image from a file and stream it with specified dimensions to a specified port.
 * Add random noise to differentiate between each image streamed.
 *
 * If the window key-value pair is set to 1, and provided the dimensions of the image read from file is 
 * larger than the dimensions of the image to be streamed (as specified in the width and height key-value pairs), 
 * then we extract a sub-image of the required dimensions.  
 * The scan pattern is random if the random key-value pair is set to 1; otherwise it is a regular scan pattern in
 * row major order, with x and y increment equal to the values specified by the xShift and yShift key-value pair. 
 * In either case (random or regular scan), the module also writes the gaze angles to an output port, 
 * simulating the functionality of the attentionSelection module.
 * It also writes encoder values for the head, simulating the functionality of the /robot/head/state:o port on the robot.
 *
 * 
 * \section lib_sec Libraries
 *
 * - YARP.
 *
 * \section parameters_sec Parameters
 * 
 * <b>Command-line Parameters</b>
 * 
 * The following key-value pairs can be specified as command-line parameters by prefixing -- to the key 
 * (e.g. \c --from file.ini . The value part can be changed to suit your needs; the default values are shown below. 
 *
 * - \c from \c imageSource.ini     
 *     - The configuration file
 *
 * - \c context \c components/imageSource/config
 *     - specifies the sub-path from \c $DREAM_ROOT/release
 * to the configuration file
 *
 * - \c name \c imageSource
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
 * The complete output port name to which the image should be streamed
 * 
 * Key | Value
 * :-- | :----
 * gazePort  |  /gaze:o
 * 
 * The complete output port name to which the gaze angles should be streamed
 *
 * Key | Value
 * :-- | :----
 * encoderPort  |  /robot/head/state:o
 * 
 * The complete output port name to which the encoder values should be streamed
 * 
 * Key | Value
 * :-- | :----
 * imageFile  |  image.ppm
 * 
 * specifies the image filename
 *
 * Key | Value
 * :-- | :----
 * frequency  |  10
 * 
 * specifies the number of images to be streamed per second.  
 * A low frequency avoids this module hogging the CPU; this is important if you are streaming several images
 * 
 * Key | Value
 * :-- | :----
 * width  |  320
 * 
 * specifies the width of the image to be streamed.  
 * The image read from the file is rescaled if necessary.
 * 
 * Key | Value
 * :-- | :----
 * height  |  240
 * 
 * specifies the height of the image to be streamed.  
 * The image read from the file is rescaled if necessary.
 * 
 * Key | Value
 * :-- | :----
 * noise  |  20
 * 
 * specifies the random noise level
 * 
 * Key | Value
 * :-- | :----
 * window  |  0
 * 
 * specifies whether or not to extract a sub-image (default 0 is not to do so, in which case the image is scaled)
 * 
 * Key | Value
 * :-- | :----
 * random  |  0
 * 
 * specifies the scan pattern when extracting a sub-image (default 0 is regular scan, row major order)
 * 
 * Key | Value
 * :-- | :----
 * xShift  |  5
 * specifies the horizontal shift in pixels to be used in window mode
 * 
 * Key | Value
 * :-- | :----
 * yShift  |  5
 * 
 * specifies the vertical shift in pixels to be used in window mode
 * 
 * Key | Value
 * :-- | :----
 * horizontalViewAngle  |  120.0
 * 
 * specifies the horizontal field of view in degrees
 * 
 * Key | Value
 * :-- | :----
 * verticalViewAngle  |  90.0
 * 
 * specifies the vertical field of view in degrees
 * 
 * Note: neither of the port names above (\c /image:o or \c /gaze:o) will be prefixed with module name (i.e. \c /imageSource)
 * since the purpose of this module is to simulate the output ports of other modules (e.g. \c /robot/cam/left or \c /attentionSelection/o:position)
 * 
 * \section portsa_sec Ports Accessed
 * 
 * - None
 *                      
 * \section portsc_sec Ports Created
 *
 * <b>Input ports</b>
 *
 *  - \c /imageSource
 * 
 * This port is used to change the parameters of the module at run time or stop the module.
 * The following commands are available
 * 
 *     help
 *     quit
 *     set noise         <n>     ... set the random noise level (where <n> is an integer number in the range 0-255).
 *
 * Note that the name of this port mirrors whatever is provided by the \c --name parameter value
 * The port is attached to the terminal so that you can type in commands and receive replies.
 * The port can be used by other modules but also interactively by a user through the yarp rpc directive, viz.: \c yarp \c rpc \c /imageSource
 * This opens a connection from a terminal to the port and allows the user to then type in commands and receive replies.
 *       
 *
 * <b>Output ports</b>
 *
 *     /imageSource
 * 
 * see above
 *
 *     /image:o
 *     /gaze:o
 *     /robot/head/state:o
 *
 * <b>Port types </b>
 *
 * The functional specification only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *
 *     BufferedPort<ImageOf<PixelRgb> >   outputPort;      
 *     BufferedPort<VectorOf<double> >    gazeOutPort;     //double azimuth, elevation, 'a', 0, 0
 *     BufferedPort<VectorOf<double> >    encoderOutPort;  //double 0, 1, 2, 3, 4, 5
 * 
 * Note that the protocol used for the gazeOutPort is the same as that used by the attentionSelection module
 * when controlling the controlGaze2 module using the /pos port.
 *
 * \section in_files_sec Input Data Files
 *
 * \c image.ppm, or whatever is specified as a argument for the \c --imageFile \c key-value. 
 *
 * \section out_data_sec Output Data Files
 *
 * None
 *
 * \section conf_file_sec Configuration Files
 *
 * \c imageSource.ini  in \c $DREAM_ROOT/release/components/imageSource/config
 * 
 * \section tested_os_sec Tested OS
 *
 * - Windows
 *
 * \section example_sec Example Instantiation of the Component
 * 
 * <tt>imageSource --name imageSource --context components/imageSource/config --from imageSource.ini </tt>
 *
 * \author David Vernon
 * 
 * Copyright (C) 2014 DREAM Consortium
 * 
 */


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
 * 22/09/09  First version validated   DV
 * 20/11/09  Added windowing and gaze functionality   DV
 * 04/01/10  Added encoder functionality   DV
 * 05/04/11  Added parameters to specify window shift in x and y directions DV
 * 06/10/14  Moved imageSourceThread->stop() from ImageSource::close() to ImageSource::interruptModule()
 *           to solve a problem with the module hanging when requested to stop DV
 * 18/11/14  Corrected documentation of the context parameter DV
 * 17/02/15  Documentation markup updated (Rob Homewood)
 * 
 */ 

#include <iostream>
#include <string>
#include <cstdlib>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/Time.h>
 
using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;
using namespace yarp::sig::file;
  


class ImageSourceThread : public RateThread
{
private:

   /* class variables */

   int      x, y;
   PixelRgb rgbPixel;
   ImageOf<PixelRgb> inputImage;
   int input_width;
   int input_height;
   int input_depth;
   int xOffset;
   int yOffset;
   bool debug;
   bool windowFlag;

   /* thread parameters: they are pointers so that they refer to the original variables in imageSource */

   BufferedPort<ImageOf<PixelRgb> > *imagePortOut;   
   BufferedPort<VectorOf<double> >  *gazePortOut;   
   BufferedPort<VectorOf<double> >  *encoderPortOut;   
   int *widthValue;     
   int *heightValue;     
   int *noiseValue; 
   int *windowValue; 
   int *randomValue; 
   int *xShiftValue;
   int *yShiftValue;
   double *horizontalViewAngleValue;
   double *verticalViewAngleValue;

   string *imageFilenameValue;  

public:

   /* class methods */

   ImageSourceThread(BufferedPort<ImageOf<PixelRgb> > *imageOut,  
                     BufferedPort<VectorOf<double> >  *gazePortOut,
                     BufferedPort<VectorOf<double> >  *encoderPortOut, 
                     string *imageFilename,  int period, int *width, int *height, int *noise, int *window, int *random,
                     int *xShift, int *yShift,
                     double *horizontalViewAngle, double *verticalViewAngle);
   bool threadInit();     
   void threadRelease();
   void run(); 
};


class ImageSource:public RFModule
{

   /* class variables */

   bool debug; 

   /* module parameters */

   string moduleName;
   string outputPortName;  
   string gazePortName;
   string encoderPortName;
   string handlerPortName;
   string imageFilename;
   int    frequency;
   int    width;
   int    height;
   int    noiseLevel;
   int    window;
   int    random;
   int    xShift;
   int    yShift;
   double horizontalViewAngle;
   double verticalViewAngle;

   /* class variables */

   BufferedPort<ImageOf<PixelRgb> > imageOut;     // output port
   BufferedPort<VectorOf<double> >  gazeOut;      // gaze port
   BufferedPort<VectorOf<double> >  encoderOut;   // encoder port
   Port handlerPort;                              // a port to handle messages 

   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   ImageSourceThread *imageSourceThread;

public:
   
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
};


 