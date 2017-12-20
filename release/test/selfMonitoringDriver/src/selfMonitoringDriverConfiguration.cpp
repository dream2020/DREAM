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
  * 15/09/2016 Version 1.0: ... (James Kennedy)
  */ 

#include "selfMonitoringDriver.h"
#include "selfMonitoringDriverYarpInterface.h"
#include "selfMonitoringDriverController.h"

bool SelfMonitoringDriver::configure(yarp::os::ResourceFinder &rf){
   /* get and set the module name */
   _moduleName = rf.check("name", Value("selfMonitoringDriver"), "module name (string)").asString();

   setName(_moduleName.c_str());

   /* get the name of the input and output ports */
   _proposedToSupervisorInName = "/" +  getName(rf.check("_proposedToSupervisorIn", Value("/proposedToSupervisor:i"), "Input proposedToSupervisor port (string)").asString());
   _smsSummaryInName = "/" +  getName(rf.check("_smsSummaryIn", Value("/smsSummary:i"), "Input smsSummary port (string)").asString());
   _updatedDataInName = "/" +  getName(rf.check("_updatedDataIn", Value("/updatedData:i"), "Input updatedData port (string)").asString());
   _affectiveStateInName = "/" +  getName(rf.check("_affectiveStateIn", Value("/affectiveState:i"), "Input affectiveState port (string)").asString());
   _attentionSwitchOffInName = "/" +  getName(rf.check("_attentionSwitchOffIn", Value("/attentionSwitchOff:i"), "Input attentionSwitchOff port (string)").asString());
   _reactionSwitchOffInName = "/" +  getName(rf.check("_reactionSwitchOffIn", Value("/reactionSwitchOff:i"), "Input reactionSwitchOff port (string)").asString());
   _therapistGazeCommandInName = "/" +  getName(rf.check("_therapistGazeCommandIn", Value("/therapistGazeCommand:i"), "Input therapistGazeCommand port (string)").asString());
   _selectedActionInName = "/" +  getName(rf.check("_selectedActionIn", Value("/selectedAction:i"), "Input selectedAction port (string)").asString());
   _userDelibInName = "/" +  getName(rf.check("_userDelibIn", Value("/userDelib:i"), "Input userDelib port (string)").asString());
   _startStopInName = "/" +  getName(rf.check("_startStopIn", Value("/startStop:i"), "Input startStop port (string)").asString());
   _commandSuccessInName = "/" + getName(rf.check("_commandSuccessIn", Value("/commandSuccess:i"), "Input commandSuccess port (string)").asString());

   _getChildBehaviourOutName = "/" +  getName(rf.check("_getChildBehaviourOut", Value("/getChildBehaviour:o"), "output getChildBehaviour port (string)").asString());
   _getChildPerformanceOutName = "/" +  getName(rf.check("_getChildPerformanceOut", Value("/getChildPerformance:o"), "output getChildPerformance port (string)").asString());
   _engagementFlagOutName = "/" +  getName(rf.check("_engagementFlagOut", Value("/engagementFlag:o"), "output engagementFlag port (string)").asString());
   _selectedBySupervisorOutName = "/" +  getName(rf.check("_selectedBySupervisorOut", Value("/selectedBySupervisor:o"), "output selectedBySupervisor port (string)").asString());
   _therapistCommandOutName = "/" +  getName(rf.check("_therapistCommandOut", Value("/therapistCommand:o"), "output therapistCommand port (string)").asString());
   _deliberativeFeedbackOutName = "/" +  getName(rf.check("_deliberativeFeedbackOut", Value("/deliberativeFeedback:o"), "output deliberativeFeedback port (string)").asString());
   _getInterventionStatusOutName = "/" +  getName(rf.check("_getInterventionStatusOut", Value("/getInterventionStatus:o"), "output getInterventionStatus port (string)").asString());
   _sensorySummaryOutName = "/" +  getName(rf.check("_sensorySummaryOut", Value("/sensorySummary:o"), "output sensorySummary port (string)").asString());
   _suggestedActionOutName = "/" +  getName(rf.check("_suggestedActionOut", Value("/suggestedAction:o"), "output suggestedAction port (string)").asString());
   _actionFeedbackOutName = "/" +  getName(rf.check("_actionFeedbackOut", Value("/actionFeedback:o"), "output actionFeedback port (string)").asString());
   _userDataOutName = "/" +  getName(rf.check("_userDataOut", Value("/userData:o"), "output userData port (string)").asString());
   _fallingInterruptionOutName = "/" +  getName(rf.check("_fallingInterruptionOut", Value("/fallingInterruption:o"), "output fallingInterruption port (string)").asString());
   _interventionCommandOutName = "/" + getName(rf.check("_interventionCommandOut", Value("/interventionCommand:o"), "output interventionCommand port (string)").asString());


   /* open the ports */
   if (!_proposedToSupervisorIn.open(_proposedToSupervisorInName.c_str())){
      std::cout << getName() << ": unable to open port " << _proposedToSupervisorInName << std::endl;
      return false;
   }
   if (!_smsSummaryIn.open(_smsSummaryInName.c_str())){
      std::cout << getName() << ": unable to open port " << _smsSummaryInName << std::endl;
      return false;
   }
   if (!_updatedDataIn.open(_updatedDataInName.c_str())){
      std::cout << getName() << ": unable to open port " << _updatedDataInName << std::endl;
      return false;
   }
   if (!_affectiveStateIn.open(_affectiveStateInName.c_str())){
      std::cout << getName() << ": unable to open port " << _affectiveStateInName << std::endl;
      return false;
   }
   if (!_attentionSwitchOffIn.open(_attentionSwitchOffInName.c_str())){
      std::cout << getName() << ": unable to open port " << _attentionSwitchOffInName << std::endl;
      return false;
   }
   if (!_reactionSwitchOffIn.open(_reactionSwitchOffInName.c_str())){
      std::cout << getName() << ": unable to open port " << _reactionSwitchOffInName << std::endl;
      return false;
   }
   if (!_therapistGazeCommandIn.open(_therapistGazeCommandInName.c_str())){
      std::cout << getName() << ": unable to open port " << _therapistGazeCommandInName << std::endl;
      return false;
   }
   if (!_selectedActionIn.open(_selectedActionInName.c_str())){
      std::cout << getName() << ": unable to open port " << _selectedActionInName << std::endl;
      return false;
   }
   if (!_userDelibIn.open(_userDelibInName.c_str())){
      std::cout << getName() << ": unable to open port " << _userDelibInName << std::endl;
      return false;
   }
   if (!_startStopIn.open(_startStopInName.c_str())){
      std::cout << getName() << ": unable to open port " << _startStopInName << std::endl;
      return false;
   }
   if (!_commandSuccessIn.open(_commandSuccessInName.c_str())) {
	   std::cout << getName() << ": unable to open port " << _commandSuccessInName << std::endl;
	   return false;
   }

   if (!_getChildBehaviourOut.open(_getChildBehaviourOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getChildBehaviourOutName << std::endl;
      return false;
   }
   if (!_getChildPerformanceOut.open(_getChildPerformanceOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getChildPerformanceOutName << std::endl;
      return false;
   }
   if (!_engagementFlagOut.open(_engagementFlagOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _engagementFlagOutName << std::endl;
      return false;
   }
   if (!_selectedBySupervisorOut.open(_selectedBySupervisorOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _selectedBySupervisorOutName << std::endl;
      return false;
   }
   if (!_therapistCommandOut.open(_therapistCommandOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _therapistCommandOutName << std::endl;
      return false;
   }
   if (!_deliberativeFeedbackOut.open(_deliberativeFeedbackOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _deliberativeFeedbackOutName << std::endl;
      return false;
   }
   if (!_getInterventionStatusOut.open(_getInterventionStatusOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _getInterventionStatusOutName << std::endl;
      return false;
   }
   if (!_sensorySummaryOut.open(_sensorySummaryOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _sensorySummaryOutName << std::endl;
      return false;
   }
   if (!_suggestedActionOut.open(_suggestedActionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _suggestedActionOutName << std::endl;
      return false;
   }
   if (!_actionFeedbackOut.open(_actionFeedbackOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _actionFeedbackOutName << std::endl;
      return false;
   }
   if (!_userDataOut.open(_userDataOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _userDataOutName << std::endl;
      return false;
   }
   if (!_fallingInterruptionOut.open(_fallingInterruptionOutName.c_str())){
      std::cout << getName() << ":unable to open port " << _fallingInterruptionOutName << std::endl;
      return false;
   }
   if (!_interventionCommandOut.open(_interventionCommandOutName.c_str())) {
       std::cout << getName() << ":unable to open port " << _interventionCommandOutName << std::endl;
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
   _selfMonitoringDriverYarpInterface = new SelfMonitoringDriverYarpInterface();

   /* Create the main class: Controller */ 
   _selfMonitoringDriverController = new SelfMonitoringDriverController(_selfMonitoringDriverYarpInterface);
   
   /* create the computation thread and pass pointers to the input and output ports and the yarpInterface*/
   _selfMonitoringDriverThread = new SelfMonitoringDriverThread(&_proposedToSupervisorIn, &_smsSummaryIn, &_updatedDataIn, &_affectiveStateIn, &_attentionSwitchOffIn, &_reactionSwitchOffIn, &_therapistGazeCommandIn, &_selectedActionIn, &_userDelibIn, &_startStopIn, &_commandSuccessIn, &_getChildBehaviourOut, &_getChildPerformanceOut, &_engagementFlagOut, &_selectedBySupervisorOut, &_therapistCommandOut, &_deliberativeFeedbackOut, &_getInterventionStatusOut, &_sensorySummaryOut, &_suggestedActionOut, &_actionFeedbackOut, &_userDataOut, &_fallingInterruptionOut, &_interventionCommandOut);
   _selfMonitoringDriverThread->setYarpInterface(_selfMonitoringDriverYarpInterface);
   
   /* Init the YarpInterface to allow the communication */
   
   _selfMonitoringDriverYarpInterface->init(_selfMonitoringDriverThread, _selfMonitoringDriverController);
   
   _selfMonitoringDriverThread->start();

   return true;
}

bool SelfMonitoringDriver::interruptModule(){
   _proposedToSupervisorIn.interrupt();
   _smsSummaryIn.interrupt();
   _updatedDataIn.interrupt();
   _affectiveStateIn.interrupt();
   _attentionSwitchOffIn.interrupt();
   _reactionSwitchOffIn.interrupt();
   _therapistGazeCommandIn.interrupt();
   _selectedActionIn.interrupt();
   _userDelibIn.interrupt();
   _startStopIn.interrupt();
   _commandSuccessIn.interrupt();
   _getChildBehaviourOut.interrupt();
   _getChildPerformanceOut.interrupt();
   _engagementFlagOut.interrupt();
   _selectedBySupervisorOut.interrupt();
   _therapistCommandOut.interrupt();
   _deliberativeFeedbackOut.interrupt();
   _getInterventionStatusOut.interrupt();
   _sensorySummaryOut.interrupt();
   _suggestedActionOut.interrupt();
   _actionFeedbackOut.interrupt();
   _userDataOut.interrupt();
   _fallingInterruptionOut.interrupt();
   _interventionCommandOut.interrupt();
   _handlerPort.interrupt();

   return true;
}

bool SelfMonitoringDriver::close(){
   /* close the ports */
   _proposedToSupervisorIn.close();
   _smsSummaryIn.close();
   _updatedDataIn.close();
   _affectiveStateIn.close();
   _attentionSwitchOffIn.close();
   _reactionSwitchOffIn.close();
   _therapistGazeCommandIn.close();
   _selectedActionIn.close();
   _userDelibIn.close();
   _startStopIn.close();
   _commandSuccessIn.close();
   _getChildBehaviourOut.close();
   _getChildPerformanceOut.close();
   _engagementFlagOut.close();
   _selectedBySupervisorOut.close();
   _therapistCommandOut.close();
   _deliberativeFeedbackOut.close();
   _getInterventionStatusOut.close();
   _sensorySummaryOut.close();
   _suggestedActionOut.close();
   _actionFeedbackOut.close();
   _userDataOut.close();
   _fallingInterruptionOut.close();
   _interventionCommandOut.close();
   _handlerPort.close();

   /* stop the computation thread */
   _selfMonitoringDriverThread->stop();

   return true;
}

bool SelfMonitoringDriver::respond(const Bottle &command, Bottle &reply){
    reply.clear();

    if (command.get(0).asString() == "help") {
        reply.addString("help: test");
    }
    else {
        reply.addString(command.get(0).asString() + " being executed...");

        if (command.get(0).asString() == "test") { _selfMonitoringDriverController->test(); }
        //else if (command.get(0).asString() == "command") { _selfMonitoringDriverController->function(); }
        else { reply.addString("no command recognised"); }
    }

    return true;
}

bool SelfMonitoringDriver::updateModule(){
   return true;
}

double SelfMonitoringDriver::getPeriod(){
   return 0.1;
}
