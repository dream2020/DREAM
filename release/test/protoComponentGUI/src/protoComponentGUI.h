/** @file protoComponentGUI.h  Interface file for the example DREAM component
 *
 * Version 1.2
 * 
 * 18/11/2014
 *
 *
 * \defgroup DREAM_protoComponentGUI protoComponentGUI
 *
 * Interface file for the example DREAM component
 * 
 * \section component_description Component Description
 * 
 * protoComponentGUI is a simple example to illustrate how to use the FLTK library 
 * to create a graphic user interface component that can send and receive information  
 * to and fromother components using variety of YARP ports. 
 * 
 * protoComponentGUI complements the functionality of the protoComponent component:
 * 
 * - reading the binary image output from protoComponent and displaying it in a graphic display, 
 * - reading the statistics text from protoComponent and displaying it in an output box,
 * - using an FLTK GUI widget slider to set the binary threshold and send it to protoComponent,
 * - and reading & displaying the original colour image in a separate display. 
 *
 * It makes use of a library \c guiUtilities developed by David Vernon.  This library implements
 * several classes to manipulate and display images.  It also contains example callback functions
 * for standard FLTK evaluators such as sliders and radio buttons as well as writing messages to 
 * a display box. guiUtilities is included in the DREAM software project so that it can be extended
 * as required.
 *
 * It also illustrates how to use the YARP resourceFinder class to locate files, 
 * in this case the DREAM logo image file, in the standard DREAM directories searched by YARP.
 *
 * A complete tutorial on this example is available on the DREAM wiki.
 *
 * 
 * \section lib_sec Libraries
 *
 * - YARP
 * - FLTK: 
 *     - fltk.lib
 *     - fltkgl.lib
 *     - fltkimages.lib
 *     - fltkpng.lib
 *     - fltkzlib.lib
 * - guiUtilities.lib
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
 * - \c from \c protoComponentGUI.ini
 *     - specifies the configuration file
 *
 * - \c context \c components/protoComponentGUI/config
 *     - specifies the sub-path from \c $DREAM_ROOT/release
 *   to the configuration file
 *
 * - \c name \c protoComponentGUI
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
 * colourImageInputPort  |  /colourimage:i
 * 
 * specifies the image input port name for the colour image 
 * 
 * Key | Value
 * :-- | :----  
 * binaryImageInputPort  |  /binaryimage:i
 * 
 * specifies the image input port name for the binary image 
 * 
 * Key | Value
 * :-- | :----
 * statisticsInputPort  |  /statistics:i
 * 
 * specifies the statistics input port name 
 * 
 * Key | Value
 * :-- | :----
 * thresholdOutputPort  |  /threshold:o
 * 
 * specifies the threshold output port name 
 * 
 * Note: all these filename strings will be prefixed by \c /protoComponentGUI
 * (or whatever else is specifed by the \c name parameter)
 * 
 * Key | Value
 * :-- | :----
 * threshold  |  7 
 * 
 * specifies the threshold value
 * 
 * \section portsa_sec Ports Accessed
 * 
 * - None
 *                      
 * \section portsc_sec Ports Created
 *
 * <b>Input ports</b>
 *
 *  - \c /protoComponentGUI
 * This port is used to change the parameters of the module at run time or stop the module. \n
 * The following commands are available
 * 
 *     help \n
 *     quit \n
 *     set thr         <n>     ... set the threshold for binary segmentation of the input RGB image (where <n> is an integer number)
 *
 * Note that the name of this port mirrors whatever is provided by the \c --name parameter value
 * The port is attached to the terminal so that you can type in commands and receive replies.
 * The port can be used by other modules but also interactively by a user through the yarp rpc directive, viz.: \c yarp \c rpc \c /protoComponentGUI
 * This opens a connection from a terminal to the port and allows the user to then type in commands and receive replies.
 *       
 *     /protoComponentGUI/colourImage:i
 *     /protoComponentGUI/binaryImage:i   
 *     /protoComponentGUI/statistics:i
 *
 * <b>Output ports</b>
 *
 *     /protoComponentGUI/threshold:o *
 *     /protoComponentGUI (see above)
 *
 * <b>Port types </b>
 *
 * The functional specification above only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *
 *     BufferedPort<ImageOf<PixelRgb> >    colourImageIn; 
 *     BufferedPort<ImageOf<PixelRgb> >    binaryImageIn; 
 *     BufferedPort                        statisticsIn;     
 *     BufferedPort<VectorOf<int> >        thresholdOut; //int threshold 
 *
 * Thus, there are three types of port in use here, all buffered: 
 * 
 * - image, 
 * - vector of int (there will be just one element in the vector in this case), and 
 * - a bottle comprising various alphanumeric messages.
 *
 *
 * \section in_files_sec Input Data Files
 * 
 * \c DREAM_223x59.png 
 *
 * This the DREAM logo. It is located in C:/DREAM/release/components/protoComponentGUI/config 
 * but the resourceFinder identifies this path automatically.
 *
 * \section out_data_sec Output Data Files
 *
 * None
 *
 * \section conf_file_sec Configuration Files
 *
 * \c protoComponentGUI.ini   
 *
 * \section example_sec Example Instantiation of the Component
 * 
 * <tt>protoComponentGUI --name protoComponentGUI --context components/protoComponentGUI/config --from protoComponentGUI.ini </tt>
 *
 * \author 
 * 
 * David Vernon, University of Skövde
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
 * 30/10/14  Version 1.0: First version validated (David Vernon)
 * 16/11/14  Version 1.1: Fixed problem when the default display size is too big for the available window size (David Vernon)
 * 18/11/14  Version 1.2: Corrected documentation of the context parameter (David Vernon)
 * 17/02/15  Documentation markup updated (Rob Homewood)
 *
 */  
 
#include <iostream>
#include <string>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
 
#include "guiUtilities.h"

using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;
  
#define STRINGLENGTH 132 // used to define a string when post-processing the bottle messages

class ProtoComponentGUIThread : public Thread {

private:

   /* class variables */

   bool              debug;
   int               x, y;
   PixelRgb          rgbPixel;
   int               *thresholdValue;   
   int               numberOfForegroundPixels;
   int               rgb_width;
   int               rgb_height;
   int               rgb_depth;
   int               binary_width;
   int               binary_height;
   int               binary_depth;
   unsigned char     pixel_value;
   float             float_pixel_value;
   double            thresholdValueDouble;
   string            logoFilenameValue;
   int               temp;

   ImageOf<PixelRgb> *rgbYARPImage;
   ImageOf<PixelRgb> *binaryYARPImage;
   DVimage           *rgbDVImage;
   DVimage           *binaryDVImage;
   Bottle            *statisticsMessage; 

   /* thread parameters: they are pointers so that they refer to the original variables in protoComponentGUI */

   BufferedPort<ImageOf<PixelRgb> > *colourImagePortIn;
   BufferedPort<ImageOf<PixelRgb> > *binaryImagePortIn;
   BufferedPort<Bottle>             *statisticsPortIn; 
   BufferedPort<VectorOf<int> >     *thresholdPortOut; 

public:

   /* class methods */

   ProtoComponentGUIThread(BufferedPort<ImageOf<PixelRgb> > *colourImageIn, 
                           BufferedPort<ImageOf<PixelRgb> > *binaryImageIn,
                           BufferedPort<Bottle>             *statisticsIn, 
                           BufferedPort<VectorOf<int> >     *thresholdOut, 
                           int *threshold, 
                           string logoFilename);
   bool threadInit();     
   void threadRelease();
   void run(); 
};


class ProtoComponentGUI:public RFModule {

   /* module parameters */

   string moduleName;
   string colourImageInputPortName;
   string thresholdOutputPortName;
   string binaryImageInputPortName;
   string statisticsInputPortName;  
   string handlerPortName;
   string logoFilename;
   int    thresholdValue;

   /* class variables */

   BufferedPort<ImageOf<PixelRgb> > colourImageIn;  // example image input port
   BufferedPort<ImageOf<PixelRgb> > binaryImageIn;  // example image input port
   BufferedPort<Bottle>             statisticsIn;   // example bottle input port
   BufferedPort<VectorOf<int> >     thresholdOut;   // example vector output port
   Port handlerPort;                                // a port to handle interactive messages (also uses bottles)

   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   ProtoComponentGUIThread *protoComponentGUIThread;

public:
   
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
};

 