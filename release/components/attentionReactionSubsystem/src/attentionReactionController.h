 /** @file attentionReactionController.h
  *
  * Version 1.0
  * 
  * 10/08/2016
  *
  *
  * \defgroup DREAM_attentionReactionSubsystem attentionReactionSubsystem
  *
  * 
  * \section lib_sec Libraries
  *
  * YARP.
  *
  * \section parameters_sec Parameters
  * 
  * <b>Command-line Parameters </b> 
  * 
  * The following key-value pairs can be specified as command-line parameters 
  * by prefixing \c -- to the key e.g. \c --from file.ini. 
  * The value part can be changed to suit your needs; the default values are 
  * shown below. 
  *
  * <b>Configuration File Parameters </b>
  *
  * The following key-value pairs can be specified as parameters in the configuration file 
  * (they can also be specified as command-line parameters if you so wish). 
  * The value part can be changed to suit your needs; the default values are shown below. 
  *
  * \author 
     * 
 * Pablo Gómez, Vrije Universiteit Brussel, Belgium.
 * 
 * Copyright (C) 2016 DREAM Consortium
 * 
 */

/* 
 * Copyright (C) 2016 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Pablo Gomez, Vrije Universiteit Brussel
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
#include <windows.h>
#include <vector>
#include <math.h>
#include <initializer_list>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/Thread.h>

using namespace std;
using namespace yarp::sig;
using namespace yarp::os; 

#define PAUSE 5000 // used in SLEEP function 

class AttentionReactionYarpInterface;
class AttentionSystemThread;
class EyeBlinkingThread;

class AttentionReactionController: public Thread {
	public:
	AttentionReactionController(AttentionReactionYarpInterface* yarp, AttentionSystemThread* attentionModel, EyeBlinkingThread* eyeBlinking);
	
	void getFromAttentionSwitchOff(bool switchOff);
    void getFromAttentionBias(VectorOf<double> data);
    void getFromGazeCommand(VectorOf<double> data);
    void getFromFaces(VectorOf<double> dataX, VectorOf<double> dataY, VectorOf<double> dataZ);
    void getFromSound(VectorOf<double> data);
    void getFromSocialGaze(VectorOf<double> data);
    void getFromReactionSwitchOff(bool switchOff);
    void getFromFaceExpression(int data); 
    void getFromBumpers(int bumpers); 
    void getFromHeadTactile(int head); 
    void getFromHandTactile(int hand); 
    //void getFromRobotSensors(int hand); /*check type*/

    void getFromCheckMutualGaze(bool data); 
    void getFromRecognizeSpeech(bool speech); 
    void getFromAffectiveState(int state); 
    void getFromBalance(bool data);
    
    void getFromActionFeedback(Bottle data); 
       
    void sendAttentionOutput(double _outputCoordinateX,double _outputCoordinateY,double _outputCoordinateZ);
    void sendSocialReactionOutput();
    void sendEyeBlinkingOutput(Bottle blinkingBehavior);
    void sendFallingReactionOutput(int output);
    
    vector<string> split(const string &s, char delim, vector<string> &elems);
    vector<string> split(const string &s, char delim);

    bool threadInit();
    void threadRelease(); 
    void run();

private:
	
	AttentionReactionYarpInterface* _yarpInterface;
    AttentionSystemThread* _attentionModel;
    EyeBlinkingThread* _eyeBlinking;

	bool _debug;
    bool _isBusy;
    bool _gazeSent;
    
	bool _attentionSwitchOff;
    bool _reactionSwitchOff;
    int _socialFaceExpression;
    int _socialFaceExpressionPrevious;
    int _socialReaction;
    int _socialSpeech;
    bool _micromotions;
    bool _micromotionsPrevious;
        
    bool _gazeVariation;
    int _affectiveState;
    bool _recognizeSpeech;
    Bottle _eyeBlinkingBehavior;

    int _conversationalGesture;

    bool _outOfBalance;
    bool _hasFallen;

    double _outputCoordinateX;
    double _outputCoordinateY;
    double _outputCoordinateZ;
    VectorOf<double> _attentionFocus;
    VectorOf<double> _previousAttentionFocus;    
};