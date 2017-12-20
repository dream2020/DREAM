assessChildPerformanceTEST

Author: Serge Thill
Email: serge.thill@his.se
Institution: University of Skövde
Date: 18/11/2016 (original version)
======================

This test application demonstrates the use of the assessChildPerformance component (part of WP5).

Resources required to run the test:
 --assessChildPerformance and assessChildPerformanceDriver (provided in its own folder) components 
 
To run the test/expected results:
1. Run both components, connect the ports
2. In a console, run

yarp rpc /assessChildPerformanceDriver

Press Enter to cycle through the possible communications and observe the readouts printed in the console that demonstrate functionality. The test cycles through possible requests, simulates good and bad performance for each, and prints the received evaulation, if any, in the terminal.

Some of these tests will take time to complete since the events are meant to happen within a time window (here set to 1.5s) and some can only be evaluated at the end of it (e.g. no movement). Output values that can be observed are 0 for bad performance, 1 for good performance, a scalar in between (set to 0.7 in the test) for good performance on trajectory identification (this scalar is the confidence reported by the sensory analysis component). A value of -1 means that, for some reason, the component was unable to assess performance.

There are some known issues:
* rapid cycling (quickly pressing Enter) may give false values during the test as assessChildPerformance may begin to lag behind the requests. This should not be a problem in practice since steps in the intervention script do not cycle that rapidly.

* the component may fail to report good performance on "no sandtray touch". It should however only report bad performance if there in fact was a touch; in that sense it nonetheless works as intended.

System architecture ports being used:

/assessChildPerformance/identifyVoice:i 
/assessChildPerformance/getEyeGaze:i 
/assessChildPerformance/getBodyPose:i
/assessChildPerformance/identifyTrajectory:i 
/assessChildPerformance/getInterventionStatus:i 
/assessChildPerformance/interactionEvent:i 

/assessChildPerformance/identifyTrajectory:o
/assessChildPerformance/getChildPerformance:o

The component has some parameters set in the config file. These are:

* sandtray_edge: this should be the z coordinate of the edge of the sandtray over which the child is not supposed to move when "no movement" is expected. This value needs to be calibrated to something reasonable while running the system.

To test that changing values in the unit test has an effect, bear in mind that the unit test moves the skeleton from coordinates 100 towards the origin (and beyond) in increments of -10. Setting the parameter to a value above 100 will therefore report bad performance in both good and bad cases while setting it to a sufficiently low (negative) vlaue will report good performance in both good and bad cases.

* gaze_thr: when requesting a gaze left or right, this defines the minimum distance from 0 that the gaze must be in order to count as being in one of those directions. Increase this value if the system seems to be mis-characterising straight gaze as correct left/right gaze; decrease it corret left/right gaze is not recognised as such.

To verify functionality in the unit test, note that good glances will be correctly identified only if the threshold is less than 50 (50 being the coordinate at which the unit test places a "correct" glance).

* flip_lr: it is ambiguous whether left/right in the scripts refers to the robot's or the child's left/right. This value can be set to -1 to invert the interpretation if needed.

To verify functionality in the unit test, note that flipping the value will invert performance on the relevant left/right events.