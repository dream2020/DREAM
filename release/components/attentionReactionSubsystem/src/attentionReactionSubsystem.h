/** @file attentionReactionSubsystem.h  interface file for the attentionReaction Subsystem
 *
 * Version 1.0
 * 
 * 10/08/2016
 *
 *
 * \defgroup DREAM_attentionReactionSubsystem attentionReactionSubsystem
 * 
 * Interface file for the attentionReaction Subsystem in the cognitive controller component
 * 
 * \section component_description Component Description
 * 
 * The attentionReaction Subsystem, as described in Deliverables D6.1 and D6.2, comprises the attention and the reactive subsystems.
 *
 * The attention subsystem is a combination of perceptual attention, in which perceptual stimuli (reported by, for example, face detection
 * or sound localization in work package WP4) that are particularly salient in the current context have to
 * be selected, and attention emulation (from the Deliberative subsystem) to direct the robot’s attention
 * and gaze. A gaze reaction triggered in social interactions is produced by the reactive subsystem.
 * These inputs provide the robot with a locus of attention that it can use to organize its behavior. The
 * attention subsystem is open for commands from the Self-Monitoring subsystem to overrule this locus of
 * attention whenever the therapist considers it is needed.
 * 
 * For such purpose we have built a target selection algorithm adapted from Zaraki et al.'s model where 
 * a bottom-up attention model based on social features is presented. Some simplifications of such
 * model have been done to adapt it to our context.
 * 
 * The Attention subsystem provides the gaze direction towards the Actuation subsystem.
 *  
 * The Reactive subsystem, as described in Deliverable D6.1, comprises three modules:
 *
 * - Falling Reaction, 
 * - Social Reaction, and
 * - Eye Blinking. 
 *  
 * The Falling Reaction module will be periodically checking the balance of the robot using the sensory
 * information available. Changes in the balance may end up in a fall. In such case, a signal will
 * be sent to the Self-Monitoring subsystem to interrupt any other running behavior, and a damage 
 * avoidance behavior that fits the situation will be triggered. Since the robot will be
 * placed in a table and in case it falls it will be into the floor from certain height, there is no actual need
 * to implement getting up behaviors. However, as the Nao robot includes such behaviors they will be
 * taken into account. Additionally, the robot should include some speech acts to reduce the impact of
 * such dramatic situation for the kid as saying that it has been a little bit clumsy or that it is tired today.
 * Finally, back at its feet, the robot may apologize in order to engage the child back to the intervention
 * or call the re-engagement module in the Deliberative subsystem and it will send a signal to the
 * Self-Monitoring subsystem to restore the system functionality
 *
 *
 * The purpose of the social reaction module is to provide the appropriate social behavior in order to give 
 * the impression of the robot being socially alive. This module receives as input the sensory
 * information where it is specified the child’s social and affective state i.e. whether she/he 
 * is expressing an emotion or is performing a physical behavior (such as touching the robot unexpectedly).
 * For each of these behaviors there should be a set of facial expressions and speech acts available
 * to choose among them. Ideally it should randomize among them in order to look less predictable.
 * Such reactive facial expressions and speech acts should be defined by the therapists and will
 * be stored in the library of the Actuation subsystem. The functionality of this module can be 
 * switched on and off when needed through the Self-Monitoring subsystem.
 *
 *
 * Given the amount of studies made to model human blinking behavior we don’t need to do our
 * own but to use that one that best fits our requirements. Within the context in which DREAM will
 * be applied, we need to recreate a blinking behavior mainly focused on the communicative behaviors
 * and gaze shifts. For such reason, we consider that Ford et al.’s model covers these needs and provide
 * accurate data to implement their model. 
 * Ford et al. defines a model which considers multiple communicative facial behaviors and includes
 * an individual blinking model for each of them. For each identified communicative behavior
 * there is a probability to blink, a determined length, and so on. Moreover there is a passive behavior
 * which simulates a physiological blink mechanism (for cleaning or humidifying the eye) that can be
 * activated when no other blinking behavior has been triggered. To perform the blinking motion there
 * is a blink morphology model which defines, based on statistics, if the blink is simple or multiple, full
 * or half, its duration, etc.
 * 
 *
 * The components that interact with this one have been developed as basic simulators that provide 
 * the expected output. For some of them, such expected output was provided through a GUI.
 * 
 * Such GUI has been used to simulate the sensory information of the robot as a first step in the
 * implementation of this system. It should be replace by a sensory information simulator of an specific
 * robot, i.e. Nao robot. But for the purpose of validating the attentionReaction Subsystem the GUI is an equally
 * good option.
 * On the other hand, this system uses actual actuators to show its outputs. This implementation has
 * been done in a Nao Robot using different layers of Yarp (naoInterface component), keeping its platform-independent
 * flavor.
 * It also includes an Actuation subsystem simulator (actuationSimulator component) which receives the outputs of the attentionReaction Subsystem
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
 * - \c from \c attentionReactionSubsystem.ini 
 *     - specifies the configuration file
 *
 * - \c context \c components/attentionReactionSubsystem/config
 *     - specifies the sub-path from \c $DREAM_ROOT/release to the configuration file
 *
 * - \c name \c attentionReactionSubsystem
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
 * from  |  attentionReactionSubsystem.ini
 * 
 * specifies the configuration file
 * 
 * Key | Value
 * :-- | :----
 * context  |  components/attentionReactionSubsystem/config
 * 
 * specifies the sub-path from \c $DREAM_ROOT/release/ 
 * to the configuration file
 * 
 * Key | Value
 * :-- | :----
 * name  |  attentionReactionSubsystem
 * 
 * specifies the name of the module (used to form the stem of module port names)  
 *
 *
 * \section portsa_sec Ports Accessed
 * 
 * <b>Input ports</b>
 * 
 *     /attentionReactionSubsystem/checkMutualGaze:i
 *     /attentionReactionSubsystem/getFaces:i
 *     /attentionReactionSubsystem/getSoundDirection:i
 *     /attentionReactionSubsystem/identifyFaceExpression:i 
 *     /attentionReactionSubsystem/recognizeSpeech:i 
 *                      
 * \section portsc_sec Ports Created
 *
 * <b>Input ports</b>
 * 
 *     /attentionReactionSubsystem/actionFeedback:i
 *     /attentionReactionSubsystem/attentionBias:i
 *     /attentionReactionSubsystem/affectiveState:i
 *     /attentionReactionSubsystem/attentionSwitchOff:i 
 *     /attentionReactionSubsystem/reactionSwitchOff:i
 *     /attentionReactionSubsystem/therapistGazeCommand:i
 *     /attentionReactionSubsystem/robotSensors:i
 * 
 * <b>Output ports</b>
 * 
 *    /attentionReactionSubsystem/eyeBlinking:o
 *    /attentionReactionSubsystem/fallingReaction:o
 *    /attentionReactionSubsystem/fallingReactionSpeech:o
 *    /attentionReactionSubsystem/socialFacialExpression:o
 *    /attentionReactionSubsystem/socialReaction:o
 *    /attentionReactionSubsystem/socialReactionSpeech:o
 *    /attentionReactionSubsystem/fallingInterruption:o
 * 
 * <b>Port types </b>
 *
 * The functional specification only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *
 * BufferedPort<VectorOf<double>>    *getFacesIn,
 * BufferedPort<VectorOf<double>>    *getSoundDirectionIn,
 * BufferedPort<VectorOf<double>>    *attentionBiasIn, 
 * BufferedPort<VectorOf<double>>    *therapistGazeCommandIn, 
 * BufferedPort<VectorOf<int>>       *checkMutualGazeIn,
 * BufferedPort<Bottle>			     *recognizeSpeechIn,
 * BufferedPort<Bottle>              *affectiveStateIn, 
 * BufferedPort<VectorOf<int>>    *identifyFaceExpressionIn,  
 * BufferedPort<Bottle>			     *robotSensorsIn,
 * BufferedPort<VectorOf<int>>       *attentionSwitchOffIn,
 * BufferedPort<VectorOf<int>>       *reactionSwitchOffIn,
 * BufferedPort<Bottle>			     *actionFeedbackIn,
 * BufferedPort<VectorOf<double>>	 *elicitedAttentionOut,
 * BufferedPort<Bottle>			     *eyeBlinkingOut,
 * BufferedPort<VectorOf<int>>       *fallingInterruptionOut,
 * BufferedPort<VectorOf<int>>	     *fallingReactionOut,
 * BufferedPort<VectorOf<int>>	     *fallingReactionSpeechOut, 
 * BufferedPort<VectorOf<int>>       *socialReactionOut,
 * BufferedPort<VectorOf<int>>	     *socialReactionSpeechOut,
 * BufferedPort<VectorOf<int>>       *socialFacialExpressionOut
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
 * \c attentionReactionSubsystem.ini   
 *
 *
 * \section example_sec Example Instantiation of the Component
 * 
 * <tt>attentionReactionSubsystem --name attentionReactionSubsystem --context components/attentionReactionSubsystem/config --from attentionReactionSubsystem.ini </tt>
 *
 *
 * \author 
 * 
 * Pablo Gómez, Vrije Universiteit Brussel, Belgium.
 * 
 * Copyright (C) 2016 DREAM Consortium
 * 
 */

/* 
 * Copyright (C) 2016 DREAM Consortium
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
 * 10/08/16  First version validated (Pablo Gomez)
 */  


#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <map>
#include <sstream>
#include <windows.h>
#include <stdlib.h>
#include <math.h>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>

using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;
#define STRINGLENGTH 132 // used to define a string when post-processing the bottle messages
#define PAUSE 2000 // used in SLEEP function 
#define SIZE 5 

class AttentionReactionYarpInterface;
class AttentionReactionController;
class AttentionSystemThread;
class EyeBlinkingThread;

class GetFacesInAnalyser : public BufferedPort<VectorOf<double>>{
   virtual void onRead(VectorOf<double>& getFacesInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class GetSoundDirectionInAnalyser : public BufferedPort<VectorOf<double>>{
   virtual void onRead(VectorOf<double>& getSoundDirectionInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class AttentionBiasInAnalyser : public BufferedPort<VectorOf<double>>{
   virtual void onRead(VectorOf<double>& attentionBiasInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class TherapistGazeCommandInAnalyser : public BufferedPort<VectorOf<double>>{
   virtual void onRead(VectorOf<double>& therapistGazeCommandInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class AttentionSwitchOffInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& attentionSwitchOffInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class IdentifyFaceExpressionInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& identifyFaceExpressionInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class RobotSensorsInAnalyser : public BufferedPort<Bottle>{
   virtual void onRead(Bottle& robotSensorsInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class ReactionSwitchOffInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& reactionSwitchOffInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class CheckMutualGazeInAnalyser : public BufferedPort<VectorOf<int>>{
   virtual void onRead(VectorOf<int>& checkMutualGazeInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class RecognizeSpeechInAnalyser : public BufferedPort<Bottle>{
   virtual void onRead(Bottle& recognizeSpeechInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class AffectiveStateInAnalyser : public BufferedPort<Bottle>{
   virtual void onRead(Bottle& affectiveStateInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class ActionFeedbackInAnalyser : public BufferedPort<Bottle>{
   virtual void onRead(Bottle& actionFeedbackInData);
   AttentionReactionYarpInterface *attentionReactionYarpInterface;
public:
   void setAttentionReactionInterface(AttentionReactionYarpInterface* yarpInterface);
};

class AttentionReactionSubsystemThread : public Thread {

private:

   /* class variables */
   bool              debug;
      
   /* thread parameters: they are pointers so that they refer to the original variables in eyeBlinking */

   BufferedPort<VectorOf<double>>    *getFacesPortIn;
   BufferedPort<VectorOf<double>>    *getSoundDirectionPortIn;
   BufferedPort<VectorOf<double>>    *attentionBiasPortIn; 
   BufferedPort<VectorOf<double>>    *therapistGazeCommandPortIn; 
   BufferedPort<VectorOf<int>>       *checkMutualGazePortIn; 
   BufferedPort<Bottle>			     *recognizeSpeechPortIn;
   BufferedPort<Bottle>              *affectiveStatePortIn; 
   BufferedPort<VectorOf<int>>		 *identifyFaceExpressionPortIn;  
   BufferedPort<Bottle>			     *robotSensorsPortIn;
   BufferedPort<VectorOf<int>>       *attentionSwitchOffPortIn;
   BufferedPort<VectorOf<int>>       *reactionSwitchOffPortIn;
   BufferedPort<Bottle>			     *actionFeedbackPortIn;
   BufferedPort<VectorOf<double>>	 *elicitedAttentionPortOut;
   BufferedPort<Bottle>			     *eyeBlinkingPortOut;
   BufferedPort<VectorOf<int>>       *fallingInterruptionPortOut;
   BufferedPort<VectorOf<int>>	     *fallingReactionPortOut;
   BufferedPort<VectorOf<int>>	     *fallingReactionSpeechPortOut; 
   BufferedPort<VectorOf<int>>       *socialReactionPortOut;
   BufferedPort<VectorOf<int>>	     *socialReactionSpeechPortOut;
   BufferedPort<VectorOf<int>>       *socialFacialExpressionPortOut;
   
   AttentionReactionYarpInterface* _yarpInterface;

    GetFacesInAnalyser              *getFacesInAnalyser;
	GetSoundDirectionInAnalyser     *getSoundDirectionInAnalyser;
    AttentionBiasInAnalyser         *attentionBiasInAnalyser;
	TherapistGazeCommandInAnalyser  *therapistGazeCommandInAnalyser;
    AttentionSwitchOffInAnalyser    *attentionSwitchOffInAnalyser;
   	IdentifyFaceExpressionInAnalyser  *identifyFaceExpressionInAnalyser;
    RobotSensorsInAnalyser          *robotSensorsInAnalyser;
    ReactionSwitchOffInAnalyser     *reactionSwitchOffInAnalyser;
    CheckMutualGazeInAnalyser       *checkMutualGazeInAnalyser;
	RecognizeSpeechInAnalyser       *recognizeSpeechInAnalyser;
    AffectiveStateInAnalyser        *affectiveStateInAnalyser;
    ActionFeedbackInAnalyser        *actionFeedbackInAnalyser;

   bool                             _isBusy;		//a robot behaviour button has already been pressed
   VectorOf<double>                 _elicitedAttention;
   VectorOf<double>                 _featureValue;
   VectorOf<double>                 _dist;
   VectorOf<double>                 _timer;

   std::map<std::string, int> _statusRef;
public:

   /* class methods */

   AttentionReactionSubsystemThread(  
                               BufferedPort<VectorOf<double>>    *getFacesIn,
                               BufferedPort<VectorOf<double>>    *getSoundDirectionIn,
                               BufferedPort<VectorOf<double>>    *attentionBiasIn, 
                               BufferedPort<VectorOf<double>>    *therapistGazeCommandIn, 
                               BufferedPort<VectorOf<int>>       *checkMutualGazeIn,
                               BufferedPort<Bottle>			     *recognizeSpeechIn,
                               BufferedPort<Bottle>              *affectiveStateIn, 
                               BufferedPort<VectorOf<int>>		 *identifyFaceExpressionIn,                                
                               BufferedPort<Bottle>			     *robotSensorsIn,
                               BufferedPort<VectorOf<int>>       *attentionSwitchOffIn,
                               BufferedPort<VectorOf<int>>       *reactionSwitchOffIn,
                               BufferedPort<Bottle>			     *actionFeedbackIn,
                               BufferedPort<VectorOf<double>>	 *elicitedAttentionOut,
                               BufferedPort<Bottle>			     *eyeBlinkingOut,
                               BufferedPort<VectorOf<int>>       *fallingInterruptionOut,
                               BufferedPort<VectorOf<int>>	     *fallingReactionOut,
                               BufferedPort<VectorOf<int>>	     *fallingReactionSpeechOut, 
                               BufferedPort<VectorOf<int>>       *socialReactionOut,
                               BufferedPort<VectorOf<int>>	     *socialReactionSpeechOut,
                               BufferedPort<VectorOf<int>>       *socialFacialExpressionOut
							);
   void setYarpInterface(AttentionReactionYarpInterface* yarpInterface);
   bool threadInit();     
   void threadRelease();
   void sendElicitedAttentionToActuation(double coordX, double coordY, double coordZ );
   void sendSocialFacialExpressionToActuation(int expression);
   void sendSocialReactionSpeechToActuation(int speech);
   void sendSocialReactionToActuation(int reaction);
   void sendEyeBlinkingToActuation(Bottle eyeBlinking);
   void sendFallingReactionToActuation(int falling);
   void sendFallingReactionSpeechToActuation(int fallingSpeech);
   void sendInterruptionToSelfMonitoring(int interruption);

   void run(); 
   
};


class AttentionReactionSubsystem:public RFModule {
private:
   /* module parameters */

   string moduleName;
   string getFacesInputPortName;
   string getSoundDirectionInputPortName;   
   string attentionBiasInputPortName;
   string therapistGazeCommandInputPortName; 
   string checkMutualGazeInputPortName; 
   string recognizeSpeechInputPortName;
   string affectiveStateInputPortName;
   string identifyFaceExpressionInputPortName;   
   string robotSensorsInputPortName;
   string attentionSwitchOffInputPortName;
   string reactionSwitchOffInputPortName;
   string actionFeedbackInputPortName;
   string elicitedAttentionOutputPortName; 
   string eyeBlinkingOutputPortName; 
   string fallingInterruptionOutputPortName;
   string fallingReactionOutputPortName;
   string fallingReactionSpeechOutputPortName;
   string socialReactionOutputPortName;
   string socialReactionSpeechOutputPortName;
   string socialFacialExpressionOutputPortName;
   
   string handlerPortName;

   /* class variables */
   
    BufferedPort<VectorOf<double>>   getFacesIn;
    BufferedPort<VectorOf<double>>   getSoundDirectionIn;
    BufferedPort<VectorOf<double>>   attentionBiasIn;
    BufferedPort<VectorOf<double>>   therapistGazeCommandIn;
    BufferedPort<VectorOf<int>>      checkMutualGazeIn;
    BufferedPort<Bottle>			 recognizeSpeechIn;
    BufferedPort<Bottle>             affectiveStateIn;
    BufferedPort<VectorOf<int>>		 identifyFaceExpressionIn;    
    BufferedPort<Bottle>			 robotSensorsIn;
    BufferedPort<VectorOf<int>>      attentionSwitchOffIn;
    BufferedPort<VectorOf<int>>      reactionSwitchOffIn;
    BufferedPort<Bottle>			 actionFeedbackIn;
    BufferedPort<VectorOf<double>>	 elicitedAttentionOut;
    BufferedPort<Bottle>			 eyeBlinkingOut;
    BufferedPort<VectorOf<int>>      fallingInterruptionOut;
    BufferedPort<VectorOf<int>>	     fallingReactionOut;
    BufferedPort<VectorOf<int>>	     fallingReactionSpeechOut;
    BufferedPort<VectorOf<int>>      socialReactionOut;
    BufferedPort<VectorOf<int>>	     socialReactionSpeechOut;
    BufferedPort<VectorOf<int>>      socialFacialExpressionOut;

   Port handlerPort;                               // a port to handle interactive messages (also uses bottles)

   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   AttentionReactionSubsystemThread *attentionReactionSubsystemThread;
   
   AttentionReactionController* attentionReactionController;
   AttentionReactionYarpInterface* attentionReactionYarpInterface;
   AttentionSystemThread* attentionSystem;
   EyeBlinkingThread* eyeBlinking;

public:
   
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
};

 