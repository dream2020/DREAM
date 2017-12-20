  /** @file eyeBlinking.cpp 
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
 

#include "eyeBlinking.h"
#include "attentionReactionYarpInterface.h"
using namespace std;

EyeBlinkingThread::EyeBlinkingThread(AttentionReactionYarpInterface* yarp){
    _yarpInterface = yarp;
    _gazeVariation= false;
    _gazeVariationPrevious= false;
    _mutualGaze= false;
    _recognizeSpeech = false;
    _mutualGazePrevious= false;
    _socialFaceExpression = 0;
    _socialFaceExpressionPrevious = 0;
    _affectiveState = 0;
    _affectiveStatePrevious = 0;
    _probabilityBlink = 0.0;
    _numberOcurrences = 0;
    _blinkAction.clear();
}

bool EyeBlinkingThread::threadInit() 
{
    
   return true;
}

void EyeBlinkingThread::threadRelease() 
{
   /* for example, delete dynamically created data-structures */
}
void EyeBlinkingThread::run(){
    
}


Bottle EyeBlinkingThread::blinkMorphology() 
{
   
   Bottle blink;
   double duration  = 0.0;
   double aux       = (double)(rand() %100)/100;
   blink.clear();
   if(aux < 0.85){ //Single blink
	   blink.addString("Single");
   }
   else{
	   aux = (double)(rand() %100)/100;
   
	   if(aux < 0.8){ //double blink
		   blink.addString("Double");
	   }
	   else{ //triple blink
		   blink.addString("Triple");
	   }
   }

   aux = (double)(rand() %100)/100;
   
   if(aux < 0.91){ //Full blink
	   blink.addString("Full");
	   //Blink duration with 432ms as mean and 72ms as standard deviation (random number between 360 ms and 504 ms)
	   aux = (double)(rand()%144)/100; // Base (0.36) + a value between the range of [0-144] which is the difference between 504ms and 360ms
	   duration = 0.36 + aux;
	   ostringstream convert;
	   convert << duration;
	   blink.addString(convert.str()); 
   }
   else{ //half blink
	   blink.addString("Half");
	   //Blink duration with 266ms as mean and 4ms as standard deviation (random number between 262 ms and 270 ms)
	   aux = (double)(rand()%8)/100; // Base (0.262) + a value between the range of [0-8] which is the difference between 270ms and 262ms
	   duration = 0.262+aux;
	   ostringstream convert;
	   convert << duration;
	   blink.addString(convert.str());  
   }
   Sleep(PAUSE);
   return blink;
}

Bottle EyeBlinkingThread::sendMutualGazeToEyeBlinking(bool mutualGaze){
    
    _mutualGaze = mutualGaze;
    Bottle blinkingAfterMutualGaze = getEyeBlinkingBehavior();
    return blinkingAfterMutualGaze;
}

Bottle EyeBlinkingThread::sendChildSpokeToEyeBlinking(bool recognizeSpeech){
    _recognizeSpeech = recognizeSpeech;
    Bottle blinkingAfterChildSpoke = getEyeBlinkingBehavior();
    return blinkingAfterChildSpoke;
}

Bottle EyeBlinkingThread::sendAffectiveStateToEyeBlinking(int affectiveState){
    _affectiveState = affectiveState;
    Bottle blinkingAfterAffectiveState = getEyeBlinkingBehavior();
    return blinkingAfterAffectiveState;
}

Bottle EyeBlinkingThread::sendSocialFaceExpressionToEyeBlinking(int socialFaceExpression){
    _socialFaceExpression = socialFaceExpression;
    Bottle blinkingAfterSocialFaceExpression = getEyeBlinkingBehavior();
    return blinkingAfterSocialFaceExpression;
}

Bottle EyeBlinkingThread::sendGazeVariationToEyeBlinking(bool gazeVariation){
    _gazeVariation = gazeVariation;
    Bottle blinkingAfterGazeVariation = getEyeBlinkingBehavior();
    return blinkingAfterGazeVariation;
}

Bottle EyeBlinkingThread::getEyeBlinkingBehavior(){
    
    _blinkAction.clear();   
    if((_gazeVariation && _mutualGaze)&&((_gazeVariation != _gazeVariationPrevious)||(_mutualGaze != _mutualGazePrevious))){ 
        //Robot change its gaze to look at the child
		_probabilityBlink += 0.61;
		_numberOcurrences++;
        _gazeVariationPrevious = _gazeVariation;
	    _mutualGazePrevious = _mutualGaze;
        _mutualGaze=false;
        _gazeVariation=false;
	}
    else if((_gazeVariation && !_mutualGaze)&&((_gazeVariation != _gazeVariationPrevious)||(_mutualGaze != _mutualGazePrevious))){ 
        //Robot change its gaze to look away the child
		_probabilityBlink += 0.72;
		_numberOcurrences++;
        _gazeVariationPrevious = _gazeVariation;
	    _mutualGazePrevious = _mutualGaze;
        _gazeVariation=false;
	}
    if ((_affectiveState !=0)&&(_affectiveState != _affectiveStatePrevious)){ 
		/* Facial expressions are defined in advance and identified by an integer. When such
		integer is zero, no expression is shown.*/
		_probabilityBlink += 0.285;
		_numberOcurrences++;
        _affectiveStatePrevious = _affectiveState;
	}
    if ((_socialFaceExpression != 0)&&(_socialFaceExpression != _socialFaceExpressionPrevious)){ 
		/* Facial expressions are defined in advance and identified by an integer. When such
		integer is zero, no expression is shown.*/
		_probabilityBlink += 0.285;
		_numberOcurrences++;
        _socialFaceExpressionPrevious = 0;
	}
    if (_recognizeSpeech == true) {

		_probabilityBlink += 0.31;
		_numberOcurrences++;
        _recognizeSpeech = false;
	} 

    double aux = ((double)_numberOcurrences/50);
    if(_numberOcurrences>0){
    
        _probabilityBlink = (double)(_probabilityBlink+aux)/(double)(BLINKING_RATE+aux); //Probability normalize between 0 and 1.
    }
   
	double threshold = (double)(rand() %100)/100;
			
	if(threshold < _probabilityBlink){ //A blink occurs
		//call to blink morphology
		_blinkAction = blinkMorphology();
        _numberOcurrences = 0;
        _probabilityBlink = 0.0;
	}
	else { //No blink happened by interaction
		double threshold = (double)(rand() %100)/100;
				
		if(threshold > RATIO_BLINKING){ //A blink occurs. 
			//call to blink morphology
			_blinkAction = blinkMorphology();
            _numberOcurrences = 0;
            _probabilityBlink = 0.0;
		}
	}
    return _blinkAction;
}
