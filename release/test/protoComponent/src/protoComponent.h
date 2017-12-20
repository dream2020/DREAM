/** @file protoComponent.h  Interface file for the example DREAM component
 *
 * Version 1.2
 * 
 * 18/11/2014
 *
 * \defgroup DREAM_protoComponent protoComponent
 * 
 * Interface file for the example DREAM component
 * 
 * \section component_description Component Description
 * 
 * This application demonstates the use of the protoComponent module. 
 * This is a simple example to illustrate a component that is compliant with
 * mandatory DREAM software standards for
 * 
 * 1. File organization
 * 2. Internal source code documentation
 * 3. Component functionality
 * 
 * as well as recommended DREAM software standards for 
 * 
 * 4. Programming style
 * 5. Programming practice 
 * 
 * These standards are documented in Appendices A-E of Deliverable 3.2 
 * 
 * Overall, the standards address the 4 Cs of component-based software engineering:
 * 
 * - configuration
 * - coordination
 * - computation
 * - computation
 *
 * Functionally, the module 
 * - reads a colour image from an input port,
 * - reads a threshold value from an imput port,
 * - converts it to a binary image based on a supplied threshold,
 * - counts the number of foreground pixels and background pixels,
 * - writes the binary image to an output port,
 * - and writes the pixel statistics and the time to another output port.
 *
 * The purpose of these operations is simply to illustrate how input/ouput is performed.  
 * The image processing itself is trivial and of no importance.
 *
 * The module also reads a configuration file containing the intrinsic
 * parameters of the robot's two cameras as an example of more advanced parsing 
 * techniques.
 *
 * A complete tutorial on this example is available on the DREAM wiki 
 *
 * 
 * \section lib_sec Libraries
 *
 * - YARP.
 *
 * \section parameters_sec Parameters
 * 
 * <b>Command-line Parameters </b> 
 * 
 * The following key-value pairs can be specified as command-line parameters 
 * by prefixing \c -- to the key e.g. \c --from file.ini. 
 * The value part can be changed to suit your needs; the default values are 
 * shown below. 
 *
 * - \c from \c protoComponent.ini 
 *     - specifies the configuration file
 *
 * - \c context \c components/protoComponent/config
 *     - specifies the sub-path from \c $DREAM_ROOT/release to the configuration file
 *
 * - \c name \c protoComponent
 *     - specifies the name of the module (used to form the stem of module port names)  
 *
 *
 * <b>Configuration File Parameters </b>
 *
 * The following key-value pairs can be specified as parameters in the configuration file 
 * (they can also be specified as command-line parameters if you so wish). 
 * The value part can be changed to suit your needs; the default values are shown below. 
 * 
 * Key | Value
 * :-- | :----
 * imageInputPort | /image:i  
 *
 *  - Specifies the image input port name (this string will be prefixed by \c /protoComponent 
 *   or whatever else is specifed by the name parameter
 * 
 * Key | Value
 * :-- | :----  
 * thresholdInputPort | /threshold:i
 *
 *  - Specifies the threshold input port name (this string will be prefixed by \c /protoComponent 
 *   or whatever else is specifed by the name parameter
 * 
 * Key | Value
 * :-- | :----
 * imageOutputPort | /image:o
 *
 * - Specifies the image output port name (this string will be prefixed by \c /protoComponent 
 *   or whatever else is specifed by the name parameter
 * 
 * Key | Value
 * :-- | :----
 * statisticsOutputPort | /statistics:o
 * 
 * - Specifies the statistics output port name (this string will be prefixed by \c /protoComponent 
 *   or whatever else is specifed by the name parameter
 * 
 * Key | Value
 * :-- | :----
 * cameraConfig | cameras.ini
 * 
 * - Specifies the camera configuration file containing the intrinsic parameters of
 *   the left and right cameras
 * 
 * Key | Value
 * :-- | :----
 * threshold | 7           
 * 
 * - Specifies the threshold value
 *
 * 
 * \section portsa_sec Ports Accessed
 * 
 * - None
 *                      
 * \section portsc_sec Ports Created
 *
 * <b>Input ports</b>
 *
 *  - \c /protoComponent 
 *
 * This port is used to change the parameters of the module at run time or stop the module. \n
 * The following commands are available:
 * 
 *     help
 *     quit
 *     set thr         <n>     ... set the threshold for binary segmentation of the input RGB image (where \c <n> is an integer number)
 * 
 * Note that the name of this port mirrors whatever is provided by the \c --name parameter value
 * The port is attached to the terminal so that you can type in commands and receive replies.
 * The port can be used by other modules but also interactively by a user through the yarp rpc directive, viz.: \c yarp \c rpc \c /protoComponent
 * This opens a connection from a terminal to the port and allows the user to then type in commands and receive replies.
 *       
 *     /protoComponent/image:i   
 *     /protoComponent/threshold:i
 *
 * <b>Output ports</b>
 *
 *     /protoComponent
 *
 * see above
 *
 *     /protoComponent/image:o
 *     /protoComponent/statistics:o
 *
 * <b>Port types </b>
 *
 * The functional specification only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *
 *     BufferedPort<ImageOf<PixelRgb> >     imageIn;
 *     BufferedPort<VectorOf<int> >         thresholdIn; //int threshold 
 *     BufferedPort<ImageOf<PixelRgb> >     imageOut;
 *     BufferedPort                         statisticsOut;    
 *
 * Thus, there are three types of port in use here, all buffered: 
 * 
 * - image, 
 * - vector of int (there will be just one element in the vector in this case), and 
 * - a bottle comprising alphanumerics messages.
 *
 *
 * \section in_files_sec Input Data Files
 *
 * None
 *
 * \section out_data_sec Output Data Files
 *
 * None
 *
 * \section conf_file_sec Configuration Files
 *
 * \c protoComponent.ini   
 * \c cameras.ini  
 *
 * \section example_sec Example Instantiation of the Component
 * 
 * <tt>protoComponent --name protoComponent --context components/protoComponent/config --from protoComponent.ini </tt>
 *
 * \author 
 * 
 * <name of author>, <author institute>  
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
 * 20/08/14  Version 1.0: First version validated (David Vernon)
 * 06/10/14  Version 1.1: Additional ports for threshold input and statistics output (David Vernon)
 * 18/11/14  Version 1.2: Corrected documentation of the context parameter (David Vernon)
 * 17/02/15  Documentation markup updated (Rob Homewood)
 * 17/06/16  Docs, copyright and application files updated (Erik Billing)
 */  
 
#include <iostream>
#include <string>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
 
using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;
  
class ProtoComponentThread : public Thread {

private:

   /* class variables */

   bool               debug;
   int                x, y;
   PixelRgb           rgbPixel;
   ImageOf<PixelRgb> *image;
   int               *thresholdValue;   
   VectorOf<int>     *thresholdVector;
   int               numberOfForegroundPixels;

   /* thread parameters: they are pointers so that they refer to the original variables in protoComponent */

   BufferedPort<ImageOf<PixelRgb> > *imagePortIn;
   BufferedPort<VectorOf<int> >     *thresholdPortIn; 
   BufferedPort<ImageOf<PixelRgb> > *imagePortOut; 
   BufferedPort<Bottle>             *statisticsPortOut; 
  

public:

   /* class methods */

   ProtoComponentThread(BufferedPort<ImageOf<PixelRgb> > *imageIn, 
                        BufferedPort<VectorOf<int> >     *thresholdIn, 
                        BufferedPort<ImageOf<PixelRgb> > *imageOut,
                        BufferedPort<Bottle>             *statisticsOut, 
                        int *threshold );
   bool threadInit();     
   void threadRelease();
   void run(); 
};


class ProtoComponent:public RFModule {

   /* module parameters */

   string moduleName;
   string imageInputPortName;
   string thresholdInputPortName;
   string imageOutputPortName;
   string statisticsOutputPortName;  
   string handlerPortName;
   string cameraConfigFilename;
   float  fxLeft,  fyLeft;          // focal length
   float  fxRight, fyRight;         // focal length
   float  cxLeft,  cyLeft;          // coordinates of the principal point
   float  cxRight, cyRight;         // coordinates of the principal point
   int    thresholdValue;

   /* class variables */

   BufferedPort<ImageOf<PixelRgb> > imageIn;       // example image input port
   BufferedPort<VectorOf<int> >     thresholdIn;   // example vector input port 
   BufferedPort<ImageOf<PixelRgb> > imageOut;      // example image output port
   BufferedPort<Bottle>             statisticsOut; // example bottle output port
   Port handlerPort;                               // a port to handle interactive messages (also uses bottles)

   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   ProtoComponentThread *protoComponentThread;

public:
   
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
};

 