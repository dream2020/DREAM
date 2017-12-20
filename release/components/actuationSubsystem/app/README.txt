actuationSubsystemTEST

Author: Emmanuel Senft
Email: emmanuel.senft@plymouth.ac.uk
Institution: Plymouth University
Date: 30/11/2016
======================

This test application (actuationSubsystemTEST) demonstrates the use of the actuationSubsystem (part of WP6).

Similarly, actuactionSubWP6TEST demonstrates the use of the actuationSubsystem with real data and a real robot. This test includes actuationSubsystem, actuationDriver, naoInterface and sandtrayServer and demonstrate the real execution of the test with the robot (or a simulator). To be run, the sandtray game engine has to be started and set on a library with at least a category and few images (lib08 is a good choice). The library can be changed by pressing the button at the bottom left until reaching the library with fruits and vegetables. 

Resources required to compile and run the simple unit test:
 --actuationSubsystem and actuationDriver components
 --include/actionMessages.h include/sandtrayMessages.h include/moveSequenceId.h include/actionOrigin.h include/waiter.h include/commonFonctionalities.h include/feedbackMessages.h 
 
Both actuationSubsystem and actuationDriver require Boost to compile.

Be carefull, required to set encoding for actuationSubsystemController.cpp to unicode UTF-8 WITHOUT SIGNATURE - or it will make naoInterface crash.
 
To run the test/expected results:
1. Run the app, connect the ports
2. Start the test procedure:
	$ yarp rpc /actuationDriver
	$ test

The test will step through a script, simulating inputs to the actuation and output produced on the different ports: translation from action step to lower lever commands driving the nao and relaying informations for the sensors and results of the different actions.
It will demonstrate the functionality of all the useful actions defined in actionMessages.h.

System architecture ports being used:
/actuationSubsystem/disableRobot:o
/actuationSubsystem/enableRobot:o
/actuationSubsystem/grip:o
/actuationSubsystem/moveHand:o
/actuationSubsystem/moveHead:o
/actuationSubsystem/moveSequence:o
/actuationSubsystem/moveTorso:o
/actuationSubsystem/pointAt:o
/actuationSubsystem/release:o
/actuationSubsystem/say:o

Other ports being used:
/actuationSubsystem/eyeBlinking:i
/actuationSubsystem/fallingReaction:i
/actuationSubsystem/fallingReactionSpeech:i
/actuationSubsystem/socialFacialExpression:i
/actuationSubsystem/socialReaction:i
/actuationSubsystem/socialReactionSpeech:i
/actuationSubsystem/fallingInterruption:i
/actuationSubsystem/elicitedAttention:i
/actuationSubsystem/robotMotorFeedback:i
/actuationSubsystem/selectedAction:i
/actuationSubsystem/sandtrayReturn:i
/actuationSubsystem/sensorFeedback:i
/actuationSubsystem/robotSensors:o
/actuationSubsystem/sandtrayCommand:o
/actuationSubsystem/actionFeedback:o



The component configuration file does not contain parameters other than the port names, which need to be at their default value for the TEST to be able to connect ports.
