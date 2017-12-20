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

#include "wp3StubController.h"
#include "wp3StubYarpInterface.h"

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <stdlib.h>

using namespace std;
using namespace boost::asio;
using namespace std;

using namespace std;

Wp3StubController::Wp3StubController(Wp3StubYarpInterface *yarpInterface){
	_yarpInterface = yarpInterface;
	_io = new io_service();
	_t = new deadline_timer(*_io);
	_Itime = 33;
	wait();
	_thr = new boost::thread(boost::bind(&io_service::run, _io));
    //boost::thread t(&Wp3StubController::spamEyeGaze, this);
    
}

void Wp3StubController::spamEyeGaze() {
    while (1) {
        _yarpInterface->sendToGetEyeGaze();
        Sleep(33);
    }
}


void Wp3StubController::sendMutualGaze(const boost::system::error_code &e) {
	if (e)
		return;
	_yarpInterface->sendToCheckMutualGaze(rand() % 100 <5);
	_yarpInterface->sendToGetEyeGaze();
	wait();
}
void Wp3StubController::sendFaces(const boost::system::error_code &e) {
	if (e)
		return;
    //printf("\n\n Calling wp3stubController sendFaces \n\n");
    VectorOf<double> getFacesVector;

    int faces = (int)(rand()%3)+1;
            
    getFacesVector.resize(3*faces);
            
    for(int i = 0; i <faces;i++){
        getFacesVector(0+(i*3)) = (double)(rand() %20)+0.5;
        getFacesVector(1+(i*3)) = (double)(rand() %10)+0.5;
        getFacesVector(2+(i*3)) = (double)(rand() %3)+0.5; 
    }
	_yarpInterface->sendToGetFaces(getFacesVector);

	//wait();
}
void Wp3StubController::sendSoundDirection(const boost::system::error_code &e) {
	if (e)
		return;
    //printf("\n\n Calling wp3stubController sendSoundDirection \n\n");
    VectorOf<double> soundVector;
    soundVector.resize(2);
    soundVector(0) = -119 + (double)(rand() %230); 
    soundVector(1) = (double)(rand()%100)/100; //No vertical, this is confidence value
	_yarpInterface->sendToGetSoundDirection(soundVector);

	//wait();
}
void Wp3StubController::sendIdentifyExpression(const boost::system::error_code &e) {
	if (e)
		return;
    //printf("\n Calling wp3stubController sendIdentifyExpression \n");
    VectorOf<double> identifyExpression;
    identifyExpression.resize(1);
    identifyExpression(0) = (rand() % 4)+1; 
    _yarpInterface->sendToIdentifyFaceExpression(identifyExpression);
	//wait();
}


void Wp3StubController::wait() {
	_t->expires_from_now(boost::posix_time::milliseconds(_Itime)); //repeat rate here
	_t->async_wait(boost::bind(&Wp3StubController::sendMutualGaze, this, boost::asio::placeholders::error));
    if(rand() % 250 <15){
        _t->async_wait(boost::bind(&Wp3StubController::sendSoundDirection, this, boost::asio::placeholders::error));
    }
    if(rand() % 200 <5){
        _t->async_wait(boost::bind(&Wp3StubController::sendFaces, this, boost::asio::placeholders::error));
        if(rand() % 100 <20){
            _t->async_wait(boost::bind(&Wp3StubController::sendIdentifyExpression, this, boost::asio::placeholders::error));
        }
    }
    
    
}