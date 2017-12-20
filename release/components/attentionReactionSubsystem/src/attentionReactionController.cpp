  /** @file attentionReactionController.cpp 
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

#include "attentionReactionController.h"
#include "attentionReactionSubsystem.h"
#include "attentionSystem.h"
#include "eyeBlinking.h"
#include "attentionReactionYarpInterface.h"

using namespace std;

AttentionReactionController::AttentionReactionController(AttentionReactionYarpInterface* yarp, AttentionSystemThread* attentionModel, EyeBlinkingThread* eyeBlinking) 
{
	_yarpInterface = yarp;
    _attentionModel = attentionModel;
    _eyeBlinking = eyeBlinking;
   
    _isBusy = false;

    /*attention model variables*/
    _gazeSent = false;
	_attentionSwitchOff = true;
    _attentionFocus.resize(3);
    _attentionFocus[0] = 0.0;
    _attentionFocus[1] = 0.0;
    _attentionFocus[2] = 0.0;
    _previousAttentionFocus.resize(3);
    _previousAttentionFocus[0] = 0.0;
    _previousAttentionFocus[1] = 0.0;
    _previousAttentionFocus[2] = 0.0;

    /*reaction subsystem variables*/
    _reactionSwitchOff = true;

    /*socialReaction variables*/
    _micromotions = false;
    _micromotionsPrevious = false;
    _socialFaceExpression = 0;
    _socialFaceExpressionPrevious = 0;
    //_socialSpeech = 0;
    //_conversationalGesture = 0;

    /*eyeBlinking variables*/
    _gazeVariation = false;
    _recognizeSpeech = false;
    _eyeBlinkingBehavior.clear();
    _affectiveState = 0;

    /*falling reaction variables*/
    _outOfBalance = false;
    _hasFallen = false;
}

bool AttentionReactionController::threadInit() 
{
   return true;
}

void AttentionReactionController::threadRelease() 
{
   /* for example, delete dynamically created data-structures */
}
void AttentionReactionController::run(){

    /*This method controls the logic of the attentionReactionSubsystem*/
    while(isStopping() != true) { 

        Sleep(PAUSE);
        
        if((!_isBusy)&&(!_attentionSwitchOff)&&(!_gazeSent)){ /*Continously check attention focus if conditions are satisfied*/
            //_attentionFocus = _attentionModel->getAttentionOutput(); //Commented to avoid an initial blink when maybe it is not needed
    
            if((_attentionFocus[0] != _previousAttentionFocus[0])||(_attentionFocus[1] != _previousAttentionFocus[1])||(_attentionFocus[2] != _previousAttentionFocus[2])){
                sendAttentionOutput(_attentionFocus[0],_attentionFocus[1],_attentionFocus[2]); 
                _gazeVariation = true;
                _previousAttentionFocus[0] = _attentionFocus[0];
                _previousAttentionFocus[1] = _attentionFocus[1];
                _previousAttentionFocus[2] = _attentionFocus[2];

                if(!_reactionSwitchOff){
                    
                    Bottle blinkingAfterGazeVariation = _eyeBlinking->sendGazeVariationToEyeBlinking(_gazeVariation); 
                
                    if(blinkingAfterGazeVariation.size()>0){
                        /*printf("\n Controller _blinkingType %s", blinkingAfterGazeVariation.get(0).toString().c_str());
                        printf("\n Controller _blinkingFunction %s", blinkingAfterGazeVariation.get(1).toString().c_str());
                        printf("\n Controller _blinkingDuration %s", blinkingAfterGazeVariation.get(2).toString().c_str());*/

                        sendEyeBlinkingOutput(blinkingAfterGazeVariation); 
                    }
                }
            }
            else{
                _gazeVariation = false;
            }
            
         }
        if((!_isBusy)&&(!_reactionSwitchOff)) { /*Continously check balance if conditions are satisfied*/
            if(_outOfBalance && !_hasFallen){/*Current balance is not OK, and the robot has not recently fallen --> just fallen*/
                sendFallingReactionOutput(1); /*DamageAvoidance behavior*/
                _hasFallen = true;
                _attentionSwitchOff = true;
                _micromotions = false;
                _micromotionsPrevious = false;
                _socialFaceExpression = 0;
                //_socialSpeech = 0;

            }else if(_outOfBalance && _hasFallen){/*Current balance is not OK, and the robot has recently fallen --> on the floor (or not up yet)*/
                sendFallingReactionOutput(0); /*Getting up behavior*/
                
            }else if(!_outOfBalance && _hasFallen){/*Current balance is OK, and the robot has recently fallen --> back on its feet*/
                /*The robot selects one sentence to soften the situation*/
                sendFallingReactionOutput(2); /*Speech behavior*/
                _hasFallen = false;
                _attentionSwitchOff = false;
                _micromotions = true;
                _socialFaceExpression = 0;
            }
            else if(_micromotions && !_micromotionsPrevious) { /*Micromotions are triggered by default if conditions are satisfied*/
			    /*Perform set of micromotions: gaze-shifts, face-tracking, etc.*/
                _socialFaceExpression = 0;
			    //_socialSpeech = 0;
    
                sendSocialReactionOutput();
		    }
            
            if(!_hasFallen){
                _socialFaceExpression = 0;
                /*EyeBlinking behaviours are not dependent on reaction SwitchOff*/
                _eyeBlinkingBehavior = _eyeBlinking->getEyeBlinkingBehavior();
                if(_eyeBlinkingBehavior.size()>0){
                    /*printf("\n Controller loop _blinkingType %s", _eyeBlinkingBehavior.get(0).toString().c_str());
                    printf("\n Controller loop _blinkingFunction %s", _eyeBlinkingBehavior.get(1).toString().c_str());
                    printf("\n Controller loop _blinkingDuration %s", _eyeBlinkingBehavior.get(2).toString().c_str());*/

                    sendEyeBlinkingOutput(_eyeBlinkingBehavior); 
                }  
            }
        }
    }
}

void AttentionReactionController::getFromActionFeedback(Bottle data){
    
    if (data.get(0).asInt() == 1)
    {
		    //in this case a move has been successfully completed, so reset; flag
		    // -- if a fail message has been returned, then could try something else...
	    _isBusy = false;
        if(!_reactionSwitchOff){
            //_micromotionsPrevious    = false;
            _micromotions            = true;
        }
    }
    else{
	    //_isBusy = true;
    }
       
    return;
}
void AttentionReactionController::getFromAttentionSwitchOff(bool switchOff){
    
  _attentionSwitchOff = switchOff;
  if(_attentionSwitchOff){ /*Output coordinates centered. Send output to ACTUATION*/
     /* _gazeSent = true;
      _attentionFocus[0] = 0.0;
      _attentionFocus[1] = 0.0;
      _attentionFocus[2] = 0.0;

      sendAttentionOutput(_attentionFocus[0],_attentionFocus[1],_attentionFocus[2]); */
  }
  return;
}

void AttentionReactionController::getFromAttentionBias(VectorOf<double> data){
    
  if(_attentionSwitchOff){ 
      //printf("\n Controller tried to reach getFromAttentionBias but attention is OFF");
  }
  else if(_gazeSent){ /*Here is to control if a gaze was already sent by therapist or switchOff controller*/
      printf("\n Controller tried to reach getFromAttentionBias but a gaze was already sent");
  }
  else if(_isBusy){ /*Actuation system is busy with another sequence*/
      printf("\n Controller tried to reach getFromAttentionBias but system is busy");
  }  
  else{ /*In case attention is ON*/ /*These are the coordinates to be sent*/
      _gazeSent = true;
      _attentionFocus[0] = (double)(data[0]);
      _attentionFocus[1] = (double)(data[1]);
      _attentionFocus[2] = (double)(data[2]);
      /*printf("\n Controller _attentionEmulationX %.2f", _outputCoordinateX);
      printf("\n Controller _attentionEmulationY %.2f", _outputCoordinateY);
      printf("\n Controller _attentionEmulationZ %.2f", _outputCoordinateZ);*/

      sendAttentionOutput(_attentionFocus[0],_attentionFocus[1],_attentionFocus[2]);       
  }
  return;
}

void AttentionReactionController::getFromGazeCommand(VectorOf<double> data){
  
  if(_attentionSwitchOff){ 
      //printf("\n Controller tried to reach getFromGazeCommand but attention is OFF");
  }
  else if(_isBusy){ /*Actuation system is busy with another sequence*/
      printf("\n Controller tried to reach getFromGazeCommand but system is busy");
  }
  else{ /*In case attention is ON*/
      _gazeSent = true;
      _attentionFocus[0] = (double)(data[0]);
      _attentionFocus[1] = (double)(data[1]);
      _attentionFocus[2] = (double)(data[2]);
      /*printf("\n Controller _therapistAttentionX %.2f", _outputCoordinateX);
      printf("\n Controller _therapistAttentionY %.2f", _outputCoordinateY);
      printf("\n Controller _therapistAttentionZ %.2f", _outputCoordinateZ);*/

      sendAttentionOutput(_attentionFocus[0],_attentionFocus[1],_attentionFocus[2]); 
  }
  return;
}

void AttentionReactionController::getFromFaces(VectorOf<double> dataX, VectorOf<double> dataY, VectorOf<double> dataZ){
    
  if(_attentionSwitchOff){ 
      //printf("\n Controller tried to reach getFromFaces but attention is OFF");
  }
  else if(_gazeSent){ /*Here is to control if a gaze was already sent by therapist or switchOff controller*/
      printf("\n Controller tried to reach getFromFaces but a gaze was already sent");
  }
  
  else{ /*In case attention is ON*/ /*These are the coordinates to be sent*/
      _gazeSent = true;
      VectorOf<double> attentionAfterFaces = _attentionModel->sendFacesToAttention(dataX,dataY,dataZ); 
      _attentionFocus[0] = (double)(attentionAfterFaces[0]);
      _attentionFocus[1] = (double)(attentionAfterFaces[1]);
      _attentionFocus[2] = (double)(attentionAfterFaces[2]);
      /*printf("\n Controller _outputFacesX %.2f", _outputCoordinateX);
      printf("\n Controller _outputFacesY %.2f", _outputCoordinateY);
      printf("\n Controller _outputFacesZ %.2f", _outputCoordinateZ);*/

      sendAttentionOutput(_attentionFocus[0],_attentionFocus[1],_attentionFocus[2]); 
  }
  return;
}


void AttentionReactionController::getFromSocialGaze(VectorOf<double> data){
    
  if(_attentionSwitchOff){ 
      //printf("\n Controller tried to reach getFromSocialGaze but attention is OFF");
  }
  else if(_gazeSent){ /*Here is to control if a gaze was already sent by therapist or switchOff controller*/
      printf("\n Controller tried to reach getFromSocialGaze but a gaze was already sent");
  }
  
  else{ /*In case attention is ON*/ /*These are the coordinates to be sent*/
      _gazeSent = true;
      VectorOf<double> attentionSocialGaze = _attentionModel->sendSocialGazeToAttention(data[0],data[1],data[2]); 
      _attentionFocus[0] = (double)(attentionSocialGaze[0]);
      _attentionFocus[1] = (double)(attentionSocialGaze[1]);
      _attentionFocus[2] = (double)(attentionSocialGaze[2]);
      /*printf("\n Controller _outputSocialGazeX %.2f", _outputCoordinateX);
      printf("\n Controller _outputSocialGazeY %.2f", _outputCoordinateY);
      printf("\n Controller _outputSocialGazeZ %.2f", _outputCoordinateZ);*/

      sendAttentionOutput(_attentionFocus[0],_attentionFocus[1],_attentionFocus[2]); 
  }
  return;
}

void AttentionReactionController::getFromSound(VectorOf<double> data){

  if(_attentionSwitchOff){ 
      //printf("\n Controller tried to reach getFromSound but attention is OFF");
  }
  else if(_gazeSent){ /*Here is to control if a gaze was already sent by therapist or switchOff controller*/
      printf("\n Controller tried to reach getFromSound but a gaze was already sent");
  }
  
  else{ /*In case attention is ON*/ /*These are the coordinates to be sent*/
      _gazeSent = true;
      VectorOf<double> attentionAfterSound = _attentionModel->sendSoundToAttention(data[0],data[1],data[2]); 
      _attentionFocus[0] = (double)(attentionAfterSound[0]);
      _attentionFocus[1] = (double)(attentionAfterSound[1]);
      _attentionFocus[2] = (double)(attentionAfterSound[2]);
      /*printf("\n Controller _outputSoundX %.2f", _outputCoordinateX);
      printf("\n Controller _outputSoundY %.2f", _outputCoordinateY);
      printf("\n Controller _outputSoundZ %.2f", _outputCoordinateZ);*/

      sendAttentionOutput(_attentionFocus[0],_attentionFocus[1],_attentionFocus[2]); 
  }
  return;
}

void AttentionReactionController::getFromReactionSwitchOff(bool switchOff){

  _reactionSwitchOff = switchOff;
  
  if(_reactionSwitchOff){  
        _micromotions = false;
        _micromotionsPrevious = false;
        _socialFaceExpression = 0;
        //_socialSpeech = 0;
        sendSocialReactionOutput();
  }
  else{
      _micromotions = true;
      _socialFaceExpression = 0;
      sendSocialReactionOutput();
  }
  return;
}

void AttentionReactionController::getFromFaceExpression(int data){
  if(_reactionSwitchOff){ 
      //printf("\n Controller tried to reach getFromFaceExpression but reaction is OFF");
  }
  
  else{
      if(data == 1){ // child's face expresses happiness    /*The robot should respond by expressing happiness too*/
		    _socialFaceExpression = 1;
		    _micromotions = false;
	    }
	    else if(data == 2){ // child's face expresses sadness   /*The robot should respond by expressing no emotion*/
		    _socialFaceExpression = 0;
	    }
	    else if(data == 3){ // child's face expresses anger     /*The robot should respond by expressing no emotion*/
		    _socialFaceExpression = 0;
	    }
	    else if(data == 4){ // child's face expresses fear     /*The robot should respond by expressing no emotion*/
		    _socialFaceExpression = 0;
	    }
	    else if(data == 0){ // child's face expresses no emotion    /*The robot should respond by randomizing between no emotion and happiness*/
		    double threshold = (double)(rand() %100)/100;

		    if(threshold > 0.5){
			    /*No emotion*/
			    _socialFaceExpression = 0;
		    }
		    else{
			    /*Happiness*/
			    _socialFaceExpression = 1;
			    _micromotions = false;
		    }
	    }
      
      sendSocialReactionOutput();

      Bottle blinkingAfterSocialFaceExpression = _eyeBlinking->sendSocialFaceExpressionToEyeBlinking(_socialFaceExpression); 
        if(blinkingAfterSocialFaceExpression.size()>0){
            /*printf("\n Controller _blinkingType %s", blinkingAfterSocialFaceExpression.get(0).toString().c_str());
            printf("\n Controller _blinkingFunction %s", blinkingAfterSocialFaceExpression.get(1).toString().c_str());
            printf("\n Controller _blinkingDuration %s", blinkingAfterSocialFaceExpression.get(2).toString().c_str());*/

            sendEyeBlinkingOutput(blinkingAfterSocialFaceExpression); 
        } 
        
  }
  return;
}


void AttentionReactionController::getFromBumpers(int bumpers){ 
    if(_reactionSwitchOff){ 
        //printf("\n Controller tried to reach getFromHeadTactile but reaction is OFF");
    }
    else{

		if(bumpers == 0){
			//Direct gaze towards left foot 
            VectorOf<double> coords;
            coords.resize(3);
            coords[0] = (double)1.5;
            coords[1] = (double)0.7;
            coords[2] = (double)-0.65;
      
            getFromSocialGaze(coords);
		}
		if(bumpers == 1){
			//Direct gaze towards right foot
            VectorOf<double> coords;
            coords.resize(3);
            coords[0] = (double)1.5;
            coords[1] = (double)-0.7;
            coords[2] = (double)-0.65;
      
            getFromSocialGaze(coords);
		} 
			    
		_socialFaceExpression = 1; //Express happiness
		//_socialSpeech = 1; //Activate an affective speech
        //_conversationalGesture  =  rand() %6 +1; 
        _micromotions = false;

        sendSocialReactionOutput();

        Bottle blinkingAfterSocialFaceExpression = _eyeBlinking->sendSocialFaceExpressionToEyeBlinking(_socialFaceExpression); 
        if(blinkingAfterSocialFaceExpression.size()>0){
            /*printf("\n Controller _blinkingType %s", blinkingAfterSocialFaceExpression.get(0).toString().c_str());
            printf("\n Controller _blinkingFunction %s", blinkingAfterSocialFaceExpression.get(1).toString().c_str());
            printf("\n Controller _blinkingDuration %s", blinkingAfterSocialFaceExpression.get(2).toString().c_str());*/

            sendEyeBlinkingOutput(blinkingAfterSocialFaceExpression); 
        }
        
    } 
    return;
}

void AttentionReactionController::getFromHeadTactile(int head){ 
    if(_reactionSwitchOff){ 
        //printf("\n Controller tried to reach getFromHeadTactile but reaction is OFF");
    }
    else{
        if(head == 1){
			//Direct gaze towards child's face 
            VectorOf<double> coords;
            coords.resize(3);
            coords[0] = (double)0.0;
            coords[1] = (double)0.0;
            coords[2] = (double)0.0;
      
            getFromSocialGaze(coords);
			_socialFaceExpression = 1; //Express happiness
			//_socialSpeech = 1; //Activate an affective speech
            //_conversationalGesture  =  rand() %6 +1; 
			_micromotions = false;
		}
      
        sendSocialReactionOutput();

        Bottle blinkingAfterSocialFaceExpression = _eyeBlinking->sendSocialFaceExpressionToEyeBlinking(_socialFaceExpression); 
        if(blinkingAfterSocialFaceExpression.size()>0){
            /*printf("\n Controller _blinkingType %s", blinkingAfterSocialFaceExpression.get(0).toString().c_str());
            printf("\n Controller _blinkingFunction %s", blinkingAfterSocialFaceExpression.get(1).toString().c_str());
            printf("\n Controller _blinkingDuration %s", blinkingAfterSocialFaceExpression.get(2).toString().c_str());*/

            sendEyeBlinkingOutput(blinkingAfterSocialFaceExpression); 
        } 
        
    }      
    return;
}

void AttentionReactionController::getFromHandTactile(int hand){ 
    if(_reactionSwitchOff){ 
        //printf("\n Controller tried to reach getFromHeadTactile but reaction is OFF");
    }
    else{

        if(hand == 0){
			//Direct gaze towards left hand
            VectorOf<double> coords;
            coords.resize(3);
            coords[0] = (double)1.5;
            coords[1] = (double)0.7;
            coords[2] = (double)-0.65;
      
            getFromSocialGaze(coords);
		}
		if(hand == 1){
			//Direct gaze towards right hand 
            VectorOf<double> coords;
            coords.resize(3);
            coords[0] = (double)1.5;
            coords[1] = (double)-0.7;
            coords[2] = (double)-0.65;
      
            getFromSocialGaze(coords);
		} 
		_socialFaceExpression = 1; //Express happiness
		//_socialSpeech = 1; //Activate an affective speech
        //_conversationalGesture  =  rand() %6 +1; 
		_micromotions = false;

        sendSocialReactionOutput();

        Bottle blinkingAfterSocialFaceExpression = _eyeBlinking->sendSocialFaceExpressionToEyeBlinking(_socialFaceExpression); 
        if(blinkingAfterSocialFaceExpression.size()>0){
            /*printf("\n Controller _blinkingType %s", blinkingAfterSocialFaceExpression.get(0).toString().c_str());
            printf("\n Controller _blinkingFunction %s", blinkingAfterSocialFaceExpression.get(1).toString().c_str());
            printf("\n Controller _blinkingDuration %s", blinkingAfterSocialFaceExpression.get(2).toString().c_str());*/

            sendEyeBlinkingOutput(blinkingAfterSocialFaceExpression); 
        }
        
    }      
    return;
}

void AttentionReactionController::getFromCheckMutualGaze(bool data){ 
    if(_reactionSwitchOff){ 
        //printf("\n Controller tried to reach getFromCheckMutualGaze but reaction is OFF");
    }
    else{
        Bottle blinkingAfterMutualGaze = _eyeBlinking->sendMutualGazeToEyeBlinking(data); 
        if(blinkingAfterMutualGaze.size()>0){
            /*printf("\n Controller _blinkingType %s", blinkingAfterMutualGaze.get(0).toString().c_str());
            printf("\n Controller _blinkingFunction %s", blinkingAfterMutualGaze.get(1).toString().c_str());
            printf("\n Controller _blinkingDuration %s", blinkingAfterMutualGaze.get(2).toString().c_str());*/

            sendEyeBlinkingOutput(blinkingAfterMutualGaze); 
        }
    }
    return;
}

void AttentionReactionController::getFromRecognizeSpeech(bool speech){ 
    if(_reactionSwitchOff){ 
        //printf("\n Controller tried to reach getFromRecognizeSpeech but reaction is OFF");
    }
    else{
        if(speech){
            _recognizeSpeech = true;
            Bottle blinkingAfterChildSpoke = _eyeBlinking->sendChildSpokeToEyeBlinking(_recognizeSpeech); 
            if(blinkingAfterChildSpoke.size()>0){
                /*printf("\n Controller _blinkingType %s", blinkingAfterChildSpoke.get(0).toString().c_str());
                printf("\n Controller _blinkingFunction %s", blinkingAfterChildSpoke.get(1).toString().c_str());
                printf("\n Controller _blinkingDuration %s", blinkingAfterChildSpoke.get(2).toString().c_str());*/

                sendEyeBlinkingOutput(blinkingAfterChildSpoke); 
            }
        }
    
        else
        {
            printf("\n Controller recognizeSpeech has no content.");
        }
    }
    return;
}

void AttentionReactionController::getFromAffectiveState(int state){ 
    
    _affectiveState = state;
    if(_reactionSwitchOff){ 
        //printf("\n Controller tried to reach getFromAffectiveState but reaction is OFF");
    }
    else{
        Bottle blinkingAfterAffectiveState = _eyeBlinking->sendAffectiveStateToEyeBlinking(_affectiveState); 
        if(blinkingAfterAffectiveState.size()>0){
            /*printf("\n Controller _blinkingType %s", blinkingAfterAffectiveState.get(0).toString().c_str());
            printf("\n Controller _blinkingFunction %s", blinkingAfterAffectiveState.get(1).toString().c_str());
            printf("\n Controller _blinkingDuration %s", blinkingAfterAffectiveState.get(2).toString().c_str());*/

            sendEyeBlinkingOutput(blinkingAfterAffectiveState); 
        }
    }
    return;
}

void AttentionReactionController::getFromBalance(bool outOfBalance){
    _outOfBalance = outOfBalance;
    
    return;
}

void AttentionReactionController::sendAttentionOutput(double _outputCoordinateX,double _outputCoordinateY,double _outputCoordinateZ){
    /*send attention coordinates to Actuation subsystem*/
    Sleep(PAUSE);
    if(!_isBusy) {
        _yarpInterface->sendElicitedAttentionToActuation(_outputCoordinateX, _outputCoordinateY, _outputCoordinateZ);
    }
    else{
        printf("\n Controller tried to reach sendAttentionOutput but system is busy");
    }
   
    _gazeSent = false;
    return;
}

void AttentionReactionController::sendSocialReactionOutput(){
    /*send socialReaction to Actuation subsystem*/
    
    Sleep(PAUSE);
    if(_reactionSwitchOff){
	    _yarpInterface->sendSocialReactionToActuation(-1);
    
    }
    else{
        if(!_isBusy) {
        
            if((_micromotions)&&(_micromotions !=_micromotionsPrevious)){
	            _yarpInterface->sendSocialReactionToActuation(0);
                _micromotionsPrevious    = _micromotions;
    
            }
            if(_socialFaceExpression != _socialFaceExpressionPrevious){
                _yarpInterface->sendSocialFacialExpressionToActuation(_socialFaceExpression);
                _socialFaceExpressionPrevious = _socialFaceExpression;
                //_socialFaceExpression    = 0;
                //_micromotionsPrevious    = _micromotions;
                //_micromotions            = true;
            
            }
            /*if(_socialSpeech != 0){
                _yarpInterface->sendSocialReactionSpeechToActuation(_socialSpeech);
                _yarpInterface->sendSocialReactionToActuation(_conversationalGesture);
                _socialSpeech            = 0;
                _conversationalGesture   = 0;
            }*/
        
        }
        else{
            printf("\n Controller tried to reach sendSocialReactionOutput but system is busy");
        }
    }    
    return;
}


void AttentionReactionController::sendEyeBlinkingOutput(Bottle blinkingBehavior){
    
    if(!_reactionSwitchOff){
	    
        /*send eyeBlinking to Actuation subsystem*/
        Sleep(PAUSE);
        if(!_isBusy) {
            _yarpInterface->sendEyeBlinkingToActuation(blinkingBehavior);
        }
        else{
            printf("\n Controller tried to reach sendEyeBlinkingOutput but system is busy");
        }
    
        _gazeVariation = false;
        _recognizeSpeech = false;
        _eyeBlinkingBehavior.clear();
        _affectiveState = 0;
    }
    
    return;
}
void AttentionReactionController::sendFallingReactionOutput(int output){
    /*Send falling reaction or speech*/
    if(!_isBusy) {       
        
        if(output == 0) { /*Getting up*/
            _yarpInterface->sendFallingReactionToActuation(2);
        }
        if(output == 1) { /* If damage avoidance Send interruption to ACT and SM*/
            _yarpInterface->sendInterruptionToSelfMonitoring(1);
            _yarpInterface->sendFallingReactionToActuation(1);
            
        }
        if(output == 2) { /* If speech Send restore to ACT and SM*/
            _yarpInterface->sendInterruptionToSelfMonitoring(0);
            _yarpInterface->sendFallingReactionSpeechToActuation(1);
        }
    }
    else{
        printf("\n Controller tried to reach sendFallingReactionOutput but system is busy");
    }

    return;
}

//Method used to split a string
vector<string> AttentionReactionController::split(const string &s, char delim, vector<string> &elems){
    stringstream ss(s);
    string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> AttentionReactionController::split(const string &s, char delim){
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}
