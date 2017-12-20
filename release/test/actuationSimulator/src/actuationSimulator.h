/** @file actuationSimulator.h  Interface file for the actuator simulator to be used with the Reacive subsystem component
 *
 * Version 1.0
 * 
 * 14/08/2015
 *
 *
 * \defgroup DREAM_reactiveComponent actuationSimulator
 * 
 * Interface file for the reactive subsystem in the cognitive controller component
 * 
 * \section component_description Component Description
 * 
 * The Reactive subsystem, as described in Deliverable D6.1, comprises three modules:
 *
 * - Falling Reaction, 
 * - Social Reaction, and
 * - Eye Blinking. 
 *  
 * The current component is the implementation of the actuationSimulator placeholder component.
 * The purpose of this module is to provide a preliminary actuation subsystem to send the reactive
 * subsystem outputs to the corresponding robot. 
 *
 * The components that interact with this one have been developed as basic simulators that provide 
 * the expected output. For some of them, such expected output was provided through a GUI (reactiveSystemGUI component).
 * That is the case of the Deliberative subsystem (facial expression) and the Attention subsystem (shift of gaze).
 * Such GUI has been used to simulate the sensory information of the robot as a first step in the
 * implementation of this system. It should be replace by a sensory information simulator of an specific
 * robot, i.e. Nao robot. But for the purpose of validating the Reactive subsystem the GUI is an equally
 * good option.
 * On the other hand, this system uses actual actuators to show its outputs. This implementation has
 * been done in a Nao Robot using different layers of Yarp (naoInterface component), keeping its platform-independent
 * flavor.
 * It also includes an Actuation subsystem simulator (actuationSimulator component)  which receives the outputs of the Reactive subsystem
 * and provides the corresponding action primitives.
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
 * - \c from \c actuationSimulator.ini 
 *     - specifies the configuration file
 *
 * - \c context \c components/actuationSimulator/config
 *     - specifies the sub-path from \c $DREAM_ROOT/release to the configuration file
 *
 * - \c name \c actuationSimulator
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
 * from  |  actuationSimulator.ini
 * 
 * specifies the configuration file
 * 
 * Key | Value
 * :-- | :----
 * context  |  components/actuationSimulator/config
 * 
 * specifies the sub-path from \c $DREAM_ROOT/release/ 
 * to the configuration file
 * 
 * Key | Value
 * :-- | :----
 * name  |  actuationSimulator
 * 
 * specifies the name of the module (used to form the stem of module port names)  
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
 *     /actuationSimulator/getFallingReaction:i
 *     /actuationSimulator/getFallingReactionSpeech:i
 *     /actuationSimulator/getInterruption:i
 *     /actuationSimulator/getEyeBlinking:i 
 *     /actuationSimulator/getSocialReaction:i
 *     /actuationSimulator/getSocialReactionSpeech:i
 *     /actuationSimulator/getSocialFacialExpression:i
 *     /actuationSimulator/motorFeedback:i
 * 
 * <b>Output ports</b>
 * 
 *    /actuationSimulator/say:o
 *    /actuationSimulator/moveSequence:o
 * 
 * <b>Port types </b>
 *
 * The functional specification only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *
 * BufferedPort<Bottle>			      *getEyeBlinkingIn,
 * BufferedPort<VectorOf<int>>        *getFallingReactionIn,
 * BufferedPort<VectorOf<int>>		  *getFallingReactionSpeechIn,
 * BufferedPort<VectorOf<int>>        *getSocialReactionIn,
 * BufferedPort<VectorOf<int>>		  *getSocialReactionSpeechIn,
 * BufferedPort<VectorOf<int>>        *getSocialFacialExpressionIn,
 * BufferedPort<VectorOf<int>>        *getInterruptionIn,
 * BufferedPort<Bottle>			      *sayOut,
 * BufferedPort<Bottle>			      *moveSequenceOut,
 * BufferedPort<Bottle>               *motorFeedbackIn
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
 * \c actuationSimulator.ini   
 *
 *
 * \section example_sec Example Instantiation of the Component
 * 
 * <tt>actuationSimulator --name actuationSimulator --context components/actuationSimulator/config --from actuationSimulator.ini </tt>
 *
 *
 * \author 
 * 
 * Pablo Gómez, Vrije Universiteit Brussel, Belgium.
 * 
 * Copyright (C) 2015 DREAM Consortium
 * 
 */
  

/* 
 * Copyright (C) 2015 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Pablo Gomez, Vrije Universiteit Brussel
 * Email:   pablo.gomez.esteban@vub.ac.be
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
 * XX/07/15  First version validated (Pablo Gomez)
 */  
 
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <map>
#include <sstream>
#include <windows.h>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
 
using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;

#define STRINGLENGTH 132 // used to define a string when post-processing the bottle messages
#define PAUSE 500 // used in SLEEP function 
#define NUMBER_OF_SENTENCES 5 

  
class ActuationSimulatorThread : public Thread {

private:

   /* class variables */
   bool              debug;
      
   /* thread parameters: they are pointers so that they refer to the original variables in eyeBlinking */

   BufferedPort<Bottle>			   *getEyeBlinkingPortIn;
   BufferedPort<VectorOf<int>>     *getFallingReactionPortIn;
   BufferedPort<VectorOf<int>>	   *getFallingReactionSpeechPortIn;
   BufferedPort<VectorOf<int>>     *getSocialReactionPortIn;
   BufferedPort<VectorOf<int>>	   *getSocialReactionSpeechPortIn;
   BufferedPort<VectorOf<int>>     *getSocialFacialExpressionPortIn;
   BufferedPort<VectorOf<int>>     *getInterruptionPortIn;  
   BufferedPort<Bottle>			   *sayPortOut;
   BufferedPort<Bottle>			   *moveSequencePortOut;
   BufferedPort<Bottle>			   *motorFeedbackPortIn;
   bool _isBusy;		//a robot behaviour button has already been pressed
   std::vector<std::string> split(const std::string &s, char delim);
   std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems);
   
   bool ActuationSimulatorThread::actionFinished(int status);
   std::map<std::string, int> _statusRef;
public:

   /* class methods */

   ActuationSimulatorThread(BufferedPort<Bottle>			   *getEyeBlinkingIn,
						    BufferedPort<VectorOf<int>>        *getFallingReactionIn,
						    BufferedPort<VectorOf<int>>		   *getFallingReactionSpeechIn,
						    BufferedPort<VectorOf<int>>        *getSocialReactionIn,
						    BufferedPort<VectorOf<int>>		   *getSocialReactionSpeechIn,
						    BufferedPort<VectorOf<int>>        *getSocialFacialExpressionIn,
							BufferedPort<VectorOf<int>>        *getInterruptionIn,
						    BufferedPort<Bottle>			   *sayOut,
						    BufferedPort<Bottle>			   *moveSequenceOut,
							BufferedPort<Bottle>			   *motorFeedbackIn
							);
   bool threadInit();     
   void threadRelease();
   void run(); 
   
};


class ActuationSimulator:public RFModule {

   /* module parameters */

   string moduleName;

   string getEyeBlinkingInputPortName;
	string getFallingReactionInputPortName;
	string getFallingReactionSpeechInputPortName;
	string getSocialReactionInputPortName;
	string getSocialReactionSpeechInputPortName;
	string getSocialFacialExpressionInputPortName;
    string getInterruptionInputPortName;
	string sayOutputPortName;
	string moveSequenceOutputPortName;  
	string motorFeedbackInputPortName;
   string handlerPortName;
   
   /* class variables */

   BufferedPort<Bottle>			       getEyeBlinkingIn;
	BufferedPort<VectorOf<int>>        getFallingReactionIn;
	BufferedPort<VectorOf<int>>		   getFallingReactionSpeechIn;
	BufferedPort<VectorOf<int>>        getSocialReactionIn;
	BufferedPort<VectorOf<int>>		   getSocialReactionSpeechIn;
	BufferedPort<VectorOf<int>>        getSocialFacialExpressionIn;
    BufferedPort<VectorOf<int>>        getInterruptionIn;
	BufferedPort<Bottle>			   sayOut;
	BufferedPort<Bottle>			   moveSequenceOut;
	BufferedPort<Bottle>               motorFeedbackIn;
   Port handlerPort;                               // a port to handle interactive messages (also uses bottles)

   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   ActuationSimulatorThread *actuationSimulatorThread;

public:
   
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
};

 