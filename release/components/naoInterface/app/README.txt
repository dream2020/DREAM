naoInterfaceTEST

Author: Emmanuel Senft
Email: emmanuel.senft@plymouth.ac.uk
Institution: Plymouth University
Date: 30/11/2016
======================

This test application demonstrates the use of the the naoInterface (part of WP6).
The ip and port of the robot have to be set using the config.ini file. This component work both with the physical robot and the simulator.

Resources required to compile and run the test:
 --naoInterface and naoInterfaceDriver components
 --include/actionMessages.h include/feedbackMessages.h include/moveSequenceId include/waiter.h
 
 naoInterface uses naoQi
 naoInterfaceDriver uses Boost
 
To prevent crash, some behaviour are disabled when used in the simulator (i.e. when the ip is 127.0.0.1) as some functionalities are not reproduced in the simulator (autonomous life and loaded behaviours). The program should run smoothly both with a real robot and the simulator. If the real robot is used, two behaviour should be loaded to complete the whole test: the clapping behaviour with the name "clapping-7c9e59/behavior_1" and the move car behaviour whose name can be set in the config file. This two behaviours can be found in the config folder and loaded using choregraph (loading the behaviour and the pressing Robot application and package and install current behaviour).
 
To run the test/expected results:
1. Run the app, connect the ports
2. Start the test procedure:
	$ yarp rpc /naoInterfaceDriver
	$ test

The aim of the test is to demonstrate that all the primitives used for this year experiments are implemented and that the robot can execute different behaviours using commands on the input ports. As shown in moveSequenceId moveSequence being used for different purposes (take hard-coded positions or blink for example). Feedback are provided to indicate the end of an action. 

The component configuration file contain in addition to the port names parameters which need to be set for ip and port of the robot.
Additional choregraphe behaviours can be specified in the config file with the instruction: 
behaviourX name
with X the number of the behaviour and name the identifier in choregraphe.
These additional behaviours can be used in the script with the command:
_SCRIPT_MOVE_ X.

System architecture ports being used:
/naoInterface/enableRobot:i
/naoInterface/disableRobot:i
/naoInterface/grip:i
/naoInterface/moveHand:i
/naoInterface/moveTorso:i
/naoInterface/release:i
/naoInterface/say:i
/naoInterface/moveSequence:i
/naoInterface/moveHead:i

Other ports being used:
/naoInterface/pointAt:i
/naoInterface/sensorFeedback:o
/naoInterface/robotMotorFeedback:o



