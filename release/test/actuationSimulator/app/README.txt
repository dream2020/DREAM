The current component is the implementation of the actuationSimulator placeholder component.
Its functionality is to provide the naoInterface component with the corresponding action primitives (moveSequence or say) given the outputs
from the Reactive subsystem (fallingReaction, socialReaction and eyeBlinking).

The purpose of this module is to provide a preliminary actuation subsystem to send the reactive subsystem outputs to the corresponding robot. 

This subsystem operates with the following ports:

Input ports

     /actuationSimulator/getFallingReaction:i
     /actuationSimulator/getFallingReactionSpeech:i
     /actuationSimulator/getInterruption:i
     /actuationSimulator/getEyeBlinking:i
     /actuationSimulator/getSocialReaction:i
     /actuationSimulator/getSocialReactionSpeech:i
     /actuationSimulator/getSocialFacialExpression:i

These ports provide the component with the reactive component outputs. 
 
     /actuationSimulator/motorFeedback:i
 
This specifies whether the robot has finished a previous task. Only in those cases this component will accept new inputs 
from the reactive component.
 

Output ports
  
     /actuationSimulator/say:o
     /actuationSimulator/moveSequence:o

Through these ports this component sends its output to the reactiveSystemGUI and naoInterface components the action primitives.