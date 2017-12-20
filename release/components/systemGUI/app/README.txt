systemGUITEST

Author: Hoang-Long Cao
Email: hoang.long.cao@vub.ac.be
Institution: Vrije Universiteit Brussel
Date: 25/01/2017
======================

This test application demonstrates the use of the systemGUI (part of WP6), which allows the therapist to supervise the system.

Resources required to run the test:
 --systemGUI, systemGUIDriver components
 --guiUtilities.lib
 --systemGUI/src/buttonsLibrary.h
 --systemGUI/config/data/buttons.dream config file
 --systemGUI/config/icons config folder with icon files
 --systemGUI/config/logo.png and including systemGUI/config/dream-eu-logo.png files
 --include/scriptManagerLibrary.h, include/userListLibrary.h, include/userDataLibrary.h, include/scriptMessages.h, include/actionMessages.h, and include/actionOrigin.h shared headers
 --(including userModel/config/userdata/user_1.dream and userModel/config/userdata/userlist.dream config files)
 --(including scriptManager/config/scripts.dream config file)

To run the test/expected results:
1. Run the app.
2. Connect the ports.
3. Start the test procedure:
	$ Click the button "Create Session" with default values of Name, Session Name, and Session Partner.
	$ Check the Child History tab.
	$ Back to the Load Script tab. Choose "JA3 - systemGUI unit test" in the combobox "Load script". Click the button "Start Script". Note that when a script is running, it can be stopped by clicking the button "Stop script".
	$ Test script flow 
		- A proposed action is displayed on the GUI,
		- To approve the proposed action, click the button "Do it now". Otherwise, click the button "I will choose" and select off-script actions.
		- The timer value can be changed by a slider. Click "Change countdown time".
		- To return to the script, click the button "Back to script".	
		- When the last step is finished, a new script can be launched by clicking "Start script" button.
	$ Check Developer Info. Export to a log file.

This test demonstrates that the systemGUI can read information from different databases, interpret action messages to understandable text, and send therapist's commands to other components.

Parameters in the configuration file (systemGUI.ini):
 _getChildBehaviourIn /getChildBehaviour:i
 _getChildPerformanceIn /getChildPerformance:i
 _proposedToSupervisorIn /proposedToSupervisor:i
 _smsSummaryIn /smsSummary:i
 _selectedBySupervisorOut /selectedBySupervisor:o
 _therapistCommandOut /therapistCommand:o
 _userIDOut /userID:o
These parameters are used to change the component's port names.

System architecture ports being used:
 getChildBehaviour:i
 getChildPerformance:i

Other ports being used:
 proposedToSupervisor:i
 smsSummary:i

 selectedBySupervisor:o
 therapistCommand:o
 userID:o

Hoang-Long Cao
25/01/2017


