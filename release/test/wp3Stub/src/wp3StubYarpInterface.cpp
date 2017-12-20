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

#include "wp3StubYarpInterface.h"
#include "wp3Stub.h"
#include "wp3StubController.h"

Wp3StubYarpInterface::Wp3StubYarpInterface(){

}

void Wp3StubYarpInterface::init(Wp3StubThread* yarpThread, Wp3StubController* controller){
   _controller = controller;
   _yarpThread = yarpThread;   
}

void Wp3StubYarpInterface::getFromGetEyeGaze(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromGetGripLocation(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromGetHeadGaze(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromGetObjectTableDistance(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromGetObjects(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromIdentifyFace(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromIdentifyFaceExpression(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromIdentifyObject(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromIdentifyTrajectory(VectorOf<int> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromTrackFace(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromTrackObject(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromTrackHand(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}

void Wp3StubYarpInterface::getFromGetSoundDirection(VectorOf<double> data){
	/* Do Stuff: call methods from controller */
	return;
}


//Probably need to change type to allow to be called by non yarp class
void Wp3StubYarpInterface::sendToCheckMutualGaze(bool mutualGaze) {
	VectorOf<int> data;
	data.push_back(mutualGaze);
	_yarpThread->sendToCheckMutualGaze(data);
	return;
}

void Wp3StubYarpInterface::sendToGetFaces(VectorOf<double> data){ 
	_yarpThread->sendToGetFaces(data);
	return;
}

void Wp3StubYarpInterface::sendToGetSoundDirection(VectorOf<double> data){ 
	_yarpThread->sendToGetSoundDirection(data);
	return;
}

void Wp3StubYarpInterface::sendToIdentifyFaceExpression(VectorOf<double> data){ 
	_yarpThread->sendToIdentifyFaceExpression(data);
	return;
}

void Wp3StubYarpInterface::sendToRecognizeSpeech(Bottle data){ 
	_yarpThread->sendToRecognizeSpeech(data);
	return;
}

void Wp3StubYarpInterface::sendToGetEyeGaze(){

    double dblRand = double(randi(0, 2));
    VectorOf<double> dblToSend;
    dblToSend.push_back(dblRand);

	_yarpThread->sendToGetEyeGaze(dblToSend);
	return;
}

int Wp3StubYarpInterface::randi(int lo, int hi)
{
    int n = hi - lo + 1;
    int i = rand() % n;
    if (i < 0) i = -i;
    return lo + i;
}

void Wp3StubYarpInterface::sendToGetEyes(VectorOf<double> data){ 
	_yarpThread->sendToGetEyes(data);
	return;
}

void Wp3StubYarpInterface::sendToGetArmAngle(VectorOf<double> data){ 
	_yarpThread->sendToGetArmAngle(data);
	return;
}

void Wp3StubYarpInterface::sendToGetBody(VectorOf<double> data){ 
	_yarpThread->sendToGetBody(data);
	return;
}

void Wp3StubYarpInterface::sendToGetGripLocation(VectorOf<double> data){ 
	_yarpThread->sendToGetGripLocation(data);
	return;
}

void Wp3StubYarpInterface::sendToGetBodyPose(VectorOf<double> data){ 
	_yarpThread->sendToGetBodyPose(data);
	return;
}

void Wp3StubYarpInterface::sendToGetHands(VectorOf<double> data){ 
	_yarpThread->sendToGetHands(data);
	return;
}

void Wp3StubYarpInterface::sendToGetHead(VectorOf<double> data){ 
	_yarpThread->sendToGetHead(data);
	return;
}

void Wp3StubYarpInterface::sendToGetHeadGaze(VectorOf<double> data){ 
	_yarpThread->sendToGetHeadGaze(data);
	return;
}

void Wp3StubYarpInterface::sendToGetObjects(VectorOf<double> data){ 
	_yarpThread->sendToGetObjects(data);
	return;
}

void Wp3StubYarpInterface::sendToGetObjectTableDistance(VectorOf<double> data){ 
	_yarpThread->sendToGetObjectTableDistance(data);
	return;
}

void Wp3StubYarpInterface::sendToIdentifyFace(VectorOf<double> data){ 
	_yarpThread->sendToIdentifyFace(data);
	return;
}

void Wp3StubYarpInterface::sendToIdentifyObject(VectorOf<double> data){ 
	_yarpThread->sendToIdentifyObject(data);
	return;
}

void Wp3StubYarpInterface::sendToIdentifyTrajectory(VectorOf<double> data){ 
	_yarpThread->sendToIdentifyTrajectory(data);
	return;
}

void Wp3StubYarpInterface::sendToIdentifyVoice(VectorOf<int> data){ 
	_yarpThread->sendToIdentifyVoice(data);
	return;
}

void Wp3StubYarpInterface::sendToTrackFace(VectorOf<double> data){ 
	_yarpThread->sendToTrackFace(data);
	return;
}

void Wp3StubYarpInterface::sendToTrackObject(VectorOf<double> data){ 
	_yarpThread->sendToTrackObject(data);
	return;
}

void Wp3StubYarpInterface::sendToGetHeadGazePoint(VectorOf<double> data){ 
	_yarpThread->sendToGetHeadGazePoint(data);
	return;
}


