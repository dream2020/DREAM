 /** @file naoInterfaceModule.h
  *
  * Version 1.0
  * 
  * 12/02/2015
  *
  *
  * \defgroup DREAM_cognitiveComponent naoInterface
  *
  * Functionally, the module 
  * 
  * \section lib_sec Libraries
  *
  * YARP.
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

#ifndef NAOINTERFACEMODULE
#define NAOINTERFACEMODULE

#include <iostream>
#include <string>
#include <vector>

#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>

#include <alproxies/albasicawarenessproxy.h>
#include <alproxies/altexttospeechproxy.h>
#include <alproxies/almotionproxy.h>
#include <alproxies/albehaviormanagerproxy.h>
#include <alproxies/altrackerproxy.h>
#include <alproxies/alledsproxy.h>
#include <alproxies/alautonomouslifeproxy.h>
#include <alproxies/alrobotpostureproxy.h>
#include <alproxies/altouchproxy.h>
#include <alproxies/almemoryproxy.h>
#include <althread/almutex.h>

#define Z_OFFSET 30;

class NaoInterfaceController;

class NaoInterfaceModule : public AL::ALModule{
	public:
	NaoInterfaceModule(boost::shared_ptr<AL::ALBroker> broker, const std::string &name);
	virtual ~NaoInterfaceModule();
	
	void say(std::string sentence);
	void move(std::vector<std::string> bodyPart, std::vector<std::vector<float>> angles, std::vector<std::vector<float>> executionTime);
	void runBehaviour(std::string name);
	void blink(int repetitions, bool fullLeds, int duration);
	bool getBehaviourInstalled(std::string name);
	void rest();
	void wakeUp();
	void stand();
	void crouch();
	void closeHand(bool rightHand, bool close);
	std::vector<float> getAngles(std::vector<std::string> bodyPart);
	void moveHand(bool rightHand, float x, float y, float z);
	void moveHand(bool rightHand, float x, float y, float z, float roll);
	void moveHead(float x, float y, float z, float speed);
	void setMicromotions(bool enable);
	void setFace(int id);
	void takePosition(std::vector<std::string> bodyList,std::vector<float> vecAngles);
	void setController(NaoInterfaceController* controller);
	void setVolume(float value);
	void stopAll();
		
   /**
	* Overloading ALModule::init().
	* This is called right after the module has been loaded
	*/

	virtual void init();
	
	private:
	void onLeftBumperPressed();
	void onRightBumperPressed();
	void onLeftHandTouched();
	void onRightHandTouched();
	void onFrontTactilTouched();
	void onMiddleTactilTouched();
	void onRearTactilTouched();
	void onFalling();
	void hasFallen();
	
	NaoInterfaceController* _controller;

	AL::ALTextToSpeechProxy _textToSpeechProxy;
	AL::ALMotionProxy _motionProxy;
	AL::ALBehaviorManagerProxy _behaviorManagerProxy;
	AL::ALTrackerProxy _trackerProxy;
	AL::ALLedsProxy _ledsProxy;
	AL::ALMemoryProxy _memoryProxy;
	AL::ALRobotPostureProxy _postureProxy;
    AL::ALBasicAwarenessProxy _basicAwarenessProxy;
	boost::shared_ptr<AL::ALMutex> _callbackMutex;
	int _colorLed;
};

#endif