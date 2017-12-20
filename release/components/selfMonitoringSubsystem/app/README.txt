
selfMonitoringSubsytemTEST

Author: James Kennedy
Email: james.kennedy@plymouth.ac.uk
Institution: Plymouth University
Date: 24/09/2016
======================

This test application demonstrates the use of the selfMonitoringSubsystem component (part of WP6), which is the hub of the communication between the systemGUI, userModel, deliberativeSubsystem, attentionReactionSubsystem, and scriptManager components.

Resources required to run the test:
 --selfMonitoringSubsystem and (/test) selfMonitoringDriver components 
 --/include/userDataLibrary.h and /include/userListLibrary.h centralised shared headers

To run the test/expected results:
1. Run the app, connect the ports
2. Start the test procedure:
	$ yarp rpc /selfMonitoringDriver
	$ test
	
The test will simulate the communication that would happen between the selfMonitoringSubsystem and all other components (functionality encapsulated in selfMonitoringDriver component) when an interaction is being run.

The process will be documented through readouts in the console, i.e., where other components would take action, a readout is used instead. In summary, these steps include:
1. creating a new intervention 'session' (includes loading a user from userModel)
2. loading a script (which turns off attention and reaction in attentionReactionSubsystem)
3. simulating the autonomous running of 3 script steps (includes sending updates to the systemGUI as we go, and piping messages from deliberativeSubsystem/scriptManager to systemGUI)
4. simulating inputs from WP5 (child engagement, performance and behaviour)
5. simulate input from actuationSubsystem
6. simulate input from getInterventionStatus primitive
7. sending the data from the intervention to the userModel to save it

Parameters in the configuration file (selfMonitoringSubsystem.ini):
 _getChildBehaviourIn /getChildBehaviour:i
 _getChildPerformanceIn /getChildPerformance:i
 _engagementFlagIn /engagementFlag:i
 _actionFeedbackIn /actionFeedback:i
 _getFallingInterruptionIn /fallingInterruption:i
 _getInterventionStatusIn /getInterventionStatus:i
 _deliberativeFeedbackIn /deliberativeFeedback:i
 _interventionCommandIn /interventionCommand:i
 _sensorySummaryIn /sensorySummary:i
 _suggestedActionIn /suggestedAction:i
 _selectedBySupervisorIn /selectedBySupervisor:i
 _therapistCommandIn /therapistCommand:i
 _userDataIn /userData:i
 _affectiveStateOut /affectiveState:o
 _attentionSwitchOffOut /attentionSwitchOff:o
 _reactionSwitchOffOut /reactionSwitchOff:o
 _therapistGazeCommandOut /therapistGazeCommand:o
 _selectedActionOut /selectedAction:o
 _userDelibOut /userDelib:o
 _proposedToSupervisorOut /proposedToSupervisor:o
 _smsSummaryOut /smsSummary:o
 _startStopOut /startStop:o
 _updatedDataOut /updatedData:o
 _commandSuccessOut /commandSuccess:o

These parameters are used to change the component's port names.

System architecture ports being used:
getChildBehaviourIn getChildBehaviour:i
getChildPerformanceIn getChildPerformance:i
engagementFlagIn engagementFlag:i
getInterventionStatusIn getInterventionStatus:i

Other ports being used:
actionFeedbackIn actionFeedback:i
getFallingInterruptionIn fallingInterruption:i
deliberativeFeedbackIn deliberativeFeedback:i
interventionCommandIn interventionCommand:i
sensorySummaryIn sensorySummary:i
suggestedActionIn suggestedAction:i
selectedBySupervisorIn selectedBySupervisor:i
therapistCommandIn therapistCommand:i
userDataIn userData:i

affectiveStateOut affectiveState:o
attentionSwitchOffOut attentionSwitchOff:o
reactionSwitchOffOut reactionSwitchOff:o
therapistGazeCommandOut therapistGazeCommand:o
selectedActionOut selectedAction:o
userDelibOut userDelib:o
proposedToSupervisorOut proposedToSupervisor:o
smsSummaryOut smsSummary:o
startStopOut startStop:o
updatedDataOut updatedData:o
commandSuccessOut commandSuccess:o

The component configuration file does not contain parameters other than the port names, which need to be at their default value for the TEST to be able to connect ports.
