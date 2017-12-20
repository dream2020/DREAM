 /** @file heartbeat.h 
  *
  * Version 1.0
  * 
  * 2016-01-22
  * 
  * \defgroup DREAM_sensoryInterpretation heartbeat
  *
  * The component sends a constant heartbeat on specified output port. Each 
  * heartbeat comprise a VectorOf<long> contaning a single long value 
  * specifying the heartbeat index. 
  * 
  * \section lib_sec Libraries
  *
  * - YARP.
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
  * - \c from \c heartbeat.ini 
  *    - specifies the configuration file
  *
  * - \c context \c heartbeat/configuration
  *   - specifies the sub-childBehaviourClassification from \c $DREAM_ROOT/heartbeat/configuration 
  *   to the configuration file
  *
  * - \c name \c heartbeat   
  *   - specifies the name of the module (used to form the stem of module port names)  
  *
  * <b>Configuration File Parameters </b>
  *
  * The following key-value pairs can be specified as parameters in the configuration file 
  * (they can also be specified as command-line parameters if you so wish). 
  * The value part can be changed to suit your needs; the default values are shown below. 
  * 
  * Key           | Value
  * :--           | :----  
  *
  * heartbeatPort | /heartbeat:o
  * - specifies the name of the output port for the heartbeat. The resulting port name is 
  *   preceded with /<compoentName>
  *
  * period        | 33
  *   - integer value spcifying the heartbeat period (miliseconds)
  *                       
  * \section portsc_sec Ports Created
  *
  *  <b>Input ports</b>
  *
  *  - \c /heartbeat
  *    
  * This port is used to change the parameters of the module at run time or stop the module. \n
  * The following commands are available:
  * 
  *    help
  *    quit
  *
  *
  * <b>Output ports</b>
  *
  *  - \c /heartbeat
  *    see above
  *
  *  - \c /heartbeat/heartbeat:o
  *    BufferedPort<VectorOf<long>>
  *
  * \section conf_file_sec Configuration Files
  *
  * \c heartbeat.ini   
  *
  * \section example_sec Example Instantiation of the Module
  * 
  * <tt>heartbeat --name heartbeat --context components/heartbeat/config --from heartbeat.ini </tt>
  *
  * \author 
  * 
  * <Erik Billing>, <University of Skovde>  
  * 
  * Copyright (C) 2014 DREAM Consortium
  * 
  */
  
/* 
 * Copyright (C) 2016 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Erik Billing, University of Skovde 
 * Email:   erik.billing@his.se 
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
  * 2015-01-22 Version 1.0: First working version committed. (Erik Billing)
  */ 
 
#ifndef HEARTBEAT
#define HEARTBEAT

#include <iostream>
#include <string>
#include <vector>

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
#include <yarp/os/Time.h>

using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;


class HeartbeatThread : public RateThread{
private:
	/* class variables */
	BufferedPort<VectorOf<long>>* _heartbeatOut;
	long _beatIndex;

public:
	/* class methods */
	HeartbeatThread(int period, BufferedPort<VectorOf<long>>* heartbeatOut);
	bool threadInit();
	void threadRelease();
	void run();
};

class Heartbeat : public yarp::os::RFModule{
private:
	/* module parameters */
	string _moduleName;
	string _heartbeatOutName;
	string _handlerPortName;
	int _period;

	/* class variables */
	yarp::os::BufferedPort<VectorOf<long>> _heartbeatOut;
	yarp::os::Port _handlerPort;

	/* pointer to a new thread */
	HeartbeatThread* _heartbeatThread;
public:
	/* class methods */
	bool configure(yarp::os::ResourceFinder &rf);
	bool interruptModule();
	bool close();
	bool respond(const yarp::os::Bottle& command, Bottle& reply);
	double getPeriod();
	bool updateModule();
};

#endif
