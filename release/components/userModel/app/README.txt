userModelTEST

Author: James Kennedy
Email: james.kennedy@plymouth.ac.uk
Institution: Plymouth University
Date: 24/09/2016
======================

This test application demonstrates the use of the userModel component (part of WP6), which manages the user data in a series of XML files

Resources required to run the test:
 --userModel and (/test) userModelDriver components 
 --(including userModel/config/userdata/user_1.dream and userModel/config/userdata/userlist.dream config files)
 --/include/userDataLibrary.h and /include/userListLibrary.h centralised shared headers

To run the test/expected results:
1. Run the app, connect the ports
2. Start the test procedure:
	$ yarp rpc /userModelDriver
	$ test
	
The test will simulate the communication that would happen between the userModel component, and the selfMonitoringSubsystem and systemGUI components (functionality encapsulated in userModelDriver component).
A user data file (config/userdata/user_1.dream) has been submitted for a test user (with ID of 1) - this user will automatically be selected for the test.
The process will be documented through readouts in the console. In summary, these steps include:
1. the driver (simulating systemGUI functionality) sending a selected user ID to the userModel
2. the userModel returning the full user data to the userModelDriver (would be to selfMonitoringSubsystem)
3. the userModelDriver simulating data from an interaction (incrementing the highest historical interaction values by 1) and returning this to the userModel (note: the performance arrays are the same for all tests)
4. check the config/userdata/user_1.dream file - a new entry should exist for the simulated interaction
(/4. or run the test again and the readout of historical interaction data should be updated)


System architecture ports being used:
 --None

Other ports being used:
/userModel/userID:i
/userModel/updatedData:i

/userModel/userData:o

/userModelDriver/userData:i
/userModelDriver/userID:o
/userModelDriver/updatedData:o

The component configuration file does not contain parameters other than the port names, which need to be at their default value for the TEST to be able to connect ports.