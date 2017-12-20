/* 
 * Copyright (C) 2015 DREAM Consortium
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
 * 13/08/15  First version validated (Pablo Gomez)
 */  
 

#include "actuationSimulator.h"

ActuationSimulatorThread::ActuationSimulatorThread(
										    BufferedPort<Bottle>			   *getEyeBlinkingIn,
											BufferedPort<VectorOf<int>>        *getFallingReactionIn,
											BufferedPort<VectorOf<int>>		   *getFallingReactionSpeechIn,
											BufferedPort<VectorOf<int>>        *getSocialReactionIn,
											BufferedPort<VectorOf<int>>		   *getSocialReactionSpeechIn,
											BufferedPort<VectorOf<int>>        *getSocialFacialExpressionIn,
											BufferedPort<VectorOf<int>>        *getInterruptionIn,
											BufferedPort<Bottle>			   *sayOut,
											BufferedPort<Bottle>			   *moveSequenceOut,
											BufferedPort<Bottle>               *motorFeedbackIn
										   )
{
   getEyeBlinkingPortIn				    = getEyeBlinkingIn;
   getFallingReactionPortIn			    = getFallingReactionIn;
   getFallingReactionSpeechPortIn       = getFallingReactionSpeechIn;
   getSocialReactionPortIn			    = getSocialReactionIn;
   getSocialReactionSpeechPortIn	    = getSocialReactionSpeechIn;
   getSocialFacialExpressionPortIn	    = getSocialFacialExpressionIn;
   getInterruptionPortIn			    = getInterruptionIn;
   sayPortOut					        = sayOut;
   moveSequencePortOut				    = moveSequenceOut;
   motorFeedbackPortIn				    = motorFeedbackIn;
   
   _statusRef["CONNECTIONFAIL"]         = 0;
   _statusRef["ISIMPOSSIBLE"]           = 1;
   _statusRef["ISCANCELED"]             = 2;
   _statusRef["ISWAITING"]              = 3;
   _statusRef["ISAVAILLABLE"]           = 4;
   _statusRef["ISCOMPLETE"]             = 5;
   _statusRef["ISSTOPPED"]              = 6;
   _statusRef["ISSTARTED"]              = 7;
   _statusRef["ISFAILED"]               = 8;
   //_subId = -1;
}

bool ActuationSimulatorThread::threadInit() 
{
   /* initialize variables and create data-structures if needed */
   debug = false;

   
   return true;
}

void ActuationSimulatorThread::run(){

   /* 
    * This module will simulate the behavior of the actuation subsystem by producing the corresponding action primitives given the commands from other subsystems. So far only the 
	* reactive subsystem will send its outputs.
    */ 
	_isBusy = false;


	Bottle              *blinkingBehaviorVector;
	string               blinkingType;
	string               blinkingLenght;
	string               blinkingSpeed;
	char                 blinkingBehavior[STRINGLENGTH]; 
	string               blinkingBehaviorString;
	
	Bottle              *motorFeedbackVector;
	
	VectorOf<int>       *getInterruptionIntVector;
    int                  getInterruption;
	
	VectorOf<int>       *getFallingReactionIntVector;
    int                  fallingReaction;
	int                  fallingReactionPrevious            = 0;


	VectorOf<int>       *socialSpeechVector;
	int                  socialSpeechTaste;
	
	VectorOf<int>       *fallingSpeechVector;
	int                  fallingSpeechActivation;
    
	VectorOf<int>       *socialFaceExpressionIntVector;
    int                  socialFaceExpression;
	int                  previousSocialFaceExpression       = 0;
    
	VectorOf<int>       *socialReactionIntVector;
    int                  socialReaction;
	int                  socialReactionPrevious             = 0;
	
	int                  random;

	srand (time(NULL));
	
	while (isStopping() != true) { // the thread continues to run until isStopping() returns true
		
		motorFeedbackVector = motorFeedbackPortIn->read(false);                  
	    
		if (motorFeedbackVector != NULL) { 
			
			vector<string> inputs = split((*motorFeedbackVector).get(0).asString(), ' ');
			int status=_statusRef[inputs[2]];
			if(actionFinished(status)){      //check if the status is terminating, if yes, check if link should be activated
				
				_isBusy = false;
			}
			else{
			
				_isBusy = true;
			}
		}
		if(!_isBusy){
			
			_isBusy = true;
			Bottle &sequenceVector = moveSequencePortOut->prepare();
			sequenceVector.clear();
		
			getInterruptionIntVector = getInterruptionPortIn->read(false);                   
      
			if (getInterruptionIntVector != NULL) {

			    getInterruption = (int) (*getInterruptionIntVector)[0];
		    }
			  
			if(getInterruption == 1){ /*System will to be interrupted by the self monitoring system. Now the GUI blocks any interaction*/  

				  /*Implement fallingReaction damage avoidance behavior*/

				  getFallingReactionIntVector = getFallingReactionPortIn->read(false);                  
      
				  if (getFallingReactionIntVector != NULL) {

					 fallingReaction = (int) (*getFallingReactionIntVector)[0];
				  }	
				  if((fallingReaction == 1)&&(fallingReaction != fallingReactionPrevious)){
			
					  sequenceVector.clear();
					  sequenceVector.addString("damageAvoidance");
					  moveSequencePortOut->write(); 
					  fallingReactionPrevious = fallingReaction;

                      /*This information is sent but not implemented in Nao as it already includes a falling reaction module by default*/
				  } 
				  else if((fallingReaction == 2)&&(fallingReaction != fallingReactionPrevious)){
					  
					  sequenceVector.clear();
					  sequenceVector.addString("gettingUp");
					  moveSequencePortOut->write(); 
					  fallingReactionPrevious = fallingReaction;

                      /*This information is sent but not implemented in Nao as it already includes a falling reaction module by default*/
				  }

			  }
			  else{
				  
				/*Social facial expression*/
			    socialFaceExpressionIntVector = getSocialFacialExpressionPortIn->read(false);                   
      
			    if (socialFaceExpressionIntVector != NULL) {
				  
					socialFaceExpression = (int) (*socialFaceExpressionIntVector)[0];
				}
				if ((socialFaceExpression == 1)&&(previousSocialFaceExpression != socialFaceExpression)){ // Express happiness
				  
				    //HappyFace is called
				    sequenceVector.clear();
				    sequenceVector.addString("happyFace");
				    moveSequencePortOut->write(); 
				  
				    previousSocialFaceExpression = socialFaceExpression;
				  
			    }
			    else if ((socialFaceExpression == 2)&&(previousSocialFaceExpression != socialFaceExpression)){ // Express sadness
				  
				    //sadFace is called
				    sequenceVector.clear();
				    sequenceVector.addString("sadFace");
				    moveSequencePortOut->write(); 
				    previousSocialFaceExpression = socialFaceExpression;				  
			    }
			    else if ((socialFaceExpression == 3)&&(previousSocialFaceExpression != socialFaceExpression)){ // Express anger
				 
				    //AngryFace is called
				    sequenceVector.clear();
				    sequenceVector.addString("angryFace");
				    moveSequencePortOut->write(); 
				    previousSocialFaceExpression = socialFaceExpression;
			    }
			    else if ((socialFaceExpression == 4)&&(previousSocialFaceExpression != socialFaceExpression)){ // Express fearful
				  
				    //fearFace is called
				    sequenceVector.clear();
				    sequenceVector.addString("fearFace");
				    moveSequencePortOut->write(); 
				    previousSocialFaceExpression = socialFaceExpression;
			    }
			    else if ((socialFaceExpression == 0)&&(previousSocialFaceExpression != socialFaceExpression)){ // Express neutral face
				  
				    //neutralFace is called
				    sequenceVector.clear();
				    sequenceVector.addString("neutralFace");
				    moveSequencePortOut->write(); 
				    previousSocialFaceExpression = socialFaceExpression;
			    }
	  
			    /*eye blinking behavior*/

				blinkingBehaviorVector = getEyeBlinkingPortIn->read(false);                  
				if (blinkingBehaviorVector != NULL) {
			
					strcpy(blinkingBehavior, "blinking_");
					strcat(blinkingBehavior, (*blinkingBehaviorVector).get(0).toString().c_str());//single, double or triple
					strcat(blinkingBehavior, "_");
					strcat(blinkingBehavior, (*blinkingBehaviorVector).get(1).toString().c_str());//full or half
					strcat(blinkingBehavior, "_");
					strcat(blinkingBehavior, (*blinkingBehaviorVector).get(2).toString().c_str());//duration
					
					stringstream ss;
					ss << blinkingBehavior;
					ss >> blinkingBehaviorString;
					//blinking is called
					sequenceVector.addString(blinkingBehaviorString);
					moveSequencePortOut->write(); 
			    }

				/*Social reaction*/
			    socialReactionIntVector = getSocialReactionPortIn->read(false);                   
      
			    if (socialReactionIntVector != NULL) {

				    socialReaction = (int) (*socialReactionIntVector)[0];
			    }
			    if((socialReaction == 10)&&(socialReaction != socialReactionPrevious)){ // Gaze forward
				    //moveHead(x,y,z);
				    socialReactionPrevious = socialReaction;
		  
			    }
			    else if((socialReaction == 11)&&(socialReaction != socialReactionPrevious)){ // Gaze left hand
				    //moveHead(x,y,z);
				    socialReactionPrevious = socialReaction;
			    }
			    else if((socialReaction == 12)&&(socialReaction != socialReactionPrevious)){ // Gaze right hand
				    //moveHead(x,y,z);
				    socialReactionPrevious = socialReaction;
			    }
			    else if((socialReaction == 13)&&(socialReaction != socialReactionPrevious)){ // Gaze left foot
				    //moveHead(x,y,z);
				    socialReactionPrevious = socialReaction;
			    }
			    else if((socialReaction == 14)&&(socialReaction != socialReactionPrevious)){ // Gaze right foot
				    //moveHead(x,y,z);
				    socialReactionPrevious = socialReaction;
			    }
			    else if((socialReaction == 0)&&(socialReaction != socialReactionPrevious)){ // Micromotions
				    //SocialReaction is called
					sequenceVector.addString("micromotions");
					moveSequencePortOut->write();
					socialReactionPrevious = socialReaction;
			    }
			
				Bottle &sayVector = sayPortOut->prepare();
				sayVector.clear();
		
				/*Falling speech*/

				fallingSpeechVector = getFallingReactionSpeechPortIn->read(false);                   
      
				if (fallingSpeechVector != NULL) {
				    fallingSpeechActivation = (int) (*fallingSpeechVector)[0];
					if(fallingSpeechActivation==1){
					    /*Randomize among a set of falling sentences*/
						random = (int)(rand() %10)/NUMBER_OF_SENTENCES;
						if (random == 0){
						    sayVector.clear();
							sayVector.addString("Ouch");
							sayPortOut->write();
						}
						else if (random == 1){
						    sayVector.clear();
							sayVector.addString("Sorry, today I am a bit clumsy");
							sayPortOut->write();
						}
						else if (random == 2){
						    sayVector.clear();
							sayVector.addString("I am okay, do not worry");
							sayPortOut->write();
						}
						else if (random == 3){
						    sayVector.clear();
							sayVector.addString("Ups I just fell, but I am okay");
							sayPortOut->write();
						}
						else if (random == 4){
						    sayVector.clear();
							sayVector.addString("the floor is slippery today");
							sayPortOut->write();
						}
					}
                }
				else{ // So that no social speech will happen if a falling speech is triggered
				    /*Social speech*/

				    socialSpeechVector = getSocialReactionSpeechPortIn->read(false);                   
       		        if (socialSpeechVector != NULL) {
				        socialSpeechTaste = (int) (*socialSpeechVector)[0];
								  
					    if(socialSpeechTaste > 0){ //Affective social speech
					        /*Randomize among a set of affective sentences*/
						    random = (int)(rand() %10)/NUMBER_OF_SENTENCES;
						    if (random == 0){
						        sayVector.clear();
							    sayVector.addString("you are really nice");
							    sayPortOut->write();
                            }
						    else if (random == 1){
							    sayVector.clear();
							    sayVector.addString("that was great");
							    sayPortOut->write();
						    }
						    else if (random == 2){
							    sayVector.clear();
							    sayVector.addString("I am having fun today");
							    sayPortOut->write();
						    }
						    else if (random == 3){
							    sayVector.clear();
							    sayVector.addString("I like spending time with you");
							    sayPortOut->write();
						    }
						    else if (random == 4){
							    sayVector.clear();
							    sayVector.addString("I am glad we are playing together");
							    sayPortOut->write();
                            }
                        }
					    else if (socialSpeechTaste <0){ //Not affective social speech
					        /*Randomize among a set of not affective sentences*/
						    random = (int)(rand() %10)/NUMBER_OF_SENTENCES;
						    if (random == 0){
						        sayVector.clear();
							    sayVector.addString("I did not like that");
							    sayPortOut->write();
						    }
						    else if (random == 1){
							    sayVector.clear();
							    sayVector.addString("that hurted a bit");
							    sayPortOut->write();
						    }
						    else if (random == 2){
							    sayVector.clear();
							    sayVector.addString("oooh");
							    sayPortOut->write();
						    }
						    else if (random == 3){
							    sayVector.clear();
							    sayVector.addString("please do not do that again");
							    sayPortOut->write();
						    }
						    else if (random == 4){
							    sayVector.clear();
							    sayVector.addString("I prefer to play another game");
							    sayPortOut->write();
                            }
                        }
                    }
                }
            }
		    _isBusy = false;
		}
	}
}

void ActuationSimulatorThread::threadRelease() 
{
   /* for example, delete dynamically created data-structures */
}

//Method used to split a string
vector<string> ActuationSimulatorThread::split(const string &s, char delim, vector<string> &elems){
    stringstream ss(s);
    string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> ActuationSimulatorThread::split(const string &s, char delim){
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}
//return if a status is terminating or not
bool ActuationSimulatorThread::actionFinished(int status){
   if(status == _statusRef["CONNECTIONFAIL"]) return 0;//1
   if(status == _statusRef["ISIMPOSSIBLE"]  ) return 0;//1
   if(status == _statusRef["ISCANCELED"]    ) return 1;
   if(status == _statusRef["ISWAITING"]     ) return 0;
   if(status == _statusRef["ISAVAILLABLE"]  ) return 0;
   if(status == _statusRef["ISCOMPLETE"]    ) return 1;
   if(status == _statusRef["ISSTOPPED"]     ) return 1;
   if(status == _statusRef["ISSTARTED"]     ) return 0;
   if(status == _statusRef["ISFAILED"]      ) return 1;
   return 0;
}
