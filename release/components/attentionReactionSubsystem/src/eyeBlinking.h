 /** @file eyeBlinking.h
  *
  * Version 1.0
  * 
  * 10/08/2016
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
#include <time.h>
#include <vector>
#include <map>
#include <sstream>
#include <windows.h>
#include <stdlib.h>
#include <cmath>

#include <yarp/sig/all.h>
#include <yarp/sig/Vector.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>

using namespace std;
using namespace yarp::sig;
using namespace yarp::os; 

#define STRINGLENGTH 132 // used to define a string when post-processing the bottle messages
#define PAUSE 1000 // used in SLEEP function 
#define RATIO_BLINKING 0.9 // a random number between 0 and 1 would be compared against this one to trigger a random blink.
#define BLINKING_RATE 1.75

class AttentionReactionYarpInterface;
  
class EyeBlinkingThread : public Thread {
    public:
	EyeBlinkingThread(AttentionReactionYarpInterface* yarp);
    Bottle blinkMorphology();
    Bottle sendMutualGazeToEyeBlinking(bool mutualGaze);
    Bottle sendChildSpokeToEyeBlinking(bool recognizeSpeech);
    Bottle sendAffectiveStateToEyeBlinking(int affectiveState);
    Bottle sendSocialFaceExpressionToEyeBlinking(int socialFaceExpression);
    Bottle sendGazeVariationToEyeBlinking(bool gazeVariation);
    Bottle getEyeBlinkingBehavior();
	bool threadInit();
    void threadRelease(); 
    void run();
    
    private:
	AttentionReactionYarpInterface* _yarpInterface;
        
	bool _gazeVariation;
    bool _gazeVariationPrevious;
    bool _mutualGaze;
    bool _mutualGazePrevious;
    bool _recognizeSpeech;
    int _socialFaceExpression;
    int _socialFaceExpressionPrevious;
    int _affectiveState;
    int _affectiveStatePrevious;
    double _probabilityBlink;
    int _numberOcurrences; 
    Bottle _blinkAction;
};

