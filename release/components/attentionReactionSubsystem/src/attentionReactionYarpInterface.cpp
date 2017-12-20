 /** @file attentionReactionYarpInterface.cpp 
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

#include "attentionReactionYarpInterface.h"
#include "attentionReactionSubsystem.h"
#include "attentionReactionController.h"

using namespace std;

AttentionReactionYarpInterface::AttentionReactionYarpInterface(){

}

void AttentionReactionYarpInterface::init(AttentionReactionSubsystemThread* yarpThread, AttentionReactionController* controller){
   _id = 0;
   _controller = controller;
   _yarpThread = yarpThread;   
}


void AttentionReactionYarpInterface::getFromActionFeedback(Bottle data){
    _controller->getFromActionFeedback(data); 
    return;
}

void AttentionReactionYarpInterface::getFromFaces(VectorOf<double> data){

    //_numberOfFaces = (data).size()/3;
    _numberOfFaces = data[0];
    //printf("\n YarpInterface getFromFaces, number of faces %d",_numberOfFaces);
    if (_numberOfFaces >= 1){
        
        VectorOf<double> facesX;
        VectorOf<double> facesY;
        VectorOf<double> facesZ;
                  
        facesX.resize(_numberOfFaces);
        facesY.resize(_numberOfFaces);
        facesZ.resize(_numberOfFaces);
    
        for(int i = 0; i < _numberOfFaces;i++){        
            facesX[i] = (double)data[1+(i*3)];
            facesY[i] = (double)data[2+(i*3)];
            facesZ[i] = (double)data[3+(i*3)];
        
        }
        _controller->getFromFaces(facesX,facesY,facesZ);
    }
    else{
        printf("\n YarpInt error attention faces data");
    }
    return;
}


void AttentionReactionYarpInterface::getFromSound(VectorOf<double> data){

    _dataSound.clear();     
    _dataSound.resize(3);
    //double angle = (double)data[0]/((rand()%2)+1.1); //Divided by a random number [1.1-2] to limit the range of head motion so no over the should looks. 
                                                     //SoundDirection angle is way too unstable and it is providing weird inputs. 
    if ((data).size() == 2){    
        _dataSound = az2xyz(data[0],0);
        //printf("\n YarpInterface getFromSound X %f Y %f and Z %f, and confidence %f",_dataSound[0],_dataSound[1],_dataSound[2],data[1]);
        if(data[1] > 0.7){ //Only send sound direction if confidence is above 0.7
            _controller->getFromSound(_dataSound); 
        }
    }
    else{
        printf("\n YarpInt error attention sound data");
    }

    return;    
}

void AttentionReactionYarpInterface::getFromAttentionBias(VectorOf<double> data){

    
    if ((data).size() == 3){
        _controller->getFromAttentionBias(data);     
    }
    else{
        printf("\n YarpInt error attention bias data");
    }
    return;
}

void AttentionReactionYarpInterface::getFromGazeCommand(VectorOf<double> data){

   
    if ((data).size() == 3){
        _controller->getFromGazeCommand(data); 
    }
    else{
        printf("\n YarpInt error receiving therapist gaze command data");
    }
    return;
}

void AttentionReactionYarpInterface::getFromAttentionSwitchOff(VectorOf<int> data){
	
   _controller->getFromAttentionSwitchOff(data[0]);    
	return;
}

void AttentionReactionYarpInterface::getFromIdentifyFaceExpression(VectorOf<int> data){

    //printf("\n YarpInterface getFromIdentifyFaceExpression %d",data[0]);
    _controller->getFromFaceExpression(data[0]);    
    return;
}

/*void AttentionReactionYarpInterface::getFromBumpers(Bottle data){
	
    if(data.get(0).asString() == "Left1"){
		_controller->getFromBumpers(0);  
	}
	if(data.get(0).asString() == "Right1"){
		_controller->getFromBumpers(1);  
	}
    return;
}

void AttentionReactionYarpInterface::getFromHeadTactile(Bottle data){
	
    if((data.get(0).asString() == "Middle1")||(data.get(0).asString() == "Rear1")||(data.get(0).asString() == "Front1")){
	    _controller->getFromHeadTactile(1);	
	}
        
    return; 
}

void AttentionReactionYarpInterface::getFromHandTactile(Bottle data){
	
    if(data.get(0).asString() == "Left1"){
	    _controller->getFromHandTactile(0);  
	}
	if(data.get(0).asString() == "Right1"){
		_controller->getFromHandTactile(1);  
	} 
}*/

void AttentionReactionYarpInterface::getFromRobotSensors(Bottle data){
    //printf("\n\n ARS yarp Robot sensors %s %d \n\n",data.get(0).asString().c_str(), data.get(1).asInt());
	
    /*if((data.get(0).asString() == "LeftHand")||(data.get(0).asString() == "RightHand")){
	    _controller->getFromHandTactile(data.get(1).asInt());  
	}
	else*/ 
    if((data.get(0).asString() == "FrontTactil")||(data.get(0).asString() == "MiddleTactile")||(data.get(0).asString() == "RearTactile")){
	    _controller->getFromHeadTactile(data.get(1).asInt()); 
        //printf("\n\n ARS yarp Robot sensors HEAD \n\n");
	}
    else if(data.get(0).asString() == "Falling"){
	    _controller->getFromBalance((bool)(data.get(1).asInt()));    
	}
    if((data.get(0).asString() == "LeftBumper")||(data.get(0).asString() == "RightBumper")){
	    _controller->getFromBumpers(data.get(1).asInt()); 
        //printf("\n\n ARS yarp Robot sensors BUMPERS \n\n");
	}
}

void AttentionReactionYarpInterface::getFromCheckMutualGaze(VectorOf<int> data){
	//printf("\n YarpInterface getFromCheckMutualGaze %d",(int)(data[0]));
    _controller->getFromCheckMutualGaze((bool)(data[0]));    
    return; 
}

void AttentionReactionYarpInterface::getFromRecognizeSpeech(Bottle data){
	
    if(!data.isNull()){
        //printf("\n YarpInterface getFromRecognizeSpeech might work");
        _controller->getFromRecognizeSpeech(true);    
    }
    return; 
}

void AttentionReactionYarpInterface::getFromAffectiveState(Bottle data){
	
    _controller->getFromAffectiveState(data.get(0).asInt());    
    return; 
}

void AttentionReactionYarpInterface::getFromReactionSwitchOff(VectorOf<int> data){
	
    _controller->getFromReactionSwitchOff((bool)(data[0]));    
	return;
}

/*void AttentionReactionYarpInterface::getFromBalance(VectorOf<int> data){
	
   _controller->getFromBalance((bool)(data[0]));    
	return;
}*/

void AttentionReactionYarpInterface::sendElicitedAttentionToActuation(double coordX, double coordY, double coordZ){ 
    //printf("\n ARS YARP SEND %.2f %.2f %.2f\n",coordX,coordY,coordZ);
	//_yarpThread->sendElicitedAttentionToActuation(coordX, coordY, coordZ); //Not send this until attention issue is solved
	return;
}
void AttentionReactionYarpInterface::sendSocialFacialExpressionToActuation(int face){ 
	_yarpThread->sendSocialFacialExpressionToActuation(face);
	return;
}
void AttentionReactionYarpInterface::sendSocialReactionSpeechToActuation(int speech){ 
	//_yarpThread->sendSocialReactionSpeechToActuation(speech);
	return;
}
void AttentionReactionYarpInterface::sendSocialReactionToActuation(int reaction){ 
	_yarpThread->sendSocialReactionToActuation(reaction);
	return;
}
void AttentionReactionYarpInterface::sendEyeBlinkingToActuation(Bottle eyeBlinking){ 
	_yarpThread->sendEyeBlinkingToActuation(eyeBlinking);
	return;
}
void AttentionReactionYarpInterface::sendFallingReactionToActuation(int falling){ 
	_yarpThread->sendFallingReactionToActuation(falling);
	return;
}
void AttentionReactionYarpInterface::sendFallingReactionSpeechToActuation(int fallingSpeech){ 
	//_yarpThread->sendFallingReactionSpeechToActuation(fallingSpeech);
	return;
}
void AttentionReactionYarpInterface::sendInterruptionToSelfMonitoring(int interruption){ 
	_yarpThread->sendInterruptionToSelfMonitoring(interruption);
	return;
}
/*Azimuth and elevation values to xyz coordinates*/
VectorOf<double> AttentionReactionYarpInterface::az2xyz(double azimuth, double elevation)
{
    VectorOf<double> coordinates;
    coordinates.resize(3);

    double azimuthRadians = azimuth*PI/180;
    double elevationRadians = elevation*PI/180;
	
	double x = cos(elevationRadians) * cos(azimuthRadians); 
	double y = cos(elevationRadians) * sin(azimuthRadians);
	double z = sin(elevationRadians);

    double unitvec = sqrt((x*x) + (y*y) + (z*z));

	coordinates[0] = (double)(x / unitvec);
    coordinates[1] = (double)(y / unitvec);
    coordinates[2] = (double)(z / unitvec);

    return coordinates;
}
