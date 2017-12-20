/** @file sensoryInterpretation.h  Interface file for the sensory interpretation subsystem in the DREAM system architecture
 *
 * Version 1.0
 * 
 * 23/11/2014
 *
 *
 * \defgroup DREAM_sensoryInterpretation sensoryInterpretation
 * 
 * Interface file for the sensory interpretation subsystem in the DREAM system architecture
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
 * The current component is the implementation of the sensoryInterpretation placeholder component.
 * It provides stub functionality for all the 25 sensoryInterpretation perception primitives (see D1.3 and D3.1)
 * 
 * - checkMutualGaze()  
 * - getArmAngle(left_azimuth, left_elevation, right_azimuth, right_elevation)  
 * - getBody(x, y, z) 
 * - getBodyPose(<joint_i>)  
 * - getEyeGaze(eye, x, y, z) 
 * - getEyes(eyeL_x, eyeL_y, eyeL_z, eyeR_x, eyeR_y, eyeR_z)  
 * - getFaces(<x, y, z>) 
 * - getGripLocation(object_x, object_y, object_z, grip_x, grip_y, grip_z) 
 * - getHands(<x, y, z>) 
 * - getHead(x, y, z) 
 * - getHeadGaze(x, y, z) 
 * - getHeadGaze(p_x1, p_y1, p_z1, p_x2, p_y2, p_z2, p_x3, p_y3, p_z3, x, y, z)  
 * - getObjects(<x, y, z>) 
 * - getObjects(centre_x, centre_y, centre_z, radius, <x, y, z>)  
 * - getObjectTableDistance(object_x, object_y, object_z, vertical_distance) 
 * - getSoundDirection(threshold, azimuth, elevation)  
 * - identifyFace(x, y, z, face_id)  
 * - identifyFaceExpression(x, y, z, expression_id) 
 * - identifyObject(x, y, z, object_id)  
 * - identifyTrajectory(<x, y, z, t>, trajectory_descriptor)  
 * - identifyVoice(voice_descriptor)  
 * - recognizeSpeech(text) 
 * - trackFace(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z)  
 * - trackHand(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z) 
 * - trackObject(seed_x, seed_y, seed_z, time_interval, projected_x, projected_y, projected_z) 
 *
 * This means that it reads any input parameters from the associated input port (where necessary) and
 * writes simulated results (random values) to the associated output port.
 * 
 * This placeholder component does nothing except send and receive simulated data on each of the ports 
 * defined in the sub-system specification. The purpose of the placeholder is merely to provide 
 * a working example of the implementation and use of these ports using the data communication protocols
 * specified in Deliverable D3.1. These specifications are replicated in the section Ports Created below. 
 * 
 * The component operates as a discrete event simulation.
 * An event occurs when a perception primitive returns a value. This is modelled by a Poisson distribution, 
 * with every primitive having its own event rate.  This provides for more realistic simulation of 
 * the behaviour of the sub-system for which it is a placeholder. It also has the advantage that it is
 * possible to set all but one event rates to zero for a certain period of time, and to cycle through
 * each primitive as the simulation proceeds. This makes it easier to check the operation of the component
 * expecially when all three placeholder components are working together.
 *
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
 * - \c from \c sensoryInterpretation.ini
 *     - specifies the configuration file
 *
 * - \c context \c components/sensoryInterpretation/config
 *     - specifies the sub-path from \c $DREAM_ROOT/release/ 
 *   to the configuration file
 *
 * - \c name \c sensoryInterpretation
 *     - specifies the name of the module (used to form the stem of module port names)  
 *
 *
 * <b>Configuration File Parameters </b>
 *
 * The following key-value pairs can be specified as parameters in the configuration file 
 * (they can also be specified as command-line parameters if you so wish). 
 * The value part can be changed to suit your needs; the default values are shown below. 
 *
 * Parameters for controlling the simulation \n
 * 
 * Key | Value
 * :-- | :----
 * verbose                            | on
 * cycle                              | on
 * cycle_period                       | 0.5
 * event_rate                         | 20.0
 *
 * Parameters associated with the functions being simulated (i.e. the input and output port names) \n
 *
 * Note that all these default values for port names will be prefixed by \c /sensoryInterpretation 
 * or whatever else is specifed by the \c name parameter
 * 
 * Key | Value
 * :-- | :----
 * checkMutualGazeOutputPort         | /checkMutualGaze:o
 * getArmAngleOutputPort             | /getArmAngle:o
 * getBodyOutputPort                 | /getBody:o
 * getBodyPoseOutputPort             | /getBodyPose:o
 * getEyeGazeInputPort               | /getEyeGaze:i
 * getEyeGazeOutputPort              | /getEyeGaze:o
 * getEyesOutputPort                 | /getEyes:o
 * getFacesOutputPort                | /getFaces:o
 * getGripLocationInputPort          | /getGripLocation:i
 * getGripLocationOutputPort         | /getGripLocation:o
 * getHandsOutputPort                | /getHands:o
 * getHeadOutputPort                 | /getHead:o
 * getHeadGazeInputPort              | /getHeadGaze:i
 * getHeadGazeOutputPort             | /getHeadGaze:o
 * getObjectsOutputPort              | /getObjects:o
 * getObjectsInputPort               | /getObjects:i
 * getObjectTableDistanceInputPort   | /getObjectTableDistance:i
 * getObjectTableDistanceOutputPort  | /getObjectTableDistance:o
 * getSoundDirectionInputPort        | /getSoundDirection:i
 * getSoundDirectionOutputPort       | /getSoundDirection:o
 * identifyFaceInputPort             | /identifyFace:i
 * identifyFaceOutputPort            | /identifyFace:o
 * identifyFaceExpressionInputPort   | /identifyFaceExpression:i
 * identifyFaceExpressionOutputPort  | /identifyFaceExpression:o
 * identifyObjectInputPort           | /identifyObject:i
 * identifyObjectOutputPort          | /identifyObject:o
 * identifyTrajectoryInputPort       | /identifyTrajectory:i
 * identifyTrajectoryOutputPort      | /identifyTrajectory:o
 * identifyVoiceOutputPort           | /identifyVoice:o
 * recognizeSpeechOutputPort         | /recognizeSpeech:o
 * trackFaceInputPort                | /trackFace:i
 * trackFaceOutputPort               | /trackFace:o
 * trackHandInputPort                | /trackHand:i
 * trackHandOutputPort               | /trackHand:o
 * trackObjectInputPort              | /trackObject:i
 * trackObjectOutputPort             | /trackObject:o
 * 
 * \section portsa_sec Ports Accessed
 * 
 * - None
 *                      
 * \section portsc_sec Ports Created
 *
 * <b>Input ports</b>
 *
 *  - \c /sensoryInterpretation
 *
 * This port is used to change the parameters of the module at run time or stop the module. \n
 * The following commands are available
 * 
 *     help
 *     verbose         on      ... echo all data received and sent to the terminal
 *     verbose         off     ... run in quiet mode and do not echo  data received and sent to the terminal
 *     cycle           on      ... set event rate to zero for all but one classification primitive
 *     cycle           off     ... assign event rate specified to all three classification primitives
 *     cycle_period    <n>     ... period in seconds that an event remain non-zero during cycle mode (where <n> is an real number)
 *     event_rate      <n>     ... the mean number of times per second that a primitive is invoked (where <n> is an real number)
 *
 * Note that the name of this port mirrors whatever is provided by the \c --name parameter value
 * The port is attached to the terminal so that you can type in commands and receive replies.
 * The port can be used by other modules but also interactively by a user through the yarp rpc directive, e.g.
 * \c yarp \c rpc \c /sensoryInterpretation
 * This opens a connection from a terminal to the port and allows the user to then type in commands and receive replies.
 * 
 *     /sensoryInterpretation/getEyeGaze:i
 *     /sensoryInterpretation/getGripLocation:i
 *     /sensoryInterpretation/getHeadGaze:i
 *     /sensoryInterpretation/getObjects:i
 *     /sensoryInterpretation/getObjectTableDistance:i
 *     /sensoryInterpretation/getSoundDirection:i
 *     /sensoryInterpretation/identifyFace:i
 *     /sensoryInterpretation/identifyFaceExpression:i
 *     /sensoryInterpretation/identifyObject:i
 *     /sensoryInterpretation/identifyTrajectory:i
 *     /sensoryInterpretation/trackFace:i
 *     /sensoryInterpretation/trackHand:i
 *     /sensoryInterpretation/trackObject:i
 * 
 * <b>Output ports</b>
 *
 *     /sensoryInterpretation   (see above)
 *     /sensoryInterpretation/checkMutualGaze:o
 *     /sensoryInterpretation/getArmAngle:o
 *     /sensoryInterpretation/getBody:o
 *     /sensoryInterpretation/getBodyPose:o
 *     /sensoryInterpretation/getEyeGaze:o
 *     /sensoryInterpretation/getEyes:o
 *     /sensoryInterpretation/getFaces:o
 *     /sensoryInterpretation/getGripLocation:o
 *     /sensoryInterpretation/getHands:o
 *     /sensoryInterpretation/getHead:o
 *     /sensoryInterpretation/getHeadGaze:o
 *     /sensoryInterpretation/getObjects:o
 *     /sensoryInterpretation/getObjectTableDistance:o
 *     /sensoryInterpretation/getSoundDirection:o
 *     /sensoryInterpretation/identifyFace:o
 *     /sensoryInterpretation/identifyFaceExpression:o
 *     /sensoryInterpretation/identifyObject:o
 *     /sensoryInterpretation/identifyTrajectory:o
 *     /sensoryInterpretation/identifyVoice:o
 *     /sensoryInterpretation/recognizeSpeech:o
 *     /sensoryInterpretation/trackFace:o
 *     /sensoryInterpretation/trackHand:o
 *     /sensoryInterpretation/trackObject:o
 *
 *
 * <b>Port types </b>
 *
 * The functional specification only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *
 *     BufferedPort<VectorOf<int>>       checkMutualGazeOut; 
 *     BufferedPort<VectorOf<double>>    getArmAngleOut;
 *     BufferedPort<VectorOf<double>>    getBodyOut;
 *     BufferedPort<VectorOf<double>>    getBodyPoseOut;
 *     BufferedPort<VectorOf<int>>       getEyeGazeIn;
 *     BufferedPort<VectorOf<double>>    getEyeGazeOut;
 *     BufferedPort<VectorOf<double>>    getEyesOut;
 *     BufferedPort<VectorOf<double>>    getFacesOut;
 *     BufferedPort<VectorOf<double>>    getGripLocationIn;
 *     BufferedPort<VectorOf<double>>    getGripLocationOut;
 *     BufferedPort<VectorOf<double>>    getHandsOut;
 *     BufferedPort<VectorOf<double>>    getHeadOut;
 *     BufferedPort<VectorOf<double>>    getHeadGazeIn;
 *     BufferedPort<VectorOf<double>>    getHeadGazeOut;
 *     BufferedPort<VectorOf<double>>    getObjectsIn;
 *     BufferedPort<VectorOf<double>>    getObjectsOut;
 *     BufferedPort<VectorOf<double>>    getObjectTableDistanceIn;
 *     BufferedPort<VectorOf<double>>    getObjectTableDistanceOut;
 *     BufferedPort<VectorOf<double>>    getSoundDirectionIn;
 *     BufferedPort<VectorOf<double>>    getSoundDirectionOut;
 *     BufferedPort<VectorOf<double>>    identifyFaceIn;
 *     BufferedPort<VectorOf<int>>       identifyFaceOut;
 *     BufferedPort<VectorOf<double>>    identifyFaceExpressionIn;
 *     BufferedPort<VectorOf<int>>       identifyFaceExpressionOut;
 *     BufferedPort<VectorOf<double>>    identifyObjectIn;
 *     BufferedPort<VectorOf<int>>       identifyObjectOut;
 *     BufferedPort<VectorOf<double>>    identifyTrajectoryIn;
 *     BufferedPort<VectorOf<int>>       identifyTrajectoryOut;
 *     BufferedPort<VectorOf<int>>       identifyVoiceOut;
 *     BufferedPort<Bottle>              recognizeSpeechOut;
 *     BufferedPort<VectorOf<double>>    trackFaceIn;
 *     BufferedPort<VectorOf<double>>    trackFaceOut;
 *     BufferedPort<VectorOf<double>>    trackHandIn;
 *     BufferedPort<VectorOf<double>>    trackHandOut;
 *     BufferedPort<VectorOf<double>>    trackObjectIn;
 *     BufferedPort<VectorOf<double>>    trackObjectOut;
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
 * \c sensoryInterpretation.ini   
 *
 *
 * \section example_sec Example Instantiation of the Component
 * 
 * <tt>sensoryInterpretation --name sensoryInterpretation --context components/sensoryInterpretation/config --from sensoryInterpretation.ini </tt>
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
 * 23/11/14  First version validated (David Vernon)
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

#define NUMBER_OF_PRIMITIVES 25   // we allow for different event rates for each perception primitive being simulated
                                  // (see Deliverable D1.3)
                                  // this makes it easier to check that the simulation is work by setting the event rate 
                                  // of all but one of the primitives to zero, periodically changing the selected primitive
                                  // every 10 seconds or so, for example

#define NUMBER_OF_JOINTS 6        // the number of of joint angles describing the child’s current body pose. 
                                  // The joint angles correspond to a canonical representation of a body skeleton, 
                                  // yet to be defined (see D1.3, getBodyPose() perception primitive)

#define NUMBER_OF_FACES 2         // the number of faces that can be returned by the getFaces() primitive.
                                  // The primitive determines the locations where faces are visible 
                                  // in the therapy environment (viewed from any of the therapy cameras). 
                                  // Each location is specified by a triple of Cartesian coordinates x, y, and z. 

#define NUMBER_OF_EXPRESSIONS 4   // the number of face expressions that can be returned by the getFacesExpression() primitive.
                                  // The primitive determines the locations where faces are visible 
                                  // in the therapy environment (viewed from any of the therapy cameras). 
                                  // Each location is specified by a triple of Cartesian coordinates x, y, and z. 

#define NUMBER_OF_HANDS 2         // the number of hands that can be returned by the getHands() primitive.
                                  // The primitive determines the locations where hands are visible 
                                  // in the therapy environment (viewed from any of the therapy cameras). 
                                  // Each location is specified by a triple of Cartesian coordinates x, y, and z. 

#define NUMBER_OF_VOICES 2        // the cardinality of the voice class that can be returned by the identifyVoice() primitive.

#define NUMBER_OF_OBJECTS 2       // the number of objects that can be returned by the getObjects() primitive.
                                  // The primitive determines the locations where objects are visible 
                                  // in the therapy environment (viewed from any of the therapy cameras). 
                                  // Each location is specified by a triple of Cartesian coordinates x, y, and z.

#define NUMBER_OF_TRAJECTORIES 4  // the cardinality of the trajectory class that can be returned by the identifyTrajectory() primitive.
                                  // the trajectory defined by a vector of 4-tuples, each 4-tuple defining a 3-D location and a time-stamp.   
                                  // The possible classes are determined during the set up phase. Typically, they will correspond to 
                                  // a designated set of hand gestures to be made by the child (e.g. a wave).

#define TUPLES_IN_A_TRAJECTORY 20 // the number of 4-tuples in each trajectory descriptor
                   

class SensoryInterpretationThread : public Thread {

private:

   /* class variables */
    
   bool   debug; 
   double start_time;             // the time the current event loop begain
   double current_time;           // self-evident
   double cumulative_time;        // the total time passed so far since we last changed the primitive with a non-zero event rate when debugging
   double event_rate[NUMBER_OF_PRIMITIVES];
   double body_angles[NUMBER_OF_JOINTS];
   double elapsed_time;

   int    current_primitive;     // counter to keep track of which primitive's event rate is being used
  
   int    target_primitive;      // counter to keep track of which primitive to should be non-zero when debugging
  
   double lambda;                // the mean number of events that occur in any one similation interval 
                                 // (i.e. the time it takes to execute the main while(isStopping() != true) loop
                                 // this time is computed using yarp::os::Time::now()
                                 // lambda is the arrival rate (in events per second) x the elapsed_time (in seconds)
                                 // An event occurs when a perceptions primitive (see Deliverable D1.3) returns a value 

   int    count;                 // the number of events that actually occur in any give simulation interval
                                 // this is determined by sampling a Poisson distribution having a parameter value lambda


   /* thread parameters: they are pointers so that they refer to the original variables in sensoryInterpretation */
      
   bool                            *verbose;
   bool                            *cycle;
   double                          *cyclePeriod;
   double                          *eventRate;

   BufferedPort<VectorOf<int>>     *checkMutualGazePortOut; 
   BufferedPort<VectorOf<double>>  *getArmAnglePortOut;
   BufferedPort<VectorOf<double>>  *getBodyPortOut;
   BufferedPort<VectorOf<double>>  *getBodyPosePortOut; 
   BufferedPort<VectorOf<int>>     *getEyeGazePortIn; 
   BufferedPort<VectorOf<double>>  *getEyeGazePortOut; 
   BufferedPort<VectorOf<double>>  *getEyesPortOut; 
   BufferedPort<VectorOf<double>>  *getFacesPortOut; 
   BufferedPort<VectorOf<double>>  *getGripLocationPortIn; 
   BufferedPort<VectorOf<double>>  *getGripLocationPortOut; 
   BufferedPort<VectorOf<double>>  *getHandsPortOut;
   BufferedPort<VectorOf<double>>  *getHeadPortOut; 
   BufferedPort<VectorOf<double>>  *getHeadGazePortIn; 
   BufferedPort<VectorOf<double>>  *getHeadGazePortOut; 
   BufferedPort<VectorOf<double>>  *getObjectsPortIn; 
   BufferedPort<VectorOf<double>>  *getObjectsPortOut; 
   BufferedPort<VectorOf<double>>  *getObjectTableDistancePortIn; 
   BufferedPort<VectorOf<double>>  *getObjectTableDistancePortOut; 
   BufferedPort<VectorOf<double>>  *getSoundDirectionPortIn; 
   BufferedPort<VectorOf<double>>  *getSoundDirectionPortOut;  
   BufferedPort<VectorOf<double>>  *identifyFacePortIn;
   BufferedPort<VectorOf<int>>     *identifyFacePortOut;  
   BufferedPort<VectorOf<double>>  *identifyFaceExpressionPortIn;  
   BufferedPort<VectorOf<int>>     *identifyFaceExpressionPortOut;  
   BufferedPort<VectorOf<double>>  *identifyObjectPortIn;
   BufferedPort<VectorOf<int>>     *identifyObjectPortOut; 
   BufferedPort<VectorOf<double>>  *identifyTrajectoryPortIn;  
   BufferedPort<VectorOf<int>>     *identifyTrajectoryPortOut;  
   BufferedPort<VectorOf<int>>     *identifyVoicePortOut; 
   BufferedPort<Bottle>            *recognizeSpeechPortOut; 
   BufferedPort<VectorOf<double>>  *trackFacePortIn;
   BufferedPort<VectorOf<double>>  *trackFacePortOut;  
   BufferedPort<VectorOf<double>>  *trackHandPortIn;
   BufferedPort<VectorOf<double>>  *trackHandPortOut;  
   BufferedPort<VectorOf<double>>  *trackObjectPortIn;  
   BufferedPort<VectorOf<double>>  *trackObjectPortOut;  
  
public:

   /* class methods */

   SensoryInterpretationThread(
                               bool                            *verbose,
                               bool                            *cycle,
                               double                          *cyclePeriod,
                               double                          *eventRate,
                               BufferedPort<VectorOf<int>>     *checkMutualGazeOut,
                               BufferedPort<VectorOf<double>>  *getArmAngleOut,
                               BufferedPort<VectorOf<double>>  *getBodyOut,
                               BufferedPort<VectorOf<double>>  *getBodyPoseOut, 
                               BufferedPort<VectorOf<int>>     *getEyeGazeIn,
                               BufferedPort<VectorOf<double>>  *getEyeGazeOut, 
                               BufferedPort<VectorOf<double>>  *getEyesOut,
                               BufferedPort<VectorOf<double>>  *getFacesOut, 
                               BufferedPort<VectorOf<double>>  *getGripLocationIn, 
                               BufferedPort<VectorOf<double>>  *getGripLocationOut, 
                               BufferedPort<VectorOf<double>>  *getHandsOut,
                               BufferedPort<VectorOf<double>>  *getHeadOut,
                               BufferedPort<VectorOf<double>>  *getHeadGazeIn, 
                               BufferedPort<VectorOf<double>>  *getHeadGazeOut, 
                               BufferedPort<VectorOf<double>>  *getObjectsIn,
                               BufferedPort<VectorOf<double>>  *getObjectsOut, 
                               BufferedPort<VectorOf<double>>  *getObjectTableDistanceIn, 
                               BufferedPort<VectorOf<double>>  *getObjectTableDistanceOut, 
                               BufferedPort<VectorOf<double>>  *getSoundDirectionIn,
                               BufferedPort<VectorOf<double>>  *getSoundDirectionOut,  
                               BufferedPort<VectorOf<double>>  *identifyFaceIn,
                               BufferedPort<VectorOf<int>>     *identifyFaceOut,  
                               BufferedPort<VectorOf<double>>  *identifyFaceExpressionIn,  
                               BufferedPort<VectorOf<int>>     *identifyFaceExpressionOut,  
                               BufferedPort<VectorOf<double>>  *identifyObjectIn,
                               BufferedPort<VectorOf<int>>     *identifyObjectOut, 
                               BufferedPort<VectorOf<double>>  *identifyTrajectoryIn,  
                               BufferedPort<VectorOf<int>>     *identifyTrajectoryOut,  
                               BufferedPort<VectorOf<int>>     *identifyVoiceOut,
                               BufferedPort<Bottle>            *recognizeSpeechOut, 
                               BufferedPort<VectorOf<double>>  *trackFaceIn,
                               BufferedPort<VectorOf<double>>  *trackFaceOut, 
                               BufferedPort<VectorOf<double>>  *trackHandIn,
                               BufferedPort<VectorOf<double>>  *trackHandOut,  
                               BufferedPort<VectorOf<double>>  *trackObjectIn,  
                               BufferedPort<VectorOf<double>>  *trackObjectOut
                              );
   bool threadInit();     
   void threadRelease();
   void run(); 
   int samplePoisson(double lambda);
};


class SensoryInterpretation:public RFModule {

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
   string checkMutualGazeOutputPortName;
   string getArmAngleOutputPortName;
   string getBodyOutputPortName;
   string getBodyPoseOutputPortName;
   string getEyeGazeInputPortName;
   string getEyeGazeOutputPortName;
   string getEyesOutputPortName;
   string getFacesOutputPortName;
   string getGripLocationInputPortName;
   string getGripLocationOutputPortName;
   string getHandsOutputPortName;
   string getHeadOutputPortName;
   string getHeadGazeInputPortName;
   string getHeadGazeOutputPortName;
   string getObjectsOutputPortName;
   string getObjectsInputPortName;
   string getObjectTableDistanceInputPortName;
   string getObjectTableDistanceOutputPortName;
   string getSoundDirectionInputPortName;
   string getSoundDirectionOutputPortName;
   string identifyFaceInputPortName;
   string identifyFaceOutputPortName;
   string identifyFaceExpressionInputPortName;
   string identifyFaceExpressionOutputPortName;
   string identifyObjectInputPortName;
   string identifyObjectOutputPortName;
   string identifyTrajectoryInputPortName;
   string identifyTrajectoryOutputPortName;
   string identifyVoiceOutputPortName;
   string recognizeSpeechOutputPortName;
   string trackFaceInputPortName;
   string trackFaceOutputPortName;
   string trackHandInputPortName;
   string trackHandOutputPortName;
   string trackObjectInputPortName;
   string trackObjectOutputPortName;
  
   /* class variables */

   BufferedPort<ImageOf<PixelRgb> > imageIn;       // example image input port
   BufferedPort<VectorOf<int> >     thresholdIn;   // example vector input port 
   BufferedPort<ImageOf<PixelRgb> > imageOut;      // example image output port
   BufferedPort<Bottle>             statisticsOut; // example bottle output port
   Port handlerPort;                               // a port to handle interactive messages (also uses bottles)

   BufferedPort<VectorOf<int>>     checkMutualGazeOut;
   BufferedPort<VectorOf<double>>  getArmAngleOut;
   BufferedPort<VectorOf<double>>  getBodyOut;
   BufferedPort<VectorOf<double>>  getBodyPoseOut; 
   BufferedPort<VectorOf<int>>     getEyeGazeIn;
   BufferedPort<VectorOf<double>>  getEyeGazeOut; 
   BufferedPort<VectorOf<double>>  getEyesOut;
   BufferedPort<VectorOf<double>>  getFacesOut; 
   BufferedPort<VectorOf<double>>  getGripLocationIn; 
   BufferedPort<VectorOf<double>>  getGripLocationOut; 
   BufferedPort<VectorOf<double>>  getHandsOut;
   BufferedPort<VectorOf<double>>  getHeadOut;
   BufferedPort<VectorOf<double>>  getHeadGazeIn; 
   BufferedPort<VectorOf<double>>  getHeadGazeOut; 
   BufferedPort<VectorOf<double>>  getObjectsIn;
   BufferedPort<VectorOf<double>>  getObjectsOut; 
   BufferedPort<VectorOf<double>>  getObjectTableDistanceIn; 
   BufferedPort<VectorOf<double>>  getObjectTableDistanceOut; 
   BufferedPort<VectorOf<double>>  getSoundDirectionIn;
   BufferedPort<VectorOf<double>>  getSoundDirectionOut;  
   BufferedPort<VectorOf<double>>  identifyFaceIn;
   BufferedPort<VectorOf<int>>     identifyFaceOut;  
   BufferedPort<VectorOf<double>>  identifyFaceExpressionIn;  
   BufferedPort<VectorOf<int>>     identifyFaceExpressionOut;  
   BufferedPort<VectorOf<double>>  identifyObjectIn;
   BufferedPort<VectorOf<int   >>  identifyObjectOut; 
   BufferedPort<VectorOf<double>>  identifyTrajectoryIn;  
   BufferedPort<VectorOf<int>>     identifyTrajectoryOut;  
   BufferedPort<VectorOf<int>>     identifyVoiceOut;
   BufferedPort<Bottle>            recognizeSpeechOut; 
   BufferedPort<VectorOf<double>>  trackFaceIn;
   BufferedPort<VectorOf<double>>  trackFaceOut;  
   BufferedPort<VectorOf<double>>  trackHandIn;
   BufferedPort<VectorOf<double>>  trackHandOut;  
   BufferedPort<VectorOf<double>>  trackObjectIn;  
   BufferedPort<VectorOf<double>>  trackObjectOut; 

   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   SensoryInterpretationThread *sensoryInterpretationThread;

public:
   
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
};
 