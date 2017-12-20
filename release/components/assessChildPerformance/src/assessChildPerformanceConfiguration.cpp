/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Serge Thill, University of Skövde 
 * Email:   serge.thill@his.se 
 * Website: www.dream2020.eu 
 * 
 * This file is part of DREAM.
 * 
 * DREAM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * DREAM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with DREAM.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 /* 
  * Audit Trail
  * -----------
  * 08/11/2016 Version 1.0: ... (Serge Thill)
  */ 

#include "assessChildPerformance.h"
#include "assessChildPerformanceYarpInterface.h"
#include "assessChildPerformanceController.h"


bool AssessChildPerformance::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("assessChildPerformance"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _identifyVoiceInName = "/" +  getName(rf.check("_identifyVoiceIn", Value("/identifyVoice:i"), "Input identifyVoice port (string)").asString());
   _getEyeGazeInName = "/" +  getName(rf.check("_getEyeGazeIn", Value("/getEyeGaze:i"), "Input getEyeGaze port (string)").asString());
   _getBodyPoseInName = "/" +  getName(rf.check("_getBodyPoseIn", Value("/getBodyPose:i"), "Input getBodyPose port (string)").asString());
   _identifyTrajectoryInName = "/" +  getName(rf.check("_identifyTrajectoryIn", Value("/identifyTrajectory:i"), "Input identifyTrajectory port (string)").asString());
   _getInterventionStatusInName = "/" +  getName(rf.check("_getInterventionStatusIn", Value("/getInterventionStatus:i"), "Input getInterventionStatus port (string)").asString());
   _interactionEventInName = "/" +  getName(rf.check("_interactionEventIn", Value("/interactionEvent:i"), "Input interactionEvent port (string)").asString());

   _getChildBehaviourOutName = "/" +  getName(rf.check("_getChildBehaviourOut", Value("/getChildBehaviour:o"), "output getChildBehaviour port (string)").asString());
   _identifyTrajectoryOutName = "/" +  getName(rf.check("_identifyTrajectoryOut", Value("/identifyTrajectory:o"), "output identifyTrajectory port (string)").asString());
   _getChildPerformanceOutName = "/" +  getName(rf.check("_getChildPerformanceOut", Value("/getChildPerformance:o"), "output getChildPerformance port (string)").asString());

   /* open the ports */
   if (!_identifyVoiceIn.open(_identifyVoiceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _identifyVoiceInName << std::endl;
      return false;
   }
   if (!_getEyeGazeIn.open(_getEyeGazeInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getEyeGazeInName << std::endl;
      return false;
   }
   if (!_getBodyPoseIn.open(_getBodyPoseInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getBodyPoseInName << std::endl;
      return false;
   }
   if (!_identifyTrajectoryIn.open(_identifyTrajectoryInName.c_str())){
      std::cout << getName() << ": unable to open port " << _identifyTrajectoryInName << std::endl;
      return false;
   }
   if (!_getInterventionStatusIn.open(_getInterventionStatusInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getInterventionStatusInName << std::endl;
      return false;
   }
   if (!_interactionEventIn.open(_interactionEventInName.c_str())){
      std::cout << getName() << ": unable to open port " << _interactionEventInName << std::endl;
      return false;
   }

   if (!_getChildBehaviourOut.open(_getChildBehaviourOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getChildBehaviourOutName << std::endl;
      return false;
   }
   if (!_identifyTrajectoryOut.open(_identifyTrajectoryOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _identifyTrajectoryOutName << std::endl;
      return false;
   }
   if (!_getChildPerformanceOut.open(_getChildPerformanceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getChildPerformanceOutName << std::endl;
      return false;
   }

   /* create the handler port */
   _handlerPortName = "/";
   _handlerPortName += getName();

   if (!_handlerPort.open(_handlerPortName.c_str())){
      std::cout << getName() << ":unable to open port " << _handlerPortName << std::endl;
      return false;
   }
   
   attach(_handlerPort);

   /* Create the YarpInterface used to communicate between the main code in Controller and the other components through the thread class */
   _assessChildPerformanceYarpInterface = new AssessChildPerformanceYarpInterface();

   /* Create the main class: Controller */ 
   _assessChildPerformanceController = new AssessChildPerformanceController(_assessChildPerformanceYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _assessChildPerformanceThread = new AssessChildPerformanceThread(&_identifyVoiceIn, &_getEyeGazeIn, &_getBodyPoseIn, &_identifyTrajectoryIn, &_getInterventionStatusIn, &_interactionEventIn, &_getChildBehaviourOut, &_identifyTrajectoryOut, &_getChildPerformanceOut);
   _assessChildPerformanceThread->setYarpInterface(_assessChildPerformanceYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _assessChildPerformanceYarpInterface->init(_assessChildPerformanceThread, _assessChildPerformanceController);
   
   _assessChildPerformanceThread->start();


   /* Parameters that can be tweaked */
   _assessChildPerformanceController->sandtray_edge = rf.check("sandtray_edge", Value(0), "z coordinate of the outer sandtray edge").asDouble();
   _assessChildPerformanceController->sandtray_offset = rf.check("sandtray_offset", Value(-700), "difference in sandtray height to kinect").asDouble();
   _assessChildPerformanceController->flip_lr = rf.check("flip_lr", Value(1), "invert definition of left/right").asInt();

   _assessChildPerformanceController->traj_min_wait = rf.check("traj_min_wait", Value(1000), "Min time trajectories vectors are integrated before assessment begins (ms)").asDouble();

   _assessChildPerformanceController->wave_thr = rf.check("wave_thr", Value(5), "min. count of instances that trajectory must be observed before it counts as present").asDouble();
   _assessChildPerformanceController->cover_thr = rf.check("cover_thr", Value(5), "min. count of instances that trajectory must be observed before it counts as present").asDouble();
   _assessChildPerformanceController->hands_head_thr = rf.check("hands_head_thr", Value(5), "min. count of instances that trajectory must be observed before it counts as present").asDouble();
   _assessChildPerformanceController->fly_thr = rf.check("fly_thr", Value(5), "min. count of instances that trajectory must be observed before it counts as present").asDouble();
   _assessChildPerformanceController->drive_thr = rf.check("drive_thr", Value(5), "min. count of instances that trajectory must be observed before it counts as present").asDouble();
   _assessChildPerformanceController->smell_thr = rf.check("smell_thr", Value(5), "min. count of instances that trajectory must be observed before it counts as present").asDouble();
   _assessChildPerformanceController->traj1_thr = rf.check("traj1_thr", Value(5), "min. count of instances that trajectory must be observed before it counts as present").asDouble();
   _assessChildPerformanceController->traj2_thr = rf.check("traj2_thr", Value(5), "min. count of instances that trajectory must be observed before it counts as present").asDouble();
   _assessChildPerformanceController->traj3_thr = rf.check("traj3_thr", Value(5), "min. count of instances that trajectory must be observed before it counts as present").asDouble();
   _assessChildPerformanceController->traj4_thr = rf.check("traj4_thr", Value(5), "min. count of instances that trajectory must be observed before it counts as present").asDouble();
   _assessChildPerformanceController->traj5_thr = rf.check("knocking_thr", Value(5), "min. count of instances that trajectory must be observed before it counts as present").asDouble();

   return true;
}

bool AssessChildPerformance::interruptModule(){
   _identifyVoiceIn.interrupt();
   _getEyeGazeIn.interrupt();
   _getBodyPoseIn.interrupt();
   _identifyTrajectoryIn.interrupt();
   _getInterventionStatusIn.interrupt();
   _interactionEventIn.interrupt();
   _getChildBehaviourOut.interrupt();
   _identifyTrajectoryOut.interrupt();
   _getChildPerformanceOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool AssessChildPerformance::close(){
   /* close the ports */
   _identifyVoiceIn.close();
   _getEyeGazeIn.close();
   _getBodyPoseIn.close();
   _identifyTrajectoryIn.close();
   _getInterventionStatusIn.close();
   _interactionEventIn.close();
   _getChildBehaviourOut.close();
   _identifyTrajectoryOut.close();
   _getChildPerformanceOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _assessChildPerformanceThread->stop();

   return true;
}

bool AssessChildPerformance::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool AssessChildPerformance::updateModule(){
   return true;
}

double AssessChildPerformance::getPeriod(){
   return 0.1;
}
