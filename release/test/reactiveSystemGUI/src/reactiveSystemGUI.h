/** @file reactiveSystemGUI.h  Interface file for the social reaction module in the Reacive subsystem component
 *
 * Version 1.0
 * 
 * 14/08/2015
 *
 *
 * \defgroup DREAM_reactiveComponent reactiveSystemGUI
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
 * The current component is the implementation of the reactiveSystemGUI placeholder component.
 * 
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
 * - \c from \c reactiveSystemGUI.ini 
 *     - specifies the configuration file
 *
 * - \c context \c components/reactiveSystemGUI/config
 *     - specifies the sub-path from \c $DREAM_ROOT/release to the configuration file
 *
 * - \c name \c reactiveSystemGUI
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
 * from  |  reactiveSystemGUI.ini
 * 
 * specifies the configuration file
 * 
 * Key | Value
 * :-- | :----
 * context  |  components/reactiveSystemGUI/config
 * 
 * specifies the sub-path from \c $DREAM_ROOT/release/ 
 * to the configuration file
 * 
 * Key | Value
 * :-- | :----
 * name  |  reactiveSystemGUI
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
 *     /reactiveSystemGUI/getInterruption:i
 *     /reactiveSystemGUI/getFallingReactionSpeech:i
 *     /reactiveSystemGUI/getEyeBlinking:i
 *     /reactiveSystemGUI/getSocialReactionSpeech:i 
 *     /reactiveSystemGUI/moveSequence:i
 *     /reactiveSystemGUI/say:i
 *
 * 
 * <b>Output ports</b>
 *
 *     /reactiveSystemGUI/recognizeSpeech:o
 *     /reactiveSystemGUI/checkMutualGaze:o
 *     /reactiveSystemGUI/getDeliberativeFacialExpression:o
 *     /reactiveSystemGUI/getSpeech:o
 *     /reactiveSystemGUI/getGazeVariation:o
 *     /reactiveSystemGUI/eyeBlinkingSwitchOff:o
 *     /reactiveSystemGUI/identifyFaceExpression:o
 *     /reactiveSystemGUI/getHeadTactile:o
 *     /reactiveSystemGUI/getHandTactile:o
 *     /reactiveSystemGUI/getBumperTactile:o
 *     /reactiveSystemGUI/socialReactionSwitchOff:o
 *     /reactiveSystemGUI/getBalance:o
 *     /reactiveSystemGUI/fallingReactionSwitchOff:o
 * 
 * <b>Port types </b>
 *
 * The functional specification only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *
 * Port                            *sensoryInterpretationOut,                            
 * BufferedPort<Bottle>			   *recognizeSpeechOut,   
 * BufferedPort<VectorOf<int>>     *checkMutualGazeOut, 
 * BufferedPort<VectorOf<int>>     *identifyFaceExpressionOut, 
 * BufferedPort<VectorOf<int>>     *getDeliberativeFacialExpressionOut,
 * BufferedPort<Bottle>		       *getSpeechOut,
 * BufferedPort<VectorOf<int>>     *getGazeVariationOut,
 * BufferedPort<VectorOf<int>>     *getBalanceOut,
 * BufferedPort<Bottle>            *getHeadTactileOut,
 * BufferedPort<Bottle>            *getHandTactileOut,
 * BufferedPort<Bottle>            *getBumperTactileOut,
 * BufferedPort<Bottle>            *getEyeBlinkingIn,
 * BufferedPort<VectorOf<int>>     *eyeBlinkingSwitchOffOut,
 * BufferedPort<VectorOf<int>>     *getInterruptionIn,
 * BufferedPort<VectorOf<int>>     *fallingReactionSwitchOffOut,
 * BufferedPort<VectorOf<int>>     *socialReactionSwitchOffOut,
 * BufferedPort<VectorOf<int>>	   *getSocialReactionSpeechIn,
 * BufferedPort<VectorOf<int>>	   *getFallingReactionSpeechIn,
 * BufferedPort<Bottle>			   *moveSequenceIn,
 * BufferedPort<Bottle>			   *sayIn,
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
 * \c reactiveSystemGUI.ini   
 *
 *
 * \section example_sec Example Instantiation of the Component
 * 
 * <tt>reactiveSystemGUI --name reactiveSystemGUI --context components/reactiveSystemGUI/config --from reactiveSystemGUI.ini </tt>
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
 * 03/07/15  First version validated (Pablo Gomez)
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
#define NONE    0
#define HAPPY   1
#define SAD     2
#define ANGRY   3 
#define FEARFUL 4
#define AFFECTIVE 1
#define AGGRESIVE 2

class ReactiveSystemGUIThread : public Thread {

private:

   /* class variables */

   bool              debug;
   string            logoFilenameValue;
 
   double thresholdValueDouble;

   /* thread parameters: they are pointers so that they refer to the original variables in reactiveSystemGUI */
   
  
   Port                           *sensoryInterpretationPortOut;
   BufferedPort<Bottle>			  *recognizeSpeechPortOut;
   BufferedPort<VectorOf<int>>    *checkMutualGazePortOut; 
   BufferedPort<VectorOf<int>>    *identifyFaceExpressionPortOut;
   BufferedPort<VectorOf<int>>    *getDeliberativeFacialExpressionPortOut;
   BufferedPort<Bottle>		      *getSpeechPortOut;
   BufferedPort<VectorOf<int>>    *getGazeVariationPortOut;
   BufferedPort<VectorOf<int>>    *getBalancePortOut;
   BufferedPort<Bottle>           *getHeadTactilePortOut;
   BufferedPort<Bottle>           *getHandTactilePortOut;
   BufferedPort<Bottle>           *getBumperTactilePortOut;
   BufferedPort<Bottle>		      *getEyeBlinkingPortIn;
   BufferedPort<VectorOf<int>>    *eyeBlinkingSwitchOffPortOut;
   BufferedPort<VectorOf<int>>    *getInterruptionPortIn;
   BufferedPort<VectorOf<int>>    *fallingReactionSwitchOffPortOut;
   BufferedPort<VectorOf<int>>    *socialReactionSwitchOffPortOut;
   BufferedPort<VectorOf<int>>	  *getSocialReactionSpeechPortIn;
   BufferedPort<VectorOf<int>>	  *getFallingReactionSpeechPortIn;
   BufferedPort<Bottle>			  *moveSequencePortIn;
   BufferedPort<Bottle>			  *sayPortIn;

public:

   /* class methods */

   ReactiveSystemGUIThread(
                               Port                            *sensoryInterpretationOut,                              
							   BufferedPort<Bottle>			   *recognizeSpeechOut,   
                               BufferedPort<VectorOf<int>>     *checkMutualGazeOut, 
                               BufferedPort<VectorOf<int>>     *identifyFaceExpressionOut, 
                               BufferedPort<VectorOf<int>>     *getDeliberativeFacialExpressionOut,
                               BufferedPort<Bottle>		       *getSpeechOut,
                               BufferedPort<VectorOf<int>>     *getGazeVariationOut,
                               BufferedPort<VectorOf<int>>     *getBalanceOut,
                               BufferedPort<Bottle>            *getHeadTactileOut,
                               BufferedPort<Bottle>            *getHandTactileOut,
							   BufferedPort<Bottle>            *getBumperTactileOut,
							   BufferedPort<Bottle>            *getEyeBlinkingIn,
							   BufferedPort<VectorOf<int>>     *eyeBlinkingSwitchOffOut,
							   BufferedPort<VectorOf<int>>     *getInterruptionIn,
							   BufferedPort<VectorOf<int>>     *fallingReactionSwitchOffOut,
							   BufferedPort<VectorOf<int>>     *socialReactionSwitchOffOut,
                               BufferedPort<VectorOf<int>>	   *getSocialReactionSpeechIn,
                               BufferedPort<VectorOf<int>>	   *getFallingReactionSpeechIn,
                               BufferedPort<Bottle>			   *moveSequenceIn,
                               BufferedPort<Bottle>			   *sayIn,
							   string logoFilename);
   bool threadInit();     
   void threadRelease();
   void run(); 
};


class ReactiveSystemGUI:public RFModule {

   /* module parameters */

   string moduleName;

   string handlerPortName;
   string logoFilename;
   string sensoryInterpretationOutputPortName;   
   string recognizeSpeechOutputPortName;
   string checkMutualGazeOutputPortName;
   string identifyFaceExpressionOutputPortName;
   string getDeliberativeFacialExpressionOutputPortName;
   string getSpeechOutputPortName;
   string getGazeVariationOutputPortName;
   string getBalanceOutputPortName;
   string getHeadTactileOutputPortName;
   string getHandTactileOutputPortName;
   string getBumperTactileOutputPortName;
   string getEyeBlinkingInputPortName;
   string eyeBlinkingSwitchOffOutputPortName;
   string getInterruptionInputPortName;   
   string fallingReactionSwitchOffOutputPortName;
   string socialReactionSwitchOffOutputPortName;
   string getSocialReactionSpeechInputPortName;
   string getFallingReactionSpeechInputPortName;
   string moveSequenceInputPortName;
   string sayInputPortName;

   /* class variables */
     
   Port                            handlerPort;                  // a port to handle interactive messages (also uses bottles)

   Port                            sensoryInterpretationOut;
   
   BufferedPort<Bottle>			  recognizeSpeechOut;  
   BufferedPort<VectorOf<int>>    checkMutualGazeOut; 
   BufferedPort<VectorOf<int>>    identifyFaceExpressionOut;     
   BufferedPort<VectorOf<int>>    getDeliberativeFacialExpressionOut;
   BufferedPort<Bottle>		      getSpeechOut;
   BufferedPort<VectorOf<int>>    getGazeVariationOut;
   BufferedPort<VectorOf<int>>    getBalanceOut;
   BufferedPort<Bottle>           getHeadTactileOut;
   BufferedPort<Bottle>           getHandTactileOut;
   BufferedPort<Bottle>           getBumperTactileOut;
   BufferedPort<Bottle>           getEyeBlinkingIn;
   BufferedPort<VectorOf<int>>    eyeBlinkingSwitchOffOut;
   BufferedPort<VectorOf<int>>    getInterruptionIn;
   BufferedPort<VectorOf<int>>    fallingReactionSwitchOffOut;
   BufferedPort<VectorOf<int>>    socialReactionSwitchOffOut;
   BufferedPort<VectorOf<int>>	  getSocialReactionSpeechIn;
   BufferedPort<VectorOf<int>>	  getFallingReactionSpeechIn;
   BufferedPort<Bottle>			  moveSequenceIn;
   BufferedPort<Bottle>			  sayIn;


   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   ReactiveSystemGUIThread *reactiveSystemGUIThread;

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
void happy_cb(Fl_Widget *b,void *destination);
void sad_cb(Fl_Widget *b,void *destination);
void angry_cb(Fl_Widget *b,void *destination);
void fearful_cb(Fl_Widget *b,void *destination);
void none_cb(Fl_Widget *b,void *destination);
void affective_cb(Fl_Widget *b,void *destination);
void aggresive_cb(Fl_Widget *b,void *destination);
 