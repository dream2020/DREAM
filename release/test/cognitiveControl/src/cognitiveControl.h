/** @file cognitiveControl.h  Interface file for the cognitive control subsystem in the DREAM system architecture
 *
 * Version 1.0
 * 
 * 04/12/2014
 *
 *
 * \defgroup DREAM_cognitiveControl cognitiveControl
 * 
 * Interface file for the cognitive control subsystem in the DREAM system architecture
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
 * - childBehaviourClassification
 * - cognitiveControl
 * 
 * The current component is the implementation of the cognitiveControl placeholder component.
 * It provides stub functionality for the ten cognitiveControl primitives (see D3.1).
 *
 * - grip()
 * - moveHand(handDescriptor, x, y, z, roll)
 * - moveHead (x, y, z)
 * - moveSequence(sequenceDescriptor)
 * - moveTorso (x, y, z)
 * - say(text, tone)
 * - enableRobot()
 * - disableRobot()
 * - getInterventionState(interventionDescriptor, stateDescriptor, cognitiveModeDescriptor)
 *
 * This means that it reads any input parameters from the associated input port (where necessary) and
 * writes simulated results (random values) to the associated output port (where necessary).  
 * In particular, is reads all the ports exposed by the sensoryInterpretation and 
 * childBehaviourClassification placeholder components and, if operating in verbose mode, 
 * echoes the data read to the terminal.   It then writes simulated (random) data to the ports
 * associated with the ten cognitiveControl primitives.
 * 
 * This placeholder component does nothing except send and receive simulated data on each of the ports 
 * defined in the sub-system specification. The purpose of the placeholder is merely to provide 
 * a working example of the implementation and use of these ports using the data communication protocols
 * specified in Deliverable D3.1. These specifications are replicated in the section Ports Created below. 
 * 
 * The component operates as a discrete event simulation.
 * An event occurs when a classification primitive returns a value. This is modelled by a Poisson distribution, 
 * with each primitive having its own event rate.  This provides for more realistic simulation of 
 * the behaviour of the sub-system for which it is a placeholder. It also has the advantage that it is
 * possible to set all but one event rates to zero for a certain period of time, and to cycle through
 * each primitive as the simulation proceeds. This makes it easier to check the operation of the component
 * expecially when all three placeholder components are working together.
 *
 * Four simulation parameters are provided to allow the developer to adjust the simulation without having 
 * to recompile the placeholder code.  These are:
 *
 *     verbose       If this parameter has the value "on", messages will be sent to the run server to echo 
 *                   the input and output to/from each primitive; if "off" no messages are sent.
 *
 *     cycle         If this parameter has the value "on", only one primitive at a time is set to have  
 *                   a non-zero event rate; if "off" all primitives have the specified non-zero event rate.
 *
 *     cycle_period  The period for which a primitive is the only one to have a non-zero event rate.
 *
 *     event_rate    The number of events (i.e. perception primitive invocations) per second.
 *                   At present, the same event rate is applied to all primitives except in cycle mode.
 *
 * The event rate determines the Poisson distribution parameter lambda, the mean number of events.
 * Lambda is computed as the event rate x the time taken to execute one discrete event loop.
 * This event loop time is measured in real-time and is not a fixed discrete event simulation interval.
 * Again, this produces a more realistic simulation. 
 * 
 * The functionality of each sub-system will be developed incrementally as the project progresses 
 * with new components being developed to implement each primitive. Integration of these new primitives is
 * made easy by simply setting the event rate of the stub versions in this placeholder to zero.
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
 * Key | Value
 * :-- | :----
 * from  |  cognitiveControl.ini
 * 
 * specifies the configuration file
 * 
 * Key | Value
 * :-- | :----
 * context  |  components/cognitiveControl/config
 * 
 * specifies the sub-path from \c $DREAM_ROOT/release/ 
 * to the configuration file
 * 
 * Key | Value
 * :-- | :----
 * name  |  cognitiveControl
 * 
 * specifies the name of the module (used to form the stem of module port names)  
 *
 *
 * <b>Configuration File Parameters </b>
 *
 * The following key-value pairs can be specified as parameters in the configuration file 
 * (they can also be specified as command-line parameters if you so wish). 
 * The value part can be changed to suit your needs; the default values are shown below. 
 *
 * Parameters for controlling the simulation
 * 
 *     verbose                           on
 *     cycle                             on
 *     cycle_period                      0.5
 *     event_rate                        20.0
 *
 * Parameters associated with the functions being simulated (i.e. the input and output port names) \n
 *
 * Note that all these default values for port names will be prefixed by \c /cognitiveControl 
 * or whatever else is specifed by the \c name parameter
 *
 * Ports associated with sensoryInterpretation component\subsystem
 *
 *     checkMutualGazeInputPort          /checkMutualGaze:i
 *     getArmAngleInputPort              /getArmAngle:i
 *     getBodyInputPort                  /getBody:i
 *     getBodyPoseInputPort              /getBodyPose:i
 *     getEyeGazeOutputPort              /getEyeGaze:o
 *     getEyeGazeInputPort               /getEyeGaze:i
 *     getEyesInputPort                  /getEyes:i
 *     getFacesInputPort                 /getFaces:i
 *     getGripLocationOutputPort         /getGripLocation:o
 *     getGripLocationInputPort          /getGripLocation:i
 *     getHandsInputPort                 /getHands:i
 *     getHeadInputPort                  /getHead:i
 *     getHeadGazeInputPort              /getHeadGaze:i
 *     getHeadGazeOutputPort             /getHeadGaze:o
 *     getObjectsInputPort               /getObjects:i
 *     getObjectsOutputPort              /getObjects:o
 *     getObjectTableDistanceInputPort   /getObjectTableDistance:i
 *     getObjectTableDistanceOutputPort  /getObjectTableDistance:o
 *     getSoundDirectionInputPort        /getSoundDirection:i
 *     getSoundDirectionOutputPort       /getSoundDirection:o
 *     identifyFaceInputPort             /identifyFace:i
 *     identifyFaceOutputPort            /identifyFace:o
 *     identifyFaceExpressionInputPort   /identifyFaceExpression:i
 *     identifyFaceExpressionOutputPort  /identifyFaceExpression:o
 *     identifyObjectInputPort           /identifyObject:i
 *     identifyObjectOutputPort          /identifyObject:o
 *     identifyTrajectoryInputPort       /identifyTrajectory:i
 *     identifyTrajectoryOutputPort      /identifyTrajectory:o
 *     identifyVoiceInputPort            /identifyVoice:i
 *     recognizeSpeechInputPort          /recognizeSpeech:i
 *     trackFaceInputPort                /trackFace:i
 *     trackFaceOutputPort               /trackFace:o
 *     trackHandInputPort                /trackHand:i
 *     trackHandOutputPort               /trackHand:o
 *     trackObjectInputPort              /trackObject:i
 *     trackObjectOutputPort             /trackObject:o
 *   
 * Ports associated with childBehaviourClassification component/subsystem
 *
 *     getChildBehaviourInputPort        /getChildBehaviour:i
 *     getChildMotivationInputPort       /getChildMotivation:i
 *     getChildPerformanceInputPort      /getChildPerformance:i
 * 
 * Ports associated with cognitiveControl component/subsystem
 *
 *     gripInputPort                    /grip:i
 *     moveHandInputPort                /moveHand:i
 *     moveHeadInputPort                /moveHead:i
 *     moveSequenceInputPort            /moveSequence:i
 *     moveTorsoInputPort               /moveTorso:i
 *     sayInputPort                     /say:i
 *     enableRobotInputPort             /enableRobot:i
 *     getInterventionStateOutputPort   /getInterventionState:o
 *
 * \section portsa_sec Ports Accessed
 * 
 * - None
 *                      
 * \section portsc_sec Ports Created
 *
 * <b>Input ports</b>
 *
 *  - \c /cognitiveControl
 * 
 * This port is used to change the parameters of the module at run time or stop the module.
 * The following commands are available
 * 
 *     help
 *     verbose          on          ... echo all data received and sent to the terminal
 *     verbose          off         ... run in quiet mode and do not echo  data received and sent to the terminal
 *     cycle            on          ... set event rate to zero for all but one classification primitive
 *     cycle            off         ... assign event rate specified to all three classification primitives
 *     cycle_period     <n>         ... period in seconds that an event remain non-zero during cycle mode (where <n> is an real number)
 *     event_rate       <n>         ... the mean number of times per second that a primitive is invoked (where <n> is an real number)
 *
 * Note that the name of this port mirrors whatever is provided by the \c --name parameter value
 * The port is attached to the terminal so that you can type in commands and receive replies.
 * The port can be used by other modules but also interactively by a user through the yarp rpc directive, e.g.
 * \c yarp \c rpc \c /cognitiveControl
 * This opens a connection from a terminal to the port and allows the user to then type in commands and receive replies.
 * 
 *     /cognitiveControl  (see above)
 * 
 * Ports originating in \c sensoryInterpretation:
 * 
 *     /cognitiveControl/checkMutualGaze:i
 *     /cognitiveControl/getArmAngle:i
 *     /cognitiveControl/getBody:i
 *     /cognitiveControl/getBodyPose:i
 *     /cognitiveControl/getEyeGaze:i
 *     /cognitiveControl/getEyes:i
 *     /cognitiveControl/getFaces:i
 *     /cognitiveControl/getGripLocation:i
 *     /cognitiveControl/getHands:i
 *     /cognitiveControl/getHead:i
 *     /cognitiveControl/getHeadGaze:i
 *     /cognitiveControl/getObjects:i
 *     /cognitiveControl/getObjectTableDistance:i
 *     /cognitiveControl/getSoundDirection:i
 *     /cognitiveControl/identifyFace:i
 *     /cognitiveControl/identifyFaceExpression:i
 *     /cognitiveControl/identifyObject:i
 *     /cognitiveControl/identifyTrajectory:i
 *     /cognitiveControl/identifyVoice:i
 *     /cognitiveControl/recognizeSpeech:i
 *     /cognitiveControl/trackFace:i
 *     /cognitiveControl/trackHand:i
 *     /cognitiveControl/trackObject:i
 * 
 * Ports originating in \c childBehaviourClassification:
 * 
 *     /cognitiveControl/getChildBehaviour:i
 *     /cognitiveControl/getChildMotivation:i
 *     /cognitiveControl/getChildPerformance:i
 * 
 * Ports originating in \c systemArchitectureGUI:
 * 
 *     /cognitiveControl/grip:i
 *     /cognitiveControl/moveHand:i
 *     /cognitiveControl/moveHead:i
 *     /cognitiveControl/moveSequence:i
 *     /cognitiveControl/moveTorso:i
 *     /cognitiveControl/say:i
 *     /cognitiveControl/enableRobot:i
 * 
 * <b>Output ports</b>
 * 
 * Ports directed at \c sensoryInterpretation:
 * 
 *     /cognitiveControl/getEyeGaze:o
 *     /cognitiveControl/getGripLocation:o
 *     /cognitiveControl/getHeadGaze:o
 *     /cognitiveControl/getObjects:o
 *     /cognitiveControl/getObjectTableDistance:o
 *     /cognitiveControl/getSoundDirection:o
 *     /cognitiveControl/identifyFace:o
 *     /cognitiveControl/identifyFaceExpression:o
 *     /cognitiveControl/identifyObject:o
 *     /cognitiveControl/identifyTrajectory:o
 *     /cognitiveControl/trackFace:o
 *     /cognitiveControl/trackHand:o
 *     /cognitiveControl/trackObject:o
 * 
 * Ports directed at \c systemArchitectureGUI:
 * 
 *     /cognitiveControl/getInterventionState:o
 * 
 * <b>Port types </b>
 *
 * The functional specification only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *
 *     BufferedPort<VectorOf<int>>       checkMutualGazeIn;
 *     BufferedPort<VectorOf<double>>    getArmAngleIn;
 *     BufferedPort<VectorOf<double>>    getBodyIn;
 *     BufferedPort<VectorOf<double>>    getBodyPoseIn;
 *     BufferedPort<VectorOf<double>>    getEyeGazeIn;
 *     BufferedPort<VectorOf<int >>      getEyeGazeOut;
 *     BufferedPort<VectorOf<double>>    getEyesIn;
 *     BufferedPort<VectorOf<double>>    getFacesIn;
 *     BufferedPort<VectorOf<double>>    getGripLocationIn;
 *     BufferedPort<VectorOf<double>>    getGripLocationOut;
 *     BufferedPort<VectorOf<double>>    getHandsIn;
 *     BufferedPort<VectorOf<double>>    getHeadIn;
 *     BufferedPort<VectorOf<double>>    getHeadGazeIn;
 *     BufferedPort<VectorOf<double>>    getHeadGazeOut;
 *     BufferedPort<VectorOf<double>>    getObjectsIn;
 *     BufferedPort<VectorOf<double>>    getObjectsOut;
 *     BufferedPort<VectorOf<double>>    getObjectTableDistanceIn;
 *     BufferedPort<VectorOf<double>>    getObjectTableDistanceOut;
 *     BufferedPort<VectorOf<double>>    getSoundDirectionIn;
 *     BufferedPort<VectorOf<double>>    getSoundDirectionOut;
 *     BufferedPort<VectorOf<int>>       identifyFaceIn;
 *     BufferedPort<VectorOf<double>>    identifyFaceOut;
 *     BufferedPort<VectorOf<int>>       identifyFaceExpressionIn;
 *     BufferedPort<VectorOf<double>>    identifyFaceExpressionOut
 *     BufferedPort<VectorOf<int>>       identifyObjectIn;
 *     BufferedPort<VectorOf<double>>    identifyObjectOut;
 *     BufferedPort<VectorOf<int>>       identifyTrajectoryIn;
 *     BufferedPort<VectorOf<double>>    identifyTrajectoryOut;
 *     BufferedPort<VectorOf<int>>       identifyVoiceIn;
 *     BufferedPort<Bottle>              recognizeSpeechIn;
 *     BufferedPort<VectorOf<double>>    trackFaceIn;
 *     BufferedPort<VectorOf<double>>    trackFaceOut;
 *     BufferedPort<VectorOf<double>>    trackHandIn;
 *     BufferedPort<VectorOf<double>>    trackHandOut;
 *     BufferedPort<VectorOf<double>>    trackObjectIn;
 *     BufferedPort<VectorOf<double>>    trackObjectOut;
 *
 *     BufferedPort<VectorOf<double>>    getChildBehaviourIn;
 *     BufferedPort<VectorOf<double>>    getChildMotivationIn;
 *     BufferedPort<VectorOf<double>>    getChildPerformanceIn;
 *
 *     BufferedPort<VectorOf<int>>       gripIn;
 *     BufferedPort<VectorOf<double>>    moveHandIn;
 *     BufferedPort<VectorOf<double>>    moveHeadIn;
 *     BufferedPort<VectorOf<int>>       moveSequenceIn;
 *     BufferedPort<VectorOf<double>>    moveTorsoIn;
 *     BufferedPort<Bottle>              sayIn;
 *     BufferedPort<VectorOf<int>>       enableRobotIn;
 *     BufferedPort<VectorOf<int>>       getInterventionStateOut;
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
 * \c cognitiveControl.ini   
 *
 *
 * \section example_sec Example Instantiation of the Component
 * 
 * <tt>cognitiveControl --name cognitiveControl --context components/cognitiveControl/config --from cognitiveControl.ini </tt>
 *
 *
 * \author 
 * 
 * David Vernon, University of Skövde, Sweden.
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
 * 04/12/14  First version validated (David Vernon)
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
 
#include <time.h>

using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;
  
#define TRUE 1
#define FALSE 0

#define NUMBER_OF_PRIMITIVES 1    // we allow for different event rates for each output primitive being simulated
                                  // (see Deliverable D3.1)
                                  // this makes it easier to check that the simulation is work by setting the event rate 
                                  // of all but one of the primitives to zero, periodically changing the selected primitive
                                  // every few seconds or so.

#define NUMBER_OF_INTERVENTIONS 7 // used in the getInterventionState() primitive to identify which intervention 
                                  // is currently be enacted

#define NUMBER_OF_MODES 2         // used in the getInterventionState() primitive to identify whether the system is following 
                                  // an intervention or cognitively adapting to an unexpected situation

#define NUMBER_OF_STATES 15       // used in the getInterventionState() primitive to identify the current state in the 
                                  // current intervention

#define STRINGLENGTH 132          // used to define a string when post-processing the bottle messages

 
class CognitiveControlThread : public Thread {

private:

   /* class variables */
    
   bool   debug; 
   double start_time;             // the time the current event loop begain
   double current_time;           // self-evident
   double cumulative_time;        // the total time passed so far since we last changed the primitive with a non-zero event rate when debugging
   double event_rate[NUMBER_OF_PRIMITIVES];
   double elapsed_time;

   int    current_primitive;     // counter to keep track of which primitive's event rate is being used
  
   int    target_primitive;      // counter to keep track of which primitive should be non-zero when debugging
  
   double lambda;                // the mean number of events that occur in any one similation interval 
                                 // (i.e. the time it takes to execute the main while(isStopping() != true) loop
                                 // this time is computed using yarp::os::Time::now()
                                 // lambda is the arrival rate (in events per second) x the elapsed_time (in seconds)
                                 // An event occurs when a perceptions primitive (see Deliverable D1.3) returns a value 

   int    count;                 // the number of events that actually occur in any give simulation interval
                                 // this is determined by sampling a Poisson distribution having a parameter value lambda


   /* thread parameters: they are pointers so that they refer to the original variables in cognitiveControl */
      
   bool                            *verbose;
   bool                            *cycle;
   double                          *cyclePeriod;
   double                          *eventRate;

   BufferedPort<VectorOf<int>>     *checkMutualGazePortIn; 
   BufferedPort<VectorOf<double>>  *getArmAnglePortIn;
   BufferedPort<VectorOf<double>>  *getBodyPortIn;
   BufferedPort<VectorOf<double>>  *getBodyPosePortIn; 
   BufferedPort<VectorOf<double>>  *getEyeGazePortIn;  
   BufferedPort<VectorOf<int>>     *getEyeGazePortOut; 
   BufferedPort<VectorOf<double>>  *getEyesPortIn; 
   BufferedPort<VectorOf<double>>  *getFacesPortIn; 
   BufferedPort<VectorOf<double>>  *getGripLocationPortIn;    
   BufferedPort<VectorOf<double>>  *getGripLocationPortOut; 
   BufferedPort<VectorOf<double>>  *getHandsPortIn;
   BufferedPort<VectorOf<double>>  *getHeadPortIn; 
   BufferedPort<VectorOf<double>>  *getHeadGazePortIn;  
   BufferedPort<VectorOf<double>>  *getHeadGazePortOut; 
   BufferedPort<VectorOf<double>>  *getObjectsPortIn;
   BufferedPort<VectorOf<double>>  *getObjectsPortOut; 
   BufferedPort<VectorOf<double>>  *getObjectTableDistancePortIn;
   BufferedPort<VectorOf<double>>  *getObjectTableDistancePortOut; 
   BufferedPort<VectorOf<double>>  *getSoundDirectionPortIn;  
   BufferedPort<VectorOf<double>>  *getSoundDirectionPortOut; 
   BufferedPort<VectorOf<int>>     *identifyFacePortIn;
   BufferedPort<VectorOf<double>>  *identifyFacePortOut;
   BufferedPort<VectorOf<int>>     *identifyFaceExpressionPortIn;    
   BufferedPort<VectorOf<double>>  *identifyFaceExpressionPortOut;  
   BufferedPort<VectorOf<int>>     *identifyObjectPortIn; 
   BufferedPort<VectorOf<double>>  *identifyObjectPortOut;
   BufferedPort<VectorOf<int>>     *identifyTrajectoryPortIn;  
   BufferedPort<VectorOf<double>>  *identifyTrajectoryPortOut;  
   BufferedPort<VectorOf<int>>     *identifyVoicePortIn; 
   BufferedPort<Bottle>            *recognizeSpeechPortIn; 
   BufferedPort<VectorOf<double>>  *trackFacePortIn;     
   BufferedPort<VectorOf<double>>  *trackFacePortOut;
   BufferedPort<VectorOf<double>>  *trackHandPortIn; 
   BufferedPort<VectorOf<double>>  *trackHandPortOut;
   BufferedPort<VectorOf<double>>  *trackObjectPortIn;   
   BufferedPort<VectorOf<double>>  *trackObjectPortOut;  

   BufferedPort<VectorOf<double>>  *getChildBehaviourPortIn;   
   BufferedPort<VectorOf<double>>  *getChildMotivationPortIn; 
   BufferedPort<VectorOf<double>>  *getChildPerformancePortIn; 
  
   BufferedPort<VectorOf<int>>     *gripPortIn;
   BufferedPort<VectorOf<double>>  *moveHandPortIn;
   BufferedPort<VectorOf<double>>  *moveHeadPortIn;
   BufferedPort<VectorOf<int>>     *moveSequencePortIn;
   BufferedPort<VectorOf<double>>  *moveTorsoPortIn;
   BufferedPort<Bottle>            *sayPortIn;
   BufferedPort<VectorOf<int>>     *enableRobotPortIn;
   BufferedPort<VectorOf<int>>     *getInterventionStatePortOut;

public:

   /* class methods */

   CognitiveControlThread(
                          bool                            *verbose,
                          bool                            *cycle,
                          double                          *cyclePeriod,
                          double                          *eventRate,
                          BufferedPort<VectorOf<int>>     *checkMutualGazeIn,
                          BufferedPort<VectorOf<double>>  *getArmAngleIn,
                          BufferedPort<VectorOf<double>>  *getBodyIn,
                          BufferedPort<VectorOf<double>>  *getBodyPoseIn, 
                          BufferedPort<VectorOf<double>>  *getEyeGazeIn, 
                          BufferedPort<VectorOf<int>>     *getEyeGazeOut,
                          BufferedPort<VectorOf<double>>  *getEyesIn,
                          BufferedPort<VectorOf<double>>  *getFacesIn, 
                          BufferedPort<VectorOf<double>>  *getGripLocationIn,   
                          BufferedPort<VectorOf<double>>  *getGripLocationOut, 
                          BufferedPort<VectorOf<double>>  *getHandsIn,
                          BufferedPort<VectorOf<double>>  *getHeadIn,
                          BufferedPort<VectorOf<double>>  *getHeadGazeIn, 
                          BufferedPort<VectorOf<double>>  *getHeadGazeOut, 
                          BufferedPort<VectorOf<double>>  *getObjectsIn,
                          BufferedPort<VectorOf<double>>  *getObjectsOut, 
                          BufferedPort<VectorOf<double>>  *getObjectTableDistanceIn, 
                          BufferedPort<VectorOf<double>>  *getObjectTableDistanceOut, 
                          BufferedPort<VectorOf<double>>  *getSoundDirectionIn,
                          BufferedPort<VectorOf<double>>  *getSoundDirectionOut,  
                          BufferedPort<VectorOf<int>>     *identifyFaceIn,
                          BufferedPort<VectorOf<double>>  *identifyFaceOut,  
                          BufferedPort<VectorOf<int>>     *identifyFaceExpressionIn,  
                          BufferedPort<VectorOf<double>>  *identifyFaceExpressionOut,  
                          BufferedPort<VectorOf<int>>     *identifyObjectIn,
                          BufferedPort<VectorOf<double>>  *identifyObjectOut, 
                          BufferedPort<VectorOf<int>>     *identifyTrajectoryIn, 
                          BufferedPort<VectorOf<double>>  *identifyTrajectoryOut,  
                          BufferedPort<VectorOf<int>>     *identifyVoiceIn,
                          BufferedPort<Bottle>            *recognizeSpeechIn, 
                          BufferedPort<VectorOf<double>>  *trackFaceIn,
                          BufferedPort<VectorOf<double>>  *trackFaceOut, 
                          BufferedPort<VectorOf<double>>  *trackHandIn,
                          BufferedPort<VectorOf<double>>  *trackHandOut,  
                          BufferedPort<VectorOf<double>>  *trackObjectIn,  
                          BufferedPort<VectorOf<double>>  *trackObjectOut,
                          BufferedPort<VectorOf<double>>  *getChildBehaviourIn,
                          BufferedPort<VectorOf<double>>  *getChildMotivationIn,
                          BufferedPort<VectorOf<double>>  *getChildPerformanceIn,
                          BufferedPort<VectorOf<int>>     *gripIn,
                          BufferedPort<VectorOf<double>>  *moveHandIn,
                          BufferedPort<VectorOf<double>>  *moveHeadIn,
                          BufferedPort<VectorOf<int>>     *moveSequenceIn,
                          BufferedPort<VectorOf<double>>  *moveTorsoIn,
                          BufferedPort<Bottle>            *sayIn,
                          BufferedPort<VectorOf<int>>     *enableRobotIn,
                          BufferedPort<VectorOf<int>>     *getInterventionStateOut
                         );
   bool threadInit();     
   void threadRelease();
   void run(); 
   int samplePoisson(double lambda);
};


class CognitiveControl:public RFModule {

   /* simulation control parameters */

   string verboseParameter; 
   bool   verbose;
   string cycleParameter;
   bool   cycle;
   double cyclePeriod;
   double eventRate;

   /* module functional parameters */

   string moduleName; 
   string handlerPortName;
   string checkMutualGazeInputPortName;
   string getArmAngleInputPortName;
   string getBodyInputPortName;
   string getBodyPoseInputPortName;
   string getEyeGazeOutputPortName;
   string getEyeGazeInputPortName;
   string getEyesInputPortName;
   string getFacesInputPortName;
   string getGripLocationOutputPortName;
   string getGripLocationInputPortName;
   string getHandsInputPortName;
   string getHeadInputPortName;
   string getHeadGazeOutputPortName;
   string getHeadGazeInputPortName;
   string getObjectsInputPortName;
   string getObjectsOutputPortName;
   string getObjectTableDistanceOutputPortName;
   string getObjectTableDistanceInputPortName;
   string getSoundDirectionOutputPortName;
   string getSoundDirectionInputPortName;
   string identifyFaceOutputPortName;
   string identifyFaceInputPortName;
   string identifyFaceExpressionOutputPortName;
   string identifyFaceExpressionInputPortName;
   string identifyObjectOutputPortName;
   string identifyObjectInputPortName;
   string identifyTrajectoryOutputPortName;
   string identifyTrajectoryInputPortName;
   string identifyVoiceInputPortName;
   string recognizeSpeechInputPortName;
   string trackFaceOutputPortName;
   string trackFaceInputPortName;
   string trackHandOutputPortName;
   string trackHandInputPortName;
   string trackObjectOutputPortName;
   string trackObjectInputPortName;
   string getChildBehaviourInputPortName;
   string getChildMotivationInputPortName;
   string getChildPerformanceInputPortName;
   string gripInputPortName;
   string moveHandInputPortName;
   string moveHeadInputPortName;
   string moveSequenceInputPortName;
   string moveTorsoInputPortName;
   string sayInputPortName;
   string enableRobotInputPortName; 
   string getInterventionStateOutputPortName;

   /* class variables */

   Port                            handlerPort;      // a port to handle interactive messages (also uses bottles)

   BufferedPort<VectorOf<int>>     checkMutualGazeIn;
   BufferedPort<VectorOf<double>>  getArmAngleIn;
   BufferedPort<VectorOf<double>>  getBodyIn;
   BufferedPort<VectorOf<double>>  getBodyPoseIn; 
   BufferedPort<VectorOf<double>>  getEyeGazeIn;
   BufferedPort<VectorOf<int>>     getEyeGazeOut;
   BufferedPort<VectorOf<double>>  getEyesIn;
   BufferedPort<VectorOf<double>>  getFacesIn; 
   BufferedPort<VectorOf<double>>  getGripLocationIn; 
   BufferedPort<VectorOf<double>>  getGripLocationOut; 
   BufferedPort<VectorOf<double>>  getHandsIn;
   BufferedPort<VectorOf<double>>  getHeadIn;
   BufferedPort<VectorOf<double>>  getHeadGazeIn; 
   BufferedPort<VectorOf<double>>  getHeadGazeOut; 
   BufferedPort<VectorOf<double>>  getObjectsIn;
   BufferedPort<VectorOf<double>>  getObjectsOut; 
   BufferedPort<VectorOf<double>>  getObjectTableDistanceIn; 
   BufferedPort<VectorOf<double>>  getObjectTableDistanceOut; 
   BufferedPort<VectorOf<double>>  getSoundDirectionIn;
   BufferedPort<VectorOf<double>>  getSoundDirectionOut;  
   BufferedPort<VectorOf<int>>     identifyFaceIn;
   BufferedPort<VectorOf<double>>  identifyFaceOut;  
   BufferedPort<VectorOf<int>>     identifyFaceExpressionIn;  
   BufferedPort<VectorOf<double>>  identifyFaceExpressionOut;  
   BufferedPort<VectorOf<int>>     identifyObjectIn;
   BufferedPort<VectorOf<double>>  identifyObjectOut; 
   BufferedPort<VectorOf<int>>     identifyTrajectoryIn;  
   BufferedPort<VectorOf<double>>  identifyTrajectoryOut;  
   BufferedPort<VectorOf<int>>     identifyVoiceIn;
   BufferedPort<Bottle>            recognizeSpeechIn; 
   BufferedPort<VectorOf<double>>  trackFaceIn;
   BufferedPort<VectorOf<double>>  trackFaceOut;  
   BufferedPort<VectorOf<double>>  trackHandIn;
   BufferedPort<VectorOf<double>>  trackHandOut;  
   BufferedPort<VectorOf<double>>  trackObjectIn;  
   BufferedPort<VectorOf<double>>  trackObjectOut; 
   BufferedPort<VectorOf<double>>  getChildBehaviourIn;
   BufferedPort<VectorOf<double>>  getChildMotivationIn;
   BufferedPort<VectorOf<double>>  getChildPerformanceIn;
   BufferedPort<VectorOf<int>>     gripIn;
   BufferedPort<VectorOf<double>>  moveHandIn;
   BufferedPort<VectorOf<double>>  moveHeadIn;
   BufferedPort<VectorOf<int>>     moveSequenceIn;
   BufferedPort<VectorOf<double>>  moveTorsoIn;
   BufferedPort<Bottle>            sayIn;
   BufferedPort<VectorOf<int>>     enableRobotIn; 
   BufferedPort<VectorOf<int>>     getInterventionStateOut;

   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   CognitiveControlThread *cognitiveControlThread;

public:
   
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
};
 