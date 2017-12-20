/* 
 * Copyright (C) 2014 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Serge Thill, University of Skövde 
 * Email:   serge.thill@his.se 
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
  * 08/11/2016 Version 1.0: ... (Serge Thill)
  */ 

#include <yarp/os/Time.h>
#include "assessChildEngagementController.h"
#include "assessChildEngagementYarpInterface.h"

using namespace std;
using namespace yarp::os;

AssessChildEngagementController::AssessChildEngagementController(AssessChildEngagementYarpInterface *yarpInterface){
	_yarpInterface = yarpInterface;

	_smile = -1;
  	_gaze = -1;
  	_body = -1;

}


void AssessChildEngagementController::gotSmile(int data) {


	if (_smile == -1) {

		evalSmile(data);


		if (_body != -1 && _gaze != -1) {
			_yarpInterface->sendToEngagementFlag(getEngagement());
			reset_state();
		}
	}
}

void AssessChildEngagementController::gotBody(double data) {


	if (_body == -1) {

		evalBody(data);

		if (_smile != -1 && _gaze != -1) {
			_yarpInterface->sendToEngagementFlag(getEngagement());
			reset_state();
		}
	}
}

void AssessChildEngagementController::gotGaze(int data) {

	if (_gaze == -1) {
		evalGaze(data);

		if (_body != -1 && _smile != -1) {
			_yarpInterface->sendToEngagementFlag(getEngagement());
			reset_state();
		}
	}

}


/* *** Helper functions *** */

void AssessChildEngagementController::reset_state() {

	_smile = -1;
  	_gaze = -1;
  	_body = -1;

}

double AssessChildEngagementController::getEngagement() {


	//Below is the code for a truth table as per specification in the 
	//Developer meeting notes. However, the current requirements
	//from Cristina are different, and implemented below that
	/*
	double engagement[] = {0, 1, 2, 3, 4, 5, 6, 7};
	int idx = (_smile << 2) + (_gaze << 1) + _body;

	if (idx >= 0 && idx < 8) return engagement[idx];
	else                     return -1; //this should never happen
	*/

	return _smile + _gaze + _body;

}

//check if there is mutual gaze - this is the easy one since the answer is already given
void AssessChildEngagementController::evalGaze(int data) {

	_gaze = data;

	//apparently we can read -1 in case of error, so don't count mutual gaze in engagement unless
	//it is explicitly reported as present
	if (_gaze != 1) _gaze = 0;
}

//check if the child is in front of the body - ideally the relevant coordinate should be near zero
void AssessChildEngagementController::evalBody(double data) {


	if (data >= centre_pos-body_thr && data <= centre_pos + body_thr) { 
		_body = 1;
	} else {
		_body = 0;
	}

}

//check if there was a smile - this is also given directly in the data
void AssessChildEngagementController::evalSmile(int data) {

	if (data == 0) { //we don't know what value to check for yet - it might not be 1
		_smile = 1;

	} else {
		_smile = 0;
	}

}