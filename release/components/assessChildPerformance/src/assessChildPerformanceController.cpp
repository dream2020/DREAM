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
#include "assessChildPerformanceController.h"
#include "assessChildPerformanceYarpInterface.h"
#include <fstream>
#include <numeric>
#include "performanceMessages.h"

using namespace std;
using namespace yarp::os;

AssessChildPerformanceController::AssessChildPerformanceController(AssessChildPerformanceYarpInterface *yarpInterface){
	_yarpInterface = yarpInterface;

	_printlogs = 0;

	//various thresholds for detecting different trajectories.
	//traj_thrs[i] = 0;


	vector<double> filling;
	_eyeGaze.push_back(filling);
	_eyeGaze.push_back(filling);
	_eyeGaze.push_back(filling);
 	reset_state();

}

/* *** Reception of possibly relevant events *** */

void AssessChildPerformanceController::gotVoice(int voice_id) {

	//we were looking for a voice event and we found one
	if (check_no_sandtray() == 0 && _expected_voice_event != _PERF_NO_CHECK_) {

		if (voice_id > 0) _performance = _PERF_GOOD_;

		_yarpInterface->sendToGetChildPerformance(_performance);
		reset_state();
	}
}

void AssessChildPerformanceController::gotGaze(double xData, double yData, double zData) {

	if(_eyeGaze[0].size()>9){
		_eyeGaze[0].erase(_eyeGaze[0].begin());
		_eyeGaze[1].erase(_eyeGaze[1].begin());
		_eyeGaze[2].erase(_eyeGaze[2].begin());
	}
	_eyeGaze[0].push_back(xData);
	_eyeGaze[1].push_back(yData);
	_eyeGaze[2].push_back(zData);

	//we were looking for a gaze event and we found one
	if (check_time() == _PERF_ON_TIME_ && check_no_sandtray() == 0 && _expected_gaze_event != _PERF_NO_CHECK_) {
		// check that we have a head position
		if (_headPosition[0] != _PERF_NO_HEAD_) {
			// use head to calculate vector that would intersect at the sandtray plane
			double coef =  (_headPosition[1] - sandtray_offset)/mean(_eyeGaze[1]);
			double targetX = _headPosition[0]-coef*mean(_eyeGaze[0]);
			double targetZ = _headPosition[2]-coef*mean(_eyeGaze[2]);

			//cout << "target X: " << targetX << " target Z: " << targetZ <<  "coef : " << coef <<endl;

			if (_expected_gaze_event == _PERF_CHECK_LOOK_LEFT_ ||
				_expected_gaze_event == _PERF_CHECK_LOOK_RIGHT_) {
				bool bLookLeft = false;
				bool bLookRight = false;

				// if looking for a gaze, see if we are a zone
				if (targetZ > _PERF_SAND_Z1_ && targetZ < _PERF_SAND_Z2_) {
					if (targetX > _PERF_SAND_X1_ && targetX < _PERF_SAND_X2_) {
						bLookRight = true;
					}
					else if (targetX < (-_PERF_SAND_X1_) && targetX > (-_PERF_SAND_X2_)) {
						bLookLeft = true;
					}

					if (bLookLeft || bLookRight) {
						_performance = _PERF_BAD_;
						
						if ((_expected_gaze_event == _PERF_CHECK_LOOK_LEFT_) && bLookLeft) {
							_performance = _PERF_GOOD_;
						}
						else if ((_expected_gaze_event == _PERF_CHECK_LOOK_RIGHT_) && bLookRight) {
							_performance = _PERF_GOOD_;
						}

						//cout << "Performance: " << _performance << endl;
						_yarpInterface->sendToGetChildPerformance(_performance);
						reset_state();
					}
				}
			}
		}
	}
}

void AssessChildPerformanceController::gotPose(double pose[]) {

	//double no_movement_thr = 10; //would need to be tuned based on real data - move to .ini if we want to use this in the future

	//[3,4,5] gives z,y,z of head - update for use elsewhere
	_headPosition[0] = pose[3];
	_headPosition[1] = pose[4];
	_headPosition[2] = pose[5];

	//we were looking for a pose event and we found one
	if (check_no_sandtray() == 0 && _expected_pose_event != _PERF_NO_CHECK_) {

		//performance evaluations depend on movement over time, so we can only 
		//send a performance evaluation after the time has elapsed.
		if (check_time() == _PERF_ON_TIME_) {

			//first time we got a pose -record it.
			if (empty_pose_check() == 1) {

				copy_pose(pose);

			} else {
				switch (_expected_pose_event) {

					case _PERF_CHECK_POINT_LEFT_: //point left 
					//current strategy is to do nothing here but to compare
					//body pose at the beginning and the end of the time window
					//to check for an appropriate pointing.
					//this may need to be revised.
					break;

					case _PERF_CHECK_POINT_RIGHT_: //point right 
					//as for point left.
					break;

					case _PERF_CHECK_NO_MOVEMENT_: //no movement.

						//check for movement and if there is, note bad performance.
						//if (pose_rmse(pose) > no_movement_thr) _performance = 0;
						
						if (no_movement(pose) == 0) {
							_performance = _PERF_BAD_;
							_yarpInterface->sendToGetChildPerformance(_performance);
							reset_state();
						} else {
							_performance = _PERF_GOOD_;
						}

					break;

					//something unexpected; do nothing
					default:

					break;
				}

			}

		//the time window has ended - let's evaluate the performance
		} else {

			//evaluate pointing
			if ( _expected_pose_event == _PERF_CHECK_POINT_LEFT_ || _expected_pose_event == _PERF_CHECK_POINT_RIGHT_) {
				_performance = check_pointing(pose);
			}

			//evaluate no movement - already done: if it was never set to 0

			_yarpInterface->sendToGetChildPerformance(_performance);
			reset_state();
		}

	}
	
}

/*
void AssessChildPerformanceController::gotTrajectory(double traj[]) {

	double result;

	//we were looking for a trajectory, and it turned up on time
	if (_expected_sa_event != -1 && check_time() == 1) {

		result = traj[_expected_sa_event]; //the likelihood of the trajectory we want

		for (int i=0; i < 10; i++) {
			if (i == _expected_sa_event -1) {
				continue;
			}

			//some non-desired movement has higher probability than the desired one
			//we interpret this as bad performance
			if (traj[i] > result) {
				_performance = 0;
				break;
			}
		}

		//did not see a reason to declare bad performance, so our performance is what "result" says
		if (_performance == -1) {
			_performance = result;				
		}

		_yarpInterface->sendToGetChildPerformance(_performance);
		reset_state();
	}
	
}*/

/*
void AssessChildPerformanceController::gotTrajectory(double traj[]) {
	
	//we were looking for a trajectory, and it turned up on time
	if (check_time() == _PERF_ON_TIME_ && _expected_sa_event != _PERF_NO_CHECK_) {

		if (traj[_expected_sa_event] == 1) {
			_performance = _PERF_GOOD_;

			_yarpInterface->sendToGetChildPerformance(_performance);
			reset_state();
		}
	}
	
}*/

void AssessChildPerformanceController::gotTrajectory(double traj[]) {
	
	//we were looking for a trajectory, and it turned up on time
	if (check_time() == _PERF_ON_TIME_ && _expected_sa_event != _PERF_NO_CHECK_) {

		//First event, so set the counter
		if (_traj_count_start == -1) _traj_count_start = Time::now() + traj_min_wait/1000;

		//just keep counting. the events as they come in
		for (int i = 1; i < 12; i++) {
			_traj_vector_count[i] += traj[i];
			_sum_traj_events += traj[i];
		}

		//we've collected enough data, let's start looking for good behaviour
		if (Time::now() >= _traj_count_start) {


			//for (int i = 1; i < 12; i++) cout << "****Behav " << i << ": " << _traj_vector_count[i] << endl;
			//cout << "****expected behav count: " << _traj_vector_count[_expected_sa_event] << endl << " total count: " <<_sum_traj_events << endl;

			//The behaviour we were looking for exceeds the threshold - good performance - not a good way according to Emmanuel
			/*
			if (_traj_vector_count[_expected_sa_event] >= traj_thr*_traj_multipliers[_expected_sa_event]/_sum_traj_events) {
				
				_performance = _PERF_GOOD_;
				_yarpInterface->sendToGetChildPerformance(_performance);
				reset_state();
			}*/

			//cout << "****expected behav count: " << _traj_vector_count[_expected_sa_event]  << " thr: " << traj_thrs[_expected_sa_event]*_traj_multipliers[_expected_sa_event] << endl;

			//logevents("****expected behav count: " + to_string(_traj_vector_count[_expected_sa_event]) + " thr: " + to_string(_traj_thrs[_expected_sa_event]));

			//this test works with a simple count - should be more reliable than a percentage threshold
			if (_traj_vector_count[_expected_sa_event] >= _traj_thrs[_expected_sa_event]) {
				
				_performance = _PERF_GOOD_;
				_yarpInterface->sendToGetChildPerformance(_performance);
				reset_state();
			}
		}
	}
}





void AssessChildPerformanceController::gotSandtray(int sandtray_event) {

	/* reminder of what sandtray_event can actually be:
        0 	good sandtray move
		1 	bad sandtray move
		2 	touch on sandtray image
		3 	touch off sandtray image
		4 	child touch robot-owned sandtray image

	*/

	//we were looking for a sandtray event, and it turned up on time
	if (check_time() == _PERF_ON_TIME_ && _expected_sandtray_event != _PERF_NO_CHECK_) {

		//logevents("Got sandtray event " + to_string(sandtray_event) + "; expected interaction is " + to_string(_expected_sandtray_event));

		//_expected_sandtray_event and sandtray_event follow different specs, so 
		//we cannot just compare the values.
		switch(_expected_sandtray_event) {

			case _PERF_SANDTRAY_GOOD_: //child perform good sandtray move
				if (sandtray_event == _PERF_SANDTRAY_CHECK_GOOD_) {
					_performance = _PERF_GOOD_;
					_yarpInterface->sendToGetChildPerformance(_performance);
				} else if (sandtray_event == _PERF_SANDTRAY_CHECK_BAD_) {
					_performance = _PERF_BAD_;
					_yarpInterface->sendToGetChildPerformance(_performance);
				}
				break;

			case _PERF_SANDTRAY_TOUCH_: //child touch sandtray image
				if (sandtray_event == _PERF_SANDTRAY_TOUCH_) {
					_performance = _PERF_GOOD_;
					_yarpInterface->sendToGetChildPerformance(_performance);
				}
				break;

			case _PERF_SANDTRAY_NO_TOUCH_: //child does not touch sandtray
				//any event would result from a touch, so this is bad performance.
				_performance = _PERF_BAD_;
				break;

			case _PERF_TOUCH_ROBOT_OWNED_: //child touch robot-owned sandtray image 
				if (sandtray_event == _PERF_SANDTRAY_CHECK_TOUCH_OWNED_) {
					_performance = _PERF_GOOD_;
					_yarpInterface->sendToGetChildPerformance(_performance);
				}
				break;
		}

	}
	if (_performance != _PERF_NONE_) {
		_yarpInterface->sendToGetChildPerformance(_performance);
		reset_state();
	}

}


/* *** Definition of what event to find *** */


void AssessChildPerformanceController::checkSA(int behaviour, int expected_rt) {

	reset_state();

	if (behaviour == 9) behaviour = 0;
	else behaviour -= 10; //the 11 behaviours of interest are indexed 1-11 in WP4/5 but 11-21 in WP6's request

 	_expected_sa_event = behaviour;
 	_rt = Time::now() + double(expected_rt)/1000;

 	_yarpInterface->sendToIdentifyTrajectory(behaviour, expected_rt);

 	//_rt += 1; // add some buffer to give SA time to respond. The alternative to this is to shorten the sent RT.

}

void AssessChildPerformanceController::checkSandtray(int interaction, int expected_rt) {

	reset_state();

 	_expected_sandtray_event = interaction;
 	_rt = Time::now() + double(expected_rt)/1000;

 	cout << "Expecting SA event " << _expected_sandtray_event << " within " << _rt << "s\n";

}

void AssessChildPerformanceController::checkOther(int behaviour, int expected_rt) {

	reset_state();

	if (behaviour == _PERF_LOOK_RIGHT_) {
		_expected_gaze_event = _PERF_CHECK_LOOK_RIGHT_;
	}
	else if (behaviour == _PERF_LOOK_LEFT_) {
		_expected_gaze_event = _PERF_CHECK_LOOK_LEFT_;
	}
	else if (behaviour == _PERF_POINT_LEFT_) {
		_expected_pose_event = _PERF_CHECK_POINT_LEFT_;
	}
	else if (behaviour == _PERF_POINT_LEFT_) {
		_expected_pose_event = _PERF_CHECK_POINT_RIGHT_;
	}
	else if (behaviour == _PERF_NO_MOVEMENT_) {
		_expected_pose_event = _PERF_CHECK_NO_MOVEMENT_;
	}
	else if (behaviour == _PERF_SPEAK_) {
		_expected_pose_event = _PERF_CHECK_CHILD_SPEAKS_;
	}

 	_rt = Time::now() + double(expected_rt)/1000;

}

/* *** Helper functions *** */

//called when a behaviour we might be looking for occurs - we need to check if it is still within time.
int AssessChildPerformanceController::check_time() {

	//logevents("Entered time check");

	//not looking for any behaviour, so not relevant
	if (_expected_sandtray_event == _PERF_NO_CHECK_ && _expected_sa_event   == _PERF_NO_CHECK_ &&
 	  	_expected_pose_event     == _PERF_NO_CHECK_ && _expected_gaze_event == _PERF_NO_CHECK_ &&
 	    _expected_voice_event    == _PERF_NO_CHECK_) {

		//logevents("No behaviour expected");

		return _PERF_OUT_OF_TIME_;
	}

	//no more time to respond - we may have to return bad performance
	if (Time::now() > _rt) {

		//logevents("Some behaviour expected and out of time");

		//the assessment of some behaviours actually depends on evaluation for the entire time window.
		//For those, we cannot report bad performance just because the time is up.  For behaviours that
		//should have been evaluated by now, however, we report bad performance and reset the component
		if (_expected_sandtray_event != _PERF_SANDTRAY_NO_TOUCH_ && _expected_pose_event == _PERF_NO_CHECK_) {
			_performance = _PERF_BAD_;
			_yarpInterface->sendToGetChildPerformance(_performance);
			reset_state();
		} 
		return _PERF_OUT_OF_TIME_;

	} else {
		//logevents("Some behaviour expected and within time");
		return _PERF_ON_TIME_;
	}
}

//for behaviours where body pose matters: check if we do not yet have an initial pose recorded
int AssessChildPerformanceController::empty_pose_check() {

	for (int i = 0; i < 30; i++) {

		if (_posetrack[i] != -1) {
			return 0;
		}
	}

	return 1;

}

//copy the current version of the body pose so we can compare against it in the future
void AssessChildPerformanceController::copy_pose(double pose[]) {

	for (int i = 0; i < 30; i++) {
		_posetrack[i] = pose[i];
	}
}

//rmse for body pose only, so this assumes a 30-element array
double AssessChildPerformanceController::pose_rmse(double pose[]) {

	double e;
	double se = 0;

	for (int i = 0; i < 30; i++) {
		e = _posetrack[i] - pose[i];
		se += (e*e);
	}

	return sqrt(se/30);
}

//NEW definition of no movement - we only care about whether or not the child crosses over the sandtray
int AssessChildPerformanceController::no_movement(double pose[]) {

//	double thr = 100; // the z coordinate of the relevant edge of the sandtray 

	//logevents("Sandtray edge is set to " + to_string(sandtray_edge));

	int y = 1;

	for (int i = 2; i < 30; i+=3) {

		if (pose[i] < sandtray_edge) {
			//logevents("pose " + to_string(i) + " is " + to_string(pose[i]));
			y = 0;

		}
	}

	return y;
}

double AssessChildPerformanceController::check_pointing(double pose []) {

	double wrist_x_loc;		//current location of the pointing wrist - should be in the expected space along x
	double wrist_z_change;	//did the wrist move forward?

	double y = _PERF_BAD_;	//assume bad performance by default

	//figure out the change along the z axis. pointing should make this negative
	double left_wrist_z_change = pose[14] - _posetrack[14];
	double right_wrist_z_change = pose[26] - _posetrack[26];

	//left hand moved more, assume the child is pointing with the left
	if (left_wrist_z_change < right_wrist_z_change) {
		wrist_z_change = left_wrist_z_change;
		wrist_x_loc = pose[12];
	} else {
		wrist_z_change = right_wrist_z_change;
		wrist_x_loc = pose[24];
	}

	switch(_expected_pose_event) {

		case _PERF_CHECK_POINT_LEFT_: //point left expected 
			if (wrist_z_change < 0 && flip_lr*wrist_x_loc < 0) {
				y = _PERF_GOOD_;
			}

		break;

		case _PERF_CHECK_POINT_RIGHT_: //point right
			if (wrist_z_change < 0 && flip_lr*wrist_x_loc > 0) {
				y = _PERF_GOOD_;
			}

		break;

		default: //absolutely should never happen - we're only supposed to call this function from an appropriate if clause
		y = _PERF_NONE_;
	}

	return y;
}

//reset whatever it is we were looking for if a new request arrives
void AssessChildPerformanceController::reset_state() {
 	_expected_sandtray_event = _PERF_NO_CHECK_;
 	_expected_sa_event = _PERF_NO_CHECK_;
 	_expected_pose_event = _PERF_NO_CHECK_;
 	_expected_gaze_event = _PERF_NO_CHECK_;
 	_expected_voice_event = _PERF_NO_CHECK_;

	_headPosition[0] = _PERF_NO_HEAD_;
	_headPosition[1] = _PERF_NO_HEAD_;
	_headPosition[2] = _PERF_NO_HEAD_;

 	_performance = _PERF_NONE_;

 	_rt = -1; //time (s) at which we expect a response at the latest 
 	_traj_count_start = -1;
 	_sum_traj_events = 0;

 	for (int i = 0; i < 30; i++) _posetrack[i] = -1;
	for (int i = 0; i < 12; i++) _traj_vector_count[i] = 0;

	_traj_thrs[0] = 0; 
	_traj_thrs[1] = wave_thr;
  	_traj_thrs[2] = cover_thr;
   	_traj_thrs[3] = hands_head_thr;
  	_traj_thrs[4] = fly_thr;
	_traj_thrs[5] = drive_thr;
  	_traj_thrs[6] = smell_thr;
   	_traj_thrs[7] = traj1_thr;
  	_traj_thrs[8] = traj2_thr;
  	_traj_thrs[9] = traj3_thr;
   	_traj_thrs[10] = traj4_thr;
   	_traj_thrs[11] = traj5_thr;

}

//we need to detect "no" sandtray move, which is hard since the sandtray doesn't send events then
//so this function can be used whenever some other event has been received to check this.

int AssessChildPerformanceController::check_no_sandtray() {

	//looking for no touch, and the time's up
	if (_expected_sandtray_event == _PERF_SANDTRAY_NO_TOUCH_ && check_time() == _PERF_OUT_OF_TIME_) {
		_performance = _PERF_GOOD_;

		_yarpInterface->sendToGetChildPerformance(_performance);
		reset_state();

		return 1;
	} else {
		return 0;
	}

}

void AssessChildPerformanceController::logevents(std::string msg) {

	if (_printlogs == 1) {
		ofstream myfile;
   		myfile.open ("component_log.txt");
  	 	myfile << msg << "\n";
   		myfile.close();
   	}

}

double AssessChildPerformanceController::mean(vector<double> vec){
	return accumulate( vec.begin(), vec.end(), 0.0)/vec.size();
}