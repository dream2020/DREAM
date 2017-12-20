assessChildEngagementTEST

Author: Serge Thill
Email: serge.thill@his.se
Institution: University of Skövde
Date: 18/11/2016
======================

This test application demonstrates the use of the assessChildEngagement component (part of WP5).

Resources required to run the test:
 --assessChildEngagement and assessChildEngagementDriver (provided in its own folder) components 
 
To run the test/expected results:
1. Run both components, connect the ports
2. In a console, run:

yarp rpc /assessChildEngagementDriver

Press Enter to cycle through the possible communications and observe the readouts printed in the console that demonstrate functionality. The test cycles through every possible combination of inputs and should show correct output in each case (the engagement is the sum of the number of behaviours that show engagement).

Note:
The component behaves according to the latest requirements by Cristina, which are slightly different from the truth table specified at the Developer's meeting. While the component checks for smiling behaviour, placement in front of the robot, and mutual gaze (the test app simulates the corresponding data) as per the meeting notes, engagement is actually defined - as stated above - as the sum of such behaviours that the child displays (i.e. ranging from 0 to 3). This has no effect on the system architecture where the port used transmits a VectorOf<int> in any case.

System architecture ports being used:

/assessChildEngagement/engagementFlag:o 
/assessChildEngagement/getBody:i
/assessChildEngagement/checkMutualGaze:i
/assessChildEngagement/identifyFaceExpression:i

The following parameters can be configured in the .ini: 

*centre_tolerance: this defines how far off-centre the child can be while still being considered to be in front of the robot. Adjust as needed. To verify functionality in the unit test, note that the test simulates an off-centre body at location +10, so a value higher than 10 will result in the unit test always reporting a central position.