/** @file attentionReactionSystemGUI.h  Interface file for the attentionReaction Component 
 *
 * Version 1.0
 * 
 * 14/08/2015
 *
 *
 * \defgroup DREAM_cognitiveComponent attentionReactionSystemGUI
 * 
 * Interface file for the attentionReaction subsystem in the cognitive controller component
 * 
 * \section component_description Component Description
 *  
 * The current component is the implementation of the attentionReactionSystemGUI placeholder component.
 * 
 *
 * This component is used to provide with simulated inputs to the attentionReaction component.
 * The GUI has been used to simulate the sensory information of the robot as a first step in the
 * implementation of this system. It should be replace by a sensory information simulator of an specific
 * robot, i.e. Nao robot. But for the purpose of validating the attentionReaction subsystem the GUI is an equally
 * good option.
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
 * - \c from \c attentionReactionSystemGUI.ini 
 *     - specifies the configuration file
 *
 * - \c context \c components/attentionReactionSystemGUI/config
 *     - specifies the sub-path from \c $DREAM_ROOT/release to the configuration file
 *
 * - \c name \c attentionReactionSystemGUI
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
 * from  |  attentionReactionSystemGUI.ini
 * 
 * specifies the configuration file
 * 
 * Key | Value
 * :-- | :----
 * context  |  components/attentionReactionSystemGUI/config
 * 
 * specifies the sub-path from \c $DREAM_ROOT/release/ 
 * to the configuration file
 * 
 * Key | Value
 * :-- | :----
 * name  |  attentionReactionSystemGUI
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
 *  <b>Input ports</b>
 * 
 *     /reactiveSystemGUI/eyeBlinking:i
 *     /reactiveSystemGUI/fallingInterruption:i
 *     /reactiveSystemGUI/fallingReactionSpeech:i
 *     /reactiveSystemGUI/moveSequence:i
 *     /reactiveSystemGUI/say:i
 *     /reactiveSystemGUI/socialReactionSpeech:i 
 *
 * 
 * <b>Output ports</b>
 *
 *     /reactiveSystemGUI/recognizeSpeech:o
 *     /reactiveSystemGUI/checkMutualGaze:o
 *     /reactiveSystemGUI/identifyFaceExpression:o
 *     /reactiveSystemGUI/affectiveState:o
 *     /reactiveSystemGUI/robotSensors:o
 *     /reactiveSystemGUI/reactionSwitchOff:o
 *     /reactiveSystemGUI/getFaces:o
 *     /reactiveSystemGUI/getSoundDirection:o
 *     /reactiveSystemGUI/attentionBias:o
 *     /reactiveSystemGUI/therapistGazeCommand:o
 *     /reactiveSystemGUI/attentionSwitchOff:o
 * 
 * <b>Port types </b>
 *
 * The functional specification only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *                        
 * BufferedPort<Bottle>			      *recognizeSpeechOut,   
 * BufferedPort<VectorOf<int>>        *checkMutualGazeOut, 
 * BufferedPort<Bottle>               *affectiveStateOut,
 * BufferedPort<Bottle>               *eyeBlinkingIn,
 * BufferedPort<VectorOf<int>>        *fallingInterruptionIn,
 * BufferedPort<VectorOf<int>>	      *fallingReactionSpeechIn,
 * BufferedPort<Bottle>			      *moveSequenceIn,
 * BufferedPort<Bottle>			      *sayIn,
 * BufferedPort<VectorOf<double>>     *getFacesOut,
 * BufferedPort<VectorOf<double>>     *getSoundDirectionOut,
 * BufferedPort<VectorOf<int>>		  *identifyFaceExpressionOut, 
 * BufferedPort<Bottle>               *robotSensorsOut,
 * BufferedPort<VectorOf<double>>     *attentionBiasOut, 
 * BufferedPort<VectorOf<double>>     *therapistGazeCommandOut,
 * BufferedPort<VectorOf<int>>        *attentionSwitchOffOut,
 * BufferedPort<VectorOf<int>>        *reactiveSwitchOffOut,
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
 * \c attentionReactionSystemGUI.ini   
 *
 *
 * \section example_sec Example Instantiation of the Module
 * 
 * <tt>attentionReactionSystemGUI --name attentionReactionSystemGUI --context components/attentionReactionSystemGUI/config --from attentionReactionSystemGUI.ini </tt>
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
#include <time.h>
#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
 
#include "guiUtilities.h"

using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;
  
#define STRINGLENGTH 200 // used to define a string when post-processing the bottle messages

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

class AttentionReactionSystemGUIThread : public Thread {

private:

   /* class variables */

   bool              debug;
   string            logoFilenameValue;
 
   double thresholdValueDouble;

   /* thread parameters: they are pointers so that they refer to the original variables in reactiveSystemGUI */
  
   BufferedPort<Bottle>			    *recognizeSpeechPortOut;
   BufferedPort<VectorOf<int>>      *checkMutualGazePortOut; 
   BufferedPort<Bottle>             *affectiveStatePortOut;
   BufferedPort<Bottle>		        *eyeBlinkingPortIn;
   BufferedPort<VectorOf<int>>      *fallingInterruptionPortIn;
   BufferedPort<VectorOf<int>>	    *fallingReactionSpeechPortIn;
   BufferedPort<Bottle>			    *moveSequencePortIn;
   BufferedPort<Bottle>			    *sayPortIn;
   BufferedPort<VectorOf<double>>   *getFacesPortOut;
   BufferedPort<VectorOf<double>>   *getSoundDirectionPortOut;
   BufferedPort<VectorOf<int>>		*identifyFaceExpressionPortOut;
   BufferedPort<Bottle>             *robotSensorsPortOut;   
   BufferedPort<VectorOf<double>>   *attentionBiasPortOut;
   BufferedPort<VectorOf<double>>   *therapistGazeCommandPortOut; 
   BufferedPort<VectorOf<int>>      *attentionSwitchOffPortOut;
   BufferedPort<VectorOf<int>>      *reactiveSwitchOffPortOut;
   
public:

   /* class methods */

   AttentionReactionSystemGUIThread(                             
							   BufferedPort<Bottle>			   *recognizeSpeechOut,   
                               BufferedPort<VectorOf<int>>     *checkMutualGazeOut, 
                               BufferedPort<Bottle>            *affectiveStateOut,
    						   BufferedPort<Bottle>            *eyeBlinkingIn,
							   BufferedPort<VectorOf<int>>     *fallingInterruptionIn,
							   BufferedPort<VectorOf<int>>	   *fallingReactionSpeechIn,
                               BufferedPort<Bottle>			   *moveSequenceIn,
                               BufferedPort<Bottle>			   *sayIn,
                               BufferedPort<VectorOf<double>>  *getFacesOut,
                               BufferedPort<VectorOf<double>>  *getSoundDirectionOut,
                               BufferedPort<VectorOf<int>>	   *identifyFaceExpressionOut, 
                               BufferedPort<Bottle>            *robotSensorsOut,                               
                               BufferedPort<VectorOf<double>>  *attentionBiasOut, 
                               BufferedPort<VectorOf<double>>  *therapistGazeCommandOut,
                               BufferedPort<VectorOf<int>>     *attentionSwitchOffOut,
                           	   BufferedPort<VectorOf<int>>     *reactiveSwitchOffOut,
							   string logoFilename);
   bool threadInit();     
   void threadRelease();
   void run(); 
};


class AttentionReactionSystemGUI:public RFModule {

   /* module parameters */

   string moduleName;

   string handlerPortName;
   string logoFilename; 
   string recognizeSpeechOutputPortName;
   string checkMutualGazeOutputPortName;
   string affectiveStateOutputPortName;   
   string eyeBlinkingInputPortName;
   string fallingInterruptionInputPortName;   
   string fallingReactionSpeechInputPortName;
   string moveSequenceInputPortName;
   string sayInputPortName;
   string getFacesOutputPortName;
   string getSoundDirectionOutputPortName;
   string identifyFaceExpressionOutputPortName;
   string robotSensorsOutputPortName;
   string attentionBiasOutputPortName;
   string therapistGazeCommandOutputPortName; 
   string attentionSwitchOffOutputPortName;
   string reactiveSwitchOffOutputPortName;
 
   /* class variables */
     
   Port                            handlerPort;                  // a port to handle interactive messages (also uses bottles)

   BufferedPort<Bottle>			  recognizeSpeechOut;  
   BufferedPort<VectorOf<int>>    checkMutualGazeOut;    
   BufferedPort<Bottle>           affectiveStateOut;
   BufferedPort<Bottle>           eyeBlinkingIn;
   BufferedPort<VectorOf<int>>    fallingInterruptionIn;
   BufferedPort<VectorOf<int>>	  fallingReactionSpeechIn;
   BufferedPort<Bottle>			  moveSequenceIn;
   BufferedPort<Bottle>			  sayIn;
   BufferedPort<VectorOf<double>> getFacesOut;
   BufferedPort<VectorOf<double>> getSoundDirectionOut;
   BufferedPort<VectorOf<int>>	  identifyFaceExpressionOut;     
   BufferedPort<Bottle>           robotSensorsOut;
   BufferedPort<VectorOf<double>> attentionBiasOut; /*provides coordinates and behavior*/
   BufferedPort<VectorOf<double>> therapistGazeCommandOut; /*provides coordinates and behavior*/
   BufferedPort<VectorOf<int>>    attentionSwitchOffOut;
   BufferedPort<VectorOf<int>>    reactiveSwitchOffOut;
   
   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   AttentionReactionSystemGUIThread *attentionReactionSystemGUIThread;

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
 