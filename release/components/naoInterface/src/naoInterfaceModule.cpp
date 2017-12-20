 /** @file naoInterfaceModule.cpp
  *
  * Version 1.0
  * 
  * 15/08/2016
  *
  *
  * \defgroup DREAM_cognitiveComponent naoInterface
  *
  * Functionally, the module 
  * 
  * \section lib_sec Libraries
  *
  * YARP
  * Boost
  * Naoqi
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
  * <name of author>, <author institute>  
  * 
  * Copyright (C) 2014 DREAM Consortium
  * 
  */

 /* 
  * Audit Trail
  * -----------
  */ 
  
  /* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Emmanuel Senft, PLYM 
 * Email:   emmanuel.senft@plymouth.ac.uk 
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
 
#include "naoInterfaceModule.h"
#include "naoInterfaceController.h"
#include "moveSequenceId.h"
#include <althread/alcriticalsection.h>
#include <almath/types/alaxismask.h>
#include <algorithm>

#define M_PI       3.14159265358979323846
const int DEBUG = 1;

using namespace std;

NaoInterfaceModule::NaoInterfaceModule(boost::shared_ptr<AL::ALBroker> broker, const std::string &name)
  : AL::ALModule(broker, name) 
{
	// Describe the module here. This will appear on the webpage
	setModuleDescription("Robot control module.");

	_textToSpeechProxy = AL::ALTextToSpeechProxy(getParentBroker());
	_motionProxy = AL::ALMotionProxy(getParentBroker());
	_behaviorManagerProxy = AL::ALBehaviorManagerProxy(getParentBroker());
	_trackerProxy = AL::ALTrackerProxy(getParentBroker());
	_memoryProxy = AL::ALMemoryProxy(getParentBroker());
	_postureProxy = AL::ALRobotPostureProxy(getParentBroker());
	_basicAwarenessProxy = AL::ALBasicAwarenessProxy(getParentBroker());
	_ledsProxy.on("FaceLeds"); /*or set them randomly*/
	_colorLed = 0xffffff;
	_controller = NULL;
	//_textToSpeechProxy.getAvailableVoices();
	vector<string> availableLang = _textToSpeechProxy.getAvailableLanguages();
	if (std::find(availableLang.begin(), availableLang.end(), "Romanian") != availableLang.end()) {
		_textToSpeechProxy.setLanguage("Romanian");
	}
	else {
		cout << "Romanian not installed" << endl;
	}
	_motionProxy.setBreathEnabled("Body", false);
}

NaoInterfaceModule::~NaoInterfaceModule()
{
	 /*unsubscribe */
}

void NaoInterfaceModule::init()
{
	functionName("onLeftBumperPressed", getName(), "Left Bumper");
	BIND_METHOD(NaoInterfaceModule::onLeftBumperPressed);
	_memoryProxy.subscribeToEvent("LeftBumperPressed", "NaoInterfaceModule", "onLeftBumperPressed");

	functionName("onRightBumperPressed", getName(), "Right Bumper");
	BIND_METHOD(NaoInterfaceModule::onRightBumperPressed);
	_memoryProxy.subscribeToEvent("RightBumperPressed", "NaoInterfaceModule", "onRightBumperPressed");

	functionName("onFrontTactilTouched", getName(), "Front Tactil");
	BIND_METHOD(NaoInterfaceModule::onFrontTactilTouched);
	_memoryProxy.subscribeToEvent("FrontTactilTouched", "NaoInterfaceModule", "onFrontTactilTouched");

	functionName("onMiddleTactilTouched", getName(), "Middle Tactil");
	BIND_METHOD(NaoInterfaceModule::onMiddleTactilTouched);
	_memoryProxy.subscribeToEvent("MiddleTactilTouched", "NaoInterfaceModule", "onMiddleTactilTouched");

	functionName("onRearTactilTouched", getName(), "Rear Tactil");
	BIND_METHOD(NaoInterfaceModule::onRearTactilTouched);
	_memoryProxy.subscribeToEvent("RearTactilTouched", "NaoInterfaceModule", "onRearTactilTouched");

	functionName("onLeftHandTouched", getName(), "Left Hand");
	BIND_METHOD(NaoInterfaceModule::onLeftHandTouched);
	_memoryProxy.subscribeToEvent("HandLeftLeftTouched", "NaoInterfaceModule", "onLeftHandTouched");
	_memoryProxy.subscribeToEvent("HandLeftRightTouched", "NaoInterfaceModule", "onLeftHandTouched");
	_memoryProxy.subscribeToEvent("HandLeftBackTouched", "NaoInterfaceModule", "onLeftHandTouched");

	functionName("onRightHandTouched", getName(), "Rigth Hand");
	BIND_METHOD(NaoInterfaceModule::onRightHandTouched);
	_memoryProxy.subscribeToEvent("HandRightLeftTouched", "NaoInterfaceModule", "onRightHandTouched");
	_memoryProxy.subscribeToEvent("HandRightRightTouched", "NaoInterfaceModule", "onRightHandTouched");
	_memoryProxy.subscribeToEvent("HandRightBackTouched", "NaoInterfaceModule", "onRightHandTouched");

	functionName("onFalling", getName(), "Falling");
	BIND_METHOD(NaoInterfaceModule::onFalling);
	_memoryProxy.subscribeToEvent("robotIsFalling", "NaoInterfaceModule", "onFalling");

	functionName("hasFallen", getName(), "Fallen");
	BIND_METHOD(NaoInterfaceModule::onFalling);
	_memoryProxy.subscribeToEvent("robotHasFallen", "NaoInterfaceModule", "hasFallen");

	std::vector<std::string> names;
	names.push_back("FaceLedRight0");
	names.push_back("FaceLedRight1");
	names.push_back("FaceLedRight2");
	names.push_back("FaceLedRight7");
	names.push_back("FaceLedRight6");
	names.push_back("FaceLedLeft0");
	names.push_back("FaceLedLeft1");
	names.push_back("FaceLedLeft2");
	names.push_back("FaceLedLeft7");
	names.push_back("FaceLedLeft6");

	_ledsProxy.createGroup("HalfBlink", names);
}

void NaoInterfaceModule::setController(NaoInterfaceController* controller) {
	_controller = controller;
}

void NaoInterfaceModule::say(string sentence){
	//change speed
	 sentence = "\\RSPD=67\\ " + sentence;
	 _textToSpeechProxy.say(sentence);
}

bool NaoInterfaceModule::getBehaviourInstalled(string name){
	return _behaviorManagerProxy.isBehaviorInstalled(name);
}

void NaoInterfaceModule::move(vector<string> bodyPart, vector<vector<float>> angles, vector<vector<float>> executionTimes){
	AL::ALValue angleLists;
	AL::ALValue timeLists;
	AL::ALValue bodyPartLists;
	
	int jointNumber = angles.size();
	int partNumber = bodyPart.size(); 

	angleLists.arraySetSize(jointNumber);
	timeLists.arraySetSize(jointNumber);
	bodyPartLists.arraySetSize(partNumber);

	for(int i=0;i<jointNumber;i++){
		angleLists[i] = angles[i];
		timeLists[i] = executionTimes[i];
	}
   
   for(int i=0;i<partNumber;i++){
      bodyPartLists[i] = bodyPart[i];
   }

   _motionProxy.angleInterpolation(bodyPart, angleLists, timeLists, 1);
}

void NaoInterfaceModule::runBehaviour(string name){
	_behaviorManagerProxy.runBehavior(name);
}
void NaoInterfaceModule::blink(int repetitions, bool fullLeds, int duration) {
	string groupLeds;
	_ledsProxy.fadeRGB("FaceLeds", _colorLed, 0.0);

	if (fullLeds) 
		groupLeds = "FaceLeds";
	else 
		groupLeds = "HalfBlink";
	 
	for (int i = 0; i < repetitions; i++) {
		_ledsProxy.fadeRGB(groupLeds, 0x000000, double(duration) / 5. / 1000.); //blinking time shortened by a fourth for NAO as it has no eyelids to move and divided by 1000 to move from milliseconds to seconds
		_ledsProxy.fadeRGB("FaceLeds", _colorLed, 0.0);
	}
}

vector<float> NaoInterfaceModule::getAngles(vector<string> bodyPart){
	AL::ALValue bodyPartLists;
    int partNumber = bodyPart.size(); 

	bodyPartLists.arraySetSize(partNumber);
	for(int i=0;i<partNumber;i++){
		bodyPartLists[i] = bodyPart[i];
	}
	
   return _motionProxy.getAngles(bodyPartLists, true);
}

void NaoInterfaceModule::rest(){
   _motionProxy.rest();
}

void NaoInterfaceModule::wakeUp(){
   _motionProxy.wakeUp();
}

void NaoInterfaceModule::stand() {
	_postureProxy.goToPosture("Stand",0.2);
}

void NaoInterfaceModule::crouch(){
	_motionProxy.rest();
	_motionProxy.setStiffnesses("Body",0.7);
}

void NaoInterfaceModule::closeHand(bool rightHand, bool close) {
	if (close)
		_motionProxy.closeHand(rightHand ? "RHand" : "LHand");
	else
		_motionProxy.openHand(rightHand ? "RHand" : "LHand");
}

void NaoInterfaceModule::moveHand(bool rightHand, float x, float y, float z, float roll) {
	std::string chainName = rightHand ? "RArm" : "LArm";
	int space = 2;
	std::vector<float> angleLists(6, 0.0f);
	angleLists[0] = x;
	angleLists[1] = y;
	angleLists[2] = z;
	angleLists[3] = roll / 360. * 2 * M_PI;
	if (DEBUG)
		cout << "-naoInterface- moveHand x "<<x<<" y "<<y<<" z "<<z<<" roll "<<roll / 360. * 2 * M_PI << endl;
	float times = 3.f;
	int axisMask = AL::Math::AXIS_MASK_ALL;
	_motionProxy.positionInterpolations(chainName, space, angleLists, axisMask, times);
}

void NaoInterfaceModule::moveHand(bool rightHand, float x, float y, float z) {
	vector<float> vec;
	vec.push_back(x);
	vec.push_back(y);
	vec.push_back(z);

	string effector = rightHand ? "RArm" : "LArm";

	_trackerProxy.pointAt(effector, vec, 0.1);
}


void NaoInterfaceModule::moveHead(float x, float y, float z, float speed) {
	vector<float> vec;
	vec.push_back(x);
	vec.push_back(y);
	vec.push_back(z);
	_trackerProxy.lookAt(vec, 0, speed, 0);
}

void NaoInterfaceModule::setMicromotions(bool enable) {
	AL::ALValue breathConfig;
    breathConfig.arraySetSize(2);

    AL::ALValue tmp;
    tmp.arraySetSize(2);
    tmp[0] = "Bpm";
    tmp[1] = 12.0f;
    breathConfig[0] = tmp;

    tmp[0] = "Amplitude";
    tmp[1] = 0.5f;
    breathConfig[1] = tmp;
    _motionProxy.setBreathConfig(breathConfig);
        
    
    if (enable) {
		//_autonomousLifeProxy.setRobotOffsetFromFloor(0.6); //Specifying that the robot is placed 0.6 meters above the ground (according to the workbench specifications
		//_autonomousLifeProxy.setState("solitary"); 
        /**************************************************/
        //printf("\n Autonomous Life should go ON now. \n");
        _motionProxy.setBreathEnabled("Legs", true);
        _motionProxy.setBreathEnabled("Arms", true);
        _motionProxy.setBreathEnabled("Body", true);
        _motionProxy.setBreathConfig(breathConfig);

		_basicAwarenessProxy.setEngagementMode("SemiEngaged");
		_basicAwarenessProxy.startAwareness();
        
        //_motionProxy.setBreathConfig([['Bpm', 12], ['Amplitude', 0.5]]);
	}
	else{
        //printf("\n Autonomous Life should go OFF now. \n");
        _motionProxy.setBreathEnabled("Legs", false);
        _motionProxy.setBreathEnabled("Arms", false);
        _motionProxy.setBreathEnabled("Body", false);

		_basicAwarenessProxy.stopAwareness();

        //moveHead(1,0,0,0.2); //To center back its gaze		
        //bool enabled = _motionProxy.getBreathEnabled("Body");
		//
        //_autonomousLifeProxy.setState("disabled"); 
        /**************************************************/
        
	}
}

void NaoInterfaceModule::setFace(int id) {
	_ledsProxy.on("FaceLeds");
	string name = "FaceLeds";
	float duration = 1.0;

	switch(id) {
	case _MOVE_HAPPY_FACE_: _colorLed = 0xF008000; //Green
		break;
	case _MOVE_SAD_FACE_: _colorLed = 0x0000ff; //Blue
		break;
	case _MOVE_ANGRY_FACE_: _colorLed = 0xff0000; //Red
		break;
	case _MOVE_FEAR_FACE_: _colorLed = 0xffff00; //Yellow
		break;
	case _MOVE_NEUTRAL_FACE_: _colorLed = 0xffffff; //white
		duration = 5.0;
		break;
	}
	_ledsProxy.fadeRGB(name, _colorLed, duration);
}

void NaoInterfaceModule::takePosition(vector<string> bodyList, vector<float> angles) {
	vector<float> time(angles.size(), 3.0);
	bool isAbsolute = 1;
	_motionProxy.angleInterpolation(bodyList, angles, time, isAbsolute);
}


void NaoInterfaceModule::onLeftBumperPressed() {
	float state = _memoryProxy.getData("LeftBumperPressed");
	_controller->sensors("LeftBumper", state>0.5);
}

void NaoInterfaceModule::onRightBumperPressed() {
	float state = _memoryProxy.getData("RightBumperPressed");
	_controller->sensors("RightBumper", state>0.5);
}

void NaoInterfaceModule::onLeftHandTouched() {
	float state = (float)(_memoryProxy.getData("HandLeftLeftTouched")) + (float)(_memoryProxy.getData("HandLeftRightTouched")) + (float)(_memoryProxy.getData("HandLeftBackTouched"));
	_controller->sensors("LeftHand", state>0.5);
}

void NaoInterfaceModule::onRightHandTouched() {
	float state = (float)(_memoryProxy.getData("HandRightLeftTouched")) + (float)(_memoryProxy.getData("HandRightRightTouched")) + (float)(_memoryProxy.getData("HandRightBackTouched"));
	_controller->sensors("RightHand", state>0.5);
}
void NaoInterfaceModule::onFrontTactilTouched() {
	float state = _memoryProxy.getData("FrontTactilTouched");
	_controller->sensors("FrontTactil", state>0.5);
}

void NaoInterfaceModule::onMiddleTactilTouched() {
	float state = _memoryProxy.getData("MiddleTactilTouched");
	_controller->sensors("MiddleTactil", state>0.5);
}

void NaoInterfaceModule::onRearTactilTouched() {
	float state = _memoryProxy.getData("RearTactilTouched");
	_controller->sensors("RearTactil", state>0.5);
}

void NaoInterfaceModule::onFalling() {
	_controller->sensors("Falling", 1);
}

void NaoInterfaceModule::hasFallen() {
	_controller->sensors("Falling", 0);
}

void NaoInterfaceModule::setVolume(float value) {
	_textToSpeechProxy.setVolume(value);
}

void NaoInterfaceModule::stopAll() {
	_behaviorManagerProxy.stopAllBehaviors();
}