WP6TEST

Author: Emmanuel Senft
Email: emmanuel.senft@plymouth.ac.uk
Institution: Plymouth University
Date: 04/01/2017
======================
This test application demonstrates the use of the whole WP6 and the 5 different types of intervention scripts used in DREAM (joint attention, turn taking category, info sharing, imitation object and pattern).

For this test, a scripts.dream file is provided with examples of each scenario demonstrating all the required actions. In this current versions, the scripts may not be perfect (often the robot does not return in standing position for example), but a tool: the scriptGenerator allows an easy creation and modification of existing scripts. This scriptGenerator can be found under release/tools, and stored in release/components/scriptManager/config/scripts.dream (or in another place if scpecified in the the config file of the scriptGenerator in release/tools/scriptGenerator/config/config.ini).

When running WP6TEST, a gui will appear that allow you to select and execute different scripts. To start a script press create session, select the appropriate script in the drop-menu and the press start script. Some scripts require human inputs on the sandtray to select/move an image, on the GUI to select the performance (good or bad) to replace the sensory inputs or finally, manually touching the robot to complete grab or drop an object. 

Scripts dependent on the sandtray requires that the sandtrayGame is running on the sandtray and configured as defined in the release/components/sandtrayServer/app/README.txt: the config file in the sandtray game engine (release/components/sandtrayServer/config/game_engine/settings.ini on the Sandtray itself) needs to point to the ip address of the computer running the app. 

Scripts dependent on inputs from the robot requires that a Nao robot is running and connected, specify the IP and port under release/components/naoInterface/config/naoInterface.ini.
 
WP3 stub generates randomly a good or bad gaze to simulate inputs from sensory subsystem and WP5 stub generates performance according to the sandtray inputs. For some scripts (such as imitation), no performance can be obtained using the Sandtray so the user has to manually use the GUI to provide the good/bad performance information.

Finally, as the pattern game uses another game engine, when switching from the pattern game to another game or vice versa, the app has to be restarted.

Resources required to compile and run the test:
 --systemGUI, sandtrayServer, wp3Stub, naoInterface, actuationSubsystem, scriptManager, selfMonitoringSubsystem, sandtrayEvent, userModel, wp5Stub, attentionReactionSubsystem, deliberativeSubsystem components
 --include/actionMessages.h include/actionOrigin.h include/commonFonctionalities.h include/feedbackMessages.h include/moveSequenceId.h include/naoMessages.h  include/sandtrayMessages.h include/scriptManagerLibrary.h include/scriptMessages.h include/scriptMessages.h include/userListLibrary.h include/waiter.h 
 
To run the test/expected results:
0. Start and configure the robot (according to naoInterface Readme)
1. Run the app, connect the ports
2. Start a sandtrayGame if required (with the correct computer ip)
3. Press create session on the GUI
4. Press start script

