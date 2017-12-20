 /** @file attentionSystem.h
  *
  * Version 1.0
  * 
  * 10/08/2016
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
#define TIME_LIMIT 10  
#define PEAK 30 
#define SIZE 5 /*3 faces, sound and socialGaze*/

class AttentionReactionYarpInterface;
  
class AttentionSystemThread : public Thread {
    public:
	AttentionSystemThread(AttentionReactionYarpInterface* yarp);
	VectorOf<double> sendSoundToAttention(double coordX, double coordY, double coordZ);
    VectorOf<double> sendFacesToAttention(VectorOf<double> coordX, VectorOf<double> coordY,VectorOf<double> coordZ);
    VectorOf<double> sendSocialGazeToAttention(double coordX, double coordY, double coordZ);
    bool threadInit();
    void threadRelease(); 
    void run();
    VectorOf<double> getAttentionOutput();
    void sendAttentionOutput(double _outputCoordinateX,double _outputCoordinateY,double _outputCoordinateZ);
	
    private:
	AttentionReactionYarpInterface* _yarpInterface;
    
    VectorOf<double>                 _elicitedAttention;
    VectorOf<double>                 _elicitedAttentionVector;
    VectorOf<double>                 _featureValue;
    VectorOf<double>                 _dist;
    VectorOf<double>                 _timer;
    double                          _maxDist;
    int                             _maxIndex;
    int                             _maxIndex_previous;
    double                          _maxElicitedAttention;
    VectorOf<double>                _getFaceX;
    VectorOf<double>                _getFaceY;
    VectorOf<double>                _getFaceZ;
    
    double                          _soundX;
    double                          _soundY;
    double                          _soundZ;

    double                          _socialGazeX;
    double                          _socialGazeY;
    double                          _socialGazeZ;

    VectorOf<double>                _previousFaceX;
    VectorOf<double>                _previousFaceY;
    VectorOf<double>                _previousFaceZ;

    double                          _previousSocialGazeX;
    double                          _previousSocialGazeY;
    double                          _previousSocialGazeZ;
    
    double                          _previousSoundX;
    double                          _previousSoundY;
    double                          _previousSoundZ;
        
	bool                            _attentionSwitchOff;
    double                          _outputCoordinateX;
    double                          _outputCoordinateY;
    double                          _outputCoordinateZ;   
};

