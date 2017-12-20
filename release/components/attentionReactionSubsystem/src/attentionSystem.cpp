  /** @file attentionSystem.cpp 
  *
  * Version 1.0
  * 
  * 10/08/2016
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
 

#include "attentionSystem.h"
#include "attentionReactionYarpInterface.h"
using namespace std;

AttentionSystemThread::AttentionSystemThread(AttentionReactionYarpInterface* yarp){
   _yarpInterface = yarp;
   _elicitedAttention.resize(SIZE,0.0);
   _elicitedAttentionVector.resize(3,0.0);
   _featureValue.resize(SIZE,0.0);
   _dist.resize(SIZE,0.0);
   _timer.resize(SIZE,TIME_LIMIT);
   _maxIndex_previous = 99; /*Just an out of range index*/
   _maxIndex = 98; /*Just an out of range index*/
   _maxDist = 0.0;  
   _maxElicitedAttention = 0.0;
   _previousFaceX.resize(3,1.0);
   _previousFaceY.resize(3,1.0);
   _previousFaceZ.resize(3,1.0);
   _getFaceX.resize(3,0.0);
   _getFaceY.resize(3,0.0);
   _getFaceZ.resize(3,0.0);
   _soundX = 0.0;
   _soundY = 0.0;
   _soundZ = 0.0;
   _socialGazeX = 0.0;
   _socialGazeY = 0.0;
   _socialGazeZ = 0.0;
   _previousSoundX = 1.0;
   _previousSoundY = 1.0;
   _previousSoundZ = 1.0;
   _previousSocialGazeX = 1.0;
   _previousSocialGazeY = 1.0;
   _previousSocialGazeZ = 1.0;
}

bool AttentionSystemThread::threadInit() 
{
   return true;
}

void AttentionSystemThread::threadRelease() 
{
   /* for example, delete dynamically created data-structures */
}
void AttentionSystemThread::run(){

	while (isStopping() != true) { // this should be fixed later on

       Sleep(PAUSE);
       
       //Might help to use distance instead of coordinates to control the timers as it is highly
       //improbable that coordinates does not change over a couple of seconds:if previousDistance =aprox.  distance -+ x 

       if((_previousFaceX[0] == _getFaceX[0])&&(_previousFaceY[0]==_getFaceY[0])&&(_previousFaceZ[0]==_getFaceZ[0])&&(_timer[0]!=0)&&(_timer[0]!=TIME_LIMIT)) {
        _timer[0] = _timer[0]+0.1;
        }
        if((_previousFaceX[1] == _getFaceX[1])&&(_previousFaceY[1]==_getFaceY[1])&&(_previousFaceZ[1]==_getFaceZ[1])&&(_timer[1]!=0)&&(_timer[1]!=TIME_LIMIT)) {
            _timer[1] = _timer[1]+0.1;
        }
        if((_previousFaceX[2] == _getFaceX[2])&&(_previousFaceY[2]==_getFaceY[2])&&(_previousFaceZ[2]==_getFaceZ[2])&&(_timer[2]!=0)&&(_timer[2]!=TIME_LIMIT)) {
            _timer[2] = _timer[2]+0.1;
        }
        if((_previousSoundX == _soundX)&&(_previousSoundY==_soundY)&&(_previousSoundZ==_soundZ)&&(_timer[3]!=0)&&(_timer[3]!=TIME_LIMIT)) {
            _timer[3] = _timer[3]+0.5;
        }
        if((_previousSocialGazeX == _socialGazeX)&&(_previousSocialGazeY==_socialGazeY)&&(_previousSocialGazeZ==_socialGazeZ)&&(_timer[4]!=0)&&(_timer[4]!=TIME_LIMIT)) {
            _timer[4] = _timer[4]+1;
        }	
	}
}

VectorOf<double> AttentionSystemThread::sendSoundToAttention(double coordX, double coordY, double coordZ){
    
    VectorOf<double> attentionCoordinates;
    attentionCoordinates.resize(3);
    
    _soundX = coordX;
    _soundY = coordY;
    _soundZ = coordZ;
                        
    /*Add values to vectors*/
    _elicitedAttention[3] = 0.0;
    _dist[3] = _soundX + _soundY+ _soundZ;
    _timer[3] = 1.0;
    _featureValue[3] = 0.0;
    if(_dist[3] > _maxDist) {
        _maxDist = _dist[3];
    }
    
    attentionCoordinates = getAttentionOutput(); 
    
    _previousSoundX = _soundX;
    _previousSoundY = _soundY;
    _previousSoundZ = _soundZ;
    
    return attentionCoordinates;
}

VectorOf<double> AttentionSystemThread::sendFacesToAttention(VectorOf<double> coordX, VectorOf<double> coordY,VectorOf<double> coordZ){
    
    VectorOf<double> attentionCoordinates;
    attentionCoordinates.resize(3);
    
    _getFaceX[0] = coordX[0];
    _getFaceY[0] = coordY[0];
    _getFaceZ[0] = coordZ[0];
    
    _elicitedAttention[0] = 0.0;
    _dist[0] = _getFaceX[0] + _getFaceY[0]+ _getFaceZ[0];
    _timer[0] = 1.0;
    _featureValue[0] = 0.0;
    if(_dist[0] > _maxDist) {
        _maxDist = _dist[0];
    }
    
    if(coordX.size()>1) {
        _getFaceX[1] = coordX[1];
        _getFaceY[1] = coordY[1];
        _getFaceZ[1] = coordZ[1];

        _elicitedAttention[1] = 0.0;
        _dist[1] = _getFaceX[1] + _getFaceY[1]+ _getFaceZ[1];
        _timer[1] = 1.0;
        _featureValue[1] = 0.0;
        if(_dist[1] > _maxDist) {
            _maxDist = _dist[1];
        }
    }
    else{
        _getFaceX[1] = 0.0;
        _getFaceY[1] = 0.0;
        _getFaceZ[1] = 0.0;

        _elicitedAttention[1] = 0.0;
        _dist[1] = 0.0;
        _timer[1] = TIME_LIMIT;
        _featureValue[1] = 0.0;
    }
    if(coordX.size()>2) {
        _getFaceX[2] = coordX[2];
        _getFaceY[2] = coordY[2];
        _getFaceZ[2] = coordZ[2];

        _elicitedAttention[2] = 0.0;
        _dist[2] = _getFaceX[2] + _getFaceY[2]+ _getFaceZ[2];
        _timer[2] = 1.0; 
        _featureValue[2] = 0.0;
        if(_dist[2] > _maxDist) {
            _maxDist = _dist[2];
        }
    }
    else{
        _getFaceX[2] = 0.0;
        _getFaceY[2] = 0.0;
        _getFaceZ[2] = 0.0;

        _elicitedAttention[2] = 0.0;
        _dist[2] = 0.0;
        _timer[2] = TIME_LIMIT;
        _featureValue[2] = 0.0;
    }
                                    
    attentionCoordinates = getAttentionOutput(); 
    
    _previousFaceX[0] = _getFaceX[0];
    _previousFaceY[0] = _getFaceY[0];
    _previousFaceZ[0] = _getFaceZ[0];
    _previousFaceX[1] = _getFaceX[1];
    _previousFaceY[1] = _getFaceY[1];
    _previousFaceZ[1] = _getFaceZ[1];
    _previousFaceX[2] = _getFaceX[2];
    _previousFaceY[2] = _getFaceY[2];
    _previousFaceZ[2] = _getFaceZ[2];
    
    return attentionCoordinates;
}

VectorOf<double> AttentionSystemThread::getAttentionOutput(){
    
    double aux;
    int j=0;
    double auxDist = 0.0;
    double auxTime = 0.0;
    bool attentionStimuli = false;
    
    srand (time(NULL));

    double outputCoordinateX = 0.0;
    double outputCoordinateY = 0.0;
    double outputCoordinateZ = 0.0;

    /*Get attention value for each of the social features. Non-repeated random values.*/
                        
    for(int i =0; i<SIZE;i++){
        _featureValue[i] = i+1;    
    }
    
    for(int i =SIZE; i>0;i--){
        j = (int)(rand()%SIZE); 
        aux = _featureValue[i];
        _featureValue[i] = _featureValue[j];
        _featureValue[j] = aux;
    }
    _featureValue[3] = 1.0;
    /*Once we got all the variables we use a vector of length N to store the elicited attention value of each point of interest*/
    for(int i =0; i<SIZE;i++){
        _elicitedAttention[i] = _featureValue[i]/SIZE;
        auxDist = 1-(_dist[i]/_maxDist);
        _elicitedAttention[i] = _elicitedAttention[i] + auxDist;
        auxTime = 1-(_timer[i]/TIME_LIMIT);
        if(auxTime>0)
            _elicitedAttention[i] = _elicitedAttention[i]*PEAK*auxTime;
        else {
            _elicitedAttention[i] = 0;
            _timer[i] = TIME_LIMIT;
        }
        if(_elicitedAttention[i] > _maxElicitedAttention){
            _maxElicitedAttention = _elicitedAttention[i];
            _maxIndex = i;
        }
    }
    if(_maxElicitedAttention == 0.0){
        _maxIndex = 98;
    }

    /*Get coordinates associated to maxIndex*/
    switch(_maxIndex) {
        case 0:
            outputCoordinateX = _getFaceX[0];
            outputCoordinateY = _getFaceY[0];
            outputCoordinateZ = _getFaceZ[0];
            break;
        case 1:
            outputCoordinateX = _getFaceX[1];
            outputCoordinateY = _getFaceY[1];
            outputCoordinateZ = _getFaceZ[1];
            break;
        case 2:
            outputCoordinateX = _getFaceX[2];
            outputCoordinateY = _getFaceY[2];
            outputCoordinateZ = _getFaceZ[2];
            break;
        case 3:
            outputCoordinateX = _soundX;
            outputCoordinateY = _soundY;
            outputCoordinateZ = _soundZ;
            break;
        case 4:
            outputCoordinateX = _socialGazeX;
            outputCoordinateY = _socialGazeY;
            outputCoordinateZ = _socialGazeZ;
            break;
        case 98:
            outputCoordinateX = 0.0;
            outputCoordinateY = 0.0;
            outputCoordinateZ = 0.0;
            break;

    }
    
    for(int i =0; i<SIZE;i++){
        if(_dist[i] > 0){
            attentionStimuli = true;
        }
    }
    if(attentionStimuli){
        _elicitedAttentionVector[0] = outputCoordinateX;
        _elicitedAttentionVector[1] = outputCoordinateY;
        _elicitedAttentionVector[2] = outputCoordinateZ;
    }               
    
    _maxIndex_previous = _maxIndex;
    _maxElicitedAttention = 0.0;

    return _elicitedAttentionVector;
}

VectorOf<double> AttentionSystemThread::sendSocialGazeToAttention(double coordX, double coordY, double coordZ){
        
    VectorOf<double> attentionCoordinates;
    attentionCoordinates.resize(3);
    
    _socialGazeX = coordX;
    _socialGazeY = coordY;
    _socialGazeZ = coordZ;
                        
    /*Add values to vectors*/
    _elicitedAttention[4] = 0.0;
    _dist[4] = _socialGazeX + _socialGazeY+ _socialGazeZ;
    _timer[4] = 1.0;
    _featureValue[4] = 0.0;
    if(_dist[4] > _maxDist) {
        _maxDist = _dist[4];
    }
    attentionCoordinates = getAttentionOutput(); 
    
    _previousSocialGazeX = _socialGazeX;
    _previousSocialGazeY = _socialGazeY;
    _previousSocialGazeZ = _socialGazeZ;
    
    return attentionCoordinates;
}
