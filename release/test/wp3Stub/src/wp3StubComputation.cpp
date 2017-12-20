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

 
 /* 
  * Audit Trail
  * -----------
  * 20/09/2016 Version 1.0: ... (Emmanuel Senft)
  */ 

#include "wp3Stub.h"
#include "wp3StubYarpInterface.h"

Wp3StubThread::Wp3StubThread(BufferedPort<VectorOf<double>>* getEyeGazeIn, BufferedPort<VectorOf<double>>* getGripLocationIn, BufferedPort<VectorOf<double>>* getHeadGazeIn, BufferedPort<VectorOf<double>>* getObjectTableDistanceIn, BufferedPort<VectorOf<double>>* getObjectsIn, BufferedPort<VectorOf<double>>* identifyFaceIn, BufferedPort<VectorOf<double>>* identifyFaceExpressionIn, BufferedPort<VectorOf<double>>* identifyObjectIn, BufferedPort<VectorOf<int>>* identifyTrajectoryIn, BufferedPort<VectorOf<double>>* trackFaceIn, BufferedPort<VectorOf<double>>* trackObjectIn, BufferedPort<VectorOf<double>>* trackHandIn, BufferedPort<VectorOf<double>>* getSoundDirectionIn, BufferedPort<VectorOf<int>>* checkMutualGazeOut, BufferedPort<VectorOf<double>>* getFacesOut, BufferedPort<VectorOf<double>>* getSoundDirectionOut, BufferedPort<VectorOf<double>>* identifyFaceExpressionOut, BufferedPort<Bottle>* recognizeSpeechOut, BufferedPort<VectorOf<double>>* getEyeGazeOut, BufferedPort<VectorOf<double>>* getEyesOut, BufferedPort<VectorOf<double>>* getArmAngleOut, BufferedPort<VectorOf<double>>* getBodyOut, BufferedPort<VectorOf<double>>* getGripLocationOut, BufferedPort<VectorOf<double>>* getBodyPoseOut, BufferedPort<VectorOf<double>>* getHandsOut, BufferedPort<VectorOf<double>>* getHeadOut, BufferedPort<VectorOf<double>>* getHeadGazeOut, BufferedPort<VectorOf<double>>* getObjectsOut, BufferedPort<VectorOf<double>>* getObjectTableDistanceOut, BufferedPort<VectorOf<double>>* identifyFaceOut, BufferedPort<VectorOf<double>>* identifyObjectOut, BufferedPort<VectorOf<double>>* identifyTrajectoryOut, BufferedPort<VectorOf<int>>* identifyVoiceOut, BufferedPort<VectorOf<double>>* trackFaceOut, BufferedPort<VectorOf<double>>* trackObjectOut, BufferedPort<VectorOf<double>>* getHeadGazePointOut){
	/* assign the input and output ports */
	_getEyeGazeIn = getEyeGazeIn;
	_getGripLocationIn = getGripLocationIn;
	_getHeadGazeIn = getHeadGazeIn;
	_getObjectTableDistanceIn = getObjectTableDistanceIn;
	_getObjectsIn = getObjectsIn;
	_identifyFaceIn = identifyFaceIn;
	_identifyFaceExpressionIn = identifyFaceExpressionIn;
	_identifyObjectIn = identifyObjectIn;
	_identifyTrajectoryIn = identifyTrajectoryIn;
	_trackFaceIn = trackFaceIn;
	_trackObjectIn = trackObjectIn;
	_trackHandIn = trackHandIn;
	_getSoundDirectionIn = getSoundDirectionIn;
	_checkMutualGazeOut = checkMutualGazeOut;
	_getFacesOut = getFacesOut;
	_getSoundDirectionOut = getSoundDirectionOut;
	_identifyFaceExpressionOut = identifyFaceExpressionOut;
	_recognizeSpeechOut = recognizeSpeechOut;
	_getEyeGazeOut = getEyeGazeOut;
	_getEyesOut = getEyesOut;
	_getArmAngleOut = getArmAngleOut;
	_getBodyOut = getBodyOut;
	_getGripLocationOut = getGripLocationOut;
	_getBodyPoseOut = getBodyPoseOut;
	_getHandsOut = getHandsOut;
	_getHeadOut = getHeadOut;
	_getHeadGazeOut = getHeadGazeOut;
	_getObjectsOut = getObjectsOut;
	_getObjectTableDistanceOut = getObjectTableDistanceOut;
	_identifyFaceOut = identifyFaceOut;
	_identifyObjectOut = identifyObjectOut;
	_identifyTrajectoryOut = identifyTrajectoryOut;
	_identifyVoiceOut = identifyVoiceOut;
	_trackFaceOut = trackFaceOut;
	_trackObjectOut = trackObjectOut;
	_getHeadGazePointOut = getHeadGazePointOut;
   
	_getEyeGazeInAnalyser = new GetEyeGazeInAnalyser();
	_getGripLocationInAnalyser = new GetGripLocationInAnalyser();
	_getHeadGazeInAnalyser = new GetHeadGazeInAnalyser();
	_getObjectTableDistanceInAnalyser = new GetObjectTableDistanceInAnalyser();
	_getObjectsInAnalyser = new GetObjectsInAnalyser();
	_identifyFaceInAnalyser = new IdentifyFaceInAnalyser();
	_identifyFaceExpressionInAnalyser = new IdentifyFaceExpressionInAnalyser();
	_identifyObjectInAnalyser = new IdentifyObjectInAnalyser();
	_identifyTrajectoryInAnalyser = new IdentifyTrajectoryInAnalyser();
	_trackFaceInAnalyser = new TrackFaceInAnalyser();
	_trackObjectInAnalyser = new TrackObjectInAnalyser();
	_trackHandInAnalyser = new TrackHandInAnalyser();
	_getSoundDirectionInAnalyser = new GetSoundDirectionInAnalyser();
   
	getEyeGazeIn->useCallback(*_getEyeGazeInAnalyser);
	getGripLocationIn->useCallback(*_getGripLocationInAnalyser);
	getHeadGazeIn->useCallback(*_getHeadGazeInAnalyser);
	getObjectTableDistanceIn->useCallback(*_getObjectTableDistanceInAnalyser);
	getObjectsIn->useCallback(*_getObjectsInAnalyser);
	identifyFaceIn->useCallback(*_identifyFaceInAnalyser);
	identifyFaceExpressionIn->useCallback(*_identifyFaceExpressionInAnalyser);
	identifyObjectIn->useCallback(*_identifyObjectInAnalyser);
	identifyTrajectoryIn->useCallback(*_identifyTrajectoryInAnalyser);
	trackFaceIn->useCallback(*_trackFaceInAnalyser);
	trackObjectIn->useCallback(*_trackObjectInAnalyser);
	trackHandIn->useCallback(*_trackHandInAnalyser);
	getSoundDirectionIn->useCallback(*_getSoundDirectionInAnalyser);
}

void Wp3StubThread::setYarpInterface(Wp3StubYarpInterface* yarpInterface){
	_yarpInterface = yarpInterface;
	_getEyeGazeInAnalyser->setWp3Stub(_yarpInterface);
	_getGripLocationInAnalyser->setWp3Stub(_yarpInterface);
	_getHeadGazeInAnalyser->setWp3Stub(_yarpInterface);
	_getObjectTableDistanceInAnalyser->setWp3Stub(_yarpInterface);
	_getObjectsInAnalyser->setWp3Stub(_yarpInterface);
	_identifyFaceInAnalyser->setWp3Stub(_yarpInterface);
	_identifyFaceExpressionInAnalyser->setWp3Stub(_yarpInterface);
	_identifyObjectInAnalyser->setWp3Stub(_yarpInterface);
	_identifyTrajectoryInAnalyser->setWp3Stub(_yarpInterface);
	_trackFaceInAnalyser->setWp3Stub(_yarpInterface);
	_trackObjectInAnalyser->setWp3Stub(_yarpInterface);
	_trackHandInAnalyser->setWp3Stub(_yarpInterface);
	_getSoundDirectionInAnalyser->setWp3Stub(_yarpInterface);
}

bool Wp3StubThread::threadInit(){
	/* initilize the thread */
	return true;
}

void Wp3StubThread::run(){

}

void Wp3StubThread::threadRelease(){
	
	/* release any dynamically created resources */
}

void Wp3StubThread::sendToCheckMutualGaze(VectorOf<int> data){
	VectorOf<int>& container = _checkMutualGazeOut->prepare();
	container = data;
	_checkMutualGazeOut->write();
}

void Wp3StubThread::sendToGetFaces(VectorOf<double> data){
	VectorOf<double>& container = _getFacesOut->prepare();
	container = data;
	_getFacesOut->write();
}

void Wp3StubThread::sendToGetSoundDirection(VectorOf<double> data){
	VectorOf<double>& container = _getSoundDirectionOut->prepare();
	container = data;
	_getSoundDirectionOut->write();
}

void Wp3StubThread::sendToIdentifyFaceExpression(VectorOf<double> data){
	VectorOf<double>& container = _identifyFaceExpressionOut->prepare();
	container = data;
	_identifyFaceExpressionOut->write();
}

void Wp3StubThread::sendToRecognizeSpeech(Bottle data){
	Bottle& container = _recognizeSpeechOut->prepare();
	container = data;
	_recognizeSpeechOut->write();
}

void Wp3StubThread::sendToGetEyeGaze(VectorOf<double> data){
	VectorOf<double>& container = _getEyeGazeOut->prepare();
	container = data;
	_getEyeGazeOut->write();
}

void Wp3StubThread::sendToGetEyes(VectorOf<double> data){
	VectorOf<double>& container = _getEyesOut->prepare();
	container = data;
	_getEyesOut->write();
}

void Wp3StubThread::sendToGetArmAngle(VectorOf<double> data){
	VectorOf<double>& container = _getArmAngleOut->prepare();
	container = data;
	_getArmAngleOut->write();
}

void Wp3StubThread::sendToGetBody(VectorOf<double> data){
	VectorOf<double>& container = _getBodyOut->prepare();
	container = data;
	_getBodyOut->write();
}

void Wp3StubThread::sendToGetGripLocation(VectorOf<double> data){
	VectorOf<double>& container = _getGripLocationOut->prepare();
	container = data;
	_getGripLocationOut->write();
}

void Wp3StubThread::sendToGetBodyPose(VectorOf<double> data){
	VectorOf<double>& container = _getBodyPoseOut->prepare();
	container = data;
	_getBodyPoseOut->write();
}

void Wp3StubThread::sendToGetHands(VectorOf<double> data){
	VectorOf<double>& container = _getHandsOut->prepare();
	container = data;
	_getHandsOut->write();
}

void Wp3StubThread::sendToGetHead(VectorOf<double> data){
	VectorOf<double>& container = _getHeadOut->prepare();
	container = data;
	_getHeadOut->write();
}

void Wp3StubThread::sendToGetHeadGaze(VectorOf<double> data){
	VectorOf<double>& container = _getHeadGazeOut->prepare();
	container = data;
	_getHeadGazeOut->write();
}

void Wp3StubThread::sendToGetObjects(VectorOf<double> data){
	VectorOf<double>& container = _getObjectsOut->prepare();
	container = data;
	_getObjectsOut->write();
}

void Wp3StubThread::sendToGetObjectTableDistance(VectorOf<double> data){
	VectorOf<double>& container = _getObjectTableDistanceOut->prepare();
	container = data;
	_getObjectTableDistanceOut->write();
}

void Wp3StubThread::sendToIdentifyFace(VectorOf<double> data){
	VectorOf<double>& container = _identifyFaceOut->prepare();
	container = data;
	_identifyFaceOut->write();
}

void Wp3StubThread::sendToIdentifyObject(VectorOf<double> data){
	VectorOf<double>& container = _identifyObjectOut->prepare();
	container = data;
	_identifyObjectOut->write();
}

void Wp3StubThread::sendToIdentifyTrajectory(VectorOf<double> data){
	VectorOf<double>& container = _identifyTrajectoryOut->prepare();
	container = data;
	_identifyTrajectoryOut->write();
}

void Wp3StubThread::sendToIdentifyVoice(VectorOf<int> data){
	VectorOf<int>& container = _identifyVoiceOut->prepare();
	container = data;
	_identifyVoiceOut->write();
}

void Wp3StubThread::sendToTrackFace(VectorOf<double> data){
	VectorOf<double>& container = _trackFaceOut->prepare();
	container = data;
	_trackFaceOut->write();
}

void Wp3StubThread::sendToTrackObject(VectorOf<double> data){
	VectorOf<double>& container = _trackObjectOut->prepare();
	container = data;
	_trackObjectOut->write();
}

void Wp3StubThread::sendToGetHeadGazePoint(VectorOf<double> data){
	VectorOf<double>& container = _getHeadGazePointOut->prepare();
	container = data;
	_getHeadGazePointOut->write();
}


void GetEyeGazeInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void GetGripLocationInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void GetHeadGazeInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void GetObjectTableDistanceInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void GetObjectsInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void IdentifyFaceInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void IdentifyFaceExpressionInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void IdentifyObjectInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void IdentifyTrajectoryInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void TrackFaceInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void TrackObjectInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void TrackHandInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}

void GetSoundDirectionInAnalyser::setWp3Stub(Wp3StubYarpInterface* yarpInterface){
   _wp3StubYarpInterface=yarpInterface;
}



void GetEyeGazeInAnalyser::onRead(VectorOf<double>& getEyeGazeInData){
   _wp3StubYarpInterface->getFromGetEyeGaze(getEyeGazeInData);
}

void GetGripLocationInAnalyser::onRead(VectorOf<double>& getGripLocationInData){
   _wp3StubYarpInterface->getFromGetGripLocation(getGripLocationInData);
}

void GetHeadGazeInAnalyser::onRead(VectorOf<double>& getHeadGazeInData){
   _wp3StubYarpInterface->getFromGetHeadGaze(getHeadGazeInData);
}

void GetObjectTableDistanceInAnalyser::onRead(VectorOf<double>& getObjectTableDistanceInData){
   _wp3StubYarpInterface->getFromGetObjectTableDistance(getObjectTableDistanceInData);
}

void GetObjectsInAnalyser::onRead(VectorOf<double>& getObjectsInData){
   _wp3StubYarpInterface->getFromGetObjects(getObjectsInData);
}

void IdentifyFaceInAnalyser::onRead(VectorOf<double>& identifyFaceInData){
   _wp3StubYarpInterface->getFromIdentifyFace(identifyFaceInData);
}

void IdentifyFaceExpressionInAnalyser::onRead(VectorOf<double>& identifyFaceExpressionInData){
   _wp3StubYarpInterface->getFromIdentifyFaceExpression(identifyFaceExpressionInData);
}

void IdentifyObjectInAnalyser::onRead(VectorOf<double>& identifyObjectInData){
   _wp3StubYarpInterface->getFromIdentifyObject(identifyObjectInData);
}

void IdentifyTrajectoryInAnalyser::onRead(VectorOf<int>& identifyTrajectoryInData){
   _wp3StubYarpInterface->getFromIdentifyTrajectory(identifyTrajectoryInData);
}

void TrackFaceInAnalyser::onRead(VectorOf<double>& trackFaceInData){
   _wp3StubYarpInterface->getFromTrackFace(trackFaceInData);
}

void TrackObjectInAnalyser::onRead(VectorOf<double>& trackObjectInData){
   _wp3StubYarpInterface->getFromTrackObject(trackObjectInData);
}

void TrackHandInAnalyser::onRead(VectorOf<double>& trackHandInData){
   _wp3StubYarpInterface->getFromTrackHand(trackHandInData);
}

void GetSoundDirectionInAnalyser::onRead(VectorOf<double>& getSoundDirectionInData){
   _wp3StubYarpInterface->getFromGetSoundDirection(getSoundDirectionInData);
}



