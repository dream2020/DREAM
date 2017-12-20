/** @file systemArchitectureGUI.h  Interface file for the graphic user interface component in the DREAM system architecture
 *
 * Version 1.0
 * 
 * 08/12/2014
 *
 * \defgroup DREAM_systemArchitectureGUI systemArchitectureGUI
 *
 * Interface file for the graphic user interface component in the DREAM system architecture
 *
 * \section component_description Component Description
 *
 * The DREAM system architecture, described in Deliverable D3.1, comprises three main sub-systems 
 * that correspond to the following work-packages
 *
 * - WP4: Sensing and Interpretation, 
 * - WP5: Child Behaviour Analysis, and 
 * - WP6: Robot Behaviour. 
 * 
 * Initially, these three sub-systems are implemented by three placeholder components, as follows.
 * 
 * - sensoryInterpretation
 * - cognitiveControl
 * - cognitiveControl
 * 
 * This component is a Graphic User Interface (GUI) to facilitate external control of the robot by a user 
 * (either a therapist or a software developer) and to provide the user with an easily-to-understand view 
 * on the current state of the robot cognitive control. 
 * 
 * It also provides a graphic rendering of the child's behavioural state, degree of engagement,
 * and degree of performance in the current intervention.
 *
 * All of the cognitiveController input and output ports can be connected to this GUI as well as the output ports
 * from the childBehaviourClassification component.
 *
 * Additionally, the user interface allows a software developer to change the parameters that control the 
 * discrete event simulation of each of the three placeholder component sub-systems.
 * Four simulation parameters are provided to allow the developer to adjust the simulation without having 
 * to recompile the placeholder code.  These are:
 *
 * - \c verbose
 *     - If this parameter has the value "on", messages will be sent to the run server to echo 
 *       the input and output to/from each primitive; if "off" no messages are sent.
 *
 * - \c cycle
 *     - If this parameter has the value "on", only one primitive at a time is set to have  
 *       a non-zero event rate; if "off" all primitives have the specified non-zero event rate.
 *
 * - \c cycle_period
 *     - The period for which a primitive is the only one to have a non-zero event rate.
 *
 * - \c event_rate
 *     - The number of events (i.e. perception primitive invocations) per second.
 *       At present, the same event rate is applied to all primitives except in cycle mode.
 *
 * The event rate determines the Poisson distribution parameter lambda, the mean number of events.
 * Lambda is computed as the event rate x the time taken to execute one discrete event loop.
 * This event loop time is measured in real-time and is not a fixed discrete event simulation interval.
 * Again, this produces a more realistic simulation. 
 * 
 * These parameters are set using the port associated with each placeholder subsystem component
 * using commands sent in a bottle:
 *
 * - \c verbose       on | off
 * - \c cycle         on | off
 * - \c cycle_period  <n>
 * - \c event_rate    <n>
 *
 * The GUI makes use of a library \c guiUtilities developed by David Vernon.  This library implements
 * several classes to manipulate and display images.  It also contains example callback functions
 * for standard FLTK evaluators such as sliders and radio buttons as well as writing messages to 
 * a display box. guiUtilities is included in the DREAM software project so that it can be extended
 * as required.
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
 *     - guiUtilities.lib
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
 * - \c from \c systemArchitectureGUI.ini 
 *     - specifies the configuration file
 *
 * - \c context \c systemArchitectureGUI/configuration
 *     - specifies the sub-path from \c $DREAM_ROOT/systemArchitectureGUI/configuration 
 *       to the configuration file
 *
 * - \c name \c systemArchitectureGUI  
 *     - specifies the name of the module (used to form the stem of module port names)  
 *
 *
 * <b>Configuration File Parameters </b>
 *
 * The following key-value pairs can be specified as parameters in the configuration file 
 * (they can also be specified as command-line parameters if you so wish). 
 * The value part can be changed to suit your needs; the default values are shown below. 
 *   
 * Ports associated with sensoryInterpretation component/subsystem                   
 *
 * Key | Value
 * :-- | :----
 * \c sensoryInterpretationOutputPort         |    \c sensoryInterpretation:o 
 *
 * Ports associated with childBehaviourClassification component/subsystem
 *
 * Key | Value
 * :-- | :----
 * \c getChildBehaviourInputPort              |      \c getChildBehaviour:i
 * \c getChildMotivationInputPort             |      \c getChildMotivation:i
 * \c getChildPerformanceInputPort            |      \c getChildPerformance:i
 * \c childBehaviourClassificationOutputPort  |      \c childBehaviourClassification:o
 * 
 * Ports associated with cognitiveControl component/subsystem
 *
 * Key | Value
 * :-- | :----
 * \c getInterventionStateInputPort           |     \c getInterventionState:i
 * \c gripOutputPort                          |     \c grip:o
 * \c moveHandOutputPort                      |     \c moveHand:o
 * \c moveHeadOutputPort                      |     \c moveHead:o
 * \c moveSequenceOutputPort                  |     \c moveSequence:o
 * \c moveTorsoOutputPort                     |     \c moveTorso:o
 * \c sayOutputPort                           |     \c say:o
 * \c enableRobotOutputPort                   |     \c enableRobot:o
 * \c cognitiveControlOutputPort              |     \c cognitiveControl:o
 *       
 * 
 *  Note: all these filename strings will be prefixed by \c /systemArchitectureGUI
 *  (or whatever else is specifed by the \c name parameter)
 * 
 * \section portsa_sec Ports Accessed
 * 
 * - None
 *                      
 * \section portsc_sec Ports Created
 *
 * <b>Input ports</b>
 *
 *  - \c /systemArchitectureGUI
 * 
 * This port is used to change the parameters of the module at run time or stop the module. \n
 * The following commands are available:
 * 
 *     help
 *     quit
 * 
 * Note that the name of this port mirrors whatever is provided by the \c --name parameter value.
 * The port is attached to the terminal so that you can type in commands and receive replies.
 * The port can be used by other modules but also interactively by a user through the yarp rpc directive, 
 * viz.: \c yarp \c rpc \c /systemArchitectureGUI
 * This opens a connection from a terminal to the port and allows the user to then type in commands and receive replies.
 * 
 * Ports originating in \c childBehaviourClassification:
 * 
 *     /systemArchitectureGUI/getChildBehaviour:i            
 *     /systemArchitectureGUI/getChildMotivation:i           
 *     /systemArchitectureGUI/getChildPerformance:i          
 * 
 * Ports originating in \c cognitiveControl:                  
 *
 *     /systemArchitectureGUI/getInterventionState:i         
 *
 * <b>Output ports</b>
 *
 *     /systemArchitectureGUI/grip:o                         
 *     /systemArchitectureGUI/moveHand:o                     
 *     /systemArchitectureGUI/moveHead:o                     
 *     /systemArchitectureGUI/moveSequence:o                 
 *     /systemArchitectureGUI/moveTorso:o                    
 *     /systemArchitectureGUI/say:o                          
 *     /systemArchitectureGUI/enableRobot:o                  
 *     /systemArchitectureGUI                                
 *     /sensoryInterpretation:o                              
 *     /childBehaviourClassification:o                       
 *     /cognitiveControl:o                                   
 *
 * <b>Port types </b>
 *
 * The functional specification above only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 

 *     Port                             sensoryInterpretationOut;        
 *    
 *     BufferedPort<VectorOf<double>>    getChildBehaviourIn;              
 *     BufferedPort<VectorOf<double>>    getChildMotivationIn;             
 *     BufferedPort<VectorOf<double>>    getChildPerformanceIn;            
 *     Port                              childBehaviourClassificationOut; 
 *    
 *     BufferedPort<VectorOf<int>>       getInterventionStateIn;           
 *     BufferedPort<VectorOf<int>>       gripOut;                         
 *     BufferedPort<VectorOf<double>>    moveHandOut;                     
 *     BufferedPort<VectorOf<double>>    moveHeadOut;                     
 *     BufferedPort<VectorOf<int>>       moveSequenceOut;                 
 *     BufferedPort<VectorOf<double>>    moveTorsoOut;                    
 *     BufferedPort<Bottle>              sayOut;                          
 *     BufferedPort<VectorOf<int>>       enableRobotOut;                  
 *     Port                              cognitiveControlOut;
 *
 * \section in_files_sec Input Data Files
 *
 * \c DREAM_223x59.png 
 *
 * This the DREAM logo. It is located in C:/DREAM/release/components/systemArchitectureGUI/config 
 * but the resourceFinder identifies this path automatically.
 *
 * \section out_data_sec Output Data Files
 *
 * None
 *
 * \section conf_file_sec Configuration Files
 *
 * \c systemArchitectureGUI.ini   
 *
 * \section example_sec Example Instantiation of the Component
 * 
 * <tt>systemArchitectureGUI --name systemArchitectureGUI --context components/systemArchitectureGUI/config --from systemArchitectureGUI.ini </tt>
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
 * 08/10/14  First version validated (David Vernon)
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

/* definitions for the GUI layout */

#define BEHAVIOUR_DISPLAY_WIDTH   300         // width of the child behaviour classification profile displays
#define BEHAVIOUR_DISPLAY_HEIGHT   50         // height of the child behaviour classification profile displays
#define NUMBER_OF_CHILD_BEHAVIOURAL_STATES 5  // the number of behavioural states that can be returned by the getChildBehaviour() primitive.
                                              // For each state, the primitive determines the probability that the child is in that state  
                                              // and returns a list of (state, probability) tuples.
#define X_AXIS        1
#define Y_AXIS        2
#define Z_AXIS        3
#define ROLL_AXIS     4  
#define LEFT_HAND     0
#define RIGHT_HAND    1
#define HAPPY_VOICE   1
#define SAD_VOICE     2
#define ANGRY_VOICE   3 
#define FEARFUL_VOICE 4

class SystemArchitectureGUIThread : public Thread {

private:

   /* class variables */

   bool              debug;
   string            logoFilenameValue;
 
   double thresholdValueDouble;

   /* thread parameters: they are pointers so that they refer to the original variables in systemArchitectureGUI */
   
   bool                            *verbose;
   bool                            *cycle;
   double                          *cyclePeriod;
   double                          *eventRate;

   Port                            *sensoryInterpretationPortOut;

   BufferedPort<VectorOf<double>>  *getChildBehaviourPortIn;   
   BufferedPort<VectorOf<double>>  *getChildMotivationPortIn; 
   BufferedPort<VectorOf<double>>  *getChildPerformancePortIn; 
   Port                            *childBehaviourClassificationPortOut;
  
   BufferedPort<VectorOf<int>>     *getInterventionStatePortIn;
   BufferedPort<VectorOf<int>>     *gripPortOut;
   BufferedPort<VectorOf<double>>  *moveHandPortOut;
   BufferedPort<VectorOf<double>>  *moveHeadPortOut;
   BufferedPort<VectorOf<int>>     *moveSequencePortOut;
   BufferedPort<VectorOf<double>>  *moveTorsoPortOut;
   BufferedPort<Bottle>            *sayPortOut;
   BufferedPort<VectorOf<int>>     *enableRobotPortOut;
   Port                            *cognitiveControlPortOut;

public:

   /* class methods */

   SystemArchitectureGUIThread(
                               bool                            *verbose,
                               bool                            *cycle,
                               double                          *cyclePeriod,
                               double                          *eventRate,
                               Port                            *sensoryInterpretationOut,
                               BufferedPort<VectorOf<double>>  *getChildBehaviourIn,   
                               BufferedPort<VectorOf<double>>  *getChildMotivationIn, 
                               BufferedPort<VectorOf<double>>  *getChildPerformanceIn, 
                               Port                            *childBehaviourClassificationOut, 
                               BufferedPort<VectorOf<int>>     *getInterventionStateIn,
                               BufferedPort<VectorOf<int>>     *gripOut,
                               BufferedPort<VectorOf<double>>  *moveHandOut,
                               BufferedPort<VectorOf<double>>  *moveHeadOut,
                               BufferedPort<VectorOf<int>>     *moveSequenceOut,
                               BufferedPort<VectorOf<double>>  *moveTorsoOut,
                               BufferedPort<Bottle>            *sayOut,
                               BufferedPort<VectorOf<int>>     *enableRoboOut,
                               Port                            *cognitiveControlOut,
                               string logoFilename);
   bool threadInit();     
   void threadRelease();
   void run(); 
};


class SystemArchitectureGUI:public RFModule {

   /* simulation control parameters */

   string verboseParameter; 
   bool   verbose;
   string cycleParameter;
   bool   cycle;
   double cyclePeriod;
   double eventRate;

   /* module parameters */

   string moduleName;

   string handlerPortName;
   string logoFilename;
   string sensoryInterpretationOutputPortName;
   string getChildBehaviourInputPortName;
   string getChildMotivationInputPortName;
   string getChildPerformanceInputPortName;
   string childBehaviourClassificationOutputPortName;
   string gripOutputPortName;
   string moveHandOutputPortName;
   string moveHeadOutputPortName;
   string moveSequenceOutputPortName;
   string moveTorsoOutputPortName;
   string sayOutputPortName;
   string enableRobotOutputPortName;
   string getInterventionStateInputPortName;
   string cognitiveControlOutputPortName;


   /* class variables */
     
   Port                            handlerPort;                  // a port to handle interactive messages (also uses bottles)

   Port                            sensoryInterpretationOut;
   BufferedPort<VectorOf<double>>  getChildBehaviourIn;   
   BufferedPort<VectorOf<double>>  getChildMotivationIn; 
   BufferedPort<VectorOf<double>>  getChildPerformanceIn; 
   Port                            childBehaviourClassificationOut;
   BufferedPort<VectorOf<int>>     getInterventionStateIn;
   BufferedPort<VectorOf<int>>     gripOut;
   BufferedPort<VectorOf<double>>  moveHandOut;
   BufferedPort<VectorOf<double>>  moveHeadOut;
   BufferedPort<VectorOf<int>>     moveSequenceOut;
   BufferedPort<VectorOf<double>>  moveTorsoOut;
   BufferedPort<Bottle>            sayOut;
   BufferedPort<VectorOf<int>>     enableRobotOut;
   Port                            cognitiveControlOut;

   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   SystemArchitectureGUIThread *systemArchitectureGUIThread;

public:
   
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
};

void pause(double period);
void get_input_cb(Fl_Widget *temp, void *);
void happy_voice_cb(Fl_Widget *b,void *destination);
void sad_voice_cb(Fl_Widget *b,void *destination);
void angry_voice_cb(Fl_Widget *b,void *destination);
void fearful_voice_cb(Fl_Widget *b,void *destination);
 