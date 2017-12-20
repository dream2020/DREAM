attentionReaction Subsystem TEST
================================

The attentionReaction Subsystem, as described in Deliverables D6.1 and D6.2, comprises the attention and the reactive subsystems.

The attention subsystem is a combination of perceptual attention provided by sensory information from WP4
 and (/getSoundDirection:i and /getFaces:i) attention emulation from the Deliberative subsystem (\attentionBias:i) to direct the robot’s attention and gaze (/elicitedAttention:o). A gaze reaction triggered in social interactions is produced by the reactive subsystem. These inputs provide the robot with a locus of attention that it can use to organize its behavior. The attention subsystem is open for commands from the Self-Monitoring subsystem (/therapistGazeCommand:i and \attentionSwitchOff:i) to overrule this locus of attention whenever the therapist considers it is needed.
 
The Reactive subsystem, as described in Deliverable D6.1, comprises three modules:
- Falling Reaction, 
- Social Reaction, and
- Eye Blinking. 
  
The Falling Reaction module will be periodically checking the balance of the robot using the sensory
information available (\robotSensors:i). Changes in the balance may end up in a fall. In such case, a signal will
be sent to the Self-Monitoring subsystem to interrupt (\fallingInterruption:o) any other running behavior, and a damage 
avoidance behavior that fits the situation will be triggered (\fallingReaction:o). Additionally, the robot should include some speech acts to reduce the impact of such dramatic situation for the kid (\fallingReactionSpeech:o). 

The purpose of the social reaction module is to provide the appropriate social behavior in order to give the impression of the robot being socially alive. This module receives as input the sensory information where it is specified the child’s social and affective state (\identifyFaceExpression:i and \affectiveState:i). For each of these behaviors there should be a set of facial expressions and speech acts available to choose among them (\socialReaction:o, \socialReactionSpeech:o and \socialFacialExpression:o). The functionality of this module can be switched on and off when needed through the Self-Monitoring subsystem (\reactionSwitchOff:i).

We need to recreate a blinking behavior (\eyeBlinking:o) mainly focused on the communicative behaviors
and gaze shifts (\checkMutualGaze:i and \recognizeSpeech:i). 

 
This subsystem operates with the following ports:

From System Architecture:

Input ports
 
     /attentionReactionSubsystem/checkMutualGaze:i
     /attentionReactionSubsystem/getFaces:i
     /attentionReactionSubsystem/getSoundDirection:i
     /attentionReactionSubsystem/identifyFaceExpression:i 
     /attentionReactionSubsystem/recognizeSpeech:i 

These ports provide the component with sensory information. For this stage of the development all these outputs are sent through the attentionReactionSystemGUI component.
 
Ports created:

Input ports

     /attentionReactionSubsystem/attentionBias:i
     /attentionReactionSubsystem/affectiveState:i
     /attentionReactionSubsystem/attentionSwitchOff:i 
     /attentionReactionSubsystem/reactionSwitchOff:i
     /attentionReactionSubsystem/therapistGazeCommand:i
     /attentionReactionSubsystem/robotSensors:i

These ports provide the component with deliberative and GUI components outputs. For this stage of the development all these outputs are sent through the attentionReactionSystemGUI component.

     /attentionReactionSubsystem/actionFeedback:i
 
This specifies whether the robot has finished a previous task. Only in those cases this component will accept new inputs.


Output ports
  
     /attentionReactionSubsystem/eyeBlinking:o
     /attentionReactionSubsystem/fallingReaction:o
     /attentionReactionSubsystem/fallingReactionSpeech:o
     /attentionReactionSubsystem/socialFacialExpression:o
     /attentionReactionSubsystem/socialReaction:o
     /attentionReactionSubsystem/socialReactionSpeech:o
     /attentionReactionSubsystem/fallingInterruption:o
     /attentionReactionSubsystem/elicitedAttention:o

Through these ports this component sends its output to the actuationSubsystem component.


This component functionality depends on a driver component. The attentionReactionSystemGUI is used as a driver to provide this component with sensory information and outputs coming from other components still to be developed (\checkMutualGaze:i, \getFaces:i, \getSoundDirection:i, \identifyFaceExpression:i, \recognizeSpeech:i, \attentionBias:i, \affectiveState:i, \attentionSwitchOff:i, \reactionSwitchOff:i and \therapistGazeCommand:i). Sensory information could have been obtained by using the sensory interpretation placeholder but we wanted to have control over the timing of sending different inputs so for such reason we opted for using a GUI.
 
On the other hand, this system uses two stub components to show its outputs. First, it sends its outputs (\eyeBlinking:o, \fallingReaction:o, \fallingReactionSpeech:o, \socialFacialExpression:o, \socialReaction:o, \socialReactionSpeech:o, \fallingInterruption:o and \elicitedAttention:o) to the actuationSubsystem which has been recently developed (still it is a first version) but here it is just used as a stub component. Outputs from this actuationSubsystem are sent to another stub component which is the naoInterface component which send the motor commands to the Nao robot.


Instructions to validate component's functionality
===================================================
Once launched the GUI several buttons, check boxes and text boxes are shown. Let's go through them one by one.

- outOfBalance: when activated it interrupts any running behavior. No damage avoidance behavior is used so far. Once deactivated, it resumes normal behavior.
- left_hand_touch, right_hand_touch, left_foot_touch, right_foot_touch, get_head_touch: when activated social reactions might be triggered directing robot's gaze towards where it has been touched, expressing happiness (green eyes) and/or speech.
- mutual_gaze_control: a boolean value. If activated it influences the eye blinking behavior. A blink may occur.
- sound direction, get faces, attention bias, therapist command: each of these buttons generates random coordinates as input for the attention module. The expected output is a gaze towards new coordinates.
- swithOff reactive, switchOff attention: These two turn on/off the reactive or the attention modules. When off, associated inputs in the GUI are deactivated. Micromotions are stopped when the reactive subsystem is off and resumed when on.
- Child say: influences the eye blinking behavior. A blink might occur.
- Identify Facial Expression: given therapist's requirements the robot should only consider child's happy face and change the color of its eyes to green.
- Robot say: does not make the robot say anything but influences the eye blinking behavior. A blink may occur.
- Affective State: according therapist's requirements only happines might be expressed by changing the color of its eyes to green.


Outputs are shown on the robot. Micromotions are launched since interaction begins (by being touched, identified a facial expression, and so on).

config.ini file does not include anything. However it requires to update naoInterface/config/naoInterface.ini with the IP address of the robot

The only library resource used is regarding the guiUtilities.lib already described in other components.
