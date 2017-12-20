deliberativeSubsystemTEST

Author: Emmanuel Senft
Email: emmanuel.senft@plymouth.ac.uk
Institution: Plymouth University
Date: 30/11/2016
======================

This test application (deliberativeSubsystemTEST) demonstrates the use of the the deliberativeSubsystem (part of WP6). 

Similarly, delibSubWP6TEST demonstrates the use of the deliberativeSubsystem with other components in WP6 to show the execution of every step of the unit test on the nao. This test includes deliberativeSubsystem, deliberativeDriver, actuationSubsystem, actuationDriver, naoInterface, sandtrayServer and sandtrayEvent and demonstrate the real execution of the test with the robot (it does not work on the simulator as key behaviours are missing and prevent to go to the next step). To be run, the sandtray game engine has to be started and has to include the lib06 and lib08. Unlike actuactionSubWP6, when testing with the real robot, the middle tactil head sensor has to be touched when nao puts its left hand forward (this can be also simulated using choregraphe and the box raise event with the key "MiddleTactilTouched" and pressing 1 when required).

Resources required to compile and run the simple test:
 --deliberativeSubsystem and deliberativeDriver components
 --include/scriptMessages.h include/sandtrayMessages.h include/actionMessages.h include/sandtrayMessages.h include/feedbackMessages.h include/actionOrigin.h include/waiter.h include/commonFonctionalities.h
 
Both deliberativeSubsystem and deliberativeDriver require Boost to compile.

Be careful, required to set encoding for deliberativeSubsystemController.cpp to unicode UTF-8 WITHOUT SIGNATURE - or it will make naoInterface crash.
 
 
To run the test/expected results:
1. Run the app, connect the ports
2. Start the test procedure:
	$ yarp rpc /deliberativeDriver
	$ test

The test will step through a script, simulating inputs in the deliberative subsystem and output produced on the different ports: interpreting the script steps to send suggestions to the GUI and commands to the actuation. The main port used is interventionCommand, the port used for sending the script steps which will be parsed by the deliberativeSubsystem, sent as action to the GUI to be validated and then send to the actuation to be interpreted in primitives used in naoInterface. NaoInterface executes the action and return a feedback relayed to the deliberativeSubsystem via the actionFeedback port, and the deliberativeSubsystemwhich decides to go to the next action. The selectedAction port is implemented to know when an action is being executed and to know what to wait to send the message to go to the next step. The deliberativeSubsystem also receives information about the interaction using GetChildPerformance and CheckMutualGaze, sandtrayEvent and sandtrayReturn. The therapist can also use the port selectedAction to inform the deliberative of certain event or force the execution of actions. The output ports used are: interactionEvent (relay sandtrayEvent), getInterventionStatus (to inform the expectations concerning the performance), commandSuccess (to move to the next step), sandtrayCommand and mainly suggestedAction (to send action to be validated by the therapist).

System architecture ports being used:
/deliberativeSubsystem/getChildPerformance:i
/deliberativeSubsystem/checkMutualGaze:i
/deliberativeSubsystem/getInterventionStatus:o
/deliberativeSubsystem/interactionEvent:o

Other ports being used:
/deliberativeSubsystem/interventionCommand:i
/deliberativeSubsystem/selectedAction:i
/deliberativeSubsystem/actionFeedback:i
/deliberativeSubsystem/sandtrayEvent:i
/deliberativeSubsystem/commandSuccess:o
/deliberativeSubsystem/suggestedAction:o
/deliberativeSubsystem/sandtrayCommand:o

The component configuration file does not contain parameters other than the port names, which need to be at their default value for the TEST to be able to connect ports.