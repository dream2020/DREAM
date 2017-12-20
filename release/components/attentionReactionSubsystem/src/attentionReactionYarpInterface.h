 /** @file attentionReactionInterfaceYarpInterface.h 
  *
  * Version 1.0
  * 
  * 10/08/2016
  *
  *
  * \defgroup DREAM_attentionReactionSubsystem attentionReactionSubsystem
  * 
  * Class used to provided a simpler interface between the main code and the YARP layer.
  * Provide an output function sending data for each port, and an input function called 
  * when a message is received on a port.
  * 
  * \section lib_sec Libraries
  *
  * YARP.
  *
  * <b>Port types </b>
  *
  *
  * \section in_files_sec Input Data Files
  *
  * None
  *
  * \section out_data_sec Output Data Files
  *
  * None
  *
  * \author 
  * 
  * <Pablo Gomez Esteban>, <VUB>  
  * 
  * Copyright (C) 2016 DREAM Consortium
  * 
  */ 
  /* 
 * Copyright (C) 2016 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Pablo Gomez Esteban, VUB
 * Email:   pablo.gomez.esteban@vub.ac.be 
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
 * 10/08/16  First version validated (Pablo Gomez)
 */ 
 
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
#define PI 3.1416 //Defines PI value needed to convert degrees to coordinates

using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;

class AttentionReactionSubsystemThread;
class AttentionReactionController;
class AttentionSystem;
class EyeBlinking;

class AttentionReactionYarpInterface{
private:
   AttentionReactionSubsystemThread* _yarpThread;
   AttentionReactionController* _controller;
   
   int _id;
   
public:
	/* class methods */
	AttentionReactionYarpInterface();
    void init(AttentionReactionSubsystemThread* yarpThread, AttentionReactionController* controller);
	VectorOf<double> az2xyz(double azimuth, double elevation);
    void getFromFaces(VectorOf<double> data);
	void getFromSound(VectorOf<double> data);
    void getFromAttentionBias(VectorOf<double> data);
	void getFromGazeCommand(VectorOf<double> data);
	void getFromAttentionSwitchOff(VectorOf<int> data);
    void getFromIdentifyFaceExpression(VectorOf<int> data);
	/*void getFromBumpers(Bottle data);
    void getFromHeadTactile(Bottle data);
	void getFromHandTactile(Bottle data);*/
    void getFromRobotSensors(Bottle data);
	void getFromReactionSwitchOff(VectorOf<int> data);
	void getFromCheckMutualGaze(VectorOf<int> data);
    void getFromRecognizeSpeech(Bottle data);
    void getFromAffectiveState(Bottle data);
    //void getFromBalance(VectorOf<int> data);
    void getFromActionFeedback(Bottle data);
	
    void sendElicitedAttentionToActuation(double coordX, double coordY, double coordZ );
    void sendSocialFacialExpressionToActuation(int expression);
    void sendSocialReactionSpeechToActuation(int speech);
    void sendSocialReactionToActuation(int reaction);
    void sendEyeBlinkingToActuation(Bottle eyeBlinking);
    void sendFallingReactionToActuation(int falling);
    void sendFallingReactionSpeechToActuation(int fallingSpeech);
    void sendInterruptionToSelfMonitoring(int interruption);

    int                     _numberOfFaces;
    VectorOf<double>        _dataSound;
};

