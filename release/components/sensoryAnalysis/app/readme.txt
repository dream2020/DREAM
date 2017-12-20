Sensory Analysis Test
======================

This application is to test the functionalities for SensoryAnalysis component in on-line environment. The functionality of this component can be described as follows,
1) display 3 RGB camera iamges, 2 Kinect RGB images and 2 Kinect Depth images in GUI
2) display analysis result on GUI
3) save video, voice and analysis result into files
5) save the yarp output result to sensoryInterpretationLogger.log in the component configure folder

System architecture ports being used:
  
checkMutualGazeOutputPort         | /checkMutualGaze:o
getArmAngleOutputPort             | /getArmAngle:o
getBodyOutputPort                 | /getBody:o
getBodyPoseOutputPort             | /getBodyPose:o
getEyeGazeInputPort               | /getEyeGaze:i
getEyeGazeOutputPort              | /getEyeGaze:o
getEyesOutputPort                 | /getEyes:o
getFacesOutputPort                | /getFaces:o
getGripLocationInputPort          | /getGripLocation:i
getGripLocationOutputPort         | /getGripLocation:o
getHandsOutputPort                | /getHands:o
getHeadOutputPort                 | /getHead:o
getHeadGazeInputPort              | /getHeadGaze:i
getHeadGazeOutputPort             | /getHeadGaze:o
getObjectsOutputPort              | /getObjects:o
getObjectsInputPort               | /getObjects:i
getObjectTableDistanceInputPort   | /getObjectTableDistance:i
getObjectTableDistanceOutputPort  | /getObjectTableDistance:o
getSoundDirectionInputPort        | /getSoundDirection:i
getSoundDirectionOutputPort       | /getSoundDirection:o
identifyFaceInputPort             | /identifyFace:i
identifyFaceOutputPort            | /identifyFace:o
identifyFaceExpressionInputPort   | /identifyFaceExpression:i
identifyFaceExpressionOutputPort  | /identifyFaceExpression:o
identifyObjectInputPort           | /identifyObject:i
identifyObjectOutputPort          | /identifyObject:o
identifyTrajectoryInputPort       | /identifyTrajectory:i
identifyTrajectoryOutputPort      | /identifyTrajectory:o
identifyVoiceOutputPort           | /identifyVoice:o
recognizeSpeechOutputPort         | /recognizeSpeech:o
trackFaceInputPort                | /trackFace:i
trackFaceOutputPort               | /trackFace:o
trackHandInputPort                | /trackHand:i
trackHandOutputPort               | /trackHand:o
trackObjectInputPort              | /trackObject:i
trackObjectOutputPort             | /trackObject:o 


Resources required to run the test:
 
-- The configuration file for sensoryAnalysis from sensoryAnalysis.ini
-- The configuration file for sensoryInterpretationLogger, sensoryInterpretationLogger.ini
-- sensoryInterpretationLogger, component
	
Parameter setting in the configuration file (sensoryAnalysis.ini):
--recordingPath                   C:/DreamRecordings

The path for recording data. Please make sure the folder is created befor running the app.

Ports being used in the APP:

/sensoryAnalysis/getArmAngle:o
/sensoryInterpretationLogger/getArmAngle:i
/sensoryAnalysis/getBody:o
/sensoryInterpretationLogger/getBody:i
/sensoryAnalysis/getBodyPose:o
/sensoryInterpretationLogger/getBodyPose:i
/sensoryAnalysis/getEyeGaze:o
/sensoryInterpretationLogger/getEyeGaze:i
/sensoryAnalysis/getEyes:o
/sensoryInterpretationLogger/getEyes:i
/sensoryAnalysis/getFaces:o
/sensoryInterpretationLogger/getFaces:i
/sensoryAnalysis/getGripLocation:o
/sensoryInterpretationLogger/getGripLocation:i
/sensoryAnalysis/getHead:o
/sensoryInterpretationLogger/getHead:i
/sensoryAnalysis/getHeadGaze:o
/sensoryInterpretationLogger/getHeadGaze:i
/sensoryAnalysis/getObjects:o
/sensoryInterpretationLogger/getObjects:i
/sensoryAnalysis/getObjectTableDistance:o
/sensoryInterpretationLogger/getObjectTableDistance:i
/sensoryAnalysis/getSoundDirection:o
/sensoryInterpretationLogger/getSoundDirection:i
/sensoryAnalysis/identifyFace:o
/sensoryInterpretationLogger/identifyFace:i
/sensoryAnalysis/identifyFaceExpression:o
/sensoryInterpretationLogger/identifyFaceExpression:i
/sensoryAnalysis/identifyObject:o
/sensoryInterpretationLogger/identifyObject:i
/sensoryAnalysis/identifyTrajectory:o
/sensoryInterpretationLogger/identifyTrajectory:i
/sensoryAnalysis/identifyVoice:o
/sensoryInterpretationLogger/identifyVoice:i
/sensoryAnalysis/trackFace:o
/sensoryInterpretationLogger/trackFace:i
/sensoryAnalysis/trackObject:o
/sensoryInterpretationLogger/trackObject:i




Yinfeng Fang
15/02/2016


