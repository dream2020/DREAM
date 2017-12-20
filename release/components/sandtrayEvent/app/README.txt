sandtrayEventTEST

Author: James Kennedy
Email: james.kennedy@plymouth.ac.uk
Institution: Plymouth University
Date: 06/07/2016
======================

PLEASE NOTE THAT THIS TEST APP IS IDENTICAL TO THE SANDTRAYSERVER TEST -- THEY ARE PART OF THE SAME SUBSYSTEM (THIS IS HERE FOR COMPLETION/CONFORMITY TO STANDARDS ONLY)
This test application demonstrates the use of the the sandtray subsystem (part of WP6), which consists of the sandtrayServer component, the sandtrayEvent component and the sandtray game engine code (found in /sandtrayServer/config)

Resources required to run the test:
 --sandtrayServer, sandtrayEvent, and (/test) sandtrayServerDriver components
 --include/sandtrayMessages.h (centralised communication header for passing human-readable commands to the sandtray from other components)
 --sandtray game engine code (pre-compiled)
 --(easier on a machine with 2 monitors; the primary one having a resolution of 1920x1080. Otherwise, the game engine can run on another machine - copy the whole folder, but the settings.ini within the game engine folder needs to have the ServerIP parameter updated to point at the machine running YARP).

To run the test/expected results:
1. Run the app, connect the ports
2. Launch the sandtray game engine (qtsandtray.exe from the sandtrayServer/config/game_engine/release/ folder)
3. Start the test procedure:
	$ yarp rpc /sandtrayServerDriver
	$ test

[
an extended test can be run using the other game engine, as follows:
1. Run the app, connect the ports
2. Launch the sandtray pattern game engine (qtsandtray.exe from the sandtrayServer/config/pattern_game/release/ folder)
3. Start the test procedure:
	$ yarp rpc /sandtrayServerDriver
	$ testpattern
	
this test will start running and at one point will wait for the user to make a categorisation (click on square or star and drag to next slot in sequence) before continuing
]
	
The test will step through a series of commands on the game engine, with a 3 second delay in between each.
A series of actions should occur on the game engine (documented through readouts in the console).
Some events do not create actions in the game engine, but return data, which will be displayed in the console. In the final system, other components will use this data to perform tasks.
The aim of the test is to demonstrate that the functionality of the sandtray can be manipulated from other YARP components via the sandtray subsystem. The sandtrayServerDriver stands in for commands coming from the actuation subsystem and the deliberative subsystem (which orchestrate the robot movements).

System architecture ports being used:
 --None

Other ports being used:
/sandtrayServer/sandtrayCommand:i
/sandtrayServer/sandtrayReturn:o

/sandtrayEvent/sandtrayEvent:o

/sandtrayServerDriver/sandtrayEvent:i
/sandtrayServerDriver/sandtrayReturn:i
/sandtrayServerDriver/sandtrayCommand:o

The component configuration file does not contain parameters other than the port names, which need to be at their default value for the TEST to be able to connect ports.