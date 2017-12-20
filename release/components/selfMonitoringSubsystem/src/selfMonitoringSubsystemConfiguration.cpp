/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  James Kennedy, Plymouth University 
 * Email:   james.kennedy@plymouth.ac.uk 
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
  * 04/07/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "selfMonitoringSubsystem.h"
#include "selfMonitoringSubsystemYarpInterface.h"
#include "selfMonitoringSubsystemController.h"

bool SelfMonitoringSubsystem::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("selfMonitoringSubsystem"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _getChildBehaviourInName = "/" +  getName(rf.check("_getChildBehaviourIn", Value("/getChildBehaviour:i"), "Input getChildBehaviour port (string)").asString());
   _getChildPerformanceInName = "/" +  getName(rf.check("_getChildPerformanceIn", Value("/getChildPerformance:i"), "Input getChildPerformance port (string)").asString());
   _engagementFlagInName = "/" +  getName(rf.check("_engagementFlagIn", Value("/engagementFlag:i"), "Input engagementFlag port (string)").asString());
   _actionFeedbackInName = "/" +  getName(rf.check("_actionFeedbackIn", Value("/actionFeedback:i"), "Input actionFeedback port (string)").asString());
   _fallingInterruptionInName = "/" +  getName(rf.check("_fallingInterruptionIn", Value("/fallingInterruption:i"), "Input fallingInterruption port (string)").asString());
   _getInterventionStatusInName = "/" +  getName(rf.check("_getInterventionStatusIn", Value("/getInterventionStatus:i"), "Input getInterventionStatus port (string)").asString());
   _interventionCommandInName = "/" + getName(rf.check("_interventionCommandIn", Value("/interventionCommand:i"), "Input interventionCommand port (string)").asString());
   _deliberativeFeedbackInName = "/" +  getName(rf.check("_deliberativeFeedbackIn", Value("/deliberativeFeedback:i"), "Input deliberativeFeedback port (string)").asString());
   _sensorySummaryInName = "/" +  getName(rf.check("_sensorySummaryIn", Value("/sensorySummary:i"), "Input sensorySummary port (string)").asString());
   _suggestedActionInName = "/" +  getName(rf.check("_suggestedActionIn", Value("/suggestedAction:i"), "Input suggestedAction port (string)").asString());
   _selectedBySupervisorInName = "/" +  getName(rf.check("_selectedBySupervisorIn", Value("/selectedBySupervisor:i"), "Input selectedBySupervisor port (string)").asString());
   _therapistCommandInName = "/" +  getName(rf.check("_therapistCommandIn", Value("/therapistCommand:i"), "Input therapistCommand port (string)").asString());
   _userDataInName = "/" +  getName(rf.check("_userDataIn", Value("/userData:i"), "Input userData port (string)").asString());

   _affectiveStateOutName = "/" +  getName(rf.check("_affectiveStateOut", Value("/affectiveState:o"), "output affectiveState port (string)").asString());
   _attentionSwitchOffOutName = "/" +  getName(rf.check("_attentionSwitchOffOut", Value("/attentionSwitchOff:o"), "output attentionSwitchOff port (string)").asString());
   _reactionSwitchOffOutName = "/" +  getName(rf.check("_reactionSwitchOffOut", Value("/reactionSwitchOff:o"), "output reactionSwitchOff port (string)").asString());
   _therapistGazeCommandOutName = "/" +  getName(rf.check("_therapistGazeCommandOut", Value("/therapistGazeCommand:o"), "output therapistGazeCommand port (string)").asString());
   _selectedActionOutName = "/" +  getName(rf.check("_selectedActionOut", Value("/selectedAction:o"), "output selectedAction port (string)").asString());
   _userDelibOutName = "/" +  getName(rf.check("_userDelibOut", Value("/userDelib:o"), "output userDelib port (string)").asString());
   _proposedToSupervisorOutName = "/" +  getName(rf.check("_proposedToSupervisorOut", Value("/proposedToSupervisor:o"), "output proposedToSupervisor port (string)").asString());
   _smsSummaryOutName = "/" +  getName(rf.check("_smsSummaryOut", Value("/smsSummary:o"), "output smsSummary port (string)").asString());
   _startStopOutName = "/" +  getName(rf.check("_startStopOut", Value("/startStop:o"), "output startStop port (string)").asString());
   _updatedDataOutName = "/" +  getName(rf.check("_updatedDataOut", Value("/updatedData:o"), "output updatedData port (string)").asString());
   _commandSuccessOutName = "/" +  getName(rf.check("_commandSuccessOut", Value("/commandSuccessOut:o"), "output updatedData port (string)").asString());

   /* open the ports */
   if (!_getChildBehaviourIn.open(_getChildBehaviourInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getChildBehaviourInName << std::endl;
      return false;
   }
   if (!_getChildPerformanceIn.open(_getChildPerformanceInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getChildPerformanceInName << std::endl;
      return false;
   }
   if (!_engagementFlagIn.open(_engagementFlagInName.c_str())){
      std::cout << getName() << ": unable to open port " << _engagementFlagInName << std::endl;
      return false;
   }
   if (!_actionFeedbackIn.open(_actionFeedbackInName.c_str())){
      std::cout << getName() << ": unable to open port " << _actionFeedbackInName << std::endl;
      return false;
   }
   if (!_interventionCommandIn.open(_interventionCommandInName.c_str())) {
       std::cout << getName() << ": unable to open port " << _interventionCommandInName << std::endl;
       return false;
   }
   if (!_fallingInterruptionIn.open(_fallingInterruptionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _fallingInterruptionInName << std::endl;
      return false;
   }
   if (!_getInterventionStatusIn.open(_getInterventionStatusInName.c_str())){
      std::cout << getName() << ": unable to open port " << _getInterventionStatusInName << std::endl;
      return false;
   }
   if (!_deliberativeFeedbackIn.open(_deliberativeFeedbackInName.c_str())){
      std::cout << getName() << ": unable to open port " << _deliberativeFeedbackInName << std::endl;
      return false;
   }
   if (!_sensorySummaryIn.open(_sensorySummaryInName.c_str())){
      std::cout << getName() << ": unable to open port " << _sensorySummaryInName << std::endl;
      return false;
   }
   if (!_suggestedActionIn.open(_suggestedActionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _suggestedActionInName << std::endl;
      return false;
   }
   if (!_selectedBySupervisorIn.open(_selectedBySupervisorInName.c_str())){
      std::cout << getName() << ": unable to open port " << _selectedBySupervisorInName << std::endl;
      return false;
   }
   if (!_therapistCommandIn.open(_therapistCommandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _therapistCommandInName << std::endl;
      return false;
   }
   if (!_userDataIn.open(_userDataInName.c_str())){
      std::cout << getName() << ": unable to open port " << _userDataInName << std::endl;
      return false;
   }

   if (!_affectiveStateOut.open(_affectiveStateOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _affectiveStateOutName << std::endl;
      return false;
   }
   if (!_attentionSwitchOffOut.open(_attentionSwitchOffOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _attentionSwitchOffOutName << std::endl;
      return false;
   }
   if (!_reactionSwitchOffOut.open(_reactionSwitchOffOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _reactionSwitchOffOutName << std::endl;
      return false;
   }
   if (!_therapistGazeCommandOut.open(_therapistGazeCommandOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _therapistGazeCommandOutName << std::endl;
      return false;
   }
   if (!_selectedActionOut.open(_selectedActionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _selectedActionOutName << std::endl;
      return false;
   }
   if (!_userDelibOut.open(_userDelibOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _userDelibOutName << std::endl;
      return false;
   }
   if (!_proposedToSupervisorOut.open(_proposedToSupervisorOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _proposedToSupervisorOutName << std::endl;
      return false;
   }
   if (!_smsSummaryOut.open(_smsSummaryOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _smsSummaryOutName << std::endl;
      return false;
   }
   if (!_startStopOut.open(_startStopOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _startStopOutName << std::endl;
      return false;
   }
   if (!_updatedDataOut.open(_updatedDataOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _updatedDataOutName << std::endl;
      return false;
   }
   if (!_commandSuccessOut.open(_commandSuccessOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _commandSuccessOutName << std::endl;
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
   _selfMonitoringSubsystemYarpInterface = new SelfMonitoringSubsystemYarpInterface();

   /* Create the main class: Controller */
   string sContext = rf.getContextPath();
   _selfMonitoringSubsystemController = new SelfMonitoringSubsystemController(_selfMonitoringSubsystemYarpInterface, sContext);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _selfMonitoringSubsystemThread = new SelfMonitoringSubsystemThread(&_getChildBehaviourIn, &_getChildPerformanceIn, &_engagementFlagIn, &_actionFeedbackIn, &_fallingInterruptionIn, &_getInterventionStatusIn, &_deliberativeFeedbackIn, &_sensorySummaryIn, &_suggestedActionIn, &_selectedBySupervisorIn, &_therapistCommandIn, &_userDataIn, &_interventionCommandIn, &_affectiveStateOut, &_attentionSwitchOffOut, &_reactionSwitchOffOut, &_therapistGazeCommandOut, &_selectedActionOut, &_userDelibOut, &_proposedToSupervisorOut, &_smsSummaryOut, &_startStopOut, &_updatedDataOut, &_commandSuccessOut);
   _selfMonitoringSubsystemThread->setYarpInterface(_selfMonitoringSubsystemYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   _selfMonitoringSubsystemYarpInterface->init(_selfMonitoringSubsystemThread, _selfMonitoringSubsystemController);
   
   _selfMonitoringSubsystemThread->start();

   return true;
}

bool SelfMonitoringSubsystem::interruptModule(){
   _getChildBehaviourIn.interrupt();
   _getChildPerformanceIn.interrupt();
   _engagementFlagIn.interrupt();
   _actionFeedbackIn.interrupt();
   _fallingInterruptionIn.interrupt();
   _getInterventionStatusIn.interrupt();
   _deliberativeFeedbackIn.interrupt();
   _sensorySummaryIn.interrupt();
   _suggestedActionIn.interrupt();
   _selectedBySupervisorIn.interrupt();
   _therapistCommandIn.interrupt();
   _userDataIn.interrupt();
   _affectiveStateOut.interrupt();
   _attentionSwitchOffOut.interrupt();
   _reactionSwitchOffOut.interrupt();
   _therapistGazeCommandOut.interrupt();
   _selectedActionOut.interrupt();
   _userDelibOut.interrupt();
   _proposedToSupervisorOut.interrupt();
   _smsSummaryOut.interrupt();
   _startStopOut.interrupt();
   _updatedDataOut.interrupt();
   _commandSuccessOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool SelfMonitoringSubsystem::close(){
   /* close the ports */
   _getChildBehaviourIn.close();
   _getChildPerformanceIn.close();
   _engagementFlagIn.close();
   _actionFeedbackIn.close();
   _fallingInterruptionIn.close();
   _getInterventionStatusIn.close();
   _deliberativeFeedbackIn.close();
   _sensorySummaryIn.close();
   _suggestedActionIn.close();
   _selectedBySupervisorIn.close();
   _therapistCommandIn.close();
   _userDataIn.close();
   _affectiveStateOut.close();
   _attentionSwitchOffOut.close();
   _reactionSwitchOffOut.close();
   _therapistGazeCommandOut.close();
   _selectedActionOut.close();
   _userDelibOut.close();
   _proposedToSupervisorOut.close();
   _smsSummaryOut.close();
   _startStopOut.close();
   _updatedDataOut.close();
   _commandSuccessOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _selfMonitoringSubsystemThread->stop();

   return true;
}

bool SelfMonitoringSubsystem::respond(const Bottle &command, Bottle &reply){
   return true;
}

bool SelfMonitoringSubsystem::updateModule(){
   return true;
}

double SelfMonitoringSubsystem::getPeriod(){
   return 0.1;
}
